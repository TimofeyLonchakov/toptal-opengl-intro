#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include "model3d.h"

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QMatrix4x4>

class GlWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    enum
    {
        eFragmentColor  = 0x00FF00,
        eCursorColor    = 0xFF0000
    };
public:
    GlWidget(QWidget* parent);
    ~GlWidget();

public:
    void SetModel(Model3D& model);

public:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void FreeRenderData();
    void GetMatrixTransform(QMatrix4x4& matrixVertex,
                            const Model3DEx& model);

private:
    static float* GenerateVertexBuffer(const Model3D& model);
    static void GenerateNormalsBuffer(const Model3D& model, float* coords);
    static void ReleaseVertexBuffer(float* buffer);

protected:
    void mouseMoveEvent( QMouseEvent* pEvent );
    void mousePressEvent( QMouseEvent* pEvent );
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void wheelEvent( QWheelEvent* pEvent );

private:
    static const double m_wheelStep;
    static const double m_scaleMin;
    static const double m_scaleMax;

private:
    Model3DEx               m_model;
    GLuint                  m_hVertexes;
    GLuint                  m_hNormals;
    QOpenGLShaderProgram    m_shaderProgram;
    int                     m_coordVertex;
    int                     m_coordNormal;
    int                     m_matrixVertex;
    int                     m_matrixNormal;
    int                     m_colorFragment;

private:
    double      m_scaleCoeff;
    QMatrix4x4  m_matrixRotate;
    bool        m_mouseMove;
    QPoint      m_mousePos;
};

#endif // GL_WIDGET_H
