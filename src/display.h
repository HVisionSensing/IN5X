#ifndef DISPLAY_H
#define DISPLAY_H

#include "opencv2/opencv.hpp"
#include "recognition.h"

using namespace cv;

class display
{
public:
    display();

    static void drawPoints(Mat img, Point2i center, std::vector<Point2i> vect_extrem,
                           std::vector<Point2i> vect_multi, std::vector<int> coords);
    static void drawConvexhull(Mat img,std::vector<std::vector<Point2i> > hullDefects);
    static void drawAllRecognition(Mat img,Point2i center,std::vector<Point2i>vect_extrem,std::vector<Point2i>vect_multi,std::vector<std::vector<Point2i> > hullDefects,Recognition::STATE state);
};

#endif // DISPLAY_H
