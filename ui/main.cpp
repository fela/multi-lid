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


#include <QApplication>

#include "languagedetectorapp.h"
#include "../lib/simplemultilanguageguesser.h"
#include "../lib/trigramlanguageguesser.h"

//int main(/*int argc, char *argv[]*/)
//{
//    test();
////    QString text;
////
////    cout << "> ";
////    cout << flush;
////    //cin >> text;
////    text = cin.readLine();
////    cout << ">"+text+"<";
////
////    GuessLanguage gl;
////    gl.setLimits(10, 1);
////
////    QStringList languages = gl.identify(text);
////
////    cout << "Lingue trovate " << languages.size() << ":\n";
////    foreach(QString s, languages) {
////        cout << s << "\n";
////    }
////    cout << endl;
//}

 int main(int argc, char *argv[]) {
     QApplication app(argc, argv);

     TrigramLanguageGuesser *lg = new TrigramLanguageGuesser;
     SimpleMultiLanguageGuesser *mlg = new SimpleMultiLanguageGuesser(lg);

     LanguageDetectorApp *window = new LanguageDetectorApp(mlg);
     window->show();

     return app.exec();
 }
