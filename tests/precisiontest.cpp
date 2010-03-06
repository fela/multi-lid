/**
 * Copyright (C)  2009 Fela Winkelmolen <fela.kde@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include <QTextStream>
#include <QStringList>
#include <QMap>

#include "../lib/trigramlanguageguesser.h"

// execute the test with a given LanguageGuesser
void test(LanguageGuesser *lg);

// auxiliary function executes a test for a given language and a given string
void test(QString text, Language l, LanguageGuesser *lg);

// auxiliary function returns the wrapped substring
QString subString(QStringList text, int lenghtInWords, int start);




 int main(/*int argc, char *argv[]*/)
 {
     QTextStream(stdout) << "Testing TrigramLanguageGuesser\n";
     TrigramLanguageGuesser lg;
     test(&lg);

     return 0;
 }





 void test(LanguageGuesser *lg)
 {
    QString en = "This is a test string, let's see how well it "
        "will work. I will take random pieces of this phrase and look at what "
        "size the precision will start to degrade. We also hava sme typos. "
        "How is t going?";
    QString nl = "Laten we eens proberen hoe goed dit werkt. "
        "Nederlandse text is dit. Gaat het doet? Ik denk het wel. Misschien "
        "ook wt fauten schijven. Ik how van de lage landen. Pippo de clawn. "
        "we zullen zien";
    QString it = "Scriviamo un po' di testo di prova anche per "
        "l'italiano. Ciao come va? Bene mi chiamo Pippo. Non sono mai bravo "
        "ad inventarmi del testo da scrivere, infatti scrivo sempre cavolate. "
        "Ora nn devo dimenticarmi di scrivire testo con qualhe errore. Okay "
        "fatto";

    test(en, EN, lg);
    test(it, IT, lg);
    test(nl, NL, lg);
}

void test(QString text, Language language, LanguageGuesser *languageGuesser)
{
    QStringList words = text.split(' ');

    // for each string lenght maps the number of strings tested
    QMap<int, int> triesMap;
    // for each string length maps the number of correct test
    QMap<int, int> correctMap;

    // the following length variable indicates the length in words
    for (int length = words.length() / 3; length > 0; --length) {
        for (int i = 0; i < words.length(); ++i) {
            QString substr = subString(words, length, i);
            if (substr.length() < 3) continue;
            QList<QPair<Language,double> > guessedLanguages =
                    languageGuesser->guessLanguage(substr);
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


 QString subString(QStringList text, int lenghtInWords, int start)
 {
//    if (start < 0) {
//        start = qrand() % text.length();
//    }
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


