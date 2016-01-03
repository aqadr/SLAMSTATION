#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <pthread.h>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/LU>

#include <GL/glu.h>
#include "Socket.h"

using namespace std;
using namespace Eigen;

#ifndef Pi
#define Pi 3.1415926535897932384626433832795
#endif
#define sqrt2 1.41421356237


class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



public slots:
    void on_timer_timeout();

signals:

    void statusChanged(string status);
    void modeChanged(string status);
    void writeDataToFile(string data);

private:

    QTimer *timer;
    void draw();


    QPoint lastPos;

    int     buttonState;
    float   camera_trans[3];
    float   camera_rot[3];
    float   camera_trans_lag[3];
    float   camera_rot_lag[3];
    float inertia;
    std::vector<Matrix<double,3,1> > PositionVec;


    Matrix4d CameraToWorldMatrix;

    //Quad Drawing functions
    GLUquadric *quadricObj;
    float propAngle,propSpeed;
    void drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC);
    void DrawTorus( double innerRadius, double outerRadius, GLint sides, GLint rings );
    void drawBlade();
    void drawPropellers(int rotorDirection);
    void drawQuad();





    void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);



    /**----drawing the reference lines----**/
    void DrawReferenceLines();
    /**decoding the incoming pose data and store it in the pose_matrix**/
    void command_decode(char* ptr);
    /**decoding the incoming pose data and store it in the pose_matrix**/
    void pose_decode(char* ptr, Matrix4d &pose_matrix);

public:

    Socket*test_server;
 //   MainWindow *mainWindow;

};

#endif // MYGLWIDGET_H
