////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#include "../include/gl_widget.h"
#include "../include/model2d_processing.h"

#include <QBitmap>
#include <QOpenGLFunctions>
#include <QtGlobal>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>

const double GlWidget::m_wheelStep = 1.05;
const double GlWidget::m_scaleMin = 0.001;
const double GlWidget::m_scaleMax = (1.0 / GlWidget::m_scaleMin);

GlWidget::GlWidget(QWidget* parent)
    :   QOpenGLWidget(parent)
    ,   m_hVertexes(0)
    ,   m_hNormals(0)
    ,   m_coordVertex(-1)
    ,   m_coordNormal(-1)
    ,   m_matrixVertex(-1)
    ,   m_matrixNormal(-1)
    ,   m_colorFragment(-1)
    ,   m_scaleCoeff(1.0)
    ,   m_mouseMove(false)
{
}

GlWidget::~GlWidget()
{
    FreeRenderData();
}

void GlWidget::SetModel(Model3D& model)
{
    FreeRenderData();
    if(!model.IsValid())
    {
        return;
    }

    m_model.Release();
    m_model.Swap(model);
    m_model.Recalc();

    float* coords = GenerateVertexBuffer(m_model);
    if(0 != coords)
    {
        QOpenGLFunctions funcs(QOpenGLContext::currentContext());
        funcs.glGenBuffers(1, &m_hVertexes);
        funcs.glGenBuffers(1, &m_hNormals);
        if((0 != m_hVertexes) && (0 != m_hNormals))
        {
            funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hVertexes);
            funcs.glBufferData(GL_ARRAY_BUFFER,
                (3 * 3 * m_model.GetTriangleCount() * sizeof(float)),
                coords,
                GL_STATIC_DRAW);

            GenerateNormalsBuffer(m_model, coords);
            funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hNormals);
            funcs.glBufferData(GL_ARRAY_BUFFER,
                (3 * 3 * m_model.GetTriangleCount() * sizeof(float)),
                coords,
                GL_STATIC_DRAW);

            funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else
        {
            FreeRenderData();
        }
        ReleaseVertexBuffer(coords);
    }
}

void GlWidget::initializeGL()
{
    QOpenGLWidget::initializeGL();

    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
        QString::fromUtf8(
            "#version 400\r\n"
            "\r\n"
            "layout (location = 0) in vec3 coordVertexes;\r\n"
            "layout (location = 1) in vec3 coordNormals;\r\n"
            "flat out float lightIntensity;\r\n"
            "\r\n"
            "uniform mat4 matrixVertex;\r\n"
            "uniform mat4 matrixNormal;\r\n"
            "\r\n"
            "void main()\r\n"
            "{\r\n"
            "   gl_Position = matrixVertex * vec4(coordVertexes, 1.0);\r\n"
            "   lightIntensity = abs((matrixNormal * vec4(coordNormals, 1.0)).z);\r\n"
            "}"));
    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
        QString::fromUtf8(
            "#version 400\r\n"
            "\r\n"
            "flat in float lightIntensity;\r\n"
            "\r\n"
            "layout (location = 0) out vec4 FragColor;\r\n"
            "uniform vec3 fragmentColor;\r\n"
            "\r\n"
            "void main()\r\n"
            "{\r\n"
            "    FragColor = vec4(fragmentColor * lightIntensity, 1.0);\r\n"
            "}"));
    m_shaderProgram.link();
    m_shaderProgram.bind();

    m_coordVertex = m_shaderProgram.attributeLocation(QString::fromUtf8("coordVertexes"));
    m_coordNormal = m_shaderProgram.attributeLocation(QString::fromUtf8("coordNormals"));
    m_matrixVertex = m_shaderProgram.uniformLocation(QString::fromUtf8("matrixVertex"));
    m_matrixNormal = m_shaderProgram.uniformLocation(QString::fromUtf8("matrixNormal"));
    m_colorFragment = m_shaderProgram.uniformLocation(QString::fromUtf8("fragmentColor"));

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
}

