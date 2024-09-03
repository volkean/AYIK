#include "ayikword.h"

AyikWord::AyikWord()
{
}
AyikWord::AyikWord(const QString& name,const QString& meaning,const QString& rating,const QString& tag)
{
    this->name=name;
    this->meaning=meaning;
    this->rating=rating;
    this->tag=tag;
}

QString AyikWord::getName() const
{
    return name;
}
QString AyikWord::getMeaning() const
{
    return meaning;
}
QString AyikWord::getRating() const
{
    return rating;
}
QString AyikWord::getTag() const
{
    return tag;
}
void AyikWord::setName(const QString& name)
{
    this->name=name;
}
void AyikWord::setMeaning(const QString& meaning)
{
    this->meaning=meaning;
}
void AyikWord::setRating(const QString& rating)
{
    this->rating=rating;
}
void AyikWord::setTag(const QString& tag)
{
    this->tag=tag;
}

