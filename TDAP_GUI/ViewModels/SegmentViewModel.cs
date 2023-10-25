// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using Microsoft.CodeAnalysis.Emit;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TDAP;

namespace TDAP_GUI.ViewModels
{
    public class SegmentViewModel : ViewModelBase
    {
        private Segment Model;

        public string SegmentID
        {
            get => Model.SegmentID; set => Model.SegmentID = value;
        }

        public int turns_per_section
        {
            get => Model.turns_per_section;
            set => Model.turns_per_section = value;
        }

        public int n_sections
        {
            get => Model.n_sections;
            set => Model.n_sections = value;
        }

        public double radius_inner
        {
            get => Model.radius_inner;
            set => Model.radius_inner = value;
        }

        public int strands_per_turn
        {
            get => Model.strands_per_turn; 
            set => Model.strands_per_turn = value;
        }

        public double h_abv_yoke
        {
            get => Model.h_abv_yoke;
            set => Model.h_abv_yoke = value;
        }

        public double t_cond
        {
            get => Model.t_cond; 
            set => Model.t_cond = value;
        }

        public double h_cond
        {
            get => Model.h_cond; 
            set => Model.h_cond = value;
        }

        public double t_ins
        {
            get => Model.t_ins; 
            set => Model.t_ins = value;
        }

        public double r_cond_corner
        {
            get => Model.r_cond_corner; 
            set => Model.r_cond_corner = value;
        }

        public double h_spacer
        {
            get => Model.h_spacer; 
            set => Model.h_spacer = value;
        }

        public SegmentViewModel(Segment model)
        {
            Model = model;
        }
    }
}
