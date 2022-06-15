#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "cpcamera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, CPCameraInterface
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void cameraState(int cameraId, int state) override;
    void processCapturedImage(int cameraId, const QImage& img) override;
    void cameraReadyForCapture(int cameraId, bool ready) override;

private:
    Ui::MainWindow *ui;

    CPCamera camera1;
    //CPCamera camera2;
    bool camera1AutoCapture;
    //bool camera2AutoCapture;
};
#endif // MAINWINDOW_H
