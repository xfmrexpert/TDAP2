// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathNet.Numerics.LinearAlgebra;
using System.Numerics;
using MatrixExponential;
using MathNet.Numerics;
using System.Diagnostics;
using LinAlg = MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.Distributions;
using System.Security.Cryptography;

namespace TDAP
{
    using Matrix_d = LinAlg.Matrix<double>;
    using Matrix_c = LinAlg.Matrix<Complex>;
    using Vector_d = LinAlg.Vector<double>;
    using Vector_c = LinAlg.Vector<Complex>;

    public class MTLModel
    {
        private MatrixBuilder<double> M_d = Matrix_d.Build;
        private MatrixBuilder<Complex> M_c = Matrix_c.Build;
        private VectorBuilder<double> V_d = Vector_d.Build;
        private VectorBuilder<Complex> V_c = Vector_c.Build;

        private Transformer tfmr;

        public MTLModel(Transformer _tfmr)
        {
            tfmr = _tfmr;
        }

        public void CalcL()
        {
            foreach (var wdg in tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    for (int i = 0; i < seg.n_sections; i++)
                    {
                        for (int j = 0; j < seg.turns_per_section; j++)
                        {
                        }
                    }
                }
            }
        }

        private double CalcSelfInductance(double h, double w, double r_avg)
        {
            double mu_0 = 4 * Math.PI * 1e-7;
            double GMD = Math.Exp(0.5 * Math.Log(h * h + w * w) + 2 * w / (3 * h) * Math.Atan(h / w) + 2 * h / (3 * w) * Math.Atan(w / h) - w * w / (12 * h * h) * Math.Log(1 + h * h / (w * w)) - h * h / (12 * w * w) * Math.Log(1 + w * w / (h * h)) - 25 / 12);
            double L_s = mu_0 * r_avg * Math.Log(8 * r_avg / GMD - 2);
            return L_s;
        }

        private double CalcInductanceCoaxLoops(double r_a, double z_a, double r_b, double z_b)
        {
            double mu_0 = 4 * Math.PI * 1e-7;
            double d = Math.Abs(z_b - z_a);
            double k = Math.Sqrt(4 * r_a * r_b / ((r_a + r_b) * (r_a + r_b) + d * d));
            double L_ab = 2 * mu_0 / k * Math.Sqrt(r_a * r_b) * ((1 - k * k / 2) * EllipticK(k) - EllipticE(k) + 1);
            return L_ab;
        }

        private (double r_i, double z_i, double r_j, double z_j) CalcLyleLoops(double r, double z, double w, double h)
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

