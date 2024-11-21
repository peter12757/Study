#include "OpenglRender.h"


OpenglRender::OpenglRender(QObject *parent)
    :m_pParent(parent){

}

OpenglRender::~OpenglRender()
{

}

void OpenglRender::initialize()
{
    initializeOpenGLFunctions();


     glEnable(GL_DEPTH_TEST);
    //现代opengl渲染管线依赖着色器来处理传入的数据
    //着色器：就是使用openGL着色语言(OpenGL Shading Language, GLSL)编写的一个小函数,
    //       GLSL是构成所有OpenGL着色器的语言,具体的GLSL语言的语法需要读者查找相关资料
    //初始化顶点着色器 对象
     int success =0;

    m_pVSHader = new QOpenGLShader(QOpenGLShader::Vertex, m_pParent);
    //编译顶点着色器程序
    bool bCompile = m_pVSHader->compileSourceCode(vSrcCode);
    qDebug()<<"m_pVSHader->compileSourceCode:"<<bCompile;
    glGetShaderiv(m_pVSHader->shaderId(), GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        // 获取错误日志
        glGetShaderInfoLog(m_pVSHader->shaderId(), 1024, NULL, infoLog);
        qDebug() << "ERROR::SHADER_COMPILATION_ERROR of type: " << m_pVSHader->shaderType() << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
    }

    //初始化片段着色器 功能gpu中yuv转换成rgb
    m_pFSHader = new QOpenGLShader(QOpenGLShader::Fragment, m_pParent);
    //将glsl源码送入编译器编译着色器程序
    bCompile = m_pFSHader->compileSourceCode(fSrcCode);
    qDebug()<<"m_pFSHader->compileSourceCode:"<<bCompile;
    glGetShaderiv(m_pFSHader->shaderId(), GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        // 获取错误日志
        glGetShaderInfoLog(m_pFSHader->shaderId(), 1024, NULL, infoLog);
        qDebug() << "ERROR::SHADER_COMPILATION_ERROR of type: " << m_pFSHader->shaderType() << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
    }

    #define PROGRAM_VERTEX_ATTRIBUTE 0
    #define PROGRAM_TEXCOORD_ATTRIBUTE 1
    //创建着色器程序容器
    m_pShaderProgram = new QOpenGLShaderProgram;
    //将片段着色器添加到程序容器
    m_pShaderProgram->addShader(m_pFSHader);
    //将顶点着色器添加到程序容器
    m_pShaderProgram->addShader(m_pVSHader);

    //链接所有所有添入到的着色器程序
    m_pShaderProgram->link();

    // 检查着色器编译错误
    glGetShaderiv(m_pShaderProgram->programId(), GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        // 获取错误日志
        glGetShaderInfoLog(m_pShaderProgram->programId(), 1024, NULL, infoLog);
        qDebug() << "ERROR::SHADER_COMPILATION_ERROR program" << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
    }
    //激活所有链接
    m_pShaderProgram->bind();
//绑定属性vertexIn到指定位置ATTRIB_VERTEX,该属性在顶点着色源码其中有声明
    int vertsLocation = m_pShaderProgram->attributeLocation("vertexIn");
    //绑定属性textureIn到指定位置ATTRIB_TEXTURE,该属性在顶点着色源码其中有声明
    int textureLocation = m_pShaderProgram->attributeLocation("textureIn");



    //读取着色器中的数据变量tex_y, tex_u, tex_v的位置,这些变量的声明可以在
    //片段着色器源码中可以看到
    y.textureUniform = m_pShaderProgram->uniformLocation("tex_y");
    u.textureUniform =  m_pShaderProgram->uniformLocation("tex_u");
    v.textureUniform =  m_pShaderProgram->uniformLocation("tex_v");
    qDebug("textureUniform_y = %d textureUniform_u=%d textureUniform_v=%d\n", y.textureUniform, u.textureUniform, v.textureUniform);


    //设置属性ATTRIB_VERTEX的顶点矩阵值以及格式
    glVertexAttribPointer(vertsLocation, 2, GL_FLOAT, 0, 0, vertexVertices);
    //设置属性ATTRIB_TEXTURE的纹理矩阵值以及格式
    glVertexAttribPointer(textureLocation, 2, GL_FLOAT, 0, 0, textureVertices);
    //启用ATTRIB_VERTEX属性的数据,默认是关闭的
    glEnableVertexAttribArray(vertsLocation);
    //启用ATTRIB_TEXTURE属性的数据,默认是关闭的
    glEnableVertexAttribArray(textureLocation);

    //分别创建y,u,v纹理对象
    y.m_pTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    u.m_pTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    v.m_pTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    y.m_pTexture->create();
    u.m_pTexture->create();
    v.m_pTexture->create();

    //获取返回y分量的纹理索引值
    y.id = y.m_pTexture->textureId();
    //获取返回u分量的纹理索引值
    u.id = u.m_pTexture->textureId();
    //获取返回v分量的纹理索引值
    v.id = v.m_pTexture->textureId();
    qDebug("addr=%x id_y = %d id_u=%d id_v=%d\n", this, y.id, u.id, v.id);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void OpenglRender::paintGL()
{
    //加载y数据纹理
    //激活纹理单元GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    //使用来自y数据生成纹理
    glBindTexture(GL_TEXTURE_2D, y.id);
    //使用内存中m_pBufYuv420p数据创建真正的y数据纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, y.data->m_nBufW, y.data->m_nBufH, 0, GL_RED, GL_UNSIGNED_BYTE, y.data->m_pBufYuv);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    qDebug()<<"-y.id:"<<y.id<<"-y.textureUniform:"<<y.textureUniform<<"-y.m_nBufW:"<<y.data->m_nBufW<<"-y.m_nBufH:"<<y.data->m_nBufH;
    //加载u数据纹理
    glActiveTexture(GL_TEXTURE1);//激活纹理单元GL_TEXTURE1
    glBindTexture(GL_TEXTURE_2D, u.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, u.data->m_nBufW, u.data->m_nBufH, 0, GL_RED, GL_UNSIGNED_BYTE, u.data->m_pBufYuv);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    qDebug()<<"-u.id:"<<u.id<<"-u.textureUniform:"<<u.textureUniform<<"-u.m_nBufW:"<<u.data->m_nBufW<<"-u.m_nBufH:"<<u.data->m_nBufH;
    //加载v数据纹理
    glActiveTexture(GL_TEXTURE2);//激活纹理单元GL_TEXTURE2
    glBindTexture(GL_TEXTURE_2D, v.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, v.data->m_nBufW, v.data->m_nBufH, 0, GL_RED, GL_UNSIGNED_BYTE, v.data->m_pBufYuv);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    qDebug()<<"-v.id:"<<v.id<<"-v.textureUniform:"<<v.textureUniform<<"-v.m_nBufW:"<<v.data->m_nBufW<<"-v.m_nBufH:"<<v.data->m_nBufH;
    //指定y纹理要使用新值 只能用0,1,2等表示纹理单元的索引，这是opengl不人性化的地方
    //0对应纹理单元GL_TEXTURE0 1对应纹理单元GL_TEXTURE1 2对应纹理的单元
    glUniform1i(y.textureUniform, 0);
    //指定u纹理要使用新值
    glUniform1i(u.textureUniform, 1);
    //指定v纹理要使用新值
    glUniform1i(v.textureUniform, 2);
    //使用顶点数组方式绘制图形
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void OpenglRender::resize(int width, int height)
{
    glViewport(0,0,width,height);
}



