#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), camera1(0, this), camera2(1, this), camera1AutoCapture(true), resizeFactor(3)
{
    ui->setupUi(this);

    //ui->camera2ComboBox->setCurrentIndex(1);
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
            //ui->camera2StartButton->setText(tr("start"));
        }
        displayCapturedImage(cameraId);
    } else if (state == 1) {
        if (cameraId == 0) {
            ui->camera1StartButton->setText(tr("start"));
            camera1AutoCapture = true;
        } else {
            //ui->camera2StartButton->setText(tr("start"));
            camera2AutoCapture = true;
        }
    } else if (state == 2) {
        if (cameraId == 0) {
            ui->camera1StartButton->setText(tr("stop"));
        } else {
            //ui->camera2StartButton->setText(tr("stop"));
        }
        displayViewfinder(cameraId);
    }
#endif
}

void MainWindow::processCapturedImage(int cameraId, const QImage& img) {
    qDebug() << "processCapturedImage: " << cameraId << " img: " << img.width() << "-" << img.height() << " " << img.format();
    //img.save ("preview.bmp", "bmp");
    imageSize.setWidth(img.width());
    imageSize.setHeight(img.height());
    //QImage small = img.scaled(img.width()/resizeFactor, img.height()/resizeFactor);
    timer.start();
#if 1
    if (cameraId == 0) {
        barcode.setImage(img, resizeFactor);
    } else {
        //arcFaceEngine.setImage(img);
    }
#else
    camera1.takeImage();
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
        //ui->camera2ComboBox->addItem(cameraInfo.description(), index);
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
        //comboBox = ui->camera2ComboBox;
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
    //ui->camera2Label->setText(QString("Starting..."));
    //camera2.startCamera();

}

void MainWindow::camera1Changed(int index) {
    qDebug() << "camera1Changed: " << index;
    camera1.setCamera(getSelectedCameraInfo(0), ui->camera1Viewfinder);
}

void MainWindow::camera2Changed(int index) {
    qDebug() << "camera2Changed: " << index;
    //camera2.setCamera(getSelectedCameraInfo(1), ui->camera2Viewfinder);
}


void MainWindow::updateBarcodeDecodeResult(int decodeState) {
    Q_UNUSED(decodeState);
    qint64 t = timer.elapsed();
    qDebug() << "updateBarcodeDecodeResult: " << decodeState << "time: " << t;
#if 1
    if (decodeState == 0) {
        qDebug() << "RESULT: " << barcode.decodeResult.isValid();
#if 0
        for (auto&& result : barcode.decodeResults) {
            qDebug() << "RESULT: " << result.isValid();

            if (result.isValid()) {
                QString text = QString::fromWCharArray(result.text().c_str());
                qDebug() << "RESULT: " << text;
                qDebug() << "Position: " << result.position().size();
                for (auto&& point : result.position()) {
                    qDebug() << "Point: " << point.x << "-" << point.y;
                }
                qDebug() << "Status: " << (int)result.status();
                qDebug() << "Ecc: " << result.ecLevel().c_str();
                qDebug() << "Format: " << (int)result.format();
                ui->camera1Label->setText(text);
                break;
            }
        }
#else
        QString text = QString::fromWCharArray(barcode.decodeResult.text().c_str());
        qDebug() << "RESULT: " << text;

        qDebug() << "Position: " << barcode.decodeResult.position().size();
        for (auto&& point : barcode.decodeResult.position()) {
            qDebug() << "Point: " << point.x << "-" << point.y;
        }
        qDebug() << "Status: " << (int)barcode.decodeResult.status();
        qDebug() << "Ecc: " << QString::fromWCharArray(barcode.decodeResult.ecLevel().c_str());
        qDebug() << "Format: " << (int)barcode.decodeResult.format();
        ui->camera1Label->setText(text);

        QVector<QPoint> points;
        QSize viewFinderSize = ui->camera1Viewfinder->getSize();
        for (auto&& point : barcode.decodeResult.position()) {
            points.append(QPoint(point.x * resizeFactor* viewFinderSize.width()/imageSize.width() , point.y * resizeFactor* viewFinderSize.height()/imageSize.height() ));
        }
        ui->camera1Viewfinder->setText(text);
        ui->camera1Viewfinder->setPoints(points);


        QImage image = barcode.getImage();
        qDebug() << "image size: " << image.size();

        // crop
        /*
         *
         * QRect rect(10, 20, 30, 40);
            QPixmap original('image.png');
            QPixmap cropped = original.copy(rect);
         * */


        QVector<QPoint> cropPoints;
        for (auto&& point : barcode.decodeResult.position()) {
            cropPoints.append(QPoint(point.x * resizeFactor , point.y * resizeFactor));
        }

        QRect rect(cropPoints.at(0).x(), cropPoints.at(0).y(), cropPoints.at(1).x() - cropPoints.at(0).x(), cropPoints.at(2).y() - cropPoints.at(1).y());


        int crop_width = rect.width();
        int crop_height = rect.height();

        int offset_x = (23*crop_width/21 - crop_width)/2;
        int offset_y = (23*crop_height/21 - crop_height)/2;

        rect.setLeft(rect.left() - offset_x);
        rect.setTop(rect.top() - offset_y);

        rect.setWidth(23*crop_width/21);
        rect.setHeight(rect.width());//23*crop_height/21);


        qDebug() << "crop rect: " << rect;

        QImage cropped = image.copy(rect);

        QPixmap pixmap = QPixmap::fromImage(cropped);

        int w = ui->cropLabel->width();
        int h = ui->cropLabel->height();

        ui->cropLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

#endif
    } else {
        ui->camera1Label->setText(QString(""));
        QVector<QPoint> points;
        ui->camera1Viewfinder->setText(QString("No detected"));
        ui->camera1Viewfinder->setPoints(points);

    }
    camera1.takeImage();
#endif
}


void MainWindow::displayViewfinder(int cameraId) {
    if (cameraId == 0) {
        ui->camera1StackedWidget->setCurrentIndex(0);
    } else {
        //ui->camera2StackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::displayCapturedImage(int cameraId) {
    if (cameraId == 0) {
        ui->camera1StackedWidget->setCurrentIndex(1);
    } else {
        //ui->camera2StackedWidget->setCurrentIndex(1);
    }
}

