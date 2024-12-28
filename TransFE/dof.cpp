/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "dof.h"

size_t ndof;

void DOF::set_value(double dof_value) {
	value = dof_value;
}

double DOF::get_value() {
	return value;
}

void DOF::set_status(DOFStatus dof_status) {

	if (status == DOFStatus::Free && dof_status != DOFStatus::Free) {
		ndof = ndof - 1;
	}
	status = dof_status;
}

DOFStatus DOF::get_status() {
	return status;
}

void DOF::set_eqnumber(size_t eqnumber) {
	eq_number = eqnumber; //probably should check for overflow here
}

size_t DOF::get_eqnumber() {
	return eq_number;
}
