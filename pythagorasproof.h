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
        //Делаем копии дабы не портить память
        Rules _rules = rules;
        QVector< QString > _data = data;

        //Обходим список првил до тех пор
        //пока обход не даст ни одного действия
        int i = 0;
        while( i < _rules.size() )
        {
            //Обход списка условий в правиле
            bool check = false;
            for( int j = 0; j < _rules.at(i).first.size(); j++ )
            {
                //Обход списка данных
                check = false;
                for( int k = 0; k < _data.size(); k++ )
                {
                    if( _rules.at(i).first.at(j) == data.at(k)  )
                    {
                        check = true;
                        break;
                    }
                }

                //Выход из данного правила если
                //найдено не совпадение с одним из условий
                if( check != true )
                    break;
            }

            //Если условие полностью удовлетворено,
            //записываем "действие" в список данных,
            //удаляем првило (дабы не получить зацикливание)
            //и запускаем обход правил сначала
            if( check == true )
            {
                for( int j = 0; j < _rules.at(i).second.size(); j++ )
                {
                    _data.push_back( _rules.at(i).second.at(j) );
                }

                _rules.remove( i );

                i = 0;
            }
            else
            {
                i++;
            }

        }

        data = _data;
    }

    void view()
    {
        for( int i = data.size()-1; i >= 0; i-- )
        {
            emit send_info( data.at(i) );
        }
    }

private:

    //Каждое правило состоит из списка условий и спска действий
    Rules rules;
    QVector< QString > data;
};

#endif // PYTHAGORASPROOF_H
