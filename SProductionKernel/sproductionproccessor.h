#ifndef SPRODUCTIONPROCCESSOR_H
#define SPRODUCTIONPROCCESSOR_H


#include <QString>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QDebug>

#include "sproductionobjects.h"

struct TreeViewItem
{
    QString name;
    QVector<QString> links;
};

class SProductionProccessor
{
public:
    SProductionProccessor(){}

    void addObject( const QString& exp )
    {
        QStringList params = exp.split(" ");

        if( params.size() == 2 )
        {
            if( params.at(0) == "Obj" && !isObjectCreated( params.at(1) ) )
                _objects.push_back( new SPKObject( "OBJ" + QString::number(_objects.size()), params.at(1) ) );
        }

        if( params.size() == 3 )
        {
            //Для переменных
            if( params.at(0).startsWith("$") && params.at(1) == "=" )
            {
                QString right = "";

                if( params.at(2).contains(":") )
                {
                    right = execFunction( params.at(2) );
                }
                else
                {
                    right = params.at(2);
                }

                if( right != "" )
                    _vars[ params.at(0) ] = right;

                return;
            }

            //int a = 0;
            SPKLinkType type = SPKLink::getType( params.at(0) );
            SPKObject * objFrom = NULL;
            SPKObject * objTo = NULL;


            QString nameFrom = "";
            QString nameTo = "";

            if( params.at(1).startsWith("$") )
                nameFrom = _vars[ params.at(1) ];
            else
                nameFrom = params.at(1);

            if( params.at(2).startsWith("$") )
                nameTo = _vars[ params.at(2) ];
            else
                nameTo = params.at(2);

            for( int i = 0; i < _objects.size(); i++ )
            {
                if( _objects.at(i)->name() == nameFrom )
                    objFrom = _objects.at(i);

                if( _objects.at(i)->name() == nameTo )
                    objTo = _objects.at(i);
            }

            if( objFrom != NULL && objTo != NULL && objFrom != objTo )
            {
                objFrom->addLink( new SPKLink( type, objTo ) );
            }
        }
    }

    void addRule( const QString& exp )
    {
        QStringList params = exp.split(" : ");

        QVector<QString> conds = params.at(1).split(", ").toVector();
        QVector<QString> actions = params.at(2).split(", ").toVector();

        _rules.push_back( new SPKRule( "RULE" + QString::number( _rules.size() ), conds, actions ) );
    }

    void proccess()
    {
        int i = 0;
        while( i < _rules.size() )
        {
            QVector<SPKObject*> copyObjects = _objects;
            SPKRule * rule = _rules.at(i);

            bool doSomeThing = true;
            while( doSomeThing )
            {
                QMap<QString, QString> objUsed;
                QVector<QString> variants;

                for( int j = 0; j < rule->conditional().size(); j++ )
                {
                    for( int k = 0; k < copyObjects.size(); k++ )
                    {
                        SPKObject * object = copyObjects.at(k);

                        if( isHaveLink( object, rule->conditional().at(j) ) )
                        {
                            variants.push_back( getHaveLink( object, rule->conditional().at(j) ) );
                            objUsed[ "%" + QString::number( objUsed.size()+1 ) ] = object->name();

                            //Некрасиво придумать способ лучше
                            copyObjects.remove( k );
                            break;
                        }
                    }
                }

                if( rule->conditional().size() == variants.size() )
                {
                    for( int j = 0; j < rule->actions().size(); j++ )
                    {
                        QString act = rule->actions().at(j);
                        for( auto it = objUsed.begin(); it != objUsed.end(); it++ )
                        {
                            act = act.replace( it.key(), it.value() );
                        }

                        addObject( act );
                        doSomeThing = true;
                    }
                }
                else
                {
                    doSomeThing = false;
                }
            }

            i++;
        }
    }

    QVector<TreeViewItem> treeView() const
    {
        QVector<TreeViewItem> res;

        for( int i = 0; i < _objects.size(); i++ )
        {
            TreeViewItem item;

            item.name = _objects.at(i)->name();

            for( int j = 0; j < _objects.at(i)->links().size(); j++ )
            {
                QString linkText = "[" + SPKLink::getType( _objects.at(i)->links().at(j)->type() ) + "] " + _objects.at(i)->links().at(j)->object()->name();
                item.links.push_back( linkText );
            }

            res.push_back( item );
        }

        return res;
    }

    void view() const
    {
        qDebug() << "------DEBUG DATA----------";
        for( int i = 0; i < _objects.size(); i++ )
        {
            _objects.at(i)->view();
        }
        qDebug() << "---------------------";


        qDebug() << "------DEBUG VARS----------";
        for( auto it = _vars.begin(); it != _vars.end(); it++ )
        {
            qDebug() << "   " + it.key() + " = " + it.value();
        }
        qDebug() << "---------------------";


        qDebug() << "------DEBUG RULES----------";
        for( int i = 0; i < _rules.size(); i++ )
        {
            _rules.at(i)->view();
        }
        qDebug() << "---------------------";

    }

private:
    bool isConditionalSuccess( int size, const QVector< QVector<QString> >& variants )
    {
        int count = 0;
        for( int i = 0; i < variants.size(); i++ )
        {
            if( variants.at(i).size() >= 1 )
                count++;
        }

        return size == count;
    }

