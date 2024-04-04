// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Drawing;
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
            GeomPoint? pt = Points.Find(p => p.x == x && p.y == y);
            if (pt == null)
            {
                pt = new GeomPoint(x, y);
                Points.Add(pt);
            }
            return pt;
        }

        public GeomLine AddLine(GeomPoint pt1,  GeomPoint pt2) 
        {
            GeomLine? line = Lines.Find(l => (l.pt1 == pt1 && l.pt2 == pt2) || (l.pt1 == pt2 && l.pt2 == pt1));
            if (line == null)
            {
                line = new GeomLine(pt1, pt2);
                Lines.Add(line);
            }
            return line;
        }

        public GeomArc AddArc(GeomPoint startPt, GeomPoint endPt, double radius, double sweepAngle)
        {
            GeomArc? arc = Arcs.Find(a => (a.StartPt == startPt && a.EndPt == endPt && a.Radius == radius && a.SweepAngle == sweepAngle) ||
                                          (a.StartPt == endPt && a.EndPt == startPt && a.Radius == radius && a.SweepAngle == -sweepAngle));
            if (arc == null)
            {
                arc = new GeomArc(startPt, endPt, radius, sweepAngle);
                Arcs.Add(arc);
            }
            return arc;
        }

        public GeomLineLoop AddLineLoop(params GeomEntity[] entities)
        {
            //TODO: Check for duplicate line loop
            var LineLoop = new GeomLineLoop(entities.ToList<GeomEntity>());
            LineLoops.Add(LineLoop);
            return LineLoop;
        }

        public GeomSurface AddSurface(GeomLineLoop boundary, params GeomLineLoop[] holes)
        {
            //TODO: Check for duplicate surface
            GeomSurface surface = new GeomSurface(boundary, holes);
            Surfaces.Add(surface);
            return surface;
        }

        public GeomLineLoop AddRoundedRectangle(double x_center, double y_center, double h, double w, double corner_radius=0)
        {
            if (corner_radius == 0)
            {
                return AddRectangle(x_center, y_center, h, w);
            }

            double ll_x = x_center - w / 2d;
            double ll_y = y_center - h / 2d;
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
            // add boundary loop
            var boundary = AddLineLoop(left, upper_left, top, upper_right, right, lower_right, bottom, lower_left);
            return boundary;
        }

        public GeomLineLoop AddRectangle(double x_center, double y_center, double h, double w)
        {
            double ll_x = x_center - w / 2d;
            double ll_y = y_center - h / 2d;
            var LL = AddPoint(ll_x, ll_y);
            var UL = AddPoint(ll_x, ll_y + h);
            var UR = AddPoint(ll_x + w, ll_y + h);
            var LR = AddPoint(ll_x + w, ll_y);
            // add lines for sides
            var left = AddLine(LL, UL);
            var top = AddLine(UL, UR);
            var right = AddLine(UR, LR);
            var bottom = AddLine(LR, LL);
            // add boundary loop
            var boundary = AddLineLoop(left, top, right, bottom);
            return boundary;
        }

        public (double x, double y) GetBounds()
        {
            double maxX = double.MinValue;
            foreach (var pt in Points)
            {
                if (pt.x > maxX) maxX = pt.x;
            }

            double maxY = double.MinValue;
            foreach (var pt in Points)
            {
                if (pt.y > maxY) maxY = pt.y;
            }

            return (maxX, maxY);
        }
    }
}
