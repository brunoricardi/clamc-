/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 02:30:17 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: cell.h
 * @Last modified by:   bruno
 * @Last modified time: Mon 10 Dec, 2018 | 04:16:42 -02:00
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
#ifndef SIMULATIONCELL_H
#define SIMULATIONCELL_H

#include "includes.h"
#include "dataTypes.h"
#include "metrics.h"

class simulation_cell
{
public:
    simulation_cell();     //class constructor
    ~simulation_cell();    //class destructor
public:
    int dim;        // spacial dimension
    int nparts;  //number of particles
    double density; //number density
    double length;  //linear length of hyper-cubic cell
    struct scales{
        double length1;
        double length2;
        double energy1;
        double energy2;
    };
    scales scale;

public:
    void set_parameters(int _dim, int _nparts, double _density);     //sets cell parameters from outside
    void set_scales(double ls1, double ls2, double es1, double es2);    //sets length and energy scales
};

#endif /* SIMULATIONCELL_H */
