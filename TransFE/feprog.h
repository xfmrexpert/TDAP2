/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "MeshDB/mesh.h"
#include "magaxistaticanalysis.h"
#include <sstream>

class FEProg {

public:
	MagAxiStaticAnalysis* theAnalysis;
	FEProg();
	~FEProg();
	std::shared_ptr<Mesh> run_FEA(const std::string& filename, int formulation);
	/*ostringstream outStream;*/

};
	