#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_actionTaspa_triggered();

    void on_actionMuppa_triggered();

    void on_pushButton_2_clicked();

    void on_actionTaspa1_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_action_triggered();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_actionKL_triggered();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_actionHaspa_triggered();

    void on_pushButton_7_clicked();

    void resizeEvent(QResizeEvent *event);
    void on_actionHaspa_critical_triggered();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_actionESHC_triggered();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_8_clicked();

    void on_actionHaspa_GA_triggered();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_actionOnline_Help_triggered();

    void on_actionsecond_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
