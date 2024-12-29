/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "magaxistaticanalysis.h"
#include "mapping2d.h"
#include "mapping2daxi.h"
#include "mapping1d2d.h"
#include "lintrisf.h"
#include "quadtrisf.h"
#include "linquadsf.h"
#include "quadquadsf.h"
#include "lin1dsf.h"
#include "quad1dsf.h"
#include "typedefs.h"

 //#include "precisiontimer.h"
#include <iostream>

using namespace std;

constexpr auto PI = 3.141592653589793238512808959406186204433;

size_t ndof = 0;

MagAxiStaticAnalysis::MagAxiStaticAnalysis(int form)
{
	if (ndof != 0) {
		//WARNING: Indicates previous calculation was not properly destroyed!
		ndof = 0;
	}
	formulation = form;
	field = std::make_unique<Field<double>>();
}

std::unique_ptr<StiffnessContributor<double>> MagAxiStaticAnalysis::makeStiffContrib(MeshFace& face) {
	const auto& nodes = face.getNodes(); // Cache nodes for efficiency
	size_t nen = nodes.size();

	std::shared_ptr<ShapeFunction> sf = nullptr;
	if (face.numEdges() == 3) {  //triangle
		if (nen == 3) { //1st order triangle
			sf = std::make_shared<LinTriSF>();
		}
		else if (nen == 6) { //2nd order triangle
			sf = std::make_shared<QuadTriSF>();
		}
		else {
			throw std::runtime_error("Unknown element type for triangle!");
		}
	}
	else if (face.numEdges() == 4) { //quad
		if (nen == 4) { //1st order quad
			sf = std::make_shared<LinQuadSF>();
		}
		else if (nen == 9) { //2nd order quad
			sf = std::make_shared<QuadQuadSF>();
		}
		else {
			throw std::runtime_error("Unknown element type for quadrilateral!");
		}
	}
	else { //dunno
		throw std::runtime_error("Unknown element type!");
	}

	shared_ptr<Mapping> mapping;
	if (formulation == 2) {
		mapping = std::make_shared<Mapping2DAxi>(&face, sf);
	}
	else {
		mapping = std::make_shared<Mapping2D>(&face, sf);
	}
	return std::make_unique<MagAxiStaticSC>(&face, field.get(), mapping, sf, formulation);
};

std::unique_ptr<ForceContributor<double>> MagAxiStaticAnalysis::makeForceContrib(MeshFace& face) {
	if (face.getClassification()->getAttribute("J") != NO_ATTRIB) {
		size_t nen = face.getNodes().size();  //inefficient...
		std::shared_ptr<ShapeFunction> sf = nullptr;
		if (face.numEdges() == 3) {  //triangle
			if (nen == 3) { //1st order triangle
				sf = std::make_shared<LinTriSF>();
			}
			else if (nen == 6) { //2nd order triangle
				sf = std::make_shared<QuadTriSF>();
			}
			else {
				std::cerr << "Unknown element type!" << std::endl;
			}
		}
		else if (face.numEdges() == 4) { //quad
			if (nen == 4) { //1st order quad
				sf = std::make_shared<LinQuadSF>();
			}
			else if (nen == 9) { //2nd order quad
				sf = std::make_shared<QuadQuadSF>();
			}
			else {
				std::cerr << "Unknown element type!" << std::endl;
			}
		}
		else { //dunno
			std::cerr << "Unknown element type!" << std::endl;
			exit(1);
		}
		shared_ptr<Mapping> mapping;
		if (formulation == 2) {
			mapping = std::make_shared<Mapping2DAxi>(&face, sf);
		}
		else {
			mapping = std::make_shared<Mapping2D>(&face, sf);
		}

		return std::make_unique<MagAxiStaticFC>(&face, field.get(), mapping, sf, formulation);
	}
	return nullptr;
};

std::unique_ptr<ForceContributor<double>> MagAxiStaticAnalysis::makeForceContrib(MeshEdge& edge) {
	return nullptr;
};

