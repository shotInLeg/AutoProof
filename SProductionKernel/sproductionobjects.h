#ifndef SPRODUCTIONOBJECTS_H
#define SPRODUCTIONOBJECTS_H

#include <QString>
#include <QVector>
#include <QMap>

enum ObjType { Obj = 0, PartOf = 1, DoOn = 2, InProperty = 3 };

class SPKObject
{
public:
    SPKObject()
    {
        this->_type = Obj;
    }

    ~SPKObject()
    {
        qDebug() << "delete Object[" << this << "]";
    }

    QString name() const;
    QString data() const;


    void setName( const QString& name );
    void setData( const QString& data );

    virtual SPKObject* object() const
    { return NULL; }
    virtual SPKObject* subject() const
    { return NULL; }

protected:
    ObjType _type;
    QString _name;
    QString _data;
};

class SPKPartOf : public SPKObject
{
public:
    SPKPartOf(  )
    {
        this->_type = PartOf;
    }

    SPKPartOf( const QString& name, SPKObject * parent, SPKObject * subject )
    {
        this->_name = name;
        this->_parent = parent;
        this->_subject = subject;
    }

    ~SPKPartOf()
    {
        qDebug() << "delete PartOf["<< this <<"]( " << _parent << " -> " <<_subject << " )";
    }

    virtual SPKObject* subject() const;

protected:
    SPKObject * _parent;
    SPKObject * _subject;
};

class SPKDoOn : public SPKObject
{
public:
    SPKDoOn()
    {
        this->_type = DoOn;
    }

    SPKDoOn( const QString& name, SPKObject * doObj, SPKObject * subject )
    {
        this->_name = name;
        this->_doObj = doObj;
        this->_subject = subject;
    }

    ~SPKDoOn()
    {
        qDebug() << "delete DoOn["<< this <<"]( " << _doObj << " -> " << _subject << " )";
    }

    virtual SPKObject* object() const;
    virtual SPKObject* subject() const;

protected:
    SPKObject * _doObj;
    SPKObject * _subject;

};

class SPKInProperty : public SPKObject
{
public:
    SPKInProperty()
    {
        this->_type = InProperty;
    }

    SPKInProperty( const QString& name, SPKObject * property, SPKObject * subject )
    {
        this->_name = name;
        this->_property = property;
        this->_subject = subject;
    }

    ~SPKInProperty()
    {
        qDebug() << "delete DoOn["<< this <<"]( " << _property << " -> " << _subject << " )";
    }

    virtual SPKObject* object() const;
    virtual SPKObject* subject() const;

protected:
    SPKObject * _property;
    SPKObject * _subject;
};



#endif // SPRODUCTIONOBJECTS_H
