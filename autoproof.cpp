#include "autoproof.h"
#include "ui_autoproof.h"

AutoProof::AutoProof(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoProof)
{
    ui->setupUi(this);
    triangle = new TrangleView(50, 75);
    triangle->setGeometry( triangle->geometry().x(), triangle->geometry().y(), 200, 200 );
    ui->widgetLayout->addWidget( triangle );

    proof = new PythagorasProof();

    connect( proof, SIGNAL( send_info(QString) ), this, SLOT( view(QString) ) );

    step = 1;
}

AutoProof::~AutoProof()
{
    delete ui;
}

void AutoProof::on_bSend_clicked()
{
    //repaint();
    //triangle->setStep( ++step );

    /*if( step == 2 )
        view( "Доставиваем еще один треугольник меняя местами размеры соторон." );
    if( step == 3 )
        view( "Достраиваем до трапеции." );

    if( step >= 4 )
        proof->proof( step-3 );*/

    QString text = ui->lineEdit->text();

    QStringList list = text.split("=");
    QStringList _if = list[0].split(",");
    QStringList _then = list[1].split(",");


    proof->addRule( _if.toVector(), _then.toVector() );

    ui->lineEdit->clear();
}

void AutoProof::viewTriangle()
{

}

void AutoProof::paintEvent(QPaintEvent * event)
{
}

void AutoProof::view(QString str)
{
    ui->teDesc->append( str );
}

void AutoProof::on_bRender_clicked()
{
    triangle->setABAE(ui->sbAB->value(), ui->sbAE->value());
    //triangle->repaint();
}

void AutoProof::on_bAuto_clicked()
{
    /*for( int  i = 0; i < 3; i++ )
    {
        triangle->setStep( ++step );

        if( step == 2 )
            view( "Доставиваем еще один треугольник меняя местами размеры соторон." );
        if( step == 3 )
            view( "Достраиваем до трапеции." );
    }*/


    QString text = ui->lineEdit->text();

    QStringList _data = text.split(",");


    for( int i = 0; i < _data.size(); i++ )
    {
        proof->addData( _data[i] );
    }

    ui->lineEdit->clear();
}

void AutoProof::on_pushButton_clicked()
{
    proof->proof();
    proof->view();
}
