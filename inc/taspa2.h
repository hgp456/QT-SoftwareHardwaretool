#ifndef TASPA2_H
#define TASPA2_H

#include <QWidget>

namespace Ui {
class taspa2;
}

class taspa2 : public QWidget
{
    Q_OBJECT

public:
    explicit taspa2(QWidget *parent = nullptr);
    ~taspa2();

private:
    Ui::taspa2 *ui;
};

#endif // TASPA2_H
