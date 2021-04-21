# X-Ray Beam Example

Simulate a 15 keV x-ray beam on a Diamond detector.

## Notes 

Set default physics list as G4EmStandardPhysics_option4 for high precision low energy physics; G4ParticleGun for simple event 
generation, default is a gamma with 15 keV starting from a circular vertex region with 8.7 mum Gaussian profile into the diamond.

Geometry is a vacuum box world with a box made of carbon at density 3.515 g/cm^3 inserted. Measures of the diamond box are 
5.1 x 3.6 x 0.6 mm^3. 

Scorers: interactions in the diamond volume - deposited energy and location in 3D. Output in ROOT file.

## Build instruction

At Warwick, SCRTP, use cvmfs as the easiest environment setup (with bash):

source /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc10-opt/setup.sh

which sets up Geant4 10.7 and GCC10 on a CentOS7 background. ROOT 6.22 will also be available. Just create a 'build' 
directory, then 

cd build; cmake ..; make

and run in the build directory for instance as 

./scattering -m run.mac

Find out about CLI options using --help option.

A simple ROOT script demonstrating reading and analysis of results is also part of the package. Using the environment from 
above also permits access to ROOT. Type:

root (or 'root -b' without graphics display)

At the prompt then type:

.L analyseRootOutput.C

and call one of the functions named in the script for instance, type

shortsummary("")

which loads the default output file called xrayout.root. The script is not quite complete yet. It should allow to make 
relevant plots eventually.

