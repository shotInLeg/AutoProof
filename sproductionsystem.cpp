#include "sproductionsystem.h"
#include "ui_sproductionsystem.h"

SProductionSystem::SProductionSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SProductionSystem)
{
    ui->setupUi(this);
    proc = new SProductionProccessor;
}

SProductionSystem::~SProductionSystem()
{
    delete ui;
}

void SProductionSystem::on_pushButton_clicked()
{
    QString exp = ui->lineEdit->text();

    if( exp.startsWith("if") )
    {
        proc->addRule( exp );
    }
    else
    {
        proc->addObject( exp );
    }

    proc->proccess();

    ui->treeWidget->clear();
    viewTree( proc->treeView() );
}

void SProductionSystem::viewTree(const QVector<TreeViewItem> &tree)
{
    QTreeWidgetItem *lparentItem = new QTreeWidgetItem();
    lparentItem->setText(0, "Объекты и их связи");

    for(int i = 0; i < tree.size(); i++)
    {
        QTreeWidgetItem *object = new QTreeWidgetItem();
        object->setText(0, tree[i].name);

        lparentItem->addChild( object );

        for( int j = 0; j < tree.at(i).links.size(); j++ )
        {
            QTreeWidgetItem * child = new QTreeWidgetItem();
            child->setText(0, tree.at(i).links.at(j) );

            object->addChild( child );
        }
    }

    ui->treeWidget->addTopLevelItem( lparentItem );
}
