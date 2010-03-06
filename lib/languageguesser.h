#ifndef LANGUAGEGUESSER_H
#define LANGUAGEGUESSER_H

#include <QPair>
#include <QList>

#include "language.h"

class LanguageGuesser
{
public:
    virtual QList<QPair<Language,double> > guessLanguage(const QString &text)=0;
};

#endif // LANGUAGEGUESSER_H
