#ifndef SIMPLEMULTILANGUAGEGUESSER_H
#define SIMPLEMULTILANGUAGEGUESSER_H

#include "multilanguageguesser.h"
#include "languageguesser.h"

class SimpleMultiLanguageGuesser : public MultiLanguageGuesser
{
public:
    // after the call this takes over ownership of lg
    SimpleMultiLanguageGuesser(LanguageGuesser *lg);
    virtual ~SimpleMultiLanguageGuesser();

    virtual QList<QPair<Language,QString> > guessLanguages(const QString &text);

private:
    LanguageGuesser *languageGuesser;
};

#endif // SIMPLEMULTILANGUAGEGUESSER_H
