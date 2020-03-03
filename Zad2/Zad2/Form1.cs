using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication2
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
            dungeon = new Bitmap("C:/Users/student/Documents/Visual Studio 2015/Projects/WindowsFormsApplication2/WindowsFormsApplication2/dungeon.png");
            
            timer1.Enabled = true;
            buffer = new Bitmap(dungeon.Width, dungeon.Height);

            ClientSize = new Size(dungeon.Width, dungeon.Height);

            Random random = new Random();
            for (int i = 0; i < 5; ++i)
            {
                ghosts.Add(new GhostNPC(new PointF((float)random.NextDouble()*ClientSize.Width,
                    (float)random.NextDouble()*ClientSize.Height),
                    ClientSize));
            }
            player = new Player(new PointF((float)random.NextDouble() * ClientSize.Width,
                    (float)random.NextDouble() * ClientSize.Height),
                    ClientSize);

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            --dungeonX;
            dungeonX %= dungeon.Width;
            foreach (GhostNPC ghost in ghosts)
            {
                ghost.Move();
            }
            player.Move();
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
