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
#include "display.h"

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

    Histogram1D h(img);
    Mat histImg = h.getHistogramImage();
    imshow("Histogram", histImg);

    h.cumulHist();
    h.derivCumul();

    //std::vector<u_char> vecSeuil = h.getSeuilByDerivCumul();

<<<<<<< HEAD
    Mat thresholded = Processing::threshold(img,vecSeuil[0]);
=======
    Mat thresholded = Processing::threshold(img,28);
    imshow("Threshold",thresholded);
>>>>>>> b13a3297701ebdf3b8bba67f242162dbe392d852

    Mat blur=PreProcessing::getMedianBlur(thresholded,9);
    imshow("Blur",blur);

<<<<<<< HEAD
    Mat ero = Processing::getErosion(inv,kernel,1);
    Mat fermeture = Processing::getDilatation(ero,kernel,1);
    imshow("main",fermeture);

    Mat squelette = Processing::getThinning(fermeture,Processing::CONNEXITY_8,50);

    Mat distance = Processing::getDistanceTransform(fermeture);
    Mat squeletteUChar = PreProcessing::getUCHARImage(distance,1.0);
    PreProcessing::getExpansion(squeletteUChar);
    Point2i center = Processing::getDistanceMax(squeletteUChar);
    std::vector<Point2i> vec_pt = Processing::getSkelExtremity(squelette);

    display::drawPoints(img,center,vec_pt);

    imshow("DT",squeletteUChar);
    imshow("Squelette Final",squelette);
=======
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
>>>>>>> b13a3297701ebdf3b8bba67f242162dbe392d852

    waitKey(0);
    fs.release();
	return 0;
}

