#ifndef MUPPARESULT_H
#define MUPPARESULT_H

#include <QWidget>
#include "muppa.h"
#include <QGraphicsView>
namespace Ui {
class MuppaResult;
}

class MuppaResult : public QWidget
{
    Q_OBJECT

public:
    explicit MuppaResult(QWidget *parent = nullptr,int value=0);
    ~MuppaResult();
    muppa m;
    void showEvent(QShowEvent *event);
    void setMuppa(const muppa& muppaa) {
        m = muppaa;
    }
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MuppaResult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;

};

#endif // MUPPARESULT_H
