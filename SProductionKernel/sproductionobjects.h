#ifndef SPRODUCTIONOBJECTS_H
#define SPRODUCTIONOBJECTS_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

enum ObjType { Obj = 0, PartOf = 1, DoOn = 2, InProperty = 3, InRelationship = 4 };

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
        qDebug() << "delete PartOf["<< this <<"]( " << _parent << " -> " << _subject << " )";
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

class SPKInRelationship : public SPKObject
{
public:
    SPKInRelationship()
    {
        this->_type = InRelationship;
    }

    SPKInRelationship( const QString& name, SPKObject * subject1, SPKObject * property, SPKObject * subject2 )
    {
        this->_name = name;
        this->_subject1 = subject1;
        this->_property = property;
        this->_subject2 = subject2;
    }

    ~SPKInRelationship()
    {
        qDebug() << "delete DoOn["<< this <<"]( " << _subject1 << " -> " << _property << " -> " << _subject2 << " )";
    }

    virtual SPKObject* object() const;
    virtual SPKObject* subject() const;

protected:
    SPKObject * _subject1;
    SPKObject * _property;
    SPKObject * _subject2;
};



#endif // SPRODUCTIONOBJECTS_H
