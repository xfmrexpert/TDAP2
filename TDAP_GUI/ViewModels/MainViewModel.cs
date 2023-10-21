using System.Collections.Generic;
using TDAP;

namespace TDAP_GUI.ViewModels;

public partial class MainViewModel : ViewModelBase
{
    public List<TransformerViewModel> TfmrVM { get; set; } = new List<TransformerViewModel>();

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
        Winding wdg = tfmr.AddNewWinding();
        wdg.WindingID = "Winding 1";
        Segment seg = wdg.AddNewSegment();
        seg.SegmentID = "Segment 1";
        seg = wdg.AddNewSegment();
        seg.SegmentID = "Segment 2";
        seg = wdg.AddNewSegment();
        seg.SegmentID = "Segment 3";
        return tfmr;
    }
}
