using LinAlg = MathNet.Numerics.LinearAlgebra;
using Plotly.NET.LayoutObjects;
using Plotly.NET;
using System.Numerics;
using TDAP;
using System;
using MathNet.Numerics;
using MathNet.Numerics.Data.Text;
//using Microsoft.FSharp.Collections;
using MatrixExponential;
//using Microsoft.FSharp.Data.UnitSystems.SI.UnitNames;
using MathNet.Numerics.LinearAlgebra;
using System.Diagnostics;
using System.Text;
//using Microsoft.FSharp.Core;
//using MathNet.Numerics.Distributions;
using Microsoft.Data.Analysis;
using System.Text.RegularExpressions;
//using System.Collections.Immutable;

namespace MTLTestApp
{
    using Matrix_d = LinAlg.Matrix<double>;
    using Matrix_c = LinAlg.Matrix<Complex>;
    using Vector_d = LinAlg.Vector<double>;
    using Vector_c = LinAlg.Vector<Complex>;
    using static Plotly.NET.StyleParam;

    internal class Program
    {
        static private MatrixBuilder<double> M_d = Matrix_d.Build;
        static private MatrixBuilder<Complex> M_c = Matrix_c.Build;
        static private VectorBuilder<double> V_d = Vector_d.Build;
        static private VectorBuilder<Complex> V_c = Vector_c.Build;

        static double dist_wdg_tank_right = 4;
        static double dist_wdg_tank_top = 4;
        static double dist_wdg_tank_bottom = 2;
        static double r_inner = in_to_m(15.25);
        static double t_cond = in_to_m(0.085);
        static double h_cond = in_to_m(0.3);
        static double t_ins = in_to_m(0.018);
        static double h_spacer = in_to_m(0.188);
        static double r_cond_corner = 0.1 * t_cond;
        static int num_discs = 14;
        static int turns_per_disc = 20;
        static double eps_oil = 1.0; //2.2;
        static double eps_paper = 2.2; //3.5;
        static double rho_c = 1.68e-8; //ohm-m;
        static Complex Rs = Complex.Zero;
        static Complex Rl = Complex.Zero;

        //static double dist_wdg_tank_right = 40.0 / 1000.0;
        //static double dist_wdg_tank_top = 40.0 / 1000.0;
        //static double dist_wdg_tank_bottom = 40.0 / 1000.0;
        //static double r_inner = 20.0 / 1000.0;
        //static double t_cond = 3.0 / 1000.0;
        //static double h_cond = 12.0 / 1000.0;
        //static double t_ins = 0.5 / 1000.0;
        //static double h_spacer = 6.0 / 1000.0;
        //static int num_discs = 2;
        //static int turns_per_disc = 6;
        //static double eps_oil = 2.2;
        //static double eps_paper = 3.5;

        static double eps0 = 8.854e-12;

        static int num_freqs = 1000;
        static double min_freq = 10e3;
        static double max_freq = 50e6;

        private static double in_to_m(double x_in)
        {
            return x_in * 25.4 / 1000;
        }

