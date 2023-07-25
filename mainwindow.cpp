#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include "taspa.h"
#include "tasparesult.h"
#include "muppa.h"
#include "mupparesult.h"
#include "mmm.h"
#include "mmmresult.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Hello World";
}
void MainWindow::on_actionTaspa_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionMuppa_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionTaspa1_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_action_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_2_clicked()
{
    // Open a file dialog to select the text file
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->plainTextEdit->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();


            QStringList dataList;  // 存储解析后的字符串数据

//            int  max;  // 存储解析后的整数数据
            QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

            // 解析数据并存储到相应的变量或数组中
            QStringList lines = fileContent.split("\n");


            // 解析第一部分数据：数组大小
            dataList = lines[0].split(",");
//            max=dataList[0].toInt();
            ui->lineEdit->setText(dataList[0]);
            
            // 解析第二部分数据:执行时间
            dataList = lines[2].split(",");
//            qDebug() << dataList.size();
//            for (const QString& data : dataList) {
//                intList.append(data.toInt());
//            }

            QString text = dataList.join(" ");
            ui->lineEdit_2->setText(text);


            // 解析第三部分数据:释放时间
            dataList = lines[4].split(",");
//            qDebug() << dataList.size();
//            for (const QString& data : dataList) {
//                intList.append(data.toInt());
//            }

            QString text1 = dataList.join(" ");
            ui->lineEdit_3->setText(text1);



//              解析第四部分数据:依赖矩阵
             for (int i = 6; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit->insertPlainText(dataList.join(" "));
             }

//            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}





void MainWindow::on_pushButton_3_clicked()
{

}


void MainWindow::on_pushButton_4_clicked()
{
    int max=0;
    QVector<int> intList;
    QVector<int> RetimeInput;
    QVector<QVector<int>> RelyFunction;
    max=ui->lineEdit->text().toInt();
    qDebug()<< max;

    QString text=ui->lineEdit_2->text();
    QStringList dataList=text.split(" ");
    for (const QString& data : dataList) {
        intList.append(data.toInt());
    }

    text=ui->lineEdit_3->text();
    dataList=text.split(" ");
    for (const QString& data : dataList) {
        RetimeInput.append(data.toInt());
    }


    QStringList lines  = ui->plainTextEdit->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        RelyFunction.append(row);
    }
    Taspa t(max);
    //传递参数

    t.val.assign(intList.begin(), intList.end()); //传递任务运行时间
    t.Retime.assign(RetimeInput.begin(),RetimeInput.end());
    for (const QVector<int>& subVector : RelyFunction) {
        t.array.emplace_back(subVector.begin(), subVector.end());
    }
    t.Run();

//    tasparesult tasparesultwindow;

    tasparesult *tasparesultwindow = new tasparesult(nullptr,max);
    tasparesultwindow->setTaspa(t);
    tasparesultwindow->show();

//    tasparesultwindow.raise();
//    tasparesultwindowactivateWindow();
//     此处代码用于校验输入是否正确
//    QString output;
//    for (int i = 0; i < intList.size(); i++) {
//        output.append(QString::number(intList[i]) + " ");
//    }
//    qDebug().noquote() << output;

//    output.clear();
//    for (int i = 0; i < intList.size(); i++) {
//        output.append(QString::number(RetimeInput[i]) + " ");
//    }
//    qDebug().noquote() << output;


//    output.clear();
//    for (const QVector<int>& row : RelyFunction) {
//        output.clear();
//        for (int value : row) {
//            output.append(QString::number(value) + " ");
//        }
//        qDebug().noquote() << output;
//    }

}


