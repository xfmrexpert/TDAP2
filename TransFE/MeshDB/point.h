/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
 
#pragma once

#include <stdexcept>
#include <ostream>

 /// The point class is a container for 3D (and 2D) spatial coordinates.
class point {
public:
    /// Coordinates are public for direct access.
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    /// Default constructor initializes to (0.0, 0.0, 0.0).
    constexpr point() = default;

    /// Constructor for 2D points (z defaults to 0.0).
    constexpr point(double in_x, double in_y) : x(in_x), y(in_y), z(0.0) {}

    /// Constructor for 3D points.
    constexpr point(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {}

    /// Returns the n-th coordinate (0 for x, 1 for y, 2 for z).
    double X(int n) const {
        switch (n) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default:
            throw std::out_of_range("Invalid coordinate index. Must be 0, 1, or 2.");
        }
    }

    /// Equality operator.
    constexpr bool operator==(const point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    /// Inequality operator.
    constexpr bool operator!=(const point& other) const {
        return !(*this == other);
    }

    /// Output stream operator for easy printing.
    friend std::ostream& operator<<(std::ostream& os, const point& pt) {
        os << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
        return os;
    }
};
