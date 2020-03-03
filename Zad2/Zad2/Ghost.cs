using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    class Ghost
    {
        protected Bitmap texture = new Bitmap("C:/Users/student/Documents/Visual Studio 2015/Projects/WindowsFormsApplication2/WindowsFormsApplication2/ghost.jpg");
        protected RectangleF rect = new RectangleF(0f, 0f, 48f, 48f);
        protected PointF dir = new PointF(1f, 1f);
        protected PointF velocity = new PointF(1f, 1f);
        protected Size mapSize;

        public Ghost(PointF _pos, Size _mapSize)
        {
            texture.MakeTransparent(texture.GetPixel(0, 0));
            rect.X = _pos.X;
            rect.Y = _pos.Y;
            mapSize = _mapSize;
        }

        public void Move()
        {
            if (rect.Left <= 0) dir.X = 1;
            if (rect.Right >= mapSize.Width) dir.X = -1;
            if (rect.Top <= 0) dir.Y = 1;
            if (rect.Bottom >= mapSize.Height) dir.Y = -1;

            rect.X += dir.X * velocity.X;
            rect.Y += dir.Y * velocity.Y;
        }

        public void Draw(Graphics g)
        {
            g.DrawImage(texture, rect);
        }

    }
}
