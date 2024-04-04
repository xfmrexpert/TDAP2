// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class GeomSurface : GeomEntity
    {
        public GeomLineLoop Boundary { get; set; }

        public List<GeomLineLoop>? Holes { get; set; } = new List<GeomLineLoop>();

        public GeomSurface(GeomLineLoop boundary, params GeomLineLoop[] holes)
        {
            Boundary = boundary;
            Holes = holes.ToList();
        }
    }
}
