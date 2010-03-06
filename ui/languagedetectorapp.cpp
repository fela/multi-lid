#include <QGridLayout>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextCharFormat>

#include "languagedetectorapp.h"

LanguageDetectorApp::LanguageDetectorApp(MultiLanguageGuesser *mlg)
        :languageGuesser(mlg)
{
    setCentralWidget(new QWidget);

    list = new QListWidget;
    edit = new QTextEdit;
    button = new QPushButton("&Update");

    list->setMaximumWidth(100);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(edit, 0, 0, 2, 1);
    layout->addWidget(list, 0, 1);
    layout->addWidget(button, 1, 1);
    layout->setColumnStretch(0, 1);

    centralWidget()->setLayout(layout);
    setGeometry(0, 0, 700, 400);

    connect(button, SIGNAL(clicked()), SLOT(updateLanguages()));
    connect(edit, SIGNAL(cursorPositionChanged()), SLOT(resetFormatting()));

    initText();
}

LanguageDetectorApp::~LanguageDetectorApp()
{
    delete languageGuesser;
}

void LanguageDetectorApp::initText()
{
    const char *s = "Le lingue del mondo, esito ciascuna di uno sviluppo "
                "storico in una data regione del mondo, si chiamano lingue "
                "storico-naturali. Storiche perché hanno una storia nella quale"
                " sono protagonisti i parlanti di tali lingue, naturali per "
                "contrapporle alle lingue artificiali rispetto alle quali esse "
                "solitamente hanno maggiore complessità (esistono tuttavia "
                "delle lingue artificiali ben più complesse di alcune lingue "
                "storico-naturali). A set of commonly accepted signs (indices, "
                "icons or symbols) is only one feature of language; all "
                "languages must define (1) the structural relationships between"
                " these signs in a system of grammar, (2) the context wherein "
                "the signs are used (pragmatics) and (3) dependent on their "
                "context the content specificity, i.e. its meaning (semantics)."
                " Rules of grammar are one of the characteristics sometimes "
                "said to distinguish language from other forms of "
                "communication. They allow a finite set of signs to be "
                "manipulated to create a potentially infinite number of "
                "grammatical utterances. However, this definition is "
                "self-circular. The structural relationships make sense only "
                "within language; the structure of language exists only in "
                "language. It is impossible to have a logically correct "
                "definition of a noun or verb. And logic itself concerns itself"
                " with propositions which are closely linked with content "
                "specificity i.e. semantics. Hoewel het taalvermogen, inclusief"
                " het begrijpen en kunnen toepassen van regels van grammatica "
                "en syntaxis, uniek lijkt voor mensen, is het moeilijk te "
                "doorgronden volgens welk scenario dit zich in de loop van de "
                "evolutie heeft ontwikkeld. Grofweg staan hier twee standpunten"
                 " tegenover elkaar (zie verder het artikel over glottogonie).";
    edit->insertPlainText(QString::fromUtf8(s));
}

void LanguageDetectorApp::updateLanguages()
{
    QString text = edit->toPlainText();
    edit->clear(); // everything is rewritten

    QList<QPair<Language,QString> >
            languages = languageGuesser->guessLanguages(text);

    updateColorMap(languages);

    for (int i = 0; i < languages.size(); ++i) {
        Language language = languages[i].first;
        QString s = languages[i].second;

        QTextCharFormat format;
        format.setBackground(colorMap[language]);

        edit->setCurrentCharFormat(format);
        edit->insertPlainText(s + " ");
    }
    // delete last empty space
    edit->textCursor().deletePreviousChar();
}

void LanguageDetectorApp::resetFormatting()
{
    edit->setCurrentCharFormat(QTextCharFormat());
}

QColor LanguageDetectorApp::randomColor()
{
    int r = qrand() % 100 + 155;
    int g = qrand() % 100 + 155;
    int b = qrand() % 100 + 155;
    return QColor(r, g, b);
}

void LanguageDetectorApp::updateColorMap(
        const QList<QPair<Language,QString> > &languagesMap)
{
    colorMap.clear();

    QPair<Language,QString> p;
    foreach (p, languagesMap) {
        Language l = p.first;
        if (!colorMap.contains(l)) {
            colorMap.insert(l, randomColor());
        }
    }

    updateList();
}

void LanguageDetectorApp::updateList()
{
    list->clear();
    foreach (Language l, colorMap.keys()) {
        QListWidgetItem *item = new QListWidgetItem(l);
        item->setBackground(colorMap[l]);
        list->addItem(item);
    }
}
