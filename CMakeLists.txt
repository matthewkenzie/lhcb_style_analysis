cmake_minimum_required (VERSION 2.6)

project (Analysis)

# Get ROOT
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(ROOT REQUIRED COMPONENTS MathCore MathMore RIO Hist Tree RooFit RooFitCore TMVA RooStats)

# Get Boost
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${ROOT_INCLUDE_DIR}")
include_directories("${Boost_INCLUDE_DIR}")

#aux_source_directory(src SOURCES)
#message( "Non recur" )
#foreach( fil ${SOURCES} )
  #message( ${fil} )
#endforeach()

# get all the source files
# message( "Full recur" )
file ( GLOB_RECURSE RECUR_SOURCES src/*.cc )
foreach( fil ${RECUR_SOURCES} )
  file( RELATIVE_PATH pat ${PROJECT_SOURCE_DIR} ${fil} )
  #message( ${pat} )
  list( APPEND SOURCES ${pat} )
endforeach()

#message("Done")
#message( ${ALL_SOURCES} )

# Make the executables
file( GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/main/*.cpp" )
foreach( source_file ${MAIN_SOURCES} )
  string( REPLACE ".cpp" "" exec_name_path ${source_file} )
  get_filename_component( exec_name ${exec_name_path} NAME )
  #message( ${exec_name} ${source_file} )
  add_executable( ${exec_name} ${source_file} ${SOURCES} )
  target_link_libraries( ${exec_name} ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
  install( TARGETS ${exec_name} DESTINATION "${PROJECT_SOURCE_DIR}/bin" )
endforeach()

