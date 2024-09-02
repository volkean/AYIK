#include "ayikword.h"

ayikWord::ayikWord()
{
}
ayikWord::ayikWord(const QString& name,const QString& meaning,const QString& rating,const QString& tag)
{
    this->name=name;
    this->meaning=meaning;
    this->rating=rating;
    this->tag=tag;
}

QString ayikWord::getName() const
{
    return name;
}
QString ayikWord::getMeaning() const
{
    return meaning;
}
QString ayikWord::getRating() const
{
    return rating;
}
QString ayikWord::getTag() const
{
    return tag;
}
void ayikWord::setName(const QString& name)
{
    this->name=name;
}
void ayikWord::setMeaning(const QString& meaning)
{
    this->meaning=meaning;
}
void ayikWord::setRating(const QString& rating)
{
    this->rating=rating;
}
void ayikWord::setTag(const QString& tag)
{
    this->tag=tag;
}

