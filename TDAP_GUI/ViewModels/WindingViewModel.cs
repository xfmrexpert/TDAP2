// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using TDAP;

namespace TDAP_GUI.ViewModels
{
    public partial class WindingViewModel : ViewModelBase
    {
        private Winding Model;

        private TransformerViewModel parentTfmrVM;

        public ObservableCollection<SegmentViewModel> SegmentVMs { get; set; }

        public string WindingID
        {
            get => Model.WindingID;
            set => Model.WindingID = value;
        }

        public double MVA
        {
            get => Model.MVA;
            set => Model.MVA = value;
        }

        public double Voltage
        {
            get => Model.Voltage;
            set => Model.Voltage = value;
        }

        public int CurrentDirection
        {
            get => Model.CurrentDirection;
            set => Model.CurrentDirection = value;
        }

        [RelayCommand]
        public void AddSegment()
        {
            SegmentVMs.Add(new SegmentViewModel(Model.AddNewSegment(), this));
        }

        public WindingViewModel(Winding model, TransformerViewModel parentTfmrVM)
        {
            Model = model;
            this.parentTfmrVM = parentTfmrVM;
            SegmentVMs = new ObservableCollection<SegmentViewModel>();
            foreach (var segment in model.Segments) 
            {
                SegmentVMs.Add(new SegmentViewModel(segment, this));
            }
        }

        [RelayCommand]
        public void DeleteWinding()
        {
            Model.Tfmr.Windings.Remove(Model);
            parentTfmrVM.WindingVMs.Remove(this);
        }
    }
}
