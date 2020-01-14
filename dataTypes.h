/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 01:32:06 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: dataTypes.h
 * @Last modified by:   bruno
 * @Last modified time: Mon 14 Jan, 2019 | 02:06:55 -02:00
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
#ifndef DATATYPES_H
#define DATATYPES_H

#include "includes.h"

typedef std::vector<double> vec1d;
typedef std::vector<vec1d> vec2d;
typedef std::pair<double, double> statistical_scalar;
typedef std::vector< std::pair<double,double> > statistical_vector;


struct DataFlags
{
    bool gr;
    bool boo;
    double boo_mode;
    bool kd;
};

#endif /* DATATYPES_H */
