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

	void set_value(T);
	T get_value();

	void set_status(DOFStatus);
	DOFStatus get_status();

	void set_eqnumber(size_t);
	size_t get_eqnumber();

protected:

private:
	T value = 0;
	DOFStatus status;
	size_t eq_number;  //the number of nodes could be limited by the size of this variable

};

