
#include <QtWidgets>
#include <QtOpenGL>
#include "myglwidget.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <sys/ioctl.h>
#include <fcntl.h>              /* low-level i/o */


MyGLWidget::MyGLWidget(QWidget *parent)
 : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    buttonState         = 0;
    camera_trans[0]= 0,camera_trans[1]= -0.2,camera_trans[2]= -20;
    camera_rot[0]=0,camera_rot[1]=0,camera_rot[2]= 0;
    camera_trans_lag[0]=0, camera_trans_lag[1]=-0.2, camera_trans_lag[2]=-10;
    camera_rot_lag[0]=0, camera_rot_lag[1]=0, camera_rot_lag[2]=0;
    inertia = 0.1f;
    propAngle=0,propSpeed=2;

    timer = new QTimer(this);


    quadricObj = gluNewQuadric();

    CameraToWorldMatrix=Matrix4d::Identity();

    connect(timer,SIGNAL(timeout()),this, SLOT(on_timer_timeout()));
        timer->start(20);
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::on_timer_timeout()
{

    updateGL();
}
QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}



void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

   // static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //--Handle the camera angle.
    for (int c = 0; c < 3; ++c) {
            camera_trans_lag[c] += (camera_trans[c] - camera_trans_lag[c]) * inertia;
            camera_rot_lag[c] += (camera_rot[c] - camera_rot_lag[c]) * inertia;
    }
    glTranslatef(camera_trans_lag[0], camera_trans_lag[1], camera_trans_lag[2]);
    glRotatef(camera_rot_lag[0], 1.0, 0.0, 0.0);
    glRotatef(camera_rot_lag[1], 0.0, 1.0, 0.0);

    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
   glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(60.0, (float) width / (float) height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
   // int mods;
    Qt::MouseButton state = event->button();

        if (state == Qt::LeftButton)
            buttonState =1;
        else
            buttonState = 0;
     Qt::KeyboardModifiers mods =  event->modifiers();
        //mods = glutGetModifiers();
        if (mods & Qt::ShiftModifier)
        {
            buttonState = 2;
        }
        else if (mods & Qt::ControlModifier) {
            buttonState = 3;
        }

}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (buttonState == 3)
    {
        // left+middle = zoom
        camera_trans[2] += (dy / 100.0f) * 0.5f * fabs(camera_trans[2]);
    }
    else if (buttonState & 2)
    {
        // middle = translate
        camera_trans[0] += dx / 100.0f;
        camera_trans[1] -= dy / 100.0f;
    }
    else if (buttonState & 1)
    {
        // left = rotate
        camera_rot[0] += dy / 5.0f;
        camera_rot[1] += dx / 5.0f;
    }

    lastPos = event->pos();
     updateGL();


}

/**----drawing the reference lines----**/
void MyGLWidget::DrawReferenceLines() {

    glLineWidth(1.5);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(-10.0, -10.0, 0.0);
    glVertex3f(-10.0, 10.0, 0.0);

    glVertex3f(-6.0, -10.0, 0.0);
    glVertex3f(-6.0, 10.0, 0.0);

    glVertex3f(-2.0, -10.0, 0.0);
    glVertex3f(-2.0, 10.0, 0.0);

    glVertex3f(2.0, -10.0, 0.0);
    glVertex3f(2.0, 10.0, 0.0);

    glVertex3f(6.0, -10.0, 0.0);
    glVertex3f(6.0, 10.0, 0.0);

    glVertex3f(10.0, -10.0, 0.0);
    glVertex3f(10.0, 10.0, 0.0);

    glVertex3f(14.0, -10.0, 0.0);
    glVertex3f(14.0, 10.0, 0.0);

    glVertex3f(18.0, -10.0, 0.0);
    glVertex3f(18.0, 10.0, 0.0);

    glVertex3f(-10.0, -6.0, 0.0);
    glVertex3f(18.0, -6.0, 0.0);

    glVertex3f(-10.0, -2.0, 0.0);
    glVertex3f(18.0, -2.0, 0.0);

    glVertex3f(-10.0, 2.0, 0.0);
    glVertex3f(18.0, 2.0, 0.0);

    glVertex3f(-10.0, 6.0, 0.0);
    glVertex3f(18.0, 6.0, 0.0);

    glVertex3f(-10.0, 10.0, 0.0);
    glVertex3f(18.0, 10.0, 0.0);

    glEnd();
}

