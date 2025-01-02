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

#include <list>
#include "LinearFormIntegrator.h"

template <typename T>
class LinearForm
{
public:

	LinearForm(FESpace<T>* fe_space) : fe_space(fe_space) { };

	void addIntegrator(std::unique_ptr<LinearFormIntegrator> integrator)
	{
		integrators.push_back(std::move(integrator));
	}

	void Assemble(Assembler<T>& assem)
	{
		for (const auto& entity : fe_space->getMesh()->getEntities())
		{
			for (auto& integrator : integrators)
			{
				integrator->evaluate(*entity, assem);
			}
		}
	}

protected:
	FESpace<T>* fe_space;
	std::list<std::unique_ptr<LinearFormIntegrator>> integrators;

};