        private double CalcMutualInductance_Lyle(double r_a, double z_a, double h_a, double w_a, double r_b, double z_b, double h_b, double w_b)
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
        public void CalcC()
        {
            double eps_0 = 8.852e-12;
            double eps_oil = 2.2;
            double eps_paper = 3.5;

            foreach (var wdg in tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    Matrix_d C_seg = M_d.Dense(seg.Turns, seg.Turns);

                    for (int i = 0; i < seg.n_sections; i++)
                    {
                        double C_abv;
                        double C_bel;
                        double dist_to_ground = 1000;

                        // For now, let's calculate four capacitances for each turn
                        // If first disc (section), above is next disc, below is tank
                        // TODO: How to handle segments above and below
                        if (i == 0)
                        {
                            C_abv = eps_0 * eps_oil * seg.t_cond / (seg.h_spacer + 2 * seg.t_ins);
                            C_bel = eps_0 * eps_oil * seg.t_cond / dist_to_ground;
                        }
                        else if (i == (seg.n_sections - 1)) // If last disc, above is tank, below is previous disc
                        {
                            C_abv = eps_0 * eps_oil * seg.t_cond / dist_to_ground;
                            C_bel = eps_0 * eps_oil * seg.t_cond / (seg.h_spacer + 2 * seg.t_ins);
                        }
                        else
                        {
                            C_abv = C_bel = eps_0 * eps_oil * seg.t_cond / (seg.h_spacer + 2 * seg.t_ins);
                        }

                        for (int j = 0; j < seg.turns_per_section; j++)
                        {                            
                            double C_lt;
                            double C_rt;

                            // If first turn in section, left is inner winding or core, right is next turn
                            if (j == 0)
                            {
                                C_lt = eps_0 * eps_paper * seg.h_cond / (2 * seg.t_ins);
                                C_rt = eps_0 * eps_paper * seg.h_cond / dist_to_ground;
                            }
                            else if (j == (seg.turns_per_section - 1)) // If last turn in section, left is previous turn, right is outer winding or tank
                            {
                                C_lt = eps_0 * eps_paper * seg.h_cond / dist_to_ground;
                                C_rt = eps_0 * eps_paper * seg.h_cond / (2 * seg.t_ins);
                            }
                            else
                            {
                                C_lt = C_rt = eps_0 * eps_paper * seg.h_cond / (2 * seg.t_ins);
                            }
                            
                            // Assemble C_abv, C_bel, C_lt, C_rt into C_seg
                            // TODO
                        }
                    }
                }
            }
        }

        // The following follows the axisymmetric MTL calulation outline in the Fattal paper
        // HA is lower left-hand quadrant of left-hand side matrix
        // HB is lower right-hand quadrant of the LHS matric
        // HA and HB are "terminal" constraints dictated by winding type (terminal here meaning the
        // terminations of each winding turn when viewed as parallel transmission lines)
        public Matrix_d CalcHA(int n)
        {
            Matrix_d HA11 = M_d.DenseIdentity(n);
            Matrix_d HA21 = M_d.Dense(n, n);
            Matrix_d HA12 = M_d.Dense(n, n);
            for (int t = 0; t < (n-1); t++)
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

        public Matrix_c CalcHB(int n, double f)
        {
            Matrix_c HB11 = M_c.Dense(n, n);
            HB11[0, 0] = 0.0; //Rs
            Matrix_c HB12 = M_c.Dense(n, n);
            Matrix_c HB21 = M_c.Dense(n, n);
            for (int t = 0; t < (n-1); t++)
            {
                HB21[t, t + 1] = 1.0;
            }
            Matrix_c HB22 = -1.0 * M_c.DenseIdentity(n);
            HB22[n - 1, n - 1] = Complex.ImaginaryOne * 2d * Math.PI * f; //Had a * 0 on the end?
            Matrix_c HB1 = HB11.Append(HB12);
            Matrix_c HB2 = HB21.Append(HB22);
            Matrix_c HB = HB1.Stack(HB2);
            return HB;
        }

        public Vector_c CalcResponseAtFreq(double f, Matrix_d Gamma, Matrix_d HA, Matrix_d R, Matrix_d K, Matrix_d L, Matrix_d C, int n)
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
        public List<Vector_c> CalcResponse(Vector_d r, Matrix_d R, Matrix_d K, Matrix_d L, Matrix_d C, int n)
        {
            // Gamma is the diagonal matrix of conductors radii (eq. 2)
            Matrix_d Gamma = M_d.DenseOfDiagonalVector(2d * Math.PI * r);
            Matrix_d HA = CalcHA(n);

            // Create vector of frequencies
            var freqs = Generate.LinearSpaced(10000, 100e3, 50e6);

            List<Vector_c> V_turn = new List<Vector_c>();
            var Y = new List<Complex>();


            foreach (var f in freqs)
            {
                var vi_vec = CalcResponseAtFreq(f, Gamma, HA, R, K, L, C, n);
                var gain_at_freq = vi_vec.SubVector(n, n) / vi_vec[0];
                V_turn.Add(gain_at_freq); //[n: 2 * n]
                Y.Add(vi_vec[2 * n]); //Original code took absolute val of vi_vec[2*n]
            }

            return V_turn;
        }

        // Impedances passed here are per turn, not per unit length
        // C matrix should be in self/mutual form, not Maxwell form
        public List<double[]> CalcResponseMTL(Matrix_d L_matrix, Matrix_d C_matrix, Vector_d R_t, Vector_d K_t, Vector_d d_t)
        {
            int numturns = d_t.Count;
            Matrix_d R_array = M_d.Dense(numturns, numturns);
            Matrix_d L_array = M_d.Dense(numturns, numturns);
            Matrix_d G_array = M_d.Dense(numturns, numturns);
            Matrix_d C_array = M_d.Dense(numturns, numturns);
            Matrix_d K_array = M_d.Dense(numturns, numturns);

            // Calculate PUL from turn impedances & matrix assembly
            for (int t = 0; t < numturns; t++)
            {
                // Add R
                R_array[t, t] = R_t[t];
                K_array[t, t] = K_t[t];

                // Add L & C, translating to PUL
                L_array[t, t] = L_matrix[t, t] / (Math.PI * d_t[t] / 1000);

                for (int t2 = t + 1; t2 < numturns; t2++)
                {
                    L_array[t, t2] = L_matrix[t, t2] / ((Math.PI * d_t[t] + Math.PI * d_t[t2]) / 2 / 1000);
                    L_array[t2, t] = L_matrix[t2, t] / ((Math.PI * d_t[t] + Math.PI * d_t[t2]) / 2 / 1000);
                }
            }

            for (int t = 0; t < numturns; t++)
            {
                C_array[t, t] = C_matrix[t, t] / (Math.PI * d_t[t] / 1000);
                for (int t2 = 0; t2 < numturns; t2++)
                {
                    if (t != t2)
                    {
                        C_array[t, t] += C_matrix[t, t2] / (Math.PI * d_t[t] / 1000);
                        C_array[t2, t2] += C_matrix[t, t2] / (Math.PI * d_t[t2] / 1000);
                        C_array[t, t2] = -C_matrix[t, t2] / ((Math.PI * d_t[t] + Math.PI * d_t[t2]) / 2 / 1000);
                        C_array[t2, t] = -C_matrix[t2, t] / ((Math.PI * d_t[t] + Math.PI * d_t[t2]) / 2 / 1000);
                    }
                }
            }

            // r is vector of turn radii in meters
            // d (given above) is turn diameters in mm
            var r = d_t / 2d / 1000d;
            var V_resp = CalcResponse(r, R_array, K_array, L_array, C_array, numturns);

            //V_response = []
            //for t in range(numturns):
            //    V_response.append([])
            //for Vf in V:
            //    for t in range(numturns - 1):
            //        V_response[t].append(20 * math.log10(abs(Vf[t])))

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

        const double eps0 = 8.852e-12; // F/m
        const double eps_r = 1.0;

        public double C_tt_analytic(double d1_mm, double d2_mm, double r_c_mm)
        {
            double r_c = r_c_mm / 1000;
            double d = (Math.Abs(d2_mm - d1_mm) / 2 - 2 * r_c_mm) / 1000;
            double s = Math.Abs(d2_mm - d1_mm) / 2 / 1000;
            //return eps0 * eps_r * 2*r/s * (math.pi * (d1_mm+d2_mm)/2/1000)
            //return math.pi*eps0*eps_r/math.log(s/r) #/ (math.pi * (d1_mm+d2_mm)/2/1000)
            //return math.pi*eps0*eps_r/math.log(s/r)
            double r_avg = (d1_mm / 2 + d2_mm / 2) / 2 / 1000;
            return eps0 * eps_r * (2 * Math.PI * r_avg * 2 * r_c) / d;
        }

        public double C_tg_analytic(double d_t, double h_cg, double r_c)
        {
            // d_t is the centerline radius of the turn in mm
            double r = d_t / 2 / 1000;
            double h = h_cg / 1000;
            double a = r_c / 1000;
            double R = r + a; // outer radius in m
            double k = R * R / (R * R + h * h);
            //return 2*math.pi*eps0*eps_r / (r * math.cosh(h/r))'
            // From https://www.mdpi.com/1996-1073/11/5/1090/htm
            return 4 * Math.Pow(Math.PI, 2) * eps0 * eps_r * R / (Math.Log(8 * R / a) - EllipticK(k * k) * k);
            //return 2*math.pi*eps0*eps_r / math.log(2*h/r)
        }

        public double L_s_analytic(double d_mm, double r_c_mm)
        {
            double r_c = r_c_mm / 1000;
            double r_t = d_mm / 2 / 1000;
            // Typical formula assuming 
            double L = 4 * Math.PI * 1e-7 * r_t * (Math.Log(8 * r_t / r_c) - 2.0); //1.75)
            double mu_0 = 4 * Math.PI * 1e-7;
            double k = Math.Sqrt(4 * r_t * (r_t - r_c) / Math.Pow(2 * r_t - r_c, 2));
            return mu_0 * Math.Sqrt(r_t * (r_t - r_c)) * ((2 / k - k) * EllipticK(k) - (2 / k) * EllipticE(k));
            // print(L)
            return L;
        }

        public double L_m_analytic(double d1_mm, double d2_mm, double h_cg_mm)
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
            return mu_0 * Math.Sqrt(ra * rb) * ((2 / k - k) * EllipticK(k) - (2 / k) * EllipticE(k));
            //return math.pi * mu_0 * ra**2 / (2*rb)
        }

        public double R_c(double r_c_mm, double thick_mm)
        {
            double r_o = r_c_mm / 1000; // convert to m
            double r_i = (r_c_mm - thick_mm) / 1000; // convert to m
            double rho_c = 1.68e-8; //ohm-m
            return rho_c / (Math.PI * (r_o * r_o - r_i * r_i));
        }

        public Matrix_d Calc_Lmatrix_analytic(double[] d_t, double h_cg, double r_c)
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

        public Matrix_d Calc_Cmatrix_analytic(double[] d_t, double h_cg, double r_c)
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

        //public double R_c(double r_c_mm, double thick_mm)
        //{
        //    double r_o = r_c_mm / 1000; //convert to m
        //    double r_i = (r_c_mm - thick_mm) / 1000; //convert to m
        //    double rho_c = 1.68e-8; //ohm-m
        //    return rho_c / (Math.PI * (r_o * r_o - r_i * r_i));
        //}

        // maximum number of iterations of a series
        public const int SeriesMax = 250;

        // Series for complete elliptic integral of the first kind (A&S 17.3.11):
        //   K(k) = (pi/2) ( 1 + (1/2 k)^2 + (1/2 k 3/4 k)^2 + (1/2 k 3/4 k 5/6 k)^2 + ... )

        private static double EllipticK_Series(double k)
        {
            double z = 1.0;
            double f = 1.0;
            for (int n = 1; n < SeriesMax; n++)
            {
                double f_old = f;
                z = z * (2 * n - 1) / (2 * n) * k;
                f += z * z;
                if (f == f_old) return (Math.PI / 2.0 * f);
            }
            throw new NonconvergenceException();
        }

        // Asymptotic expansion of Elliptic integral of first kind (DLMF 19.21.1)
        //   K(k) = \sum_n [ (1/2)_n k' / n! ]^2 [ ln(1/k') + Psi(n+1) - Psi(n+1/2) ]
        // Since this is a power series in k', it is good close to k = 1
        // Converges in 8 terms at k' = 0.1, 12 terms at k' = 0.25, 23 terms at k' = 0.5, 53 terms at k' = 0.75
        // Accurate even at k' = 0.75; because all terms are the same sign, no cancelations occur even when terms do not decrease rapidly

        private static double EllipticK_Asymptotic(double k1)
        {
            double p = 1.0;
            double q = 2.0 * Math.Log(2.0) - Math.Log(k1);
            double f = q;
            for (int n = 1; n < SeriesMax; n++)
            {
                double f_old = f;
                p *= k1 * (n - 0.5) / n;
                q -= 1.0 / (n * (2 * n - 1));
                double df = p * p * q;
                f += df;
                if (f == f_old) return (f);
            }
            throw new NonconvergenceException();
        }

        // K(k) can be expressed as an arithmetic-geometric mean (A&S 17.6)
        //   K(k) = = \frac{\pi}{2 AGM(1-k, 1+k)} = \frac{\pi}{2 AGM(1,k')}
        // AGM(a,b) is determined by taking the arithmetic mean (a+b)/2 and the geometric mean sqrt(ab) and re-inserting them as
        // arguments until convergence is achieved.

        private static double EllipticK_AGM(double k)
        {

            double a = 1.0;
            double b = Math.Sqrt((1.0 - k) * (1.0 + k));

            // Starting from 1-k, 1+k, the first iteration will always take us to 1, k',
            // so although it looks prettier (more symmetric) to start with 1+k, 1-k,
            // it's computationally faster to start with 1, k'.

            return 0.5 * Math.PI / AGM(a, b);

        }

        private static double AGM(double x, double y)
        {

            // The basic technique of AGM is to compute
            //   x_{n+1} = (x_{n} + y_{n}) / 2
            //   y_{n+1} = \sqrt{ x_{n} y_{n} }
            // until the both coverge to same value.

            // A naively implemented convergence test (x == y) can cause endless
            // looping because x and y dance around final value, changing the last
            // few bits each time in a cycle. So we have to terminate when "close enough".

            // You can do a series development to show that
            //   agm(c-d, c+d) = c [ 1 - 1/4 r^2 - 5/64 r^4 - O(r^6) \right]
            // where r = d/c, and the coefficient of r^6 is << 1.
            // So we can stop when (d/c)^6 ~ \epsilon, or
            // d ~ c \epsilon^(1/6), and then use this series to avoid
            // taking last few roots. By simple algebra,
            //   c = (x+y)/2  d = (x-y)/2
            // and we would need to form the sum to take the next iteration anyway.

            for (int n = 1; n < SeriesMax; n++)
            {

                double s = x + y;
                double t = x - y;

                if (Math.Abs(t) < Math.Abs(s) * agmEpsilon)
                {
                    double rSquared = Math.Pow(t / s, 2);
                    return 0.5 * s * (1.0 - rSquared * (1.0 / 4.0 + 5.0 / 64.0 * rSquared));
                }

                // This can go wrong when x * y underflows or overflows. This won't
                // happen when called for EllipticE since we start with x=1. But
                // don't make this method public until we can handle whole domain.
                y = Math.Sqrt(x * y);
                x = 0.5 * s;

            }

            throw new NonconvergenceException();

        }

        private static readonly double agmEpsilon = Math.Pow(1.0E-16, 1.0 / 6.0);

        public static double EllipticK (double k) {
            if (k < 0.0) {
                throw new ArgumentOutOfRangeException(nameof(k));
            } else if (k < 0.25) {
                // For small k, use the series near k~0.
                return (EllipticK_Series(k));
            } else if (k < 0.875) {
                // For intermediate k, use the AGM method.
                return (EllipticK_AGM(k));
            } else if (k < 1.0) {
                // For large k, use the asymptotic expansion. (k' = 0.484 at k=0.875)
                // Note Math.Sqrt((1.0-k)*(1.0+k)) is significantly more accurate for small 1-k
                // than Math.Sqrt(1.0-k*k), at the cost of one extra flop.
                // My testing indicates a rms error of 1E-17 vs 3E-14, max error of 1E-16 vs 2E-13.
                double k1 = Math.Sqrt((1.0 - k) * (1.0 + k));
                Debug.Assert(k1 > 0.0);
                return (EllipticK_Asymptotic(k1));
            } else if (k == 1.0) {
                return (Double.PositiveInfinity);
            } else if (k <= Double.PositiveInfinity) {
                throw new ArgumentOutOfRangeException(nameof(k));
            } else {
                Debug.Assert(Double.IsNaN(k));
                return (k);
            }
        }

        // Series for complete elliptic integral of the second kind
        // Converges in 7 terms for k = 0.1, 12 for k = 0.25, 22 for k = 0.5, 49 for k = 0.75; accurate for all these cases

        private static double EllipticE_Series(double k)
        {

            // (Pi/2) [ 1 - (1/2 k)^2 / 1 - (1/2 k 3/4 k)^2 / 3 - (1/2 k 3/4 k 5/6 k)^2 / 5 + ... ]

            double z = 1.0;
            double f = 1.0;
            for (int n = 1; n < SeriesMax; n++)
            {
                double f_old = f;
                z *= k * (n - 0.5) / n;
                f -= z * z / (2 * n - 1);
                if (f == f_old) return (Math.PI / 2.0 * f);
            }

            throw new NonconvergenceException();

        }

        // 7 terms at k'=0.1, 12 at k'=0.25, 23 at k'=0.5, 52 at k'=0.75

        private static double EllipticE_Asymptotic(double k1)
        {

            double k12 = k1 * k1;
            double p = k12 / 2.0;
            double q = 2.0 * Math.Log(2.0) - 0.5 - Math.Log(k1);
            double f = 1.0 + p * q;
            for (int m = 1; m < SeriesMax; m++)
            {
                double f_old = f;
                p *= (m - 0.5) * (m + 0.5) / m / (m + 1) * k12;
                q -= 1.0 / (2 * m - 1) / (2 * m) + 1.0 / (2 * m + 1) / (2 * m + 2);
                f += p * q;
                if (f == f_old) return (f);
            }
            throw new NonconvergenceException();

        }

        /// <summary>
        /// Computes the complete elliptic integral of the second kind.
        /// </summary>
        /// <param name="k">The elliptic modulus, which must lie between zero and one.</param>
        /// <returns>The value of the Legendre integral E(k).</returns>
        /// <exception cref="ArgumentOutOfRangeException"><paramref name="k"/> lies outside [0, 1].</exception>
        /// <remarks>
        /// <para>E(k) is defined as the complete elliptic integral:</para>
        /// <img src="../images/EllipticEIntegral.png" />
        /// <para>It appears in the Legendre reduction of integrals of rational funtions.</para>
        /// <para>The perimeter of an ellipse with major axis a and eccentricity e is 4 a E(e).</para>
        /// <para>Be aware that some authors use the the parameter m = k<sup>2</sup> instead of the modulus k.</para>
        /// </remarks>
        /// <seealso cref="EllipticE(double,double)"/>
        /// <seealso href="http://en.wikipedia.org/wiki/Elliptic_integral"/>
        /// <seealso href="http://mathworld.wolfram.com/CompleteEllipticIntegraloftheSecondKind.html"/>
        public static double EllipticE(double k)
        {
            // These expansions are accurate in the intermediate region, but require many terms.
            // It would be good to use a faster approach there, like we do for K.
            if (k < 0.0)
            {
                throw new ArgumentOutOfRangeException(nameof(k));
            }
            else if (k < 0.71)
            {
                return (EllipticE_Series(k));
            }
            else if (k < 1.0)
            {
                double k1 = Math.Sqrt((1.0 - k) * (1.0 + k));
                Debug.Assert(k1 > 0.0);
                return (EllipticE_Asymptotic(k1));
            }
            else if (k == 1.0)
            {
                return (1.0);
            }
            else if (k <= Double.PositiveInfinity)
            {
                throw new ArgumentOutOfRangeException(nameof(k));
            }
            else
            {
                Debug.Assert(Double.IsNaN(k));
                return (k);
            }
        }

        public class NonconvergenceException : Exception
        {

            /// <summary>
            /// Initializes a new non-convergence exception.
            /// </summary>
            public NonconvergenceException() : base("The algorithm did not converge within the allowed number of iterations.") { }

            /// <summary>
            /// Initializes a new non-convergence exception with the given exception message.
            /// </summary>
            /// <param name="message">The exception message.</param>
            public NonconvergenceException(String message) : base(message) { }

            /// <summary>
            /// Initializes a new non-convergence exception with the given exception message and inner exception.
            /// </summary>
            /// <param name="message">The exception message.</param>
            /// <param name="innerException">The inner exception.</param>
            public NonconvergenceException(String message, Exception innerException) : base(message, innerException) { }

        }

    }
}
