// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class DetailedModel
    {
        private Transformer tfmr;

        public DetailedModel(Transformer _tfmr)
        {
            tfmr = _tfmr;
        }

        public void GenerateTurn(double x_ll, double y_ll, double w_cond, double h_cond, double t_ins, double radius_corner_cond)
        {
            //Conductor
            GenerateRoundedRect(x_ll, y_ll, radius_corner_cond);
            // Insulation
            GenerateRoundedRect(x_ll - t_ins, y_ll - t_ins, radius_corner_cond + t_ins);
        }

        private void GenerateRoundedRect(double x_ll, double y_ll, double radius_corner)
        {

        }
    }
}
