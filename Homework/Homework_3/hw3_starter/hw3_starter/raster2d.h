#ifndef RASTER2D_H
#define RASTER2D_H

#include <vector>
#include <string>
#include <iostream>

// Local:
#include "util.h"
#include "SimpleImage.h"

// from SimpleImage.h
class RGBColor;

class Raster2d {

private:
    std::vector<Line2f> m_lines;
    std::vector<Triangle2f> m_triangles;
    SimpleImage *m_image;
    float m_x_min, m_x_max, m_y_min, m_y_max;
    unsigned m_x_res, m_y_res;

public:
    Raster2d(int x_res, int y_res, RGBColor bg = RGBColor(0.5, 0.5, 0.5),
             float x_min = 0., float x_max = 1.,
             float y_min = 0., float y_max = 1.) :
        m_x_res(x_res), m_y_res(y_res),
        m_x_min(x_min), m_y_min(y_min),
        m_x_max(x_max), m_y_max(y_max)
    {
        m_image = new SimpleImage(m_x_res, m_y_res, bg);
    }

    ~Raster2d(){
        delete m_image;
    }

    bool load_file(string filename);
    bool save_out(string filename);
    void draw();

private:
    // io =====================================================
    bool load_polylines(string filename); 
    bool load_triangles(string filename);

    // drawing =====================================================
    void draw_line(Line2f l);
    void draw_triangle(Triangle2f t);

	// transforms ==================================================
	Point2f translate(Point2f p, Point2f t);
	Point2f scale(Point2f p, Point2f s);
	Point2f rotate(Point2f p, Point2f r);

    // a few helpers =====================================================
    void set_box(float x_min, float x_max, float y_min, float y_max) {
        m_x_min = x_min;
        m_x_max = x_max;
        m_y_min = y_min;
        m_y_max = y_max;
    }

    int clamp(int value, int min, int max){ return std::max(std::min(value, max), min); }

    // Warning: this can be out of bounds!
    int pixel_x(float x){
        float temp = ( x - m_x_min) * (float) m_x_res / (m_x_max - m_x_min);
        return (int)temp;
    }

    // Warning: this can be out of bounds!
    int pixel_y(float y){
        float temp = ( y - m_y_min) * (float) m_y_res / (m_y_max - m_y_min);
        return (int)temp;
    }

    bool on_screen(int x, int y){
        return (x >= 0 && x < m_x_res && y >= 0 && y < m_y_res);
    }
};

#endif

