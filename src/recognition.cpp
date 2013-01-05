#include "recognition.h"

Recognition::Recognition()
{
}
Recognition::STATE Recognition::getState(std::vector<Point2i> defects)
{
    //DISTANCE EUCLIDIENNE
    /*int nb = 0;
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {

        double dist = sqrt(pow((*it).x-center.x,2)+pow((*it).y-center.y,2));
        if(dist > 40.0)
            nb++;
    }*/
    int nb = defects.size()-1;
    if(nb < 0)
        nb = 0;
    switch(nb)
    {
    case 0:
        return HAND_0;
        break;
    case 1:
        return HAND_1;
        break;
    case 2:
        return HAND_2;
        break;
    case 3:
        return HAND_3;
        break;
    case 4:
        return HAND_4;
        break;
    case 5:
        return HAND_5;
        break;
    default:
        return ALIEN_HAND;
        break;
    }
}

Recognition::STATE Recognition::getRecognition(std::vector<Point2i> vect_extrem, std::vector<Point2i> vect_mutli, Point2i center,std::vector<std::vector<Point2i> >  convexhull)
{
    STATE state;
    std::vector<Point2i> hullP = convexhull[0];
    std::vector<Point2i> defects = convexhull[1];
    state = getState(defects);
    return state;
}


