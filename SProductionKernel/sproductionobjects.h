#ifndef SPRODUCTIONOBJECTS_H
#define SPRODUCTIONOBJECTS_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

enum ObjType { OTNon = 0, OTSimple = 1, OTRule = 2 };
enum LinkType { LTNon = 0, LTPartOf = 1, LTDoOn, LTInProp, LTPair, LTLive, LTNotLive, LTIf};
const QVector<QString> LinkTypeStr = { "Non", "PartOf", "DoOn", "InProp", "Pair", "Live", "NotLive", "If" };
enum FuncType { FTNon = 0, FTPartOf = 1, FTDoOn, FTInProp, FTPair, FTLive, FTNotLive };

class SPKObject;

class SPKLink
{
public:
    SPKLink()
    {
        this->_type = LTNon;
    }

    SPKLink(  SPKObject * obj, LinkType type )
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
public:
    SPKLinkLink()
    {
        this->_type = LTIf;
    }

    SPKLinkLink( SPKObject * obj, LinkType )
    {
        this->_object = obj;
        this->_type = LTIf;
    }

    SPKLinkLink( SPKObject * obj, LinkType type, SPKObject * obj2 )
    {
        this->_object = obj;
        this->_type = type;
        this->_link = new SPKLink( obj2 ,type );
    }

    virtual SPKLink * link() const
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
        this->_type = OTSimple;
        this->_name = "Untitled";
    }

    SPKObject( const QString& name )
    {
        this->_name = name;
        this->_type = OTSimple;
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

    QVector< SPKLink* > links() const
    {
        return _link;
    }

    virtual void addLink( SPKObject* obj, LinkType linkType, SPKObject* obj2 = NULL  )
    {
        _link.push_back( new SPKLink(obj, linkType) );
    }

    virtual void addThenObject( const QString& )
    {}

    virtual QVector< QString> newObjects() const
    {
        return QVector<QString>();
    }

protected:
    ObjType _type;
    QString _name;
    QVector< SPKLink* > _link;
};

class SPKRule : public SPKObject
{
public:
    SPKRule()
    {
        this->_type = OTRule;
        this->_name = "Untitled";
    }

    SPKRule( const QString& name )
    {
        this->_name = name;
        this->_type = OTRule;
    }

    ~SPKRule()
    {
        qDebug() << "delete Object[" << this << "](" << _name << ")";
    }

    virtual void addLink( SPKObject* obj, LinkType linkType, SPKObject* obj2 = NULL )
    {
        _link.push_back( new SPKLinkLink( obj, linkType, obj2 ) );
    }

    virtual void addThenObject( const QString& action )
    {
        this->_then.push_back( action );
    }

    virtual QVector< QString > newObjects() const
    {
        return _then;
    }

protected:
    QVector<QString> _then;
};







#endif // SPRODUCTIONOBJECTS_H