        static void Main(string[] args)
        {

            Console.WriteLine("Hello, World!");

            string directoryPath = @"C:\Users\tcraymond\source\repos\TDAP2\MTLTestApp\bin\Debug\net8.0"; // Specify the directory path
            List<(string FileName, double NumericValue)> filesWithValues = new List<(string FileName, double NumericValue)>();

            // Get all files in the directory
            string[] files = Directory.GetFiles(directoryPath);

            // Regex to extract the numeric value from the filename
            Regex regex = new Regex(@"L_getdp_(\d+\.\d+E\d+)", RegexOptions.IgnoreCase);

            foreach (string file in files)
            {
                string fileName = Path.GetFileName(file);

                // Check if the filename starts with 'L_getdp'
                if (fileName.StartsWith("L_getdp", StringComparison.OrdinalIgnoreCase))
                {
                    Match match = regex.Match(fileName);
                    if (match.Success)
                    {
                        // Convert the extracted string to a double
                        double value = double.Parse(match.Groups[1].Value, System.Globalization.NumberStyles.Float);

                        // Add both the filename and the numeric value to the list
                        filesWithValues.Add((fileName, value));
                    }
                }
            }

            // Sort the list by the numeric value
            filesWithValues.Sort((a, b) => a.NumericValue.CompareTo(b.NumericValue));

            List<(double Freq, Matrix<double> L_matrix)> L_matrices_getdp = new List<(double Freq, Matrix<double> L_matrix)>();

            // Output the sorted filenames and their values
            foreach (var file in filesWithValues)
            {
                Console.WriteLine($"{file.FileName} - {file.NumericValue}");
                L_matrices_getdp.Add((file.NumericValue, DelimitedReader.Read<double>(file.FileName, false, ",", false)));
            }

            List<DataFrame> measuredData = new List<DataFrame>();
            for (int j = 1; j <= 6; j++)
            {
                measuredData.Add(ReadCSVToDF($"26DEC2023_Rough_NoCore/SDS0000{j}_bode.csv"));
            }
            

            int numturns = turns_per_disc * num_discs;

            //GenerateGeometry();

            Matrix<double> L_matrix_analytic = Calc_Lmatrix_analytic();
            Matrix<double> C_matrix_analytic = Calc_Cmatrix_analytic();

            //DisplayMatrixAsTable(C_matrix_analytic / 1e-12); //pF/m
            //DisplayMatrixAsTable(L_matrix_analytic / 1e-9); //nH/m

            Matrix<double> C_matrix_getdp = DelimitedReader.Read<double>("C_getdp.csv", false, ",", false);
            Matrix<double> L_matrix_getdp = DelimitedReader.Read<double>("L_getdp.csv", false, ",", false);

            //DisplayMatrixAsTable(C_matrix_getdp / 1e-12); //pF/m
            //DisplayMatrixAsTable(L_matrix_getdp / 1e-9);

            double[] R_t = new double[numturns];
            for (int t = 0; t < numturns; t++)
            {
                R_t[t] = R_c(h_cond, t_cond);
                //R_t[t] = R_c_old(r_c, t_cwall);
            }

            //TODO: Factor in skin effect mo' better
            double[] K_t = new double[numturns];
            
            for (int t = 0; t < numturns; t++)
            {
                K_t[t] = 0;
                //K_t[t] = 2/(r_c/1000)*Math.Sqrt(rho_c*4*Math.PI*1e-7/Math.PI);
            }

            double[] d_t = new double[numturns];
            for (int disc = 0; disc < num_discs; disc++)
            {
                for (int turn = 0; turn < turns_per_disc; turn++)
                {
                    if (disc % 2 == 0)
                    {
                        //out to in
                        d_t[disc * turns_per_disc + turn] = 2 * (r_inner + (turns_per_disc - turn) * (t_cond + 2 * t_ins) - (t_cond / 2 + t_ins));
                    }
                    else
                    {
                        //in to out
                        d_t[disc * turns_per_disc + turn] = 2 * (r_inner + turn * (t_cond + 2 * t_ins) + (t_cond / 2 + t_ins));
                    }
                    Console.WriteLine($"Turn {disc * turns_per_disc + turn}: d_t={d_t[disc * turns_per_disc + turn]}");
                }
            }

            var V_response_analytic = CalcResponseMTL(L_matrices_getdp, C_matrix_getdp, V_d.Dense(R_t), V_d.Dense(K_t), V_d.Dense(d_t));

            //var V_response_lumped = CalcResponseLumped(L_matrix_analytic, C_matrix_getdp, V_d.Dense(R_t), V_d.Dense(d_t), numturns);

            //var freqs = Generate.LinearSpaced(num_freqs, min_freq, max_freq);
            var freqs = Generate.LogSpaced(num_freqs, Math.Log10(min_freq), Math.Log10(max_freq));

            LinearAxis xAxis = new LinearAxis();
            xAxis.SetValue("title", "xAxis");
            xAxis.SetValue("showgrid", false);
            xAxis.SetValue("showline", true);
            xAxis.SetValue("type", "log");

            LinearAxis yAxis = new LinearAxis();
            yAxis.SetValue("title", "yAxis");
            yAxis.SetValue("showgrid", false);
            yAxis.SetValue("showline", true);

            Layout layout = new Layout();
            layout.SetValue("xaxis", xAxis);
            layout.SetValue("yaxis", yAxis);
            layout.SetValue("showlegend", true);

            var charts = new List<GenericChart.GenericChart>();
            int i = 0;
            for (int t = 40; t < numturns - 1; t = t + 40)
            {
                //var traces = new List<Plotly.NET.Trace>();
                var chart1 = Chart2D.Chart.Line<double, double, string>(x: freqs, y: V_response_analytic[t], Name: "Calculated", LineColor: Color.fromString("Red")).WithLayout(layout);
                //trace.SetValue("x", freqs);
                //trace.SetValue("y", V_response_analytic[t]);
                //trace.SetValue("mode", "lines");
                //trace.SetValue("name", $"Turn {t + 1}");
                
                //var trace2 = new Plotly.NET.Trace("scatter");
                
                var chart2 = Chart2D.Chart.Line<double, double, string>(x: measuredData[i]["Frequency(Hz)"].Cast<double>().ToList(), y: measuredData[i]["CH2 Amplitude(dB)"].Cast<double>().ToList(), Name: "Measured", LineColor: Color.fromString("Blue")).WithLayout(layout);
                //trace2.SetValue("x", measuredData[i]["Frequency(Hz)"]);
                //trace2.SetValue("y", measuredData[i]["CH2 Amplitude(dB)"]);
                //trace2.SetValue("mode", "lines");
                //trace2.SetValue("name", $"Turn {t + 1}");
                
                i++;

                //FSharpList<Plotly.NET.Trace> traces = ListModule.OfSeq<Plotly.NET.Trace>([trace, trace2]);

                charts.Add(Chart.Combine([chart1, chart2]).WithTitle($"Turn {t}"));
                //charts.Add(chart2);

            }

            var subplotGrid = Chart.Grid<IEnumerable<GenericChart.GenericChart>>(3, 2).Invoke(charts).WithSize(1600, 1200);

            // Show the combined chart with subplots
            subplotGrid.Show();
        }

