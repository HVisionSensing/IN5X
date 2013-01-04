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

std::vector<int> Processing::getExtractCoord(Mat img)
{
    int xL,xR,yU,yD;
    std::vector<int> coords;

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

    coords.push_back(xL);
    coords.push_back(xR);
    coords.push_back(yU);
    coords.push_back(yD);

    return coords;
}

Mat Processing::getExtractMat(Mat img, std::vector<int> coords)
{
    int width=coords[1]-coords[0];
    int height=coords[3]-coords[2];

    Rect rect = Rect(coords[0],coords[2],width,height);
    Mat extracted = img(rect);

    return extracted;

}

Mat Processing::getCannyDetector(Mat img, double Sh, double Sb)
{
    Mat dst = img.clone();

    Canny(img,dst,Sh,Sb);

    return dst;
}
Mat Processing::getThinning(Mat1b img,int code,int iteration)
{
    bool isDone = false;
    ELEM_STATE currentState = NO;
    Mat1b dst = img.clone();
    Mat1b diff = Mat(img.rows,img.cols,CV_8UC1);
    Mat1b tmp;
    tmp = dst.clone();
    int it =0;
    while(it < iteration*8)
    {
        it++;
        currentState = getNextState(currentState);

        for(int i = 1; i < img.rows-1; i++)
            for(int j=1; j < img.cols-1; j++)
            {
                switch(currentState)
                {
                case N:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i-1,j-1) == 255 &&
                            dst.at<u_char>(i-1,j) == 255 &&
                            dst.at<u_char>(i-1,j+1) == 255 &&
                            dst.at<u_char>(i+1,j-1) == 0 &&
                            dst.at<u_char>(i+1,j) == 0 &&
                            dst.at<u_char>(i+1,j+1) == 0 ) tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case NE:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i-1,j) == 255 &&
                            dst.at<u_char>(i-1,j+1) == 255 &&
                            dst.at<u_char>(i,j+1) == 255 &&
                            dst.at<u_char>(i,j-1) == 0 &&
                            dst.at<u_char>(i+1,j-1) == 0 &&
                            dst.at<u_char>(i+1,j) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case E:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i+1,j+1) == 255 &&
                            dst.at<u_char>(i,j+1) == 255 &&
                            dst.at<u_char>(i+1,j+1) == 255 &&
                            dst.at<u_char>(i-1,j-1) == 0 &&
                            dst.at<u_char>(i,j-1) == 0 &&
                            dst.at<u_char>(i+1,j-1) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case SE:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i,j+1) == 255 &&
                            dst.at<u_char>(i+1,j) == 255 &&
                            dst.at<u_char>(i+1,j+1) == 255 &&
                            dst.at<u_char>(i-1,j-1) == 0 &&
                            dst.at<u_char>(i-1,j) == 0 &&
                            dst.at<u_char>(i,j-1) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case S:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i+1,j-1) == 255 &&
                            dst.at<u_char>(i+1,j) == 255 &&
                            dst.at<u_char>(i+1,j+1) == 255 &&
                            dst.at<u_char>(i-1,j-1) == 0 &&
                            dst.at<u_char>(i-1,j) == 0 &&
                            dst.at<u_char>(i-1,j+1) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case SO:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i,j-1) == 255 &&
                            dst.at<u_char>(i+1,j-1) == 255 &&
                            dst.at<u_char>(i+1,j) == 255 &&
                            dst.at<u_char>(i-1,j) == 0 &&
                            dst.at<u_char>(i-1,j+1) == 0 &&
                            dst.at<u_char>(i,j+1) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case O:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i-1,j-1) == 255 &&
                            dst.at<u_char>(i,j-1) == 255 &&
                            dst.at<u_char>(i+1,j-1) == 255 &&
                            dst.at<u_char>(i-1,j+1) == 0 &&
                            dst.at<u_char>(i,j+1) == 0 &&
                            dst.at<u_char>(i+1,j+1) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                case NO:
                    if(dst.at<u_char>(i,j) == 255 &&
                            dst.at<u_char>(i-1,j-1) == 255 &&
                            dst.at<u_char>(i-1,j) == 255 &&
                            dst.at<u_char>(i,j-1) == 255 &&
                            dst.at<u_char>(i,j+1) == 0 &&
                            dst.at<u_char>(i+1,j) == 0 &&
                            dst.at<u_char>(i+1,j+1) == 0 )  tmp.at<u_char>(i,j) = 255;
                    else tmp.at<u_char>(i,j) = 0;
                    break;
                default:
                    break;
                }
            }

        for(int i = 1; i < dst.rows-1; i++)
            for(int j=1; j < dst.cols-1; j++)
            {
                if(tmp.at<u_char>(i,j) == 255)
                    dst.at<u_char>(i,j) = 0;
            }
    }
    return dst;
}
Processing::ELEM_STATE Processing::getNextState(Processing::ELEM_STATE cur)
{
    ELEM_STATE next;
    switch(cur)
    {
    case N:
        next = NE;
        break;
    case NE:
        next = E;
        break;
    case E:
        next = SE;
        break;
    case SE:
        next = S;
        break;
    case S:
        next = SO;
        break;
    case SO:
        next = O;
        break;
    case O:
        next = NO;
        break;
    case NO:
        next = N;
        break;
    }
    return next;
}

