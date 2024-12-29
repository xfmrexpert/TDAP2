/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

enum class DOFStatus {
	Free = 1,
	Zero = 2,
	Fixed = 3
};

extern size_t ndof;

/// This class represents a Degree of Freedom (DOF). 
template <class T>
class DOF {

public:

	///Default constructor
	DOF() {
		status = DOFStatus::Free;
		eq_number = 0;
		ndof++;
	}

	~DOF() {
		if (status == DOFStatus::Free) {
			ndof--;
		}
	}

	void set_value(T dof_value)
	{
		value = dof_value;
	}

	T get_value() {
		return value;
	}

	void set_status(DOFStatus dof_status) {

		if (status == DOFStatus::Free && dof_status != DOFStatus::Free) {
			ndof = ndof - 1;
		}
		status = dof_status;
	}

	DOFStatus get_status() {
		return status;
	}

	void set_eqnumber(size_t eqnumber) {
		eq_number = eqnumber; //probably should check for overflow here
	}

	size_t get_eqnumber() {
		return eq_number;
	}

protected:

private:
	T value = 0;
	DOFStatus status;
	size_t eq_number;  //the number of nodes could be limited by the size of this variable

};

