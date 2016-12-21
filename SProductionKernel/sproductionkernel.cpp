#include "sproductionkernel.h"


void SProductionKernel::addObject(const QString &line)
{
    ExpType type = SProductionParser::getExpressionType( line );

    if( type = IF )
    {
        makeIf( line );
    }
    else if( type == MAKEOBJ )
    {
        makeObj( line );
    }
    else if( type == MAKEVAR )
    {
        makeVar( line );
    }
    else if( type == MAKEPARTOF || type == MAKEDOON ||  type == MAKEINPROP ||  type == MAKELIVE || type == MAKENOTLIVE )
    {
        makeLinks( line );
    }
}

void SProductionKernel::proccess()
{
    //Обходим все узлы графа объектов и ищем объект-условие
    for( int i = 0; i < data.size(); i++ )
    {
        if( data.at(i)->type() == OTRule )
        {
            SPKObject * rule = data.at(i);

            QVector<SPKLink*> ruleLink = rule->links(); //Условия объекта-услоивие
            QVector<bool> ruleStatus; //Вектор удовлетворительных условий
            QVector<QString> ruleThen = rule->newObjects(); //Действия объекта-условие

            //Обходим все связи объекта-условие
            for( int j = 0; j < ruleLink.size(); j++ )
            {
                SPKLink * findLink = ruleLink.at(j)->link();
                SPKObject * obj = ruleLink.at(j)->object();

                //Проверяем вы выполнениы ли условия объекта-условие
                for( int k = 0; k < obj->links().size(); k++ )
                {
                    //Прямая проверка
                    if( obj->links().at(k)->type() == findLink->type() &&  obj->links().at(k)->object() == findLink->object() )
                    {
                        ruleStatus.push_back( true );
                        break;
                    }

                    //Косвенная проверка, она же проверка наследования PartOf
                    if( obj->links().at(k)->type() == findLink->type() )
                    {
                        if( findPartOf( obj->links().at(k)->object(), findLink->object() ) )
                        {
                            ruleStatus.push_back( true );
                            break;
                        }
                    }
                }

                //Если количечтво удовлетворительных условий равно количеству условий в объекте-условие
                //то выаолняем действия из секции "Да"
                if( ruleStatus.size() == ruleLink.size() )
                {
                    //Обходим все действия
                    for( int j = 0; j < ruleThen.size(); j++ )
                    {
                        ExpType typeExp = SProductionParser::getExpressionType( ruleThen.at(i) );

                        //Если действие имеет тип EACH то выполняем его для всех с кем связан объект условие
                        //Аналог цикла foreach
                        if( typeExp == EACH )
                        {
                            QMap<QString, QString> property = SProductionParser::parse( ruleThen.at(i) );
                            QMap<QString, QString> propertyFunc = SProductionParser::parse( ruleThen.at(i) );

                            //Если объект является первым параметорм выражения
                            if( SProductionParser::getExpressionType( property["from"] ) == VAR )
                            {
                                for( int k = 0; k < ruleLink.size(); k++ )
                                {
                                    QString action = property["func"] + " " + ruleLink.at(k)->object()->name() + " " + property["to"];
                                    addObject( action );
                                }
                            }

                            //Если объект является вторым параметром выражения
                            if( SProductionParser::getExpressionType( property["to"] ) == VAR )
                            {
                                for( int k = 0; k < ruleLink.size(); k++ )
                                {
                                    QString action = property["func"] + " " + propertyFunc["from"] + " " + ruleLink.at(k)->object()->name();
                                    addObject( action );
                                }
                            }

                        }
                        else if( typeExp == NON )
                        {
                            continue;
                        }
                        else //Если объект опознан, но не является EACH просто выполняем его
                        {
                            addObject( ruleThen.at(i) );
                        }
                    }
                }
            }
        }
    }
}

bool SProductionKernel::findPartOf(SPKObject *obj, SPKObject *parent)
{
    QVector<SPKLink*> links = obj->links();
    for( int i = 0; i < links.size(); i++ )
    {
        if( links.at(i)->type() == LTPartOf && links.at(i)->object() == parent )
            return true;

        if( links.at(i)->type() == LTPartOf )
            return findPartOf( links.at(i)->object(), parent );
    }

    return false;
}

