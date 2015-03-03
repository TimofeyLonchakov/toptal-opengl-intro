#ifndef MODEL3D_H
#define MODEL3D_H

#include <QString>

struct Model3DData
{
    unsigned int    m_triangleCount;
    unsigned int    m_pointCount;
    unsigned int*   m_pointIndices;
    float*          m_pointCoords;
};

class Model3D : private Model3DData
{
public:
    Model3D();
    virtual ~Model3D();

public:
    bool IsValid() const;
    void Release();
    Model3D& operator=(const Model3D& modelSrc);
    Model3D& Swap(Model3D& modelSrc);
    void Assign(Model3DData& modelData);
    bool Load(const QString& sFilePath);
    bool Save(const QString& sFilePath) const;

public:
    unsigned int GetTriangleCount() const   {   return m_triangleCount; };
    unsigned int GetPointCount() const      {   return m_pointCount;    };
    const unsigned int* GetIndexes() const  {   return m_pointIndices;  };
    const float* GetPoints() const          {   return m_pointCoords;   };
};

class Model3DEx : public Model3D
{
public:
    Model3DEx();
    ~Model3DEx();

public:
    void Recalc();
    bool GetRadius(float& radius) const;
    bool GetCenter(float& x, float& y, float& z) const;
    bool GetMin(float& x, float& y, float& z) const;
    bool GetMax(float& x, float& y, float& z) const;

private:
    float   m_center[3];
    float   m_minCoord[3];
    float   m_maxCoord[3];
    float   m_radius;
};

#endif // MODEL3D_H
