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

    Matrix<double> evaluatePt(point ptRef, const FiniteElement& fe, const MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        int nDofs = fe.numLocalDOFs();

        Matrix<double> Ke = Matrix<double>(nDofs, nDofs);

        const double mu_r = entity.getClassification()->getAttribute("mu");
        const double mu = 4.0 * PI * 1.0e-7 * mu_r;

        const double sigma = entity.getClassification()->getAttribute("sigma");

        double r_phys = quadData.ptPhys.x;

        // At each quadrature point, we need dPhiPhys and detJ
        Matrix<double> dPhi_dx = quadData.dPhiPhys;

		//std::cout << "dPhiPhys: \n" << dPhiPhys << std::endl;

        double detJ = quadData.detJ;

        // The measure factor for axisymmetric integrals: r * detJ * wq
        double measure = r_phys * detJ;

        const auto& Phi = fe.N(ptRef); // shape function values

        for (int i = 0; i < nDofs; i++)
        {
            // partial derivatives wrt r,z
            double dAr_i = dPhi_dx(i, 0); // partial wrt r
            double dAz_i = dPhi_dx(i, 1); // partial wrt z

            double val_i = Phi(i); // shape function value for A_phi

            for (int j = 0; j < nDofs; j++)
            {
                double dAr_j = dPhi_dx(j, 0);
                double dAz_j = dPhi_dx(j, 1);
                double val_j = Phi(j);

                // 1) Grad dot grad part: mu^-1 * (dr_i * dr_j + dz_i * dz_j)
                double gradTerm = (1 / mu) * (dAr_i * dAr_j + dAz_i * dAz_j);
				//double gradTerm = (1 / mu) * ( (dAr_i * dAr_j + dAz_i * dAz_j) + (val_i * dAr_j + val_j * dAr_i) / r_phys + val_i * val_j / (r_phys * r_phys) );
                /*auto dNdx = dPhiPhys;
				auto N = phi;
                double gradTerm = (1 / mu) * (dNdx(i, 0) * dNdx(j, 0) + dNdx(i, 1) * dNdx(j, 1));
                gradTerm = gradTerm / r_phys;*/

                // 2) The "1/r^2" term => - mu^-1 * A_phi * w / r^2
                double r2Term = -(1 / mu) * (val_i * val_j) / (r_phys * r_phys);

                // 3) The conduction / eddy current term => - j * omega * sigma * A_phi * w
                // i.e.  - j * omega * sigma * (val_i * val_j)
                std::complex<double> cplxTerm(0.0, -m_omega * sigma * val_i * val_j);

				//std::cout << "DOF: " << i << ", " << j << " k: " << gradTerm * r_phys << std::endl;
				//std::cout << "r2Term: " << r2Term << std::endl;

                // Combine the real contributions for the local stiffness
                double realContrib = (gradTerm + r2Term) * measure;
				Ke(i, j) += ((1 / mu) * (dPhi_dx(i, 0) * dPhi_dx(j, 0) + dPhi_dx(i, 1) * dPhi_dx(j, 1)) - (1 / mu) * Phi(i) * Phi(j) / (r_phys * r_phys)) * measure;

            }
        }

		return Ke;
    };

private:

    double m_omega = 2 * PI * 60.0; // angular freq, e.g. 2*pi*f
};
