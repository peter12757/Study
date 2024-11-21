#include "XVideoWidget.h"

XVideoWidget::XVideoWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    qDebug()<<"XVideoWiget create";
    render = new OpenglRender(this);
    filename = "out240x128.yuv";
    video_width = 240;
    video_height = 128;
    qDebug()<<"XVideoWiget create"<<filename<<"width:"<<video_width<<"height:"<<video_height;
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
     qDebug()<<"format version"<<format.version().first<<"-"<<format.version().second;
}

XVideoWidget::~XVideoWidget() {
    qDebug()<<"XVideoWiget destroy";
    if(fp) {
        fclose(fp);
        fp = nullptr;
    }
    if(render) {
        delete render;
        render = nullptr;
    }
}

void XVideoWidget::initializeGL() {
    qDebug()<<"XVideoWiget::initializeGL"<<filename<<"width:"<<video_width<<"height:"<<video_height;

    fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        qDebug()<<"fp open fail";
    }
    render->y.data = new RenderData(video_width,video_height);
    render->u.data = new RenderData(video_width/2,video_height/2);
    render->v.data = new RenderData(video_width/2,video_height/2);
    render->initialize();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(40);

}

void XVideoWidget::paintGL() {

    if (feof(fp)) {
        // fseek(fp,0,SEEK_SET);
        qDebug()<<"XVideoWiget::paintGL complete";
        timer->stop();
        return;
    }
    fread(render->y.data->m_pBufYuv, 1, render->y.data->m_nBufW*render->y.data->m_nBufH, fp);
    fread(render->u.data->m_pBufYuv, 1, render->u.data->m_nBufW*render->u.data->m_nBufH, fp);
    fread(render->v.data->m_pBufYuv, 1, render->v.data->m_nBufW*render->v.data->m_nBufH, fp);
    render->paintGL();
    // qDebug()<<"XVideoWiget::psintGL end";


}

void XVideoWidget::resizeGL(int width,int height) {
    qDebug()<<"XVideoWiget::resizeGL width:"<<width<<"  height:"<<height;
    if(height == 0)// 防止被零除
    {
        height = 1;// 将高设为1
    }
    //设置视口
    // render->resize(width,height);
}