        private static Matrix<double> InterpolateInductance(List<(double Freq, Matrix<double> L_matrix)> L_matrices, double freq)
        {
            if (freq <= L_matrices[0].Freq) return L_matrices[0].L_matrix;
            if (freq >= L_matrices[L_matrices.Count - 1].Freq) return L_matrices[L_matrices.Count - 1].L_matrix;
            for (int i = 0; i < L_matrices.Count - 1; i++)
            {
                if (freq >= L_matrices[i].Freq && freq <= L_matrices[i+1].Freq)
                {
                    double f1 = L_matrices[i].Freq;
                    double f2 = L_matrices[i + 1].Freq;
                    var L1 = L_matrices[i].L_matrix;
                    var L2 = L_matrices[i + 1].L_matrix;

                    return L1 + (L2 - L1) * (freq - f1) / (f2 - f1);
                }
            }
            return null;
        }

        public static void GenerateGeometry()
        {
            double bdry_radius = 1000; //radius of outer boundary of finite element model

            var geometry = new Geometry();

            var conductor_bdrys = new GeomLineLoop[num_discs * turns_per_disc];

            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                (double r, double z) = GetTurnMidpoint(i);
                var conductor_bdry = geometry.AddRoundedRectangle(r, z, h_cond, t_cond, r_cond_corner);
                var insulation_bdry = geometry.AddRoundedRectangle(r, z, h_cond + 2*t_ins, t_cond + 2*t_ins, r_cond_corner + t_ins);
                var conductor_surface = geometry.AddSurface(conductor_bdry);
                var insulation_surface = geometry.AddSurface(insulation_bdry, conductor_bdry);
                conductor_bdrys[i] = insulation_bdry;
            }

            var pt_origin = geometry.AddPoint(0, 0);
            var pt_axis_top = geometry.AddPoint(0, bdry_radius);
            var pt_axis_bottom = geometry.AddPoint(0, -bdry_radius);
            var axis_upper = geometry.AddLine(pt_origin, pt_axis_top);
            var axis_lower = geometry.AddLine(pt_axis_bottom, pt_origin);
            var right_bdry = geometry.AddArc(pt_axis_top, pt_axis_bottom, bdry_radius, Math.PI);

            var outer_bdry = geometry.AddLineLoop(axis_lower, axis_upper, right_bdry);

            var interior_surface = geometry.AddSurface(outer_bdry, conductor_bdrys);
        }

        //PUL Inductances
        private static Matrix<double> Calc_Lmatrix_analytic()
        {
            var M = Matrix<double>.Build;

            Matrix<double> L = M.Dense(num_discs * turns_per_disc, num_discs * turns_per_disc);

            for (int i = 0; i < num_discs * turns_per_disc; i++)
            {
                (double r_i, double z_i) = GetTurnMidpoint(i);
                L[i, i] = CalcSelfInductance(h_cond, t_cond, r_i) / (2 * Math.PI * r_i);
                for (int j = i + 1; j < num_discs * turns_per_disc; j++)
                {
                    (double r_j, double z_j) = GetTurnMidpoint(j);
                    L[i, j] = CalcMutualInductance_Lyle(r_i, z_i, h_cond, t_cond, r_j, z_j, h_cond, t_cond) / (2 * Math.PI * r_i);
                    L[j, i] = CalcMutualInductance_Lyle(r_i, z_i, h_cond, t_cond, r_j, z_j, h_cond, t_cond) / (2 * Math.PI * r_j);
                }
            }

            return L;
        }

