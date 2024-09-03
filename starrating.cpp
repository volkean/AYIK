#include <QtGui>
#include <math.h>

#include "starrating.h"

const int PaintingScaleFactor = 20;

StarRating::StarRating(QWidget *parent) : QWidget(parent)
{

}
StarRating::StarRating(int starCount, int maxStarCount)
{
    myStarCount = starCount;
    myMaxStarCount = maxStarCount;

    starPolygon << QPointF(1.0, 0.5);
    for (int i = 1; i < 5; ++i)
        starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),
                               0.5 + 0.5 * sin(0.8 * i * 3.14));

    diamondPolygon << QPointF(0.4, 0.5) << QPointF(0.5, 0.4)
                   << QPointF(0.6, 0.5) << QPointF(0.5, 0.6)
                   << QPointF(0.4, 0.5);


}

QSize StarRating::sizeHint() const
{
    return PaintingScaleFactor * QSize(myMaxStarCount, 1);
}

void StarRating::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paint(&painter, rect(), this->palette(),mode);
}
void StarRating::paint(QPainter *painter, const QRect &rect,
                       const QPalette &palette, EditMode mode) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    //brush.setColor(QColor(255,255,153));
    brush.setColor(QColor(128,0,64));
    painter->setBrush(brush);

    int yOffset = (rect.height() - PaintingScaleFactor) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);

    for (int i = 0; i < myMaxStarCount; ++i) {
        if (i < myStarCount) {
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        } else if (mode == Editable) {
            painter->drawPolygon(diamondPolygon, Qt::WindingFill);
        }
        painter->translate(1.0, 0.0);
    }

    painter->restore();
}
void StarRating::mouseMoveEvent(QMouseEvent *event)
{
    int star = starAtPosition(event->x());

    if (star != starCount() && star != -1) {
        setStarCount(star);
        update();
    }
}

void StarRating::mouseReleaseEvent(QMouseEvent * /* event */)
{
    //emit editingFinished();
}
void StarRating::mousePressEvent(QMouseEvent *event)
{
    int star = starAtPosition(event->x());

    if (star != starCount() && star != -1) {
        setStarCount(star);
        update();
    }
}
int StarRating::starAtPosition(int x)
{
    int star = (x / (sizeHint().width()
                     / maxStarCount())) + 1;
    if (star <= 0 || star > maxStarCount())
        return -1;

    return star;
}
void StarRating::setStarCount(int starCount)
{
    if(myStarCount != starCount) emit starCountChanged(starCount);
    myStarCount = starCount;
    update();
}
