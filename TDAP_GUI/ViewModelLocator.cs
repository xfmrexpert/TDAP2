// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using CommunityToolkit.Mvvm.DependencyInjection;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TDAP_GUI.ViewModels;

namespace TDAP_GUI
{
    /// <summary>
    /// This class contains static references to all the view models in the
    /// application and provides an entry point for the bindings.
    /// </summary>
    public class ViewModelLocator
    {
        public MainViewModel MainView => Ioc.Default.GetRequiredService<MainViewModel>();
    }
}
