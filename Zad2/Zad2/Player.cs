using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    class Player : Ghost
    {
        public Player(PointF _pos, Size _mapSize) : base(_pos, _mapSize)
        {
            texture = new Bitmap("C:/Users/student/Documents/Visual Studio 2015/Projects/WindowsFormsApplication2/WindowsFormsApplication2/ghost2.png");
            texture.MakeTransparent(texture.GetPixel(0, 0));
        }

        public void MouseMove(int X, int Y)
        {
            float dirX = X - rect.X;
            float dirY = Y - rect.Y;
            dir.X = Math.Sign(dirX);
            dir.Y = Math.Sign(dirY);
            velocity.X = Math.Abs(dirX/100);
            velocity.Y = Math.Abs(dirY/100);
        }
    }
}
