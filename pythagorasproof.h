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
    using Rules = QVector< QPair< QVector<QString>, QVector<QString> > >;

    PythagorasProof(){}
    ~PythagorasProof()
    {}

private slots:

signals:
    void send_info( QString );

public:

    void addData( const QString& data )
    {
        this->data.push_back( data );
    }

    void addRule(const QVector<QString>& _if, const QVector<QString>& _then )
    {
        this->rules.push_back( { _if, _then } );
    }

    void proof()
    {
        Rules _rules = rules;
        QVector< QString > _data = data;

        int i = 0;
        while( i < _rules.size() )
        {
            bool check = false;
            for( int j = 0; j < _rules.at(i).first.size(); j++ )
            {
                check = false;
                for( int k = 0; k < _data.size(); k++ )
                {
                    if( _rules.at(i).first.at(j) == data.at(k)  )
                    {
                        check = true;
                        break;
                    }
                }

                if( check != true )
                    break;
            }

            if( check == true )
            {
                for( int j = 0; j < _rules.at(i).second.size(); j++ )
                {
                    _data.push_back( _rules.at(i).second.at(j) );
                }

                rules.remove( i );

                i = 0;
            }
        }
    }

private:
    Rules rules;
    QVector< QString > data;
};

#endif // PYTHAGORASPROOF_H
