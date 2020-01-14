# clamc++
Classical Monte Carlo Simulator in C++

January 14, 2019.

CLAMC is Classical TVN Monte Carlo code written in C++ by Bruno R. de Abreu (bruno.ifgw08@gmail.com).

	This program and the accompanying materials are made available under explicit agreement
      between Bruno R. de Abreu and the end user. You may not redistribute the code and
      accompanying material to anyone.
	On the event that the software is used to generate data that is used implicitly or explicitly
      for research purposes, proper acknowledgment must be provided in the citations section of
      publications.
	This software cannot be used for commercial purposes in any way whatsoever.

	Copyright: (c) 2019 Bruno R. de Abreu
			All rights reserved.

The code is composed of 10 modules that are connected into the final executable. They are:

- estimator: routines to calculate averages and statistical errors;
- file_manager: controls output and input to files;
- main: calls modules to perform simulation;
- measures: calculates physical properties of the system;
- metrics: calculates metric-related quantities, mainly distances;
- MonteCarlo: MC engine for update of configurations;
- physics: contains the definition of interaction, i.e. characterizes the physical system;
- simulation_cell: generic parameters for the simulation cell;
- simulator: defines different running modes of the code;
- statistics: other statistical properties such as histograms;

Up to the present date, the code does not have a test routine to check compilation. However, the libraries are standard C++ libraries and a sufficiently updated generic compiler should not present any compilation issues. As it can be seen in the Makefile, g++ is granted to work well (I have used gcc 5.5.0).




INPUT:
For every single simulation, a file "simulation.inp" is automatically read. If this file is absent, the code should return an error. The commands consist of 6 capital characters strings that are detailed below. Their order is irrelevant. However, most of them cannot be absent for a proper simulation, since default values are not defined in the code. Arguments of the commands follow with a blank " " character separator. Each command must come in a different line.

- BOXDIM (int)
Mandatory, single integer argument: dimension of the space where the system is defined.

- DENSIT (double)
Mandatory, single double argument: number-density of the system, (# particles) / (hyper-volume).

- NPARTS (int)
Mandatory, single integer argument: total number of particles.

- SCALES (double) (double) (double) (double)
Mandatory, four double arguments: length (first two) and energy (last two) scales of the system. One pair can be set to zero, but this is fundamental for the proper working of the potential function. For hard-core systems, one of the scales is the size of the atoms.

- DISPLA (double)
Mandatory, single double argument: trial displacement for Monte Carlo movements. Should be adjusted during test runs in order to give proper acceptance ratio. Default value is 1.0, which may not be a good choice at all.

- CHKPNT (double)
Optional, single double argument: check-point intervals. It will multiply the total number of MC sweeps by this number and the simulation will be saved every time one check point is reached.

- CONFIG (string)
Optional, single string argument: code will start from configuration read in this file. If absent, code will start from random configuration. This file must contain a header that will be ignored.

- EQUILI (int) (double) (double) (int)
Optional, four arguments. It will initiate the Equilibrate Mode. First argument is the number of MC sweeps, second argument is the final inverse temperature to be reached. Third argument is the inv. temp. increment, and the fourth argument is the number of increments to be used.

- WARMUP (int) (double)
Optional, integer and double argument. Initiates Warmup Mode. First argument is the number of MC sweeps, second argument is the inverse temperature.

- GETDAT (int) (int) (double) (int) <string> <string> ... <double>
Optional, multiple arguments. Initiates Data Mode. Only with this command the code will calculate physical properties. First argument is the number of MC sweeps in each MC block (second argument). Third argument is the inverse temperature. Fourth argument is the number of physical properties that you want to calculate (set to 0 if you want just the energy.) If this argument is N, there must be N folowing strings that indicate the property of interest. As of today, "gr" for the radial distribution of pairs (two-dimensional systems only) and "boo" for the bond-orientational order parameter (also in 2D) are implemented. In the later case, after the string there must come a double argument that indicates the bond-mode.




OUTPUT:
Depending on the running mode, different files will be generated.

- output.log
Everything that would be printed in the screen will be printed to this file. Of valuable interest is the check-point current number (block number for GETDAT mode).

- config.inp
Configuration of the system. Each column corresponds to one spatial dimenstion, each row corresponds to a particle.


In case the GETDAT mode is activated, there could also be the files:
- energy_block.dat
Energy and error for each MC block.

- energy_estimator.dat
Energy estimator and error of the simulation.

- gr.dat
Radial distribution of pairs. First column is the distance between particles, second column the correlation function and third column the associated statistical error.

- boo<int>_block.dat
Bond orientational order parameter for each MC block. First column is the value, second the error.

- boo<int>_estimator.dat
Bond orientational order parameter estimator for the simulation. First column is the value, second the error.

