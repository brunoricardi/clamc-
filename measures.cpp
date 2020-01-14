/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Wed 12 Dec, 2018 | 11:25:28 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: measures.cpp
 * @Last modified by:   bruno
 * @Last modified time: Mon 14 Jan, 2019 | 03:55:40 -02:00
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
#include "measures.h"

measures::measures()
{
}

measures::~measures()
{
}

void measures::set_parameters(simulation_cell _cell)
{
    this->cell.set_parameters(_cell.dim, _cell.nparts, _cell.density);
    this->cell.set_scales(_cell.scale.length1, _cell.scale.length2, _cell.scale.energy1, _cell.scale.energy2);
    this->M.set_metric(this->cell.length);
    this->phys.set_parameters(_cell);
}

double measures::energy(vec2d* config)
{
    vec2d reldist;
    reldist = this->M.full_relative_distance(config);
    this->relative_distances = this->M.full_relative_distance(config);
    double pot = 0;
    int p1, p2;
    for(p1=0; p1 < config->size(); p1++)
    {
        for(p2=0; p2 < p1; p2++)
            pot += this->phys.potential(reldist[p1][p2]);
    }
    return pot;
}

double measures::diff_energy(vec2d* in_config, vec2d* fi_config, int particle)
{
    vec1d in_rd, fi_rd;
    in_rd = this->M.single_relative_distance(&(*in_config)[particle], in_config);
    fi_rd = this->M.single_relative_distance(&(*fi_config)[particle], fi_config);
    double in_pot=0, fi_pot=0;
    int i;
    for(i = 0; i < in_config->size(); i++)
    {
        in_pot += this->phys.potential(in_rd[i]);
        fi_pot += this->phys.potential(fi_rd[i]);
    }
    return (fi_pot - in_pot);
}

vec1d measures::create_distance_grid()
{
    double ls;
    if(this->cell.scale.energy1 >= this->cell.scale.energy2)
        ls = this->cell.scale.length1;
    else
        ls = this->cell.scale.length2;
    double rc;
    if(this->cell.length / 2.0 > 30.0*ls)
        rc = 30.0*ls;
    else
        rc = this->cell.length / 2.0;
    vec1d edges;
    double dr;
    dr = rc / 100.0;
    int i;
    for(i = 0; i < 101; i++)
        edges.push_back(double(i)*dr);
    this->distance_bin_edges = edges;
    vec1d center, area;
    double r, a;
    for(i = 0; i < 100; i++)
    {
        r = 0.5*(edges[i] + edges[i+1]);
        center.push_back(r);
        a = 3.1415926*(edges[i+1]*edges[i+1] - edges[i]*edges[i]);
        area.push_back(a);
    }
    this->distance_bin_centers = center;
    this->distance_bin_areas = area;
    return center;
}

vec1d measures::gr(vec2d* config)
{
    vec2d reldist;
    reldist = this->relative_distances;
    int nrows, ncols;
    nrows = config->size();
    ncols = (*config)[0].size();
    int i, j;
    vec1d distances;
    for(i=0; i < nrows; i++)
    {
        for(j=0; j < i; j++)
            distances.push_back(reldist[i][j]);
    }
    vec1d freq;
    freq = this->stat.histogram(&distances, &this->distance_bin_edges);
    double norm;
    double value;
    vec1d gr;
    for(i=0; i < freq.size(); i++)
    {
        norm = this->distance_bin_areas[i]*this->cell.density*this->cell.nparts / 2.0;
        value = freq[i] / norm;
        gr.push_back(value);
    }
    return gr;
}

double measures::bond_order(vec2d* config, double m)
{
    vec2d reldist;
    reldist = this->relative_distances;
    //find bonds
    std::vector< std::pair<int,int> > bonds;
    std::pair<int, int> bond;
    int p1, p2;
    double r1, r2;
    for(p1=0; p1 < config->size(); p1++)
    {
        for(p2=0; p2 < p1; p2++)
        {
            if(reldist[p1][p2] <= this->cell.scale.length2)
            {
                r1 = this->M.magnitude(&(*config)[p1]);
                r2 = this->M.magnitude(&(*config)[p2]);
                if(r1 >= r2)
                {
                    bond.first = p2;
                    bond.second = p1;
                }
                else
                {
                    bond.first = p1;
                    bond.second = p2;
                }
                bonds.push_back(bond);
            }
        }
    }
    //bond positions and angles
    vec1d bond_position, bond_angles;
    int b;
    double cos_alpha, alpha;
    for(b=0; b < bonds.size(); b++)
    {
        p1 = bonds[b].first;
        p2 = bonds[b].second;
        bond_position = this->M.relative_position(&(*config)[p2], &(*config)[p1]);
        cos_alpha = bond_position[0] / this->M.magnitude(&bond_position);
        alpha = acos(cos_alpha);
        if(bond_position[1] < 0.0)
        {
            bond_angles.push_back(-alpha);
        }
        else
        {
            bond_angles.push_back(alpha);
        }
    }
    double Nb;
    Nb = double(bond_angles.size());

    double re=0.0;
    double im=0.0;
    for(b=0; b < bond_angles.size(); b++)
    {
        re += cos(m*bond_angles[b]);
        im += sin(m*bond_angles[b]);
    }
    re = re / Nb;
    im = im / Nb;
    double B;
    B = re*re + im*im;
    return B;

}

vec2d measures::recip_density(vec2d* config)
{
    int nparts;
    nparts = config->size();
    std::complex<double> c_i(0.0,1.0);
    double klim = 8.0*PI;
    double dk;
    dk = 2.0*klim / 99.0;
    vec1d grid_k;
    double k;
    int i,j;
    int gsize=100;
    k = -klim;
    for(i=0;i<gsize;i++)
    {
        grid_k.push_back(k);
        k += dk;
    }
    std::complex<double> c_zero(0.0,0.0);
    std::vector< std::complex<double> > c_zeros;
    for(i=0; i < gsize; i++)
        c_zeros.push_back(c_zero);
    std::vector< std::vector< std::complex<double> > > Ft;
    for(i=0; i < gsize;i++)
        Ft.push_back(c_zeros);

    //std::cout << "Everything fine until here: variables declared" << std::endl;
    int m,n;
    double kx, ky, kr;
    std::complex<double> f;
    for(m=0; m<gsize; m++)
    {
        for(n=0; n<gsize; n++)
        {
            kx = grid_k[m];
            ky = grid_k[n];
            f = (0.0, 0.0);
            for(j=0; j<nparts; j++)
            {
                kr = kx*(*config)[j][0] + ky*(*config)[j][1];
                f += std::exp(-c_i*kr);
            }
            Ft[m][n] = f;
        }
    }
    //std::cout << "Calculated fourier transform!" << std::endl;


    vec2d FouT;
    vec1d zeros;
    int counter = 0;
    for(i=0; i<gsize*gsize; i++)
        zeros.push_back(0.0);
    for(i=0;i<3;i++)
        FouT.push_back(zeros);

    //std::cout << "Declared variables for intensity" << std::endl;
    for(m=0; m<gsize; m++)
    {
        for(n=0;n<gsize;n++)
        {
            f = Ft[m][n];
            FouT[0][counter] = grid_k[m];
            FouT[1][counter] = grid_k[n];
            FouT[2][counter] = std::norm(f);
            counter++;
        }
    }
    //std::cout << "Calculated intensity" << std::endl;
    return FouT;
}
