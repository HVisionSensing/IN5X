/*
 * main.cpp
 *
 *  Created on: 8 nov. 2012
 *      Author: IN5X-team
 */

#include <opencv.hpp>

int main(int argc, char ** argv)
{
	/**** Init window ***/
	cvNamedWindow("Image viewer",CV_WINDOW_AUTOSIZE);


	CvMat* im;
	im = (CvMat*)cvLoad("./res/5.yml");
	cvShowImage("Image viewer",im);

	cvWaitKey(0);
	return 0;
}



