/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 03:47:01 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: MonteCarlo.cpp
 * @Last modified by:   bruno
 * @Last modified time: Wed 09 Jan, 2019 | 04:31:35 -02:00
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
#include "MonteCarlo.h"

MonteCarlo::MonteCarlo()
{
}

MonteCarlo::~MonteCarlo()
{
}

void MonteCarlo::set_displacement(double _delta)
{
    this->delta = _delta;
}

void MonteCarlo::set_parameters(simulation_cell _cell)
{
    this->cell.set_parameters(_cell.dim, _cell.nparts, _cell.density);
    this->cell.set_scales(_cell.scale.length1, _cell.scale.length2, _cell.scale.energy1, _cell.scale.energy2);
    this->M.set_metric(this->cell.length);
    this->measure.set_parameters(_cell);
}

long double MonteCarlo::random_number()
{
    return this->unif(rng);
}


std::pair<bool, double> MonteCarlo::single_particle_sweep(vec2d* configuration, double beta)
{
    double max_part;
    max_part = this->cell.nparts;
    int mvpart;
    mvpart = int( max_part*this->random_number() );
    vec2d trial_configuration;
    trial_configuration = (*configuration);
    int dir;
    double shift;
    for(dir = 0; dir < this->cell.dim; dir++)
    {
        shift = (this->delta / 2.0)*(2.0*this->random_number() - 1.0);
        trial_configuration[mvpart][dir] = this->M.pbc( (*configuration)[mvpart][dir] + shift );
    }
    double dE;
    dE = this->measure.diff_energy(configuration, &trial_configuration, mvpart);
    double boltzmann;
    boltzmann = exp(-dE*beta);
    double judge;
    bool result;
    if(boltzmann >= 1.0)
    {
        (*configuration) = trial_configuration;   //accept move
        result = true;
    }
    else
    {
        judge = this->random_number();
        if (boltzmann > judge)
        {
            (*configuration) = trial_configuration;   //accept move
            result = true;
        }
        else { result = false; }   //reject move, do nothing
    }
    std::pair< bool, double > outcome;
    outcome.first = result;
    if(result)
    {
        outcome.second = dE;
    }
    else { outcome.second = 0.0; }
    return outcome;
}
