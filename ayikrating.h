#ifndef AYIKRATING_H
#define AYIKRATING_H

#include <QtGui>
#include "starrating.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>

class ayikRating : public QWidget
{
public:
    ayikRating(bool chk,bool rateable,int min,int max);

    QString getRating();

private:
    QHBoxLayout* layout;
    starrating* rating;
    QCheckBox* chkbox;
    //QLabel* label;
};

#endif // AYIKRATING_H
