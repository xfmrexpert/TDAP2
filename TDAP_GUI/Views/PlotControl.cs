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
using Avalonia.Data;
using Microsoft.CodeAnalysis;
using static System.Collections.Specialized.BitVector32;
using System.Numerics;
using System.Xml.Linq;
using CommunityToolkit.Mvvm.Messaging;
using TDAP_GUI.ViewModels;
using Avalonia.Input;
using Avalonia.Visuals.Platform;

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
            WeakReferenceMessenger.Default.Register<MainViewModel.RefreshPlotMessage>(this, (r, m) =>
            {
                this.InvalidateVisual();
            });

            PointerWheelChanged += OnWheel;
        }

        protected void OnWheel(object sender, PointerWheelEventArgs e) {
            scale = scale * (1 + 0.1 * e.Delta.Y / Math.Abs(e.Delta.Y));
            this.InvalidateVisual();
        }

        public static readonly StyledProperty<Transformer> TfmrProperty =
            AvaloniaProperty.Register<PlotControl, Transformer>(nameof(Tfmr));

        public Transformer Tfmr
        {
            get => GetValue(TfmrProperty);
            set
            {
                SetValue(TfmrProperty, value);
                SetInitialScale();
            }
        }

        private double scale = 1.0;
        private double height = 1.0;

        public void SetInitialScale()
        {
            double maxRadius = 0;
            foreach (var wdg in Tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    if (seg.radius_outer > maxRadius) maxRadius = seg.radius_outer;
                }
            }

            height = Tfmr.Core.WindowHeight;
            var width = maxRadius + Tfmr.Dist_WdgTank - Tfmr.Core.LegRadius;

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
        }

        public override void Render(DrawingContext drawingContext)
        {
            if (scale == 1.0) SetInitialScale();
            if (double.IsNaN(Bounds.Height) || double.IsNaN(Bounds.Width)) return;

            var pen = new Pen(Brushes.Green, 1, lineCap: PenLineCap.Square);
            var boundPen = new Pen(Brushes.Red);

            double maxRadius = 0;
            foreach (var wdg in Tfmr.Windings)
            {
                foreach (var seg in wdg.Segments)
                {
                    if (seg.radius_outer > maxRadius) maxRadius = seg.radius_outer;
                }
            }
            var width = maxRadius + Tfmr.Dist_WdgTank - Tfmr.Core.LegRadius;
            drawingContext.FillRectangle(Brushes.Black, new Rect(0, 0, width * scale, height * scale));
            drawingContext.DrawRectangle(boundPen, new Rect(0, 0, width * scale, height * scale));
            
            bool simple = false;
            if (Tfmr != null)
            {
                if (simple)
                {
                    foreach (var wdg in Tfmr.Windings)
                    {
                        foreach (var seg in wdg.Segments)
                        {
                            Point ul = new Point(seg.radius_inner - Tfmr.Core.LegRadius, height - (seg.h_abv_yoke + seg.height));
                            Point lr = new Point(seg.radius_outer - Tfmr.Core.LegRadius, height - seg.h_abv_yoke);
                            Rect rect = new Rect(ul * scale, lr * scale);
                            drawingContext.DrawRectangle(boundPen, rect);
                        }
                    }

                    if (Tfmr.Mesh.numNodes() > 0)
                    {
                        RenderLeakageFlux(drawingContext);
                    }
                }
                else
                {
                    RenderDetailedModel(drawingContext);
                }
            }
            
            
            
        }

        private void RenderDetailedModel(DrawingContext drawingContext)
        {
            var model = new DetailedModel(Tfmr);
            model.GenerateGeometry();
            var pen = new Pen(Brushes.Green, 1, lineCap: PenLineCap.Square);
            TDAP.Geometry geom = model.Geometry;
            height = Tfmr.Core.WindowHeight;
            foreach (var line in geom.Lines)
            {
                drawingContext.DrawLine(pen, new Point(line.pt1.x * scale, (height - line.pt1.y) * scale), new Point(line.pt2.x * scale, (height - line.pt2.y) * scale));
            }
            foreach (var arc in geom.Arcs)
            {
                var sg = new StreamGeometry();
                using (var sgc = sg.Open())
                {
                    sgc.BeginFigure(new Point(arc.StartPt.x * scale, (height - arc.StartPt.y) * scale), false);
                    sgc.ArcTo(new Point(arc.EndPt.x * scale, (height - arc.EndPt.y) * scale), new Size(arc.Radius * scale, arc.Radius * scale), arc.SweepAngle, false, SweepDirection.Clockwise);
                    sgc.EndFigure(false);
                    drawingContext.DrawGeometry(Brushes.Red, pen, sg);
                }
                
            }
        }

        private void RenderLeakageFlux(DrawingContext drawingContext)
        {     
            double min = 0;
            double max = 0;

            double xmin = 99999;
            double xmax = 0;
            double ymin = 99999;
            double ymax = 0;

            int numContours = 30;

            //first pass through to get min an max	
            foreach (var node in Tfmr.Mesh.getNodes())
            {
                var DOFs = node.getDOFs();
                var dof = DOFs.First();
                double DOF_value = DOFs[0].get_value() * 2 * Math.PI * node.x();
                if (DOF_value < min)
                {
                    min = DOF_value;
                }
                if (DOF_value > max)
                {
                    max = DOF_value;
                }
                if (node.x() < xmin)
                {
                    xmin = node.x();
                }
                if (node.x() > xmax)
                {
                    xmax = node.x();
                }
                if (node.y() < ymin)
                {
                    ymin = node.y();
                }
                if (node.y() > ymax)
                {
                    ymax = node.y();
                }
            }

            double pltInterval = (max - min) / (numContours - 1);
            //TDAP::frmContourPlot ^ plotForm = gcnew TDAP::frmContourPlot();
            //plotForm->Show();

            var pen = new Pen(Brushes.Gray);

            for (int i = 0; i < numContours; i++)
            {
                double contLine = min + i * pltInterval;
                foreach (var face in Tfmr.Mesh.getFaces())
                {
                    double[] x = new double[2];
                    double[] y = new double[2];
                    int pt_idx = 0;
                    foreach (var edge in face.MeshEdges)
                    {
                        Node node1 = edge.getVertex(0).getNodes()[0];
                        DOF dof1 = node1.getDOFs()[0];
                        Node node2 = edge.getVertex(1).getNodes()[0];
                        DOF dof2 = node2.getDOFs()[0];
                        double dof1_value = dof1.get_value() * 2 * Math.PI * node1.x();
                        double dof2_value = dof2.get_value() * 2 * Math.PI * node2.x();

                        if (((dof1_value >= contLine) && (dof2_value < contLine)) || ((dof1_value <= contLine) && (dof2_value > contLine)))
                        {
                            //draw line
                            x[pt_idx] = node1.x() + (node2.x() - node1.x()) * (contLine - dof1_value) / (dof2_value - dof1_value);
                            y[pt_idx] = node1.y() + (node2.y() - node1.y()) * (contLine - dof1_value) / (dof2_value - dof1_value);
                            pt_idx++;
                        }
                    }
                    if (pt_idx == 2)
                    {
                        //plotPath->AddLine(x[0], y[0], x[1], y[1]);
                        //plotPath->StartFigure();
                        ///plotForm->lines->Add(PointF(x[0], y[0]));
                        ///plotForm->lines->Add(PointF(x[1], y[1]));
                        drawingContext.DrawLine(pen, new Point((x[0] - Tfmr.Core.LegRadius) * scale, (height - y[0])*scale), new Point((x[1] - Tfmr.Core.LegRadius) * scale, (height - y[1])*scale));
                    }
                    else
                    {
                        //error
                        int a = 1;
                    }
                }
            }

            //plotForm->setPhysicalSize(xmin, xmax, ymin, ymax);
            //plotForm->drawPlot();
        }
    }
}
