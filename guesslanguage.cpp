/*  This file is part of the KDE libraries
    Copyright (c) 2006 Jacob R Rideout <kde@jacobrideout.net>
    Copyright (c) 2009 Jakub Stachowski <qbast@go2.pl>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTime>
#include <QHash>
#include <QMap>
#include <QStringList>
#include <QTextStream>

#include <QDebug>

#include "guesslanguage.h"

/*
All language tags should be valid according to IETF BCP 47, as codefied in RFC 4646.
ISO 639-1 codes should be used for the language part except for cases where there
exists no code, then 639-3 codes should be used. Country codes should only be used
in special cases. Scripts can be differentiated by IANA subtags, availble here:
http://www.iana.org/assignments/language-subtag-registry
The script tags corresond to ISO 15924

An overview of the best practices concerning language tagging is available here:
http://www.w3.org/International/articles/language-tags/Overview.en.php

lang tags should use underscores (_) rather than hypens (-) to sepereate subsections.

EXCEPTIONS:
For cases of known differences from the above tagging scheme and major
spellcheckers such aspell/hunspell/myspell, the scheme used by the spell checkers
shall be used. All exception shall be noted here:

BCP        SPELLCHECK
az-Latn    az


*/

//K_GLOBAL_STATIC_WITH_ARGS(Sonnet::UnicodeData, blockData, ("Blocks.map") )


namespace Sonnet
{



class GuessLanguagePrivate
{
public:
    GuessLanguagePrivate();

    static QHash< QString, QHash<QString,int> > models;

    void load_models( );
    QStringList find_runs(const QString & text);
    QList<QString> createOrderedModel(const QString& content);
    int distance( const QList<QString>& model, const QHash<QString,int>& knownModel );
    QList<QPair<Language,double> > check(const QString & sample, const QStringList& langs);
    QStringList identify(const QString& sample, const QStringList& scripts);

    static QStringList BASIC_LATIN;
    static QStringList EXTENDED_LATIN;
    static QStringList ALL_LATIN;
    static QStringList CYRILLIC;
    static QStringList ARABIC;
    static QStringList DEVANAGARI;
    static QStringList SINGLETONS;
    static QStringList PT;

    //UnicodeData* ucd;

