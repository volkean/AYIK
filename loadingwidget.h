#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QtGui>
#include <QtWidgets/QWidget>

class LoadingWidget : public QWidget
{
    Q_OBJECT

public:

    LoadingWidget(QWidget *parent = 0);

    void paint(QPainter *painter, const QRect &rect, const QPalette &palette) const;
    QSize sizeHint() const;
    void setVisible(bool visible);

protected:

    void paintEvent(QPaintEvent *event);

    QVector<QRect> rects;
    int counter;
    QTimer timer;

private slots:
    void slot_repaint();
};

#endif // LOADINGWIDGET_H
