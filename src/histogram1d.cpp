#include "histogram1d.h"

Histogram1D::Histogram1D() {
    histSize[0] = 512;
    hranges[0] = 0;
    hranges[1] = 255;
    ranges[0] = hranges;
    channels[0] = 0; // by default, we look at channel 0
}

MatND Histogram1D::getHistogram(const Mat &image) {
    MatND hist;

    // Compute histogram
    calcHist(&image,
                1, // histogram from 1 image only
                channels, // the channel used
                Mat(), // no mask is used
                hist, // the resulting histogram
                1, // it is a 1D histogram
                histSize, // number of bins
                ranges // pixel value range
             );

    return hist;
}

// Computes the 1D histogram and returns an image of it.
Mat Histogram1D::getHistogramImage(const Mat &image) {
    // Compute histogram first
    MatND hist = getHistogram(image);

    // Get min and max bin values
    double maxVal = 0;
    double minVal = 0;

    minMaxLoc(hist, &minVal, &maxVal, 0, 0);

    // Image on which to display histogram
    Mat histImg(histSize[0], histSize[0], CV_8U, Scalar(255));

    // set highest point at 90% of nbins
    int hpt = static_cast<int>(0.9*histSize[0]);

    // Draw a vertical line for each bin
    for (int h = 0; h < histSize[0]; h++ ) {
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);

        // This function draws a line between 2 points
        line(histImg, Point(h, histSize[0]),
        Point(h, histSize[0] - intensity),
        Scalar::all(0));
    }

    return histImg;
}
