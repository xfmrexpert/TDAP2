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

#include "BilinearFormIntegrator.h"

class AxiMagBilinearIntegrator : public BilinearFormIntegrator<double>
{
public:
	using BilinearFormIntegrator<double>::BilinearFormIntegrator;

    Matrix<double> evaluatePt(point ptRef, const FiniteElementBase& fe, MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        size_t nDofs = fe.numLocalDOFs();

        Matrix<double> Ke = Matrix<double>::Zero(nDofs, nDofs);

        const double mu_r = entity.getClassification()->getAttribute("mu");
        const double mu = 4.0 * PI * 1.0e-7 * mu_r;

        const double sigma = entity.getClassification()->getAttribute("sigma");

        double s_phys = quadData.ptPhys.x;

        // At each quadrature point, we need dPhiPhys and detJ
        Matrix<double> dN_dx = quadData.dN_dx;

		//std::cout << "dN_dx: \n" << dN_dx << std::endl;

        double detJ = quadData.detJ;

        // Attempt to cast to a scalar shape function 
        auto scalar_sf = dynamic_cast<const ScalarShapeFunction*>(fe.ShapeFunction());
        if (!scalar_sf) {
            throw std::runtime_error("AxiMagBilinearIntegrator: The provided FE is not scalar (H1).");
        }

        const auto& N = scalar_sf->N(ptRef); // shape function values

        for (int i = 0; i < nDofs; i++)
        {
            // partial derivatives wrt s,z
            double dWs_i = dN_dx(i, 0); // partial wrt s
            double dWz_i = dN_dx(i, 1); // partial wrt z

            for (int j = 0; j < nDofs; j++)
            {
                double dWs_j = dN_dx(j, 0);
                double dWz_j = dN_dx(j, 1);
                
                Ke(i, j) += (1 / mu) * (4 * s_phys * dWs_i * dWs_j + dWz_i * dWz_j) * detJ;

            }
        }
        //std::cout << "Ke: " << std::endl;
        //std::cout << Ke << std::endl;
		return Ke;
    };

private:

    double m_omega = 2 * PI * 60.0; // angular freq, e.g. 2*pi*f
};
