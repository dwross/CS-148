#include <fstream>
#include <iostream>

// local
#include "raster2d.h"

//io.cpp -----  file input/output
bool Raster2d::load_file(string filename){

    // peek at the extension
    std::string::size_type idx;
    idx = filename.rfind('.');

    if(idx != std::string::npos)
    {
        std::string extension = filename.substr(idx + 1);

        if (extension == "pline")
            return load_polylines(filename);

        else if (extension == "tri")
            return load_triangles(filename);

        else {
            std::cerr << "ERROR: unable to load file " << filename << "  -- unknown extension." << std::endl;
            std::cerr << "Input only polylines (.pline) or triangles. (.tri)" << std::endl;
        }
    }

    // No filename extension found, or none matching {.pline, .tri}
    return false;
}

bool check_index(int index, unsigned size){
    if(index < 0 || index >= size){
        std::cerr<<"ERROR: unable to load triangles file; index out of range (" << index << ")" << std::endl;
        return false;
    }
    return true;
}

bool Raster2d::load_triangles(string filename){
    m_triangles.clear();
    std::ifstream ifs(filename.c_str());

    if(ifs.is_open()){
        // file begins with the x,y dimensions
        float x_min, x_max, y_min, y_max;
        ifs >> x_min; ifs >> x_max;
        ifs >> y_min; ifs >> y_max;

        set_box(x_min, x_max, y_min, y_max);

        char type;
        vector<Point2f> points;
        vector<RGBColor> colors;
		vector<Point2f> trans;
		vector<Point2f> sca;
		vector<Point2f> rotat;

		int i = 0;
        int index, t0, t1, t2;
        float x0, x1;
        float c0, c1, c2;

        while (ifs >> type) {
            if ( type == 'p'){
                ifs >> x0 ; ifs >> x1;
                points.push_back(Point2f(x0, x1));
                colors.push_back(RGBColor(1., 1., 1.)); // default color is white.
            }
            else if ( type == 'c'){
                ifs >> index;
                ifs >> c0; ifs >> c1 ; ifs >> c2;
                if(check_index(index, points.size()))
                {
                    // set the corresponding color
                    colors[index] = RGBColor(c0, c1, c2);
                }
            }
			else if ( type == 's'){
				ifs >> x0 ; ifs >> x1;
				sca.push_back(Point2f(x0, x1));
			}
			else if ( type == 'm'){
				ifs >> x0 ; ifs >> x1;
				trans.push_back(Point2f(x0, x1));
			}
			else if ( type == 'r'){
				ifs >> x0 ; ifs >> x1;
				rotat.push_back(Point2f(x0, x1));
			}
            else if ( type == 't'){
                ifs >> t0; ifs >> t1; ifs >> t2;
                if(check_index(t0, points.size()) && check_index(t1, points.size()) && check_index(t2, points.size())){
                    Triangle2f t = Triangle2f(points[t0], points[t1], points[t2], sca[i], trans[i], rotat[i]);
                    t.color_verts(colors[t0], colors[t1], colors[t2]);
                    m_triangles.push_back(t);
					i++;
                }
            }
            else {
                std::cerr<<"error parsing " << filename << std::endl;
                std::cerr<<"offending line: " << type << std::endl;
                return false;
            }
        }
    }
    return true;

}

bool Raster2d::load_polylines(string filename){
    m_lines.clear();
    std::ifstream ifs(filename.c_str());

    if(ifs.is_open()){
        // file begins with the x,y dimensions
        float x_min, x_max, y_min, y_max;
        ifs >> x_min; ifs >> x_max;
        ifs >> y_min; ifs >> y_max;

        set_box(x_min, x_max, y_min, y_max);

        string line;
        float x, y, a, b, mx, my, sx, sy, rx, ry;
        while (ifs >> line) {
            if (line == "polyline"){
                ifs >> x;
                ifs >> y;
                Point2f start(x,y);
                bool new_pline = false;
                while (!new_pline && ifs >> a && ifs >> b){
                    Point2f end(a,b);
					ifs >> sx;
					ifs >> sy;
					Point2f scale(sx,sy);
					ifs >> mx;
					ifs >> my;
					Point2f move(mx,my);
					ifs >> rx;
					ifs >> ry;
					Point2f rotate(rx,ry);
                    m_lines.push_back(Line2f(start, end, scale, move, rotate));
                    start = end;
                    if((ifs >> std::ws).peek() == 'p') new_pline = true;
                }

            }
            else {
                std::cerr<<"error parsing " << filename << std::endl;
                std::cerr<<"offending line: " << line << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool Raster2d::save_out(string filename){
    std::cout << "Saving output to "<< filename << "." << std::endl;
    return m_image->WritePNG(filename);
}
