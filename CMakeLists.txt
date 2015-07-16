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

# generate root dictionary
file ( GLOB TMP_CUSTOM_ROOT_INCS include/roofit/*.h )
# strip full path name
foreach( fil ${TMP_CUSTOM_ROOT_INCS} )
  file( RELATIVE_PATH pat ${PROJECT_SOURCE_DIR}/include ${fil} )
  list( APPEND CUSTOM_ROOT_INCS ${pat} )
endforeach()

# build dict
set ( CUSTOM_ROOT_DICT "RootDict" )
ROOT_GENERATE_DICTIONARY( ${CUSTOM_ROOT_DICT} ${CUSTOM_ROOT_INCS}  )
add_custom_target( ${CUSTOM_ROOT_DICT} DEPENDS ${CUSTOM_ROOT_DICT}.cxx ${CUSTOM_ROOT_DICT}.h )
list( APPEND SOURCES ${CUSTOM_ROOT_DICT}.cxx )

# Make single shared library
add_library( ${CMAKE_PROJECT_NAME}Lib SHARED ${SOURCES} )
add_dependencies( ${CMAKE_PROJECT_NAME}Lib ${CUSTOM_ROOT_DICT} )
set_property(TARGET ${CMAKE_PROJECT_NAME}Lib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries( ${CMAKE_PROJECT_NAME}Lib ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
# install it
install( TARGETS ${CMAKE_PROJECT_NAME}Lib LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/lib" )

# Make and install the executables
file( GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/main/*.cpp" )
foreach( source_file ${MAIN_SOURCES} )
  string( REPLACE ".cpp" "" exec_name_path ${source_file} )
  get_filename_component( exec_name ${exec_name_path} NAME )
  add_executable( ${exec_name} ${source_file} )
  target_link_libraries( ${exec_name} ${CMAKE_PROJECT_NAME}Lib )
  install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exec_name} DESTINATION ${PROJECT_SOURCE_DIR}/bin )
endforeach()

