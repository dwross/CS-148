#include "raster2d.h"

void Raster2d::draw(){
	if(!m_lines.empty()){
		for(unsigned int i = 0; i < m_lines.size(); i++){
			Line2f l;
			l.a.x = m_lines[i].a.x;
			l.a.y = m_lines[i].a.y;
			l.b.x = m_lines[i].b.x;
			l.b.y = m_lines[i].b.y;
			l.s.x = m_lines[i].s.x;
			l.s.y = m_lines[i].s.y;
			l.m.x = m_lines[i].m.x;
			l.m.y = m_lines[i].m.y;
			l.r.x = m_lines[i].r.x;
			l.r.y = m_lines[i].r.y;
			draw_line(l);
		}
	}else{
		for(unsigned int i = 0; i < m_triangles.size(); i++){
			Triangle2f tri;
			tri.a.x = m_triangles[i].a.x;
			tri.a.y = m_triangles[i].a.y;
			tri.b.x = m_triangles[i].b.x;
			tri.b.y = m_triangles[i].b.y;
			tri.c.x = m_triangles[i].c.x;
			tri.c.y = m_triangles[i].c.y;
			tri.a_color.r = m_triangles[i].a_color.r;
			tri.a_color.g = m_triangles[i].a_color.g;
			tri.a_color.b = m_triangles[i].a_color.b;
			tri.b_color.r = m_triangles[i].b_color.r;
			tri.b_color.g = m_triangles[i].b_color.g;
			tri.b_color.b = m_triangles[i].b_color.b;
			tri.c_color.r = m_triangles[i].c_color.r;
			tri.c_color.g = m_triangles[i].c_color.g;
			tri.c_color.b = m_triangles[i].c_color.b;
			tri.s.x = m_triangles[i].s.x;
			tri.s.y = m_triangles[i].s.y;
			tri.m.x = m_triangles[i].m.x;
			tri.m.y = m_triangles[i].m.y;
			tri.r.x = m_triangles[i].r.x;
			tri.r.y = m_triangles[i].r.y;
			draw_triangle(tri);
		}
	}
}
Point2f Raster2d::scale(Point2f p, Point2f s){
	//scale matrix represented in homogenous coordinates
	float m[3][3]={{s.x,0.0f,0.0f},{0.0f,s.y,0.0f},{0.0f,0.0f,1.0f}};
	//vector of point represented in homogenous coordinates
	float v[3]={p.x,p.y,1.0f};
	float re[3]={0.0f,0.0f,0.0f};
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			re[i] += m[i][j]*v[j];
		}
	}
	Point2f result;
	result.x = re[0];
	result.y = re[1];
	return result;
}
Point2f Raster2d::translate(Point2f p, Point2f t){
	//translate matrix represented in homogenous coordinates
	float m[3][3]={{1.0f,0.0f,t.x},{0.0f,1.0f,t.y},{0.0f,0.0f,1.0f}};
	//vector of point represented in homogenous coordinates
	float v[3]={p.x,p.y,1.0f};
	float re[3]={0.0f,0.0f,0.0f};
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			re[i] += m[i][j]*v[j];
		}
	}
	Point2f result;
	result.x = re[0];
	result.y = re[1];
	return result;
}
Point2f Raster2d::rotate(Point2f p, Point2f r){
	//rotation matrix represented in homogenous coordinates
	float m[3][3]={{cos(r.x),-sin(r.x),0.0f},{sin(r.x),cos(r.x),0.0f},{0.0f,0.0f,1.0f}};
	//vector of point represented in homogenous coordinates
	float v[3]={p.x,p.y,1.0f};
	float re[3]={0.0f,0.0f,0.0f};
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			re[i] += m[i][j]*v[j];
		}
	}
	Point2f result;
	result.x = re[0];
	result.y = re[1];
	return result;
}
// rasterize line
void Raster2d::draw_line(Line2f l){
	float x0 = l.a.x;
	float y0 = l.a.y;
	float x1 = l.b.x;
	float y1 = l.b.y;
	int transform_order = (int)l.r.y;
	int transform_type = 0;
	for(int i = 0; i < 3; i++){
		transform_type = transform_order % 10;
		transform_order /= 10;
		//scale
		if(transform_type == 1){
			if(l.s.x||l.s.y){
				Point2f point1(x0,y0);
				Point2f scal(l.s.x,l.s.y);
				Point2f result = scale(point1, scal);
				x0 = result.x;
				y0 = result.y;
				Point2f point2(x1,y1);
				result = scale(point2, scal);
				x1 = result.x;
				y1 = result.y;
			}
		}
		//translate
		if(transform_type == 2){
			if(l.m.x||l.m.y){
				Point2f point1(x0,y0);
				Point2f move(l.m.x,l.m.y);
				Point2f result = translate(point1, move);
				x0 = result.x;
				y0 = result.y;
				Point2f point2(x1,y1);
				result = translate(point2, move);
				x1 = result.x;
				y1 = result.y;
			}
		}
		//rotate
		if(transform_type == 3){
			if(l.r.x){
				Point2f point1(x0,y0);
				Point2f rot(l.r.x,l.r.y);
				Point2f result = rotate(point1, rot);
				x0 = result.x;
				y0 = result.y;
				Point2f point2(x1,y1);
				result = rotate(point2, rot);
				x1 = result.x;
				y1 = result.y;
			}
		}
	}
	//if any end of the line is outside the drawing area don't draw it
	if(!(((x0 < m_x_min) || (x0 > m_x_max)) ||
		 ((x1 < m_x_min) || (x1 > m_x_max)) ||
		 ((y0 < m_y_min) || (y0 > m_y_max)) ||
		 ((y1 < m_y_min) || (y1 > m_y_max)))){
		int x_pxl = 0;
		int y_pxl = 0;
		//if the line is vertical or horizontal
		//the algorithm is not need so just draw it
		if(x0 == x1){
			x_pxl = pixel_x(x0);
			if(y1 < y0) swap(y0,y1);
			for(y_pxl = pixel_y(y0); y_pxl < pixel_y(y1); y_pxl++){
				if(x_pxl < 0) x_pxl = 0;
				if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
				if(y_pxl < 0) y_pxl = 0;
				if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
				m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
			}
		}else if(y0 == y1){
			y_pxl = pixel_y(y0);
			if(x1 < x0) swap(x0,x1);
			for(x_pxl = pixel_x(x0); x_pxl < pixel_x(x1); x_pxl++){
				if(x_pxl < 0) x_pxl = 0;
				if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
				if(y_pxl < 0) y_pxl = 0;
				if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
				m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
			}
		//if the slope is 1 or -1
		//the algorithm is not need so just draw it
		}else if((((y0 - y1)/(x0 - x1)) == 1) || (((y0 - y1)/(x0 - x1)) == -1)){
			if(x1 < x0){swap(x0,x1); swap(y0,y1);}
			if(y0 < y1){
				y_pxl = pixel_y(y0);
				for(x_pxl = pixel_x(x0); x_pxl < pixel_x(x1); x_pxl++){
					if(x_pxl < 0) x_pxl = 0;
					if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
					if(y_pxl < 0) y_pxl = 0;
					if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
					m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
					y_pxl++;
				}
			}else{
				y_pxl = pixel_y(y1);
				for(x_pxl = pixel_x(x1); x_pxl > pixel_x(x0); x_pxl--){
					if(x_pxl < 0) x_pxl = 0;
					if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
					if(y_pxl < 0) y_pxl = 0;
					if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
					m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
					y_pxl++;
				}
			}
		//everything else needs the algorithm;
		}else{
			float x_len = abs(x0 - x1);
			float y_len = abs(y0 - y1);
			float error = 0;
			float d_error = 0;
			//if the longest side of the line is along x
			if(x_len > y_len){
				d_error = abs(y_len/x_len);
				if(x1 < x0){swap(x0,x1); swap(y0,y1);}
				//if the slope along x is positive
				if(y0 < y1){
					y_pxl = pixel_y(y0);
					for(x_pxl = pixel_x(x0); x_pxl < pixel_x(x1); x_pxl++){
						if(x_pxl < 0) x_pxl = 0;
						if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
						if(y_pxl < 0) y_pxl = 0;
						if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
						m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
						error += d_error;
						if(error >= 0.5){
							y_pxl++;
							error -= 1.0;
						}
					}
				//if the slope along x is negative
				}else{
					y_pxl = pixel_y(y1);
					for(x_pxl = pixel_x(x1); x_pxl > pixel_x(x0); x_pxl--){
						if(x_pxl < 0) x_pxl = 0;
						if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
						if(y_pxl < 0) y_pxl = 0;
						if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
						m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
						error += d_error;
						if(error >= 0.5){
							y_pxl++;
							error -= 1.0;
						}
					}
				}
			//if the longest side of the line is along y
			}else{
				d_error = abs(x_len/y_len);
				if(y1 < y0){swap(x0,x1); swap(y0,y1);}
				//if the slope along y is positive
				if(x0 < x1){
					x_pxl = pixel_x(x0);
					for(y_pxl = pixel_y(y0); y_pxl < pixel_y(y1); y_pxl++){
						if(x_pxl < 0) x_pxl = 0;
						if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
						if(y_pxl < 0) y_pxl = 0;
						if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
						m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
						error += d_error;
						if(error >= 0.5){
							x_pxl++;
							error -= 1.0;
						}
					}
				//if the slope along y is negative
				}else{
					x_pxl = pixel_x(x1);
					for(y_pxl = pixel_y(y1); y_pxl > pixel_y(y0); y_pxl--){
						if(x_pxl < 0) x_pxl = 0;
						if(x_pxl >= m_x_res) x_pxl = m_x_res - 1;
						if(y_pxl < 0) y_pxl = 0;
						if(y_pxl >= m_y_res) y_pxl = m_y_res - 1;
						m_image->setPixel(x_pxl,y_pxl,RGBColor(1,0,0));
						error += d_error;
						if(error >= 0.5){
							x_pxl++;
							error -= 1.0;
						}
					}
				}
			}
		}
	}
}

