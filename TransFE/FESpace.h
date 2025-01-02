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
#include "MeshDB/mesh.h"
#include "FiniteElement.h"
#include "ElementTransform.h"
#include "IntegrationRule.h"

struct ElementQuadratureData
{
    Matrix<double> J;      // Jacobian at this quad point
    Matrix<double> invJ;
    double         detJ;
    Matrix<double> dPhiPhys;  // Physical shape function derivatives
    // possibly shape values, other PDE-specific data
};

struct ElementData
{
    // For each quadrature point, store geometry + shape data
    std::vector<ElementQuadratureData> quadData;
    // Possibly store dof indices, etc.
};

 /// FESpace ties a FiniteElement to a Mesh, building a global DOF structure.
template <typename T>
class FESpace
{
public:
	FESpace(Mesh* mesh, std::unique_ptr<FiniteElement> fe, std::unique_ptr<ElementTransform> transform, std::unique_ptr<IntegrationRule> int_rule) 
        : mesh(mesh), fe(std::move(fe)), transform(std::move(transform)), int_rule(std::move(int_rule)) {
        setupGlobalDofs();
    }

    /// Number of global DOFs in this FE space.
    size_t numGlobalDofs() const { return ndof; }

    void setupGlobalDofs() {
        ndof = 0;
        const auto& nodes = mesh->getNodes();
		int nnd = fe->numLocalDOFs();
        DOFs.resize(nodes.size() * nnd);
        for (size_t i = 0; i < nodes.size(); i++) {
            DOFs[i].resize(nnd);
            nodes[i]->setID(i);
            for (size_t j = 0; j < nnd; j++) {
                DOFs[i][j] = std::make_unique<DOF<T>>();
                DOFs[i][j]->set_eqnumber(ndof);
                ndof++;
            }
        }
    }

    inline std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity) {
        std::vector<DOF<T>*> rtnDOFs;
        const auto& nodes = entity.getNodes();
        for (const auto& node : nodes) {
            for (const auto& dof : DOFs[node->getID()]) {
                rtnDOFs.push_back(dof.get());
            }
        }
        return rtnDOFs;
    }

    inline std::vector<DOF<T>*> getDOFsForNode(const Node& node) {
        std::vector<DOF<T>*> rtnDOFs;
        for (const auto& dof : DOFs[node.getID()]) {
            rtnDOFs.push_back(dof.get());
        }
        return rtnDOFs;
    }

    /// Access to the underlying FE object
    const FiniteElement* getFiniteElement() const { return fe.get(); }

	const IntegrationRule* getIntegrationRule() const { return int_rule.get(); }

    ElementData computeElementData(const MeshEntity& entity) const
    {
        ElementData eData;

        // 1. get element's FiniteElement & the MeshEntity with its node coords
        const FiniteElement& fe = *this->fe;

        // 2. For each quad point in 'rule'
        for (int q = 0; q < int_rule->numIntPts(); q++)
        {
            point ptRef = int_rule->IntPts()[q];

            // Compute dPhiRef
            auto dPhiRef = fe.grad_N(ptRef);

            // Build J, detJ, invJ
            auto J = transform->Jacobian(ptRef, entity, dPhiRef);
            double detJ = J.CalculateDeterminant();
            auto invJ = J.CalculateInverse();

            // Transform dPhiRef => dPhiPhys
            Matrix<double> dPhiPhys = transformReferenceToPhysical(dPhiRef, invJ);

            // store
            ElementQuadratureData qd;
            qd.J = J;
            qd.invJ = invJ;
            qd.detJ = detJ;
            qd.dPhiPhys = dPhiPhys;
            eData.quadData.push_back(std::move(qd));
        }

        return eData;
    }

private:
    /// Transform shape-function derivatives from reference to physical coords.
    /// dPhiRef: (nDofs x refDim)
    /// invJ:    (refDim x refDim)
    /// returns: (nDofs x refDim)
    Matrix<double> transformReferenceToPhysical(const Matrix<double>& dPhiRef, const Matrix<double>& invJ) const
    {
        size_t nDofs = dPhiRef.GetNumberOfRows();     // e.g. number of shape functions
        size_t refDim = dPhiRef.GetNumberOfColumns();     // 2 in 2D, or 3 in 3D

        // We'll create an output matrix of the same shape
        Matrix<double> dPhiPhys = Matrix<double>(nDofs, refDim);

        // Multiply each row of dPhiRef by invJ
        // (row-vector) x (refDim x refDim) => (row-vector)
        for (size_t i = 0; i < nDofs; i++)
        {
            for (size_t c = 0; c < refDim; c++)
            {
                // compute dPhiPhys(i, c) by dot product of dPhiRef row i with invJ column c
                double sum = 0.0;
                for (size_t k = 0; k < refDim; k++)
                {
                    sum += dPhiRef(i, k) * invJ(k, c);
                }
                dPhiPhys(i, c) = sum;
            }
        }

        return dPhiPhys;
    }

private:
    Mesh* mesh;
    std::unique_ptr<FiniteElement> fe;
    std::unique_ptr<ElementTransform> transform;
	std::unique_ptr<IntegrationRule> int_rule;

	std::vector<std::vector<std::unique_ptr<DOF<T>>>> DOFs; // Vector of DOFs for each node
    int ndof = 0;
};