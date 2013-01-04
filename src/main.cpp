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

    imshow("Image",img);

    Histogram1D h(img);
    Mat histImg = h.getHistogramImage();
    imshow("Histogram", histImg);

    h.cumulHist();
    h.derivCumul();

    //std::cout<<h.getHistogramDerivCumul()<<std::endl;

    //int seuil = PreProcessing::getThreshold(h.getHistogram());

    std::vector<u_char> vecSeuil = h.getSeuilByDerivCumul();

    /*imshow("Histogram cumulé",h.getHistogramCumulImage());
    imshow("Histogram cumulé dérivé",h.getHistogramCumulDerivImage());*/

    Mat thresholded = Processing::threshold(img,28);
    imshow("Threshold",thresholded);

    Mat blur=PreProcessing::getMedianBlur(thresholded,8);
    imshow("Blur",blur);

    std::vector<int> coords = Processing::getExtractCoord(blur);
    Mat extracted = Processing::getExtractMat(blur,coords);
    imshow("Extracted",extracted);
    Mat inv = Processing::getInverse(blur);

    Processing::getConvexHullMat(inv);

    /*Mat squelette = Processing::getThinning(inv,Processing::CONNEXITY_8,50);

    Mat distance = Processing::getDistanceTransform(inv);
    Mat squeletteUChar = PreProcessing::getUCHARImage(distance,1.0);
    PreProcessing::getExpansion(squeletteUChar);
    Point2i center = Processing::getDistanceMax(squeletteUChar);
    std::vector<Point2i> vec_pt = Processing::getSkelExtremity(squelette);

    display::drawPoints(img,center,vec_pt,coords);

    imshow("DT",squeletteUChar);
    imshow("Squelette Final",squelette);*/

    waitKey(0);
    fs.release();
	return 0;
}