Point2i Processing::getDistanceMax(Mat img)
{
    Point_<int> pt;
    int indX = 0;
    int indY = 0;
    u_char max = 0;
    for(int i = 0; i < img.rows; i++)
        for(int j=0; j < img.cols; j++)
        {
            if(img.at<u_char>(i,j) >= max)
            {
                max = img.at<u_char>(i,j);
                indX = i;
                indY = j;
            }
        }
    pt = Point_<int>(indY,indX);
    return pt;
}

std::vector<Point2i> Processing::getSkelExtremity(Mat img)
{
    ELEM_STATE currentState = NO;
    Point2i pt;
    std::vector<Point2i> vect_pt;
    int it =0;
    while(it < 8)
    {
        it++;
        currentState = getNextState(currentState);

        for(int i = 1; i < img.rows-1; i++)
            for(int j=1; j < img.cols-1; j++)
            {
                switch(currentState)
                {
                case N:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i-1,j) == 255 &&
                            img.at<u_char>(i,j-1) == 0 &&
                            img.at<u_char>(i+1,j-1) == 0 &&
                            img.at<u_char>(i+1,j) == 0 &&
                            img.at<u_char>(i+1,j+1) == 0 &&
                            img.at<u_char>(i,j+1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NE:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i-1,j+1) == 255 &&
                            img.at<u_char>(i-1,j-1) == 0 &&
                            img.at<u_char>(i,j-1) == 0 &&
                            img.at<u_char>(i+1,j-1) == 0 &&
                            img.at<u_char>(i+1,j) == 0 &&
                            img.at<u_char>(i+1,j+1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case E:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i,j+1) == 255 &&
                            img.at<u_char>(i-1,j-1) == 0 &&
                            img.at<u_char>(i-1,j) == 0 &&
                            img.at<u_char>(i,j-1) == 0 &&
                            img.at<u_char>(i+1,j-1) == 0 &&
                            img.at<u_char>(i+1,j) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SE:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i+1,j+1) == 255 &&
                            img.at<u_char>(i-1,j+1) == 0 &&
                            img.at<u_char>(i-1,j) == 0 &&
                            img.at<u_char>(i-1,j-1) == 0 &&
                            img.at<u_char>(i,j-1) == 0 &&
                            img.at<u_char>(i+1,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case S:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i+1,j) == 255 &&
                            img.at<u_char>(i,j+1) == 0 &&
                            img.at<u_char>(i-1,j+1) == 0 &&
                            img.at<u_char>(i-1,j) == 0 &&
                            img.at<u_char>(i-1,j-1) == 0 &&
                            img.at<u_char>(i,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SO:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i+1,j-1) == 255 &&
                            img.at<u_char>(i+1,j+1) == 0 &&
                            img.at<u_char>(i,j+1) == 0 &&
                            img.at<u_char>(i-1,j+1) == 0 &&
                            img.at<u_char>(i-1,j) == 0 &&
                            img.at<u_char>(i-1,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case O:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i,j-1) == 255 &&
                            img.at<u_char>(i-1,j) == 0 &&
                            img.at<u_char>(i-1,j+1) == 0 &&
                            img.at<u_char>(i,j+1) == 0 &&
                            img.at<u_char>(i+1,j+1) == 0 &&
                            img.at<u_char>(i+1,j) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NO:
                    if(img.at<u_char>(i,j) == 255 &&
                            img.at<u_char>(i-1,j-1) == 255 &&
                            img.at<u_char>(i-1,j+1) == 0 &&
                            img.at<u_char>(i,j+1) == 0 &&
                            img.at<u_char>(i+1,j+1) == 0 &&
                            img.at<u_char>(i+1,j) == 0 &&
                            img.at<u_char>(i+1,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                default:
                    break;
                }
            }
    }
    return vect_pt;
}
std::vector<Point2i> Processing::getMultiPoints(Mat img)
{
    ELEM_STATE currentState = NO;
    Point2i pt;
    std::vector<Point2i> vect_pt;
    int it =0;
    while(it < 8)
    {
        it++;
        currentState = getNextState(currentState);

        for(int i = 1; i < img.rows-1; i++)
            for(int j=1; j < img.cols-1; j++)
            {
                switch(currentState)
                {
                case N:
                    if((img.at<u_char>(i,j-1) == 255 &&
                        img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i,j+1) == 255 &&
                        img.at<u_char>(i+1,j) == 255 &&
                        img.at<u_char>(i-1,j-1) == 0 &&
                        img.at<u_char>(i-1,j+1) == 0 &&
                        img.at<u_char>(i+1,j+1) == 0 &&
                        img.at<u_char>(i+1,j-1) == 0) ||
                            (img.at<u_char>(i,j-1) == 255 &&
                             img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i,j+1) == 255 &&
                             img.at<u_char>(i+1,j) == 255 &&
                             img.at<u_char>(i-1,j) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NE:
                    if((img.at<u_char>(i-1,j-1) == 255 &&
                        img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i+1,j-1) == 255 &&
                        img.at<u_char>(i+1,j+1) == 255 &&
                        img.at<u_char>(i-1,j) == 0 &&
                        img.at<u_char>(i,j-1) == 0 &&
                        img.at<u_char>(i,j+1) == 0 &&
                        img.at<u_char>(i+1,j) == 0)||
                            (img.at<u_char>(i-1,j-1) == 255 &&
                             img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i+1,j-1) == 255 &&
                             img.at<u_char>(i+1,j+1) == 255 &&
                             img.at<u_char>(i-1,j+1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case E:
                    if((img.at<u_char>(i-1,j) == 255 &&
                        img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i+1,j) == 255 &&
                        img.at<u_char>(i,j-1) == 255 &&
                        img.at<u_char>(i-1,j-1) == 0 &&
                        img.at<u_char>(i-1,j+1) == 0 &&
                        img.at<u_char>(i+1,j+1) == 0 &&
                        img.at<u_char>(i+1,j-1) == 0)||
                            (img.at<u_char>(i-1,j) == 255 &&
                             img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i+1,j) == 255 &&
                             img.at<u_char>(i,j-1) == 255 &&
                             img.at<u_char>(i,j+1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SE:
                    if((img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i-1,j-1) == 255 &&
                        img.at<u_char>(i-1,j+1) == 255 &&
                        img.at<u_char>(i+1,j-1) == 255 &&
                        img.at<u_char>(i-1,j) == 0 &&
                        img.at<u_char>(i,j-1) == 0 &&
                        img.at<u_char>(i,j+1) == 0 &&
                        img.at<u_char>(i+1,j) == 0)||
                            (img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i-1,j-1) == 255 &&
                             img.at<u_char>(i-1,j+1) == 255 &&
                             img.at<u_char>(i+1,j-1) == 255 &&
                             img.at<u_char>(i-1,j) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case S:
                    if((img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i-1,j) == 255 &&
                        img.at<u_char>(i,j+1) == 255 &&
                        img.at<u_char>(i,j-1) == 255 &&
                        img.at<u_char>(i-1,j-1) == 0 &&
                        img.at<u_char>(i-1,j+1) == 0 &&
                        img.at<u_char>(i+1,j+1) == 0 &&
                        img.at<u_char>(i+1,j-1) == 0)||
                            (img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i-1,j) == 255 &&
                             img.at<u_char>(i,j+1) == 255 &&
                             img.at<u_char>(i,j-1) == 255 &&
                             img.at<u_char>(i+1,j) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SO:
                    if((img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i-1,j-1) == 255 &&
                        img.at<u_char>(i-1,j+1) == 255 &&
                        img.at<u_char>(i+1,j+1) == 255 &&
                        img.at<u_char>(i-1,j) == 0 &&
                        img.at<u_char>(i,j-1) == 0 &&
                        img.at<u_char>(i,j+1) == 0 &&
                        img.at<u_char>(i+1,j) == 0)||
                            (img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i-1,j-1) == 255 &&
                             img.at<u_char>(i-1,j+1) == 255 &&
                             img.at<u_char>(i+1,j+1) == 255 &&
                             img.at<u_char>(i+1,j-1) == 0 )){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case O:
                    if((img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i-1,j) == 255 &&
                        img.at<u_char>(i,j+1) == 255 &&
                        img.at<u_char>(i+1,j) == 255 &&
                        img.at<u_char>(i-1,j-1) == 0 &&
                        img.at<u_char>(i-1,j+1) == 0 &&
                        img.at<u_char>(i+1,j+1) == 0 &&
                        img.at<u_char>(i+1,j-1) == 0)||
                            (img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i-1,j) == 255 &&
                             img.at<u_char>(i,j+1) == 255 &&
                             img.at<u_char>(i+1,j) == 255 &&
                             img.at<u_char>(i,j-1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NO:
                    if((img.at<u_char>(i,j) == 255 &&
                        img.at<u_char>(i-1,j+1) == 255 &&
                        img.at<u_char>(i+1,j+1) == 255 &&
                        img.at<u_char>(i+1,j-1) == 255 &&
                        img.at<u_char>(i-1,j) == 0 &&
                        img.at<u_char>(i,j-1) == 0 &&
                        img.at<u_char>(i,j+1) == 0 &&
                        img.at<u_char>(i+1,j) == 0)||
                            (img.at<u_char>(i,j) == 255 &&
                             img.at<u_char>(i-1,j+1) == 255 &&
                             img.at<u_char>(i+1,j+1) == 255 &&
                             img.at<u_char>(i+1,j-1) == 255 &&
                             img.at<u_char>(i-1,j-1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                default:
                    break;
                }
            }
    }
    return vect_pt;
}

void Processing::convexityDefects(std::vector<Point>& contour, std::vector<int>& hull, std::vector<Point>& convexDefects){
    if(hull.size() > 0 && contour.size() > 0){
    CvSeq* contourPoints;
    CvSeq* defects;
    CvMemStorage* storage;
    CvMemStorage* strDefects;
    CvMemStorage* contourStr;
    CvConvexityDefect *defectArray = 0;

    strDefects = cvCreateMemStorage();
    defects = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq),sizeof(CvPoint), strDefects );

    //We transform our vector<Point> into a CvSeq* object of CvPoint.
    contourStr = cvCreateMemStorage();
    contourPoints = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), contourStr);
    for(int i=0; i<(int)contour.size(); i++) {
        CvPoint cp = {contour[i].x,  contour[i].y};
        cvSeqPush(contourPoints, &cp);
    }

    //Now, we do the same thing with the hull index
    int count = (int)hull.size();
    //int hullK[count];
    int* hullK = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++){hullK[i] = hull.at(i);}
    CvMat hullMat = cvMat(1, count, CV_32SC1, hullK);

    //We calculate convexity defects
    storage = cvCreateMemStorage(0);
    defects = cvConvexityDefects(contourPoints, &hullMat, storage);
    defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*defects->total);
    cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
    //printf("DefectArray %i %i\n",defectArray->end->x, defectArray->end->y);

    //We store defects points in the convexDefects parameter.
    for(int i = 0; i<defects->total; i++){
        CvPoint ptf;
        ptf.x = defectArray[i].depth_point->x;
        ptf.y = defectArray[i].depth_point->y;
        convexDefects.push_back(ptf);
    }

    //We release memory
    cvReleaseMemStorage(&contourStr);
    cvReleaseMemStorage(&strDefects);
    cvReleaseMemStorage(&storage);
    }
}

