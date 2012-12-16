#ifndef PROCESSING_H
#define PROCESSING_H

#include <opencv2/opencv.hpp>

using namespace cv;

class Processing
{
public:
    Processing();
    static Mat threshold(Mat src, u_char seuil);
    static Mat multyThreshold(Mat src, std::vector<u_char> seuils);
    static Mat getErosion(Mat& img,InputArray kernel);
    static Mat getDilatation(Mat &img,InputArray kernel);
    static Mat getKernel(int shape,Size size);
};

#endif // PROCESSING_H
