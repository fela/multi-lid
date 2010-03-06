#ifndef LANGUAGEDETECTORAPP_H
#define LANGUAGEDETECTORAPP_H

#include <QMainWindow>

#include <QColor>
#include <QMap>

#include "../lib/multilanguageguesser.h"

class QTextEdit;
class QListWidget;
class QPushButton;

class LanguageDetectorApp : public QMainWindow
{
    Q_OBJECT
public:
    // this gets the ownership of mlg
    LanguageDetectorApp(MultiLanguageGuesser *mlg);
    virtual ~LanguageDetectorApp();

private:
    MultiLanguageGuesser *languageGuesser;

    QTextEdit *edit;
    QListWidget *list;
    QPushButton *button;

    QMap<Language, QColor> colorMap;

    // auxiliary functions
    void initText();
    QColor randomColor();
    void updateColorMap(const QList<QPair<Language,QString> > &languagesMap);
    void updateList();

private slots:
    void updateLanguages();
    void resetFormatting();
};

#endif // LANGUAGEDETECTORAPP_H
