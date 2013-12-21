
#include <QDebug>
#include "module.h"

Module::Module()
{
}

QString &Module::id()
{
    if (module.contains("id"))
        return module["id"].toString();
    return QString("");
}

void Module::id(QString &id)
{
    module["id"] = id;

}

void Module::name(QString &name)
{
    module["name"] = name;
}

QString &Module::name()
{
    if (module.contains("name"))
        return module["name"].toString();
    return QString("");
}

QVariant Module::data(int role) const
{
    qDebug() << role;
//    if (role == Qt::UserRole + 10)
//        return module["files"].toArray();
//    else if (role == Qt::UserRole + 11)
//        return module["id"].toString();
//    else
    return QVariant(module["name"].toString());
}

void Module::setData(const QVariant &value, int role)
{
//    if (role == Qt::UserRole+10)
//        module["files"] = value.toJsonArray();
//    else if (role == Qt::UserRole + 11)
//        module["id"] = value.toString();
//    else
    module["name"] =  value.toString();

    qDebug() << module << QString(" testet");
}

int Module::type() const {
    return UserType;
}
QStandardItem *Module::clone() const
{
    Module* ret = new Module();
    ret->module["id"] = module["id"];
    ret->module["name"] = module["name"];
    return ret;
}
