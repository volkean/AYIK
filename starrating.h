#ifndef STARRATING_H
#define STARRATING_H

#include <QtWidgets/QWidget>

class starrating : public QWidget
{
    Q_OBJECT

public:
    enum EditMode { Editable, ReadOnly };

    starrating(QWidget *parent = 0);
    starrating(int starCount = 1, int maxStarCount = 5);

    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette, EditMode mode) const;
    QSize sizeHint() const;
    int starCount() const { return myStarCount; }
    int maxStarCount() const { return myMaxStarCount; }
    void setStarCount(int starCount);
    void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }
    void setEditable(bool editable=true) { mode = editable ? Editable : ReadOnly; }

signals:
    void starCountChanged(int starCount);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    int starAtPosition(int x);

    EditMode mode;
    QPolygonF starPolygon;
    QPolygonF diamondPolygon;
    int myStarCount;
    int myMaxStarCount;


};

#endif // STARRATING_H
