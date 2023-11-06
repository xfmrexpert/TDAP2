cmake_minimum_required(VERSION 3.0.0)
project(TransFE VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(TransFE main.cpp algebraicsystem.cpp discretesystem.cpp displacementconstraint.cpp dof.cpp elasticityfc.cpp elasticitysc.cpp electrostaticanalysis.cpp electrostaticfc.cpp electrostaticsc.cpp feanalysis.cpp forcecontributor.cpp lin1dsf.cpp linearsystemassembler.cpp linquadsf.cpp lintetsf.cpp lintrisf.cpp magaxistaticanalysis.cpp magaxistaticfc.cpp magaxistaticsc.cpp mapping1d2d.cpp mapping2d.cpp mapping2daxi.cpp mapping3d.cpp quadquadsf.cpp quadtrisf.cpp stiffnesscontributor.cpp MeshDB/geomentity.cpp MeshDB/mesh.cpp MeshDB/meshedge.cpp MeshDB/meshentity.cpp MeshDB/meshface.cpp MeshDB/meshregion.cpp MeshDB/meshvertex.cpp MeshDB/node.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
