#ifndef XVIDEOWIGET_H
#define XVIDEOWIGET_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include "opengl/OpenglRender.h"
#include <QtOpenGLWidgets/QOpenGLWidget>





class XVideoWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    XVideoWidget(QWidget* parent);
    ~XVideoWidget();

protected:
    void paintGL(); //刷新现实
    void initializeGL();    //初始化
    void resizeGL(int width,int height);    //窗口大小变化

private:
    void bindGL(int index);

    //data

private:

    OpenglRender *render;

    FILE *fp;

    QTimer *timer;


    unsigned char* m_pBufYuv;

    std::string filename;
    int video_width; //视频分辨率宽
    int video_height; //视频分辨率高


};

#endif // XVIDEOWIGET_H
