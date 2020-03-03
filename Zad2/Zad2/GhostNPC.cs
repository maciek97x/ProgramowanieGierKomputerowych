using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    class GhostNPC : Player
    {
        public GhostNPC(PointF _pos, Size _mapSize) : base(_pos, _mapSize)
        {
            texture = new Bitmap("C:/Users/student/Documents/Visual Studio 2015/Projects/WindowsFormsApplication2/WindowsFormsApplication2/ghost.jpg");
            texture.MakeTransparent(texture.GetPixel(0, 0));
        }
        
        public void KeyDown(Keys KeyCode)
        {
            switch (KeyCode)
            {
                case Keys.Right:
                    velocity.X *= 1.2f;
                    break;
                case Keys.Left:
                    velocity.X /= 1.2f;
                    break;
                case Keys.Up:
                    velocity.Y *= 1.2f;
                    break;
                case Keys.Down:
                    velocity.Y /= 1.2f;
                    break;
            }
        }
    }
}
