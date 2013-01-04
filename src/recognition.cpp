#include "recognition.h"

Recognition::Recognition()
{
}

void Recognition::displayRecognition(STATE state)
{
}

Recognition::STATE Recognition::getNumberFingers(std::vector<Point2i> vect_extrem, Point2i center)
{
    //DISTANCE EUCLIDIENNE
    int nb = 0;
    std::vector<Point2i>::iterator it = vect_extrem.begin();
    for(;it != vect_extrem.end();it++)
    {

        double dist = sqrt(pow((*it).x-center.x,2)+pow((*it).y-center.y,2));
        if(dist > 40.0)
            nb++;
    }
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

Recognition::STATE Recognition::getRecognition(std::vector<Point2i> vect_extrem, Point2i center)
{
    STATE state = getNumberFingers(vect_extrem,center);
    displayRecognition(state);
    return state;
}


