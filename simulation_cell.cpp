/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 02:41:34 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: cell.cpp
 * @Last modified by:   bruno
 * @Last modified time: Mon 10 Dec, 2018 | 04:20:30 -02:00
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
#include "simulation_cell.h"

simulation_cell::simulation_cell()
{
}

simulation_cell::~simulation_cell()
{
}

void simulation_cell::set_parameters(int _dim, int _nparts, double _density)
{
    this->dim = _dim;
    this->nparts = _nparts;
    this->density = _density;
    this->length = pow( (double(_nparts) / _density), (1.0/double(_dim)) );
}

void simulation_cell::set_scales(double ls1, double ls2, double es1, double es2)
{
    this->scale.length1 = ls1;
    this->scale.length2 = ls2;
    this->scale.energy1 = es1;
    this->scale.energy2 = es2;
}
