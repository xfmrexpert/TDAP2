﻿// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using Avalonia.Controls;
using Avalonia.Interactivity;
using TDAP_GUI.ViewModels;

namespace TDAP_GUI.Views;

public partial class MainView : UserControl
{
    public MainView()
    {
        InitializeComponent();
        DataContext = new MainViewModel();
    }

    private void RefreshClick(object sender, RoutedEventArgs e)
    {
        //do something on click
        plotCtl.InvalidateVisual();
    }
}
