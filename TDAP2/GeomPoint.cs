// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class GeomPoint
    {
        public double x { get; set; }
        public double y { get; set; }

        public GeomPoint(double x, double y)
        {
            this.x = x;
            this.y = y;
        }
    }
}
