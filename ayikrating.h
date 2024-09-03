#ifndef AYIKRATING_H
#define AYIKRATING_H

#include <QtGui>
#include "starrating.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>

class AyikRating : public QWidget
{
public:
    AyikRating(bool chk,bool rateable,int min,int max);

    QString getRating();

private:
    QHBoxLayout* layout;
    StarRating* rating;
    QCheckBox* checkBox;
};

#endif // AYIKRATING_H
