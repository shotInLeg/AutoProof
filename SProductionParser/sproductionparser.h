#ifndef SPRODUCTIONPARSER_H
#define SPRODUCTIONPARSER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

#include "SProductionKernel/sproductionobjects.h"

enum ExpType { NON = 0, IF = 1, EACH, OBJ, VAR, PARTOF, DOON, INPROP, LIVE, NOTLIVE, MAKEOBJ, MAKEVAR, MAKEPARTOF, MAKEDOON, MAKEINPROP, MAKELIVE, MAKENOTLIVE };

class SProductionParser
{
public:
    SProductionParser();

    static QString getLinkType( LinkType type )
    {
        int number = (int)type;
        return LinkTypeStr.at(number);
    }

    static LinkType getLinkType( const QString& type )
    {
        for( int i = 0; i < LinkTypeStr.size(); i++ )
        {
            if( LinkTypeStr.at(i) == type )
                return (LinkType)i;
        }

        return LTNon;
    }

    static LinkType getLinkType( ExpType type )
    {
        switch (type)
        {
        case MAKEPARTOF:
            return LTPartOf;
            break;
        case MAKEDOON:
            return LTDoOn;
            break;
        case MAKEINPROP:
            return LTInProp;
            break;
        case MAKELIVE:
            return LTLive;
            break;
        case MAKENOTLIVE:
            return LTNotLive;
            break;
        default:
            return LTNon;
            break;
        }

        return LTNon;
    }

    static ExpType getExpressionType( const QString& exp )
    {

        if( exp.startsWith( "if" ) && exp.contains(" : ") )
        {
            QStringList _if = exp.split(" : ");

            if( _if.size() == 3 )
                return IF;
        }
        else if( exp.startsWith( "Each" ) && exp.contains(" ") )
        {
            return EACH;
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
                return MAKEPARTOF;
        }
        else if( exp.startsWith("DoOn") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return MAKEDOON;
        }
        else if( exp.startsWith("InProp") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return MAKEINPROP;
        }
        else if( exp.startsWith("Live") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return MAKELIVE;
        }
        else if( exp.startsWith("NotLive") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 3 )
                return MAKENOTLIVE;
        }
        else if( exp.startsWith("PartOf") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 2 )
                return PARTOF;
        }
        else if( exp.startsWith("DoOn") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 2 )
                return DOON;
        }
        else if( exp.startsWith("InProp") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 2 )
                return INPROP;
        }
        else if( exp.startsWith("Live") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 2 )
                return LIVE;
        }
        else if( exp.startsWith("NotLive") && exp.contains(" ") )
        {
            QStringList pattern = exp.split(" ");

            if( pattern.size() == 2 )
                return NOTLIVE;
        }

        return NON;
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

            propertyObj["name"] = "RULE";
            propertyObj["condition"] = sections.at(1);
            propertyObj["then"] = sections.at(2);
        }
        else if( type == OBJ )
        {
            propertyObj["name"] = exp;
        }
        else if( type == VAR )
        {
            propertyObj["name"] = exp;
        }
        else if( type == EACH )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 4 )
            {
                qDebug() << "ERROR AT PARSE EACH ON: " << exp;
                exit(-1);
            }

            propertyObj["type"] = sections.at(1);
            propertyObj[""] = sections.at(2);
            propertyObj["to"] = sections.at(3);
        }
        else if( type == MAKEOBJ )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE MAKEOBJ ON: " << exp;
                exit(-1);
            }

            propertyObj["name"] = sections.at(1);
        }
        else if( type == MAKEVAR )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE MAKE* ON: " << exp;
                exit(-1);
            }

            propertyObj["name"] = sections.at(1);
            propertyObj["value"] = sections.at(2);
        }
        else if( type == MAKEPARTOF || type == MAKEDOON || type == MAKEINPROP || type == MAKELIVE || type == MAKENOTLIVE )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 3 )
            {
                qDebug() << "ERROR AT PARSE MAKE* ON: " << exp;
                exit(-1);
            }

            propertyObj["from"] = sections.at(1);
            propertyObj["to"] = sections.at(2);
        }
        else if( type == PARTOF || type == DOON || type == INPROP || type == LIVE || type == NOTLIVE )
        {
            QStringList sections = exp.split(" ");

            if( sections.size() < 2 )
            {
                qDebug() << "ERROR AT PARSE * ON: " << exp;
                exit(-1);
            }

            propertyObj["from"] = sections.at(1);
            propertyObj["to"] = sections.at(2);
        }

        return propertyObj;
    }
};

#endif // SPRODUCTIONPARSER_H
