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
    point ptPhys;
    Matrix<double> J;
    Matrix<double> invJ;
    double detJ;
    Matrix<double> dN_dx;
};

struct ElementData
{
    // For each quadrature point, store geometry + shape data
    std::vector<ElementQuadratureData> quadData;
    // Possibly store dof indices, etc.
};

template <typename T>
class FESpaceBase {
public:
    FESpaceBase(Mesh* mesh, std::unique_ptr<FiniteElementBase> fe, std::unique_ptr<IntegrationRule> int_rule) 
        : mesh(mesh), fe(std::move(fe)), int_rule(std::move(int_rule)) {
        
    }

    virtual ~FESpaceBase() = default;

    virtual ElementData computeElementData(MeshEntity& entity) const = 0;

    // Access to the underlying finite element
    virtual const FiniteElementBase* getFiniteElement() const = 0;

    const IntegrationRule* getIntegrationRule() const { return int_rule.get(); }

    const Mesh* getMesh() const { return mesh; }

    // Method to assign equation numbers
    void assignEquationNumbers() {
        ndof = 0;
        for (auto& entityDOFs : DOFs) {
            for (auto& dof : entityDOFs) {
                if (dof->get_status() == DOFStatus::Free) {
                    dof->set_eqnumber(ndof++);
                }
                else {
                    dof->set_eqnumber(0); // Or another sentinel value
                }
            }
        }
    }

    /// Number of global DOFs in this FE space.
    size_t numGlobalDOFs() const { return ndof; }

    virtual void setupGlobalDOFs() = 0;

    virtual inline std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity) = 0;

protected:
    std::unique_ptr<FiniteElementBase> fe;
    Mesh* mesh;
    std::unique_ptr<IntegrationRule> int_rule;
    std::vector<std::vector<std::unique_ptr<DOF<T>>>> DOFs; // Vector of DOFs for each node
    int ndof = 0;
};

