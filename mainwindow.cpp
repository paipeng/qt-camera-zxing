#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), camera1(0, this)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::cameraState(int cameraId, int state) {
    qDebug() << "cameraState: " << cameraId << " state: " << state;
#if 0
    if (state == 0) {
        if (cameraId == 0) {
            ui->camera1StartButton->setText(tr("start"));
        } else {
            ui->camera2StartButton->setText(tr("start"));
        }
        displayCapturedImage(cameraId);
    } else if (state == 1) {
        if (cameraId == 0) {
            ui->camera1StartButton->setText(tr("start"));
            camera1AutoCapture = true;
        } else {
            ui->camera2StartButton->setText(tr("start"));
            camera2AutoCapture = true;
        }
    } else if (state == 2) {
        if (cameraId == 0) {
            ui->camera1StartButton->setText(tr("stop"));
        } else {
            ui->camera2StartButton->setText(tr("stop"));
        }
        displayViewfinder(cameraId);
    }
#endif
}

void MainWindow::processCapturedImage(int cameraId, const QImage& img) {
    //qDebug() << "processCapturedImage: " << cameraId << " img: " << img.width() << "-" << img.height();
#if 0
    timer.start();
    if (cameraId == 0) {
        barcodeDecoder.setImage(img);
    } else {
        arcFaceEngine.setImage(img);
    }
#endif
}


void MainWindow::cameraReadyForCapture(int cameraId, bool ready) {
    qDebug() << "cameraReadyForCapture: " << cameraId << " state: " << ready;
#if 0
    if (ready) {
        if (cameraId == 0) {
            camera1.takeImage();
        } else {
            if (camera2AutoCapture) {
                camera2AutoCapture = false;
                camera2.takeImage();
            }
        }
    }
#endif
}

