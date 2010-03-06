#include "languagestring.h"

#include <QPair>
#include <QMap>

#include <QTextStream>

#include <cmath>

GuessLanguage *LanguageString::_dont_use_directly_languageGuesser = 0;

QString LanguageString::subString(int lenghtInWords, int start) {
    if (start < 0) {
        start = qrand() % text.length();
    }
    int end = start + lenghtInWords;



    if (end <= text.length()) {
        QStringList res = text.mid(start, lenghtInWords);
        return res.join(" ");
    } else {
        QStringList res1 = text.mid(start, text.length()-start);
        QStringList res2 = text.mid(0, end % text.length());
        return (res1 + res2).join(" ");
    }
}



void LanguageString::test() {

    // for each string lenght maps the number of strings tested
    QMap<int, int> triesMap;
    // for each string length maps the number of correct test
    QMap<int, int> correctMap;

    // the following length variable indicates the length in words
    for (int length = text.length() / 3; length > 0; --length) {
        for (int i = 0; i < text.length(); ++i) {
            QString substr = subString(length, i);
            if (substr.length() < 3) continue;
            QList<QPair<Language,double> > guessedLanguages =
                    languageGuesser()->identify(substr);
            triesMap[substr.size()] += 1;
            if (!guessedLanguages.empty()) {
                correctMap[substr.size()] +=
                        Language(guessedLanguages.first().first) == language;
            }
       }
    }

    //QTextStream cin(stdin);
    QTextStream cout(stdout);

    cout << "Testing language " << language << "\n";

    //int max = triesMap.keys().last();
    int max = 71;
    int tries = 0;
    int correct = 0;
    int startingFrom = 3;
    for (int i = 3; i <= max; ++i) {
        tries += triesMap.value(i);
        correct += correctMap.value(i);

        if ((i-2)%3 == 0 || i == max) {
            // every 3 iterations and at last iteration
            cout << startingFrom << "-" << i << ": " << correct << "/"
                 << tries << "(" << double(correct) / tries * 100 << "%)\n";
            startingFrom = i+1;
            tries = 0;
            correct = 0;
        }
    }

//    foreach (int size, triesMap.keys()) {
//        int tries = triesMap.value(size);
//        int correct = correctMap.value(size);
//        cout << size << ": " << correct << "/" << tries << "("
//             << double(correct) / tries * 100 << "%)\n";
//    }
    cout << endl;
}

GuessLanguage *LanguageString::languageGuesser() {
    if (_dont_use_directly_languageGuesser == 0) {
        _dont_use_directly_languageGuesser = new GuessLanguage();
    }

    return _dont_use_directly_languageGuesser;
}
