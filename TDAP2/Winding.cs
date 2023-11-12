// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using System.Collections.Generic;
using System;
using Newtonsoft.Json;

namespace TDAP
{
    public class Winding
    {
        public string WindingID { get; set; } = "Winding";
        public double MVA {get; set;}
        public double Voltage {get; set;}
        public int CurrentDirection {get; set;}


        public Transformer Tfmr {get; private set;}
        public List<Segment> Segments {get; set;} = new List<Segment>();

        public Winding(Transformer tfmr)
        {
            Tfmr = tfmr;
        }

        public Segment AddNewSegment()
        {
            Segment newSegment = new Segment(this);
            Segments.Add(newSegment);
            return newSegment;
        }

        public void WriteSegmentsGmsh(GmshFile outFile)
        {
            foreach (var segment in Segments)
            {
                segment.WriteSegmentGmsh(outFile);
            }
        }

        [JsonIgnore]
        public double TotalArea
        {
            get
            {
                double area = 0;
                foreach (var segment in Segments)
                {
                    area += segment.Area;
                }
                return area;
            }
        }

        [JsonIgnore]
        public int TotalTurns
        {
            get
            {
                int turns = 0;
                foreach (var segment in Segments)
                {
                    turns += segment.Turns;
                }
                return turns;
            }
        }

        [JsonIgnore]
        public double Current
        {
            get
            {
                if (Tfmr.NumPhases == 1)
                {
                    return MVA / (Voltage / Math.Sqrt(3)) * 1000 * CurrentDirection;
                }
                else if (Tfmr.NumPhases == 3)
                {
                    return MVA / 3 / (Voltage / Math.Sqrt(3)) * 1000 * CurrentDirection;
                }
                else
                {
                    throw new Exception("Number of phases needs to be 1 or 3.");
                }
            }
        }


    }
}