        private static Matrix<double> Calc_Cmatrix_analytic()
        {
            double eps_0 = 8.854e-12;

            var M = Matrix<double>.Build;

            Matrix<double> C = M.Dense(num_discs * turns_per_disc, num_discs * turns_per_disc);

            //TODO: Need to modify to go from out to in and in to out
            for (int i = 0; i < num_discs; i++)
            {
                double C_abv;
                double C_bel;
                int n_abv;
                int n_bel;
                double dist_to_ground = 10;
                double k = 1.0 / 3.0;
                Console.WriteLine($"Disc {i}");
                // For now, let's calculate four capacitances for each turn
                // If last disc (section), above is prev disc, below is tank
                // TODO: How to handle segments above and below
                if (i == (num_discs - 1))
                {
                    C_abv = eps_0 * eps_oil * t_cond / (h_spacer + 2 * t_ins);
                    C_abv = eps_0 * (k / (2 * t_ins / eps_paper + (2 * t_ins + h_spacer) / eps_oil) + (1 - k) / (2 * t_ins / eps_paper + (2 * t_ins + h_spacer) / eps_paper)) * (t_cond + 2 * t_ins);
                    n_abv = i - 1;
                    C_bel = eps_0 * eps_oil * t_cond / dist_to_ground;
                    n_bel = -1;
                    Console.WriteLine($"Last (bottom) Disc: C_abv={C_abv} C_bel={C_bel}");
                }
                else if (i == 0) // If first disc, above is tank, below is next disc
                {
                    C_abv = eps_0 * eps_oil * t_cond / dist_to_ground;
                    n_abv = -1;
                    C_bel = eps_0 * eps_oil * t_cond / (h_spacer + 2 * t_ins);
                    C_bel = eps_0 * (k / (2 * t_ins / eps_paper + (2 * t_ins + h_spacer) / eps_oil) + (1 - k) / (2 * t_ins / eps_paper + (2 * t_ins + h_spacer) / eps_paper)) * (t_cond + 2 * t_ins);
                    n_bel = i + 1;
                    Console.WriteLine($"First (top) Disc: C_abv={C_abv} C_bel={C_bel}");
                }
                else
                {
                    C_abv = C_bel = eps_0 * eps_oil * t_cond / (h_spacer + 2 * t_ins);
                    C_abv = C_bel = eps_0 * (k / (2 * t_ins / eps_paper + (2 * t_ins + h_spacer) / eps_oil) + (1 - k) / (2 * t_ins / eps_paper + (2 * t_ins + h_spacer) / eps_paper)) * (t_cond + 2 * t_ins);
                    n_abv = i - 1;
                    n_bel = i + 1;
                    Console.WriteLine($"Middle Disc: C_abv={C_abv} C_bel={C_bel}");
                }

                for (int j = 0; j < turns_per_disc; j++)
                {
                    double C_lt;
                    double C_rt;

                    bool out_to_in = (i % 2 == 0);

                    // If first turn in section, left is inner winding or core, right is next turn
                    if ((j == 0 && !out_to_in) || (j == (turns_per_disc - 1) && out_to_in))
                    {
                        C_lt = eps_0 * eps_paper * h_cond / dist_to_ground;
                        C_rt = eps_0 * eps_paper * (h_cond + 2 * t_ins) / (2 * t_ins);
                        
                    }
                    else if ((j == (turns_per_disc - 1) && !out_to_in) || (j == 0 && out_to_in)) // If last turn in section, left is previous turn, right is outer winding or tank
                    {
                        C_lt = eps_0 * eps_paper * (h_cond + 2 * t_ins) / (2 * t_ins);
                        C_rt = eps_0 * eps_paper * h_cond / dist_to_ground;
                    }
                    else
                    {
                        C_lt = C_rt = eps_0 * eps_paper * (h_cond + 2 * t_ins) / (2 * t_ins);
                    }

                    int disc_abv = i - 1;
                    if (disc_abv < 0)
                    {
                        // ground above
                        n_abv = -1;
                    }
                    else
                    {
                        n_abv = disc_abv * turns_per_disc + (turns_per_disc - j - 1);
                    }
                    int disc_bel = i + 1;
                    if (disc_bel >= num_discs)
                    {
                        // ground below
                        n_bel = -1;
                    }
                    else
                    {
                        n_bel = disc_bel * turns_per_disc + (turns_per_disc - j - 1);
                    }

                    int n_lt;
                    int n_rt;

                    if (i % 2 == 0) //out to in
                    {
                        n_lt = i * turns_per_disc + j + 1;
                        n_rt = i * turns_per_disc + j - 1;
                        if (j == 0)
                        {
                            n_rt = -1;
                        }
                        if (j >= (turns_per_disc - 1))
                        {
                            n_lt = -1;
                        }
                        //Console.WriteLine("Out to in");
                    }
                    else //in to out
                    {
                        n_lt = i * turns_per_disc + j - 1;
                        n_rt = i * turns_per_disc + j + 1;
                        if (j == 0)
                        {
                            n_lt = -1;
                        }
                        if (j >= (turns_per_disc - 1))
                        {
                            n_rt = -1;
                        }
                        //Console.WriteLine($"{j}");
                        //Console.WriteLine("In to out");
                    }

                    int n = i * turns_per_disc + j;
                    Console.WriteLine($"n: {n} n_abv: {n_abv} n_bel: {n_bel} n_lt: {n_lt} n_rt: {n_rt}");

                    // Assemble C_abv, C_bel, C_lt, C_rt into C_seg
                    C[n, n] = C_abv + C_bel + C_lt + C_rt;
                    if (n_abv >= 0)
                    {
                        C[n, n_abv] = -C_abv;
                        C[n_abv, n] = -C_abv;
                    }
                    if (n_bel >= 0)
                    {
                        C[n, n_bel] = -C_bel;
                        C[n_bel, n] = -C_bel;
                    }
                    if (n_lt >= 0)
                    {
                        C[n, n_lt] = -C_lt;
                        C[n_lt, n] = -C_lt;
                    }
                    if (n_rt >= 0)
                    {
                        C[n, n_rt] = -C_rt;
                        C[n_rt, n] = -C_rt;
                    }
                }
            }

            return C;
        }

