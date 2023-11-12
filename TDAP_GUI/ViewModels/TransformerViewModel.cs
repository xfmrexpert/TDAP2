// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using System.Threading.Tasks;
using TDAP;
using static TDAP_GUI.ViewModels.MainViewModel;
using Newtonsoft.Json.Serialization;

namespace TDAP_GUI.ViewModels
{
    public partial class TransformerViewModel : ViewModelBase
    {
        public enum NumPhasesEnum { Single, Three };

        public Transformer Model { get; set; }

        public ObservableCollection<WindingViewModel> WindingVMs { get; set; } = new ObservableCollection<WindingViewModel>();

        public string? Filename { get; set; }
        
        public NumPhasesEnum NumPhases
        {
            get
            {
                if (Model.NumPhases == 1) return NumPhasesEnum.Single;
                else if (Model.NumPhases == 3) return NumPhasesEnum.Three;
                else throw new Exception("Invalid number of phases");
            }
            set
            {
                if (value == NumPhasesEnum.Single) Model.NumPhases = 1;
                else if (value == NumPhasesEnum.Three) Model.NumPhases = 3;
                else throw new Exception("Invalid number of phases");
            }
        }

        public int NumPhasesIdx
        {
            get
            {
                if (Model.NumPhases == 1) return 0;
                else if (Model.NumPhases == 3) return 1;
                else throw new Exception("Invalid number of phases");
            }
            set
            {
                if (value == 0) Model.NumPhases = 1;
                else if (value == 1) Model.NumPhases = 3;
                else throw new Exception("Invalid index for number of phases");
            }
        }

        public double Dist_WdgTank
        {
            get => Model.Dist_WdgTank;
            set => Model.Dist_WdgTank = value;
        }

        [RelayCommand]
        public void AddWinding()
        {
            WindingVMs.Add(new WindingViewModel(Model.AddNewWinding(), this));
        }

        public TransformerViewModel(Transformer model)
        {
            Model = model;
            WindingVMs = new ObservableCollection<WindingViewModel>();
            foreach (var wdg in model.Windings)
            {
                WindingVMs.Add(new WindingViewModel(wdg, this));
            }
        }

        public void LoadFromFile()
        {
            string jsonString = File.ReadAllText(Filename!);
            Model = JsonConvert.DeserializeObject<Transformer>(jsonString, new JsonSerializerSettings
            {
                PreserveReferencesHandling = PreserveReferencesHandling.Objects,
                MetadataPropertyHandling = MetadataPropertyHandling.ReadAhead
            })!;
        }

        public void StoreToFile()
        {
            
            string jsonString = JsonConvert.SerializeObject(Model, Formatting.Indented, new JsonSerializerSettings
            {
                PreserveReferencesHandling = PreserveReferencesHandling.Objects
            });
            File.WriteAllText(Filename!, jsonString);
        }

    }
}