void GlWidget::paintGL()
{
    // Erase the scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Check whether the model exists
    if((0 == m_hVertexes) || (0 == m_hNormals))
    {
        return;
    }

    // Set Projection mode
    glMatrixMode(GL_PROJECTION);

    // Clear transformation parameters
    glLoadIdentity();

    // Get model transformation matrix
    QMatrix4x4 matrixVertex;
    GetMatrixTransform(matrixVertex, m_model);

    // Set Shader Program object' parameters
    m_shaderProgram.setUniformValue(m_matrixVertex, matrixVertex);
    m_shaderProgram.setUniformValue(m_matrixNormal, m_matrixRotate);
    QColor fragmentColor(eFragmentColor);
    m_shaderProgram.setUniformValue(m_colorFragment,
        static_cast<GLfloat>(fragmentColor.red()) / 256.0f,
        static_cast<GLfloat>(fragmentColor.green()) / 256.0f,
        static_cast<GLfloat>(fragmentColor.blue()) / 256.0f);

    QOpenGLFunctions funcs(QOpenGLContext::currentContext());

    // Vertex data
    glEnableClientState(GL_VERTEX_ARRAY);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hVertexes);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    funcs.glVertexAttribPointer(m_coordVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal data
    glEnableClientState(GL_NORMAL_ARRAY);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hNormals);
    glNormalPointer(GL_FLOAT, 0, 0);
    funcs.glEnableVertexAttribArray(m_coordNormal);
    funcs.glVertexAttribPointer(m_coordNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw frame
    glDrawArrays(GL_TRIANGLES, 0, (3 * m_model.GetTriangleCount()));

    funcs.glDisableVertexAttribArray(m_coordNormal);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void GlWidget::resizeGL(int w, int h)
{
    // TODO
    QOpenGLWidget::resizeGL(w, h);
}

void GlWidget::FreeRenderData()
{
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());
    if(0 != m_hVertexes)
    {
        funcs.glDeleteBuffers(1, &m_hVertexes);
        m_hVertexes = 0;
    }
}

void GlWidget::GetMatrixTransform(QMatrix4x4& matrixVertex,
                                  const Model3DEx& model)
{
    matrixVertex.setToIdentity();

    QMatrix4x4 matrixScaleScreen;
    double dimMin = static_cast<double>(qMin(width(), height()));
    float scaleScreenVert = static_cast<float>(dimMin * 2.0 /
        static_cast<double>(height()));
    float scaleScreenHorz = static_cast<float>(dimMin * 2.0 /
        static_cast<double>(width()));
    matrixScaleScreen.scale(scaleScreenHorz, scaleScreenVert, 1.0f);

    QMatrix4x4 matrixCenter;
    float centerX, centerY, centerZ;
    model.GetCenter(centerX, centerY, centerZ);
    matrixCenter.translate(-centerX, -centerY, -centerZ);

    QMatrix4x4 matrixScale;
    float radius = 1.0;
    model.GetRadius(radius);
    float scale = static_cast<float>(m_scaleCoeff / radius / 2.0);
    matrixScale.scale(scale, scale, 0.5f / radius);

    QMatrix4x4 matrixTranslateScene;
    matrixTranslateScene.translate(0.0f, 0.0f, -0.5f);

    matrixVertex = matrixScaleScreen * matrixTranslateScene * matrixScale * m_matrixRotate * matrixCenter;
}

float* GlWidget::GenerateVertexBuffer(const Model3D& model)
{
    const unsigned int triangleCount = model.GetTriangleCount();
    float* pointCoord = new float[3 * 3 * triangleCount];

    if(0 != pointCoord)
    {
        const unsigned int* indexes = model.GetIndexes();
        const float* pointCoordSrc = model.GetPoints();
        float* coord = pointCoord;
        for(unsigned int triangle = 0 ; triangle < triangleCount ; ++triangle)
        {
            for(unsigned int vertex = 0 ; vertex < 3 ; ++vertex, ++indexes)
            {
                const float* coordSrc = (pointCoordSrc + (3 * (*indexes)));
                (*(coord++)) = (*(coordSrc++));
                (*(coord++)) = (*(coordSrc++));
                (*(coord++)) = (*(coordSrc++));
            }
        }
    }

    return pointCoord;
}

