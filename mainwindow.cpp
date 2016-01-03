#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<cstdio>

using namespace std;

void setSpinBoxValue(QDoubleSpinBox* spn,QLineEdit* txtMin,QLineEdit* txtMax,int value)
{

    spn->setMinimum(txtMin->text().toDouble());
    spn->setMaximum(txtMax->text().toDouble());

    double minValue = txtMin->text().toDouble();
    double maxValue = txtMax->text().toDouble();
    double actualValue = ((maxValue-minValue)/10000.0)*value + minValue;
    spn->setValue(actualValue);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    slamInitialized = false;

    server_socket = new Socket();
    ///--storing data
    localPosData = fopen("localPosData.csv", "w");  // using absolute path name of file

    if (localPosData == NULL)
        fprintf(stderr, "Unable to open file.\n");

    fprintf(localPosData,"Pose_00,Pose_01,Pose_02,Pose_03,Pose_10,Pose_11,Pose_12,Pose_13,Pose_20,Pose_21,Pose_22,Pose23,Pose_30,Pose_31,Pose_32,Pose_33\n ");

    ui->setupUi(this);
    ui->myGLWidget->test_server = server_socket;
    setSpinBoxValue(ui->spnPitch,ui->txtPitchMin,ui->txtPitchMax,ui->sldrPitch->value());
    setSpinBoxValue(ui->spnRoll,ui->txtRollMin,ui->txtRollMax,ui->sldrRoll->value());
    setSpinBoxValue(ui->spnYaw,ui->txtYawMin,ui->txtYawMax,ui->sldrYaw->value());
    setSpinBoxValue(ui->spnThrust,ui->txtThrustMin,ui->txtThrustMax,ui->sldrThrust->value());

    /*ui->spnRoll->setMinimum(ui->txtRollMin->text().toDouble());
    ui->spnRoll->setMaximum(ui->txtRollMax->text().toDouble());
    ui->spnYaw->setMinimum(ui->txtYawMin->text().toDouble());
    ui->spnYaw->setMaximum(ui->txtYawMax->text().toDouble());
    ui->spnThrust->setMinimum(ui->txtThrustMin->text().toDouble());
    ui->spnThrust->setMaximum(ui->txtThrustMax->text().toDouble());
*/
     //  connect(ui->btnArm,SIGNAL(released()),this,SLOT(on_sldrRoll_sliderMoved(int)));
    //connect(ui->myGLWidget,SIGNAL(statusChanged(string)),this,SLOT()

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sliderMoved(QLineEdit*txtMin,QLineEdit*txtMax,QDoubleSpinBox*spn ,int value,string tag)
{
    char buff[30];
    float minValue = txtMin->text().toFloat();
    float maxValue = txtMax->text().toFloat();
    float actualValue = ((maxValue-minValue)/10000.0)*value + minValue;
    tag+="%-10f";
    sprintf(buff,tag.c_str(),actualValue) ;
    cout<<buff<<endl;
    fflush(stdout);
    setSpinBoxValue(spn,txtMin,txtMax,value);

    server_socket->send_data (buff, strlen(buff));
}

void MainWindow::on_sldrRoll_sliderMoved(int value)
{
   sliderMoved(ui->txtRollMin,ui->txtRollMax,ui->spnRoll,value,"rl");
}

void MainWindow::on_sldrPitch_sliderMoved(int value)
{
   sliderMoved(ui->txtPitchMin,ui->txtPitchMax,ui->spnPitch,value,"pt");
}

void MainWindow::on_sldrYaw_sliderMoved(int value)
{
   sliderMoved(ui->txtYawMin,ui->txtYawMax,ui->spnYaw,value,"yw");
}

void MainWindow::on_sldrThrust_sliderMoved(int value)
{
   sliderMoved(ui->txtThrustMin,ui->txtThrustMax,ui->spnThrust,value,"tr");
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

        slamInitialized = true;
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

void MainWindow::on_btnHover_released()
{
    on_spnP_valueChanged(ui->spnP->value());
    usleep(10000);
    on_spnI_valueChanged(ui->spnI->value());
    usleep(10000);
    on_spnD_valueChanged(ui->spnD->value());
    usleep(10000);
    server_socket->send_data ((char*)"HOVER", 5);
}


void MainWindow::on_spnP_valueChanged(double value)
{
    char buff[30];
    sprintf(buff,"GP%lf",value) ;
    server_socket->send_data (buff, strlen(buff));
}

void MainWindow::on_spnI_valueChanged(double value)
{
    char buff[30];
    sprintf(buff,"GI%lf",value) ;
    server_socket->send_data (buff, strlen(buff));
}

void MainWindow::on_spnD_valueChanged(double value)
{
    char buff[30];
    sprintf(buff,"GD%lf",value) ;
    server_socket->send_data (buff, strlen(buff));
}



void MainWindow::on_myGLWidget_statusChanged(string status)
{
    //cout<<status<<endl;
    //fflush(stdout);

    ui->lblStatus->setText(status.c_str());
}

void MainWindow::on_myGLWidget_modeChanged(string status)
{

    ui->lblMode->setText(status.c_str());
}

void MainWindow::on_myGLWidget_writeDataToFile(string data)
{
    if(slamInitialized)
    fprintf(localPosData,(const char*)data.c_str());
}
bool MainWindow::getSLAMInit()
{
    return slamInitialized;
}
