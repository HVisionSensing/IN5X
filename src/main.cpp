/*
 * main.cpp
 *
 *  Created on: 8 nov. 2012
 *      Author: IN5X-team
 */

#include <opencv2/opencv.hpp>

#include "histogram1d.h"
#include "preprocessing.h"
#include "processing.h"
using namespace cv;

void calcSeuil(MatND &hist);

int main(void)
{
    FileStorage fs("../IN5X/res/7.yml", FileStorage::READ);

    Mat temp;

    fs["M1"] >> temp;

    Mat img =PreProcessing::getUCHARImage(temp,5.0);
    PreProcessing::getExpansion(img);
    PreProcessing::getMedianFilter3(img);

    imshow("Image viewer", img);

    Histogram1D h(img);
    Mat histImg = h.getHistogramImage();
    imshow("Histogram", histImg);

    h.cumulHist();
    h.derivCumul();

    //std::vector<u_char> vecSeuil = h.getSeuilByDerivCumul();

    Mat thresholded = Processing::threshold(img,28);
    imshow("Threshold",thresholded);

    Mat blur=PreProcessing::getMedianBlur(thresholded,9);
    imshow("Blur",blur);

    Mat extracted = Processing::getExtractMat(blur);
    imshow("Extracted",extracted);

    Mat inv = Processing::getInverse(extracted);

    Mat distance = Processing::getDistanceTransform(inv);

    Mat distanceUChar = PreProcessing::getUCHARImage(distance,1.0);
    PreProcessing::getExpansion(distanceUChar);

    Mat kernel2 = Processing::getKernel(CV_SHAPE_RECT,Size_<int>(2,2));
    Mat squeletteDMap = Processing::getSkeletonByDistanceMap(distanceUChar);
    squeletteDMap=Processing::getErosion(squeletteDMap,kernel2,1);
    squeletteDMap=Processing::getDilatation(squeletteDMap,kernel2,1);

    imshow("Cartes des distances",distanceUChar);
    imshow("Squelette",squeletteDMap);

    waitKey(0);
    fs.release();
	return 0;
}

