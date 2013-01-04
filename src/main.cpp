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
    FileStorage fs("../IN5X/res/4.yml", FileStorage::READ);

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

    std::vector<u_char> vecSeuil = h.getSeuilByDerivCumul();

    Mat thresholded = Processing::threshold(img,vecSeuil[0]);

    Mat kernel = Processing::getKernel(CV_SHAPE_RECT,Size_<int>(3,3));
    Mat inv = Processing::getInverse(thresholded);

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

    waitKey(0);
    fs.release();
	return 0;
}

