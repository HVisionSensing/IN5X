/*
 * main.cpp
 *
 *  Created on: 8 nov. 2012
 *      Author: IN5X-team
 */

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char ** argv)
{
	/**** Init window ***/
    namedWindow("Image viewer");

    FileStorage fs("../IN5X/res/1.yml",FileStorage::READ);

    Mat img;

    fs["M1"] >> img;

    //std::cout << img.rows << img.cols << std::endl;

    imshow("Image viewer",img);



    waitKey(0);
    fs.release();
	return 0;
}



