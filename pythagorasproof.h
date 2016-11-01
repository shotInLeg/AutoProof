#ifndef PYTHAGORASPROOF_H
#define PYTHAGORASPROOF_H

#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QDebug>


class PythagorasProof : public QObject
{
Q_OBJECT
public:
    PythagorasProof(){}
    ~PythagorasProof()
    {}

private slots:

signals:
    void send_info( QString );

public:

    void proof( int step )
    {
        if( step == 1 || step == -1 )
        {
            S1 = "2(AB*AE)+BE2";
            S2 = "DE*CD+AB*AE+CD*AE+AB*DE";

            emit send_info( "Имея треугольник ABE досторили его до трапеции, по следующим правилам:" );
            emit send_info( "   Сторону DE сделаем равной стороне AB" );
            emit send_info( "   Сторону CD сделаем равной стороне AE" );
        }

        if( step == 2 || step == -1 )
        {
            emit send_info( "Так как мы достраивали теругольник до трапеции, то сумма прощадей трех треугольников равна прощади трапеции." );
            emit send_info( "    " + S1 + " = " + S2 );
        }

        if( step == 3 || step == -1 )
        {
            emit send_info( "Поскольку стороны AB и DE, AE и CD равны проведем замену в формуле расчета площадей:" );

            S1 = S1.replace("DE", "AB").replace("CD", "AE");
            S2 = S2.replace("DE", "AB").replace("CD", "AE");

            emit send_info( "    " + S1 + " = " + S2 );
        }

        if( step == 4 || step == -1 )
        {
            emit send_info( "Приведем подобные:" );

            S2 = replaceMirror( S2 );

            emit send_info( "    " + S1 + " = " + S2 );
        }

        if( step == 5 || step == -1 )
        {
            emit send_info( "Привеподобные в левой и правой частях:" );

            QString res = replaceEqual( S1+"="+S2 );

            emit send_info( "    " + res );
        }
    }

    QString replaceMirror( const QString& s )
    {
        QString p[] = { "AB", "DE", "AE", "CD" };

        QString res = s;

        for( int i = 0 ; i < 4; i++ )
        {
            res = res.replace(p[i]+"*"+p[i], p[i]+"2" );
        }

        for( int i = 0 ; i < 4; i++ )
        {
            for( int j = 0; j < 4; j++ )
            {
                if( i == j )
                    continue;

                res = res.replace( p[i]+"*"+p[j]+"+"+p[i]+"*"+p[j], "2(" + p[i]+"*"+p[j] + ")" );
            }
        }

        for( int i = 0 ; i < 4; i++ )
        {
            res = res.replace(p[i]+"+"+p[i], "2" + p[i] );
        }

        return res;

    }

    QString replaceEqual( const QString& s )
    {
        QString res = "";

        QStringList equal = s.split("=");


        QStringList left = equal[0].split("+");
        QStringList right = equal[1].split("+");

        for( int i = 0; i < left.size(); i++ )
        {
            for( int j = 0; j < right.size(); j++ )
            {
                if( left[i] == right[j] )
                {
                    left[i] = "";
                    right[j] = "";
                }
            }
        }

        for( int i = 0; i < left.size(); i++ )
        {
            if( left[i] != "" )
                res += left[i];

            if( left[i] != "" && i != left.size()-1 )
                res += "+";
        }

        res += "=";

        for( int i = 0; i < right.size(); i++ )
        {
            if( right[i] != "" )
                res += right[i];

            if( right[i] != "" && i != right.size()-1 )
                res += "+";
        }


        return res;
    }

private:
    QString S1;
    QString S2;
};

#endif // PYTHAGORASPROOF_H
