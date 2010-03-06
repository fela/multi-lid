#include "trigramlanguageguesser.h"

#include <QStringList>
#include <QFile>
#include <QMap>

// static
QHash< QString, QHash<QString,int> > TrigramLanguageGuesser::models;

QStringList TrigramLanguageGuesser::BASIC_LATIN;
QStringList TrigramLanguageGuesser::EXTENDED_LATIN;
QStringList TrigramLanguageGuesser::ALL_LATIN;

TrigramLanguageGuesser::TrigramLanguageGuesser()
{
    BASIC_LATIN << "en"  << "ha" << "so"  << "id" << "la" << "sw" << "eu" <<
            "nr" << "nso" << "zu" << "xh" << "ss" << "st" << "tn" << "ts";
    EXTENDED_LATIN << "cs" << "af" << "pl" << "hr" << "ro" << "sk" << "sl" <<
            "tr" << "hu" << "az" << "et" << "sq" << "ca" << "es" << "fr" <<
            "de" << "nl" << "it" << "da" << "is" << "nb" << "sv" << "fi" <<
            "lv" << "pt" << "ve" << "lt" << "tl" << "cy" ;
    ALL_LATIN << BASIC_LATIN << EXTENDED_LATIN;

        if (models.isEmpty())
        loadModels();
}

void TrigramLanguageGuesser::loadModels()
{
    QString triMapFile = "Trigrams.map";

    QFile sin(triMapFile);
    if ( ! sin.open(QIODevice::ReadOnly) )
        return;

    QDataStream in(&sin);
    in >> models;
}


QList<QPair<Language,double> > TrigramLanguageGuesser::guessLanguage(
        const QString &text)
{
     if (text.isEmpty())
        return QList<QPair<Language,double> >();

    return guessLanguage(text, ALL_LATIN);
}

QList<QPair<Language,double> > TrigramLanguageGuesser::guessLanguage(
        const QString &text, const QStringList &languages)
{
    int const MIN_LENGTH = 3;
    QList<QPair<Language,double> > res;
    if (text.size() < MIN_LENGTH)
        return res;

    QMap<int,QString> scores;
    QList<QString> model = createModel(text);

    foreach (QString key, languages)
    {
        QString lkey = key.toLower();

        if ( models.contains(lkey) )
        {
            scores.insertMulti(distance(model, models[lkey]), key);
        }
    }

    if (scores.isEmpty())
        return res;

    double tot = 0;
    foreach (int score, scores.keys()) {
        foreach (Language l, scores.values(score)) {
            // not normalized confidence
            double confidence = (text.size()-2) * 300 - score;
            if (confidence > 0) {
                res << QPair<Language,double>(l, confidence);
                tot += confidence;
            }
        }
    }

    // normalize confidence (sum of all confidences should be 1)
    for (int i = 0; i < res.size(); ++i) {
        res[i].second /= tot;
    }

    return res;
}

QList<QString>TrigramLanguageGuesser::createModel(const QString& content)
{
    QHash<QString,int> trigrams;
    QMap<int,QString> oTrigrams; // ordered trigrams

    for (int i = 0; i < (content.size() - 2); ++i)
    {
        QString tri = content.mid(i, 3).toLower();
        trigrams[tri]+=1;
    }

    QStringList keys = trigrams.keys();

    foreach (const QString& key, keys)
    {
        const QChar* data=key.constData();
        bool hasTwoSpaces=(data[1].isSpace() &&
                           (data[0].isSpace() || data[2].isSpace()));

        if (!hasTwoSpaces) oTrigrams.insertMulti(-trigrams[key], key);
    }

    return oTrigrams.values();
}

int TrigramLanguageGuesser::distance( const QList<QString>& model,
                                    const QHash<QString,int>& knownModel )
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

