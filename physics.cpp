/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 03:10:49 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: physics.cpp
 * @Last modified by:   bruno
 * @Last modified time: Wed 12 Dec, 2018 | 11:35:00 -02:00
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
#include "physics.h"

physics::physics()
{
}

physics::~physics()
{
}

void physics::set_parameters(simulation_cell _cell)
{
    this->cell.set_parameters(_cell.dim, _cell.nparts, _cell.density);
    this->cell.set_scales(_cell.scale.length1, _cell.scale.length2, _cell.scale.energy1, _cell.scale.energy2);
    this->M.set_metric(this->cell.length);
}

double physics::potential(double distance)
{
    if (distance <= this->cell.scale.length1)
    {
        //std::cout << distance << "   " << this->cell.scale.length1 << "   " << this->cell.scale.energy1 << std::endl;
        return this->cell.scale.energy1;
    }
    else if (distance > this->cell.scale.length1 && distance <= this->cell.scale.length2)
        return this->cell.scale.energy2;
    else
        return 0.0;
}
