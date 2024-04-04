// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    // This is presently configured for a circular arc segment.  An open question is whether to extend to an elliptical arc segment.
    // Note that SweepAngle is assumed to proceed clockwise (which may actually be counter-intuitive)
    // Possible TODO: Allow for start point, center point, end point definition of circular arc
    // TODO: Only need start or end point, not both, if sweep angle is given.
    public class GeomArc : GeomEntity
    {
        public GeomPoint StartPt { get; set; }
        public GeomPoint EndPt { get; set; }
        public double Radius { get; set; }
        public double SweepAngle { get; set; } //Clockwise

        public GeomArc(GeomPoint startPt, GeomPoint endPt, double radius, double sweepAngle)
        {
            StartPt = startPt;
            EndPt = endPt;
            Radius = radius;
            SweepAngle = sweepAngle;
        }   
    }
}
