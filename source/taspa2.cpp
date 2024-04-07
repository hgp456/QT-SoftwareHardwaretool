#include "taspa2.h"
#include "ui_taspa2.h"

taspa2::taspa2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::taspa2)
{
    ui->setupUi(this);
}

taspa2::~taspa2()
{
    delete ui;
}
