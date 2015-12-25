#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<cstdio>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    server_socket = new Socket();
    ui->setupUi(this);
  //  connect(ui->btnArm,SIGNAL(released()),this,SLOT(on_sldrRoll_sliderMoved(int)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sliderMoved(QLineEdit*txtMin,QLineEdit*txtMax,int value,string tag)
{
    char buff[30];
    float minValue = txtMin->text().toFloat();
    float maxValue = txtMax->text().toFloat();
    float actualValue = ((maxValue-minValue)/10000.0)*value + minValue;
    tag+="%-10f";
    sprintf(buff,tag.c_str(),actualValue) ;
    cout<<buff<<endl;
    server_socket->send_data (buff, strlen(buff));
}

void MainWindow::on_sldrRoll_sliderMoved(int value)
{
   sliderMoved(ui->txtRollMin,ui->txtRollMax,value,"rl");
}

void MainWindow::on_sldrPitch_sliderMoved(int value)
{
   sliderMoved(ui->txtPitchMin,ui->txtPitchMax,value,"pt");
}

void MainWindow::on_sldrYaw_sliderMoved(int value)
{
   sliderMoved(ui->txtYawMin,ui->txtYawMax,value,"yw");
}

void MainWindow::on_sldrThrust_sliderMoved(int value)
{
   sliderMoved(ui->txtThrustMin,ui->txtThrustMax,value,"tr");
}

void MainWindow::on_btnArm_released()
{
    server_socket->send_data((char*)"ARM", 3);
}

void MainWindow::on_btnTakeoff_released()
{
    //char *send="TAKEOFF";
    if(ui->btnTakeoff->text().toStdString() == "Takeoff")
    {
        server_socket->send_data ((char*)"TAKEOFF", 7);
        ui->btnTakeoff->setText("Land");
    }
    else
    {
        server_socket->send_data ((char*)"LAND", 4);
        ui->btnTakeoff->setText("Takeoff");
    }
    usleep(10000);
}

void MainWindow::on_btnHome_released()
{
    server_socket->send_data ((char*)"HOME", 4);
}

void MainWindow::on_btnStart_released()
{
    server_socket->send_data ((char*)"START", 5);
}

void MainWindow::on_btnStop_released()
{
    server_socket->send_data ((char*)"STOP", 4);
}

void MainWindow::on_btnInit_released()
{
    if(ui->btnInit->text().toStdString() == "Init SLAM")
    {
        server_socket->send_data ((char*)"GO", 2);
        ui->btnInit->setText("Onboard_control_ON");
        ui->lblStatus->setText("initialized");
    }
    else if(ui->btnInit->text().toStdString() == "Onboard_control_ON")
    {
        server_socket->send_data ((char*)"ON", 2);
        ui->btnInit->setText("Onboard_control_OFF");
    }
    else if(ui->btnInit->text().toStdString() == "Onboard_control_OFF")
    {
        server_socket->send_data ((char*)"OFF", 3);
        ui->btnInit->setText("Onboard_control_ON");
    }

}
