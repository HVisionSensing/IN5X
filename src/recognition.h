#ifndef RECOGNITION_H
#define RECOGNITION_H

#include "opencv2/opencv.hpp"

using namespace cv;

class Recognition
{

public:
    enum STATE {HAND_OPEN,HAND_0,HAND_1,HAND_2,HAND_3,HAND_4,HAND_5,HAND_CLOSE,ALIEN_HAND};

    Recognition();
    static STATE getRecognition(std::vector<Point2i> vect_extrem,
                                std::vector<Point2i> vect_mutli,
                                Point2i center,
                                std::vector<std::vector<Point2i> > convexhull
                                );
    static STATE getState(std::vector<Point2i> defects);

};

#endif // RECOGNITION_H
