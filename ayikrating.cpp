#include "ayikrating.h"

AyikRating::AyikRating(bool chk,bool rateable,int min,int max)
{
    layout = new QHBoxLayout(this);
    if(chk) {
        checkBox = new QCheckBox();
        layout->addWidget(checkBox);
    }
    rating = new StarRating(min,max);
    rating->setEditable(rateable);
    layout->addWidget(rating);
    layout->addStretch();
}

QString AyikRating::getRating()
{
    int starCount = rating->starCount();
    return QString::number(starCount);
}
