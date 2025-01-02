/***************************************************************************
 *   Copyright (C) 2005-2025 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   Use of this source code is governed by an MIT-style                   *
 *   license that can be found in the LICENSE.txt file or at               *
 *   https://opensource.org/licenses/MIT.                                  *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "FEAnalysis.h"
#include "displacementconstraint.h"
#include "LinearSystemAssembler.h"
#include "AlgebraicSystem.h"

class MagAxiStaticAnalysis : public FEAnalysis<double> {
public:
	MagAxiStaticAnalysis(int form);

	~MagAxiStaticAnalysis() = default;

	virtual void solve() override;

	virtual void recover() override; //recover them all important secondary variables

	virtual std::unique_ptr<Constraint<double>> makeConstraint(MeshEdge& edge) override;
	virtual std::unique_ptr<Constraint<double>> makeConstraint(MeshVertex& vertex) override;

	/// Create the appropriate Constraint for a face
	virtual std::unique_ptr<Constraint<double>> makeConstraint(MeshFace& face) override { return nullptr; };

	virtual void saveOut(const std::string& filename) override;

	int formulation;

protected:
	Vector<double> B;
	LinearSystemAssembler<double> assembler;

};
