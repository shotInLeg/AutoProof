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

    step = 1;
}

AutoProof::~AutoProof()
{
    delete ui;
}

void AutoProof::on_bSend_clicked()
{
    //repaint();
    triangle->setStep( ++step );
}

void AutoProof::viewTriangle()
{

}

void AutoProof::paintEvent(QPaintEvent * event)
{
}

void AutoProof::on_bRender_clicked()
{
    triangle->setABAE(ui->sbAB->value(), ui->sbAE->value());
    //triangle->repaint();
}
