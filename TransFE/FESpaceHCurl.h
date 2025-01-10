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

 /// FESpace for H(curl) edge (Nedelec) finite element spaces
template <typename T>
class FESpaceHCurl : public FESpaceBase<T> {
public:
    // Constructor now accepts FiniteElement<G, F>
    FESpaceHCurl(Mesh* mesh, std::unique_ptr<FiniteElementBase> fe, std::unique_ptr<IntegrationRule> int_rule)
        : fe(std::move(fe)), FESpaceBase<T>(mesh, std::move(int_rule))
    {
        setupGlobalDofs();
    }

    void setupGlobalDofs() override {
        ndof = 0;
        const auto& edges = this->mesh->getEdges();
        int nnd = 1; // fe->numLocalDOFs();
        this->DOFs.resize(edges.size() * nnd);
        for (size_t i = 0; i < edges.size(); i++) {
            this->DOFs[i].resize(nnd);
            edges[i]->setID(i);
            for (size_t j = 0; j < nnd; j++) {
                this->DOFs[i][j] = std::make_unique<DOF<T>>();
                this->DOFs[i][j]->set_eqnumber(ndof);
                ndof++;
            }
        }
    }

    void numberDOFs() override {
        size_t labeldof = 0;
        const auto& edges = this->mesh->getEdges();
        for (const auto& edge : edges) {
            const auto& edgeDOFs = this->DOFs[edge->getID()];
            for (const auto& dof : edgeDOFs) {
                if (dof->get_status() == DOFStatus::Free) {
                    dof->set_eqnumber(labeldof);
                    labeldof++;
                }
            }
        }
    };

    inline std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity) override {
        std::vector<DOF<T>*> rtnDOFs;
        const auto& edges = entity.getEdges();
        for (const auto& edge : edges) {
            for (const auto& dof : this->DOFs[edge->getID()]) {
                rtnDOFs.push_back(dof.get());
            }
        }
        return rtnDOFs;
    }

    /// Access to the underlying FE object
    const FiniteElementBase* getFiniteElement() const { return fe.get(); }

    ElementData computeElementData(MeshEntity& entity) const
    {
        ElementData eData;

        for (int q = 0; q < this->int_rule->numIntPts(); q++)
        {
            ElementQuadratureData qd;

            point ptRef = this->int_rule->IntPts()[q];
            //const auto& geomShapeFunctions = fe->Transform()->N(ptRef);

            point ptPhys = fe->Transform()->mapReferencePointToPhysical(ptRef, entity);
            qd.ptPhys = ptPhys;

            // Compute gradients in reference space
            //auto geom_dN_ds = fe->Transform()->grad_N(ptRef);

            // Compute Jacobian for geometry using the specialized transform
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
            qd.dN_dx = fe->ShapeFunction()->grad_N(ptRef) * invJ;

            // Store everything into ElementQuadratureData
            eData.quadData.push_back(std::move(qd));
        }

        return eData;
    }

private:
    std::unique_ptr<FiniteElementBase> fe;
};

