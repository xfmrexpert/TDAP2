// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public static class DoubleExtensions
    {
        /// <summary>Compare two doubles taking in account the double precision potential error.</summary>
        /// <remarks>
        /// This method works fine for floats. That's because all the float values can be represented by a double value,
        /// because they're "built" in the same way, just with more possible digits.
        /// </remarks>
        public static bool AboutEquals(this float value1, double value2) => AboutEquals((double)value1, value2);

        /// <summary>Compare two doubles taking in account the double precision potential error.</summary>
        /// <remarks>
        /// This method works fine for floats. That's because all the float values can be represented by a double value,
        /// because they're "built" in the same way, just with more possible digits.
        ///
        /// Also bare in mind that truncation errors accumulate on calculation. The more you do, the more you should increase the epsilon.
        /// You get really better performance when you can determine the contextual epsilon first.
        /// </remarks>
        /// <param name="value1">The first value</param>
        /// <param name="value2">The second value</param>
        /// <param name="precalculatedContextualEpsilon">The precalculated-contextual-epsilon. Use <see cref="GetContextualEpsilon"/> for this.</param>
        /// <returns>true if the two values fall within the given margin, false otherwise</returns>
        public static bool AboutEquals(this double value1, double value2, double? precalculatedContextualEpsilon = null)
        {
            precalculatedContextualEpsilon ??= Math.Max(Math.Abs(value1), Math.Abs(value2)).GetContextualEpsilon();

            return Math.Abs(value1 - value2) <= precalculatedContextualEpsilon;
        }

        public static double GetContextualEpsilon(this double biggestPossibleContextualValue) => biggestPossibleContextualValue * 1E-15;
    }
}