void Processing::getConvexHullMat(Mat img)
{
    std::vector<std::vector<Point2i> > contours;


    // Find contours
    findContours(img, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // Find the convex hull object for each contour
    std::vector<std::vector<Point2i> > hullP(contours.size());
    std::vector<std::vector<int> > hullI(contours.size());
    std::vector<std::vector<Point2i> > defects(contours.size());
    std::vector<std::vector<Point2i> > test;
    for( int i = 0; i < contours.size(); i++ )
    {
        std::vector<Point2i> tmp;


        convexHull(Mat(contours[i]), hullP[i], false);
        convexHull(Mat(contours[i]), hullI[i], false);
        convexityDefects(contours[i],hullI[i],defects[i]);
        for(int j =0;j<contours[i].size();j++)
        {
            tmp.push_back(hullP[i][j]);
            tmp.push_back(defects[i][j]);
        }

        test.push_back(tmp);
    }

    // Find the convex defects


    // Draw contours + hull results
    Mat drawing = Mat::zeros(img.size(), CV_8UC3 );
    drawContours( drawing, contours, -1, Scalar_<u_char>(255,0,0), 1, 8, vector<Vec4i>(), 0, Point() );
    drawContours( drawing, hullP, -1, Scalar_<u_char>(0,0,255), 1, 8, vector<Vec4i>(), 0, Point() );



    drawContours( drawing, test, -1, Scalar_<u_char>(0,255,0), 1, 8, vector<Vec4i>(), 0, Point() );

    imshow("contours",drawing);


}
