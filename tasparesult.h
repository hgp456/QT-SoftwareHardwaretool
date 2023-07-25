#ifndef TASPARESULT_H
#define TASPARESULT_H
#include "taspa.h"
#include <QWidget>
#include <QGraphicsView>
namespace Ui {
class tasparesult;
}

class tasparesult : public QWidget
{
    Q_OBJECT

public:
    explicit tasparesult(QWidget *parent = nullptr,int taspaValue=0);
    ~tasparesult();
    Taspa t;
    void showEvent(QShowEvent *event);
    void setTaspa(const Taspa& taspa) {
        t = taspa;
    }
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::tasparesult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;

};

#endif // TASPARESULT_H
