#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H

#include <opencv2/opencv.hpp>

using namespace cv;

class Histogram1D
{
private:
    int histSize[2]; // number of bins
    float hranges[2]; // min and max pixel value
    const float* ranges[1];
    int channels[1]; // only 1 channel used here
    int mNbPixel;
    MatND mHist;
    MatND mHistNorm;
    MatND mHistCumul;
    MatND mHistCumulDeriv;



public:
    const static int DERIV_FACTOR = 17;
    Histogram1D();
    Histogram1D(const Mat &image);
    MatND getHistogram();
    MatND getHistogramNorm();
    MatND getHistogramCumul();
    MatND getHistogramDerivCumul();
    Mat getHistogramImage();
    Mat getHistogramCumulImage();
    Mat getHistogramNormImage();
    Mat getHistogramCumulDerivImage();
    std::vector<u_char> getSeuilByDerivCumul();

    std::vector<u_char> mVecSeuil;

    void normalizeHist(float value);
    void cumulHist();
    void derivCumul();
    void computeHistogram(const Mat &image);
};

#endif
