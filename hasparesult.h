#ifndef HASPARESULT_H
#define HASPARESULT_H

#include <QWidget>
#include "haspa.h"
#include <QGraphicsView>

namespace Ui {
class hasparesult;
}

class hasparesult : public QWidget
{
    Q_OBJECT

public:
    explicit hasparesult(QWidget *parent = nullptr,int tasknum=0);
//    void showEvent(QShowEvent *event);
    void showEvent(QShowEvent *event);

    haspa hsp;
    void setHaspa(const haspa& hs) {
        hsp = hs;
    }
    ~hasparesult();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::hasparesult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;
};

#endif // HASPARESULT_H
