using MathNet.Numerics.LinearAlgebra;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using LinAlg = MathNet.Numerics.LinearAlgebra;

//OLD SHIT FOR DEBUGGING.  NOT USEFUL.

namespace MTLTestApp
{
    using Matrix_d = LinAlg.Matrix<double>;
    using Matrix_c = LinAlg.Matrix<Complex>;
    using Vector_d = LinAlg.Vector<double>;
    using Vector_c = LinAlg.Vector<Complex>;
    using static Plotly.NET.StyleParam;

    internal class Spiral
    {
        static private MatrixBuilder<double> M_d = Matrix_d.Build;
        static private MatrixBuilder<Complex> M_c = Matrix_c.Build;
        static private VectorBuilder<double> V_d = Vector_d.Build;
        static private VectorBuilder<Complex> V_c = Vector_c.Build;

        static public void ScratchPad()
        {
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
