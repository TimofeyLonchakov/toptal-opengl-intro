////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#ifndef MODEL_CREATOR_H
#define MODEL_CREATOR_H

#include "model3d.h"

#include <QImage>
#include <QByteArray>
#include <QBitArray>

class ModelCreator
{
public:
    ModelCreator(const QImage& image,
                 Model3D& model);
    ~ModelCreator();

public:
    void Create3DModel();

private:
    class PixelData
    {
    public:
        enum
        {
            eWrongIndex = 0xFFFFFFFF
        };
        enum ESide
        {
            eLeft,
            eTop,
            eRight,
            eBottom,

            eSideCount
        };
        enum ECorner
        {
            eTopLeft,
            eTopRight,
            eBottomRight,
            eBottomLeft,

            eCornerCount
        };

    public:
        PixelData();
        ~PixelData();

    public:
        unsigned int    m_x;
        unsigned int    m_y;
        bool            m_edge[eSideCount];
        unsigned int    m_pointIndex[eCornerCount];
        bool            m_corner[eCornerCount];
    };

private:
    void GetPixelCount();
    void CreatePixelData();
    void CreateBinaryData();

private:
    const QImage&   m_image;
    Model3D&        m_model;
    QBitArray       m_pixelSet;
    unsigned int    m_pixelCount;
    PixelData*      m_pixelData;
    unsigned int    m_pointCount;
    QBitArray       m_cornerSet;
    unsigned int    m_cornerCount;
    unsigned int    m_edgeCount;
};

#endif // MODEL_CREATOR_H
