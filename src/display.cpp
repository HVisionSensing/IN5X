#include "display.h"

display::display()
{
}

void display::drawPoints(Mat img, Point2i center, std::vector<Point2i> vect_extrem,
                         std::vector<Point2i> vect_multi, std::vector<int> coords)
{
    Mat tmp;
    cvtColor(img,tmp,CV_GRAY2RGB);
    Point2i l = center;
    l.x += coords[0];
    l.y += coords[2];
    circle(tmp,l,1,Scalar_<u_char>(0,0,255));
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {
        Point2i pt = (*it);
        pt.x += coords[0];
        pt.y += coords[2];
        circle(tmp,pt,1,Scalar_<u_char>(255,0,0));
        line(tmp,pt,l,Scalar_<u_char>(255,255,0));
    }
    std::vector<Point2i>::iterator it1 = vect_multi.begin();
    for(;it1 != vect_multi.end();it1++)
    {
        Point2i pt = (*it1);
        pt.x += coords[0];
        pt.y += coords[2];
        circle(tmp,pt,1,Scalar_<u_char>(0,255,0));
    }

    imshow("Image des points caractéristique",tmp);
}
