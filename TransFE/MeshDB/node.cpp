/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "node.h"

bool operator== (Node& lhs, Node& rhs) {
	if (lhs.x() == rhs.x()) {
		if (lhs.y() == rhs.y()) {
			if (lhs.z() == rhs.z()) {
				return true;
			}
		}
	}
	return false;
}

bool operator< (Node& lhs, Node& rhs) {
	if (lhs.x() < rhs.x()) {
		return true;
	}
	else {
		return false;
	}
}

