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
    FileStorage fs("../IN5X/res/4.yml", FileStorage::READ);

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

    std::vector<u_char> vecSeuil = h.getSeuilByDerivCumul();

    Mat thresholded = Processing::threshold(img,vecSeuil.at(1));

    Mat kernel = Processing::getKernel(CV_SHAPE_RECT,Size_<int>(3,3));
    Mat inv = Processing::getInverse(thresholded);

    Mat ero = Processing::getErosion(inv,kernel,1);
    Mat fermeture = Processing::getDilatation(ero,kernel,1);

    Mat squelette = Processing::getDistanceTransform(fermeture);

    Mat squeletteUChar = PreProcessing::getUCHARImage(squelette,1.0);
    PreProcessing::getExpansion(squeletteUChar);
    Mat squeletteFinal = Processing::getMaxLocHyst(squeletteUChar,180,(u_char)0.4*180,Processing::CONNEXITY_8);

    imshow("DT",squeletteUChar);
    imshow("Squelette Final",squeletteFinal);


    waitKey(0);
    fs.release();
	return 0;
}

