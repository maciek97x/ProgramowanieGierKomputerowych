using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab2
{
    class Ghost
    {
        protected Bitmap texture = new Bitmap("C:/Users/student/Documents/Visual Studio 2015/Projects/lab2/lab2/ghost.jpg");
        public RectangleF rect = new RectangleF(0f, 0f, 48f, 48f);
        public float radius = 24f;
        protected PointF dir = new PointF(1f, 1f);
        protected PointF velocity = new PointF(1f, 1f);
        protected Size mapSize;

        public Ghost(PointF _pos, PointF _velocity)
        {
            texture.MakeTransparent(texture.GetPixel(0, 0));
            rect.X = _pos.X;
            rect.Y = _pos.Y;
            velocity = _velocity;
        }

        public void Move()
        {
            rect.X += velocity.X;
            rect.Y += velocity.Y;
        }

        public void Draw(Graphics g)
        {
            g.DrawImage(texture, rect);
        }
        
        public void Collision(Rectangle other)
        {
            if (rect.Left <= 0) velocity.X = Math.Abs(velocity.X);
            if (rect.Right >= other.Width) velocity.X = -Math.Abs(velocity.X);
            if (rect.Top <= 0) velocity.Y = Math.Abs(velocity.Y);
            if (rect.Bottom >= other.Height) velocity.Y = -Math.Abs(velocity.Y);
        }

        public void Collision(Ghost other)
        {
            PointF delta = new PointF(rect.X, rect.Y);
            delta.X -= other.rect.X;
            delta.Y -= other.rect.Y;

            // minimum translation distance to push balls apart after intersecting
            float d = (float)Math.Sqrt(delta.X * delta.X + delta.Y * delta.Y);

            PointF mtd = new PointF(delta.X, delta.Y);
            mtd.X *= (radius + other.radius - d) / d;
            mtd.Y *= (radius + other.radius - d) / d;

            // inverse mass quantities
            float im1 = 1 / (radius * radius);
            float im2 = 1 / (other.radius * other.radius);

            // push-pull them apart based off their mass
            rect.X += mtd.X * (im1 / (im1 + im2));
            rect.Y += mtd.Y * (im1 / (im1 + im2));
            other.rect.X -= mtd.X * (im2 / (im1 + im2));
            other.rect.Y -= mtd.Y * (im2 / (im1 + im2));

            // impact speed
            PointF v = new PointF(velocity.X, velocity.Y);
            v.X -= other.velocity.X;
            v.Y -= other.velocity.Y;
            PointF mtd_normalized = new PointF();
            mtd_normalized.X = mtd.X / (float)Math.Sqrt(mtd.X * mtd.X + mtd.Y * mtd.Y);
            mtd_normalized.Y = mtd.Y / (float)Math.Sqrt(mtd.X * mtd.X + mtd.Y * mtd.Y);
            float vn = v.X * mtd_normalized.X + v.Y * mtd_normalized.Y;

            // sphere intersecting but moving away from each other already
            if (vn > 0.0f) return;

            // collision impulse
            float i = -2.0f * vn / (im1 + im2);

            PointF impulse = new PointF(mtd_normalized.X, mtd_normalized.Y);
            impulse.X *= i;
            impulse.Y *= i;

            // change in momentum
            velocity.X += impulse.X * im1;
            velocity.Y += impulse.Y * im1;
            other.velocity.X -= impulse.X * im2;
            other.velocity.Y -= impulse.Y * im2;
        }
    }
}
