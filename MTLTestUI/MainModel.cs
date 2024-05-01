using Avalonia.Controls.Shapes;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.Data.Text;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using TDAP;
using System.Collections.Concurrent;
using System.Threading;
using Newtonsoft.Json.Linq;
using System.Data;
using System.Xml.Linq;
using System.Text.RegularExpressions;
//using static System.Net.WebRequestMethods;

namespace MTLTestUI
{
    internal class MainModel
    {
        double dist_wdg_tank_right = 4;
        double dist_wdg_tank_top = 2;
        double dist_wdg_tank_bottom = 2;
        double r_inner = in_to_m(15.25);
        double t_cond = in_to_m(0.085);
        double h_cond = in_to_m(0.3);
        double t_ins = in_to_m(0.018);
        double h_spacer = in_to_m(0.188);
        double r_cond_corner;
        int num_discs = 14;
        int turns_per_disc = 20;
        double eps_oil = 1.0; //2.2;
        double eps_paper = 2.2; //3.5;

        int phyAir;
        int phyExtBdry;
        int phyAxis;
        int[] phyTurnsCondBdry;
        int[] phyTurnsCond;
        int[] phyTurnsIns;

        private static double in_to_m(double x_in)
        {
            return x_in * 25.4 / 1000;
        }

        public Geometry GenerateGeometry()
        {
            double bdry_radius = 1; //radius of outer boundary of finite element model

            double z_offset = (num_discs * (h_cond + 2 * t_ins) + (num_discs - 1) * h_spacer + dist_wdg_tank_bottom + dist_wdg_tank_top) / 2;

            r_cond_corner = 0.1 * t_cond;

            var geometry = new Geometry();

            var conductorins_bdrys = new GeomLineLoop[num_discs * turns_per_disc];
            //var turn_surfaces = new GeomSurface[num_discs * turns_per_disc];
            //var ins_surfaces = 

            phyTurnsCond = new int[num_discs * turns_per_disc];
            phyTurnsCondBdry = new int[num_discs * turns_per_disc];
            phyTurnsIns = new int[num_discs * turns_per_disc];

            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                (double r, double z) = GetTurnMidpoint(i);
                z = z - z_offset;
                var conductor_bdry = geometry.AddRoundedRectangle(r, z, h_cond, t_cond, r_cond_corner);
                conductor_bdry.AttribID = phyTurnsCondBdry[i] = i + 2 * num_discs * turns_per_disc + 4;
                var insulation_bdry = geometry.AddRoundedRectangle(r, z, h_cond + 2 * t_ins, t_cond + 2 * t_ins, r_cond_corner + t_ins);
                var conductor_surface = geometry.AddSurface(conductor_bdry);
                conductor_surface.AttribID = phyTurnsCond[i] = i + 4;
                var insulation_surface = geometry.AddSurface(insulation_bdry, conductor_bdry);
                insulation_surface.AttribID = phyTurnsIns[i] = i + num_discs * turns_per_disc + 4;
                conductorins_bdrys[i] = insulation_bdry;
            }

            var pt_origin = geometry.AddPoint(0, 0);
            var pt_axis_top = geometry.AddPoint(0, bdry_radius);
            var pt_axis_bottom = geometry.AddPoint(0, -bdry_radius);
            var axis = geometry.AddLine(pt_axis_bottom, pt_axis_top);
            //var axis_lower = geometry.AddLine(pt_axis_bottom, pt_origin);
            axis.AttribID = phyAxis = 3;
            //axis_lower.AttribID = 3;
            var right_bdry = geometry.AddArc(pt_axis_top, pt_axis_bottom, bdry_radius, Math.PI);

            var outer_bdry = geometry.AddLineLoop(axis, right_bdry);
            outer_bdry.AttribID = phyExtBdry = 2;

            var interior_surface = geometry.AddSurface(outer_bdry, conductorins_bdrys);
            interior_surface.AttribID = phyAir = 1;

            GmshFile gmshFile = new GmshFile("case.geo");
            gmshFile.lc = 0.2;
            gmshFile.CreateFromGeometry(geometry);
            gmshFile.writeFile();

