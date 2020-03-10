using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab2
{
    public partial class Form1 : Form
    {
        private int dungeonX = 0;
        private Bitmap dungeon;
        private List<GhostNPC> ghosts = new List<GhostNPC>();
        private Player player;
        private Bitmap buffer;
        public Form1()
        {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            Image img = Image.FromFile("C:/Users/student/Documents/Visual Studio 2015/Projects/lab2/lab2/dungeon.png");
            dungeon = new Bitmap(img, new Size((int)(img.Width * 1.5), (int)(img.Height * 1.5)));

            timer1.Enabled = true;
            buffer = new Bitmap(dungeon.Width, dungeon.Height);

            ClientSize = new Size(dungeon.Width, dungeon.Height);

            Random random = new Random();
            for (int i = 0; i < 5; ++i)
            {
                ghosts.Add(new GhostNPC(new PointF((float)random.NextDouble() * ClientSize.Width,
                    (float)random.NextDouble() * ClientSize.Height),
                    new PointF((float)(random.NextDouble() * 4 - 2),
                    (float)(random.NextDouble() * 4 - 2))));
            }
            player = new Player(new PointF((float)random.NextDouble() * ClientSize.Width,
                    (float)random.NextDouble() * ClientSize.Height),
                    new PointF((float)(random.NextDouble() * 4 - 2),
                    (float)(random.NextDouble() * 4 - 2)));
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            --dungeonX;
            dungeonX %= dungeon.Width;
            foreach (GhostNPC ghost in ghosts)
            {
                ghost.Move();
                ghost.Collision(ClientRectangle);
            }
            player.Move();
            player.Collision(ClientRectangle);


            for (int i = 0; i < ghosts.Count; ++i)
            {
                if (ghosts[i].rect.Width / 2 + player.rect.Width / 2 >
                    Math.Sqrt(Math.Pow(ghosts[i].rect.X - player.rect.X, 2) +
                              Math.Pow(ghosts[i].rect.Y - player.rect.Y, 2)))
                {
                    ghosts[i].Collision(player);
                }
                for (int j = i + 1; j < ghosts.Count; ++j)
                {
                    if (ghosts[i].rect.Width/2 + ghosts[j].rect.Width/2 >
                        Math.Sqrt(Math.Pow(ghosts[i].rect.X - ghosts[j].rect.X, 2) +
                                  Math.Pow(ghosts[i].rect.Y - ghosts[j].rect.Y, 2)))
                    {
                        ghosts[i].Collision(ghosts[j]);
                    }
                }
            }

            using (Graphics g = Graphics.FromImage(buffer))
            {
                g.DrawImage(dungeon, new Point(dungeonX, 0));
                g.DrawImage(dungeon, new Point(dungeonX + dungeon.Width, 0));
                foreach (GhostNPC ghost in ghosts)
                {
                    ghost.Draw(g);
                }
                player.Draw(g);
            }
            using (Graphics g = CreateGraphics())
            {
                g.DrawImage(buffer, new Point(0, 0));
            }
        }
        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            foreach (GhostNPC ghost in ghosts)
            {
                ghost.KeyDown(e.KeyCode);
            }
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            player.MouseMove(e.X, e.Y);
        }
    }
}
