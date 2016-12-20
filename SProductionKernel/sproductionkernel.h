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
    SProductionKernel();

    void addObject( const QString& line )
    {
        ExpType type = SProductionParser::getExpressionType( line );
        QMap<QString, QString> property = SProductionParser::parse( line );

        if( type = IF )
        {
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

                        if( data.at(i)->name() == from )
                        {
                            from = data.at(i);
                        }

                        if( data.at(i)->name() == to )
                        {
                            to = data.at(i);
                        }

                        if( child != NULL && parent != NULL )
                        {
                            LinkType linkType = Non;

                            if( typeCond == "PartOf" )
                                linkType = PartOf;
                            else if( typeCond == "DoOn" )
                                linkType = DoOn;
                            else if( typeCond == "InProp" )
                                linkType = InProp;
                            else if( typeCond == "Pair")
                                linkType = Pair;
                            else
                                linkType = Non;

                            newRule->addLink( from, linkType, to );
                        }
                    }
                }
            }

            //Заполняем список ветки "Да"
            for( int i = 0; i < then.size(); i++ )
            {
                ExpType typeAction = SProductionParser::getExpressionType( then.at(i) );

                if( typeAction == MAKEOBJ )
                {
                    QString name = then.at(i).split(" ").at(1);
                    newRule->addThenObject( new SPKObject( name ) );
                }
                else if( type == PARTOF || type == DOON ||  type == INPROP )
                {
                    QString fromName = then.at(i).split(" ").at(0);
                    QString toName = then.at(i).split(" ").at(2);

                    SPKObject * from = NULL;
                    SPKObject * to = NULL;

                    for( int j = 0; j < data.size(); j++ )
                    {
                        if( data.at(i)->name() == fromName )
                        {
                            child = data.at(i);
                        }

                        if( data.at(i)->name() == toName )
                        {
                            parent = data.at(i);
                        }
                    }

                    QVector<SPKObject*> thenIf = newRule->newObjects();
                    for( int j = 0; j < thenIf.size(); j++ )
                    {
                        if( thenIf.at(j)->name() == fromName )
                        {
                            child = thenIf.at(j);
                        }

                        if( thenIf.at(j)->name() == toName )
                        {
                            parent = thenIf.at(i);
                        }
                    }

                    if( child != NULL && parent != NULL )
                    {

                        LinkType linkType = Non;

                        switch (type)
                        {
                        case PARTOF:
                            linkType = PartOf;
                            break;
                        case DOON:
                            linkType = DoOn;
                            break;
                        case INPROP:
                            linkType = InProp;
                            break;
                        default:
                            linkType = Non;
                            break;
                        }

                        child->addLink( parent, linkType );
                    }


                }
            }

            data.push_back( newRule );
        }
        else if( type == OBJ )
        {
        }
        else if( type == VAR )
        {
        }
        else if( type == MAKEOBJ )
        {
            data.push_back( new SPKObject( property["name"] ) );
        }
        else if( type == MAKEVAR )
        {
            for( int i = 0; i < data.size(); i++ )
            {
                if( data.at(i)->name() == property["value"] )
                {
                    var[ property["name"] ] = data.at(i);
                    break;
                }
            }
        }
        else if( type == PARTOF || type == DOON ||  type == INPROP )
        {
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
                    LinkType linkType = Non;

                    switch (type)
                    {
                    case PARTOF:
                        linkType = PartOf;
                        break;
                    case DOON:
                        linkType = DoOn;
                        break;
                    case INPROP:
                        linkType = InProp;
                        break;
                    default:
                        linkType = Non;
                        break;
                    }

                    child->addLink( parent, linkType );
                }
            }
        }
    }

    void proccess()
    {
    }

protected:


public:
    void debugData() const
    {
        qDebug() << "-------DEBUG DATA--------";
        for( auto it = data.begin(); it != data.end(); it++ )
        {
            qDebug() << it.key() << " : " << (*it)->signature();
        }
    }

private:
    QVector< SPKObject* > data;
    QMap<QString, SPKObject*> var;
};

#endif // SPRODUCTIONKERNEL_H
