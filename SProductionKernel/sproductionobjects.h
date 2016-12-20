#ifndef SPRODUCTIONOBJECTS_H
#define SPRODUCTIONOBJECTS_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

enum ObjType { Non = 0, Simple = 1, Rule = 2 };
enum LinkType { Non = 0, PartOf = 1, DoOn, InProp, Pair, If};

class SPKLink
{
public:
    SPKLink()
    {
        this->_type = Non;
    }

    SPKLink( LinkType type, SPKObject * obj )
    {
        this->_type = type;
        this->_object = obj;
    }

    LinkType type() const
    {
        return this->_type;
    }

    virtual SPKObject * object() const
    {
        return _object;
    }

    virtual SPKLink * link() const
    {
        return NULL;
    }

protected:
    LinkType _type;
    SPKObject * _object;
};

class SPKLinkLink : public SPKLink
{
    SPKLinkLink()
    {
        this->_type = If;
    }

    SPKLinkLink( SPKObject * obj, LinkType )
    {
        this->_object = obj;
        this->_type = If;
    }

    SPKLinkLink( SPKObject * obj, LinkType type, SPKLink * link )
    {
        this->_object = obj;
        this->_type = type;
        this->_link = link;
    }

    virtual SPKObject * link() const
    {
        return _link;
    }

protected:
    SPKLink * _link;
};

class SPKObject
{
public:
    SPKObject()
    {
        this->_type = Simple;
        this->_name = "Untitled";
    }

    SPKObject( const QString& name )
    {
        this->_name = name;
        this->_type = Simple;
    }

    ~SPKObject()
    {
        qDebug() << "delete Object[" << this << "](" << _name << ")";
    }

    QString name() const
    {
        return _name;
    }

    ObjType type() const
    {
        return this->_type;
    }

    QVector< SPKLink > links() const
    {
        return _link;
    }

    virtual void addLink( SPKObject* obj, LinkType linkType, SPKObject* obj2 = NULL  )
    {
        _link.push_back( new SPKLink( obj, linkType ) );
    }

    virtual void addThenObject( SPKObject * )
    {}

    virtual QVector< SPKObject* > newObjects() const
    {
        return QVector< SPKObject* >();
    }

protected:
    ObjType _type;
    QString _name;
    QVector< SPKLink > _link;
};

class SPKRule : public SPKObject
{
public:
    SPKRule()
    {
        this->_type = If;
        this->_name = "Untitled";
    }

    SPKRule( const QString& name )
    {
        this->_name = name;
        this->_type = If;
    }

    ~SPKObject()
    {
        qDebug() << "delete Object[" << this << "](" << _name << ")";
    }

    virtual void addLink( SPKObject* obj, LinkType linkType, SPKObject* obj2 = NULL )
    {
        _link.push_back( new SPKLinkLink( obj, linkType, obj2 ) );
    }

    virtual void addThenObject( SPKObject * obj )
    {
        this->_then.push_back( obj );
    }

    virtual QVector< SPKObject* > newObjects() const
    {
        return _then;
    }

protected:
    QVector< SPKObject *> _then;
};







#endif // SPRODUCTIONOBJECTS_H
