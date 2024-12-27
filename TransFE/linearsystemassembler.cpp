/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "linearsystemassembler.h"
#include <iostream>

void LinearSystemAssembler::accept(Matrix<double> k, std::vector<DOF*> dofs)
{
	//given a stiffness contributor stiffness matrix and list of associated DOF objects
	//add into global matrix
	std::size_t ki, kj, kc;
	std::size_t size = dofs.size();
	for(std::size_t i=0; i<size; i++)  //loop over rows of stiffness contributor
	{
		auto idof = dofs[i]; //get ith degree of freedom from the DOF list
		ki = idof->get_eqnumber(); //get global equation number for row from the DOF object
		//std::cout << "ki=" << ki << std::endl;
		for(std::size_t j=0; j < size; j++) //for the current row, loop over the columns of the local stiffness matrix
		{
			auto jdof = dofs[j]; //get jth degree of freedom from the DOF list
			kj = jdof->get_eqnumber();  //get global equation number for the column from the DOF object
			//std::cout << "kj=" << kj << std::endl;
			if(kj >= ki) //check to make sure this term is in the upper triangle
			{
				//std::cout << "In upper triangle" << std::endl;
				if(jdof->get_status()==DOF_Free && idof->get_status()==DOF_Free) //unconstrained
				{
					//std::cout << "Unconstrained. Adding " << k(i, j) << " to K(" << ki << ", " << kj << ") :" << std::endl;
					(*K).coeffRef(ki,kj) += k(i, j); //add the local stiffness term to the global stiffness matrix
				}
				else //if both not free, then one or both are constrained
				{ 
					//If one of the dofs is constrained as Fixed and the other is free, proper term must
					//go in load vector
					DOF* cdof=nullptr; //we'll need the value of the non-zero essential BC from the DOF
					if(jdof->get_status()==DOF_Free && idof->get_status()==DOF_Fixed) //adds to the kj force term
					{
						//std::cout << "i is constrained" << std::endl;
						kc = kj;
						cdof = idof;
					}
					else if(idof->get_status()==DOF_Free && jdof->get_status()==DOF_Fixed) //adds to the ki force term
					{
						//std::cout << "j is constrained" << std::endl;
						kc = ki;
						cdof = jdof;
					}
					//kc is the equation number, cdof is the constrained dof
					//std::cout << "Stiffness value = " << std::endl << k << std::endl;
					if(cdof!=nullptr)
					{
						//std::cout << "Constained DOF is " << cdof->get_value() << std::endl;
						(*f)[kc] = (*f)[kc] - cdof->get_value()*k(i, j);
						//std::cout << "Updated f[kc]: " << (*f)[kc] << std::endl;
					}
				}
			}
		}
	}
}

void LinearSystemAssembler::accept(Vector<double> local_f, std::vector<DOF*> dofs)
{
	size_t size = dofs.size();
	for(std::size_t i=0; i < size; i++) //loop over rows of force contributor
	{
		auto idof = dofs[i]; //get ith degree of freedom from the DOF list
		auto ki = idof->get_eqnumber(); //get global equation number for row from the DOF object
		if(idof->get_status()==DOF_Free)
		{
			//std::cout << "Adding force term..." << std::endl;
			//std::cout << local_f;
			(*f)[ki] = (*f)[ki] + local_f[i]; //add the local force term to the global force vector
		}
		else
		{ //if not free, then constrained
			//doing nothing...
		}
	}
};