        public static double R_c(double h_m, double w_m)
        {
            return rho_c / (h_m * w_m);
        }

        private static double CalcSelfInductance(double h, double w, double r_avg)
        {
            double mu_0 = 4 * Math.PI * 1e-7;
            double GMD = Math.Exp(0.5 * Math.Log(h * h + w * w) + 2 * w / (3 * h) * Math.Atan(h / w) + 2 * h / (3 * w) * Math.Atan(w / h) - w * w / (12 * h * h) * Math.Log(1 + h * h / (w * w)) - h * h / (12 * w * w) * Math.Log(1 + w * w / (h * h)) - 25 / 12);
            double L_s = mu_0 * r_avg * (Math.Log(8 * r_avg / GMD) - 2);
            Console.WriteLine($"r_avg: {r_avg} GMD: {GMD} L_s: {L_s/1e-9} L_s/l: {L_s/(2*Math.PI*r_avg)/1e-9}");
            return L_s;
        }

        private static double CalcInductanceCoaxLoops(double r_a, double z_a, double r_b, double z_b)
        {
            double mu_0 = 4 * Math.PI * 1e-7;
            double d = Math.Abs(z_b - z_a);
            double k = Math.Sqrt(4 * r_a * r_b / ((r_a + r_b) * (r_a + r_b) + d * d));
            double L_ab = 2 * mu_0 / k * Math.Sqrt(r_a * r_b) * ((1 - k * k / 2) * Elliptic.EllipticK(k) - Elliptic.EllipticE(k));
            return L_ab;
        }

        // r is mean radius of each turn
        // z is height of mid-point
        private static (double r_i, double z_i, double r_j, double z_j) CalcLyleLoops(double r, double z, double w, double h)
        {
            double r_i, z_i, r_j, z_j;

            if (h > w)
            {
                // Two rings with r_avg = r_1 and at d +/- beta
                double r_adj = r * (1 + w * w / (24 * r * r));
                double beta = Math.Sqrt(((h * h) - (w * w)) / 12);
                r_i = r_adj;
                z_i = z + beta;
                r_j = r;
                z_j = z - beta;
            }
            else
            {
                // Two rings with r_avg = r_1 and r_1 + 2*delta and at z = d
                double r_adj = r * (1 + h * h / (24 * r * r));
                double delta = Math.Sqrt(((w * w) - (h * h)) / 12);
                r_i = r;
                z_i = z;
                r_j = r + 2 * delta;
                z_j = z;
            }

            return (r_i, z_i, r_j, z_j);
        }

