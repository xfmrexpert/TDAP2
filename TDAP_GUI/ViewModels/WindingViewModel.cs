using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using TDAP;

namespace TDAP_GUI.ViewModels
{
    public class WindingViewModel : ViewModelBase
    {
        private Winding Model;

        public List<SegmentViewModel> SegmentVMs { get; set; }

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

        public void AddNewSegment()
        {
            SegmentVMs.Add(new SegmentViewModel(Model.AddNewSegment()));
        }

        public WindingViewModel(Winding model)
        {
            Model = model;
            SegmentVMs = new List<SegmentViewModel>();
            foreach (var segment in model.Segments) 
            {
                SegmentVMs.Add(new SegmentViewModel(segment));
            }
        }
    }
}
