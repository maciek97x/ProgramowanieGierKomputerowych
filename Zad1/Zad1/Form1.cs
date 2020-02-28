using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Zad1
{
    public partial class Form1 : Form
    {
        private RectangleF elipseRect = new RectangleF(0f, 0f, 30f, 30f);
        private PointF elipseDir = new PointF(1f, 1f);
        private PointF elipseVelocity = new PointF(1f, 1f);
        private Bitmap buffer;
        
        public Form1()
        {
            InitializeComponent();
        }

        private void drawStripes()
        {
            using (Graphics g = Graphics.FromImage(buffer))
            {
                for (int i = 0; i <= ClientRectangle.Height / 64; ++i)
                {
                    if (i % 2 == 0)
                    {
                        g.FillRectangle(Brushes.Aqua, 0, 64 * i, ClientRectangle.Width, 32);
                    }
                    else
                    {
                        g.FillRectangle(Brushes.Aquamarine, 0, 64 * i, ClientRectangle.Width, 32);
                    }
                }
            }
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
            elipseRect.X = (ClientRectangle.Width - elipseRect.Width) / 2;
            elipseRect.Y = (ClientRectangle.Height - elipseRect.Height) / 2;
            buffer = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            timer1.Enabled = true;
            drawStripes();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (elipseRect.Left <= 0) elipseDir.X = 1;
            if (elipseRect.Right >= ClientRectangle.Width) elipseDir.X = -1;
            if (elipseRect.Top <= 0) elipseDir.Y = 1;
            if (elipseRect.Bottom >= ClientRectangle.Height) elipseDir.Y = -1;

            RectangleF prevRect = elipseRect;

            elipseRect.X += elipseDir.X * elipseVelocity.X;
            elipseRect.Y += elipseDir.Y * elipseVelocity.Y;

            using (Graphics g = Graphics.FromImage(buffer))
            {
                g.FillEllipse(SystemBrushes.ButtonFace, prevRect);
                for (int i = (int)(elipseRect.Top / 128); i <= (elipseRect.Bottom + 32) / 128 ; ++i)
                {
                    g.FillRectangle(Brushes.Aqua, 0, 128 * i, ClientRectangle.Width, 32);
                }
                g.FillEllipse(Brushes.Blue, elipseRect);
                for (int i = (int)(elipseRect.Top / 128); i <= (elipseRect.Bottom + 32) / 128; ++i)
                {
                    g.FillRectangle(Brushes.Aquamarine, 0, 128 * i + 64, ClientRectangle.Width, 32);
                }
            }

            using (Graphics g = CreateGraphics())
            {
                g.DrawImage(buffer, new Point(0, 0));
            }
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Right:
                    elipseVelocity.X *= 1.2f;
                    break;
                case Keys.Left:
                    elipseVelocity.X /= 1.2f;
                    break;
                case Keys.Up:
                    elipseVelocity.Y *= 1.2f;
                    break;
                case Keys.Down:
                    elipseVelocity.Y /= 1.2f;
                    break;
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
        }

        private void Form1_ResizeEnd(object sender, EventArgs e)
        {
            buffer = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            drawStripes();
            if (elipseRect.Left <= 0 || elipseRect.Right >= ClientRectangle.Width ||
                elipseRect.Top <= 0 || elipseRect.Bottom >= ClientRectangle.Height)
            {
                elipseRect.X = (ClientRectangle.Width - elipseRect.Width) / 2;
                elipseRect.Y = (ClientRectangle.Height - elipseRect.Height) / 2;
            }
        }
    }
}
