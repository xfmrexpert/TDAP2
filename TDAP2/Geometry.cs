// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class Geometry
    {
        public List<GeomPoint> Points { get; private set; } = new List<GeomPoint>();

        public List<GeomLine> Lines { get; private set; } = new List<GeomLine>();

        public List<GeomArc> Arcs { get; private set; } = new List<GeomArc>();

        public Geometry() { }

        public GeomPoint AddPoint(double x, double y)
        {
            GeomPoint pt = new GeomPoint(x, y);
            Points.Add(pt);
            return pt;
        }

        public GeomLine AddLine(GeomPoint pt1,  GeomPoint pt2) 
        {
            GeomLine line = new GeomLine(pt1, pt2);
            Lines.Add(line);
            return line;
        }

        public GeomArc AddArc(GeomPoint startPt, GeomPoint endPt, double radius, double sweepAngle)
        {
            GeomArc arc = new GeomArc(startPt, endPt, radius, sweepAngle);
            Arcs.Add(arc);
            return arc;
        }
    }
}
