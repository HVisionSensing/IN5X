#include "histogram1d.h"

Histogram1D::Histogram1D() {
    histSize[0] = 512;
    histSize[1] = 512;
    hranges[0] = 0;
    hranges[1] = 255;
    ranges[0] = hranges;
    channels[0] = 0; // by default, we look at channel 0

}

Histogram1D::Histogram1D(const Mat &image) {
    histSize[0] = 256;
    histSize[1]= 256;
    hranges[0] = 0;
    hranges[1] = 255;
    ranges[0] = hranges;
    channels[0] = 0; // by default, we look at channel 0

    computeHistogram(image);

}

void Histogram1D::computeHistogram(const Mat &image) {

    mNbPixel = image.rows*image.cols;
    // Compute histogram
    calcHist(&image,
                1, // histogram from 1 image only
                channels, // the channel used
                Mat(), // no mask is used
                mHist, // the resulting histogram
                1, // it is a 1D histogram
                histSize, // number of bins
                ranges // pixel value range
             );
}
MatND Histogram1D::getHistogram()
{
    if(!mHist.empty())
    {
        return mHist;
    }
}
MatND Histogram1D::getHistogramNorm()
{
    if(!mHistNorm.empty())
    {
        return mHistNorm;
    }
}
MatND Histogram1D::getHistogramCumul()
{
    if(!mHistCumul.empty())
    {
        return mHistCumul;
    }
}
MatND Histogram1D::getHistogramDerivCumul()
{
    if(!mHistCumulDeriv.empty())
    {
        return mHistCumulDeriv;
    }
}

// Computes the 1D histogram and returns an image of it.
Mat Histogram1D::getHistogramImage() {

    // Get min and max bin values
    double maxVal = 0;
    double minVal = 0;

    minMaxLoc(mHist, &minVal, &maxVal, 0, 0);

    // Image on which to display histogram
    Mat histImg(histSize[0], histSize[1], CV_8U, Scalar(255));

    // set highest point at 90% of nbins
    int hpt = static_cast<int>(0.9*histSize[1]);

    // Draw a vertical line for each bin
    for (int h = 0; h < histSize[0]; h++ ) {
        float binVal = mHist.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);

        // This function draws a line between 2 points
        line(histImg, Point(h, histSize[1]),
             Point(h, histSize[1] - intensity),
             Scalar::all(0));
    }
    return histImg;
}

Mat Histogram1D::getHistogramNormImage() {

    // Get min and max bin values
    double maxVal = 0;
    double minVal = 0;

    minMaxLoc(mHistNorm, &minVal, &maxVal, 0, 0);

    // Image on which to display histogram
    Mat histImg(histSize[0], histSize[1], CV_8U, Scalar(255));

    // set highest point at 90% of nbins
    int hpt = static_cast<int>(0.9*histSize[1]);

    // Draw a vertical line for each bin
    for (int h = 0; h < histSize[0]; h++ ) {
        float binVal = mHistNorm.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);

        // This function draws a line between 2 points
        line(histImg, Point(h, histSize[1]),
             Point(h, histSize[1] - intensity),
             Scalar::all(0));
    }
    return histImg;
}

Mat Histogram1D::getHistogramCumulImage() {

    // Get min and max bin values
    double maxVal = 0;
    double minVal = 0;

    minMaxLoc(mHistCumul, &minVal, &maxVal, 0, 0);

    // Image on which to display histogram
    Mat histImg(histSize[0], histSize[1], CV_8U, Scalar(255));

    // set highest point at 90% of nbins
    int hpt = static_cast<int>(0.9*histSize[1]);

    // Draw a vertical line for each bin
    for (int h = 0; h < histSize[0]; h++ ) {
        float binVal = mHistCumul.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);

        // This function draws a line between 2 points
        line(histImg, Point(h, histSize[1]),
             Point(h, histSize[1] - intensity),
             Scalar::all(0));
    }
    return histImg;
}

Mat Histogram1D::getHistogramCumulDerivImage() {

    // Get min and max bin values
    double maxVal = 0;
    double minVal = 0;

    minMaxLoc(mHistCumulDeriv, &minVal, &maxVal, 0, 0);

    // Image on which to display histogram
    Mat histImg(histSize[0], histSize[1], CV_8U, Scalar(255));

    // set highest point at 90% of nbins
    int hpt = static_cast<int>(0.9*histSize[1]);

    // Draw a vertical line for each bin
    for (int h = 0; h < histSize[0]; h++ ) {
        float binVal = mHistCumulDeriv.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);

        // This function draws a line between 2 points
        line(histImg, Point(h, histSize[1]),
             Point(h, histSize[1] - intensity),
             Scalar::all(0));
    }
    return histImg;
}
void Histogram1D::normalizeHist(float value)
{
    mHistNorm = Mat(mHist.rows,mHist.cols,CV_32FC1,Scalar(0.0));
    for(int i = 0 ; i < mHist.rows ; i++)
    {
        mHistNorm.at<float>(i)= mHist.at<float>(i)/mNbPixel*value;
    }
}
void Histogram1D::cumulHist()
{
    mHistCumul = Mat(mHist.rows,mHist.cols,CV_32FC1,Scalar(0.0));
    for(int i = 0 ; i < mHist.rows ; i++)
    {
        mHistCumul.at<float>(i)= mHistCumul.at<float>(i-1) + mHist.at<float>(i);
    }
}

void Histogram1D::derivCumul()
{
    if(!mHistCumul.empty())
    {
        mHistCumulDeriv= Mat(mHist.rows,mHist.cols,CV_32FC1,Scalar(0.0));

        for (int i = 0 ; i < mHistCumulDeriv.rows ; i++)
        {
            float tmp = 0;
            for(int k = i-DERIV_FACTOR ; k < i+DERIV_FACTOR ; k++ )
                tmp += (mHistCumul.at<float>(k)/(2*DERIV_FACTOR+1));
            mHistCumulDeriv.at<float>(i) = mHistCumul.at<float>(i)-tmp;
        }
        std::cout << mHistCumulDeriv << std::endl;
    }
}

std::vector<u_char> Histogram1D::getSeuilByDerivCumul()
{
    if(!mHistCumulDeriv.empty())
    {
        for(int i = 1 ; i < mHistCumulDeriv.rows-1; i++)
            if( mHistCumulDeriv.at<float>(i-1) > 0 && mHistCumulDeriv.at<float>(i) < 0 )
                mVecSeuil.push_back(i);
    }
    return mVecSeuil;
}
