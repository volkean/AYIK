#include "loadingwidget.h"

#include <QtGui>
#include <math.h>

const int PaintingScaleFactor = 1;

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent)
{
    for(int i=0;i<50;i+=1) {
        rects << QRect(i,0,2,10);
    }
    counter=0;
    connect(&timer,SIGNAL(timeout()),this,SLOT(slot_repaint()));
}
QSize LoadingWidget::sizeHint() const
{
    return PaintingScaleFactor * QSize(10, 10);
}
void LoadingWidget::paintEvent(QPaintEvent *evt)
{
    counter = (counter + 1)%rects.size();
    QPainter painter(this);
    paint(&painter, evt->rect(), this->palette());
}
void LoadingWidget::paint(QPainter *painter, const QRect &rect, const QPalette &palette) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(QColor(59,89,152)));
    painter->setBrush(palette.highlight());
    painter->translate(rect.x(), rect.y());
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);
    for(int i=0;i<counter;i++)
        painter->drawRect(rects.at(i));

    //painter->translate(1.0, 0.0);

    painter->restore();
}
void LoadingWidget::slot_repaint()
{
    repaint();
}
void LoadingWidget::setVisible(bool visible)
{
    if(visible==false) {
        timer.stop();
        counter=0;
    } else {
        timer.start(100);
    }
    QWidget::setVisible(visible);
}