        private static double CalcMutualInductance_Lyle(double r_a, double z_a, double h_a, double w_a, double r_b, double z_b, double h_b, double w_b)
        {
            var ta = CalcLyleLoops(r_a, z_a, w_a, h_a);
            double r_1 = ta.r_i;
            double z_1 = ta.z_i;
            double r_2 = ta.r_j;
            double z_2 = ta.z_j;

            var tb = CalcLyleLoops(r_b, z_b, w_b, h_b);
            double r_3 = tb.r_i;
            double z_3 = tb.z_i;
            double r_4 = tb.r_j;
            double z_4 = tb.z_j;

            double L_13 = CalcInductanceCoaxLoops(r_1, z_1, r_3, z_3);
            double L_14 = CalcInductanceCoaxLoops(r_1, z_1, r_4, z_4);
            double L_23 = CalcInductanceCoaxLoops(r_2, z_2, r_3, z_3);
            double L_24 = CalcInductanceCoaxLoops(r_2, z_2, r_4, z_4);
            double L_ab = (L_13 + L_14 + L_23 + L_24) / 4;

            return L_ab;

        }

        // The following follows the axisymmetric MTL calulation outline in the Fattal paper
        // HA is lower left-hand quadrant of left-hand side matrix
        // HB is lower right-hand quadrant of the LHS matric
        // HA and HB are "terminal" constraints dictated by winding type (terminal here meaning the
        // terminations of each winding turn when viewed as parallel transmission lines)
        public static Matrix_d CalcHA(int n)
        {
            Matrix_d HA11 = M_d.DenseIdentity(n);
            Matrix_d HA21 = M_d.Dense(n, n);
            Matrix_d HA12 = M_d.Dense(n, n);
            for (int t = 0; t < (n - 1); t++)
            {
                HA12[t + 1, t] = -1.0;
            }
            Matrix_d HA22 = M_d.Dense(n, n);
            HA22[n - 1, n - 1] = 1.0;
            Matrix_d HA1 = HA11.Append(HA12);
            Matrix_d HA2 = HA21.Append(HA22);
            Matrix_d HA = HA1.Stack(HA2);
            return HA;
        }

        public static Matrix_c CalcHB(int n, double f)
        {
            Matrix_c HB11 = M_c.Dense(n, n);
            HB11[0, 0] = Rs; //Source impedance
            Matrix_c HB12 = M_c.Dense(n, n);
            Matrix_c HB21 = M_c.Dense(n, n);
            for (int t = 0; t < (n - 1); t++)
            {
                HB21[t, t + 1] = 1.0;
            }
            Matrix_c HB22 = -1.0 * M_c.DenseIdentity(n);
            HB22[n - 1, n - 1] = Rl; //Impedance to ground
            Matrix_c HB1 = HB11.Append(HB12);
            Matrix_c HB2 = HB21.Append(HB22);
            Matrix_c HB = HB1.Stack(HB2);
            return HB;
        }

        public static Vector_c CalcResponseAtFreq(double f, Matrix_d Gamma, Matrix_d HA, Matrix_d R, Matrix_d K, Matrix_d L, Matrix_d C, int n)
        {
            Matrix_c HB = CalcHB(n, f);

            Matrix_c B2 = HA.ToComplex().Append(HB);

            double mu_c = 4 * Math.PI * 1e-7;
            double sigma_c = 1 / rho_c;
            Complex eta = Complex.Sqrt(2d * Math.PI * f * mu_c * sigma_c * Complex.ImaginaryOne) * t_cond / 2d;
            double R_skin = (1 / (sigma_c * h_cond * t_cond) * eta * Complex.Cosh(eta) / Complex.Sinh(eta)).Real;
            Console.WriteLine($"R_skin: {R_skin}");
            var R_f = R + Matrix_d.Build.DenseIdentity(n,n)*R_skin;

            // A = [           0              -Gamma*(R+j*2*pi*f*L)]
            //     [ -Gamma*(G+j*2*pi*f*C)                0        ]
            Matrix_c A12 = -Gamma.ToComplex() * (R_f.ToComplex() + K.ToComplex() * Math.Sqrt(f) + Complex.ImaginaryOne * 2d * Math.PI * f * L.ToComplex());
            Matrix_c A21 = -Gamma.ToComplex() * (Complex.ImaginaryOne * 2 * Math.PI * f * C.ToComplex());
            Matrix_c A1 = M_c.Dense(n, n).Append(A12);
            Matrix_c A2 = A21.Append(M_c.Dense(n, n));
            Matrix_c A = A1.Stack(A2);
            Matrix_c Phi = A.Exponential();
            Matrix_c Phi1 = Phi.SubMatrix(0, Phi.RowCount, 0, n); //Phi[:,:n]
            Matrix_c Phi2 = Phi.SubMatrix(0, Phi.RowCount, n, Phi.ColumnCount - n); //Phi[:, n:]
            Matrix_c B11 = Phi1.Append((-1.0 * M_c.DenseIdentity(n)).Stack(M_c.Dense(n, n)));
            Matrix_c B12 = Phi2.Append(M_c.Dense(n, n).Stack(-1.0 * M_c.DenseIdentity(n)));
            Matrix_c B1 = B11.Append(B12);
            Matrix_c B = B1.Stack(B2);
            Vector_c v = V_c.Dense(4 * n);
            v[2 * n] = 1.0;
            return B.Solve(v);
        }