    const int MIN_LENGTH;
    int maxItems;
    double minConfidence;
};

QHash< QString, QHash<QString,int> > GuessLanguagePrivate::models;
QStringList GuessLanguagePrivate::BASIC_LATIN;
QStringList GuessLanguagePrivate::EXTENDED_LATIN;
QStringList GuessLanguagePrivate::ALL_LATIN;
QStringList GuessLanguagePrivate::CYRILLIC;
QStringList GuessLanguagePrivate::ARABIC;
QStringList GuessLanguagePrivate::DEVANAGARI;
QStringList GuessLanguagePrivate::SINGLETONS;
QStringList GuessLanguagePrivate::PT;

GuessLanguagePrivate::GuessLanguagePrivate()
        : MIN_LENGTH(3), maxItems(1), minConfidence(0)
{
    //ucd = blockData;

    if (!BASIC_LATIN.isEmpty())
        return;

    BASIC_LATIN << "en"  << "ha" << "so"  << "id" << "la" << "sw" << "eu" << "nr" << "nso" << "zu" << "xh" << "ss" << "st" << "tn" << "ts";

    EXTENDED_LATIN << "cs" << "af" << "pl" << "hr" << "ro" << "sk" << "sl" << "tr" << "hu" << "az" << "et" << "sq" << "ca" << "es" << "fr" << "de" << "nl" << "it" << "da" << "is" << "nb" << "sv" << "fi" << "lv" << "pt" << "ve" << "lt" << "tl" << "cy" ;

    ALL_LATIN << BASIC_LATIN << EXTENDED_LATIN;

    CYRILLIC << "ru" << "uk" << "kk" << "uz" << "mn" << "sr" << "mk" << "bg" << "ky";

    ARABIC << "ar" << "fa" << "ps" << "ur";

    DEVANAGARI << "hi" << "ne";

    // NOTE mn appears twice, once for mongolian script and once for CYRILLIC
    SINGLETONS << "Armenian" << "hy" << "Hebrew" << "he" << "Bengali" << "bn" << "Gurmukhi" << "pa" << "Greek" << "el" << "Gujarati" << "gu" << "Oriya" << "or" << "Tamil" << "ta" << "Telugu" << "te" << "Kannada" << "kn" << "Malayalam" << "ml" << "Sinhala"  << "si" << "Thai" << "th" << "Lao" << "lo" << "Tibetan" << "bo" << "Burmese" << "my" << "Georgian" << "ka" << "Mongolian" << "mn" << "Khmer" << "km";

    PT << "pt_BR" << "pt_PT";

    if (models.isEmpty())
        load_models();
}

GuessLanguage::GuessLanguage() :
        d(new GuessLanguagePrivate)
{
}

GuessLanguage::~GuessLanguage()
{
    delete d;
}

QList<QPair<Language,double> > GuessLanguage::identify(const QString & text) const
{

    if (text.isEmpty())
        return QList<QPair<Language,double> >();

    //return d->identify( text, d->find_runs(text) );
    return d->check(text, d->ALL_LATIN);
}

void GuessLanguage::setLimits(int n, double dx)
{
    d->maxItems=n;
    d->minConfidence=dx;
}

void GuessLanguagePrivate::load_models()
{

    QTime t;
    t.start();
    //QString triMapFile =  KStandardDirs::locate( "data", "sonnet/unicode/maps/Trigrams.map" );
    QString triMapFile = "Trigrams.map";

    QFile sin(triMapFile);
    if ( ! sin.open(QIODevice::ReadOnly) )
        return;

    QDataStream in(&sin);
    in >> models;
    //QTextStream(stdout) << "Loading took " << t.elapsed() << " ms, " << models.size() << endl;
}

/*QStringList GuessLanguagePrivate::find_runs(const QString & text)
{

    return QStringList("Basic Latin");
    const QChar * c  = text.constData();
    qint8 prevBlock = -1;
    QHash<QString,int> run_types;

    int count = 0;
    int totalCount = 0;
    QString blockName;

    while (!c->isNull())
    {
        qint8 block = ucd->categoryCode(*c);
        if (c->isLetter())
        {
            ++count;
            ++totalCount;
            if ( prevBlock != block )
            {
                if ( ucd->categoryName(block)=="Latin-1 Supplement" ||
                        ucd->categoryName(block)=="Latin Extended-A" ||
                        ucd->categoryName(block)=="IPA Extensions"
                   )
                {
                    block = ucd->categoryCode("Latin Extended-A");
                    blockName = "Extended Latin";
                }
                else
                    blockName = ucd->categoryName(block);

                prevBlock = block;

                run_types[blockName] += count;
                count = 0;
            }
        }
        ++c;
    }
    // add last count
    run_types[blockName] += count;

    QStringList relevant_runs;
    if (!totalCount) return relevant_runs;

    // return run types that used for 40% or more of the string
    // always return basic latin if found more than 15%.
    QHash<QString,int>::iterator i;
    for (i = run_types.begin(); i != run_types.end(); ++i)
    {
        if ( ( (i.value()*100) / totalCount ) >=40 )
            relevant_runs << i.key();
        else if (i.key() == "Basic Latin" && ( ( (i.value()*100) / totalCount ) >=15 ) )
            relevant_runs << i.key();
    }

    return relevant_runs;
}*/

/*QStringList GuessLanguagePrivate::identify(const QString& sample, const QStringList& scripts)
{
    if (sample.size() < 3)
        return QStringList();

    if ( scripts.contains("Hangul Syllables") ||
            scripts.contains("Hangul Jamo") ||
            scripts.contains("Hangul Compatibility Jamo") ||
            scripts.contains("Hangul"))
        return QStringList("ko");

    if ( scripts.contains("Greek and Coptic") )
        return QStringList("el");

    if ( scripts.contains("Katakana") ||
            scripts.contains("Hiragana") ||
            scripts.contains("Katakana Phonetic Extensions"))
        return QStringList("ja");

    if ( scripts.contains("CJK Unified Ideographs") ||
            scripts.contains("Bopomofo") ||
            scripts.contains("Bopomofo Extended") ||
            scripts.contains("KangXi Radicals") ||
            scripts.contains("Arabic Presentation Forms-A") )
        return QStringList("zh");

    if ( scripts.contains("Cyrillic") )
        return check( sample, CYRILLIC );

    if ( scripts.contains("Arabic") ||
            scripts.contains("Arabic Presentation Forms-A") ||
            scripts.contains("Arabic Presentation Forms-B"))
        return check( sample, ARABIC );

    if ( scripts.contains("Devanagari") )
        return check( sample, DEVANAGARI );


    // Try languages with unique scripts
    QList<QString>::iterator s;
    for (s = SINGLETONS.begin(); s != SINGLETONS.end(); s+=2)
    {
        if ( scripts.contains(*s) )
            return QStringList(*(s+1));
    }

    if ( scripts.contains("Latin Extended Additional") )
        return QStringList("vi");

    if ( scripts.contains("Extended Latin") )
    {
        QStringList latinLang = check( sample, EXTENDED_LATIN );
        //FIXME
//        if (latinLang.first == "pt")
//            return check(sample, PT);
//        else
            return latinLang;
    }
    if ( scripts.contains("Basic Latin") )
        return check( sample, ALL_LATIN );

    return QStringList();
}*/

QList<QPair<Language,double> > GuessLanguagePrivate::check(const QString & sample, const QStringList& langs)
{
    QList<QPair<Language,double> > res;
    if (sample.size() < MIN_LENGTH)
        return res;

    QMap<int,QString> scores;
    QList<QString> model = createOrderedModel(sample);

    foreach ( QString key, langs )
    {
        QString lkey = key.toLower();

        //qDebug() << "modelkey:" << lkey;
        if ( models.contains(lkey) )
        {
            scores.insertMulti( distance( model, models[lkey] ), key);
        } else {
            //qDebug() << "??????????????????" << langs.size();
        }
    }
            //qDebug() << "??????????????????" << scores.size();

    if ( scores.isEmpty() )
        return res;

    double tot = 0;
    foreach (int score, scores.keys()) {
        foreach (Language l, scores.values(score)) {
            // not normalized confidence
            double confidence = (sample.size()-2) * 300 - score;
            if (confidence > 0) {
                res << QPair<Language,double>(l, confidence);
                tot += confidence;
            }
        }
    }

    QPair<Language,double> pair;

    for (int i = 0; i < res.size(); ++i) {
        res[i].second /= tot;
    }

    //qDebug() << res << endl;


//    int counter=0;
//    double confidence=0;
//    QMapIterator<int,QString> it(scores);
//    it.next();
//    QString prev_item=it.value();
//    int prev_score=it.key();
//
//    while (it.hasNext() && counter<maxItems && confidence<=minConfidence) {
//        it.next();
//        counter++;
//        confidence += (it.key() - prev_score)/(double)it.key();
//        ret += prev_item;
//        //QTextStream(stdout) << "Adding " << prev_item << " at " << prev_score << " cc " << confidence << endl;
//        prev_item=it.value();
//        prev_score=it.key();
//    }
    return res;
}


QList<QString> GuessLanguagePrivate::createOrderedModel(const QString& content)
{
    QHash<QString,int> trigrams;
    QMap<int,QString> Otrigrams;

    for ( int i = 0; i < (content.size() - 2) ; ++i )
    {
        QString tri = content.mid( i, 3 ).toLower();
        trigrams[tri]+=1;
    }

    QStringList keys = trigrams.keys();

    foreach (const QString& key,keys)
    {
        const QChar* data=key.constData();
        bool hasTwoSpaces=(data[1].isSpace() && (data[0].isSpace() || data[2].isSpace()));

        if (!hasTwoSpaces) Otrigrams.insertMulti( - trigrams[key], key);
    }

    return Otrigrams.values();
}

int GuessLanguagePrivate::distance( const QList<QString>& model, const QHash<QString,int>& knownModel )
{
    int counter = -1;
    int dist = 0;

    static const int MAXGRAMS = 300;

    Q_FOREACH(const QString& trigram, model )
    {
        if ( knownModel.contains(trigram) )
            dist += qAbs( counter++ - knownModel.value(trigram) );
        else
            dist += MAXGRAMS;
        if (counter==(MAXGRAMS-1))
            break;
    }

    return dist;
}


}
