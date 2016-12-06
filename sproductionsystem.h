#ifndef SPRODUCTIONSYSTEM_H
#define SPRODUCTIONSYSTEM_H

#include <QMainWindow>

namespace Ui {
class SProductionSystem;
}

class SProductionSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit SProductionSystem(QWidget *parent = 0);
    ~SProductionSystem();

private:
    Ui::SProductionSystem *ui;
};

#endif // SPRODUCTIONSYSTEM_H