/**decoding the incoming pose data and store it in the pose_matrix**/
void MyGLWidget::command_decode(char* ptr){

    int offset=0;			//variable used to access the subsequent array elements
    int count=0;			//variable that counts the number of elements in the array

    while(*(ptr+offset)!='\n'){
        ++count;
        ++offset;
    }

    char buff_mode[20];
    char buff_flight[20];
    char first_char = *(ptr+0);
    char* flight_mode = buff_flight;
    char* autopilot_mode = buff_mode;
    if(first_char=='F') {

        memcpy(flight_mode,ptr+1,count-1);
        //mainWindow->lable_mode->setText(flight_mode);
        emit modeChanged(string(flight_mode));

    } else {
        //mainWindow->label->setText(ptr) ;
        emit statusChanged(string(ptr));
    }

    /*switch(first_char){
        case 'F':
            ++ptr;
            memcpy(flight_mode,ptr,count-1);
            mainWindow->lable_mode->setText(flight_mode);
            break;

        case 'M':
            ++ptr;
            memcpy(autopilot_mode,ptr,count-1);
            mainWindow->label->setText(autopilot_mode);
            break;

    }
*/

/*
    //label->setText("first line\nsecond line");
    mainWindow->label->setText(ptr);
    char* tmp=buff;
    string str;
//    std::cout<<"check the size:"<<count<<std::endl;
    char p=*(ptr+0);
    switch(p){
        case 'Q':
          memcpy(tmp,ptr,6);

          str =string(buff);
          //std::cout<<"check string:"<<str<<std::endl;
          break;

        case 'S':
          memcpy(tmp,ptr,4);
          str =string(buff);
          //std::cout<<"check string:"<<str<<std::endl;
          break;

        case 'M':
          memcpy(tmp,ptr,3);
          str =string(buff);
          //std::cout<<"check string:"<<str<<std::endl;
          break;

        case 'I':
          memcpy(tmp,ptr,7);
          str =string(buff);
          //std::cout<<"check string:"<<str<<std::endl;
          break;

    }
*/





   }



/**decoding the incoming pose data and store it in the pose_matrix**/
void MyGLWidget::pose_decode(char* ptr, Matrix4d &pose_matrix){

    int offset=0;			//variable used to access the subsequent array elements
    int count=0;			//variable that counts the number of elements in the array
    //std::cout
    while(*(ptr+offset)!='\n'){
        ++count;
        ++offset;
    }
    char buff[10];
    char* tmp=buff;
    char* new_ptr=ptr;
    float val;
    for(int i=0;i<strlen(ptr);i++){
      //cout<<"check data:"<<ptr[i]<<std::endl;
    }
    std::cout<<"check the size:"<<count<<std::endl;
    offset=0;
    while(*(ptr+offset)!='\n'){
    char p=*(ptr+offset);
    switch(p){

      case 'A':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        //std::cout<<"check val:"<<val<<std::endl;
        pose_matrix(0,0)=val;
        break;
      case 'B':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(0,1)=val;
        break;
      case 'C':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(0,2)=val;
        break;
      case 'D':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(0,3)=val;
        break;

      case 'E':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(1,0)=val;
        break;
      case 'F':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(1,1)=val;
        break;

      case 'G':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(1,2)=val;
        break;

      case 'H':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(1,3)=val;
        break;

      case 'I':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(2,0)=val;
        break;

      case 'J':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(2,1)=val;
        break;

      case 'K':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(2,2)=val;
        break;
      case 'L':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(2,3)=val;
        break;

      case 'M':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(3,0)=val;
        break;

      case 'N':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(3,1)=val;
        break;

      case 'O':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(3,2)=val;
        break;

      case 'P':
        memcpy(tmp,ptr+offset+1,8);
        val=atof(buff);
        pose_matrix(3,3)=val;
        break;
      default:
        ;
       // cout<<"Error, bad data receive,\n";
    }

    //printf("check val:%f\n",val);
    offset=offset+1;
    }
}


void MyGLWidget::drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    glPushMatrix();
    glScalef(fA,fB,fC);

    gluSphere(quadricObj,1.0f,uiSlices,uiStacks);
    glPopMatrix();

}
void MyGLWidget::DrawTorus( double innerRadius, double outerRadius, GLint sides, GLint rings ) {

   // glScalef(5,5,5);

    double TWOPI = 2 * Pi;
    for (int i = 0; i < sides; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= rings; j++) {
            for (int k = 1; k >= 0; k--) {

                double s = (i + k) % sides + outerRadius/2;
                double t = j;// % rings;
                double theta = s * TWOPI / sides;
                double rho = t * TWOPI / rings;

                double x = (outerRadius + innerRadius * cos(theta)) * cos(rho);
                double y = (outerRadius + innerRadius * cos(theta)) * sin(rho);
                double z = innerRadius * sin(theta);
                float u = (i + k) / (float)sides;
                float v = j;// (float)rings;
                float nx = cos(rho)*cos(theta);
                float ny = sin(rho)*cos(theta);
                float nz = sin(theta);
                glNormal3f(nx,ny,nz);
                glTexCoord2f(u,v);

                glVertex3d(x, y, z);
            }
        }
        glEnd();
    }
  //  glScalef(.2,.2,.2);

}

