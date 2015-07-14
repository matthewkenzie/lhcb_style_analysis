cmake_minimum_required (VERSION 2.6)

project (Analysis)

# Get ROOT
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(ROOT REQUIRED COMPONENTS MathCore MathMore RIO Hist Tree RooFit RooFitCore TMVA RooStats)

# Get Boost
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(Boost_INCLUDE_DIR "/afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt/include/boost-1_53")
  set(Boost_LIBRARY_DIR "/afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt/lib" )
  set(ENV{Boost_ROOT} /afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") 

set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost 1.53 REQUIRED COMPONENTS program_options system filesystem REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${ROOT_INCLUDE_DIR}")
include_directories("${Boost_INCLUDE_DIRS}")

# get all the source files
# message( "Full recur" )
file ( GLOB_RECURSE RECUR_SOURCES src/*.cc )
foreach( fil ${RECUR_SOURCES} )
  file( RELATIVE_PATH pat ${PROJECT_SOURCE_DIR} ${fil} )
  #message( ${pat} )
  list( APPEND SOURCES ${pat} )
endforeach()

# Make the executables
file( GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/main/*.cpp" )
foreach( source_file ${MAIN_SOURCES} )
  string( REPLACE ".cpp" "" exec_name_path ${source_file} )
  get_filename_component( exec_name ${exec_name_path} NAME )
  #message( ${exec_name} ${source_file} )
  add_library( ${exec_name}Lib SHARED ${SOURCES} )
  set_property(TARGET ${exec_name}Lib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib")
  target_link_libraries( ${exec_name}Lib ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
  add_executable( ${exec_name} ${source_file} )
  target_link_libraries( ${exec_name} ${exec_name}Lib )
  install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exec_name} DESTINATION ${PROJECT_SOURCE_DIR}/bin ) 
endforeach()

