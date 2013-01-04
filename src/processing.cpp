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
    Mat dst=Mat(img.rows,img.cols,CV_8UC1,Scalar_<u_char>(150));

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
}
