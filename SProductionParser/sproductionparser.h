#ifndef SPRODUCTIONPARSER_H
#define SPRODUCTIONPARSER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>


enum ExpressionType { VAR = 0, OBJ, MAKEOBJ, MAKEPARTOF, FUNCTION, IF, IF_BOOL, IF_THEN, IF_ELSE, NON };

struct Expression
{
    ExpressionType type;
    QVector<QString> data;

    Expression(){}
    Expression( ExpressionType type, const QVector<QString>& data )
    {
        this->type = type;
        this->data = data;
    }
};

class SProductionParser
{
public:
    SProductionParser();

    static ExpressionType getExpressionType( const QString& exp )
    {

        if( exp.startsWith( "obj" ) && exp.contains(" ") )
            return MAKEOBJ;

        if( exp.startsWith( "partof" ) && exp.contains(" ") )
            return MAKEPARTOF;

        if( exp.startsWith("if") && exp.contains(" : "))
            return IF;

        if( exp.at(0) == '$' && exp.at(1).isDigit() && !exp.contains(" ") )
            return VAR;

        if( exp.at(0).isLetter() && exp.at(0).isUpper() && !exp.contains(" ") )
            return OBJ;

        if( exp.at(0).isLetter() && exp.at(0).isLower() && exp.contains(" ") )
            return FUNCTION;

        return NON;
    }

    static Expression parseLine( const QString& exp )
    {
        Expression result( NON, QVector<QString>() );

        ExpressionType type = getExpressionType( exp );

        int aaa = 5;

        if( type == VAR )
        {
            result = Expression(type, { exp });
            qDebug() << "VAR";
        }

        if( type == OBJ )
        {
            QStringList lst = exp.split(" ");
            if( lst.size() > 1 )
            {
                QVector<QString> lstV = lst.toVector();
                lstV.remove(0);
                result = Expression( type, lstV );
            }
            qDebug() << "OBJ";
        }

        if( type == IF )
        {
            QStringList lst = exp.split(" : ");
            if( lst.size() > 2 )
            {
                QVector<QString> params;
                for( int i = 1; i < lst.size(); i++ )
                    params.push_back( lst.at(i) );

                result = Expression( type, params );
            }
            qDebug() << "IF";
        }

        if( type == FUNCTION )
        {
            QStringList lst = exp.split(" ");
            if( lst.size() > 1 )
            {
                QVector<QString> params;
                for( int i = 0; i < lst.size(); i++ )
                    params.push_back( lst.at(i) );

                result = Expression( type, params );
            }
            qDebug() << "FUNCTION";
        }
        else
        {
            result = Expression( type, QVector<QString>() );
            qDebug() << "NON";
        }

        return result;
    }

    static QVector<Expression> parseParams( ExpressionType type, const QString& exp )
    {
        QVector<Expression> result;

        if( type == IF_BOOL || type == IF_THEN || type == IF_ELSE )
        {
            QStringList lst = exp.split(",");

            for( int i = 0; i < lst.size(); i++ )
            {
                QStringList data = lst.at(i).split(" ");
                result.push_back( Expression( getExpressionType( lst.at(i) ), data.toVector() ) );
            }
        }

        return result;
    }
};

#endif // SPRODUCTIONPARSER_H
