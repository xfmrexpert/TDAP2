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

template<class T>
void DOF<T>::set_value(T dof_value)
{
	value = dof_value;
}

template<class T>
T DOF<T>::get_value() {
	return value;
}

template<class T>
void DOF<T>::set_status(DOFStatus dof_status) {

	if (status == DOFStatus::Free && dof_status != DOFStatus::Free) {
		ndof = ndof - 1;
	}
	status = dof_status;
}

template<class T>
DOFStatus DOF<T>::get_status() {
	return status;
}

template<class T>
void DOF<T>::set_eqnumber(size_t eqnumber) {
	eq_number = eqnumber; //probably should check for overflow here
}

template<class T>
size_t DOF<T>::get_eqnumber() {
	return eq_number;
}