void MainWindow::on_pushButton_5_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->plainTextEdit_2->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();


            QStringList dataList;  // 存储解析后的字符串数据

            //            int  max;  // 存储解析后的整数数据
            QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

            // 解析数据并存储到相应的变量或数组中
            QStringList lines = fileContent.split("\n");


            dataList = lines[0].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_7->setText(dataList[0]);

            // 解析第一部分数据：数组大小
            dataList = lines[2].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_4->setText(dataList[0]);

            // 解析第二部分数据:执行时间
            dataList = lines[4].split(",");
            //            qDebug() << dataList.size();
            //            for (const QString& data : dataList) {
            //                intList.append(data.toInt());
            //            }

            QString text = dataList.join(" ");
            ui->lineEdit_5->setText(text);


            // 解析第三部分数据:释放时间
            dataList = lines[6].split(",");
            //            qDebug() << dataList.size();
            //            for (const QString& data : dataList) {
            //                intList.append(data.toInt());
            //            }

            QString text1 = dataList.join(" ");
            ui->lineEdit_6->setText(text1);



            //              解析第四部分数据:依赖矩阵
            for (int i = 8; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit_2->insertPlainText(dataList.join(" "));
            }

            //            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    int max=0;
    int corenum=0;
    QVector<int> intList;
    QVector<int> RetimeInput;
    QVector<QVector<int>> RelyFunction;


    corenum=ui->lineEdit_7->text().toInt();
    qDebug()<< corenum;

    max=ui->lineEdit_4->text().toInt();
    qDebug()<< max;

    QString text=ui->lineEdit_5->text();
    QStringList dataList=text.split(" ");
    for (const QString& data : dataList) {
        intList.append(data.toInt());
    }

    text=ui->lineEdit_6->text();
    dataList=text.split(" ");
    for (const QString& data : dataList) {
        RetimeInput.append(data.toInt());
    }


    QStringList lines  = ui->plainTextEdit_2->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        RelyFunction.append(row);
    }
    Taspa t(max);
    //传递参数

    t.val.assign(intList.begin(), intList.end()); //传递任务运行时间
    t.Retime.assign(RetimeInput.begin(),RetimeInput.end());
    for (const QVector<int>& subVector : RelyFunction) {
        t.array.emplace_back(subVector.begin(), subVector.end());
    }
    t.Run();
//    qDebug()<<t.order_print();
    //    tasparesult tasparesultwindow;
    muppa muppaa(max);
    muppaa.t=t;
    muppaa.CORENUM=corenum;
    muppaa.Run();

    MuppaResult *MuppaResultwindow = new MuppaResult(nullptr,max);
    MuppaResultwindow->setMuppa(muppaa);
    MuppaResultwindow->show();

}







void MainWindow::on_pushButton_11_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();
    ui->plainTextEdit_3->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();


            QStringList dataList;  // 存储解析后的字符串数据

            QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

            QStringList lines = fileContent.split("\n");

            dataList = lines[0].split(",");
            ui->lineEdit_8->setText(dataList[0]);

            // 解析第一部分数据：数组大小
            dataList = lines[2].split(",");
            ui->lineEdit_9->setText(dataList[0]);

            // 解析第二部分数据:执行时间
            dataList = lines[4].split(",");


            QString text = dataList.join(" ");
            ui->lineEdit_10->setText(dataList[0]);


            //              解析第四部分数据:依赖矩阵
            for (int i = 6; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit_3->insertPlainText(dataList.join(" "));
            }

            //            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}


void MainWindow::on_pushButton_10_clicked()
{
    int max=0;
    int tasknum=0;
    int modulesNum=0;
    int maxTaskNum=0;
    QVector<QVector<int>> ComFunction;


    tasknum=ui->lineEdit_8->text().toInt();
    qDebug()<< tasknum;

    modulesNum=ui->lineEdit_9->text().toInt();
    qDebug()<< modulesNum;

    maxTaskNum=ui->lineEdit_10->text().toInt();
    qDebug()<< maxTaskNum;


    QStringList lines  = ui->plainTextEdit_3->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        ComFunction.append(row);
    }

    MMM mmm(tasknum);
    mmm.modulesNum=modulesNum;
    mmm.maxTaskNum=maxTaskNum;
    for (const QVector<int>& subVector : ComFunction) {
        mmm.commCost.emplace_back(subVector.begin(), subVector.end());
//        mmm.clusterarrayOri.emplace_back(subVector.begin(), subVector.end());
    }
    mmm.clusterarrayOri=mmm.commCost;
    if(ui->comboBox->currentText()=="单链接")
    {
        qDebug()<<ui->comboBox->currentText();
        mmm.Run();
    }

    MMMresult *MMMresultWindow = new MMMresult(nullptr,tasknum);
    MMMresultWindow->setMMM(mmm);
    MMMresultWindow->show();

}