void MyGLWidget::drawBlade()
{
    glDisable(GL_CULL_FACE);
    //Draw blade
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glVertex3f(0.5f, 10.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
    //Draw blade

    glEnable(GL_CULL_FACE);

}


void MyGLWidget::drawPropellers(int rotorDirection)
{
    glPushMatrix();
    static float propScale = 1;


    glScalef(propScale,propScale,propScale);


    gluCylinder(quadricObj, 0.25f, 0.25f, 3.0f, 10.0f, 10.0f);
    glTranslatef(0.0f, 0.0f, 3.0f);


    //glutSolidTorus(.5f, 10.0f, 10.0f, 30.0f);
    DrawTorus(.5f, 10.0f, 10.0f, 30.0f);
    if (rotorDirection)
        glRotatef(propAngle, 0.0f, 0.0f, 1.0f);
    else
        glRotatef(-propAngle, 0.0f, 0.0f, 1.0f);



    drawBlade();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    drawBlade();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    drawBlade();

    glPopMatrix();

}


void MyGLWidget::drawQuad()
{
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    propAngle += propSpeed*1000;

   // glMultMatrixf(glm::value_ptr(Model)); //load Model matrix
   // glBindTexture(GL_TEXTURE_2D, EarthMap);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //Draw Axes
    //drawAxes();
    glScalef(.05f,.05f,.05f);
    glRotated(90,1,0,0);
    glRotated(90,0,0,1);

    glColor4f(.75f, .75f, .75f, 1.0f);



    drawEllipsoid(20, 20, 15, 5, 5);
    glTranslatef(15.0f, 0.0f, 0.0f);
    gluSphere(quadricObj, 5.0f, 20, 20); //draw head
    glTranslatef(-15.0f, 0.0f, 0.0f);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //draw Arms
    glPushMatrix();
    glTranslatef(20.0f, 5.0f, 20.0f);
    glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);


    gluSphere(quadricObj, .5, 15, 15);

    gluCylinder(quadricObj, 0.25f, 0.25f, 40.0f*sqrt2, 10.0f, 10.0f);

    glTranslatef(0.0f,0.0f,40.0f*sqrt2);

    gluSphere(quadricObj, .5, 15, 15);
    glTranslatef(0.0f,0.0f,-40.0f*sqrt2);

    glTranslatef(20.0f*sqrt2,0.0, 20.0f*sqrt2);
    glRotatef(90.0f, 0.0, -1.0f, 0.0f);
    //drawAxes();

    gluSphere(quadricObj, .5, 15, 15);

    gluCylinder(quadricObj, 0.25f, 0.25f, 40.0f*sqrt2, 10.0f, 10.0f);

    glTranslatef(0.0f,0.0f,40.0f*sqrt2);

    gluSphere(quadricObj, .5, 15, 15);
    glTranslatef(0.0f,0.0f,-40.0f*sqrt2);

    //Draw

    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    //drawAxes();
    for(int i=0;i<4;i++)
    {
        drawPropellers(i%2);
        glTranslatef(20.0f*sqrt2, -20.0f*sqrt2, 0.0f);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    }

    glPopMatrix();


    glPopMatrix();


}

