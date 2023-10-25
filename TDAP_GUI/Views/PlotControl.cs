// Copyright 2023, T. C. Raymond
// SPDX-License-Identifier: MIT

using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Threading;
using Avalonia;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TDAP;

namespace TDAP_GUI.Views
{
    public class PlotControl : Control
    {
        static PlotControl()
        {
            AffectsRender<PlotControl>(TfmrProperty);
        }

        public PlotControl()
        {
            
        }

        public static readonly StyledProperty<Transformer> TfmrProperty =
            AvaloniaProperty.Register<PlotControl, Transformer>(nameof(Tfmr));

        public Transformer Tfmr
        {
            get => GetValue(TfmrProperty);
            set => SetValue(TfmrProperty, value);
        }

        public override void Render(DrawingContext drawingContext)
        {
            if (double.IsNaN(Bounds.Height) || double.IsNaN(Bounds.Width)) return;

            var pen = new Pen(Brushes.Green, 20, lineCap: PenLineCap.Square);
            var boundPen = new Pen(Brushes.Red);

            double maxRadius = 0;
            foreach (var wdg in Tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    if (seg.radius_outer > maxRadius) maxRadius = seg.radius_outer;
                }
            }

            var height = Tfmr.Core.WindowHeight;
            var width = maxRadius + Tfmr.Dist_WdgTank - Tfmr.Core.LegRadius;

            double scale = 1.0;
            if ((Bounds.Height / height) > (Bounds.Width / width))
            {
                scale = Bounds.Width / width;
                //scale = Bounds.Height / height;
            }
            else
            {
                scale = Bounds.Height / height;
                //scale = Bounds.Width / width;
            }

            drawingContext.DrawRectangle(boundPen, new Rect(0, 0, width*scale, height*scale));

            if (Tfmr != null)
            {
                foreach (var wdg in Tfmr.Windings)
                {
                    foreach (var seg in wdg.Segments)
                    {
                        Point ul = new Point(seg.radius_inner - Tfmr.Core.LegRadius, height - (seg.h_abv_yoke + seg.height));
                        Point lr = new Point(seg.radius_outer - Tfmr.Core.LegRadius, height - seg.h_abv_yoke);
                        Rect rect = new Rect(ul*scale, lr*scale);
                        drawingContext.DrawRectangle(boundPen, rect);
                    }
                }
            }
        }
    }
}
