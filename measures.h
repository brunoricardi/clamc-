/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Wed 12 Dec, 2018 | 11:16:04 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: measures.h
 * @Last modified by:   bruno
 * @Last modified time: Mon 14 Jan, 2019 | 01:50:48 -02:00
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
#ifndef MEASURES_H
#define MEASURES_H

#include "includes.h"
#include "dataTypes.h"
#include "metrics.h"
#include "simulation_cell.h"
#include "physics.h"
#include "statistics.h"
#include "constants.h"

class measures
{
public:
    measures();     //class constructor
    ~measures();    //class destructor
private:
    metrics M;
    simulation_cell cell;
    physics phys;
    statistics stat;
public:
    void set_parameters(simulation_cell _cell);                         //inherits parameters from simualtion cell

private:
    vec1d distance_bin_edges;
    vec1d distance_bin_centers;
    vec1d distance_bin_areas;
public:
    vec1d create_distance_grid();

private:
    vec2d relative_distances;
public:
    double energy(vec2d* configuration);    //total energy of a configuration, MUST be computed BEFORE anything else
    double diff_energy(vec2d* in_config, vec2d* fi_config, int particle);      //energy difference between configs that differ by a single particle move
    vec1d gr(vec2d* config);        //radial distribution of pairs
    double bond_order(vec2d* config, double m);  //bond-orientational order parameter, mode m
    vec2d recip_density(vec2d* config);             //calculates Fourier transform of density


};

#endif /* MEASURES_H */
