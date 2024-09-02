#ifndef AYIKWORD_H
#define AYIKWORD_H

#include <QtGui>

class ayikWord
{
public:
    ayikWord();    
    ayikWord(const QString& name,const QString& meaning,const QString& rating="5",const QString& tag="");//def. rating 5
    QString getName() const;
    QString getMeaning() const;
    QString getRating() const;
    QString getTag() const;
    void setName(const QString& name);
    void setMeaning(const QString& meaning);
    void setRating(const QString& rating);
    void setTag(const QString& tag);
private:
    QString name,meaning;
    QString rating;
    QString tag;

};

#endif // AYIKWORD_H
