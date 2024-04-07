#ifndef HASPACPRESULT_H
#define HASPACPRESULT_H

#include <QWidget>
#include <QGraphicsView>
#include "haspa_criticalpath.h"
namespace Ui {
class haspaCPresult;
}

class haspaCPresult : public QWidget
{
    Q_OBJECT

public:
    explicit haspaCPresult(QWidget *parent = nullptr,int tasknum=0);
    ~haspaCPresult();
    void showEvent(QShowEvent *event);
    HASPA_CRITICALPATH hsp;

    void setHaspa(const HASPA_CRITICALPATH& hs) {
        hsp = hs;
    }
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::haspaCPresult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;
};

#endif // HASPACPRESULT_H
