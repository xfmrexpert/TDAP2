using LinAlg = MathNet.Numerics.LinearAlgebra;
using Plotly.NET.LayoutObjects;
using Plotly.NET;
using System.Numerics;
using TDAP;
using System;
using Plotly.NET;
using Plotly.NET.LayoutObjects;
using MathNet.Numerics;
using Microsoft.FSharp.Collections;
using MatrixExponential;
using Microsoft.FSharp.Data.UnitSystems.SI.UnitNames;
using MathNet.Numerics.LinearAlgebra;
using System.Diagnostics;
using System.Text;
using Microsoft.FSharp.Core;
using MathNet.Numerics.Distributions;

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
        static int num_discs = 14;
        static int turns_per_disc = 20;
        static double eps_oil = 1.0; //2.2;
        static double eps_paper = 2.2; //3.5;

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
        static double max_freq = 1e6;

        static void Main(string[] args)
        {

            Console.WriteLine("Hello, World!");
            int numturns = turns_per_disc * num_discs;
            // Define geometry
            //int numturns = 9;
            //double d_c = 6.35; //turn diameter in mm
            //double h_cg = 97; //height above ground plane, mm
            //double t_cwall = 0.0285 * 25.4; //conductor thickness, mm
            //double[] d_t = { 641.5, 603.5, 566, 527, 478.5, 431, 392.5, 354, 315 };

            //double r_c = d_c / 2.0;

            Matrix<double> L_matrix_analytic = Calc_Lmatrix_analytic();
            Matrix<double> C_matrix_analytic = Calc_Cmatrix_analytic();
            
            //Matrix<double> L_matrix_analytic = Calc_Lmatrix_old(d_t, h_cg, r_c);
            //Matrix<double> C_matrix_analytic = Calc_Cmatrix_old(d_t, h_cg, r_c);

            double[,] C_array = new double[,]{  { 445.7, -396.3, -3.4, -1.2, -0.6, -0.7, -0.1, 0.0, -0.1, -0.8, -3.3, -14.4},
                                                { -396.3, 809.8, -393.9, -2.6, -0.8, -0.7, -0.1, -0.1, -0.6, -2.6, -4.7, -3.3},
                                                { -3.4, -393.9, 809.8, -393.9, -2.6, -1.3, -0.2, -0.6, -2.6, -4.7, -2.6, -0.8},
                                                { -1.2, -2.6, -393.9, 809.9, -393.9, -3.6, -0.8, -2.7, -4.7, -2.6, -0.6, -0.1},
                                                { -0.6, -0.8, -2.6, -393.9, 809.8, -396.7, -3.5, -4.7, -2.7, -0.6, -0.1, 0.0},
                                                { -0.7, -0.7, -1.3, -3.6, -396.7, 441.8, -16.7, -3.5, -0.8, -0.2, -0.1, -0.1},
                                                { -0.1, -0.1, -0.2, -0.8, -3.5, -16.7, 441.8, -396.7, -3.6, -1.3, -0.7, -0.7},
                                                { 0.0, -0.1, -0.6, -2.7, -4.7, -3.5, -396.7, 809.8, -393.9, -2.6, -0.8, -0.6},
                                                { -0.1, -0.6, -2.6, -4.7, -2.7, -0.8, -3.6, -393.9, 809.8, -393.9, -2.6, -1.2},
                                                { -0.8, -2.6, -4.7, -2.6, -0.6, -0.2, -1.3, -2.6, -393.9, 809.9, -393.9, -3.4},
                                                { -3.3, -4.7, -2.6, -0.6, -0.1, -0.1, -0.7, -0.8, -2.6, -393.9, 809.7, -396.3},
                                                { -14.4, -3.3, -0.8, -0.1, 0.0, -0.1, -0.7, -0.6, -1.2, -3.4, -396.3, 445.7} };

            double[,] L_array = new double[,]
                                {
                                    {352.5, 305.5, 270.2, 242.8, 221.3, 204.9, 151.2, 156.5, 160.9, 163.7, 164.1, 161.1},
                                    {305.5, 344.7, 301.9, 269.7, 244.8, 226.1, 160.7, 165.8, 169.5, 171.0, 169.3, 164.1},
                                    {270.2, 301.9, 344.6, 304.4, 274.6, 252.4, 169.6, 173.9, 176.2, 175.3, 171.0, 163.7},
                                    {242.8, 269.7, 304.4, 349.5, 311.8, 284.9, 176.9, 179.9, 179.8, 176.2, 169.5, 160.9},
                                    {221.3, 244.8, 274.6, 311.8, 359.6, 325.3, 181.8, 182.7, 179.9, 173.9, 165.8, 156.5},
                                    {204.9, 226.0, 252.4, 284.9, 325.3, 377.5, 183.2, 181.8, 176.9, 169.6, 160.7, 151.2},
                                    {151.1, 160.7, 169.6, 176.9, 181.7, 183.1, 377.5, 325.4, 285.0, 252.5, 226.1, 204.9},
                                    {156.4, 165.7, 173.9, 179.8, 182.6, 181.7, 325.4, 359.7, 311.9, 274.6, 244.9, 221.4},
                                    {160.9, 169.4, 176.1, 179.7, 179.8, 176.9, 284.9, 311.9, 349.6, 304.5, 269.7, 242.8},
                                    {163.7, 170.9, 175.3, 176.1, 173.9, 169.6, 252.5, 274.6, 304.5, 344.7, 302.0, 270.2},
                                    {164.0, 169.2, 170.9, 169.4, 165.7, 160.7, 226.1, 244.9, 269.7, 302.0, 344.8, 305.5},
                                    {161.0, 164.0, 163.7, 160.8, 156.4, 151.1, 204.9, 221.4, 242.8, 270.2, 305.5, 352.6},
                                };

            //C_matrix_analytic = M_d.DenseOfArray(C_array) * 1e-12;
            //L_matrix_analytic = M_d.DenseOfArray(L_array) * 1e-9;

            DisplayMatrixAsTable(C_matrix_analytic / 1e-12); //pF/m
            DisplayMatrixAsTable(L_matrix_analytic / 1e-9); //nH/m

            double[] R_t = new double[numturns];
            for (int t = 0; t < numturns; t++)
            {
                R_t[t] = R_c(h_cond, t_cond)*1000;
                //R_t[t] = R_c_old(r_c, t_cwall);
            }

            //TODO: Factor in skin effect mo' better
            double[] K_t = new double[numturns];
            double rho_c = 1.68e-8; //ohm-m
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

            var V_response_analytic = CalcResponseMTL(L_matrix_analytic, C_matrix_analytic, V_d.Dense(R_t), V_d.Dense(K_t), V_d.Dense(d_t));

            var V_response_lumped = CalcResponseLumped(L_matrix_analytic, C_matrix_analytic, V_d.Dense(R_t), V_d.Dense(d_t), numturns);

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

            var traces = new List<Plotly.NET.Trace>();
            for (int t = 40; t < numturns - 1; t=t+40)
            {
                Plotly.NET.Trace trace = new Plotly.NET.Trace("scatter");
                trace.SetValue("x", freqs);
                trace.SetValue("y", V_response_analytic[t]);
                trace.SetValue("mode", "lines");
                trace.SetValue("name", $"Turn {t + 1}");
                traces.Add(trace);
            }

            GenericChart
                .ofTraceObjects(true, ListModule.OfSeq(traces))
                .WithLayout(layout)
                .WithSize(800, 600)
                .Show();

            traces = new List<Plotly.NET.Trace>();
            for (int t = 40; t < numturns - 1; t = t + 40)
            {
                Plotly.NET.Trace trace = new Plotly.NET.Trace("scatter");
                trace.SetValue("x", freqs);
                trace.SetValue("y", V_response_lumped[t]);
                trace.SetValue("mode", "lines");
                trace.SetValue("name", $"Turn {t + 1}");
                traces.Add(trace);
            }

            GenericChart
                .ofTraceObjects(true, ListModule.OfSeq(traces))
                .WithLayout(layout)
                .WithSize(800, 600)
                .Show();
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
            double rho_c = 1.68e-8; //ohm-m
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
            HB11[0, 0] = 0.0; //Rs
            Matrix_c HB12 = M_c.Dense(n, n);
            Matrix_c HB21 = M_c.Dense(n, n);
            for (int t = 0; t < (n - 1); t++)
            {
                HB21[t, t + 1] = 1.0;
            }
            Matrix_c HB22 = -1.0 * M_c.DenseIdentity(n);
            HB22[n - 1, n - 1] = 0; //Impedance to ground Complex.ImaginaryOne * 2d * Math.PI * f; //Had a * 0 on the end?
            Matrix_c HB1 = HB11.Append(HB12);
            Matrix_c HB2 = HB21.Append(HB22);
            Matrix_c HB = HB1.Stack(HB2);
            return HB;
        }

        public static Vector_c CalcResponseAtFreq(double f, Matrix_d Gamma, Matrix_d HA, Matrix_d R, Matrix_d K, Matrix_d L, Matrix_d C, int n)
        {
            Matrix_c HB = CalcHB(n, f);

            Matrix_c B2 = HA.ToComplex().Append(HB);

            // A = [           0              -Gamma*(R+j*2*pi*f*L)]
            //     [ -Gamma*(G+j*2*pi*f*C)                0        ]
            Matrix_c A12 = -Gamma.ToComplex() * (R.ToComplex() + K.ToComplex() * Math.Sqrt(f) + Complex.ImaginaryOne * 2d * Math.PI * f * L.ToComplex());
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
        public static List<Vector_c> CalcResponse(Vector_d r, Matrix_d R, Matrix_d K, Matrix_d L, Matrix_d C, int n)
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
                var vi_vec = CalcResponseAtFreq(f, Gamma, HA, R, K, L, C, n);
                var gain_at_freq = vi_vec.SubVector(n, n) / vi_vec[0];
                V_turn.Add(gain_at_freq); //[n: 2 * n]
                Y.Add(vi_vec[2 * n]); //Original code took absolute val of vi_vec[2*n]
            }

            return V_turn;
        }

        // Impedances passed here are per unit length
        // C matrix should be in self/mutual form, not Maxwell form
        public static List<double[]> CalcResponseMTL(Matrix_d L_matrix, Matrix_d C_matrix, Vector_d R_t, Vector_d K_t, Vector_d d_t)
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
            var V_resp = CalcResponse(r, R_array, K_array, L_matrix, C_matrix, numturns);

            int num_freq_steps = V_resp.Count();

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

        
        private static double in_to_m(double x_in)
        {
            return x_in * 25.4 / 1000;
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

        public static new List<List<double>> CalcResponseLumped(Matrix_d L_matrix, Matrix_d C_matrix, Vector_d R_t, Vector_d d_t, int numturns)
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

        #region SpiralCoil_Impedances

        public static Matrix_d Calc_Lmatrix_old(double[] d_t, double h_cg, double r_c)
        {
            var M = Matrix_d.Build;

            Matrix_d L_matrix = M.Dense(9, 9);

            for (int t = 0; t < 9; t++)
            {
                L_matrix[t, t] = L_s_analytic(d_t[t], r_c);
            }

            for (int t1 = 0; t1 < 8; t1++)
            {
                for (int t2 = t1 + 1; t2 < 9; t2++)
                {
                    L_matrix[t1, t2] = L_m_analytic(d_t[t1], d_t[t2], h_cg);
                    L_matrix[t2, t1] = L_matrix[t1, t2];
                }
            }

            return L_matrix;
        }
        
        public static Matrix_d Calc_Cmatrix_old(double[] d_t, double h_cg, double r_c)
        {
            Matrix_d C_matrix = M_d.Dense(9, 9);

            for (int t = 0; t < 9; t++)
            {
                // Add Shunt C
                C_matrix[t, t] = C_tg_analytic(d_t[t], h_cg, r_c); // Capacitance to ground added to diagonal
            }

            // Add Series C
            for (int t1 = 0; t1 < 8; t1++)
            {
                for (int t2 = t1 + 1; t2 < 9; t2++)
                {
                    //if (t<8):
                    // C_tt = C_tt_getdp(d_t[t+1], d_t[t], r_c)
                    // C_matrix[t+1][t] = -C_tt # Capacitance between t and t+1
                    // C_matrix[t][t+1] = -C_tt
                    // C_matrix[t][t] += C_tt
                    // C_matrix[t+1][t+1] += C_tt
                    // Pseudo C-matrix
                    double C_tt = C_tt_analytic(d_t[t1], d_t[t2], r_c);
                    C_matrix[t1, t2] = C_tt; // Capacitance between t and t+1
                    C_matrix[t2, t1] = C_tt;
                }
            }

            return C_matrix;
        }

        public static double C_tt_analytic(double d1_mm, double d2_mm, double r_c_mm)
        {
            const double eps0 = 8.854e-12; // F/m
            const double eps_r = 1.0;

            double r_c = r_c_mm / 1000;
            double d = (Math.Abs(d2_mm - d1_mm) / 2 - 2 * r_c_mm) / 1000;
            double s = Math.Abs(d2_mm - d1_mm) / 2 / 1000;
            //return eps0 * eps_r * 2*r/s * (math.pi * (d1_mm+d2_mm)/2/1000)
            //return math.pi*eps0*eps_r/math.log(s/r) #/ (math.pi * (d1_mm+d2_mm)/2/1000)
            //return math.pi*eps0*eps_r/math.log(s/r)
            double r_avg = (d1_mm / 2 + d2_mm / 2) / 2 / 1000;
            return eps0 * eps_r * (2 * Math.PI * r_avg * 2 * r_c) / d;
        }

        public static double C_tg_analytic(double d_t, double h_cg, double r_c)
        {
            const double eps0 = 8.854e-12; // F/m
            const double eps_r = 1.0;

            // d_t is the centerline radius of the turn in mm
            double r = d_t / 2 / 1000;
            double h = h_cg / 1000;
            double a = r_c / 1000;
            double R = r + a; // outer radius in m
            double k = R * R / (R * R + h * h);
            //return 2*math.pi*eps0*eps_r / (r * math.cosh(h/r))'
            // From https://www.mdpi.com/1996-1073/11/5/1090/htm
            return 4 * Math.Pow(Math.PI, 2) * eps0 * eps_r * R / (Math.Log(8 * R / a) - Elliptic.EllipticK(k * k) * k);
            //return 2*math.pi*eps0*eps_r / math.log(2*h/r)
        }

        public static double L_s_analytic(double d_mm, double r_c_mm)
        {
            double r_c = r_c_mm / 1000;
            double r_t = d_mm / 2 / 1000;
            // Typical formula assuming 
            double L = 4 * Math.PI * 1e-7 * r_t * (Math.Log(8 * r_t / r_c) - 2.0); //1.75)
            double mu_0 = 4 * Math.PI * 1e-7;
            double k = Math.Sqrt(4 * r_t * (r_t - r_c) / Math.Pow(2 * r_t - r_c, 2));
            return mu_0 * Math.Sqrt(r_t * (r_t - r_c)) * ((2 / k - k) * Elliptic.EllipticK(k) - (2 / k) * Elliptic.EllipticE(k));
            // print(L)
            return L;
        }

        public static double L_m_analytic(double d1_mm, double d2_mm, double h_cg_mm)
        {
            double h = h_cg_mm / 1000;
            double s = Math.Abs(d1_mm - d2_mm) / 1000;
            double ra = d1_mm / 2 / 1000;
            double rb = d2_mm / 2 / 1000;
            if (rb < ra)
            {
                double tmp = rb;
                rb = ra;
                ra = tmp;
            }
            double mu_0 = 4 * Math.PI * 1e-7;
            double k = Math.Sqrt(4 * ra * rb / Math.Pow(ra + rb, 2));
            return mu_0 * Math.Sqrt(ra * rb) * ((2 / k - k) * Elliptic.EllipticK(k) - (2 / k) * Elliptic.EllipticE(k));
            //return math.pi * mu_0 * ra**2 / (2*rb)
        }

        public static double R_c_old(double r_c_mm, double thick_mm)
        {
            double r_o = r_c_mm / 1000; // convert to m
            double r_i = (r_c_mm - thick_mm) / 1000; // convert to m
            double rho_c = 1.68e-8; //ohm-m
            return rho_c / (Math.PI * (r_o * r_o - r_i * r_i));
        }

        #endregion
    }
}
