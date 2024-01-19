// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class GeomLine
    {
        public GeomPoint pt1 { get; set; }
        public GeomPoint pt2 { get; set; }

        public GeomLine(GeomPoint pt1, GeomPoint pt2)
        {
            this.pt1 = pt1;
            this.pt2 = pt2;
        }
    }
}
