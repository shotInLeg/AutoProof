#include "sproductionsystem.h"
#include "ui_sproductionsystem.h"

SProductionSystem::SProductionSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SProductionSystem)
{
    ui->setupUi(this);
}

SProductionSystem::~SProductionSystem()
{
    delete ui;
}

void SProductionSystem::on_bDo_clicked()
{

}
