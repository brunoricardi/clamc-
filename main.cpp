/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Mon 10 Dec, 2018 | 01:41:05 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: main.cpp
 * @Last modified by:   bruno
 * @Last modified time: Thu 20 Dec, 2018 | 03:07:05 -02:00
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
#include "clamc.h"

int main()
{
    std::fstream file;
    file.open("output.log", std::ios::out | std::ios::app);
    file << std::scientific;
    file << std::setprecision(6);
    //std::cout << std::scientific;
    //std::cout << std::setprecision(4);
    file << "Hello! This is CLAMC by Bruno R. de Abreu (bruno.ifgw08@gmail.com). \
    \nPlease refer to README.txt about instructions on crucial input parameters. \
    \nYour simulation is about to start now. Enjoy!\n" << std::endl;

    simulator sim;
    sim.read_input();
    sim.run();
    return 0;

}
