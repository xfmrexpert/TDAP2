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

#include <cstddef>
#include <type_traits>
#include <complex>

 // Define the Complex type for convenience
using Complex = std::complex<double>;

// Define DOF status constants
#define DOF_STATUS int
#define DOF_Free 1
#define DOF_Zero 2
#define DOF_Fixed 3

extern size_t ndof;

/// This class represents a Degree of Freedom (DOF).
template <typename ValueType>
class DOF {
    // Restrict ValueType to double and Complex
    static_assert(std::is_same_v<ValueType, double> || std::is_same_v<ValueType, Complex>,
        "DOF only supports double or Complex as ValueType");

public:
    /// Default constructor
    DOF() : value(0), status(DOF_Free), eq_number(0) {
        ndof++;
    }

    /// Destructor
    ~DOF() {
        if (status == DOF_Free) {
            ndof--;
        }
    }

    /// Set the value
    void set_value(const ValueType& val) { value = val; }

    /// Get the value
    ValueType get_value() const { return value; }

    /// Set the status
    void set_status(DOF_STATUS s) { status = s; }

    /// Get the status
    DOF_STATUS get_status() const { return status; }

    /// Set the equation number
    void set_eqnumber(size_t eq_num) { eq_number = eq_num; }

    /// Get the equation number
    size_t get_eqnumber() const { return eq_number; }

private:
    ValueType value;    // The value of the DOF
    DOF_STATUS status;  // The status of the DOF
    size_t eq_number;   // The equation number
};


