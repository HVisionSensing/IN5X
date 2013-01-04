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
<<<<<<< HEAD
    Mat dst=Mat(img.rows,img.cols,CV_8UC1,Scalar_<u_char>(150));

    int nbPixel = 0;
    int nbPixelModif = 0;
=======
    Mat laplace,tempSkeleton;
    Laplacian(dst, laplace, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    compare(laplace,-3,tempSkeleton,CV_CMP_LE);

    Mat skeleton = tempSkeleton;
>>>>>>> b13a3297701ebdf3b8bba67f242162dbe392d852

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

<<<<<<< HEAD
    /* if(pathCode == CONNEXITY_4)
=======
    for(int i=0; i<img.rows;i++)
>>>>>>> b13a3297701ebdf3b8bba67f242162dbe392d852
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
<<<<<<< HEAD
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

Mat Processing::getStructElem(int code, ELEM_STATE state)
{
    float data[9];
    switch(state)
    {
    case N:
        data[0] = 1;
        data[1] = 1;
        data[2] = 1;

        data[3] = 0;
        data[4] = 1;
        data[5] = 0;

        data[6] = 0.5;
        data[7] = 0.5;
        data[8] = 0.5;
        break;
    case NE:
        data[0] = 0;
        data[1] = 1;
        data[2] = 1;

        data[3] = 0.5;
        data[4] = 1;
        data[5] = 1;

        data[6] = 0.5;
        data[7] = 0.5;
        data[8] = 0;
        break;
    case E:
        data[0] = 0.5;
        data[1] = 0;
        data[2] = 1;

        data[3] = 0.5;
        data[4] = 1;
        data[5] = 1;

        data[6] = 0.5;
        data[7] = 0;
        data[8] = 1;
        break;
    case SE:
        data[0] = 0.5;
        data[1] = 0.5;
        data[2] = 0;

        data[3] = 0.5;
        data[4] = 1;
        data[5] = 1;

        data[6] = 0;
        data[7] = 1;
        data[8] = 1;
        break;
    case S:
        data[0] = 0.5;
        data[1] = 0.5;
        data[2] = 0.5;

        data[3] = 0;
        data[4] = 1;
        data[5] = 0;

        data[6] = 1;
        data[7] = 1;
        data[8] = 1;
        break;
    case SO:
        data[0] = 0;
        data[1] = 0.5;
        data[2] = 0.5;

        data[3] = 1;
        data[4] = 1;
        data[5] = 0.5;

        data[6] = 1;
        data[7] = 1;
        data[8] = 0;
        break;
    case O:
        data[0] = 1;
        data[1] = 0;
        data[2] = 0.5;

        data[3] = 1;
        data[4] = 1;
        data[5] = 0.5;

        data[6] = 1;
        data[7] = 0;
        data[8] = 0.5;
        break;
    case NO:
        data[0] = 1;
        data[1] = 1;
        data[2] = 0;

        data[3] = 1;
        data[4] = 1;
        data[5] = 0.5;

        data[6] = 0;
        data[7] = 0.5;
        data[8] = 0.5;
        break;
    default:
        break;
    }

    Mat elem = Mat(3,3,CV_32FC1,&data);
    return elem.clone();

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
    std::vector<Point2i> vec_pt;

    Point2i pt;
    for(int i = 0; i < img.rows; i++)
        for(int j=0; j < img.cols; j++)
        {
            if(img.at<u_char>(i,j) == 255)
            {
                int nb = 0;
                if(img.at<u_char>(i-1,j-1) == 255)nb++;
                if(img.at<u_char>(i-1,j) == 255)nb++;
                if(img.at<u_char>(i-1,j+1) == 255)nb++;

                if(img.at<u_char>(i,j-1) == 255)nb++;
                if(img.at<u_char>(i,j+1) == 255)nb++;

                if(img.at<u_char>(i+1,j-1) == 255)nb++;
                if(img.at<u_char>(i+1,j) == 255)nb++;
                if(img.at<u_char>(i+1,j+1) == 255)nb++;

                if(nb >= 4)
                {
                    pt = Point2i(j,i);
                    vec_pt.push_back(pt);
                }
            }
        }
    return vec_pt;
=======
    int width=xR-xL;
    int height=yD-yU;

    Rect rect = Rect(xL,yU,width,height);
    Mat extracted = img(rect);

    return extracted;

>>>>>>> b13a3297701ebdf3b8bba67f242162dbe392d852
}
