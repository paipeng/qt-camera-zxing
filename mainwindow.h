#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "cpcamera.h"
#include "cpbarcode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, CPCameraInterface
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void initCameras();
    void startCamera1();
    void startCamera2();
    const QCameraInfo getSelectedCameraInfo(int source);
    void camera1Changed(int index);
    void camera2Changed(int index);
    void updateBarcodeDecodeResult(int decodeState);

private:
    void cameraState(int cameraId, int state) override;
    void processCapturedImage(int cameraId, const QImage& img) override;
    void cameraReadyForCapture(int cameraId, bool ready) override;


private:
    void displayViewfinder(int cameraId);
    void displayCapturedImage(int cameraId);

private:
    Ui::MainWindow *ui;

    CPCamera camera1;
    CPCamera camera2;
    bool camera1AutoCapture;
    bool camera2AutoCapture;

    CPBarcode barcode;

    QElapsedTimer timer;
};
#endif // MAINWINDOW_H
