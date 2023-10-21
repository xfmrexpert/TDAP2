using System.Collections.Generic;
using System.IO;

namespace TDAP
{
    public class GmshFile
    {
        public int ElementOrder {get; set;} = 1;
        public string Filename {get; set;}
        public List<GmshPoint> points = new List<GmshPoint>();
        public List<GmshLine> lines = new List<GmshLine>();
        public List<GmshLineLoop> line_loops = new List<GmshLineLoop>();

        public GmshFile(string filename)
        {
            Filename = filename;
        }

        public void gmshRectangle(double ll_x, double ll_y, double ur_x, double ur_y)
        {

        }

        public void writeFile()
        {

        }

        
    }

    public class GmshPoint
    {
        public double x, y, z;
        public int ID;

        public GmshPoint(double in_x, double in_y, double in_z)
        {
            x=in_x;
            y=in_y;
            z=in_z;
            ID=-9999;
        }

        public void Write(StreamWriter sw){
            sw.WriteLine("Point ({0}) = {{{1}, {2}, {3}, lc}};", ID, x, y, z);
        }

    }

    public class GmshLine
    {
        public GmshPoint pt1, pt2;
        public int ID;

        public GmshLine(GmshPoint in_pt1, GmshPoint in_pt2){
            pt1 = in_pt1;
            pt2 = in_pt2;
            ID = -9999;
        }

        public void Write(StreamWriter sw){
            sw.WriteLine("Line ({0}) = {{{1}, {2}}};", ID, pt1.ID, pt2.ID);
        }

    }

    public class GmshLineLoop
    {
        public List<GmshLine> lines;
        public int ID;

        public GmshLineLoop(List<GmshLine> in_lines){
            lines = in_lines;
        }

        public void Write(StreamWriter sw){
            sw.Write("Line Loop ({0}) = {{", ID);
            bool firstLine = true;
            foreach (var line in lines) {
                if(!firstLine){
                    sw.Write(", ");
                }else{
                    firstLine = false;
                }
                sw.Write("{0}", line.ID);
            }
            sw.WriteLine("};");
        }
        
    }

}