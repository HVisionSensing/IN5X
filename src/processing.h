#ifndef PROCESSING_H
#define PROCESSING_H

#include <opencv2/opencv.hpp>

using namespace cv;

class Processing
{
private:

    static void searchConnexityPath(Mat img,Mat& dst, int i, int j, u_char Sb, u_char Sh, int pathCode);
    static bool searchConnexity(Mat img, int i, int j, u_char Sh, int connexityCode);
public:

    const static int CONNEXITY_4 = 0;
    const static int CONNEXITY_8 = 1;

    int nbPixel;

    Processing();
    static Mat threshold(Mat src, u_char seuil);
    static Mat multyThreshold(Mat src, std::vector<u_char> seuils);
    static Mat getErosion(Mat img,InputArray kernel, int iteration = 1);
    static Mat getDilatation(Mat img,InputArray kernel, int iteration = 1);
    static Mat getKernel(int shape,Size size);
    static Mat getInverse(Mat img);
    static Mat getDistanceTransform(Mat img);
    static Mat getMaxLocHyst(Mat img,u_char Sh, u_char Sb ,int connexity );
};

#endif // PROCESSING_H
