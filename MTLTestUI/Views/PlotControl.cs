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
using Avalonia.Input;
using Avalonia.Visuals.Platform;
using System.Reflection;
using Avalonia.Metadata;

namespace MTLTestUI.Views
{
    public class PlotControl : Control
    {
        static PlotControl()
        {
            AffectsRender<PlotControl>(GeometryProperty);
        }

        public PlotControl()
        {
            //WeakReferenceMessenger.Default.Register<MainViewModel.RefreshPlotMessage>(this, (r, m) =>
            //{
            //    this.InvalidateVisual();
            //});
            this.
            PointerWheelChanged += OnWheel;
            PointerPressed += OnPointerPressed;
            PointerMoved += OnPointerMoved;
            PointerReleased += OnPointerReleased;
        }

        private (double x, double y) worldBounds;
        private double scale = 1.0;
        private bool isInDrag = false;
        private Point mouseStart;
        private Point panWorldOffset = new Point(0, 0);
        private double panSpeed = 1;
        private Point mouseWorldCurrPos;

        protected void OnWheel(object sender, PointerWheelEventArgs e) {
            var currPos = e.GetPosition(this);
            var mouseWorldBeforeZoom = ScreenToWorld(currPos);
            scale = scale * (1 + 0.1 * e.Delta.Y / Math.Abs(e.Delta.Y));
            var mouseWorldAfterZoom = ScreenToWorld(currPos);
            panWorldOffset = new Point(panWorldOffset.X - (mouseWorldBeforeZoom - mouseWorldAfterZoom).X, panWorldOffset.Y + (mouseWorldBeforeZoom - mouseWorldAfterZoom).Y);
            this.InvalidateVisual();
        }

        private void OnPointerPressed(object? sender, PointerPressedEventArgs e)
        {
            if (!isInDrag)
            {
                mouseStart = e.GetPosition(this);
                isInDrag = true;
            }
        }

        protected void OnPointerMoved(object sender, PointerEventArgs e)
        {
            if(isInDrag)
            {
                var currPos = e.GetPosition(this);
                var delta = (currPos - mouseStart) / scale;
                panWorldOffset += delta * panSpeed;
                System.Diagnostics.Debug.WriteLine($"Drag...pan offset: {panWorldOffset}");
                mouseStart = currPos;
                this.InvalidateVisual();
            }
            else
            {
                mouseWorldCurrPos = ScreenToWorld(e.GetPosition(this));
            }
        }

        private void OnPointerReleased(object? sender, PointerReleasedEventArgs e)
        {
            if (isInDrag)
            {
                isInDrag = false;
            }
        }

        public static readonly StyledProperty<TDAP.Geometry> GeometryProperty =
            AvaloniaProperty.Register<PlotControl, TDAP.Geometry>(nameof(Geometry));

        public TDAP.Geometry Geometry
        {
            get => GetValue(GeometryProperty);
            set
            {
                SetValue(GeometryProperty, value);
                SetInitialScale();
            }
        }

        public void SetInitialScale()
        {
            worldBounds = Geometry.GetBounds();

            if ((Bounds.Height / worldBounds.y) > (Bounds.Width / worldBounds.x))
            {
                scale = Bounds.Width / worldBounds.x;
                //scale = Bounds.Height / height;
            }
            else
            {
                scale = Bounds.Height / worldBounds.y;
                //scale = Bounds.Width / width;
            }
        }

        public override void Render(DrawingContext drawingContext)
        {
            if (scale == 1.0) SetInitialScale();
            if (double.IsNaN(Bounds.Height) || double.IsNaN(Bounds.Width)) return;

            var pen = new Pen(Brushes.Green, 1, lineCap: PenLineCap.Square);

            var renderSize = Bounds.Size;
            drawingContext.FillRectangle(Brushes.Black, new Rect(renderSize));

            if (Geometry != null)
            {
                foreach (var line in Geometry.Lines)
                {
                    drawingContext.DrawLine(pen, WorldToScreen(new Point(line.pt1.x, line.pt1.y)), WorldToScreen(new Point(line.pt2.x, line.pt2.y)));
                }
                foreach (var arc in Geometry.Arcs)
                {
                    var sg = new StreamGeometry();
                    using (var sgc = sg.Open())
                    {
                        sgc.BeginFigure(WorldToScreen(new Point(arc.StartPt.x, arc.StartPt.y)), false);
                        sgc.ArcTo(WorldToScreen(new Point(arc.EndPt.x, arc.EndPt.y)), new Size(arc.Radius * scale, arc.Radius * scale), arc.SweepAngle, false, SweepDirection.Clockwise);
                        sgc.EndFigure(false);
                        drawingContext.DrawGeometry(Brushes.Red, pen, sg);
                    }

                }
                //drawingContext.DrawEllipse(Brushes.Blue, pen, WorldToScreen(mouseWorldCurrPos), 4, 4);
            }
        }

        // World coordinates have origin at lower left and positive y is up and positive x is right
        // Screen coordinates have origin at top left, with positive y down and positive x to the right
        private Point WorldToScreen(Point worldPt)
        {
            return new Point((worldPt.X + panWorldOffset.X) * scale, (worldBounds.y - worldPt.Y + panWorldOffset.Y) * scale);
        }

        private Point ScreenToWorld(Point screenPt)
        {
            return new Point(screenPt.X / scale - panWorldOffset.X, worldBounds.y + panWorldOffset.Y - screenPt.Y / scale);
        }

    }
}
