#include "autoproof.h"
#include "ui_autoproof.h"

AutoProof::AutoProof(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoProof)
{
    ui->setupUi(this);
    QLabel * tr = new TrangleView;
    tr->setGeometry( tr->geometry().x(), tr->geometry().y(), 200, 200 );
    ui->widgetLayout->addWidget( tr );
}

AutoProof::~AutoProof()
{
    delete ui;
}

void AutoProof::on_bSend_clicked()
{
    repaint();
}

void AutoProof::viewTriangle()
{

}

void AutoProof::paintEvent(QPaintEvent * event)
{


}
