#include "processing.h"

Processing::Processing()
{
}

Mat Processing::multyThreshold(Mat src, std::vector<u_char> seuils)
{
    Mat dst = Mat(src.rows,src.cols,CV_8UC1,Scalar(255));

    std::vector<u_char>::iterator it = seuils.begin();

    int color = 255/seuils.size();

    int k = 0;
    for(it;it != seuils.end(); it++)
    {
        for (int i  = 0 ; i < src.rows ; i++)
            for (int j = 0 ; j < src.cols ; j ++)
            {
                if(dst.at<u_char>(i,j) == 255 && src.at<u_char>(i,j) <= *it)
                {
                    dst.at<u_char>(i,j) = color*k;
                }
            }
        k++;
    }
    return dst;
}

Mat Processing::threshold(Mat src, u_char seuil)
{
    Mat dst = Mat(src.rows,src.cols,CV_8UC1,Scalar(255));

    for (int i  = 0 ; i < src.rows ; i++)
        for (int j = 0 ; j < src.cols ; j ++)
        {
            if(src.at<u_char>(i,j) <= seuil)
                dst.at<u_char>(i,j) = 0;
            else 255;
        }
    return dst;
}

Mat Processing::getErosion(Mat &img,InputArray kernel)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1);

    erode(img,dst,kernel);

    return dst;

}

Mat Processing::getDilatation(Mat &img,InputArray kernel)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1);

    dilate(img,dst,kernel);

    return dst;

}

Mat Processing::getKernel(int shape,Size size)
{
    return getStructuringElement(shape,size);
}
