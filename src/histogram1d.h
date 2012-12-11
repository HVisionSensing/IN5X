#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H

#include <opencv2/opencv.hpp>

using namespace cv;

class Histogram1D
{
private:
    int histSize[1]; // number of bins
    float hranges[2]; // min and max pixel value
    const float* ranges[1];
    int channels[1]; // only 1 channel used here


public:
    Histogram1D();
    MatND getHistogram(const Mat &image);
    Mat getHistogramImage(const Mat &image);
};

#endif