// rasterize triangle
void Raster2d::draw_triangle(Triangle2f tri){
	float ax = tri.a.x;
	float ay = tri.a.y;
	float bx = tri.b.x;
	float by = tri.b.y;
	float cx = tri.c.x;
	float cy = tri.c.y;
	int transform_order = (int)tri.r.y;
	int transform_type = 0;
	for(int i = 0; i < 3; i++){
		transform_type = transform_order % 10;
		transform_order /= 10;
		//scale
		if(transform_type == 1){
				if(tri.s.x||tri.s.y){
					Point2f point1(ax,ay);
					Point2f scal(tri.s.x,tri.s.y);
					Point2f result = scale(point1, scal);
					ax = result.x;
					ay = result.y;
					Point2f point2(bx,by);
					result = scale(point2, scal);
					bx = result.x;
					by = result.y;
					Point2f point3(cx,cy);
					result = scale(point3, scal);
					cx = result.x;
					cy = result.y;
				}
		}
		//translate
		if(transform_type == 2){
				if(tri.m.x||tri.m.y){
					Point2f point1(ax,ay);
					Point2f move(tri.m.x,tri.m.y);
					Point2f result = translate(point1, move);
					ax = result.x;
					ay = result.y;
					Point2f point2(bx,by);
					result = translate(point2, move);
					bx = result.x;
					by = result.y;
					Point2f point3(cx,cy);
					result = translate(point3, move);
					cx = result.x;
					cy = result.y;
				}
		}
		//rotate
		if(transform_type == 3){
				if(tri.r.x){
					Point2f point1(ax,ay);
					Point2f rot(tri.r.x,tri.r.y);
					Point2f result = rotate(point1, rot);
					ax = result.x;
					ay = result.y;
					Point2f point2(bx,by);
					result = rotate(point2, rot);
					bx = result.x;
					by = result.y;
					Point2f point3(cx,cy);
					result = rotate(point3, rot);
					cx = result.x;
					cy = result.y;
				}
		}
	}
	double xa = (double)pixel_x(ax);
	double ya = (double)pixel_y(ay);
	double xb = (double)pixel_x(bx);
	double yb = (double)pixel_y(by);
	double xc = (double)pixel_x(cx);
	double yc = (double)pixel_y(cy);
	double gad = ((ya-yb)*xc)+((xb-xa)*yc)+(xa*yb)-(xb*ya);
	double bed = ((ya-yc)*xb)+((xc-xa)*yb)+(xa*yc)-(xc*ya);
	for(unsigned x = 0.0; x < m_x_res; x++){
		for(unsigned y = 0.0; y < m_y_res; y++){
			double ga = (((ya-yb)*x)+((xb-xa)*y)+(xa*yb)-(xb*ya))/gad;
			double be = (((ya-yc)*x)+((xc-xa)*y)+(xa*yc)-(xc*ya))/bed;
			double al = 1.0-be-ga;
			double ep = 0.001;
	//I had to choose between just going inside the triangle or 
	//taking the inside points along with the edges
	//I chose the later because it most of the figures look like
	//the images in the assignment.  The real solution is implement
	//the shadowing that will only draw the edge on certian sides.
			if(((((al-0.0)>ep)||(fabs(al-0.0)<ep))&&(((al-1.0)<ep)||(fabs(al-1.0)<ep)))&&
			   ((((be-0.0)>ep)||(fabs(be-0.0)<ep))&&(((be-1.0)<ep)||(fabs(be-1.0)<ep)))&&
			   ((((ga-0.0)>ep)||(fabs(ga-0.0)<ep))&&(((ga-1.0)<ep)||(fabs(ga-1.0)<ep)))){
				float car = tri.a_color.r * al;
				float cag = tri.a_color.g * al;
				float cab = tri.a_color.b * al;
				float cbr = tri.b_color.r * be;
				float cbg = tri.b_color.g * be;
				float cbb = tri.b_color.b * be;
				float ccr = tri.c_color.r * ga;
				float ccg = tri.c_color.g * ga;
				float ccb = tri.c_color.b * ga;
				m_image->setPixel(x,y,RGBColor((car+cbr+ccr),(cag+cbg+ccg),(cab+cbb+ccb)));
			}
		}
	}
}



