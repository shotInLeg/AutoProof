#ifndef SPRODUCTIONPARSER_H
#define SPRODUCTIONPARSER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

#include "SProductionKernel/sproductionobjects.h"

enum ExpType { NON = 0, IF = 1, OBJ, VAR, MAKEOBJ, MAKEVAR, PARTOF, DOON, INPROP };

class SProductionParser
{
public:
    SProductionParser();

    static ExpType getExpressionType( const QString& exp )
    {

        if( exp.startsWith( "if" ) && exp.contains(" : ") )
        {
            QStringList _if = exp.split(" : ");

            if( _if.size() == 3 )
                return IF;
            else
                return NON;
        }
        else if( exp.at(0) != '$' && !exp.contains(" ") )
        {
            return VAR;
        }
        else if( exp.at(0) == '$' && !exp.contains(" ") )
        {
            return VAR;
        }
        else if( exp.startsWith("Obj") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 2 )
                return MAKEOBJ;
        }
        else if( exp.startsWith("Var") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return MAKEVAR;
        }
        else if( exp.startsWith("PartOf") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return PARTOF;
        }
        else if( exp.startsWith("DoOn") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return DOON;
        }
        else if( exp.startsWith("InProp") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return INPROP;
        }

        return Non;
    }

    static QMap<QString, QString> parse( const QString& exp )
    {
        QMap<QString, QString> propertyObj;

        ExpType type = getExpressionType( exp );

        if( type = IF )
        {
            QStringList sections = exp.split(" : ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE IF ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = "IF";
            propertyObj["name"] = "RULE";
            propertyObj["condition"] = sections.at(1);
            propertyObj["then"] = sections.at(2);
        }
        else if( type == OBJ )
        {
            propertyObj["type"] = "OBJ";
            propertyObj["name"] = exp;
        }
        else if( type == VAR )
        {
            propertyObj["type"] = "VAR";
            propertyObj["name"] = exp;
        }
        else if( type == MAKEOBJ )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE MAKEOBJ ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = "MAKEOBJ";
            propertyObj["name"] = sections.at(1);
        }
        else if( type == MAKEVAR )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE MAKEVAR ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = "MAKEVAR";
            propertyObj["name"] = sections.at(1);
            propertyObj["value"] = sections.at(2);
        }
        else if( type == PARTOF )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE PARTOF ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = "PARTOF";
            propertyObj["from"] = sections.at(1);
            propertyObj["to"] = sections.at(2);
        }
        else if( type == DOON )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE DOON ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = "DOON";
            propertyObj["from"] = sections.at(1);
            propertyObj["to"] = sections.at(2);
        }
        else if( type == INPROP )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE INPROP ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = "INPROP";
            propertyObj["from"] = sections.at(1);
            propertyObj["to"] = sections.at(2);
        }

        return propertyObj;
    }
};

#endif // SPRODUCTIONPARSER_H
