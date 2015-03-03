////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#ifndef MODEL2DPROCESSING_H
#define MODEL2DPROCESSING_H

#include "model3d.h"

#include <QPixmap>
#include <QChar>
#include <QFont>
#include <QByteArray>

namespace Model2DProcessing
{

void Create2DImage(QImage& image,
                   const QString& sText,
                   const QFont& font,
                   unsigned short dimension);
};

#endif // MODEL2DPROCESSING_H
