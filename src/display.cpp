#include "display.h"

display::display()
{
}

void display::drawPoints(Mat img, Point2i center, std::vector<Point2i> vect_extrem)
{
    Mat tmp;
    cvtColor(img,tmp,CV_GRAY2RGB);
    circle(tmp,center,5,Scalar_<u_char>(0,0,255));
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {
        circle(tmp,(*it),5,Scalar_<u_char>(255,0,0));
    }

    imshow("Image des points caractéristique",tmp);
}
