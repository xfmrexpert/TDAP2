using Avalonia.Media;
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
        _mainModel.CalcCapacitanceMatrix();
    }
}
