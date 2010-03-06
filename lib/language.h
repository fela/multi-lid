#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>

typedef QString Language;

//class Language
//{
//public:
//    Language() {}
//    explicit Language(const QString &code) : _code(code) {}
//
//    bool operator==(const Language &other) {
//        return code == other.code;
//    }
//
//private:
//    QString _code;
//};
//
//enum LanguageEnum {
//    EN,
//    IT,
//    NL
//};

const Language EN("en");
const Language IT("it");
const Language NL("nl");
const Language NO_LANG("??");

#endif // LANGUAGE_H
