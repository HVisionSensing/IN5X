#include "display.h"

display::display()
{
}

void display::drawPoints(Mat img, Point2i center, std::vector<Point2i> vect_extrem,
                         std::vector<Point2i> vect_multi, std::vector<int> coords)
{
    Mat tmp;
    cvtColor(img,tmp,CV_GRAY2RGB);
    Point2i l = center;
    circle(tmp,l,1,Scalar_<u_char>(0,0,255));
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {
        Point2i pt = (*it);
        circle(tmp,pt,1,Scalar_<u_char>(255,0,0));
        line(tmp,pt,l,Scalar_<u_char>(255,255,0));
    }
    std::vector<Point2i>::iterator it1 = vect_multi.begin();
    for(;it1 != vect_multi.end();it1++)
    {
        Point2i pt = (*it1);
        circle(tmp,pt,1,Scalar_<u_char>(0,255,0));
    }

    imshow("Image des points caracteristique",tmp);
}

void display::drawConvexhull(Mat img,std::vector<std::vector<Point2i> > hullDefects)
{
    std::vector<Point2i> hullP = hullDefects[0];
    std::vector<Point2i> defects = hullDefects[1];
    int nb = 0;

    // Draw contours + hull results
    char txt[]="0";
    txt[0]='0'+nb-1;

    Mat tmp;
    cvtColor(img,tmp,CV_GRAY2RGB);

    for(int i = 0; i < defects.size() ; i++)
    {
        /*line(tmp,defects[i],defects[i+1],Scalar_<u_char>(255,0,0));
        if((i+1) == defects.size()-1)
            line(tmp,defects[i+1],defects[0],Scalar_<u_char>(255,0,0));*/
        circle(tmp,defects[i],3,Scalar_<u_char>(255,0,0));
    }
    for(int i = 0; i < hullP.size()-1 ; i++)
    {
        line(tmp,hullP[i],hullP[i+1],Scalar_<u_char>(255,255,0));
        if((i+1) == hullP.size()-1)
            line(tmp,hullP[i+1],hullP[0],Scalar_<u_char>(255,255,0));
    }
    putText(tmp, txt, Point(50, 50),FONT_HERSHEY_SIMPLEX,1.0, Scalar_<u_char>(255, 0, 255));

    imshow("Convex hull + defects",tmp);
}
void display::drawAllRecognition(Mat img, Point2i center, std::vector<Point2i> vect_extrem, std::vector<Point2i> vect_multi, std::vector<std::vector<Point2i> > hullDefects,Recognition::STATE state)
{
    Mat tmp;
    std::vector<Point2i> hullP = hullDefects[0];
    std::vector<Point2i> defects = hullDefects[1];
    int nb = defects.size();

    cvtColor(img,tmp,CV_GRAY2RGB);
    Point2i l = center;
    circle(tmp,l,1,Scalar_<u_char>(0,0,255));
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {
        Point2i pt = (*it);
        circle(tmp,pt,1,Scalar_<u_char>(255,0,0));
        line(tmp,pt,l,Scalar_<u_char>(0,255,255));
    }
    std::vector<Point2i>::iterator it1 = vect_multi.begin();
    for(;it1 != vect_multi.end();it1++)
    {
        Point2i pt = (*it1);
        circle(tmp,pt,1,Scalar_<u_char>(0,255,0));
    }
    for(int i = 0; i < defects.size() ; i++)
    {
        /*line(tmp,defects[i],defects[i+1],Scalar_<u_char>(255,0,0));
        if((i+1) == defects.size()-1)
            line(tmp,defects[i+1],defects[0],Scalar_<u_char>(255,0,0));*/
        circle(tmp,defects[i],3,Scalar_<u_char>(255,0,0));
    }
    for(int i = 0; i < hullP.size()-1 ; i++)
    {
        line(tmp,hullP[i],hullP[i+1],Scalar_<u_char>(255,255,0));
        if((i+1) == hullP.size()-1)
            line(tmp,hullP[i+1],hullP[0],Scalar_<u_char>(255,255,0));
    }

    std::string text;

    switch(state)
    {
    case Recognition::HAND_0:
        text = " the hand is closed";
        break;
    case Recognition::HAND_1:
        text = " 1 finger is detected";
        break;
    case Recognition::HAND_2:
        text = " 2 fingers are detected";
        break;
    case Recognition::HAND_3:
        text = " 3 fingers are detected";
        break;
    case Recognition::HAND_4:
        text = " 4 fingers are detected";
        break;
    case Recognition::HAND_5:
        text = " 5 fingers are detected";
        break;
    default:
        text = " Alien Hand is detected";
        break;
    }

    putText(tmp, text, Point(0, 20),FONT_HERSHEY_SIMPLEX,0.5, Scalar_<u_char>(255, 0, 255));

    imshow("Convex hull + defects",tmp);
}
