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

#define DOF_STATUS int
#define DOF_Free 1
#define DOF_Zero 2
#define DOF_Fixed 3

extern size_t ndof;

/// This class represents a Degree of Freedom (DOF). 

class DOF {

public:

	///Default constructor
	DOF() {
		value = 0;
		status = DOF_Free;
		eq_number = 0;
		ndof++;
	}

	~DOF() {
		if (status == DOF_Free) {
			ndof--;
		}
	}

	void set_value(double);
	double get_value();

	void set_status(DOF_STATUS);
	DOF_STATUS get_status();

	void set_eqnumber(size_t);
	size_t get_eqnumber();

protected:

private:

	double value;
	DOF_STATUS status;
	size_t eq_number;  //the number of nodes could be limited by the size of this variable

};


