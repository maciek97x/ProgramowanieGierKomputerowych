using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace lab3
{
    class Ball
    {
        public PointF position = new PointF(0f, 0f);
        public float radius = 8;
        private PointF velocity = new PointF(0f, 0f);

        public Ball(PointF p, float r, PointF v)
        {
            position = p;
            radius = r;
            velocity = v;
        }

        public void Collision(Ball other)
        {
            PointF delta = position;
            delta.X -= other.position.X;
            delta.Y -= other.position.Y;

            // minimum translation distance to push balls apart after intersecting
            float d = (float)Math.Sqrt(delta.X * delta.X + delta.Y * delta.Y);

            PointF mtd = new PointF(delta.X, delta.Y);
            mtd.X *= (radius + other.radius - d) / d;
            mtd.Y *= (radius + other.radius - d) / d;

            // inverse mass quantities
            float im1 = 1 / (radius * radius);
            float im2 = 1 / (other.radius * other.radius);

            // push-pull them apart based off their mass
            position.X += mtd.X * (im1 / (im1 + im2));
            position.Y += mtd.Y * (im1 / (im1 + im2));
            other.position.X -= mtd.X * (im2 / (im1 + im2));
            other.position.Y -= mtd.Y * (im2 / (im1 + im2));

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
        public void Collision(Rectangle rect)
        {
            if (position.X - radius <= 0) velocity.X = Math.Abs(velocity.X);
            if (position.X + radius >= rect.Width) velocity.X = -Math.Abs(velocity.X);
            if (position.Y - radius <= 0) velocity.Y = Math.Abs(velocity.Y);
            if (position.Y + radius >= rect.Height) velocity.Y = -Math.Abs(velocity.Y);
        }
        public void Draw(Graphics g)
        {
            int v = (int)(Math.Sqrt(velocity.X * velocity.X + velocity.Y * velocity.Y) * 32);
            v = (int)Math.Min(v, 255);
            g.FillEllipse(new SolidBrush(Color.FromArgb(v, 0, 255 - v)), position.X - radius, position.Y - radius, 2 * radius, 2 * radius);
        }

        public void Move()
        {
            position.X += velocity.X;
            position.Y += velocity.Y;
        }
    }
}
