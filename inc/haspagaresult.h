#ifndef HASPAGARESULT_H
#define HASPAGARESULT_H

#include <QWidget>
#include <QGraphicsView>
#include "hspa_ga.h"
namespace Ui {
class haspagaresult;
}

class haspagaresult : public QWidget
{
    Q_OBJECT

public:
    explicit haspagaresult(QWidget *parent = nullptr,int tasknum=0);
    ~haspagaresult();
    hspa_ga hsp;
    void showEvent(QShowEvent *event);
    void setHaspa(const hspa_ga& hs) {
        hsp = hs;
    }
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::haspagaresult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;
};

#endif // HASPAGARESULT_H
