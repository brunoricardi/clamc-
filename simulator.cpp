/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 04:34:57 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: simulator.cpp
 * @Last modified by:   bruno
 * @Last modified time: Mon 14 Jan, 2019 | 03:21:16 -02:00
 * @License: This program and the accompanying materials are made available under explicit agreement
      between Bruno R. de Abreu and the end user. You may not redistribute the code and
      accompanying material to anyone.

      On the event that the software is used to generate data that is used implicitly or explicitly
      for research purposes, proper acknowledgment must provided  in the citations section of
      publications.

      This software cannot be used for commercial purposes in any way whatsoever.
 * @Copyright: (c) 2018 Bruno R. de Abreu
                All rights reserved.
 */
#include "simulator.h"

simulator::simulator()
{
}

simulator::~simulator()
{
}

void simulator::initialize_flags()
{
    this->flags.eqb = false;
    this->flags.warmup = false;
    this->flags.data = false;
    this->flags.initial_config = false;
    this->flags.chkpt_interval = 1.0;
    this->data_flags.gr = false;
    this->data_flags.boo = false;
    this->data_flags.kd = false;
    this->outfile.open("output.log", std::ios::out | std::ios::app);
    this->outfile << std::scientific;
    this->outfile << std::setprecision(6);
}

void simulator::read_input()
{
    this->initialize_flags();
    std::string filename, line;
    filename = "simulation.inp";
    std::ifstream file;
    file.open(filename.c_str(), std::ifstream::in);
    int nlines=0;
    while( getline(file,line) )
        nlines++;
    file.clear();
    file.seekg(0,std::ios::beg);
    int i;
    std::string command;
    int dim, nparts;
    double density;
    double ls1, ls2, es1, es2;
    double double_temp;
    std::string string_temp;
    int ndata, j;
    for(i=0; i<nlines; i++)
    {
        file >> command;
        if(command == "BOXDIM")
        {
            file >> dim;
        }
        else if(command == "NPARTS")
        {
            file >> nparts;
        }
        else if(command == "DENSIT")
        {
            file >> density;
        }
        else if(command == "SCALES")
        {
            file >> ls1;
            file >> ls2;
            file >> es1;
            file >> es2;
        }
        else if(command == "DISPLA")
        {
            file >> double_temp;
            this->mc_engine.set_displacement(double_temp);
        }
        else if(command == "CONFIG")
        {
            this->flags.initial_config = true;
            file >> string_temp;
            this->configuration = this->fm.read_file(string_temp);
        }
        else if(command == "CHKPNT")
        {
            file >> this->flags.chkpt_interval;
        }
        else if(command == "EQUILI")
        {
            this->flags.eqb = true;
            file >> this->eqb_params.nsweeps;
            file >> this->eqb_params.beta;
            file >> this->eqb_params.beta_inc;
            file >> this->eqb_params.ntemps;
        }
        else if(command == "WARMUP")
        {
            this->flags.warmup = true;
            file >> this->warmup_params.nsweeps;
            file >> this->warmup_params.beta;
        }
        else if(command == "GETDAT")
        {
            this->flags.data = true;
            file >> this->data_params.nsweeps;
            file >> this->data_params.nblocks;
            file >> this->data_params.beta;
            file >> ndata;
            for(j=0; j<ndata; j++)
            {
                file >> string_temp;
                if(string_temp == "gr")
                {
                    this->data_flags.gr = true;
                }
                if(string_temp == "boo")
                {
                    this->data_flags.boo = true;
                    file >> this->data_flags.boo_mode;
                }
                if(string_temp == "kd")
                {
                    this->data_flags.kd = true;
                }
            }
        }
    }
    this->cell.set_parameters(dim, nparts, density);
    this->cell.set_scales(ls1, ls2, es1, es2);
    this->M.set_metric(this->cell.length);
    this->mc_engine.set_parameters(this->cell);
    this->measure.set_parameters(this->cell);
    file.close();
}



void simulator::initialize_randomly()
{
    std::random_device rdev{};
    std::mt19937_64 rng{rdev()};
    std::uniform_real_distribution<long double> unif;
    double max;
    max = this->cell.length / 2.0;
    int np = this->cell.nparts;
    int d = this->cell.dim;
    double hc=0.0;
    if(this->cell.scale.energy1 >= pow(10.0,6.0))
        hc = this->cell.scale.length1;
    else if(this->cell.scale.energy2 >= pow(10.0,6.0))
        hc = this->cell.scale.length2;
    int dir;
    vec2d config;
    vec1d particle;
    double random;
    for(dir=0; dir < d; dir++)
    {
        random = unif(rng);
        particle.push_back( max*(2.0*random - 1.0) );
    }
    config.push_back(particle);
    particle.clear();
    while(config.size() < np)
    {
        for(dir=0; dir < d; dir++)
        {
            random = unif(rng);
            particle.push_back( max*(2.0*random - 1.0) );
        }
        if(this->M.check_encroachment(&particle, &config, hc) == false)
        {
            config.push_back(particle);
        }
        particle.clear();
    }
    this->configuration = config;
}



