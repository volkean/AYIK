#include "ayikrating.h"

ayikRating::ayikRating(bool chk,bool rateable,int min,int max)
{
    layout = new QHBoxLayout(this);
    if(chk) {
        chkbox = new QCheckBox();
        layout->addWidget(chkbox);
    }
    rating = new starrating(min,max);
    rating->setEditable(rateable);
    layout->addWidget(rating);
    layout->addStretch();
}

QString ayikRating::getRating()
{
    int starcount = rating->starCount();
    return QString::number(starcount);
}
