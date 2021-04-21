# X-Ray Beam Example

Simulate a 10 keV x-ray beam on a Diamond detector.

## Notes 

Set default physics list as G4EmStandardPhysics_option4 for high precision low energy physics; G4ParticleGun for simple event 
generation, default is a gamma with 10 keV starting from a circular vertex region with 90 mum radius into the diamond.

Geometry is a vacuum box world with a box made of carbon at density 3.515 g/cm^3 inserted. Measures of the diamond box are 
5.1 x 3.6 x 0.3 mm^3. 

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
