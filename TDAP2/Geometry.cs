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

        public List<GeomLineLoop> LineLoops { get; private set; } = new List<GeomLineLoop>();

        public List<GeomSurface> Surfaces { get; private set; } = new List<GeomSurface>();

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

        public GeomLineLoop AddLineLoop(params GeomEntity[] entities)
        {
            var LineLoop = new GeomLineLoop(entities.ToList<GeomEntity>());
            LineLoops.Add(LineLoop);
            return LineLoop;
        }

        public GeomSurface AddSurface(GeomLineLoop boundary, List<GeomLineLoop>? holes = null)
        {
            GeomSurface surface = new GeomSurface(boundary, holes);
            Surfaces.Add(surface);
            return surface;
        }

        public void AddRoundedRectangle(double ll_x, double ll_y, double h, double w, double corner_radius=0)
        {
            var LL1 = AddPoint(ll_x + corner_radius, ll_y);
            var LL2 = AddPoint(ll_x, ll_y + corner_radius);
            var UL1 = AddPoint(ll_x, ll_y + h - corner_radius);
            var UL2 = AddPoint(ll_x + corner_radius, ll_y + h);
            var UR1 = AddPoint(ll_x + w - corner_radius, ll_y + h);
            var UR2 = AddPoint(ll_x + w, ll_y + h - corner_radius);
            var LR1 = AddPoint(ll_x + w, ll_y + corner_radius);
            var LR2 = AddPoint(ll_x + w - corner_radius, ll_y);
            // add lines for sides
            var left = AddLine(LL2, UL1);
            var top = AddLine(UL2, UR1);
            var right = AddLine(UR2, LR1);
            var bottom = AddLine(LR2, LL1);
            // add arcs for corners
            var upper_left = AddArc(UL1, UL2, corner_radius, Math.PI/2d);
            var upper_right = AddArc(UR1, UR2, corner_radius, Math.PI / 2d);
            var lower_right = AddArc(LR1, LR2, corner_radius, Math.PI / 2d);
            var lower_left = AddArc(LL1, LL2, corner_radius, Math.PI / 2d);
            // add surface
            var surface = AddSurface();
        }

        public void AddRectangle(double ll_x, double ll_y, double h, double w)
        {
            var LL = AddPoint(ll_x, ll_y);
            var UL = AddPoint(ll_x, ll_y + h);
            var UR = AddPoint(ll_x + w, ll_y + h);
            var LR = AddPoint(ll_x + w, ll_y);
            // add lines for sides
            var left = AddLine(LL, UL);
            var top = AddLine(UL, UR);
            var right = AddLine(UR, LR);
            var bottom = AddLine(LR, LL);
        }
    }
}
