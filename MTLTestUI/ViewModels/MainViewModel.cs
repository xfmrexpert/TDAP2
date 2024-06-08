using Avalonia.Media;
using MathNet.Numerics;
using MathNet.Numerics.Data.Text;
using MathNet.Numerics.LinearAlgebra;
using System;
using System.Reflection;
using System.Threading.Tasks;

namespace MTLTestUI.ViewModels;

public partial class MainViewModel : ViewModelBase
{
    public string Greeting => "Welcome to Avalonia!";

    private MainModel _mainModel;

    public TDAP.Geometry Geometry { get; set; }

    public MainViewModel()
    {
        _mainModel = new MainModel();
        Geometry = _mainModel.GenerateGeometry();
        _mainModel.CalcMesh();
        _mainModel.CalcCapacitanceMatrix();
        int num_freqs = 10;
        double min_freq = 10e3;
        double max_freq = 1e6;
        var freqs = Generate.LogSpaced(num_freqs, Math.Log10(min_freq), Math.Log10(max_freq));
        _mainModel.CalcInductanceMatrix(60);
        foreach (var freq in freqs)
        {
            _mainModel.CalcInductanceMatrix(freq);
        }

        //CalcSelfInductancesOnly(60, 1.0);
        //CalcSelfInductancesOnly(60, 0.1);

        //CalcSelfInductancesOnly(60, 1.0, 2);

        //CalcSelfInductancesOnly(100e3, 1.0);
        //CalcSelfInductancesOnly(100e3, 0.1);

        //CalcSelfInductancesOnly(100e3, 1.0, 2);
        //CalcSelfInductancesOnly(100e3, 0.1, 2);
    }

    private void CalcSelfInductancesOnly(double freq, double meshscale = 1.0, int order = 1)
    {
        Console.WriteLine($"Mesh Scale: {meshscale}");

        int n_turns = 5; // _mainModel.num_discs * _mainModel.turns_per_disc;
        Vector<double> L_getdp = Vector<double>.Build.Dense(n_turns);

        Console.WriteLine($"Frequency: {freq.ToString("0.##E0")}");
        Console.WriteLine($"Mesh scale: {meshscale}");
        Console.WriteLine($"Order: {order}");
        _mainModel.CalcMesh(meshscale, order);

        //Parallel.For(0, n_turns, t =>
        for (int t = 0; t < n_turns; t++)
        {
            //L_getdp[t] = _mainModel.CalcInductance(t, -1, freq, order);
            (double r, double z) = _mainModel.GetTurnMidpoint(t);
            Console.WriteLine($"Self inductance for turn {t}: {L_getdp[t] / r / 1e-9}");
        }
        //);

        for (int t1 = 0; t1 < n_turns; t1++)
        {
            (double r, double z) = _mainModel.GetTurnMidpoint(t1);
            L_getdp[t1] = L_getdp[t1] / r;
        }

        //Console.Write((L_getdp/1e-9).ToVectorString());

        //DelimitedWriter.Write($"L_getdp_{freq.ToString("0.00E0")}.csv", L_getdp, ",");
        
    }
}
