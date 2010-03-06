#ifndef LANGUAGESTRING_H
#define LANGUAGESTRING_H

#include <QStringList>

#include "../lib/languageguesser.h"

class LanguageString
{
public:
    LanguageString(const Language &language, const QString &text)
            : language(language), text(text.split(' ')) {}

    const Language language;
    const QStringList text; // the words


    QString subString(int lenghtInWords, int start = -1);

    void test();

private:
    // the following function should be used instead
    static GuessLanguage *_dont_use_directly_languageGuesser;

    static GuessLanguage *languageGuesser();
};

#endif // LANGUAGESTRING_H
