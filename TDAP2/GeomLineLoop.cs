// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class GeomLineLoop : GeomEntity
    {
        public List<GeomEntity> Boundary { get; set; } = new List<GeomEntity>();

        public GeomLineLoop(List<GeomEntity> boundary)
        {
            Boundary = boundary;
        }
    }
}
