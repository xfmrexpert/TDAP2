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

#include "typedefs.h"
#include "FESpace.h"

template <typename T>
class FESpaceH1 : public FESpaceBase<T> {
public:
    // Constructor now accepts FiniteElement<G, F>
    FESpaceH1(Mesh* mesh, std::unique_ptr<FiniteElementBase> fe, std::unique_ptr<IntegrationRule> int_rule)
        : fe(std::move(fe)), FESpaceBase<T>(mesh, std::move(int_rule))
    {
        setupGlobalDOFs();
    }

    void setupGlobalDOFs() override {
        ndof = 0;
        const auto& nodes = this->mesh->getNodes();
        int nnd = 1; // fe->numLocalDOFs();
        this->DOFs.resize(nodes.size() * nnd);
        for (size_t i = 0; i < nodes.size(); i++) {
            this->DOFs[i].resize(nnd);
            nodes[i]->setID(i);
            for (size_t j = 0; j < nnd; j++) {
                this->DOFs[i][j] = std::make_unique<DOF<T>>();
                this->DOFs[i][j]->set_eqnumber(ndof);
                ndof++;
            }
        }
    }

    inline std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity) override {
        std::vector<DOF<T>*> rtnDOFs;
        const auto& nodes = entity.getNodes();
        for (const auto& node : nodes) {
            for (const auto& dof : this->DOFs[node->getID()]) {
                rtnDOFs.push_back(dof.get());
            }
        }
        return rtnDOFs;
    }

    inline std::vector<DOF<T>*> getDOFsForNode(const Node& node) {
        std::vector<DOF<T>*> rtnDOFs;
        for (const auto& dof : this->DOFs[node.getID()]) {
            rtnDOFs.push_back(dof.get());
        }
        return rtnDOFs;
    }

    /// Access to the underlying FE object
    const FiniteElementBase* getFiniteElement() const override { return fe.get(); }

    ElementData computeElementData(MeshEntity& entity) const
    {
        ElementData eData;

        for (int q = 0; q < this->int_rule->numIntPts(); q++)
        {
            ElementQuadratureData qd;

            point ptRef = this->int_rule->IntPts()[q];

            point ptPhys = fe->Transform()->mapReferencePointToPhysical(ptRef, entity);
            qd.ptPhys = ptPhys;

            // Compute Jacobian for geometry
            Matrix<double> J = fe->Transform()->Jacobian(ptRef, entity);
            double detJ = J.determinant();
            auto invJ = J.inverse();

            // Validate Jacobian
            if (detJ <= 0.0 || !std::isfinite(detJ)) {
                detJ = -1 * detJ; // WARNING: THIS IS HACKERY AND ONLY WORKS FOR LINEAR TRI ELEMENTS
                //throw std::runtime_error("Invalid geometry Jacobian determinant at quadrature point " + std::to_string(q) + ".");
            }

            qd.J = J;
            qd.detJ = detJ;
            qd.invJ = invJ;

            // Transform gradients to physical space
            // Attempt to cast to a scalar shape function 
            auto scalar_sf = dynamic_cast<const ScalarShapeFunction*>(fe->ShapeFunction());
            if (!scalar_sf) {
                throw std::runtime_error("FESpaceH1: The provided FE is not scalar (H1).");
            }
            qd.dN_dx = scalar_sf->grad_N(ptRef) * invJ;

            // Store everything into ElementQuadratureData
            eData.quadData.push_back(std::move(qd));
        }

        return eData;
    }

private:
    int ndof = 0;
    std::unique_ptr<FiniteElementBase> fe;
};
