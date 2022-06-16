#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), camera1(0, this), camera2(1, this), camera1AutoCapture(true)
{
    ui->setupUi(this);

    ui->camera2ComboBox->setCurrentIndex(1);
    initCameras();
    ui->statusbar->showMessage(tr("app_info"));

    QObject::connect(&barcode, &CPBarcode::updateBarcodeDecodeResult, this, &MainWindow::updateBarcodeDecodeResult);
    barcode.start();
    barcode.setPriority(QThread::LowestPriority);
}


MainWindow::~MainWindow()
{
    camera1.stopCamera();
    barcode.stop();
    delete ui;
}



void MainWindow::cameraState(int cameraId, int state) {
    qDebug() << "cameraState: " << cameraId << " state: " << state;
#if 1
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
    qDebug() << "processCapturedImage: " << cameraId << " img: " << img.width() << "-" << img.height() << " " << img.format();
#if 0
    QImage image;
    bool success = image.load("/Users/paipeng/Downloads/zxing-enc.png");
    qDebug() << "File loaded succesfully " << success ;
#endif
    //img.save ("preview.bmp", "bmp");
    QImage small = img.scaled(img.width()/4, img.height()/4);
#if 1
    timer.start();
    if (cameraId == 0) {
        barcode.setImage(small);
    } else {
        //arcFaceEngine.setImage(img);
    }
#endif
}


void MainWindow::cameraReadyForCapture(int cameraId, bool ready) {
    qDebug() << "cameraReadyForCapture: " << cameraId << " state: " << ready;
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
}



void MainWindow::initCameras() {
    qDebug() << "initCameras";
    const QList<QCameraInfo> availableCameras = CPCamera::getAvailableCamersInfos();

    int index = 0;
    for (const QCameraInfo &cameraInfo : availableCameras) {
        ui->camera1ComboBox->addItem(cameraInfo.description(), index);
        ui->camera2ComboBox->addItem(cameraInfo.description(), index);
        index ++;
    }
}


const QCameraInfo MainWindow::getSelectedCameraInfo(int source) {
    qDebug() << "getSelectedCameraInfo: " << source;
    const QList<QCameraInfo> availableCameras = CPCamera::getAvailableCamersInfos();

    QComboBox* comboBox = NULL;
    if (source == 0) {
        comboBox = ui->camera1ComboBox;
    } else {
        comboBox = ui->camera2ComboBox;
    }

    int index = 0;
    for (const QCameraInfo &cameraInfo : availableCameras) {
        if (index== comboBox->currentIndex()) {
            qDebug() << "selected camera found: " << cameraInfo.description();
            return cameraInfo;
        }
        index++;
    }
    QCameraInfo defaultCameraInfo;
    return defaultCameraInfo;
}

void MainWindow::startCamera1() {
    qDebug() << "startCamera1";
    camera1.startCamera();
}

void MainWindow::startCamera2() {
    qDebug() << "startCamera2";
    ui->camera2Label->setText(QString("Starting..."));
    //camera2.startCamera();

}

void MainWindow::camera1Changed(int index) {
    qDebug() << "camera1Changed: " << index;
    camera1.setCamera(getSelectedCameraInfo(0), ui->camera1Viewfinder);
}

void MainWindow::camera2Changed(int index) {
    qDebug() << "camera2Changed: " << index;
    camera2.setCamera(getSelectedCameraInfo(1), ui->camera2Viewfinder);
}


void MainWindow::updateBarcodeDecodeResult(int decodeState) {
    Q_UNUSED(decodeState);
    qDebug() << "updateBarcodeDecodeResult: " << decodeState;
#if 1
    if (decodeState == 0) {
        for (auto&& result : barcode.decodeResults) {
            //qDebug() << "RESULT: " << result.text();
            QString text = QString::fromWCharArray(result.text().c_str());
            qDebug() << "RESULT: " << text;
            ui->camera1Label->setText(text);
        }
    } else {
        ui->camera1Label->setText(QString(""));
    }

    //camera1.takeImage();
#endif
}


void MainWindow::displayViewfinder(int cameraId) {
    if (cameraId == 0) {
        ui->camera1StackedWidget->setCurrentIndex(0);
    } else {
        ui->camera2StackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::displayCapturedImage(int cameraId) {
    if (cameraId == 0) {
        ui->camera1StackedWidget->setCurrentIndex(1);
    } else {
        ui->camera2StackedWidget->setCurrentIndex(1);
    }
}
