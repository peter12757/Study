#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#define GET_STR(x) #x
#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4


//vertex shader顶点着色器源码
static const  char *vSrcCode = "\
attribute vec4 vertexIn; \
    attribute vec2 textureIn; \
    varying vec2 textureOut;  \
    void main(void)           \
{                         \
        gl_Position = vertexIn; \
        textureOut = textureIn; \
}";


//shader element 片段着色器源码
const static char *fSrcCode ="varying vec2 textureOut; \
    uniform sampler2D tex_y; \
    uniform sampler2D tex_u; \
    uniform sampler2D tex_v; \
    void main(void) \
{ \
        vec3 yuv; \
        vec3 rgb; \
        yuv.x = texture2D(tex_y, textureOut).r; \
        yuv.y = texture2D(tex_u, textureOut).r - 0.5; \
        yuv.z = texture2D(tex_v, textureOut).r - 0.5; \
        rgb = mat3( 1,       1,         1, \
               0,       -0.39465,  2.03211, \
               1.13983, -0.58060,  0) * yuv; \
        gl_FragColor = vec4(rgb, 1); \
}";

//vertex
const static GLfloat vertexVertices[] = {
    -1.0f, -1.0f, //position 0
    -1.0f, 1.0f,  //position 1
    1.0f, 1.0f,   //position 2
    1.0f, -1.0f,   //position 3
};

//texture
const static GLfloat textureVertices[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
};

class RenderData {
public:
    unsigned char* m_pBufYuv;
    //视频宽高的size，编码格式可能都不一样
    int m_nBufW;
    int m_nBufH;

public:
    RenderData(int width,int height){
        qDebug()<<"RenderData crate width:"<<width<<"height:"<<height;
        m_pBufYuv = new unsigned char[width*height];
        m_nBufW = width;
        m_nBufH = height;
    }

};

struct OpenglObj
{
    OpenglObj(){}
    GLuint textureUniform;  //纹理数据位置
    GLuint id; //纹理对象ID
    QOpenGLTexture* m_pTexture;  //纹理对象
    RenderData *data;

    void build() {
        m_pTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        m_pTexture->setSize(data->m_nBufW,data->m_nBufH);
        m_pTexture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
        m_pTexture->create();
        m_pTexture->setFormat(QOpenGLTexture::R8_UNorm);
        m_pTexture->allocateStorage();
        m_pTexture->setData(QOpenGLTexture::Red,QOpenGLTexture::UInt8,data->m_pBufYuv);
    }

};

class OpenglRender : protected QOpenGLFunctions
{
public:
    OpenglRender(QObject *parent = nullptr);
    ~OpenglRender();

    void initialize();
    void paintGL();
    void resize(int width,int height);



public:
    QObject *m_pParent;
    OpenglObj y;
    OpenglObj u;
    OpenglObj v;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    QOpenGLShader *m_pVSHader;  //顶点着色器程序对象
    QOpenGLShader *m_pFSHader;  //片段着色器对象
    QOpenGLShaderProgram *m_pShaderProgram; //着色器程序容器



};

#endif // OPENGLRENDER_H
