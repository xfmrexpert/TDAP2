// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using Newtonsoft.Json;

namespace TDAP
{
    public class Segment
    {
        public Winding parentWdg { get; private set; }

        public string SegmentID { get; set; } = "Segment";

        public int turns_per_section {get; set;}
        public int n_sections {get; set;}
        public double radius_inner {get; set;}
        public int strands_per_turn {get; set;}
        public double h_abv_yoke {get; set;}
        public double t_cond {get; set;}
        public double h_cond {get; set;}
        public double t_ins {get; set;}
        public double r_cond_corner {get; set;}
        public double h_spacer {get; set;}

        [JsonIgnore]
        public double radius_outer
        {
            get
            {
                return radius_inner + turns_per_section * (t_cond + 2 * t_ins);
            }
        }

        // for now, anyway, height ignores spacers above top disc and below bottom disc
        [JsonIgnore]
        public double height
        {
            get
            {
                return n_sections * (h_cond + 2 * t_ins) + (n_sections - 1) * h_spacer;
            }
        }

        [JsonIgnore]
        public double Area
        {
            get
            {
                return (radius_outer - radius_inner) * height;
            }
        }

        [JsonIgnore]
        public int Turns
        {
            get
            {
                return turns_per_section * n_sections;
            }
        }

        [JsonIgnore]
        public double CurrentDensity
        {
            get
            {
                double J;
                J = 3600 / parentWdg.TotalArea;
                return J;
            }
        }

        public Segment(Winding parentwdg)
        {
            parentWdg = parentwdg;
        }

        public void WriteSegmentGmsh(GmshFile outFile)
        {
            outFile.gmshRectangle(radius_inner, h_abv_yoke, radius_outer, h_abv_yoke + height);
        }

    }
}