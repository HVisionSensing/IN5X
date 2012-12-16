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
    FileStorage fs("../IN5X/res/6.yml", FileStorage::READ);

    Mat temp;

    fs["M1"] >> temp;

    Mat img =PreProcessing::getUCHARImage(temp);
    PreProcessing::getExpansion(img);
    PreProcessing::getMedianFilter3(img);

    imshow("Image viewer", img);

    Histogram1D h(img);
    Mat histImg = h.getHistogramImage();
    imshow("Histogram", histImg);

    h.cumulHist();
    h.derivCumul();

    std::vector<u_char> vecSeuil = h.getSeuilByDerivCumul();

    Mat thresholded = Processing::threshold(img,vecSeuil.at(1));
    Mat multythresholded = Processing::multyThreshold(img,vecSeuil);

    imshow("Threshold",thresholded );
    imshow("Multy Threshold",multythresholded );

    Mat kernel = Processing::getKernel(CV_SHAPE_ELLIPSE,Size_<int>(5,5));

    Mat ero = Processing::getErosion(thresholded,kernel);
    Mat ouverture = Processing::getDilatation(ero,kernel);

    Mat dil = Processing::getDilatation(thresholded,kernel);
    Mat fermeture = Processing::getErosion(dil,kernel);

    imshow("Ouverture",ouverture);
    imshow("Fermeture", fermeture);

    waitKey(0);
    fs.release();
	return 0;
}

