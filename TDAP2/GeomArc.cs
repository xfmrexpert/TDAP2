// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class GeomArc
    {
        public GeomPoint StartPt { get; set; }
        public GeomPoint EndPt { get; set; }
        public double Radius { get; set; }
        public double SweepAngle { get; set; }

        public GeomArc(GeomPoint startPt, GeomPoint endPt, double radius, double sweepAngle)
        {
            StartPt = startPt;
            EndPt = endPt;
            Radius = radius;
            SweepAngle = sweepAngle;
        }   
    }
}
