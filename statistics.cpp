/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Wed 12 Dec, 2018 | 03:41:08 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: statistics.cpp
 * @Last modified by:   bruno
 * @Last modified time: Thu 13 Dec, 2018 | 09:40:28 -02:00
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
#include "statistics.h"

statistics::statistics()
{
}

statistics::~statistics()
{
}

vec1d statistics::histogram(vec1d* data, vec1d* grid)
{
    int i,j;
    int ndata;
    ndata = data->size();
    int nbins;
    nbins = grid->size() - 1;
    vec1d hist;
    for(i=0; i < nbins; i++)
        hist.push_back(0.0);
    double count=0;
    for(i=0; i < ndata; i++)
    {
        for(j=0; j < nbins; j++)
        {
            if( (*data)[i] >= (*grid)[j] && (*data)[i] < (*grid)[j+1] )
            {
                hist[j] += 1.0;
                count += 1.0;
            }
        }
    }
    return hist;
}
