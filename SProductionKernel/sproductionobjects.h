#ifndef SPRODUCTIONOBJECTS_H
#define SPRODUCTIONOBJECTS_H


#include <QString>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QDebug>

enum SPKLinkType { LNon, LPartOf, LMainOf, LDoOn, LInProp, LLive, LNotLive };
const QVector<QString> SPKLinkTypeName = { "Non", "PartOf", "MainOf", "DoOn", "InProp", "Live", "NotLive" };

class SPKLink;
class SPKObject;

class SPKLink
{
public:
    SPKLink(){}
    SPKLink( SPKLinkType type, SPKObject * obj )
    {
        this->_type = type;
        this->_object = obj;
    }

    SPKLinkType type() const
    {
        return _type;
    }

    SPKObject * object() const
    {
        return _object;
    }

    static QString getType( SPKLinkType type )
    {
        return SPKLinkTypeName.at( (int)type );
    }

    static SPKLinkType getType( const QString& type )
    {
        for( int  i = 0; i < SPKLinkTypeName.size(); i++ )
        {
            if( SPKLinkTypeName.at(i) == type )
            {
                return (SPKLinkType)i;
            }
        }

        return LNon;
    }

protected:
    SPKLinkType _type;
    SPKObject * _object;
};

class SPKObject
{
public:
    SPKObject(){}
    SPKObject( const QString& id, const QString& name )
    {
        this->_id = id;
        this->_name = name;
    }

    QString id() const
    {
        return _id;
    }

    QString name() const
    {
        return _name;
    }

    QVector< SPKLink* > links() const
    {
        return _links;
    }

    void addLink( SPKLink * link )
    {
        qDebug() << "DEBUG: " << this->name() << " -> " << link->object()->name();

        //Заглушка от зацикливания
        for( int i = 0; i < _links.size(); i++ )
        {
            if( signature(link) == signature( _links.at(i) ) )
                return;

            if( link->object()->name() == this->name() )
                return;
        }

        //Взаимное добавление связи
        _links.push_back( link );

        if( link->type() == LPartOf )
            link->object()->addLink( new SPKLink( LMainOf, this ) );
        else if( link->type() == LMainOf )
            link->object()->addLink( new SPKLink( LPartOf, this ) );
        else
            link->object()->addLink( new SPKLink( link->type(), this ) );


        //Запрос к родитем новосвязанного объекта запросить такую же свзяь
        //для упрощения поиска
        for( int i = 0; i < link->object()->links().size(); i++ )
        {
            if( link->object()->links().at(i)->type() == LPartOf )
            {
                link->object()->links().at(i)->object()->addLink( new SPKLink( link->type(), this ) );
            }
        }
    }

    void view() const
    {
        qDebug() << "id: " + _id + "; name: " + _name;

        for( int i = 0; i < _links.size(); i++ )
        {
            qDebug() << "     [ " + SPKLink::getType( _links.at(i)->type() ) + " ] " + _links.at(i)->object()->id() + " " + _links.at(i)->object()->name();
        }

        qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    }

private:
    QString signature( SPKLink * link ) const
    {
        return "|"+SPKLink::getType( link->type() ) + "|" + link->object()->name();
    }

protected:
    QString _id;
    QString _name;
    QVector< SPKLink* > _links;
};

class SPKRule
{
public:
    SPKRule() {}
    SPKRule( const QString& id )
    {
        this->_id = id;
    }
    SPKRule( const QString& id, const QVector<QString>& cond, const QVector<QString>& actions )
    {
        this->_id = id;
        this->_conditionals = cond;
        this->_actions = actions;
    }

    QString id() const
    {
        return _id;
    }

    QVector<QString> conditional() const
    {
        return _conditionals;
    }

    QVector<QString> actions() const
    {
        return _actions;
    }

    QString signature() const
    {
        QString sig = "";


        if( _conditionals.size() > 0 )
        {
            sig += _conditionals.at(0);
        }

        for( int i = 1; i < _conditionals.size(); i++ )
        {
            sig += "," +_conditionals.at(i);
        }

        return sig;
    }

    void addConditional( const QString& cond )
    {
        this->_conditionals.push_back( cond );
    }

    void addAction( const QString& action )
    {
        this->_actions.push_back( action );
    }

    void view() const
    {
        qDebug() << "id: " + _id;

        for( int i = 0; i < _conditionals.size(); i++ )
        {
            qDebug() << "     " + _conditionals.at(i);
        }

        qDebug() << "..................";

        for( int i = 0; i < _actions.size(); i++ )
        {
            qDebug() << "     " + _actions.at(i);
        }

        qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    }

protected:
    QString _id;
    QVector<QString> _conditionals;
    QVector<QString> _actions;
};

#endif // SPRODUCTIONOBJECTS_H