void MyGLWidget::draw()
{
    //--drawing the global coordinates
       glLineWidth(1.5);
       glColor3f(1.0, 0.20, 0.0);
       glBegin(GL_LINES);
       glVertex3f(0.0, 0.0, 0.0);
       glVertex3f(10, 0, 0);
       glColor3f(0.20, 1.0, 0.0);
       glVertex3f(0.0, 0.0, 0.0);
       glVertex3f(0, 10, 0);
       glColor3f(0.0, 0.0, 1.0);
       glVertex3f(0.0, 0.0, 0.0);
       glVertex3f(0, 0, 10);
       glEnd();
       //--draw the reference GL_LINES
       DrawReferenceLines();

       // std::cout<<"check if it actually comes here"<<std::endl;
       //--receive data from the onboard computer and store them in the pose matrix
       int *check =test_server->ptr;
       int fd=*check;
       int bytes_available;
       char *pt;
       char buf[200];
       pt=buf;
       ioctl(fd,FIONREAD, & bytes_available);
       if(bytes_available>0){
               //std::cout<<"check if any bytes available:"<<std::endl;
               test_server->recv_data(buf);
               if(pt[0]=='A'){
                   pose_decode(pt, CameraToWorldMatrix);

                   char tempBuff[250];
                   sprintf(tempBuff,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n ",CameraToWorldMatrix(0,0),CameraToWorldMatrix(0,1),CameraToWorldMatrix(0,2),
                      CameraToWorldMatrix(0,3),CameraToWorldMatrix(1,0),CameraToWorldMatrix(1,1),CameraToWorldMatrix(1,2),CameraToWorldMatrix(1,3),
                      CameraToWorldMatrix(2,0),CameraToWorldMatrix(2,1),CameraToWorldMatrix(2,2),CameraToWorldMatrix(2,3),
                      CameraToWorldMatrix(3,0),CameraToWorldMatrix(3,1),CameraToWorldMatrix(3,2),CameraToWorldMatrix(3,3));


                   emit writeDataToFile(string(tempBuff));


                   Matrix<double,3,1> Pt=CameraToWorldMatrix.block<3,1>(0,3);
                   PositionVec.push_back(Pt);
               }
               else
                   command_decode(pt);


       }

       glColor3f(1.0,1.0,1.0);
       glPointSize(4);
       if(PositionVec.size()>0) {
           for(int k=0;k<PositionVec.size()-1;k++){
               glBegin(GL_POINTS);
               Matrix<double,3,1> Pt=PositionVec.at(k);
               glVertex3f(Pt[0],Pt[1],Pt[2]);
               glEnd();
           }
       }
       //--drawing the camera Center
       Matrix<double,3,1> CameraCenter=CameraToWorldMatrix.block<3,1>(0,3);
       glColor3f(1.0,1.0,1.0);
       glPointSize(4);
       glBegin(GL_POINTS);
       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glEnd();


       //std::cout<<"check the camera to world matrix:"<<CameraToWorldMatrix<<std::endl;
       //--drawing the camera coordinates
       Vector4d x_c;x_c<<2,0,0,1;
       Vector4d y_c;y_c<<0,2,0,1;
       Vector4d z_c;z_c<<0,0,2,1;
       Vector4d X_w, Y_w, Z_w;


       X_w=CameraToWorldMatrix*x_c;
       Y_w=CameraToWorldMatrix*y_c;
       Z_w=CameraToWorldMatrix*z_c;




       glColor3f(1.0, 1.0, 1.0);
       glBegin(GL_LINES);
       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(X_w(0,0),X_w(1,0),X_w(2,0));
       glColor3f(1.0, 1.0, 0.0);
       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(Y_w(0,0),Y_w(1,0),Y_w(2,0));
       glColor3f(0.0, 1.0, 1.0);
       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(Z_w(0,0),Z_w(1,0),Z_w(2,0));
       glEnd();

       // drawing the camera plane at z=1
       Vector4d pt1_c(-0.9,-0.9,1,1);
       Vector4d pt2_c(0.9,-0.9,1,1);
       Vector4d pt3_c(0.9,0.9,1,1);
       Vector4d pt4_c(-0.9,0.9,1,1);

       Vector4d Pt1_w, Pt2_w, Pt3_w, Pt4_w;
       Pt1_w=CameraToWorldMatrix*pt1_c;
       Pt2_w=CameraToWorldMatrix*pt2_c;
       Pt3_w=CameraToWorldMatrix*pt3_c;
       Pt4_w=CameraToWorldMatrix*pt4_c;



       drawQuad();
       //draw Camera
/*
       glLineWidth(1.5);
       glColor3f(1.0, 1.0, 1.0);
       glBegin(GL_LINES);
       glVertex3f(Pt1_w[0],Pt1_w[1],Pt1_w[2]);
       glVertex3f(Pt2_w[0],Pt2_w[1],Pt2_w[2]);

       glVertex3f(Pt2_w[0],Pt2_w[1],Pt2_w[2]);
       glVertex3f(Pt3_w[0],Pt3_w[1],Pt3_w[2]);

       glVertex3f(Pt3_w[0],Pt3_w[1],Pt3_w[2]);
       glVertex3f(Pt4_w[0],Pt4_w[1],Pt4_w[2]);

       glVertex3f(Pt4_w[0],Pt4_w[1],Pt4_w[2]);
       glVertex3f(Pt1_w[0],Pt1_w[1],Pt1_w[2]);

       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(Pt1_w[0],Pt1_w[1],Pt1_w[2]);

       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(Pt2_w[0],Pt2_w[1],Pt2_w[2]);

       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(Pt3_w[0],Pt3_w[1],Pt3_w[2]);

       glVertex3f(CameraCenter[0],CameraCenter[1],CameraCenter[2]);
       glVertex3f(Pt4_w[0],Pt4_w[1],Pt4_w[2]);
       glEnd();
*/

// updateGL();
}
