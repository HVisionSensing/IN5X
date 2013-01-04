#include "display.h"

display::display()
{
}

void display::drawPoints(Mat img, Point2i center, std::vector<Point2i> vect_extrem, std::vector<int> coords)
{
    Mat tmp;
    cvtColor(img,tmp,CV_GRAY2RGB);
    Point2i l = center;
    l.x += coords[0];
    l.y += coords[2];
    circle(tmp,l,5,Scalar_<u_char>(0,0,255));
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {
        Point2i pt = (*it);
        pt.x += coords[0];
        pt.y += coords[2];
        circle(tmp,pt,5,Scalar_<u_char>(255,0,0));
    }

    imshow("Image des points caractéristique",tmp);
}
