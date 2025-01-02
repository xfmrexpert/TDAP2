#pragma once
#include "typedefs.h"
#include "FESpace.h"
#include "ElementTransform.h"
#include "integrationrule.h"
#include "linearsystemassembler.h"

class BilinearForm
{
public:
	BilinearForm(FESpace<double>* feSpace, Assembler<double>* assem) : feSpace(feSpace), assem(assem)
    {
        int nDofs = feSpace->numGlobalDofs();
    };

    void evaluate(const MeshEntity& entity)
    {
        // Suppose we have nDofs shape functions
		const auto& fe = feSpace->getFiniteElement();
        int nDofs = fe->numLocalDOFs();

        Matrix<double> Ke = Matrix<double>(nDofs, nDofs);

		const auto& integration = feSpace->getIntegrationRule();

		const auto& quadPointsRef = integration->IntPts();
		const auto& quadWeights = integration->Weights();

		const double mu_r = entity.getClassification()->getAttribute("mu_r");
		const double mu = 4.0 * PI * 1.0e-7 * mu_r;

        // For each quadrature point in the reference domain
        for (size_t q = 0; q < integration->numIntPts(); q++)
        {
            point ptRef = quadPointsRef[q];
            double wq = quadWeights(q);

            // 1. Evaluate shape function derivatives in reference coords
            //    => returns an (nDofs x nRefDims) matrix
            Matrix<double> dPhiRef = fe->grad_N(ptRef);

			const auto& element_data = feSpace->computeElementData(entity);
            // At each quadrature point, we need dPhiPhys and detJ
			Matrix<double> dPhiPhys = element_data.quadData[q].dPhiPhys;
            double detJ = element_data.quadData[q].detJ;

            // for i,j
            for (int i = 0; i < nDofs; i++)
            {
                for (int j = 0; j < nDofs; j++)
                {
                    double dotVal = dPhiPhys.GetRow(i).dot(dPhiPhys.GetRow(j));
                    Ke(i, j) += (1 / mu) * dotVal * wq * detJ;
                }
            }
        }

        const auto& DOFs = feSpace->getDOFsForEntity(entity);
        assem->accept(Ke, DOFs);
    }

private:
    FESpace<double>* feSpace;
	Assembler<double>* assem;
};