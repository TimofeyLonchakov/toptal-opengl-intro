////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#include "../include/model2d_processing.h"
#include "../include/model_creator.h"

#include <QPainter>
#include <QtGlobal>
#include <QRectF>

namespace Model2DProcessing
{

void Create2DImage(QImage& image,
                   const QString& sText,
                   const QFont& font,
                   unsigned short dimension)
{
    qreal coeff = (static_cast<qreal>(QFontMetrics(font).height()) / static_cast<qreal>(dimension));
    qreal newFontPointSize = (font.pointSizeF() / coeff);

    QFont fontScaled(font);
    fontScaled.setPointSizeF(newFontPointSize);

    int width = (3 + QFontMetrics(fontScaled).boundingRect(sText).width());

    QImage imageOutput(width, dimension, QImage::Format_Mono);
    imageOutput.fill((uint)1);
    {
        QPainter painter(&imageOutput);
        painter.setFont(fontScaled);
        painter.setPen(QColor(Qt::black));

        const QRect rectDraw(
            QPoint(0, 0), QSize(width, dimension));
        painter.drawText(rectDraw, (Qt::AlignLeft | Qt::AlignTop), sText);
    }

    image = imageOutput;
}

}
