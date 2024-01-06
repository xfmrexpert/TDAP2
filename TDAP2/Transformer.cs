// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using Newtonsoft.Json;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System;
using System.Runtime.InteropServices;

namespace TDAP
{
    public class TransMFEM
    {
        [DllImport("TransMFEM.dll", CharSet = CharSet.Auto)]
        public static extern int run_calcs();
    }

    public class Transformer
    {
        public int NumPhases {get; set;} = 1;
        public double Dist_WdgTank { get; set; } = 10.0;

        public Core Core { get; set; } = new Core();
        
        public List<Winding> Windings {get; set;} = new List<Winding>();

        private Mesh mesh = new Mesh();

        [JsonIgnore]
        public Mesh Mesh { get { return mesh;}}

        public Transformer() {}

        public Winding AddNewWinding()
        {
            Winding newWinding = new Winding(this);
            Windings.Add(newWinding);
            return newWinding;
        }

        public void WriteWindingsGmsh(GmshFile outFile){
            foreach (var winding in Windings) {
                winding.WriteSegmentsGmsh(outFile);
            }
        }

        public void writeTransformerGmsh(string filename){

            GmshFile outFile = new GmshFile(filename);
            outFile.ElementOrder = 1;
            //This is ugly, but necessary...
            //Iterate through all sections to find outermost section
            double max_sec_radius = 0;
            foreach (var winding in Windings) {
                foreach (var segment in winding.Segments) {
                    if(segment.radius_outer > max_sec_radius) {
                        max_sec_radius = segment.radius_outer;
                    }
                }
            }

            //Create reactangle for core window
            //outFile->gmshRectangle(core->radius_leg, 0, core->radius_leg + max_sec_width + dist_WdgTnk, core->window_height);
            //Have to do this manually, because we want the point for the boundary condition to be in the middle of the left side.
            GmshPoint ll = new GmshPoint(Core.LegRadius, 0, 0);
            GmshPoint lr = new GmshPoint(max_sec_radius + Dist_WdgTank, 0, 0);
            GmshPoint ur = new GmshPoint(max_sec_radius + Dist_WdgTank, Core.WindowHeight, 0);
            GmshPoint ul = new GmshPoint(Core.LegRadius, Core.WindowHeight, 0);
            GmshPoint bc = new GmshPoint(max_sec_radius + Dist_WdgTank, Core.WindowHeight/2, 0); //gcnew gmshPoint(core->radius_leg, core->window_height/2, 0);
            //The "boundary condition" point MUST be first...
            outFile.points.Add(bc);
            outFile.points.Add(ll);
            outFile.points.Add(lr);
            outFile.points.Add(ur);
            outFile.points.Add(ul);
            GmshLine bottom = new GmshLine(ll, lr);
            GmshLine right_lower = new GmshLine(lr, bc);
            GmshLine right_upper = new GmshLine(bc, ur);
            GmshLine top = new GmshLine(ur, ul);
            GmshLine left = new GmshLine(ul, ll);
            outFile.lines.Add(bottom);
            outFile.lines.Add(right_lower);
            outFile.lines.Add(right_upper);
            outFile.lines.Add(top);
            outFile.lines.Add(left);
            List<GmshLine> rect_lines = new List<GmshLine>
            {
                bottom,
                right_lower,
                right_upper,
                top,
                left
            };
            GmshLineLoop rect = new GmshLineLoop(rect_lines);
            outFile.line_loops.Add(rect);

            WriteWindingsGmsh(outFile);
            outFile.writeFile();
        }

        public void WriteAttributes(string filename) {
            StreamWriter sw = File.CreateText(filename);
            //The first "entity" should always be the boundary condition point
            //where we define the potential to be 0 (half-way up the left side)
            sw.WriteLine("GeomEntity 1");
            sw.WriteLine("x_constraint 0");
            //The second "entity" should always be the air surrounding the windings
            sw.WriteLine("GeomEntity 2");
            sw.WriteLine("mu 1.0");
            //The remaining "entities" are the windings themselves
            int index = 2;
            foreach (var winding in Windings) {
                foreach (var segment in winding.Segments) {
                    index++;
                    sw.WriteLine("GeomEntity {0}", index);
                    sw.WriteLine("mu 1.0");
                    sw.WriteLine("J {0}", segment.CurrentDensity*winding.CurrentDirection);
                    sw.WriteLine("wdg_num {0}", Windings.IndexOf(winding));
                    sw.WriteLine("section_num {0}", winding.Segments.IndexOf(segment));
                }
            }
            sw.Close();
        }

        public void RunCalculations(string filename)
        {
            TransMFEM.run_calcs();
            writeTransformerGmsh(Path.ChangeExtension(filename, "geo"));
            GenerateMesh(Path.ChangeExtension(filename, "geo"));
            WriteAttributes(Path.ChangeExtension(filename, "att"));
            FEProg fea = new FEProg();
            mesh = fea.run_FEA(Path.GetDirectoryName(filename) + Path.DirectorySeparatorChar + Path.GetFileNameWithoutExtension(filename), 1);
            Console.WriteLine(mesh.numNodes());
        }

        private void GenerateMesh(string filename)
        {
            var psi = new ProcessStartInfo(@"gmsh.exe")
            {
                Arguments = $"{filename} -2",
                UseShellExecute = false,
                CreateNoWindow = true
            };
            Process.Start(psi);
        }
    }
}