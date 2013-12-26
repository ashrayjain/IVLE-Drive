#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include "parser.h"
#include <QDebug>

using namespace std;

void Parser::parse(string stringToParse) {
    cout<<stringToParse<<endl;
}

QJsonObject Parser::jsonify(QString stringToJsonify) {
    QJsonDocument d = QJsonDocument::fromJson(stringToJsonify.toUtf8());
    return d.object();
}
