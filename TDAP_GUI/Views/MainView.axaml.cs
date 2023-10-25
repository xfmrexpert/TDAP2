// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

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
