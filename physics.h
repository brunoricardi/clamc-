/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 03:02:54 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: physics.h
 * @Last modified by:   bruno
 * @Last modified time: Wed 12 Dec, 2018 | 11:35:15 -02:00
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
#ifndef PHYSICS_H
#define PHYSICS_H

#include "dataTypes.h"
#include "metrics.h"
#include "simulation_cell.h"

class physics
{
public:
    physics();      //class constructor
    ~physics();     //class destructor
private:
    metrics M;
    simulation_cell cell;
public:
    void set_parameters(simulation_cell _cell);                         //inherits parameters from simualtion cell

public:
    double potential(double distance);      //two-body, isotropic potential
    
};

#endif /* PHYSICS_H */
