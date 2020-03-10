using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab3
{

    public partial class Form1 : Form
    {
        private List<Ball> balls = new List<Ball>();
        private Bitmap buffer;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            buffer = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Random random = new Random();
            int n = 32;
            while (--n >= 0)
            {
                float r = (float)(random.NextDouble() * 28 + 4);
                balls.Add(new Ball(new PointF(r + (float)random.NextDouble() * (ClientSize.Width - 2 * r),
                        r + (float)random.NextDouble() * (ClientSize.Height - 2 * r)),
                        r,
                        new PointF((float)random.NextDouble() * 4 - 2f,
                        (float)random.NextDouble() * 4 - 2f)));
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            foreach (Ball b in balls)
            {
                b.Move();
                b.Collision(ClientRectangle);
            }
            for (int i = 0; i < balls.Count; ++i)
            {
                for (int j = i + 1; j < balls.Count; ++j)
                {
                    if (balls[i].radius + balls[j].radius >
                        Math.Sqrt(Math.Pow(balls[i].position.X - balls[j].position.X, 2) +
                                  Math.Pow(balls[i].position.Y - balls[j].position.Y, 2))) {
                        balls[i].Collision(balls[j]);
                    }
                }
            }
            using (Graphics g = Graphics.FromImage(buffer))
            {
                g.Clear(Color.White);
                foreach (Ball b in balls)
                {
                    b.Draw(g);
                }
            }
            using (Graphics g = CreateGraphics())
            {
                g.DrawImage(buffer, new Point(0, 0));
            }
        }

        private void Form1_ResizeEnd(object sender, EventArgs e)
        {
            buffer = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
        }
    }
}
