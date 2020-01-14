/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 04:30:43 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: simulator.h
 * @Last modified by:   bruno
 * @Last modified time: Mon 14 Jan, 2019 | 10:24:13 -02:00
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
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "simulation_cell.h"
#include "metrics.h"
#include "MonteCarlo.h"
#include "file_manager.h"
#include "includes.h"
#include "measures.h"
#include "estimator.h"
#include "dataTypes.h"

class simulator
{
public:
    simulator();        //class constructor
    ~simulator();       //class destructor

private:
    simulation_cell cell;
    metrics M;
    file_manager fm;
    MonteCarlo mc_engine;
    measures measure;

private:
    struct engine_flags
    {
        bool eqb;
        bool warmup;
        bool data;
        bool initial_config;
        double chkpt_interval;
    };
    engine_flags flags;
    void initialize_flags();
private:
    struct engine_parameters
    {
        int nsweeps;
        int nblocks;
        int ntemps;
        double beta;
        double beta_inc;
    };
    engine_parameters eqb_params, warmup_params, data_params;
private:
    //struct data_properties
    //{
    //    bool gr;
    //    bool boo;
    //    double boo_mode;
    //};
    //data_properties data_flags;
    DataFlags data_flags;
    
private:
    std::fstream outfile;

private:
    vec2d configuration;
    void initialize_randomly();

private:
    void equilibrate();
    void warmup();
    void get_data();

public:
    void read_input();
    void run();
};

#endif /* SIMULATOR_H */
