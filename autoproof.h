#ifndef AUTOPROOF_H
#define AUTOPROOF_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

namespace Ui {
class AutoProof;
}

class TrangleView : public QLabel
{
protected:
    virtual void paintEvent(QPaintEvent* e)
    {
        QWidget::paintEvent(e);

        QPainter p(this);

        p.setPen(Qt::green);
        p.drawLine(25,25,25,150);
        p.drawLine(25,25,100,150);
        p.drawLine(100,150,25,150);

        p.setPen(Qt::yellow);
        p.drawLine(100,150,225,150);
        p.drawLine(225,150,225,75);
        p.drawLine(100,150,225,75);

        p.setPen(Qt::red);
        p.drawLine(25,25,225,75);
        p.drawLine(26,24,101,149);
        p.drawLine(99,149,224,74);

        p.setPen(Qt::blue);
        p.drawText(20,165, "A");
        p.drawText(20,20, "B");
        p.drawText(235,75, "C");
        p.drawText(235,165, "D");
        p.drawText(100,165, "E");


    }
};

class AutoProof : public QWidget
{
    Q_OBJECT

public:
    explicit AutoProof(QWidget *parent = 0);
    ~AutoProof();

private slots:
    void on_bSend_clicked();
    void viewTriangle();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::AutoProof *ui;
};

#endif // AUTOPROOF_H
