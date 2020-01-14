/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Thu 13 Dec, 2018 | 09:47:37 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: estimator.h
 * @Last modified by:   bruno
 * @Last modified time: Mon 14 Jan, 2019 | 02:56:15 -02:00
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
#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "includes.h"
#include "dataTypes.h"
#include "file_manager.h"

class estimator
{
public:
    estimator();    //class constructor
    ~estimator();   //class destructor

private:
    statistical_scalar accumulated_scalar;      //pair that will accumulate sum(x) and sum(x^2)
    statistical_vector accumulated_vector;     //same but for vectors
    bool scalar_flag;       //flag that is true if scalar is empty
    bool vector_flag;       //flag that is true if vector is empty
    double scalar_counter;  //counter of accumulated values
    double vector_counter;  //counter of accumulated values
    file_manager fm;

public:
    void reset_accumulated_scalar();        //resets scalar estimator
    void reset_accumulated_vector();        //resets vector estimator
    //void reset_accumulated_matrix();        //resets matrix estimator
    void accumulate_scalar(double value);   //accumulates scalar
    void accumulate_vector(vec1d* value);   //accumulates vector
    //void accumulate_matrix(vec2d* value, int axis)  //accumulates matrix, just one axis
    double average_scalar();                //calculates average of scalar
    vec1d average_vector();                 //caculates average of vector
    //vec2d average_matrix(int axis);
    double error_scalar();                  //calculates error of scalar
    vec1d error_vector();                   //calculates error of vector
    void append_scalar_to_file(std::string filename, std::string header);
};

#endif /* ESTIMATOR_H */
