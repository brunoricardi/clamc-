/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 01:39:26 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: metrics.cpp
 * @Last modified by:   bruno
 * @Last modified time: Mon 17 Dec, 2018 | 02:51:15 -02:00
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

#include "metrics.h"

metrics::metrics()
{
}

metrics::~metrics()
{
}

void metrics::set_metric(double _metric)
{
    this->metric = _metric;
}

double metrics::min_image(double distance_1d)
{
   double d = std::abs(distance_1d);
   if(d > this->metric / 2.0)
   {
       return this->metric - d;
   }
   else{ return d; }
/*
   double l2;
   l2 = this->metric / 2.0;
   int n_cells;
   n_cells = int(d / l2);
   return (d - double(n_cells)*l2);
*/
}

double metrics::pbc(double position_1d)
{
    double d = position_1d;
    if (d > this->metric / 2.0)
    {
        while(d > this->metric / 2.0)
            d = d - this->metric;
    }
    else if (d < - this->metric / 2.0)
    {
        while(d < - this->metric / 2.0)
            d = d + this->metric;
    }
    return d;
}

double metrics::distance(vec1d* p1, vec1d* p2)
{
   int i;
   double d=0;
   for(i=0; i < p2->size(); i++)
       d += pow( this->min_image( (*p1)[i] - (*p2)[i] ) , 2.0 );
   return sqrt(d);
}

vec1d metrics::single_relative_distance(vec1d* p, vec2d* config)
{
   vec1d reldist;
   int part;
   int dir;
   for(part=0; part < config->size(); part++)
       reldist.push_back( this->distance( p, &(*config)[part] ) );
   return reldist;
}

vec2d metrics::full_relative_distance(vec2d* config)
{
   vec1d zeros(config->size(), 0.0);
   vec2d reldist;
   int p1,p2;
   for(p1=0; p1 < config->size(); p1++)
       reldist.push_back(zeros);
   for(p1=0; p1 < config->size(); p1++)
   {
       for(p2=0; p2 < p1; p2++)
           reldist[p1][p2] = this->distance( &(*config)[p1], &(*config)[p2]);
   }
   return reldist;
}

bool metrics::check_encroachment(vec1d *p, vec2d* config, double dist)
{
    int part;
    double d;
    bool result=false;
    for(part=0; part < config->size(); part++)
    {
       d = this->distance(p, &(*config)[part]);
       //std::cout << d << std::endl;
       if ( d < dist && d > 0.0 )
       {
           result = true;
           //std::cout << "Encroached!" << std::endl;
       }
    }
    return result;
}

vec1d metrics::mid_point(vec1d* p1, vec1d* p2)
{
    vec1d mp;
    int i;
    double mid;
    for(i=0; i < p1->size(); i++)
    {
        mid = 0.5*( (*p1)[i] + (*p2)[i] );
        mp.push_back(mid);
    }
    return mp;
}

vec1d metrics::relative_position(vec1d* p1, vec1d* p2)
{
    vec1d rp;
    int i;
    double diff;
    for(i=0; i < p1->size(); i++)
    {
        diff = (*p1)[i] - (*p2)[i];
        rp.push_back(diff);
    }
    return rp;
}

double metrics::magnitude(vec1d* p1)
{
    double mag2=0;
    int i;
    for(i=0; i < p1->size(); i++)
        mag2 += (*p1)[i] * (*p1)[i];
    return sqrt(mag2);
}
