#include <QStringList>
#include <QMap>

#include <cmath>

#include "simplemultilanguageguesser.h"

SimpleMultiLanguageGuesser::SimpleMultiLanguageGuesser(LanguageGuesser *lg)
        :languageGuesser(lg)
{
}

SimpleMultiLanguageGuesser::~SimpleMultiLanguageGuesser()
{
    delete languageGuesser;
}


QList<QPair<Language, QString> > SimpleMultiLanguageGuesser::guessLanguages(
        const QString &text)
{
    QStringList words = text.split(' ');

    QList<QMap<Language,double> > wordsLanguages;

    // initialize
    for (int i = 0; i < words.size(); ++i) {
        wordsLanguages << QMap<Language,double>();
    }

    // for every word find its more likely language
    // and propagate it two the nearby words
    for (int i = 0; i < words.size(); ++i) {
        QString end;
        if (i < words.size()-1) {
            end = " " + words[i+1].mid(0, 1);
        }
        QString word = words[i] + end;
        QList<QPair<Language,double> > languagesOfWord =
                languageGuesser->guessLanguage(word);
        if (languagesOfWord.empty()) {
            continue;
        }
        // else

        // language and confidence
        QPair<Language,double> langConf;
        foreach (langConf, languagesOfWord) {
            Language lang = langConf.first;
            double confidence = langConf.second;
            for (int j = 0; j < words.size(); ++j) {
                int offset = i - j;
                if (offset < 0) {
                    offset *= -1;
                }

                // the propagation decreases the following amount at every step
                const double DECREASE = 0.9;
                double value = std::pow(DECREASE, offset);
                wordsLanguages[j][lang] += value * confidence;
            }
        }
    }

    // for each word find the highest rated language
    QList<QPair<Language,QString> > result;

    for (int i = 0; i < words.size(); ++i) {
        double maxRating = 0;
        Language language = NO_LANG;
        // languages of word i
        QMap<Language, double> languages = wordsLanguages[i];
        foreach (Language l, languages.keys()) {
            // rating of language l
            double rating = languages[l];
            if (rating > maxRating) {
                maxRating = rating;
                language = l;
            }
        }
        result << QPair<Language,QString>(language,words[i]);
    }

    return result;
}
