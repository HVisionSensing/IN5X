#include "preprocessing.h"

PreProcessing::PreProcessing()
{}

Mat PreProcessing::getUCHARImage(Mat& img,float value)
{
    Mat dst(img.rows,img.cols,CV_8UC1);

    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j<img.cols;j++)
        {
            dst.at<unsigned char>(i,j)=u_char(img.at<float>(i,j)*255/value);
        }
    }
    return dst;
}
void PreProcessing::getMedianFilter3(Mat &img)
{
    for(int i = 1 ; i < img.rows-1 ; i++)
        for(int j = 1 ; j < img.cols-1 ; j++)
        {
            int tmp = 0;
            tmp += img.at<u_char>(i-1,j-1);
            tmp += img.at<u_char>(i-1,j-1);
            tmp += img.at<u_char>(i-1,j-1);

            tmp += img.at<u_char>(i,j-1);
            tmp += img.at<u_char>(i,j);
            tmp += img.at<u_char>(i,j+1);

            tmp += img.at<u_char>(i+1,j-1);
            tmp += img.at<u_char>(i+1,j);
            tmp += img.at<u_char>(i+1,j+1);

            img.at<u_char>(i,j) = tmp/9;
        }
}
void PreProcessing::getMedianFilter5(Mat &img)
{
    for(int i = 1 ; i < img.rows-1 ; i++)
        for(int j = 1 ; j < img.cols-1 ; j++)
        {
            int tmp = 0;
            tmp += img.at<u_char>(i-2,j-2);
            tmp += img.at<u_char>(i-2,j-1);
            tmp += img.at<u_char>(i-2,j);
            tmp += img.at<u_char>(i-2,j+1);
            tmp += img.at<u_char>(i-2,j+2);

            tmp += img.at<u_char>(i-1,j-2);
            tmp += img.at<u_char>(i-1,j-1);
            tmp += img.at<u_char>(i-1,j);
            tmp += img.at<u_char>(i-1,j+1);
            tmp += img.at<u_char>(i-1,j+2);

            tmp += img.at<u_char>(i,j-2);
            tmp += img.at<u_char>(i,j-1);
            tmp += img.at<u_char>(i,j);
            tmp += img.at<u_char>(i,j+1);
            tmp += img.at<u_char>(i,j+2);

            tmp += img.at<u_char>(i+1,j-2);
            tmp += img.at<u_char>(i+1,j-1);
            tmp += img.at<u_char>(i+1,j);
            tmp += img.at<u_char>(i+1,j+1);
            tmp += img.at<u_char>(i+1,j+2);

            tmp += img.at<u_char>(i+2,j-2);
            tmp += img.at<u_char>(i+2,j-1);
            tmp += img.at<u_char>(i+2,j);
            tmp += img.at<u_char>(i+2,j+1);
            tmp += img.at<u_char>(i+2,j+2);

            img.at<u_char>(i,j) = tmp/25;
        }
}

void PreProcessing::getExpansion(Mat &img)
{
    u_char max = 0;
    u_char min = 255 ;

    for(int i = 0 ; i < img.rows ; i++)
        for(int j = 0 ; j < img.cols ; j++)
        {
            if(img.at<u_char>(i,j) > max)
                max = img.at<u_char>(i,j);
            if(img.at<u_char>(i,j) < min)
                min = img.at<u_char>(i,j);
        }

    uchar tmp = max - min;
    for(int i = 0 ; i < img.rows ; i++)
        for(int j = 0 ; j < img.cols ; j++)
        {
            if(tmp != 0)
                img.at<u_char>(i,j) = (255*(img.at<u_char>(i,j)-min))/tmp;
        }
}

Mat1b PreProcessing::getMedianBlur(Mat img, int blurSize)
{

    Mat1b blur = Mat(img.rows,img.cols,CV_8UC1);

    for ( int i = 1; i < blurSize; i = i + 2 )
    {
        medianBlur(img,blur,i);
    }
    return blur;
}


