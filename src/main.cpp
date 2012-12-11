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
    FileStorage fs("../IN5X/res/2.yml", FileStorage::READ);

    Mat temp;

    fs["M1"] >> temp;

    Mat img(temp.rows,temp.cols,CV_8UC1);

    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j<img.cols;j++)
        {
            img.at<unsigned char>(i,j)=int(temp.at<float>(i,j)*255/5);
        }
    }

    imshow("Image viewer", img);

    Histogram1D h;
    imshow("Histogram", h.getHistogramImage(img));

    Mat thresholded;
    threshold(temp, thresholded, 0.58, 255, THRESH_BINARY);
    imshow("Thresholding", thresholded);


    // Need to implement a method to find the first pic of histogram
    // doesn't work yet
    MatND hist = h.getHistogram(temp);
    normalize(hist, hist, 1.0);

    waitKey(0);
    fs.release();
	return 0;
}