std::unique_ptr<Constraint<double>> MagAxiStaticAnalysis::makeConstraint(MeshEdge& edge) {
	auto classification = edge.getClassification();
	if (!classification) return nullptr;

	bool hasXConstraint = classification->getAttribute("x_constraint") != NO_ATTRIB;
	bool hasYConstraint = classification->getAttribute("y_constraint") != NO_ATTRIB;

	if (hasXConstraint || hasYConstraint) {
		return std::make_unique<DisplacementConstraint<double>>(&edge, field.get());
	}

	return nullptr;
};

std::unique_ptr<Constraint<double>> MagAxiStaticAnalysis::makeConstraint(MeshVertex& vertex) {
	auto classification = vertex.getClassification();
	if (!classification) return nullptr;

	bool hasXConstraint = classification->getAttribute("x_constraint") != NO_ATTRIB;
	bool hasYConstraint = classification->getAttribute("y_constraint") != NO_ATTRIB;

	if (hasXConstraint || hasYConstraint) {
		return std::make_unique<DisplacementConstraint<double>>(&vertex, field.get());
	}

	return nullptr;
};

void MagAxiStaticAnalysis::solve() {
	LinearSystemAssembler<double> assembler;
	AlgebraicSystem<double> AS(&DS, &assembler, field.get());
	AS.solve();

	//get vector of nodal displacements from AlgebraicSystem
	BigVector<double> d = *AS.get_d();

	//assign nodal displacements to DOF value
	for (auto node : theMesh->getNodes()) {
		const auto& DOFs = field->getDOFsForNode(*node);
		for (const auto& dof : DOFs) {
			if (dof->get_status() == DOFStatus::Free) {
				if (formulation <= 0) {
					dof->set_value(d[dof->get_eqnumber()]);// * 2 * PI * (*node_iter)->x());
				}
				else if (formulation == 1) {
					dof->set_value(d[dof->get_eqnumber()] * sqrt(node->x()));
				}
				else {
					dof->set_value(d[dof->get_eqnumber()] / node->x());
				}
			}
		}
	}
};

void MagAxiStaticAnalysis::recover() {

#ifdef recover
	MeshFace* face;
	vector<MeshFace*>::iterator face_iter;
	vector<DOF*> DOFs;
	double W = 0;
	/*
		B = Vector(theMesh->numNodes()*2);

		double W = 0;

		int nnd = 2;
		//calculate flux density (curl A)
	*/
	for (face_iter = theMesh->getFirstFace(); face_iter != theMesh->getLastFace(); face_iter++) {
		face = (*face_iter);
		vector<Node*> nodes = face->getNodes();
		int nen = nodes.size();

		ShapeFunction* sf;

		if (face->numEdges() == 3) { //triangle
			if (nen == 3) { //1st order triangle
				sf = new LinTriSF(face);
			}
			else if (nen == 6) { //2nd order triangle
				sf = new QuadTriSF(face);
			}
		}
		else if (face->numEdges() == 4) { //quad
			if (nen == 4) { //1st order quad
				sf = new LinQuadSF(face);
			}
			else if (nen == 9) { //2nd order quad
				sf = new QuadQuadSF(face);
			}
			else {
				cerr << "Unknown element type!" << endl;
			}
		}
		else { //dunno
			cerr << "Uknown element type!" << endl;
			exit(1);
		}

		Mapping* mapping = new Mapping2D(face, sf);
		/*
				Matrix B_n(2, nen);

				vector<double> A_n(nen);
				vector<double> stress_n(nnd);

				for(int n=0;n<nen;n++){
					DOFs = nodes[n]->getDOFs();
					A_n[n] = DOFs[0]->get_value();
				}

				for(int i=0;i<nen;i++){

					point pt = nodes[i]->pt();

					Vector N = sf->N(pt);
					double r = 0;

					for(int n=0;n<nen;n++){
						DOFs = nodes[n]->getDOFs();
						A_n[n] = DOFs[0]->get_value();
						r = r + nodes[n]->x() * N[n];
					}
					//Unlike Na, dNads at x!=a is not 0! (ie. the derivate at a node is a function of the other nodal values).
					//Therefore, B at a node must be evaluated using all nodal values (and hence another loop)
					Matrix dNds = sf->dNds(pt);
					Matrix dsdx = mapping->jacobianInverse(pt);
					Matrix dNdx = dNds * dsdx;

					double B_r = 0;
					double B_z = 0;

					for(int n=0;n<nen;n++){
						B_r = B_r + A_n[n] * dNdx[n][1];
						B_z = B_z + A_n[n] * (dNdx[n][0] + N[n]/r);
					}

					B[(nodes[i]->ID-1)*2] = B_r;
					B[(nodes[i]->ID-1)*2+1] = B_z;

				}
		*/
		if (face->getClassification()->getAttribute("J") != NO_ATTRIB) {
			double J = face->getClassification()->getAttribute("J");
			int nen = face->getNodes().size();  //inefficient...

			vector<point> IntPt = sf->IntPts();
			Vector Weight = sf->Weights();
			int numIntPts = sf->numIntPts();

			for (int i = 0; i < numIntPts; i++) {
				point pt = IntPt[i];
				Vector N = sf->N(pt);
				double weight = Weight[i];
				double r = 0;
				double A = 0;
				for (int n = 0; n < nen; n++) {
					DOFs = nodes[n]->getDOFs();
					double An = DOFs[0]->get_value();
					r = r + nodes[n]->x() * N[n];
					A = A + An * N[n];
				}
				W = W + 2 * PI * r * A * J * mapping->detJacobian(pt) * weight;
			}
			/*
						W=0;

						double r0 = nodes[0]->x();
						double r1 = nodes[1]->x();
						double r2 = nodes[2]->x();
						double z0 = nodes[0]->y();
						double z1 = nodes[1]->y();
						double z2 = nodes[2]->y();
						double D = (r1*z2-r2*z1)+(r2*z0-r0*z2)+(r0*z1-r1*z0);
						double A[3];
						double Ac=0;
						double rc=0;
						for(int n=0;n<3;n++){
							DOFs = nodes[n]->getDOFs();
							A[n] = DOFs[0]->get_value();
							Ac = Ac + A[n];
							rc = rc + nodes[n]->x();
						}
						Ac = Ac/3;
						rc = rc/3;
						W = W + J*Ac*rc*D;
						*/
		}
	}
	double X_pu = PI * 60 * W / 3600;
#endif
};