        // Returns a List of the turn responses (vector of complex gains for each turn) at each frequency.  E.g., if there are 10 turns at 1,000 frequency points, then it will return a list
        // with 1,000 entries of vectors of length 10.
        public static List<Vector_c> CalcResponse(Vector_d r, Matrix_d R, Matrix_d K, List<(double Freq, Matrix_d L)> L_f, Matrix_d C, int n)
        {
            // Gamma is the diagonal matrix of conductors radii (eq. 2)
            Matrix_d Gamma = M_d.DenseOfDiagonalVector(2d * Math.PI * r);
            Matrix_d HA = CalcHA(n);

            // Create vector of frequencies
            //var freqs = Generate.LinearSpaced(num_freqs, min_freq, max_freq);
            var freqs = Generate.LogSpaced(num_freqs, Math.Log10(min_freq), Math.Log10(max_freq));

            List<Vector_c> V_turn = new List<Vector_c>();
            var Y = new List<Complex>();

            foreach (var f in freqs)
            {
                Console.WriteLine($"Calculating at {f / 1e6}MHz");
                Matrix_d L = InterpolateInductance(L_f, f);
                Console.WriteLine($"L[0, 0]: {L[0,0]/1e-9}");
                var vi_vec = CalcResponseAtFreq(f, Gamma, HA, R, K, L, C, n);
                var gain_at_freq = vi_vec.SubVector(n, n) / vi_vec[0];
                V_turn.Add(gain_at_freq); //[n: 2 * n]
                Y.Add(vi_vec[2 * n]); //Original code took absolute val of vi_vec[2*n]
            }

            return V_turn;
        }

        // Impedances passed here are per unit length
        // C matrix should be in self/mutual form, not Maxwell form
        public static List<double[]> CalcResponseMTL(List<(double Freq, Matrix_d L_matrix)> L_matrices, Matrix_d C_matrix, Vector_d R_t, Vector_d K_t, Vector_d d_t)
        {
            int numturns = d_t.Count;
            Matrix_d R_array = M_d.Dense(numturns, numturns);
            Matrix_d K_array = M_d.Dense(numturns, numturns);

            // Turn R & K vectors into matrices
            for (int t = 0; t < numturns; t++)
            {
                // Add R
                R_array[t, t] = R_t[t];
                K_array[t, t] = K_t[t];
            }

            // r is vector of turn radii in meters
            // d (given above) is turn diameters in m
            var r = d_t / 2d;
            var V_resp = CalcResponse(r, R_array, K_array, L_matrices, C_matrix, numturns);

            int num_freq_steps = V_resp.Count;

            var V_response = new List<double[]>();
            for (int t = 0; t < numturns; t++)
            {
                V_response.Add(new double[num_freq_steps]);
            }

            // Enumerate through raw turn responses (vector of Complex Gain for each turn) at each frequency
            for (int f = 0; f < num_freq_steps; f++)
            {
                // Enumerate each turn
                for (int t = 0; t < numturns; t++)
                {
                    //Translate to dB
                    V_response[t][f] = 20d * Math.Log10(V_resp[f][t].Magnitude);
                }
            }

            return V_response;
        }

        private static (double r, double z) GetTurnMidpoint(int n)
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

