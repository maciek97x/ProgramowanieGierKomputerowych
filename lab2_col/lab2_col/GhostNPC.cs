using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab2
{
    class GhostNPC : Player
    {
        public GhostNPC(PointF _pos, PointF _velocity) : base(_pos, _velocity)
        {
            texture = new Bitmap("C:/Users/student/Documents/Visual Studio 2015/Projects/lab2/lab2/ghost.jpg");
            texture.MakeTransparent(texture.GetPixel(0, 0));
        }
        
        public void KeyDown(Keys KeyCode)
        {
        }
    }
}