void MagAxiStaticAnalysis::saveOut(const std::string& filename) {
	//output to file for viewing in gmsh
	std::ofstream outFile;

	//output to file for viewing in gmsh
	outFile.open(filename + "_fluxdensity.pos", ios::out);

	outFile << "View \"output\" {" << std::endl;

	for (const auto& face : theMesh->getFaces()) {
		auto vertexes = face->getVertices();
		if (vertexes.size() == 3) {
			outFile << "VT (";
		}
		else {
			outFile << "VQ (";
		}
		for (auto vertex : vertexes) {
			outFile << vertex->getNode()->x() << "," << vertex->getNode()->y() << "," << vertex->getNode()->z();
			if (vertex != vertexes.back()) {
				outFile << ",";
			}
		}
		outFile << "){";
		for (auto vertex : vertexes) {
			//         outFile << B[((*vertex_iter)->node->ID-1)*2] << "," << B[((*vertex_iter)->node->ID-1)*2+1] << "," << 0;
			if (vertex != vertexes.back()) {
				outFile << ",";
			}
		}
		outFile << "};" << endl;
	}
	outFile << "};" << endl;
	outFile.close();

	//output to file for viewing in gmsh
	outFile.open(filename + "_mvp.pos", ios::out);

	outFile << "View \"output\" {" << std::endl;
	//vector<Node*>::iterator node_iter;
	for (const auto& face : theMesh->getFaces()) {
		const auto& vertexes = face->getVertices();
		if (vertexes.size() == 3) {
			outFile << "ST (";
		}
		else {
			outFile << "SQ (";
		}
		for (const auto& vertex : vertexes) {
			outFile << vertex->getNode()->x() << "," << vertex->getNode()->y() << "," << vertex->getNode()->z();
			if (vertex != vertexes.back()) {
				outFile << ",";
			}
		}
		outFile << "){";
		for (const auto& vertex : vertexes) {
			const auto& DOFs = field->getDOFsForNode(*vertex->getNode());
			outFile << DOFs[0]->get_value();
			//outFile << DOFs[0]->get_value();
			if (vertex != vertexes.back()) {
				outFile << ",";
			}
		}
		outFile << "};" << std::endl;
	}
	outFile << "};" << std::endl;
	outFile.close();
};

