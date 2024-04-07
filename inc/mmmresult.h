#ifndef MMMRESULT_H
#define MMMRESULT_H
#include "mmm.h"
#include <QWidget>
#include <QGraphicsView>
namespace Ui {
class MMMresult;
}

class MMMresult : public QWidget
{
    Q_OBJECT

public:
    explicit MMMresult(QWidget *parent = nullptr,int value=0);
    ~MMMresult();
    MMM m;
    void showEvent(QShowEvent *event);
    void setMMM(const MMM& mmm) {
        m = mmm;
    }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::MMMresult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;
};

#endif // MMMRESULT_H
