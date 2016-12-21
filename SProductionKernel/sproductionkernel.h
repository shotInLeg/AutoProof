#ifndef SPRODUCTIONKERNEL_H
#define SPRODUCTIONKERNEL_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

#include "sproductionobjects.h"
#include "SProductionParser/sproductionparser.h"

class SProductionKernel
{
public:
    SProductionKernel(){}

    void addObject( const QString& line );

    void proccess();

protected:

    //Проверяет не связан ли объект с другим объектом как на прямую,
    //так и через тех с кем он связан
    bool findPartOf( SPKObject * obj, SPKObject * parent )
    ;

    //Создает объект-условие по входной строке
    void makeIf( const QString& line )
    {
        QMap<QString, QString> property = SProductionParser::parse( line );
        SPKObject * newRule = new SPKRule( "RULE" + QString::number(data.size()) );

        QStringList conditions = property["condition"].split(", ");
        QStringList then = property["then"].split(", ");

        //Заполняем список условий
        for( int i = 0; i < conditions.size(); i++ )
        {
            QStringList condition = conditions.at(i).split("|");

            if( condition.size() == 3 )
            {
                QString fromCond = condition.at(0);
                QString typeCond = condition.at(1);

                QString toCond = condition.at(2);

                //Обходим весь список данных ищем совпадения объектов
                for( int j = 0; j < data.size(); j++ )
                {
                    SPKObject * from = NULL;
                    SPKObject * to = NULL;

                    if( data.at(i)->name() == fromCond )
                    {
                        from = data.at(i);
                    }

                    if( data.at(i)->name() == toCond )
                    {
                        to = data.at(i);
                    }

                    if( from != NULL && to != NULL )
                    {
                        LinkType linkType = SProductionParser::getLinkType( typeCond );
                        newRule->addLink( from, linkType, to );
                    }
                }
            }

            if( condition.size() == 2 )
            {
                QString typeCond = condition.at(0);
                QString toCond = condition.at(1);

                if( typeCond == "each" )
                {
                    //Обходим весь список данных ищем совпадения объектов
                    for( int j = 0; j < data.size(); j++ )
                    {
                        QVector<SPKLink*> links = data.at(i)->links();
                        for( int k = 0; k < links.size(); k++ )
                        {
                            if( links.at(k)->type() == LTPartOf )
                            {
                                if( links.at(k)->object()->name() == toCond )
                                    newRule->addLink( data.at(j), LTPartOf, links.at(k)->object() );

                            }
                        }
                    }
                }
            }

            //Заполняем список ветки "Да"
            for( int i = 0; i < then.size(); i++ )
            {
                ExpType typeAction = SProductionParser::getExpressionType( then.at(i) );

                if( typeAction == NON )
                    continue;

                newRule->addThenObject( then.at(i) );
            }

            data.push_back( newRule );
        }
    }

    //Создает объект
    void makeObj( const QString& line )
    {
        QMap<QString, QString> property = SProductionParser::parse( line );
        data.push_back( new SPKObject( property["name"] ) );
    }

    //Создает переменную
    void makeVar( const QString& line )
    {

    }

    //Создет связть между объектами
    void makeLinks( const QString& line )
    {
        QMap<QString, QString> property = SProductionParser::parse( line );
        for( int i = 0; i < data.size(); i++ )
        {
            SPKObject * child = NULL;
            SPKObject * parent = NULL;

            if( data.at(i)->name() == property["from"] )
            {
                child = data.at(i);
            }

            if( data.at(i)->name() == property["to"] )
            {
                parent = data.at(i);
            }

            if( child != NULL && parent != NULL )
            {
                LinkType linkType = SProductionParser::getLinkType( SProductionParser::getExpressionType( line ) );
                child->addLink( parent, linkType );
            }
        }
    }

public:
    void debugData() const
    {
        qDebug() << "-------DEBUG DATA--------";
        for( int i = 0; i < data.size(); i++ )
        {
            SPKObject * obj = data.at(i);
            qDebug() << obj->name();
            for( int j = 0; j < obj->links().size(); j++ )
            {
                qDebug() << "    " << SProductionParser::getLinkType( obj->links().at(j)->type() ) << " -> " << obj->links().at(j)->object()->name();
            }
            qDebug() << "~~~~~~~~~~~~~~~~~~~~";
        }
    }

private:
    QVector< SPKObject* > data;
    QMap<QString, SPKObject*> var;
};

#endif // SPRODUCTIONKERNEL_H
