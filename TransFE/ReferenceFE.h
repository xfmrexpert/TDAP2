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

// Shape functions
// How to handle transformation to/from reference element
// How to handle integration

// Don't know yet where I'm going with this class.  The general idea is to 
// encapsulate the shape functions and their derivatives, plus any other
// information specific to the reference element.  It is possible this is
// already fully encapsulated in the ShapeFunction class.  Nedelec elements
// may change the calculus a bit, so perhaps a reference element class will
// be useful.

class ReferenceFE {

public:
	ReferenceFE(int ndof, int order);
	virtual ~ReferenceFE() {}
	int get_ndof() { return ndof; }
	int get_order() { return order; }
	virtual double get_phi(int i, double x) = 0;
	virtual double get_dphi(int i, double x) = 0;
	
protected:
	int ndof;
	int order;
};
