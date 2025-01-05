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
    Matrix<double> geom_J;
    Matrix<double> geom_invJ;
    double geom_detJ;
    Matrix<double> geom_dN_dx;

    // Solution-specific Jacobian details
    Matrix<double> sol_J;
    double sol_detJ;
    Matrix<double> sol_invJ;
    Matrix<double> sol_dN_dx;
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
    virtual ~FESpaceBase() = default;

    // Pure virtual functions to be implemented by derived classes
    virtual size_t numGlobalDofs() const = 0;
    virtual ElementData computeElementData(const MeshEntity& entity) const = 0;

    // Access to the underlying finite element
    virtual const FiniteElementBase* getFiniteElement() const = 0;

    virtual const IntegrationRule* getIntegrationRule() const = 0;

    virtual std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity) = 0;

    virtual const Mesh* getMesh() const = 0;

    virtual void setupGlobalDofs() = 0;

    virtual std::vector<DOF<T>*> getDOFsForNode(const Node& node) = 0;

    virtual void numberDOFs() = 0;

    // Other common functionalities can be added here
};

 /// FESpace ties a FiniteElement to a Mesh, building a global DOF structure.
template <typename G, typename F, typename T>
requires std::derived_from<G, ShapeFunction>&& std::derived_from<F, ShapeFunction>
class FESpace : public FESpaceBase<T> {
public:
    // Constructor now accepts FiniteElement<G, F>
    FESpace(Mesh* mesh, std::unique_ptr<FiniteElement<G, F>> fe,
        std::unique_ptr<ElementTransform> transform,
        std::unique_ptr<IntegrationRule> int_rule)
        : mesh(mesh), fe(std::move(fe)),
        transform(std::move(transform)),
        int_rule(std::move(int_rule))
    {
        setupGlobalDofs();
    }

    /// Number of global DOFs in this FE space.
    size_t numGlobalDofs() const { return ndof; }

    void setupGlobalDofs() {
        ndof = 0;
        const auto& nodes = mesh->getNodes();
        int nnd = 1; // fe->numLocalDOFs();
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

    void numberDOFs() {
        size_t labeldof = 0;
        const auto& nodes = mesh->getNodes();
        for (const auto& node : nodes) {
            const auto& nodeDOFs = DOFs[node->getID()];
            for (const auto& dof : nodeDOFs) {
                if (dof->get_status() == DOFStatus::Free) {
                    dof->set_eqnumber(labeldof);
                    labeldof++;
                }
            }
        }
    };

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

	const Mesh* getMesh() const { return mesh; }

    /// Access to the underlying FE object
    const FiniteElement<G, F>* getFiniteElement() const { return fe.get(); }

	const IntegrationRule* getIntegrationRule() const { return int_rule.get(); }

    ElementData computeElementData(const MeshEntity& entity) const
    {
        ElementData eData;

        bool isIsoparametric = std::is_same_v<G, F>;

        for (int q = 0; q < int_rule->numIntPts(); q++)
        {
            ElementQuadratureData qd;

            point ptRef = int_rule->IntPts()[q];
            point ptPhys = transform->transformReferenceToPhysical(ptRef, entity, fe->Geom()->N(ptRef));

            auto geom_dN_ds = fe->Geom()->grad_N(ptRef);

            auto geom_J = transform->Jacobian(ptRef, entity, geom_dN_ds);
            double geom_detJ = geom_J.determinant();
            auto geom_invJ = geom_J.inverse();

            Matrix<double> sol_dN_dx;
            Matrix<double> geom_dN_dx;

            if (isIsoparametric)
            {
                sol_dN_dx = transform->transformReferenceToPhysical(fe->Sol()->grad_N(ptRef), geom_invJ);
                geom_dN_dx = sol_dN_dx; // Same as solution
            }
            else
            {
                auto sol_dN_ds = fe->Sol()->grad_N(ptRef);
                auto sol_J = transform->Jacobian(ptRef, entity, sol_dN_ds);
                double sol_detJ = sol_J.determinant();
                auto sol_invJ = sol_J.inverse();

                sol_dN_dx = transform->transformReferenceToPhysical(sol_dN_ds, sol_invJ);
                geom_dN_dx = transform->transformReferenceToPhysical(geom_dN_ds, geom_invJ);

                // Optionally store solution Jacobian details
                qd.sol_J = sol_J;
                qd.sol_detJ = sol_detJ;
                qd.sol_invJ = sol_invJ;
            }

            // Store geometry Jacobian details
            qd.geom_J = geom_J;
            qd.geom_invJ = geom_invJ;
            qd.geom_detJ = geom_detJ;
            qd.geom_dN_dx = geom_dN_dx;

            // Store transformed solution gradients
            qd.sol_dN_dx = sol_dN_dx;

            // Store physical point
            qd.ptPhys = ptPhys;
            
            eData.quadData.push_back(qd);
        }

        return eData;
    }


private:
    Mesh* mesh;
    std::unique_ptr<FiniteElement<G, F>> fe; // Now templated on G and F
    std::unique_ptr<ElementTransform> transform;
    std::unique_ptr<IntegrationRule> int_rule;

    std::vector<std::vector<std::unique_ptr<DOF<T>>>> DOFs; // Vector of DOFs for each node
    int ndof = 0;
};