#ifndef KLRESULT_H
#define KLRESULT_H

#include <QWidget>
#include "kl.h"
#include <QGraphicsView>
namespace Ui {
class KLResult;
}

class KLResult : public QWidget
{
    Q_OBJECT

public:
    explicit KLResult(QWidget *parent = nullptr,int tasknum=0);
    ~KLResult();
    KL k;
    void showEvent(QShowEvent *event);
    void setKL(const KL& kl) {
        k = kl;
    }
private:
    Ui::KLResult *ui;
    float x=1;
    float y=1;
    QGraphicsView *graphicsView = new QGraphicsView;
};

#endif // KLRESULT_H
