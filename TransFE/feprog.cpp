/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
//#include <config.h>
#endif

#include "feprog.h"

#include <iostream>
#include <cstdlib>
#include "MeshDB/mesh.h"
#include "elasticityanalysis.h"
#include "magaxistaticanalysis.h"

#include "MeshDB/meshface.h"
#include "lintrisf.h"
#include "mapping2d.h"

//#include "precisiontimer.h"

using namespace std;

FEProg::FEProg(){
	theAnalysis = new MagAxiStaticAnalysis(0);
}

FEProg::~FEProg(){
	delete theAnalysis;
}

#include <chrono>

using namespace std;
using namespace std::chrono;

shared_ptr<Mesh> FEProg::run_FEA(const std::string& filename, int formulation)
{  
    auto start = high_resolution_clock::now();

    if(theAnalysis!=NULL){
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

    cout << "Delete analysis time: " << duration_cast<milliseconds>(delete_analysis_end - start).count() << " ms" << endl;
    cout << "New analysis time: " << duration_cast<milliseconds>(new_analysis_end - delete_analysis_end).count() << " ms" << endl;
    cout << "Read attributes time: " << duration_cast<milliseconds>(read_attributes_end - new_analysis_end).count() << " ms" << endl;
    cout << "Read mesh time: " << duration_cast<milliseconds>(read_mesh_end - read_attributes_end).count() << " ms" << endl;
    cout << "Run analysis time: " << duration_cast<milliseconds>(run_analysis_end - read_mesh_end).count() << " ms" << endl;
    cout << "Save out time: " << duration_cast<milliseconds>(save_out_end - run_analysis_end).count() << " ms" << endl;
    cout << "Total time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    return theAnalysis->getMesh();
}

