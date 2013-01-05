#ifndef INWINDOW_H
#define INWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QSlider>
#include <QFileDialog>
#include <opencv2/opencv.hpp>

#include "histogram1d.h"
#include "preprocessing.h"
#include "processing.h"
#include "display.h"
#include "recognition.h"
#include "image.h"

#include "ui_mainwindow.h"

using namespace cv;

class INWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit INWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void updateSeuil(int);
    void openDialog();

private:
    QToolBar * m_toolBar;
    QSlider * m_seuil_slide;
    Ui::MainWindow * ui;
    Image * m_img_init;
    Image * m_img_thres;
    Image * m_img_hist;
    Image * m_img_final;
    Image * m_img_skel;
    Image * m_img_hull;

    Mat m_mat_init;
    Mat m_mat_thres;
    Mat m_mat_inv;
    Mat m_mat_hist;
    Mat m_mat_skel;

    std::vector<Point2i> m_vec_extrem;
    std::vector<Point2i> m_vec_multi;
    std::vector<int> m_extract_coords;
    std::vector<std::vector<Point2i> > m_hull;
    Point2i m_center;

    Histogram1D *  h;
    int m_seuil;
    Recognition::STATE m_state;

    void compute();
    void drawPoints();
    void drawConvexhull();
    void drawAllRecognition();
};

#endif // INWINDOW_H
