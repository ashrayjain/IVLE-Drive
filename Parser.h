#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <QObject>

class Parser {
public:
    static void parse(std::string stringToParse);
    static QJsonObject jsonify(QString stringToJsonify);
};

#endif // PARSER_H
