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
    SProductionKernel( const QString& data, const QString& rules )
    {
        //Добавление данных
        QStringList dataList = data.split("\n");

        for( int i = 0; i < dataList.size(); i++ )
        {
            addData( dataList.at(i) );
        }

        //Добавление правил
        QStringList rulesList = rules.split("\n");

        for( int i = 0; i < rulesList.size(); i++ )
        {
            Expression exp = SProductionParser::parseLine( rulesList.at(i) );

            if( exp.type != IF )
                continue;

            this->rules.push_back( exp );
        }
    }

    void addResult( const QString& line )
    {
        _addData( this->result, line );
    }

    void addResult( const Expression& exp )
    {
        _addData( this->result, exp );
    }

    void addData( const QString& line )
    {
        _addData( this->data, line );
    }

    void proccess()
    {
        //Обходим все првила
        for( int i = 0; i < rules.size(); i++ )
        {
            Expression rule = rules.at(i);

            if( rule.data.size() == 2 )
            {
                QMap<QString, QString> tempData; //Для переменных $1 = MERY
                QMap<QString, bool> check; //Список условий и их стотяни выполнено/не выполнено

                QVector<Expression> if_bool = SProductionParser::parseParams( IF_BOOL, rule.data.at(0) );
                QVector<Expression> if_then = SProductionParser::parseParams( IF_THEN, rule.data.at(1) );

                //Имортируем данные в map переменных и в map условий
                for( int j = 0; j < if_bool.size(); j++ )
                {
                    if( if_bool.at(i).type == VAR && if_bool.at(i).data.size() > 2 )
                    {
                        tempData[ if_bool.at(i).data.at(0) ] = if_bool.at(i).data.at(2);
                        check[ if_bool.at(i).data.at(2) ] = false;
                    }
                    else if( if_bool.at(i).type == OBJ )
                    {
                        check[ if_bool.at(i).data.at(2) ] = false;
                    }
                }


                //Обходим список всех условий
                for( auto it = check.begin(); it != check.end(); it++ )
                {
                    //Ищем объект условия в секции данных
                    for( auto jt = data.begin(); jt != data.end(); jt++ )
                    {
                        //Сравниваем имя, сигнатуру, расширенную сигнатуру
                        if( it.key() == jt.key() || it.key() == (*jt)->signature() || it.key() == jt.key()+(*jt)->signature() )
                        {
                            *it = true;
                        }
                    }

                    //Ищем объек условия в секции результатов (для добавленных другими условиями объектов)
                    for( auto jt = result.begin(); jt != result.end(); jt++ )
                    {
                        if( it.key() == jt.key() || it.key() == (*jt)->signature() || it.key() == jt.key()+(*jt)->signature() )
                        {
                            *it = true;
                        }
                    }
                }

                //Проверяем все ли условия выполнены
                bool fullTrue = true;
                for( auto it = check.begin(); it != check.end(); it++ )
                {
                    if( *it == false )
                    {
                        fullTrue = false;
                        break;
                    }
                }

                //Если выполнен все, то добавляем все объекты их ветки ДА
                if( fullTrue )
                {
                    for( int j = 0; j < if_then.size(); j++ )
                    {
                        addResult( if_then.at(j) );
                    }
                }
            }
        }
    }

protected:
    void _addData( QMap<QString, SPKObject*>& d, const QString& line  )
    {
        Expression exp = SProductionParser::parseLine( line );

        if( exp.type == MAKEOBJ )
        {
            qDebug() << exp.data.size();
            //d[ exp.data.at(0) ] = new SPKObject();
        }

        if( exp.type == MAKEPARTOF )
        {
            SPKObject * parent = NULL;
            if( !this->data.contains( exp.data.at(0) ) && !this->result.contains( exp.data.at(0) ) )
            {
                parent = d[ exp.data.at(0) ] = new SPKObject();
            }
            else
            {
                if( this->data.contains( exp.data.at(0) ) )
                    parent = this->data[ exp.data.at(0) ];
                else if( this->result.contains( exp.data.at(0) ) )
                    parent = this->result[ exp.data.at(0) ];
            }

            if( !this->data.contains( exp.data.at(1) ) && !this->result.contains( exp.data.at(1) ) )
            {
                d[ exp.data.at(1) ] = new SPKPartOf( exp.data.at(1), parent, new SPKObject() );
            }
            else
            {
                SPKObject * child = NULL;
                if( this->data.contains( exp.data.at(1) ) )
                    child = this->data[ exp.data.at(1) ];
                else if( this->result.contains( exp.data.at(1) ) )
                    child = this->result[ exp.data.at(1) ];

                d[ exp.data.at(1) ] = new SPKPartOf( exp.data.at(1), parent, child );
            }
        }
    }

    void _addData( QMap<QString, SPKObject*>& d, const Expression& exp  )
    {
        if( exp.type == MAKEOBJ )
        {
            d[ exp.data.at(0) ] = new SPKObject();
        }
        else if( exp.type == MAKEPARTOF )
        {
            SPKObject * parent = NULL;
            if( !this->data.contains( exp.data.at(0) ) && !this->result.contains( exp.data.at(0) ) )
            {
                parent = d[ exp.data.at(0) ] = new SPKObject();
            }
            else
            {
                if( this->data.contains( exp.data.at(0) ) )
                    parent = this->data[ exp.data.at(0) ];
                else if( this->result.contains( exp.data.at(0) ) )
                    parent = this->result[ exp.data.at(0) ];
            }

            if( !this->data.contains( exp.data.at(1) ) && !this->result.contains( exp.data.at(1) ) )
            {
                d[ exp.data.at(1) ] = new SPKPartOf( exp.data.at(1), parent, new SPKObject() );
            }
            else
            {
                SPKObject * child = NULL;
                if( this->data.contains( exp.data.at(1) ) )
                    child = this->data[ exp.data.at(1) ];
                else if( this->result.contains( exp.data.at(1) ) )
                    child = this->result[ exp.data.at(1) ];

                d[ exp.data.at(1) ] = new SPKPartOf( exp.data.at(1), parent, child );
            }
        }
    }

public:
    void debugData() const
    {
        qDebug() << "-------DEBUG DATA--------";
        for( auto it = data.begin(); it != data.end(); it++ )
        {
            qDebug() << it.key() << " : " << (*it)->signature();
        }
        qDebug() << "-------DEBUG RESULT--------";
        for( auto it = data.begin(); it != data.end(); it++ )
        {
            qDebug() << it.key() << " : " << (*it)->signature();
        }
    }

private:
    QMap<QString, SPKObject*> data;
    QMap<QString, SPKObject*> result;
    QVector<Expression> rules;

protected:
};

#endif // SPRODUCTIONKERNEL_H
