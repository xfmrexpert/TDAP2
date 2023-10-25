// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System.Collections.Generic;
using TDAP;

namespace TDAP_GUI.ViewModels;

public partial class MainViewModel : ViewModelBase
{
    public List<TransformerViewModel> TfmrVM { get; set; } = new List<TransformerViewModel>();

    public Transformer Tfmr
    {
        get
        {
            return TfmrVM[0].Model;
        }
    }

    private ViewModelBase _selectedItem = new ViewModelBase();
    public ViewModelBase SelectedItem
    {
        get => _selectedItem;
        private set => SetProperty(ref _selectedItem, value);
    }

    public MainViewModel()
    {
        var tfmr = mockTransformer();
        TransformerViewModel tfmrVM = new TransformerViewModel(tfmr);
        TfmrVM.Add(tfmrVM);
        _selectedItem = tfmrVM;
    }

    private Transformer mockTransformer()
    {
        Transformer tfmr = new Transformer();
        tfmr.Core.LegRadius = in_to_m(4);
        tfmr.Core.WindowHeight = in_to_m(55.9);
        tfmr.Dist_WdgTank = in_to_m(10);
        Winding wdg = tfmr.AddNewWinding();
        wdg.WindingID = "Winding 1";
        Segment seg = wdg.AddNewSegment();
        seg.SegmentID = "Segment 1";
        seg.radius_inner = in_to_m(4.75);
        seg.n_sections = 40;
        seg.h_spacer = in_to_m(0.125);
        seg.turns_per_section = 4;
        seg.t_cond = in_to_m(0.2175);
        seg.h_cond = in_to_m(0.925625);
        seg.t_ins = in_to_m(0.05);
        seg.h_abv_yoke = in_to_m(5);
        wdg = tfmr.AddNewWinding();
        wdg.WindingID = "Winding 2";
        seg = wdg.AddNewSegment();
        seg.SegmentID = "Segment 2";
        seg.radius_inner = in_to_m(4.75+1.27+6.98);
        seg.n_sections = 10;
        seg.h_spacer = in_to_m(0.125);
        seg.turns_per_section = 4;
        seg.t_cond = in_to_m(0.2175);
        seg.h_cond = in_to_m(1.0125);
        seg.t_ins = in_to_m(0.05);
        seg.h_abv_yoke = in_to_m(5+12.25+2*10.7);
        seg = wdg.AddNewSegment();
        seg.SegmentID = "Segment 3";
        seg.radius_inner = in_to_m(4.75 + 1.27 + 6.98);
        seg.n_sections = 10;
        seg.h_spacer = in_to_m(0.125);
        seg.turns_per_section = 4;
        seg.t_cond = in_to_m(0.2175);
        seg.h_cond = in_to_m(1.0125);
        seg.t_ins = in_to_m(0.05);
        seg.h_abv_yoke = in_to_m(5);
        return tfmr;
    }

    private double in_to_m(double x)
    {
        return x * 0.0254;
    }
}
