#ifndef SPRODUCTIONSYSTEM_H
#define SPRODUCTIONSYSTEM_H

#include <QMainWindow>

#include "SProductionKernel/sproductionproccessor.h"
namespace Ui {
class SProductionSystem;
}

class SProductionSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit SProductionSystem(QWidget *parent = 0);
    ~SProductionSystem();

private slots:
    void on_pushButton_clicked();

    void viewTree( const QVector<TreeViewItem>& tree );

private:
    Ui::SProductionSystem *ui;
    SProductionProccessor * proc;
};

#endif // SPRODUCTIONSYSTEM_H