void simulator::equilibrate()
{
    std::pair< bool, double > mc_out;
    int ckp_size;
    ckp_size = int(this->flags.chkpt_interval * this->eqb_params.nsweeps);
    int n_ckp;
    n_ckp = int(this->eqb_params.nsweeps / ckp_size);
    int t, n, m, p;
    bool acc;
    int sum;
    double initial_beta, beta;
    initial_beta = this->eqb_params.beta - this->eqb_params.ntemps*this->eqb_params.beta_inc;
    if(initial_beta < 0.0)
    {
        this->outfile << "Initial inverse temperature is negative, check EQUILI parameters!" << std::endl;
        return;
    }
    double energy;
    energy = this->measure.energy(&this->configuration);
    estimator energy_est;
    std::string filename;
    for(t=0; t <= this->eqb_params.ntemps; t++)
    {
        beta = initial_beta + t*this->eqb_params.beta_inc;
        filename = "energy_eqb" + std::to_string(t) + ".dat";
        sum = 0;
        for(n = 0; n < n_ckp; n++)
        {
            energy_est.reset_accumulated_scalar();
            for(m=0; m < ckp_size; m++)
            {
                for(p=0; p < this->cell.nparts; p++)
                {
                    mc_out = this->mc_engine.single_particle_sweep(&this->configuration, beta);
                    sum += mc_out.first;
                    energy += mc_out.second;
                }
                energy_est.accumulate_scalar(energy);
            }
            energy_est.average_scalar();
            energy_est.append_scalar_to_file(filename, "## energy ## \t ## error ##");
            this->outfile << "Check-point " << n+1 << "/" << n_ckp << std::endl;
            this->fm.write_matrix(&this->configuration, "config.inp", "   ##  x  ##   \t  ##  y  ##   \t  ##  z  ##");
        }
        this->outfile << "Inverse Temperature: " << beta << "\t Acceptance: " << double(sum)/double(this->eqb_params.nsweeps*this->cell.nparts) << std::endl;
    }
}

void simulator::warmup()
{
    std::pair< bool, double > mc_out;
    int ckp_size;
    ckp_size = int(this->flags.chkpt_interval * this->warmup_params.nsweeps);
    int n_ckp;
    n_ckp = int(this->warmup_params.nsweeps / ckp_size);
    int n,m,p;
    bool acc;
    int sum;
    sum = 0;
    double energy;
    energy = this->measure.energy(&this->configuration);
    estimator energy_est;
    std::string filename;
    filename = "energy_warmup.dat";
    for(n = 0; n < n_ckp; n++)
    {
        energy_est.reset_accumulated_scalar();
        for(m=0; m < ckp_size; m++)
        {
            for(p=0; p < this->cell.nparts; p++)
            {
                mc_out = this->mc_engine.single_particle_sweep(&this->configuration, this->warmup_params.beta);
                sum += mc_out.first;
                energy += mc_out.second;
            }
            energy_est.accumulate_scalar(energy);
        }
        energy_est.average_scalar();
        energy_est.append_scalar_to_file(filename, "## energy ## \t ## error ##");
        this->outfile << "Check-point " << n+1 << "/" << n_ckp << std::endl;
        this->fm.write_matrix(&this->configuration, "config.inp", "   ##  x  ##   \t  ##  y  ##   \t  ##  z  ##");
    }
    this->outfile << "Inverse Temperature: " << this->warmup_params.beta << "\t Acceptance: " << double(sum)/double(this->warmup_params.nsweeps*this->cell.nparts) << std::endl;
}

