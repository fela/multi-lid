#ifndef TRIGRAMLANGUAGEGUESSER_H
#define TRIGRAMLANGUAGEGUESSER_H

#include <QHash>

#include "languageguesser.h"

class TrigramLanguageGuesser : public LanguageGuesser
{
public:
    TrigramLanguageGuesser();

    virtual QList<QPair<Language,double> > guessLanguage(const QString &text);

private:
    void loadModels();

    virtual QList<QPair<Language,double> > guessLanguage(
            const QString &text, const QStringList &languages);

    QList<QString> createModel(const QString& content);

    int distance(const QList<QString>& model,
                 const QHash<QString,int>& knownModel);

    static QStringList BASIC_LATIN;
    static QStringList EXTENDED_LATIN;
    static QStringList ALL_LATIN;

    static QHash< QString, QHash<QString,int> > models;
};

#endif // TRIGRAMLANGUAGEGUESSER_H
