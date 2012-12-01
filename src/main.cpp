/*
 * main.cpp
 *
 *  Created on: 8 nov. 2012
 *      Author: IN5X-team
 */

#include <opencv2/opencv.hpp>

#include "histogram1d.h"

using namespace cv;

int main(void)
{
	/**** Init window ***/
    namedWindow("Image viewer");

    FileStorage fs("../IN5X/res/1.yml", FileStorage::READ);

    Mat img;

    fs["M1"] >> img;

    //std::cout << img.rows << img.cols << std::endl;

    imshow("Image viewer", img);

    Histogram1D h;
    imshow("Histogram", h.getHistogramImage(img));

    Mat thresholded;
    threshold(img, thresholded, 0.58, 255, THRESH_BINARY);
    imshow("Thresholding", thresholded);


    // Need to implement a method to find the first pic of histogram
    // doesn't work yet
    MatND hist = h.getHistogram(img);
    normalize(hist, hist, 1.0);

    waitKey(0);
    fs.release();
	return 0;
}