    QString execFunction( const QString& func )
    {
        QStringList funcList = func.split(":");

        if( funcList.size() == 2 )
        {
            QString funcName = funcList.at(0);
            QString objName = "";

            if( funcList.at(1).startsWith("$") )
            {
                objName = _vars[ funcList.at(1) ];
            }
            else
            {
                objName = funcList.at(1);
            }


            for( int i = 0; i < _objects.size(); i++ )
            {
                if( _objects.at(i)->name() == objName )
                {
                    SPKObject * object = _objects.at(i);
                    for( int j = 0; j < object->links().size(); j++ )
                    {
                        if( SPKLink::getType( object->links().at(j)->type() ) == funcName )
                        {
                            return object->links().at(j)->object()->name();
                        }
                    }
                }
            }
        }

        return "";
    }

    bool isObjectCreated( const QString& name )
    {
        for( int i = 0; i < _objects.size(); i++ )
        {
            if( _objects.at(i)->name() == name )
                return true;
        }

        return false;
    }

    bool isHaveLink( SPKObject * obj, const QString& sigLink )
    {
        QStringList condParams = sigLink.split("|");

        for( int i = 0; i < obj->links().size() && condParams.size() == 3; i++ )
        {
            QString sigNew = "-1";
            QString linkSig = "-2";

            if( condParams.at(0) == "*" )
            {
                sigNew = "|" + condParams.at(1) + "|" + condParams.at(2);
                linkSig = signatureExpFrom( obj, i );
            }
            else if( condParams.at(1) == "*" )
            {
                sigNew = condParams.at(0) + "||" + condParams.at(2);
                linkSig = signatureExpLink( obj, i );
            }
            else if( condParams.at(2) == "*" )
            {
                sigNew = condParams.at(0) + "|" + condParams.at(1) + "|";
                linkSig = signatureExpTo( obj, i );
            }
            else
            {
                sigNew = condParams.at(0) + "|" + condParams.at(1) + "|" + condParams.at(2);
                linkSig = signatureFull( obj, i );
            }

            if( sigNew == linkSig )
                return true;
        }

        return false;
    }

    QString getHaveLink( SPKObject * obj, const QString& sigLink )
    {
        QStringList condParams = sigLink.split("|");

        for( int i = 0; i < obj->links().size() && condParams.size() == 3; i++ )
        {
            QString sigNew = "-1";
            QString linkSig = "-2";

            if( condParams.at(0) == "*" )
            {
                sigNew = "|" + condParams.at(1) + "|" + condParams.at(2);
                linkSig = signatureExpFrom( obj, i );
            }
            else if( condParams.at(1) == "*" )
            {
                sigNew = condParams.at(0) + "||" + condParams.at(2);
                linkSig = signatureExpLink( obj, i );
            }
            else if( condParams.at(2) == "*" )
            {
                sigNew = condParams.at(0) + "|" + condParams.at(1) + "|";
                linkSig = signatureExpTo( obj, i );
            }

            if( sigNew == linkSig )
                return signatureFull( obj, i );
        }

        return false;
    }

    QString getSignatureOperation( const QString& name, const QVector<QString>& signatures )
    {
        QString sig = name + ":";

        for( int i = 0; i < signatures.size(); i++ )
        {
            sig += "(" + signatures.at(i) + ")";
        }

        return sig;
    }

    bool isSignatureOperation(  const QString& signature, const QVector<QString>& signatures  )
    {
        for( int i = 0; i < signatures.size(); i++ )
        {
            if( signature == signatures.at(i) )
            {
                return true;
            }
        }

        return false;
    }

    QString signatureExpFrom( SPKObject * obj, int num ) const
    {
        if( num < obj->links().size() )
            return "|" + SPKLink::getType( obj->links().at(num)->type() ) + "|" + obj->links().at(num)->object()->name();

        return "";
    }

    QString signatureExpLink( SPKObject * obj, int num ) const
    {
        if( num < obj->links().size() )
            return obj->name() + "||" + obj->links().at(num)->object()->name();

        return "";
    }

    QString signatureExpTo( SPKObject * obj, int num ) const
    {
        if( num < obj->links().size() )
            return obj->name() + "|" + SPKLink::getType( obj->links().at(num)->type() ) + "|";

        return "";
    }

    QString signatureFull( SPKObject * obj, int num ) const
    {
        if( num < obj->links().size() )
            return obj->name() + "|" + SPKLink::getType( obj->links().at(num)->type() ) + "|" + obj->links().at(num)->object()->name();

        return "";
    }

protected:
    QVector< SPKObject* > _objects;
    QVector< SPKRule* > _rules;
    QMap<QString, QString> _vars;
};

#endif // SPRODUCTIONPROCCESSOR_H