            return geometry;
        }

        private (double r, double z) GetTurnMidpoint(int n)
        {
            double r, z;
            int disc = (int)Math.Floor((double)n / (double)turns_per_disc);
            int turn = n % turns_per_disc;
            //Console.WriteLine($"disc: {disc} turn: {turn}");

            if (disc % 2 == 0)
            {
                //out to in
                r = r_inner + (turns_per_disc - turn) * (t_cond + 2 * t_ins) - (t_cond / 2 + t_ins);
            }
            else
            {
                //in to out
                r = r_inner + turn * (t_cond + 2 * t_ins) + (t_cond / 2 + t_ins);
            }
            z = dist_wdg_tank_bottom + num_discs * (h_cond + 2 * t_ins) + (num_discs - 1) * h_spacer - (h_cond / 2 + t_ins) - disc * (h_cond + 2 * t_ins + h_spacer);
            //Console.WriteLine($"disc: {disc} turn: {turn} r: {r} z:{z}");
            return (r, z);
        }

        public void CalcMesh()
        {
            string onelab_dir = "C:\\Users\\tcraymond\\Downloads\\onelab-Windows64\\onelab-Windows64\\";
            string gmshPath = onelab_dir + "gmsh.exe";
            string model_prefix = "./";

            string model = model_prefix + "case";
            string model_msh = model + ".msh";
            string model_geo = model + ".geo";

            var sb = new StringBuilder();

            Process p = new Process();

            p.StartInfo.FileName = gmshPath;
            p.StartInfo.Arguments = model_geo + " -2 -order 2";

            p.StartInfo.WindowStyle = ProcessWindowStyle.Normal;

            // redirect the output
            //p.StartInfo.RedirectStandardOutput = true;
            //p.StartInfo.RedirectStandardError = true;

            // hookup the eventhandlers to capture the data that is received
            //p.OutputDataReceived += (sender, args) => sb.AppendLine(args.Data);
            //p.ErrorDataReceived += (sender, args) => sb.AppendLine(args.Data);

            // direct start
            //p.StartInfo.UseShellExecute = false;

            //p.Start();
            p.Start();

            // start our event pumps
            //p.BeginOutputReadLine();
            //p.BeginErrorReadLine();

            // until we are done
            p.WaitForExit();

            //string output = sb.ToString();

            //int return_code = p.ExitCode;
            //if (return_code != 0)
            //{
            //    //throw new Exception("Failed to run gmsh to calculate mesh");
            //}
        }

        public Vector<double> CalcCapacitance(int posTurn, int proc)
        {
            var f = File.CreateText($"Results/{proc}/case.pro");

            f.WriteLine("Group{");
            f.WriteLine($"Air = Region[{phyAir}];");
            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                f.WriteLine($"Turn{i} = Region[{phyTurnsCondBdry[i]}];");
            }
            //f.WriteLine($"TurnPos = Region[{phyTurnsCondBdry[posTurn]}];");
            //if (negTurn >= 0)
            //{
            //    f.WriteLine($"TurnNeg = Region[{phyTurnsCondBdry[negTurn]}];");
            //}
            //else
            //{
            //    f.WriteLine("TurnNeg = Region[{}];");
            //}
            //f.Write("TurnZero = Region[{");
            //bool firstTurn = true;
            //for (int i = 0; i < phyTurnsCond.Count(); i++)
            //{
            //    if ((i != posTurn) && (i != negTurn))
            //    {
            //        if (!firstTurn)
            //        {
            //            f.Write(", ");
            //        }
            //        else
            //        {
            //            firstTurn = false;
            //        }
            //        f.Write($"{phyTurnsCondBdry[i]}");
            //    }
            //}

            //f.Write("}];\n");
            f.Write("TurnIns = Region[{");
            bool firstTurn = true;
            for (int i = 0; i < phyTurnsIns.Count(); i++)
            {
                if (!firstTurn)
                {
                    f.Write(", ");
                }
                else
                {
                    firstTurn = false;
                }
                f.Write($"{phyTurnsIns[i]}");
            }

