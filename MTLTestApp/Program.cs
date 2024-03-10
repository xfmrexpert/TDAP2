using MathNet.Numerics.LinearAlgebra;
using Plotly.NET.LayoutObjects;
using Plotly.NET;
using System.Numerics;
using TDAP;
using System;
using Plotly.NET;
using Plotly.NET.LayoutObjects;
using MathNet.Numerics;
using Microsoft.FSharp.Collections;

namespace MTLTestApp
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            // Define geometry
            int numturns = 9;
            double d_c = 6.35; //turn diameter in mm
            double h_cg = 97; //height above ground plane, mm
            double t_cwall = 0.0285 * 25.4; //conductor thickness, mm
            double[] d_t = { 641.5, 603.5, 566, 527, 478.5, 431, 392.5, 354, 315 };

            double r_c = d_c / 2.0;
            MTLModel mtl = new MTLModel();

            Matrix<double> L_matrix_analytic = mtl.Calc_Lmatrix_analytic(d_t, h_cg, r_c);
            Matrix<double> C_matrix_analytic = mtl.Calc_Cmatrix_analytic(d_t, h_cg, r_c);

            double[] R_t = new double[numturns];
            for (int t = 0; t < numturns; t++)
            {
                R_t[t] = mtl.R_c(r_c, t_cwall);
            }

            //TODO: Factor in skin effect mo' better
            double[] K_t = new double[numturns];
            double rho_c = 1.68e-8; //ohm-m
            for (int t = 0; t < numturns; t++)
            {
                K_t[t] = 2 / (r_c / 1000) * Math.Sqrt(rho_c * 4 * Math.PI * 1e-7 / Math.PI);
            }

            var V = MathNet.Numerics.LinearAlgebra.Vector<double>.Build;
            var V_response_analytic = mtl.CalcResponseMTL(L_matrix_analytic, C_matrix_analytic, V.Dense(R_t), V.Dense(K_t), V.Dense(d_t));

            var freqs = Generate.LinearSpaced(10000, 100e3, 50e6);

            double[] x = freqs;
            double[] y = V_response_analytic[1];

            LinearAxis xAxis = new LinearAxis();
            xAxis.SetValue("title", "xAxis");
            xAxis.SetValue("showgrid", false);
            xAxis.SetValue("showline", true);

            LinearAxis yAxis = new LinearAxis();
            yAxis.SetValue("title", "yAxis");
            yAxis.SetValue("showgrid", false);
            yAxis.SetValue("showline", true);

            Layout layout = new Layout();
            layout.SetValue("xaxis", xAxis);
            layout.SetValue("yaxis", yAxis);
            layout.SetValue("showlegend", true);

            var traces = new List<Trace>();
            for (int t = 0; t < numturns - 1; t++)
            {
                Trace trace = new Trace("scatter");
                trace.SetValue("x", freqs);
                trace.SetValue("y", V_response_analytic[t]);
                trace.SetValue("mode", "lines");
                trace.SetValue("name", $"Turn {t+1}");
                traces.Add(trace);
            }

            GenericChart
                .ofTraceObjects(true, ListModule.OfSeq(traces))
                .WithLayout(layout)
                .Show();
        }
    }
}
