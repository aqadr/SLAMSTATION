#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Socket.h"
#include "ui_mainwindow.h"
#include <string>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    //sliders
    void on_sldrRoll_sliderMoved(int value);
    void on_sldrPitch_sliderMoved(int value);
    void on_sldrYaw_sliderMoved(int value);
    void on_sldrThrust_sliderMoved(int value);

    //buttons
    void on_btnArm_released();
    void on_btnTakeoff_released();
    void on_btnHome_released();
    void on_btnStart_released();
    void on_btnStop_released();
    void on_btnInit_released();

private:
    void sliderMoved(QLineEdit*txtMin,QLineEdit*txtMax,int value,string tag);

private:
    Ui::MainWindow *ui;
    Socket*server_socket;
};

#endif // MAINWINDOW_H