        static void DisplayMatrixAsTable(Matrix<double> matrix)
        {
            StringBuilder html = new StringBuilder();
            html.AppendLine("<html><head><title>Matrix Display</title></head><body>");
            html.AppendLine("<table border='1'>");

            for (int i = 0; i < matrix.RowCount; i++)
            {
                html.AppendLine("<tr>");
                for (int j = 0; j < matrix.ColumnCount; j++)
                {
                    html.AppendFormat("<td>{0}</td>", matrix[i, j]);
                }
                html.AppendLine("</tr>");
            }

            html.AppendLine("</table>");
            html.AppendLine("</body></html>");

            // Generate a unique filename in the temporary directory
            string fileName = Path.Combine(Path.GetTempPath(), $"MatrixDisplay_{Guid.NewGuid()}.html");
            File.WriteAllText(fileName, html.ToString());

            // Open the HTML file in the default web browser
            Process.Start(new ProcessStartInfo
            {
                FileName = fileName,
                UseShellExecute = true
            });
        }

        public static List<List<double>> CalcResponseLumped(Matrix_d L_matrix, Matrix_d C_matrix, Vector_d R_t, Vector_d d_t, int numturns)
        {
            Matrix_d L = M_d.Dense(numturns, numturns);
            Matrix_d R = M_d.Dense(numturns, numturns);
            Matrix_d C = M_d.Dense(numturns, numturns);
            for (int t = 0; t < numturns; t++)
            {
                L[t, t] = L_matrix[t, t] * Math.PI * d_t[t];
                C[t, t] = C_matrix[t, t] * Math.PI * d_t[t];
                R[t, t] = R_t[t] * Math.PI * d_t[t];
                for (int t2 = 0; t2 < numturns; t2++)
                {
                    if (t != t2)
                    {
                        L[t, t2] = L_matrix[t, t2] * Math.PI * d_t[t];
                        C[t, t2] = C_matrix[t, t2] * Math.PI * d_t[t];
                    }
                }
            }
            // branch-node incidence matrix
            // in this context, this matrix relates the inductor currents and the node voltages
            Matrix_d Q = M_d.Dense(numturns, numturns);
            // rows = branches
            // columns = nodes
            for (int t = 0; t < numturns; t++)
            {
                // t is branch number
                // first node in branch 
                Q[t, t] = 1.0;
                if (t != (numturns - 1))
                {
                    Q[t, t + 1] = -1.0;
                }
            }

            // Create vector of frequencies
            //var freqs = Generate.LinearSpaced(num_freqs, min_freq, max_freq);
            var freqs = Generate.LogSpaced(num_freqs, Math.Log10(min_freq), Math.Log10(max_freq));

            var Z_term = new List<double>();
            //List of lists
            var V_r = new List<List<double>>();

            Matrix_c Z = M_c.Dense(0, 0);

            for (int t = 0; t < numturns; t++)
            {
                V_r.Add(new List<double>());
            }

            foreach (var f in freqs)
            {
                Console.WriteLine($"Calculating at {f / 1e6}MHz");
                //Y = 1j * 2 * math.pi * f * C + Q.transpose() @np.linalg.inv(R + 1j * 2 * math.pi * f * L)@Q
                var Y = Complex.ImaginaryOne * 2 * Math.PI * f * C.ToComplex() + Q.ToComplex().Transpose() * (R.ToComplex() + Complex.ImaginaryOne * 2 * Math.PI * f * L.ToComplex()).Inverse()*Q.ToComplex();
                if (!Y.ConditionNumber().IsInfinity())
                {
                    //print(np.linalg.cond(Y))
                    Z = Y.Inverse();
                }
                else
                {
                    Console.WriteLine("Matrix is shite");
                }
                Z_term.Add(Z[0, 0].Magnitude);
                for (int t = 0; t < numturns - 1; t++)
                {
                    V_r[t].Add(20 * Math.Log10(Z[0, t + 1].Magnitude / Z[0, 0].Magnitude));
                }
            }

            return V_r;
        }

        public static DataFrame ReadCSVToDF(string filename)
        {
            // Read all lines from the file
            string[] allLines = File.ReadAllLines(filename);

            // Skip the first 24 lines
            var dataLines = allLines[24..];

            // Assuming the first non-skipped line contains headers
            string[] headers = dataLines[0].Split(',');

            // Prepare lists to hold data for each column
            List<DoubleDataFrameColumn> columns = new List<DoubleDataFrameColumn>();
            foreach (var header in headers)
            {
                columns.Add(new DoubleDataFrameColumn(header, 0));
            }

            // Parse each line and fill the columns
            foreach (var line in dataLines[1..]) // Skipping header line in dataLines
            {
                var values = line.Split(',');
                for (int i = 0; i < values.Length; i++)
                {
                    columns[i].Append(Double.Parse(values[i]));
                }
            }

            // Create the DataFrame
            DataFrame df = new DataFrame(columns);

            return df;
        }
    }
}
