/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 01:34:20 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: metrics.h
 * @Last modified by:   bruno
 * @Last modified time: Mon 17 Dec, 2018 | 02:55:56 -02:00
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
#ifndef METRICS_H
#define METRICS_H

#include "includes.h"
#include "dataTypes.h"
#include "constants.h"

class metrics
{
public:
    metrics();      //class constructor
    ~metrics();     //class destructor
private:
    double metric;  //distance used to compute minimal images and PBC
public:
    void set_metric(double _metric);  //sets metric from outside

public:
    double min_image(double distance_1d);   //calculates 1-dim distance according to minimal image convention
    double pbc(double position_1d);         //periodic boundary condition
    double distance(vec1d* p1, vec1d* p2); //calculates distance between two points in space
    vec1d single_relative_distance(vec1d* p, vec2d* config);    //calculates relative between one point and a set of points
    vec2d full_relative_distance(vec2d* config);    //calculates relative distance matrix of a set of points
    bool check_encroachment(vec1d *p, vec2d* config, double dist);  //checks if a point and set of points have distances smaller than a certain distance
    vec1d mid_point(vec1d* p1, vec1d* p2);          //mid point between two points
    vec1d relative_position(vec1d* p1, vec1d* p2);   //relative position between two points (vectors)
    double magnitude(vec1d* p1);    //magnitude of vector
};

#endif /* METRICS_H */
