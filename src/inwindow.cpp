#include "inwindow.h"

INWindow::INWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui = new Ui::MainWindow();
    this->ui->setupUi(this);

    m_seuil_slide = new QSlider(Qt::Horizontal);
    m_seuil_slide->setRange(0,255);

    /****************************** Tool Bar *****************************************************/
    m_toolBar = new QToolBar("Tool bar");

    m_toolBar->addWidget(m_seuil_slide);
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);
    this->addToolBar(m_toolBar);
    QObject::connect(m_seuil_slide,SIGNAL(valueChanged(int)),this,SLOT(updateSeuil(int)));
    QObject::connect(ui->actionOpen_Image,SIGNAL(triggered()),this,SLOT(openDialog()));

    m_img_init = new Image();
    m_img_thres = new Image();
    m_img_hist = new Image();
    m_img_final = new Image();
    m_img_skel = new Image();
    m_img_hull = new Image();

    m_img_init->setFixedSize(200,200);
    m_img_thres->setFixedSize(200,200);
    m_img_final->setFixedSize(200,200);
    m_img_hist->setFixedSize(200,200);
    m_img_skel->setFixedSize(200,200);
    m_img_hull->setFixedSize(200,200);

    m_img_init->show();
    m_img_thres->show();
    m_img_hist->show();
    m_img_final->show();
    m_img_skel->show();
    m_img_hull->show();

    ui->centralLayout->addWidget(m_img_thres,0,2);
    ui->centralLayout->addWidget(m_img_init,0,0);
    ui->centralLayout->addWidget(m_img_hist,0,1);
    ui->centralLayout->addWidget(m_img_skel,1,0);
    ui->centralLayout->addWidget(m_img_final,1,2);
    ui->centralLayout->addWidget(m_img_hull,1,1);
}

void INWindow::openDialog()
{
    QString filename = QFileDialog::getOpenFileName(this," Open image :","./","*.yml");
    FileStorage fs(filename.toStdString(), FileStorage::READ);
    ui->centralLayout->removeWidget(m_img_init);
    Mat temp;

    fs["M1"] >> temp;

    m_mat_init = PreProcessing::getUCHARImage(temp,5.0);
    PreProcessing::getExpansion(m_mat_init);
    PreProcessing::getMedianFilter3(m_mat_init);

    m_img_init->setImage(m_mat_init);

    h = new Histogram1D(m_mat_init);
    m_mat_hist =  h->getHistogramImage();
    h->cumulHist();
    h->derivCumul();
    m_img_hist->setImage(m_mat_hist);
    compute();
}
void INWindow::updateSeuil(int val)
{
    m_seuil = val;
    compute();
}

void INWindow::compute()
{
    Mat thresholded = Processing::threshold(m_mat_init,m_seuil);
    Mat blur=PreProcessing::getMedianBlur(thresholded,8);

    Mat inv = Processing::getInverse(blur);
    m_img_thres->setImage(inv);

    m_mat_skel = Processing::getThinning(inv,Processing::CONNEXITY_8,50);
    m_hull = Processing::getConvexHullMat(inv);

    Mat distance = Processing::getDistanceTransform(inv);
    Mat squeletteUChar = PreProcessing::getUCHARImage(distance,1.0);
    PreProcessing::getExpansion(squeletteUChar);
    m_center = Processing::getDistanceMax(squeletteUChar);
    m_vec_extrem = Processing::getSkelExtremity(m_mat_skel);
    m_vec_multi = Processing::getMultiPoints(m_mat_skel);

    if(!m_hull.empty())
        m_state = Recognition::getRecognition(m_vec_extrem,m_vec_multi,m_center,m_hull);

    drawPoints();
    drawConvexhull();
    drawAllRecognition();
}

void INWindow::drawPoints()
{
    if(!m_vec_extrem.empty() || !m_vec_multi.empty())
    {
        Mat tmp;
        cvtColor(m_mat_init,tmp,CV_GRAY2RGB);
        for(int i = 0; i < m_mat_skel.rows; i++)
            for(int j = 0 ; j < m_mat_skel.cols ; j++)
            {
                if(m_mat_skel.at<u_char>(i,j) == 255)
                    tmp.at<Vec3b>(i,j) = Vec3b(255,255,255);
            }
        Point2i l = m_center;
        circle(tmp,l,1,Scalar_<u_char>(0,0,255));
        std::vector<Point2i>::iterator it = m_vec_extrem.begin();
        for(;it != m_vec_extrem.end();it++)
        {
            Point2i pt = (*it);
            circle(tmp,pt,1,Scalar_<u_char>(255,0,0));
            line(tmp,pt,l,Scalar_<u_char>(0,0,255));
        }
        std::vector<Point2i>::iterator it1 = m_vec_multi.begin();
        for(;it1 != m_vec_multi.end();it1++)
        {
            Point2i pt = (*it1);
            circle(tmp,pt,1,Scalar_<u_char>(0,255,0));
        }
        m_img_skel->setImage(tmp);
    }
}

void INWindow::drawConvexhull()
{
    if(!m_hull.empty())
    {
        std::vector<Point2i> hullP = m_hull[0];
        std::vector<Point2i> defects = m_hull[1];

        Mat tmp;
        cvtColor(m_mat_init,tmp,CV_GRAY2RGB);

        for(int i = 0; i < defects.size() ; i++)
        {
            circle(tmp,defects[i],3,Scalar_<u_char>(255,0,0));
        }
        for(int i = 0; i < hullP.size()-1 ; i++)
        {
            line(tmp,hullP[i],hullP[i+1],Scalar_<u_char>(255,255,0));
            if((i+1) == hullP.size()-1)
                line(tmp,hullP[i+1],hullP[0],Scalar_<u_char>(255,255,0));
        }

        m_img_hull->setImage(tmp);
    }

}
void INWindow::drawAllRecognition()
{

    if(!m_hull.empty() || !m_vec_extrem.empty() || !m_vec_multi.empty())
    {
        Mat tmp;
        std::vector<Point2i> hullP = m_hull[0];
        std::vector<Point2i> defects = m_hull[1];
        int nb = defects.size();

        cvtColor(m_mat_init,tmp,CV_GRAY2RGB);
        Point2i l = m_center;
        circle(tmp,l,1,Scalar_<u_char>(0,0,255));
        std::vector<Point2i>::iterator it = m_vec_extrem.begin();
        for(;it != m_vec_extrem.end();it++)
        {
            Point2i pt = (*it);
            circle(tmp,pt,1,Scalar_<u_char>(255,0,0));
            line(tmp,pt,l,Scalar_<u_char>(0,255,255));
        }
        std::vector<Point2i>::iterator it1 = m_vec_multi.begin();
        for(;it1 != m_vec_multi.end();it1++)
        {
            Point2i pt = (*it1);
            circle(tmp,pt,1,Scalar_<u_char>(0,255,0));
        }
        for(int i = 0; i < defects.size() ; i++)
        {
            circle(tmp,defects[i],3,Scalar_<u_char>(255,0,0));
        }
        for(int i = 0; i < hullP.size()-1 ; i++)
        {
            line(tmp,hullP[i],hullP[i+1],Scalar_<u_char>(255,255,0));
            if((i+1) == hullP.size()-1)
                line(tmp,hullP[i+1],hullP[0],Scalar_<u_char>(255,255,0));
        }

        std::string text;

        switch(m_state)
        {
        case Recognition::HAND_0:
            text = " The hand is closed";
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

        m_img_final->setImage(tmp);
    }
}
