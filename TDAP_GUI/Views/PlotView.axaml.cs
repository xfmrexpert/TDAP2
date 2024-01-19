// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Shapes;
using Avalonia.Media;
using CommunityToolkit.Mvvm.Messaging;
using TDAP;
using TDAP_GUI.ViewModels;

namespace TDAP_GUI.Views
{
    public partial class PlotView : UserControl
    {
        public static readonly StyledProperty<Transformer> TfmrProperty =
            AvaloniaProperty.Register<PlotView, Transformer>(nameof(Tfmr));

        public Transformer Tfmr
        {
            get => GetValue(TfmrProperty);
            set
            {
                SetValue(TfmrProperty, value);
                DrawTfmr();
            }
        }

        public PlotView()
        {
            InitializeComponent();
            WeakReferenceMessenger.Default.Register<MainViewModel.RefreshPlotMessage>(this, (r, m) =>
            {
                DrawTfmr();
            });
            //DrawTfmr();
        }

        public void DrawTfmr()
        {
            foreach (var wdg in Tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    AddRectangle(seg.radius_inner - Tfmr.Core.LegRadius, seg.h_abv_yoke, seg.height, seg.radius_outer - seg.radius_inner);
                }
            }
            var model = new DetailedModel(Tfmr);
            model.GenerateGeometry();
            var pen = new Pen(Brushes.Green, 1, lineCap: PenLineCap.Square);
            TDAP.Geometry geom = model.Geometry;
            foreach (var line in geom.Lines)
            {
                Line ln = new Line() { StartPoint = new Point(line.pt1.x * 1000, line.pt1.y * 1000), EndPoint = new Point(line.pt2.x * 1000, line.pt2.y * 1000), Stroke = new SolidColorBrush(Colors.Green), StrokeThickness = .1 };
                mainCanvas.Children.Add(ln);
            }
            zoomBorder.EnableZoom = true;
            zoomBorder.EnablePan = true;
            zoomBorder.UniformToFill();
        }

        private void AddRectangle(double left, double bottom, double height, double width)
        {
            Rectangle rect = new Rectangle() { Height = height*1000, Width = width*1000, Stroke = new SolidColorBrush(Colors.Red), StrokeThickness=.1 };
            Canvas.SetLeft(rect, left*1000);
            Canvas.SetBottom(rect, bottom*1000);
            mainCanvas.Children.Add(rect);
            
        }
    }
}