void simulator::get_data()
{
    std::pair< bool, double > mc_out;
    int n, m, p;
    int sumt=0;
    int sumb=0;

    //##### ENERGY
    double energy, energy_b;
    energy = this->measure.energy(&this->configuration);
    estimator en_block_est;
    estimator en_total_est;
    en_total_est.reset_accumulated_scalar();

    //##### RADIAL DISTRIBUTION
    vec1d r;
    vec1d gr,gr_av,gr_err;
    vec2d G;
    r = this->measure.create_distance_grid();
    estimator gr_block_est;
    estimator gr_total_est;
    gr_total_est.reset_accumulated_vector();
    gr = this->measure.gr(&this->configuration);

    //##### BOND ORIENTATIONAL ORDER
    double boo, boo_av;
    std::string bondfile1, bondfile2;
    boo = this->measure.bond_order(&this->configuration, this->data_flags.boo_mode);
    bondfile1 = "boo" + std::to_string(int(this->data_flags.boo_mode)) + "_block.dat";
    bondfile2 = "boo" + std::to_string(int(this->data_flags.boo_mode)) + "_estimator.dat";
    estimator boo_block_est;
    estimator boo_total_est;
    boo_total_est.reset_accumulated_scalar();

    //##### RECIPROCAL DENSITY
    vec2d kdens,K;
    vec1d kdens_av,kdens_err;
    std::string kdfile;
    estimator kd_block_est;
    estimator kd_total_est;
    kd_total_est.reset_accumulated_vector();
    kdens =  this->measure.recip_density(&this->configuration);


    for(n = 0; n < this->data_params.nblocks; n++)
    {
        sumb = 0;
        en_block_est.reset_accumulated_scalar();
        boo_block_est.reset_accumulated_scalar();
        gr_block_est.reset_accumulated_vector();
        kd_block_est.reset_accumulated_vector();
        for(m=0; m < this->data_params.nsweeps; m++)
        {
            for(p=0; p < this->cell.nparts; p++)
            {
                mc_out = this->mc_engine.single_particle_sweep(&this->configuration, this->data_params.beta);
                sumt += mc_out.first;
                sumb += mc_out.first;
            }
            energy = this->measure.energy(&this->configuration);
            en_block_est.accumulate_scalar(energy);
            if(this->data_flags.gr)
            {
                gr = this->measure.gr(&this->configuration);
                gr_block_est.accumulate_vector(&gr);
            }
            if(this->data_flags.boo)
            {
                boo = this->measure.bond_order(&this->configuration, this->data_flags.boo_mode);
                boo_block_est.accumulate_scalar(boo);
            }
            if(this->data_flags.kd)
            {
                kdens = this->measure.recip_density(&this->configuration);
                kd_block_est.accumulate_vector(&(kdens[2]));
            }
        }
        energy_b = en_block_est.average_scalar();
        en_total_est.accumulate_scalar(energy_b);
        en_block_est.append_scalar_to_file("energy_data.dat", "## energy ## \t ## error ##");
        en_total_est.append_scalar_to_file("energy_estimator.dat", "## energy ## \t ## error ##");
        if(this->data_flags.gr)
        {
            gr_av = gr_block_est.average_vector();
            gr_total_est.accumulate_vector(&gr_av);
            gr_err = gr_total_est.error_vector();
            G.clear();
            G.push_back(r);
            G.push_back(gr_total_est.average_vector());
            G.push_back(gr_err);
            this->fm.write_matrix_transposed(&G, "gr.dat", "  ##  r  ##  \t##  g(r)  ##\t## error  ##");
        }
        if(this->data_flags.boo)
        {
            boo_av = boo_block_est.average_scalar();
            boo_total_est.accumulate_scalar(boo_av);
            boo_block_est.append_scalar_to_file(bondfile1, "## BOO ## \t ## error ##");
            boo_total_est.append_scalar_to_file(bondfile2, "## BOO ## \t ## error ##");
        }
        if(this->data_flags.kd)
        {
            kdens_av = kd_block_est.average_vector();
            kd_total_est.accumulate_vector(&kdens_av);
            kdens_err = kd_total_est.error_vector();
            K.clear();
            K.push_back(kdens[0]);
            K.push_back(kdens[1]);
            K.push_back(kd_total_est.average_vector());
            K.push_back(kdens_err);
            this->fm.write_matrix_transposed(&K, "k_density.dat", " ##  k_x  ##\t ##  k_y  ##\t# intensity #\t  ## err ## ");
        }
        this->outfile << "Block " << n+1 << "/" << this->data_params.nblocks << "\t\t" << "Acc: " << double(sumb) / double(this->data_params.nsweeps*this->cell.nparts) << std::endl;
        this->fm.write_matrix(&this->configuration, "config.inp", "   ##  x  ##   \t  ##  y  ##   \t  ##  z  ##");
    }
    this->outfile << "Inverse Temperature: " << this->data_params.beta << "\t Acceptance: " << double(sumt)/double(this->data_params.nsweeps*this->data_params.nblocks*this->cell.nparts) << std::endl;
}

void simulator::run()
{
    if(this->flags.initial_config == false)
    {
        this->initialize_randomly();
        this->outfile << "Initial configuration is random." << std::endl;
    }
    if(this->flags.eqb == true)
    {
        this->outfile << "\nMode Equilibrate initialized!" << std::endl;
        this->equilibrate();
    }
    if(this->flags.warmup == true)
    {
        this->outfile << "\nMode Warmup initialized!" << std::endl;
        this->warmup();
    }
    if(this->flags.data == true)
    {
        this->outfile << "\nMode Data initialized!" << std::endl;
        this->get_data();
    }

}
