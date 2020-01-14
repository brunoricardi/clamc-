/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Wed 12 Dec, 2018 | 03:37:23 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: statistics.h
 * @Last modified by:   bruno
 * @Last modified time: Thu 13 Dec, 2018 | 09:44:42 -02:00
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
#ifndef STATISTICS_H
#define STATISTICS_H

#include "includes.h"
#include "dataTypes.h"

class statistics
{
public:
    statistics();       //class constructor
    ~statistics();      //class destructor
public:
    vec1d histogram(vec1d* data, vec1d* grid);       //constructs counting histograms given data and bin edges

private:
    vec1d accumulated_scalar;
    vec2d accumulated_vector;
public:
    void reset_accumulated_scalar();
    void reset_accumulated_vector();
    void accumulate_scalar(double value);
    void accumulate_vector(vec1d* value);
};

#endif /* STATISTICS_H */