            f.Write("}];\n");

            // f.write(f"Ground = Region[{phyCore}];\n")
            f.WriteLine($"Axis = Region[{phyAxis}];");
            f.WriteLine($"Surface_Inf = Region[{phyExtBdry}];");
            f.WriteLine("Vol_Ele = Region[{Air, TurnIns}];");
            f.Write("Sur_C_Ele = Region[{");
            //f.Write($"Turn{posTurn}}}];");
            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                f.Write($"Turn{i}");
                if (i < (num_discs * turns_per_disc - 1))
                {
                    f.Write(", ");
                }
                else
                {
                    f.Write("}];\n");
                }
            }
            f.WriteLine("}");

            //TODO: Fix for case where posTurn is last turn
            string otherTurns = "";
            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                if (i != posTurn)
                {
                    otherTurns += $"Turn{i}";
                    if ((i < (num_discs * turns_per_disc - 1)))
                    {
                        otherTurns += $", ";
                    }
                }
            }

            f.WriteLine($@"
            Flag_Axi = 1;

            Include ""../../Lib_Materials.pro"";

            Function {{
                dn[Region[Axis]] = 0; 
                epsr[Region[{{Air}}]] = {eps_oil};
                epsr[Region[{{TurnIns}}]] = {eps_paper};
            }}

            Constraint {{
                {{ Name ElectricScalarPotential; Type Assign;
                    Case {{
                        {{ Region Region[Surface_Inf]; Value 0; }}
                    }}
                }}
            }}
            Constraint {{                             
                {{ Name GlobalElectricPotential; Type Assign;
                    Case {{
                        {{ Region Region[Turn{posTurn}]; Value 1.0; }}
                        {{ Region Region[{{{otherTurns}}}]; Value 0; }}
                    }}
                }}
            }}
            Constraint {{ {{ Name GlobalElectricCharge; Case {{ }} }} }}

            Include ""../../Lib_Electrostatics_v.pro"";
            ");



            f.Close();

            string onelab_dir = "C:\\Users\\tcraymond\\Downloads\\onelab-Windows64\\onelab-Windows64\\";
            string mygetdp = onelab_dir + "getdp.exe";
            string model_prefix = $"./Results/{proc}/";

            string model = model_prefix + "case";
            string model_msh = model + ".msh";
            string model_pro = model + ".pro";
            double freq = 1e3;
            Process p = new Process();

            //p.StartInfo.FileName = "cmd.exe";
            //p.StartInfo.Arguments = "/k " + mygetdp + " " + model_pro + " -msh " + model_msh + $" -setstring modelPath Results/{proc} -solve Electrostatics_v -pos Electrostatics_v -v 5";

            p.StartInfo.FileName = mygetdp;
            p.StartInfo.Arguments = model_pro + " -msh " + model_msh + $" -setstring modelPath Results/{proc} -solve Electrostatics_v -pos Electrostatics_v -v 5";
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;

            p.Start();
            p.WaitForExit();
            //int return_code = P.ExitCode;
            //if (return_code != 0)
            //{
            //    throw new Exception("Failed to run getdp in C_tt_getdp");
            //}

            (double r, double z) = GetTurnMidpoint(posTurn);

            var resultFile = File.OpenText(model_prefix + "res/q.txt");
            string line = resultFile.ReadLine();
            var C_array = Array.ConvertAll(line.Split().Skip(2).ToArray(), Double.Parse);
            var C = Vector<double>.Build.Dense(C_array);
            C = C / r;
            resultFile.Close();
            return C;
        }

        public void CalcCapacitanceMatrix()
        {
            Matrix<double> C_getdp = Matrix<double>.Build.Dense(num_discs * turns_per_disc, num_discs * turns_per_disc);

            CalcMesh();

            //for (int i = 0; i < 8; i++)
            //{
            //    string procDirectory = Directory.GetCurrentDirectory() + $"/Results/{i}/";
            //    Directory.CreateDirectory(procDirectory);
            //    File.Copy("case.msh", procDirectory + "case.msh", true);
            //}

            int proc = 0;

            //Parallel.For(0, num_discs * turns_per_disc, t =>
            //{
            for (int t = 0; t < num_discs * turns_per_disc; t++)
            {
                proc = Thread.CurrentThread.ManagedThreadId;
                string procDirectory = Directory.GetCurrentDirectory() + $"/Results/{proc}/";
                Directory.CreateDirectory(procDirectory);
                File.Copy("case.msh", procDirectory + "case.msh", true);
                C_getdp.SetRow(t, CalcCapacitance(t, proc));
                Console.WriteLine($"Self capacitance for turn {t}: {C_getdp[t, t]}");
            }//);


            //for (int t1 = 0; t1 < num_discs * turns_per_disc; t1++)
            //{
            //    for (int t2 = t1 + 1; t2 < num_discs * turns_per_disc; t2++)
            //    {
            //        double W = CalcCapacitance(t1, t2, proc);
            //        C_getdp[t1, t2] = C_getdp[t2, t1] = (W - (C_getdp[t1, t1] + C_getdp[t2, t2]) / 2) / 1;
            //        Debug.WriteLine($"Mutual capacitance between turn {t1} & {t2}: {C_getdp[t1, t2]}");
            //    }
            //}

            //Parallel.For(0, num_discs * turns_per_disc, t1 =>
            //{
            //    Parallel.For(t1+1, num_discs * turns_per_disc, t2 =>
            //    {
            //        int proc = Thread.CurrentThread.ManagedThreadId;
            //        string procDirectory = Directory.GetCurrentDirectory() + $"/Results/{proc}/";
            //        Directory.CreateDirectory(procDirectory);
            //        File.Copy("case.msh", procDirectory + "case.msh", true);
            //        double W = CalcCapacitance(t1, t2, proc);
            //        C_getdp[t1, t2] = C_getdp[t2, t1] = (W - (C_getdp[t1, t1] + C_getdp[t2, t2]) / 2) / 1;
            //        Console.WriteLine($"Mutual capacitance between turn {t1} & {t2}: {C_getdp[t1, t2]}");
            //    });
            //});

            DelimitedWriter.Write("C_getdp.csv", C_getdp, ",");
        }

        public double CalcInductance(int posTurn, int negTurn, double freq)
        {

            string dir = posTurn.ToString();
            if (negTurn >= 0)
            {
                dir += "_" + negTurn.ToString();
            }
            string model_prefix = $"./Results/{dir}/";
            Directory.CreateDirectory(Directory.GetCurrentDirectory() + $"/Results/{dir}");
#if true

            var f = File.CreateText($"Results/{dir}/case.pro");

            f.WriteLine("Group{");
            f.Write($"Air = Region[{{{phyAir}, ");
            bool firstTurn = true;
            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                if (!firstTurn)
                {
                    f.Write(", ");
                }
                else
                {
                    firstTurn = false;
                }
                f.Write($"{phyTurnsIns[i]}");
            }
            f.WriteLine("}];");

            f.WriteLine($"TurnPos = Region[{phyTurnsCond[posTurn]}];");
            if (negTurn >= 0)
            {
                f.WriteLine($"TurnNeg = Region[{phyTurnsCond[negTurn]}];");
            }
            else
            {
                f.WriteLine("TurnNeg = Region[{}];");
            }
            f.Write("TurnZero = Region[{");
            firstTurn = true;
            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                if ((i != posTurn) && (i != negTurn))
                {
                    if (!firstTurn)
                    {
                        f.Write(", ");
                    }
                    else
                    {
                        firstTurn = false;
                    }
                    f.Write($"{phyTurnsCond[i]}");
                }
            }
            f.WriteLine("}];");
            //f.WriteLine($"Ground = Region[{phyGnd}];");
            //Surface_bn0 doesn't appear to do anything (also, surface?)
            f.WriteLine($"Axis = Region[{phyAxis}];");
            f.WriteLine($"Surface_Inf = Region[{phyExtBdry}];");
            f.WriteLine("Vol_Mag += Region[{Air, TurnPos, TurnNeg, TurnZero}];");
            f.WriteLine("Vol_C_Mag += Region[{TurnPos, TurnNeg, TurnZero}];");
            f.WriteLine("}");
            //f.WriteLine($"freq={freq};");
            f.WriteLine("Include \"../../L_s_inf.pro\";");
            f.Close();

            string onelab_dir = "C:\\Users\\tcraymond\\Downloads\\onelab-Windows64\\onelab-Windows64\\";
            string mygetdp = onelab_dir + "getdp.exe";
            

            string model = model_prefix + "case";
            string model_msh = "case.msh";
            string model_pro = model + ".pro";
            
            Process p = new Process();

            //p.StartInfo.FileName = "cmd.exe";
            //p.StartInfo.Arguments = "/k " + mygetdp + " " + model_pro + " -msh " + model_msh + $" -setstring modelPath Results/{proc}/ -setnumber freq " + freq.ToString() + " -solve Magnetodynamics2D_av -pos dyn -v 5";

            p.StartInfo.FileName = mygetdp;
            p.StartInfo.Arguments = model_pro + " -msh " + model_msh + $" -setstring modelPath Results/{dir}/ -setnumber freq " + freq.ToString() + " -solve Magnetodynamics2D_av -pos dyn -v 5";
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;

            p.Start();
            p.WaitForExit(120000);
            //int return_code = P.ExitCode;
            //if (return_code != 0)
            //{
            //    throw new Exception("Failed to run getdp in C_tt_getdp");
            //}
