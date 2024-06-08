// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class FEMMFile
    {
        List<FEMMPoint> points = new List<FEMMPoint>();
        List<FEMMLine> lines = new List<FEMMLine>();
        List<FEMMArc> arcs = new List<FEMMArc>();

        public void CreateFromGeometry(Geometry geometry)
        {
            points.Clear();
            lines.Clear();
            arcs.Clear();

            foreach (var point in geometry.Points)
            {
                CreateNewPoint(point.x, point.y);
            }

            foreach (var line in geometry.Lines)
            {
                
            }

            foreach (var arc in geometry.Arcs)
            {
                
            }

            foreach (var loop in geometry.LineLoops)
            {
                
            }

            foreach (var surface in geometry.Surfaces)
            {
                
            }
        }

        public FEMMPoint CreateNewPoint(double x, double y)
        {
            return new FEMMPoint(x, y);
        }
    }

    public class FEMMPoint
    {
        double x, y;

        public FEMMPoint(double x, double y)
        {
            this.x = x;
            this.y = y;
        }
    }

    public class FEMMLine
    {

    }

    public class FEMMArc
    {

    }
}
