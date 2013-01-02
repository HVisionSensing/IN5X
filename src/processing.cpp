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

Mat Processing::getErosion(Mat img,InputArray kernel,int iteration)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1);

    erode(img,dst,kernel,Point(-1,-1),iteration);

    return dst;

}

Mat Processing::getDilatation(Mat img,InputArray kernel,int iteration)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1);

    dilate(img,dst,kernel,Point(-1,-1),iteration);

    return dst;

}

Mat Processing::getKernel(int shape,Size size)
{
    return getStructuringElement(shape,size);
}

Mat Processing::getInverse(Mat img)
{

    Mat dst = Mat(img.rows,img.cols,CV_8UC1,Scalar(255));

    for (int i  = 0 ; i < img.rows ; i++)
        for (int j = 0 ; j < img.cols ; j ++)
        {
            dst.at<u_char>(i,j) = 255 - img.at<u_char>(i,j);
        }
    return dst;
}
Mat Processing::getDistanceTransform(Mat img)
{
    Mat dst = Mat(img.rows,img.cols,CV_32FC1);
    distanceTransform(img,dst,CV_DIST_L2,CV_DIST_MASK_3);
    normalize(dst,dst,0.0,1.0,NORM_MINMAX);
    return dst;
}
Mat Processing::getSkeletonByDistanceMap(Mat dst)
{
    Mat laplace,tempSkeleton;
    Laplacian(dst, laplace, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    compare(laplace,-3,tempSkeleton,CV_CMP_LE);

    Mat skeleton = tempSkeleton;

    return skeleton;
}

int Processing::getNeighbors8(img,i,j)
{
    int k=0;

    for(r=i-1;r<=i+1;r++)
        for(c=j-1;c<=j+1;c++)
            if(r!=i || c!=j)
            {
                if(img.at<u_char>(r,c)==255) k++;
            }


    return k;
}

Mat Processing::getExtractMat(Mat img)
{
    int xL,xR,yU,yD;

    bool isXLassigned=false,isXRassigned=false,isYUassigned=false,isYDassigned=false;

    for(int i=0; i<img.rows;i++)
    {
        if(isYUassigned) break;

        for(int j=0; j<img.cols;j++)
        {
            if(img.at<u_char>(i,j)==0)
            {
                yU=i-1;
                isYUassigned=true;
                break;
            }
        }

    }
    for(int i=yU+1; i<img.rows;i++)
    {
        bool containsBlackPx=false;
        if(isYDassigned) break;

        for(int j=0; j<img.cols;j++)
        {
            if(img.at<u_char>(i,j)==0)
            {
                containsBlackPx=true;
                break;
            }
        }
        if(!containsBlackPx)
        {
            yD=i+1;
            isYDassigned=true;
        }
    }

    for(int j=0; j<img.cols;j++)
    {
        if(isXLassigned) break;

        for(int i=yU; i<yD;i++)
        {
            if(img.at<u_char>(i,j)==0)
            {
                xL=j-1;
                isXLassigned=true;
                break;
            }
        }

    }
    for(int j=xL+1; j<img.cols;j++)
    {
        bool containsBlackPx=false;
        if(isXRassigned) break;

        for(int i=yU; i<yD;i++)
        {
            if(img.at<u_char>(i,j)==0)
            {
                containsBlackPx=true;
                break;
            }
        }
        if(!containsBlackPx)
        {
            xR=j+1;
            isXRassigned=true;
        }
    }

    Mat extracted = extractMat(img,xL,xR,yU,yD);

    return extracted;
}

Mat Processing::extractMat(Mat img, int xL, int xR, int yU, int yD)
{
    int width=xR-xL;
    int height=yD-yU;

    Rect rect = Rect(xL,yU,width,height);
    Mat extracted = img(rect);

    return extracted;

}