void GlWidget::GenerateNormalsBuffer(const Model3D& model, float* coords)
{
    const unsigned int triangleCount = model.GetTriangleCount();
    const unsigned int* indexes = model.GetIndexes();
    const float* points = model.GetPoints();
    for(unsigned int triangle = 0 ; triangle < triangleCount ; ++triangle, indexes += 3)
    {
        const float* pointCoord1 = (points + (3 * indexes[0]));
        const float* pointCoord2 = (points + (3 * indexes[1]));
        const float* pointCoord3 = (points + (3 * indexes[2]));

        QVector3D normal(QVector3D::crossProduct(
            QVector3D((pointCoord2[0] - pointCoord1[0]), (pointCoord2[1] - pointCoord1[1]), (pointCoord2[2] - pointCoord1[2])),
            QVector3D((pointCoord3[0] - pointCoord1[0]), (pointCoord3[1] - pointCoord1[1]), (pointCoord3[2] - pointCoord1[2])))
                .normalized());

        (*(coords++)) = normal[0];
        (*(coords++)) = normal[1];
        (*(coords++)) = normal[2];
        (*(coords++)) = normal[0];
        (*(coords++)) = normal[1];
        (*(coords++)) = normal[2];
        (*(coords++)) = normal[0];
        (*(coords++)) = normal[1];
        (*(coords++)) = normal[2];
    }
}

void GlWidget::ReleaseVertexBuffer(float* buffer)
{
    delete [] buffer;
}

void GlWidget::mouseMoveEvent( QMouseEvent* pEvent )
{
    QOpenGLWidget::mouseMoveEvent( pEvent );

    if(m_mouseMove)
    {
        float angleHorz = ((180.0f *
            (pEvent->pos().x() - m_mousePos.x())) /
                static_cast<float>(width()));
        float angleVert = ((180.0f *
            (pEvent->pos().y() - m_mousePos.y())) /
                static_cast<float>(height()));

        QVector3D vectorZOrig(0.0f, 0.0f, 1.0f);
        QVector3D vectorZRotated(vectorZOrig);

        QMatrix4x4 matrixRotateHorz;
        matrixRotateHorz.rotate(angleHorz, QVector3D(0.0f, 1.0f, 0.0f));
        vectorZRotated = matrixRotateHorz.map(vectorZRotated).normalized();

        QMatrix4x4 matrixRotateVert;
        matrixRotateVert.rotate(angleVert, QVector3D(1.0f, 0.0f, 0.0f));
        vectorZRotated = matrixRotateVert.map(vectorZRotated).normalized();

        QVector3D axisRotation(QVector3D::crossProduct(
            vectorZRotated, vectorZOrig).normalized());

        QMatrix4x4 matrixRotate;
        matrixRotate.setToIdentity();
        matrixRotate.rotate(qRadiansToDegrees(qAcos(QVector3D::dotProduct(
            vectorZRotated, vectorZOrig))), axisRotation.normalized());

        m_matrixRotate = matrixRotate * m_matrixRotate;

        m_mousePos = pEvent->pos();
        update();
    }
}

void GlWidget::mousePressEvent( QMouseEvent* pEvent )
{
    m_mouseMove = true;
    m_mousePos = pEvent->pos();

    update();

    QOpenGLWidget::mousePressEvent( pEvent );
}

void GlWidget::mouseReleaseEvent(QMouseEvent* pEvent)
{
    m_mouseMove = false;

    update();

    QOpenGLWidget::mouseReleaseEvent( pEvent );
}

void GlWidget::wheelEvent( QWheelEvent* pEvent )
{
    int nDelta = pEvent->angleDelta().y();
    if(0 != nDelta)
    {
        if( nDelta > 0 )
        {
            if( m_scaleCoeff > m_scaleMin )
            {
                m_scaleCoeff /= m_wheelStep;
            }
        }
        else
        {
            if( m_scaleCoeff < m_scaleMax )
            {
                m_scaleCoeff *= m_wheelStep;
            }
        }
        update();
    }
}
