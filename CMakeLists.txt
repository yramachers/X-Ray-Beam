cmake_minimum_required(VERSION 3.12)
project(xraybeam VERSION 0.1.0)

# Compiler/build settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(Geant4 10.7 REQUIRED)

# Build
add_executable(xraybeam
  xraybeam.cc
  src/SEActionInitialization.cc
  src/SEDetectorConstruction.cc
  src/SEEventAction.cc
  src/SEPSEnergyDeposit.cc
  src/SEPSLocation.cc
  src/SEPrimaryGeneratorAction.cc
  src/SERunAction.cc)
target_include_directories(xraybeam PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(xraybeam PRIVATE ${Geant4_LIBRARIES})
