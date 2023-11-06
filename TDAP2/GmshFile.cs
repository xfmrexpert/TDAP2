// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace TDAP
{
    public class GmshFile
    {
        public int ElementOrder {get; set;} = 1;
        public string Filename {get; set;}
        public List<GmshPoint> points = new List<GmshPoint>();
        public List<GmshLine> lines = new List<GmshLine>();
        public List<GmshLineLoop> line_loops = new List<GmshLineLoop>();

        public GmshFile(string filename)
        {
            Filename = filename;
        }

        public void gmshRectangle(double ll_x, double ll_y, double ur_x, double ur_y)
        {
            GmshPoint ll = new GmshPoint(ll_x, ll_y, 0);
            GmshPoint lr = new GmshPoint(ur_x, ll_y, 0);
            GmshPoint ur = new GmshPoint(ur_x, ur_y, 0);
            GmshPoint ul = new GmshPoint(ll_x, ur_y, 0);
            points.Add(ll);
            points.Add(lr);
            points.Add(ur);
            points.Add(ul);
            GmshLine bottom = new GmshLine(ll, lr);
            GmshLine right = new GmshLine(lr, ur);
            GmshLine top = new GmshLine(ur, ul);
            GmshLine left = new GmshLine(ul, ll);
            lines.Add(bottom);
            lines.Add(right);
            lines.Add(top);
            lines.Add(left);
            List<GmshLine> rect_lines = new List<GmshLine>();
            rect_lines.Add(bottom);
            rect_lines.Add(right);
            rect_lines.Add(top);
            rect_lines.Add(left);
            GmshLineLoop rect = new GmshLineLoop(rect_lines);
            line_loops.Add(rect);
        }

        public void writeFile()
        {
            StreamWriter sw = File.CreateText(Filename);
            sw.WriteLine("lc = 0.02;");
            sw.WriteLine("Mesh.ElementOrder = " + ElementOrder + ";");

            int point_ID = 0;
            foreach(GmshPoint point in points){
                point_ID++;
                point.ID = point_ID;
                point.Write(sw);
            }

            int line_ID = 0;
            foreach(GmshLine line in lines){
                line_ID++;
                line.ID = line_ID;
                line.Write(sw);
            }

            int line_loop_ID = 0;
            foreach(GmshLineLoop line_loop in line_loops){
                line_loop_ID++;
                line_loop.ID = line_loop_ID;
                line_loop.Write(sw);
            }

            sw.Write("Plane Surface (1) = {1, ");
            for (int i = 2; i <= line_loop_ID; i++)
            {
                sw.Write("{0}", i);
                if (i < line_loop_ID)
                {
                    sw.Write(", ");
                }
            }
            sw.WriteLine("};");

            for (int i = 2; i <= line_loop_ID; i++)
            {
                sw.WriteLine("Plane Surface ({0}) = {{{1}}};", i, i);
            }

            sw.WriteLine("Physical Point (1) = {1};");

            for (int i = 1; i <= line_loop_ID; i++)
            {
                sw.WriteLine("Physical Surface ({0}) = {{{1}}};", i + 1, i);
            }

            sw.WriteLine("Mesh.MshFileVersion = 2;");
            sw.Close();
        }

        
    }

    public class GmshPoint
    {
        public double x, y, z;
        public int ID;

        public GmshPoint(double in_x, double in_y, double in_z)
        {
            x=in_x;
            y=in_y;
            z=in_z;
            ID=-9999;
        }

        public void Write(StreamWriter sw){
            sw.WriteLine("Point ({0}) = {{{1}, {2}, {3}, lc}};", ID, x, y, z);
        }

    }

    public class GmshLine
    {
        public GmshPoint pt1, pt2;
        public int ID;

        public GmshLine(GmshPoint in_pt1, GmshPoint in_pt2){
            pt1 = in_pt1;
            pt2 = in_pt2;
            ID = -9999;
        }

        public void Write(StreamWriter sw){
            sw.WriteLine("Line ({0}) = {{{1}, {2}}};", ID, pt1.ID, pt2.ID);
        }

    }

    public class GmshLineLoop
    {
        public List<GmshLine> lines;
        public int ID;

        public GmshLineLoop(List<GmshLine> in_lines){
            lines = in_lines;
        }

        public void Write(StreamWriter sw){
            sw.Write("Line Loop ({0}) = {{", ID);
            bool firstLine = true;
            foreach (var line in lines) {
                if(!firstLine){
                    sw.Write(", ");
                }else{
                    firstLine = false;
                }
                sw.Write("{0}", line.ID);
            }
            sw.WriteLine("};");
        }
        
    }

}