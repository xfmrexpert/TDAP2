#ifndef FEPROG_H
#define FEPROG_H

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
	DLL_API shared_ptr<Mesh> run_FEA(const std::string& filename, int formulation);
	/*ostringstream outStream;*/

};
	
#endif