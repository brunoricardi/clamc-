/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Thu 13 Dec, 2018 | 09:54:41 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: estimator.cpp
 * @Last modified by:   bruno
 * @Last modified time: Thu 13 Dec, 2018 | 04:04:56 -02:00
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
#include "estimator.h"

estimator::estimator()
{
}

estimator::~estimator()
{
}

void estimator::reset_accumulated_scalar()
{
    this->accumulated_scalar.first = 0.0;
    this->accumulated_scalar.second = 0.0;
    this->scalar_counter = 0.0;
    this->scalar_flag = true;
}

void estimator::reset_accumulated_vector()
{
    this->accumulated_vector.clear();
    this->vector_counter = 0.0;
    this->vector_flag = true;
}

void estimator::accumulate_scalar(double value)
{
    this->accumulated_scalar.first += value;
    this->accumulated_scalar.second += value*value;
    this->scalar_counter += 1.0;
    this->scalar_flag = false;
}

void estimator::accumulate_vector(vec1d* value)
{
    int i;
    if(this->vector_flag == true)
    {
        std::pair<double, double> zero;
        zero.first = 0.0;
        zero.second = 0.0;
        for (i=0; i < value->size(); i++)
        {
            this->accumulated_vector.push_back(zero);
        }
    }
    for (i=0; i < value->size(); i++)
    {
        this->accumulated_vector[i].first += (*value)[i];
        this->accumulated_vector[i].second += (*value)[i]*(*value)[i];
    }
    this->vector_counter += 1.0;
    this->vector_flag = false;
}

double estimator::average_scalar()
{
    double avg;
    avg = this->accumulated_scalar.first / this->scalar_counter;
    return avg;
}

vec1d estimator::average_vector()
{
    vec1d avg;
    int i;
    double point_avg;
    for(i=0; i < this->accumulated_vector.size(); i++)
    {
        point_avg = this->accumulated_vector[i].first / this->vector_counter;
        avg.push_back(point_avg);
    }
    return avg;
}

double estimator::error_scalar()
{
    double avg;
    avg = this->average_scalar();
    double err=0.0;
    double size;
    size = this->scalar_counter;
    if(size > 1.0)
    {
        err = (this->accumulated_scalar.second - size*avg*avg) / (size*(size - 1.0));
        err = sqrt(err);
    }
    return err;
}

vec1d estimator::error_vector()
{
    vec1d avg;
    avg = this->average_vector();
    int i;
    vec1d err;
    for(i=0; i < this->accumulated_vector.size(); i++)
        err.push_back(0.0);
    double size;
    size = this->vector_counter;
    if(size > 1.0)
    {
        //std::cout << "Im here" << std::endl;
        for(i=0; i < this->accumulated_vector.size(); i++)
        {
            err[i] = (this->accumulated_vector[i].second - size*avg[i]*avg[i]) / (size*(size - 1.0));
            err[i] = sqrt(err[i]);
            //std::cout << err[i] << std::endl;
        }
    }
    return err;
}

void estimator::append_scalar_to_file(std::string filename, std::string header)
{
    vec1d data;
    data.push_back(this->average_scalar());
    data.push_back(this->error_scalar());
    this->fm.append_vector(&data, filename, header);
}

//void estimator::write_vector_to_file(std::string filename, std::string header)
//{
//    vec2d
//}