#endif
            (double r, double z) = GetTurnMidpoint(posTurn);

            //with open(model_prefix +'L_s/res/out.txt') as f:
            //    line = f.readline()
            //    ind = float(line.split()[1])
            //return 2 * math.pi * ind

            var resultFile = File.OpenText(model_prefix + "out.txt");
            string line = resultFile.ReadLine();
            var ind = double.Parse(line.Split()[2]);
            resultFile.Close();
            return ind;
        }

        public void CalcInductanceMatrix(double freq)
        {
            int n_turns = num_discs * turns_per_disc;
            Matrix<double> L_getdp = Matrix<double>.Build.Dense(n_turns, n_turns);

            Console.WriteLine($"Frequency: {freq.ToString("0.##E0")}");
            //CalcMesh();

            Parallel.For(0, n_turns, t =>
            //for (int t = 0; t < n_turns; t++)
            {
                L_getdp[t, t] = CalcInductance(t, -1, freq);
                (double r, double z) = GetTurnMidpoint(t);
                Console.WriteLine($"Self inductance for turn {t}: {L_getdp[t, t] / r / 1e-9}");
            }
            );

            Parallel.For(0, n_turns, t1 =>
            //for (int t1 = 0; t1 < n_turns; t1++)
            {
                Parallel.For(t1 + 1, n_turns, t2 =>
                //for (int t2 = t1 + 1; t2 < n_turns; t2++)
                {
                    double W = CalcInductance(t1, t2, freq) / 2;
                    L_getdp[t1, t2] = L_getdp[t2, t1] = (2 * W - (L_getdp[t1, t1] + L_getdp[t2, t2]))/2/-1;
                    (double r, double z) = GetTurnMidpoint(t1);
                    Console.WriteLine($"Mutual inductance between turn {t1} & {t2}: {L_getdp[t1, t2] / r / 1e-9}");
                }
                );
            }
            );

            for (int t1 = 0; t1 < n_turns; t1++)
            {
                (double r, double z) = GetTurnMidpoint(t1);
                for (int t2 = 0; t2 < n_turns; t2++)
                {
                    L_getdp[t1, t2] = L_getdp[t1, t2] / r;
                }
            }

            //Console.Write((L_getdp/1e-9).ToMatrixString());

            DelimitedWriter.Write($"L_getdp_{freq.ToString("0.00E0")}.csv", L_getdp, ",");
        }
    }
}
