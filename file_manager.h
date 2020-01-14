/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Tue 11 Dec, 2018 | 03:07:43 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: print_manager.h
 * @Last modified by:   bruno
 * @Last modified time: Thu 13 Dec, 2018 | 03:21:16 -02:00
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
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "includes.h"
#include "dataTypes.h"

class file_manager
{
public:
    file_manager();     //class constructor
    ~file_manager();    //class destructor
public:
    void write_vector(vec1d* data, std::string filename, std::string header);       //writes vector to file
    void write_matrix(vec2d* data, std::string filename, std::string header);       //writes matrix to file
    void write_matrix_transposed(vec2d* data, std::string filename, std::string header); //writes transposed matrix to file
    void append_vector(vec1d* data, std::string filename, std::string header);      //appends vector to file
public:
    vec2d read_file(std::string filename);
};

#endif /* FILEMANAGER_H */
