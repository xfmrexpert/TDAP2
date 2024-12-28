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

#ifdef TRANSFE
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

DLL_API class FEProg {

public:
	MagAxiStaticAnalysis* theAnalysis;
	DLL_API FEProg();
	~FEProg();
	DLL_API std::shared_ptr<Mesh> run_FEA(const std::string& filename, int formulation);
	/*ostringstream outStream;*/

};
	