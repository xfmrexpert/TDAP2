// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using HanumanInstitute.MvvmDialogs.Avalonia;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP_GUI
{
    /// <summary>
    /// Maps view models to views in Avalonia.
    /// </summary>
    public class ViewLocator : ViewLocatorBase
    {
        /// <inheritdoc />
        protected override string GetViewName(object viewModel) => viewModel.GetType().FullName!.Replace("ViewModel", "");
    }
}
