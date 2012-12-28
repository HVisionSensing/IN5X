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
    distanceTransform(img,dst,CV_DIST_L2,CV_DIST_MASK_5);
    normalize(dst,dst,0.0,1.0,NORM_MINMAX);
    return dst;
}
Mat Processing::getMaxLocHyst(Mat img, u_char Sh, u_char Sb,int connexity )
{
    Mat dst=Mat(img.rows,img.cols,CV_8UC1);

     int nbPixel = 0;
     int nbPixelModif = 0;

    for(int i=0 ; i < img.rows ; i++)
        for(int j=0 ; j < img.cols ; j++)
        {
            if(img.at<u_char>(i,j) > Sh){dst.at<u_char>(i,j) = 255;nbPixelModif++;}
            else if (img.at<u_char>(i,j) < Sb)dst.at<u_char>(i,j) = Sb;
            else{
                if(searchConnexity(img,i,j,Sh,connexity))
                    dst.at<u_char>(i,j) = Sh;
                else dst.at<u_char>(i,j) = Sb;
            }
        }
    return dst;
}
void Processing::searchConnexityPath(Mat img,Mat& dst, int i, int j,u_char Sb,u_char Sh,int pathCode)
{

   /* if(pathCode == CONNEXITY_4)
    {
        dst.at<u_char>(i,j) = 255;
        Scalar_<u_char> vois =  Scalar_<u_char>(img.at<u_char>(i-1,j),
                                                img.at<u_char>(i,j-1),
                                                img.at<u_char>(i,j+1),
                                                img.at<u_char>(i+1,j));
        u_char max = 0;
        int ind = 0;
        for(int k = 0 ; k< vois.rows ; k++ )
            if(vois[k] > max && vois[k])
            {
                max = vois[k];
                ind =k;
            }

        if(max >= Sb)
        {
            int X;
            int Y;
            switch(ind){
            case 0:
                X=i-1;
                Y=j;
                break;
            case 1:
                X=i;
                Y=j-1;
                break;
            case 2:
                X=i;
                Y=j+1;
                break;
            case 3:
                X=i+1;
                Y=j;
                break;
            }
            if(dst.at<u_char>(X,Y) != 255)
                searchConnexityPath(img,dst,X,Y,Sb,Sh,CONNEXITY_4);
        }

    }*/
    if(pathCode == CONNEXITY_8)
    {
        dst.at<u_char>(i,j) = 255;
        u_char max = 0;
        int X = -1,Y = -1;
        int nb = 0;
        for(int r = i-1 ; r <= i+1 ; r++)
            for(int c = j-1 ; c <= j+1; c++)
            {
                if(dst.at<u_char>(r,c) == 255)nb++;
                if(img.at<u_char>(r,c) >= max && img.at<u_char>(r,c) >= Sb && dst.at<u_char>(r,c) != 255 )
                {
                    max = img.at<u_char>(r,c) ;
                    X = r;
                    Y = c;
                }
            }
        if(X != -1 && Y!=-1 && nb==1)
        {
            dst.at<u_char>(X,Y) = 255;
            searchConnexityPath(img,dst,X,Y,Sb,Sh,CONNEXITY_8);
        }
    }


}

bool Processing::searchConnexity(Mat img, int i, int j,u_char Sh, int connexityCode)
{
    if(connexityCode == CONNEXITY_8)
        {
            for(int r = i-1 ; r <= i+1 ; r++)
                for(int c = j-1 ; c <= j+1; c++)
                {
                    if( img.at<u_char>(r,c) > Sh )
                    {
                       return true;
                    }
                }
        }
    return false;
}
