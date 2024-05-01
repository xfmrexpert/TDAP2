using Avalonia.Media;
using MathNet.Numerics;
using System;
using System.Reflection;

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
        //_mainModel.CalcMesh();
        int num_freqs = 10;
        double min_freq = 10e3;
        double max_freq = 50e6;
        var freqs = Generate.LogSpaced(num_freqs, Math.Log10(min_freq), Math.Log10(max_freq));

        foreach (var freq in freqs)
        {
            _mainModel.CalcInductanceMatrix(freq);
        }
    }
}
