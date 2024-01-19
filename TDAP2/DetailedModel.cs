// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TDAP
{
    public class DetailedModel
    {
        private Transformer tfmr;

        public Geometry Geometry { get; set; }

        public DetailedModel(Transformer _tfmr)
        {
            tfmr = _tfmr;
        }

        public void GenerateGeometry()
        {
            Geometry = new Geometry();

            foreach (var wdg in tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    for (var i = 0; i < seg.n_sections; i++)
                    {
                        for (var j = 0; j < seg.turns_per_section; j++)
                        {
                            // FIXME: Need to sort out how to handle spacers above and below segments
                            double x_ll = seg.radius_inner + j * (seg.t_cond + 2 * seg.t_ins);
                            double y_ll = seg.h_abv_yoke + i * (seg.h_cond + 2 * seg.t_ins) + seg.h_spacer;
                            GenerateTurn(x_ll, y_ll, seg.t_cond, seg.h_cond, seg.t_ins, seg.r_cond_corner);
                        }
                    }
                }
            }
        }

        private void GenerateTurn(double x_ll, double y_ll, double w_cond, double h_cond, double t_ins, double radius_corner_cond)
        {
            // Conductor
            GenerateRoundedRect(x_ll, y_ll, h_cond, w_cond, radius_corner_cond);
            // Insulation
            GenerateRoundedRect(x_ll - t_ins, y_ll - t_ins, h_cond + 2*t_ins, w_cond + 2*t_ins, radius_corner_cond + t_ins);
            // TODO: Establish how to deal with holes (ie. cut conductor out of insulation rectangle)
        }

        private void GenerateRoundedRect(double x_ll, double y_ll, double height, double width, double radius_corner)
        {
            //FIXME: Need to store all of these primitives
            GeomPoint ll1 = Geometry.AddPoint(x_ll + radius_corner, y_ll);
            GeomPoint ll2 = Geometry.AddPoint(x_ll, y_ll + radius_corner);
            GeomPoint ul1 = Geometry.AddPoint(x_ll, y_ll + height - radius_corner);
            GeomPoint ul2 = Geometry.AddPoint(x_ll + radius_corner, y_ll + height);
            GeomPoint ur1 = Geometry.AddPoint(x_ll + width - radius_corner, y_ll + height);
            GeomPoint ur2 = Geometry.AddPoint(x_ll + width, y_ll + height - radius_corner);
            GeomPoint lr1 = Geometry.AddPoint(x_ll + width, y_ll + radius_corner);
            GeomPoint lr2 = Geometry.AddPoint(x_ll + width - radius_corner, y_ll);

            GeomLine left = Geometry.AddLine(ll2, ul1);
            GeomLine top = Geometry.AddLine(ul2, ur1);
            GeomLine right = Geometry.AddLine(ur2, lr1);
            GeomLine bottom = Geometry.AddLine(lr2, ll1);

            const double pi_over_2 = Math.PI / 2d;
            GeomArc ll = Geometry.AddArc(ll1, ll2, radius_corner, pi_over_2);
            GeomArc ul = Geometry.AddArc(ul1, ul2, radius_corner, pi_over_2);
            GeomArc ur = Geometry.AddArc(ur1, ur2, radius_corner, pi_over_2);
            GeomArc lr = Geometry.AddArc(lr1, lr2, radius_corner, pi_over_2);

        }
    }
}
