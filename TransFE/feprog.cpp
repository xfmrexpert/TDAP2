/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
//#include <config.h>
#endif

#include "feprog.h"

#include <iostream>
#include <cstdlib>
#include "MeshDB/mesh.h"
#include "magaxistaticanalysis.h"

#include "MeshDB/meshface.h"

FEProg::FEProg(){
	theAnalysis = new MagAxiStaticAnalysis(0);
}

FEProg::~FEProg(){
	delete theAnalysis;
}

#include <chrono>

using namespace std::chrono;

std::shared_ptr<Mesh> FEProg::run_FEA(const std::string& filename, int formulation)
{  
    auto start = high_resolution_clock::now();

    if(theAnalysis!=nullptr){
        delete theAnalysis;
    }

    auto delete_analysis_end = high_resolution_clock::now();

    theAnalysis = new MagAxiStaticAnalysis(formulation);
    auto new_analysis_end = high_resolution_clock::now();

    theAnalysis->getMesh()->readAttributes(filename + ".att");
    auto read_attributes_end = high_resolution_clock::now();

    theAnalysis->getMesh()->readMesh(filename + ".msh");
    auto read_mesh_end = high_resolution_clock::now();

    theAnalysis->getMesh()->writeMesh(filename + ".out");

    theAnalysis->run();
    auto run_analysis_end = high_resolution_clock::now();

    theAnalysis->saveOut(filename);
    auto save_out_end = high_resolution_clock::now();

    auto end = high_resolution_clock::now();

    std::cout << "Delete analysis time: " << duration_cast<milliseconds>(delete_analysis_end - start).count() << " ms" << std::endl;
    std::cout << "New analysis time: " << duration_cast<milliseconds>(new_analysis_end - delete_analysis_end).count() << " ms" << std::endl;
    std::cout << "Read attributes time: " << duration_cast<milliseconds>(read_attributes_end - new_analysis_end).count() << " ms" << std::endl;
    std::cout << "Read mesh time: " << duration_cast<milliseconds>(read_mesh_end - read_attributes_end).count() << " ms" << std::endl;
    std::cout << "Run analysis time: " << duration_cast<milliseconds>(run_analysis_end - read_mesh_end).count() << " ms" << std::endl;
    std::cout << "Save out time: " << duration_cast<milliseconds>(save_out_end - run_analysis_end).count() << " ms" << std::endl;
    std::cout << "Total time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    return theAnalysis->getMesh();
}

