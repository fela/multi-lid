#ifndef MULTILANGUAGEGUESSER_H
#define MULTILANGUAGEGUESSER_H

#include <QList>
#include <QPair>

#include "language.h"

class MultiLanguageGuesser
{
public:
    virtual QList<QPair<Language,QString> >
            guessLanguages(const QString &text)=0;
};

#endif // MULTILANGUAGEGUESSER_H
