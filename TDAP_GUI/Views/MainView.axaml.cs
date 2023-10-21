using Avalonia.Controls;
using TDAP_GUI.ViewModels;

namespace TDAP_GUI.Views;

public partial class MainView : UserControl
{
    public MainView()
    {
        InitializeComponent();
        DataContext = new MainViewModel();
    }
}
