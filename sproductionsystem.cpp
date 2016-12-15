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
    QString data = ui->pteCode->toPlainText();
    QString rules = ui->plainTextEdit->toPlainText();

    SProductionKernel sys(data, rules);
    sys.debugData();
    sys.proccess();
    sys.debugData();
}
