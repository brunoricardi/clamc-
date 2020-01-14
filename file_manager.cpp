/**
 * @Author: Bruno R. de Abreu <bruno>
 * @Date:   Tue 11 Dec, 2018 | 03:15:01 -02:00
 * @Email:  bruno.ifgw08@gmail.com
 * @Project: CLAMC
 * @Filename: file_manager.cpp
 * @Last modified by:   bruno
 * @Last modified time: Thu 13 Dec, 2018 | 10:14:25 -02:00
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
#include "file_manager.h"

file_manager::file_manager()
{
}

file_manager::~file_manager()
{
}

void file_manager::write_vector(vec1d* data, std::string filename, std::string header)
{
    std::ofstream file(filename.c_str());
    file << std::scientific;
    file << std::setprecision(6);
    file << header << std::endl;
    int row;
    for(row=0; row < data->size(); row++ )
    {
        file << (*data)[row] << std::endl;
    }
    file.close();
}

void file_manager::write_matrix(vec2d* data, std::string filename, std::string header)
{
    std::ofstream file(filename.c_str());
    file << std::scientific;
    file << std::setprecision(6);
    file << header << std::endl;
    int nrows, ncols;
    nrows = data->size();
    ncols = (*data)[0].size();
    int row, col;
    for(row=0; row < nrows; row++)
    {
        for(col=0; col < ncols-1; col++)
            file << (*data)[row][col] << "\t";
        file << (*data)[row].back() << std::endl;
    }
    file.close();
}

void file_manager::write_matrix_transposed(vec2d* data, std::string filename, std::string header)
{
    std::ofstream file(filename.c_str());
    file << std::scientific;
    file << std::setprecision(6);
    file << header << std::endl;
    int nrows, ncols;
    ncols = data->size();
    nrows = (*data)[0].size();
    int row, col;
    for(row=0; row < nrows; row++)
    {
        for(col=0; col < ncols-1; col++)
            file << (*data)[col][row] << "\t";
        file << (*data)[ncols-1][row] << std::endl;
    }
    file.close();
}

void file_manager::append_vector(vec1d* data, std::string filename, std::string header)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::out | std::ios::app);
    file << std::scientific;
    file << std::setprecision(6);
    file.seekg(0, std::ios::end);
    if(file.tellg() == 0)
    {
        file << header << std::endl;
    }
    int col;
    for(col=0; col < data->size() - 1; col++ )
    {
        file << (*data)[col] << "\t";
    }
    file << (*data).back() << std::endl;
    file.close();
}

vec2d file_manager::read_file(std::string filename)
{
    std::ifstream file;
    file.open(filename.c_str(), std::ifstream::in);
    std::string line;
    vec2d data;
    if(file)
    {
        getline(file,line);     //header
        getline(file,line);
		file.clear();
		file.seekg(0,std::ios::beg);
    }
    else{ std::cout << "File " << filename << " not found!" << std::endl; return data; }
    std::stringstream iss(line);
	int ncols=0;
	do
	{
		std::string sub;
		iss >> sub;
		if(sub.length())
			++ncols;
	}
	while(iss);
    //std::cout << ncols << std::endl;
    int nlines=0;
    while( getline(file,line) )
        nlines++;
    nlines = nlines - 1;
    file.clear();
    file.seekg(0,std::ios::beg);
    getline(file,line);     //skip header
    int i,j;
    double temp;
    vec1d templine;
    for(i=0;i<nlines;i++)
    {
        for(j=0;j<ncols;j++)
        {
            file >> temp;
            templine.push_back(temp);
        }
        data.push_back(templine);
        templine.clear();
    }
    file.close();
}
