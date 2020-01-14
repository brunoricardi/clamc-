/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 03:44:07 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: MonteCarlo.h
 * @Last modified by:   bruno
 * @Last modified time: Wed 09 Jan, 2019 | 03:54:03 -02:00
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
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "dataTypes.h"
#include "metrics.h"
#include "simulation_cell.h"
#include "physics.h"
#include "includes.h"
#include "measures.h"

class MonteCarlo{
public:
    MonteCarlo();
    ~MonteCarlo();
private:
    simulation_cell cell;
    metrics M;
    measures measure;
    double delta;     //maximum displacement of particle
public:
    void set_displacement(double _delta);           //sets Monte Carlo delta
    void set_parameters(simulation_cell _cell);     //inherits parameters from simualtion cell

private:
    std::random_device rdev{};
    std::mt19937_64 rng{rdev()};
    std::uniform_real_distribution<long double> unif;
public:
    //void initialize_rng();
    long double random_number();

public:
    std::pair<bool, double> single_particle_sweep(vec2d* configuration, double temperature);   //Monte Carlo sweep
};

#endif /* MONTECARLO_H */
