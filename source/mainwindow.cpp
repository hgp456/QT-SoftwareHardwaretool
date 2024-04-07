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
#include "kl.h"
#include "klresult.h"
#include "haspa.h"
#include "haspa_criticalpath.h"
#include <QDateTime>
#include <QElapsedTimer>
#include "hspa_ga.h"
#include "hasparesult.h"
#include "haspacpresult.h"
#include "haspagaresult.h"
#include "QMessageBox"
#include <QtHelp>
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    int nowWidth = width();
    int nowHeight= height();
    // 计算小部件的新位置

    int stack_x=nowWidth / 2 - ui->stackedWidget->width() / 2;
    int stack_y=nowHeight / 2 - ui->stackedWidget->height() / 2;;
    // 设置小部件的新位置


    ui->stackedWidget->move(stack_x,stack_y);

}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
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
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_actionMuppa_triggered()
{
    ui->stackedWidget->setCurrentIndex(7) ;
}

void MainWindow::on_actionTaspa1_triggered()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_action_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionKL_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_actionHaspa_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionHaspa_critical_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_actionESHC_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionHaspa_GA_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
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
            QString fileContent = in.readAll().replace("\r\n", "\n");;
            fileContent.replace("\r\n", "\n");
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
                  ui->plainTextEdit->appendPlainText(dataList.join(" "));
             }

//            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}





void MainWindow::on_pushButton_3_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_15->clear();
    ui->lineEdit_16->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
    ui->plainTextEdit_5->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll().replace("\r\n", "\n");

            QStringList dataList;  // 存储解析后的字符串数据

            QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

            // 解析数据并存储到相应的变量或数组中
            QStringList lines = fileContent.split("\n");


            // 解析第一部分数据：处理器数量
            dataList = lines[0].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_12->setText(dataList[0]);

            // 解析第二部分数据:硬件数
            dataList = lines[2].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_13->setText(dataList[0]);


            // 解析第三部分数据:任务数量
            dataList = lines[4].split(",");
            ui->lineEdit_14->setText(dataList[0]);

            // 解析第四部分数据:任务软执行时间
            dataList = lines[6].split(",");
            //            qDebug() << dataList.size();
            //            for (const QString& data : dataList) {
            //                intList.append(data.toInt());
            //            }

            QString text = dataList.join(" ");
            ui->lineEdit_15->setText(text);

            // 解析第五部分数据:任务硬执行时间
            dataList = lines[8].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_16->setText(text);


            // 解析第六部分数据:任务面积表

            dataList = lines[10].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_17->setText(text);



            // 解析第七部分数据:任务面积约束
            dataList = lines[12].split(",");

            ui->lineEdit_18->setText(dataList[0]);


            // 解析第八部分数据:任务释放时间

            dataList = lines[14].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_19->setText(text);



            //              解析第四部分数据:依赖矩阵
            for (int i = 16; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit_5->appendPlainText(dataList.join(" "));
            }

            //            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
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
            QString fileContent = in.readAll().replace("\r\n", "\n");;


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
                 ui->plainTextEdit_2->appendPlainText(dataList.join(" "));
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
            QString fileContent = in.readAll().replace("\r\n", "\n");;


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
                 ui->plainTextEdit_3->appendPlainText(dataList.join(" "));
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
    if(ui->comboBox->currentText()=="全链接")
    {
        qDebug()<<ui->comboBox->currentText();
        mmm.Run_MMMCAA();
    }
    if(ui->comboBox->currentText()=="均链接")
    {
        qDebug()<<ui->comboBox->currentText();
        mmm.Run_MMMAAA();
    }
    MMMresult *MMMresultWindow = new MMMresult(nullptr,tasknum);
    MMMresultWindow->setMMM(mmm);
    MMMresultWindow->show();

}




void MainWindow::on_pushButton_12_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_11->clear();
    ui->plainTextEdit_4->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll().replace("\r\n", "\n");;


            QStringList dataList;  // 存储解析后的字符串数据

            QStringList lines = fileContent.split("\n");

            dataList = lines[0].split(",");
            ui->lineEdit_11->setText(dataList[0]);

            //              解析第四部分数据:依赖矩阵
            for (int i = 2; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit_4->appendPlainText(dataList.join(" "));
            }

            //            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}


void MainWindow::on_pushButton_13_clicked()
{
    int max=0;
    int tasknum=0;
    QVector<QVector<int>> ComFunction;


    tasknum=ui->lineEdit_11->text().toInt();
    qDebug()<< tasknum;


    QStringList lines  = ui->plainTextEdit_4->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        ComFunction.append(row);
    }

    KL kl(tasknum);
    for (const QVector<int>& subVector : ComFunction) {
        kl.commCost.emplace_back(subVector.begin(), subVector.end());
    }
    kl.Run();

    KLResult *klresultWindow =  new KLResult(nullptr,tasknum);
    klresultWindow->setKL(kl);
    klresultWindow->show();
//    MMMresult *MMMresultWindow = new MMMresult(nullptr,tasknum);
//    MMMresultWindow->setMMM(mmm);
//    MMMresultWindow->show();

}



void MainWindow::on_pushButton_7_clicked()
{
    int max=0;
    int tasknum=0;
    int cpunum=0;
    int fpganum=0;

    int sLimit=0;
    QVector<int> SoftExecTime;
    QVector<int> HardExecTime;
    QVector<int> HardArea;
    QVector<int> releaseTime;
    QVector<QVector<int>> CommunicationCost;


    cpunum=ui->lineEdit_12->text().toInt();
    fpganum=ui->lineEdit_13->text().toInt();
    tasknum=ui->lineEdit_14->text().toInt();


    QString text=ui->lineEdit_15->text();
    QStringList dataList=text.split(" ");
    for (const QString& data : dataList) {
        SoftExecTime.append(data.toInt());
    }


    QString text1=ui->lineEdit_16->text();
    QStringList dataList1=text1.split(" ");
    for (const QString& data : dataList1) {
        HardExecTime.append(data.toInt());
    }

    QString text2=ui->lineEdit_17->text();
    QStringList dataList2=text2.split(" ");
    for (const QString& data : dataList2) {
        HardArea.append(data.toInt());
    }


    sLimit=ui->lineEdit_18->text().toInt();
    qDebug()<< sLimit;


    QString text3=ui->lineEdit_19->text();
    QStringList dataList3=text3.split(" ");
    for (const QString& data : dataList3) {
        releaseTime.append(data.toInt());
    }


    QStringList lines  = ui->plainTextEdit_5->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        CommunicationCost.append(row);
    }


    haspa hasp(tasknum);

    hasp.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
    hasp.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
    hasp.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
    hasp.taskattribute.areaConstraints=sLimit;
    hasp.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
    hasp.cpu.resize(cpunum);
    hasp.fpga.resize(fpganum);
    for (const QVector<int>& subVector : CommunicationCost) {
        hasp.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
    }
    hasp.run();

// KLResult *klresultWindow =  new KLResult(nullptr,tasknum);

    hasparesult  *hsp = new hasparesult(nullptr,tasknum);
    hsp->setHaspa(hasp);
    hsp->show();



//    HASPA_CRITICALPATH haspa_critical(tasknum);
//    haspa_critical.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
//    haspa_critical.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
//    haspa_critical.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
//    haspa_critical.taskattribute.areaConstraints=sLimit;
//    haspa_critical.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
//    haspa_critical.cpu.resize(cpunum);
//    haspa_critical.fpga.resize(fpganum);
//    for (const QVector<int>& subVector : CommunicationCost) {
//        haspa_critical.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
//    }
//    haspa_critical.run();


}





void MainWindow::on_pushButton_22_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_47->clear();
    ui->lineEdit_48->clear();
    ui->lineEdit_49->clear();
    ui->lineEdit_50->clear();
    ui->lineEdit_51->clear();
    ui->lineEdit_52->clear();
    ui->lineEdit_53->clear();
    ui->lineEdit_54->clear();
    ui->plainTextEdit_11->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll().replace("\r\n", "\n");;


            QStringList dataList;  // 存储解析后的字符串数据

            QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

            // 解析数据并存储到相应的变量或数组中
            QStringList lines = fileContent.split("\n");


            // 解析第一部分数据：处理器数量
            dataList = lines[0].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_47->setText(dataList[0]);

            // 解析第二部分数据:硬件数
            dataList = lines[2].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_48->setText(dataList[0]);


            // 解析第三部分数据:任务数量
            dataList = lines[4].split(",");
            ui->lineEdit_49->setText(dataList[0]);

            // 解析第四部分数据:任务软执行时间
            dataList = lines[6].split(",");
            //            qDebug() << dataList.size();
            //            for (const QString& data : dataList) {
            //                intList.append(data.toInt());
            //            }

            QString text = dataList.join(" ");
            ui->lineEdit_50->setText(text);

            // 解析第五部分数据:任务硬执行时间
            dataList = lines[8].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_51->setText(text);


            // 解析第六部分数据:任务面积表

            dataList = lines[10].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_52->setText(text);



            // 解析第七部分数据:任务面积约束
            dataList = lines[12].split(",");

            ui->lineEdit_53->setText(dataList[0]);


            // 解析第八部分数据:任务释放时间

            dataList = lines[14].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_54->setText(text);



            //              解析第四部分数据:依赖矩阵
            for (int i = 16; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit_11->appendPlainText(dataList.join(" "));
            }

            //            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}


void MainWindow::on_pushButton_23_clicked()
{
    int max=0;
    int tasknum=0;
    int cpunum=0;
    int fpganum=0;

    int sLimit=0;
    QVector<int> SoftExecTime;
    QVector<int> HardExecTime;
    QVector<int> HardArea;
    QVector<int> releaseTime;
    QVector<QVector<int>> CommunicationCost;


    cpunum=ui->lineEdit_47->text().toInt();
    fpganum=ui->lineEdit_48->text().toInt();
    tasknum=ui->lineEdit_49->text().toInt();


    QString text=ui->lineEdit_50->text();
    QStringList dataList=text.split(" ");
    for (const QString& data : dataList) {
        SoftExecTime.append(data.toInt());
    }


    QString text1=ui->lineEdit_51->text();
    QStringList dataList1=text1.split(" ");
    for (const QString& data : dataList1) {
        HardExecTime.append(data.toInt());
    }

    QString text2=ui->lineEdit_52->text();
    QStringList dataList2=text2.split(" ");
    for (const QString& data : dataList2) {
        HardArea.append(data.toInt());
    }


    sLimit=ui->lineEdit_53->text().toInt();
    qDebug()<< sLimit;


    QString text3=ui->lineEdit_54->text();
    QStringList dataList3=text3.split(" ");
    for (const QString& data : dataList3) {
        releaseTime.append(data.toInt());
    }


    QStringList lines  = ui->plainTextEdit_11->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        CommunicationCost.append(row);
    }


    //    haspa hasp(tasknum);

    //    hasp.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
    //    hasp.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
    //    hasp.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
    //    hasp.taskattribute.areaConstraints=sLimit;
    //    hasp.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
    //    hasp.cpu.resize(cpunum);
    //    hasp.fpga.resize(fpganum);
    //    for (const QVector<int>& subVector : CommunicationCost) {
    //        hasp.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
    //    }
    //    hasp.run();


    HASPA_CRITICALPATH haspa_critical(tasknum);
    haspa_critical.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
    haspa_critical.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
    haspa_critical.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
    haspa_critical.taskattribute.areaConstraints=sLimit;
    haspa_critical.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
    haspa_critical.cpu.resize(cpunum);
    haspa_critical.fpga.resize(fpganum);
    for (const QVector<int>& subVector : CommunicationCost) {
        haspa_critical.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
    }
    haspa_critical.run();

    haspaCPresult  *hsp = new haspaCPresult(nullptr,tasknum);
    hsp->setHaspa(haspa_critical);
    hsp->show();

}




void MainWindow::on_pushButton_24_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_63->clear();
    ui->lineEdit_64->clear();
    ui->lineEdit_65->clear();
    ui->lineEdit_66->clear();
    ui->lineEdit_67->clear();
    ui->lineEdit_68->clear();
    ui->lineEdit_69->clear();
    ui->lineEdit_70->clear();
    ui->plainTextEdit_12->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Check if the file can be opened
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContent = in.readAll().replace("\r\n", "\n");;


            QStringList dataList;  // 存储解析后的字符串数据

            QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

            // 解析数据并存储到相应的变量或数组中
            QStringList lines = fileContent.split("\n");


            // 解析第一部分数据：处理器数量
            dataList = lines[0].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_63->setText(dataList[0]);

            // 解析第二部分数据:硬件数
            dataList = lines[2].split(",");
            //            max=dataList[0].toInt();
            ui->lineEdit_64->setText(dataList[0]);


            // 解析第三部分数据:任务数量
            dataList = lines[4].split(",");
            ui->lineEdit_65->setText(dataList[0]);

            // 解析第四部分数据:任务软执行时间
            dataList = lines[6].split(",");
            //            qDebug() << dataList.size();
            //            for (const QString& data : dataList) {
            //                intList.append(data.toInt());
            //            }

            QString text = dataList.join(" ");
            ui->lineEdit_66->setText(text);

            // 解析第五部分数据:任务硬执行时间
            dataList = lines[8].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_67->setText(text);


            // 解析第六部分数据:任务面积表

            dataList = lines[10].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_68->setText(text);



            // 解析第七部分数据:任务面积约束
            dataList = lines[12].split(",");

            ui->lineEdit_69->setText(dataList[0]);


            // 解析第八部分数据:任务释放时间

            dataList = lines[14].split(",");

            text.clear();
            text = dataList.join(" ");
            ui->lineEdit_70->setText(text);



            //              解析第四部分数据:依赖矩阵
            for (int i = 16; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 ui->plainTextEdit_12->appendPlainText(dataList.join(" "));
            }

            //            ui->plainTextEdit->appendPlainText("1");

            file.close();
        }
    }
}


void MainWindow::on_pushButton_25_clicked()
{
    int max=0;
    int tasknum=0;
    int cpunum=0;
    int fpganum=0;

    int sLimit=0;
    QVector<int> SoftExecTime;
    QVector<int> HardExecTime;
    QVector<int> HardArea;
    QVector<int> releaseTime;
    QVector<QVector<int>> CommunicationCost;


    cpunum=ui->lineEdit_63->text().toInt();
    fpganum=ui->lineEdit_64->text().toInt();
    tasknum=ui->lineEdit_65->text().toInt();


    QString text=ui->lineEdit_66->text();
    QStringList dataList=text.split(" ");
    for (const QString& data : dataList) {
        SoftExecTime.append(data.toInt());
    }


    QString text1=ui->lineEdit_67->text();
    QStringList dataList1=text1.split(" ");
    for (const QString& data : dataList1) {
        HardExecTime.append(data.toInt());
    }

    QString text2=ui->lineEdit_68->text();
    QStringList dataList2=text2.split(" ");
    for (const QString& data : dataList2) {
        HardArea.append(data.toInt());
    }


    sLimit=ui->lineEdit_69->text().toInt();
    qDebug()<< sLimit;


    QString text3=ui->lineEdit_70->text();
    QStringList dataList3=text3.split(" ");
    for (const QString& data : dataList3) {
        releaseTime.append(data.toInt());
    }


    QStringList lines  = ui->plainTextEdit_12->toPlainText().split("\n");
    for (const QString& line : lines) {
        QStringList values = line.split(" ");
        QVector<int> row;
        for (const QString& value : values) {
            row.append(value.toInt());
        }
        CommunicationCost.append(row);
    }
}


void MainWindow::on_pushButton_26_clicked()
{
    qint64 runningtimeavg=0;
    int time = 0;
    int Taskset_num=81;
    float scheduling_avg=0;

    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", "");
    QDir folder(folderPath);
    QStringList fileList = folder.entryList(QStringList() << "*.txt", QDir::Files);
    for(int i=0;i<11;i++)
    {

    for(const QString& fileName : fileList) {
        QString filePath = folderPath + "/" + fileName;
        int max=0;
        int tasknum=0;
        int cpunum=0;
        int fpganum=0;

        int sLimit=0;
        QElapsedTimer timer;
        QVector<int> SoftExecTime;
        QVector<int> HardExecTime;
        QVector<int> HardArea;
        QVector<int> releaseTime;
        QVector<QVector<int>> CommunicationCost;
        // Read and process the file content
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
             QTextStream in(&file);
             QString fileContent = in.readAll().replace("\r\n", "\n");;
             // Rest of your file processing code here...

             QStringList dataList;  // 存储解析后的字符串数据

             QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

             QStringList lines = fileContent.split("\n");

             // 解析第一部分数据：处理器数量
             cpunum = lines[0].split(",")[0].toInt();

             fpganum= lines[2].split(",")[0].toInt();

             tasknum= lines[4].split(",")[0].toInt();


             dataList=lines[6].split(",");
             for (const QString& data : dataList) {
                 SoftExecTime.append(data.toInt());
             }

             dataList=lines[8].split(",");
             for (const QString& data : dataList) {
                 HardExecTime.append(data.toInt());
             }

             dataList=lines[10].split(",");
             for (const QString& data : dataList) {
                 HardArea.append(data.toInt());
             }

             sLimit= lines[12].split(",")[0].toInt();

             dataList=lines[14].split(",");
             for (const QString& data : dataList) {
                 releaseTime.append(data.toInt());
             }


             for (int i = 16; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 QVector<int> row;
                 for (const QString& data : dataList)
                 {
                     row.append(data.toInt());
                 }
                 CommunicationCost.append(row);
             }
             file.close();
        }

        HASPA_CRITICALPATH haspa_critical(tasknum);
        haspa_critical.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
        haspa_critical.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
        haspa_critical.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
        haspa_critical.taskattribute.areaConstraints=sLimit;
        haspa_critical.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
        haspa_critical.cpu.resize(cpunum);
        haspa_critical.fpga.resize(fpganum);
        for (const QVector<int>& subVector : CommunicationCost) {
             haspa_critical.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
        }
        timer.start();
        haspa_critical.run();

        qint64 elapsedTime = timer.nsecsElapsed() / 1000;
        runningtimeavg+=elapsedTime;
//        qDebug() << "Elapsed Time (us): " << elapsedTime;
        time+=haspa_critical.runtime;

    }

    scheduling_avg=(float)time/Taskset_num;
    qDebug() << "scheduling_avg=" << scheduling_avg;
    time=0;

    }
    qDebug() << "Running Time Avg(us): " << (float)runningtimeavg/(81*11);

    float avg = (float)runningtimeavg / (81 * 11);

    QString message = "scheduling_avg = " + QString::number(scheduling_avg) +"\n" + "Running Time Avg(us): " + QString::number(avg) ;
    QMessageBox::information(this, "Result", message);
}


void MainWindow::on_pushButton_8_clicked()
{
    qint64 runningtimeavg=0;
    int time = 0;
    int Taskset_num=81;
    float scheduling_avg=0;

    QElapsedTimer timer;

    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", "");
    QDir folder(folderPath);
    QStringList fileList = folder.entryList(QStringList() << "*.txt", QDir::Files);

    for(int i=0;i<11;i++)
    {


    for(const QString& fileName : fileList) {
        QString filePath = folderPath + "/" + fileName;
        int max=0;
        int tasknum=0;
        int cpunum=0;
        int fpganum=0;

        int sLimit=0;
        QVector<int> SoftExecTime;
        QVector<int> HardExecTime;
        QVector<int> HardArea;
        QVector<int> releaseTime;
        QVector<QVector<int>> CommunicationCost;
        // Read and process the file content
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
             QTextStream in(&file);
             QString fileContent = in.readAll().replace("\r\n", "\n");;
             // Rest of your file processing code here...

             QStringList dataList;  // 存储解析后的字符串数据

             QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

             QStringList lines = fileContent.split("\n");

             // 解析第一部分数据：处理器数量
             cpunum = lines[0].split(",")[0].toInt();

             fpganum= lines[2].split(",")[0].toInt();

             tasknum= lines[4].split(",")[0].toInt();


             dataList=lines[6].split(",");
             for (const QString& data : dataList) {
                 SoftExecTime.append(data.toInt());
             }

             dataList=lines[8].split(",");
             for (const QString& data : dataList) {
                 HardExecTime.append(data.toInt());
             }

             dataList=lines[10].split(",");
             for (const QString& data : dataList) {
                 HardArea.append(data.toInt());
             }

             sLimit= lines[12].split(",")[0].toInt();

             dataList=lines[14].split(",");
             for (const QString& data : dataList) {
                 releaseTime.append(data.toInt());
             }


             for (int i = 16; i < lines.size(); i++) {
                 dataList = lines[i].split(",");
                 QVector<int> row;
                 for (const QString& data : dataList)
                 {
                     row.append(data.toInt());
                 }
                 CommunicationCost.append(row);
             }
             file.close();
        }


        haspa hasp(tasknum);

        hasp.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
        hasp.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
        hasp.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
        hasp.taskattribute.areaConstraints=sLimit;
        hasp.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
        hasp.cpu.resize(cpunum);
        hasp.fpga.resize(fpganum);
        for (const QVector<int>& subVector : CommunicationCost) {
             hasp.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
        }
        timer.start();
        hasp.run();

        qint64 elapsedTime = timer.nsecsElapsed() / 1000;
        runningtimeavg+=elapsedTime;
        //        qDebug() << "Elapsed Time (us): " << elapsedTime;
        time+=hasp.runtime;

    }

    scheduling_avg=(float)time/Taskset_num;
    qDebug() << "scheduling_avg=" << scheduling_avg;
    time=0;

    }
    qDebug() << "Running Time Avg(us): " << (float)runningtimeavg/(81*11);
    float avg = (float)runningtimeavg / (81 * 11);

    QString message = "scheduling_avg = " + QString::number(scheduling_avg) +"\n" + "Running Time Avg(us): " + QString::number(avg) ;
    QMessageBox::information(this, "Result", message);


}


void MainWindow::on_pushButton_29_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    ui->lineEdit_87->clear();
    ui->lineEdit_88->clear();
    ui->lineEdit_89->clear();
    ui->lineEdit_90->clear();
    ui->lineEdit_91->clear();
    ui->lineEdit_92->clear();
    ui->lineEdit_93->clear();
    ui->lineEdit_94->clear();
    ui->plainTextEdit_14->clear();
    // Check if a file was selected
    if (!filePath.isEmpty()) {
    QFile file(filePath);

    // Check if the file can be opened
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString fileContent = in.readAll().replace("\r\n", "\n");;


        QStringList dataList;  // 存储解析后的字符串数据

        QVector<QVector<int>> matrix;  // 存储解析后的矩阵数据

        // 解析数据并存储到相应的变量或数组中
        QStringList lines = fileContent.split("\n");


        // 解析第一部分数据：处理器数量
        dataList = lines[0].split(",");
        //            max=dataList[0].toInt();
        ui->lineEdit_87->setText(dataList[0]);

        // 解析第二部分数据:硬件数
        dataList = lines[2].split(",");
        //            max=dataList[0].toInt();
        ui->lineEdit_88->setText(dataList[0]);


        // 解析第三部分数据:任务数量
        dataList = lines[4].split(",");
        ui->lineEdit_89->setText(dataList[0]);

        // 解析第四部分数据:任务软执行时间
        dataList = lines[6].split(",");
        //            qDebug() << dataList.size();
        //            for (const QString& data : dataList) {
        //                intList.append(data.toInt());
        //            }

        QString text = dataList.join(" ");
        ui->lineEdit_90->setText(text);

        // 解析第五部分数据:任务硬执行时间
        dataList = lines[8].split(",");

        text.clear();
        text = dataList.join(" ");
        ui->lineEdit_91->setText(text);


        // 解析第六部分数据:任务面积表

        dataList = lines[10].split(",");

        text.clear();
        text = dataList.join(" ");
        ui->lineEdit_92->setText(text);



        // 解析第七部分数据:任务面积约束
        dataList = lines[12].split(",");

        ui->lineEdit_93->setText(dataList[0]);


        // 解析第八部分数据:任务释放时间

        dataList = lines[14].split(",");

        text.clear();
        text = dataList.join(" ");
        ui->lineEdit_94->setText(text);



        //              解析第四部分数据:依赖矩阵
        for (int i = 16; i < lines.size(); i++) {
             dataList = lines[i].split(",");
             ui->plainTextEdit_14->appendPlainText(dataList.join(" "));
        }

        //            ui->plainTextEdit->appendPlainText("1");

        file.close();
    }
    }
}




void MainWindow::on_pushButton_30_clicked()
{
    int max=0;
    int tasknum=0;
    int cpunum=0;
    int fpganum=0;

    int sLimit=0;
    QVector<int> SoftExecTime;
    QVector<int> HardExecTime;
    QVector<int> HardArea;
    QVector<int> releaseTime;
    QVector<QVector<int>> CommunicationCost;


    cpunum=ui->lineEdit_87->text().toInt();
    fpganum=ui->lineEdit_88->text().toInt();
    tasknum=ui->lineEdit_89->text().toInt();


    QString text=ui->lineEdit_90->text();
    QStringList dataList=text.split(" ");
    for (const QString& data : dataList) {
    SoftExecTime.append(data.toInt());
    }


    QString text1=ui->lineEdit_91->text();
    QStringList dataList1=text1.split(" ");
    for (const QString& data : dataList1) {
    HardExecTime.append(data.toInt());
    }

    QString text2=ui->lineEdit_92->text();
    QStringList dataList2=text2.split(" ");
    for (const QString& data : dataList2) {
    HardArea.append(data.toInt());
    }


    sLimit=ui->lineEdit_93->text().toInt();
//    qDebug()<< sLimit;


    QString text3=ui->lineEdit_94->text();
    QStringList dataList3=text3.split(" ");
    for (const QString& data : dataList3) {
    releaseTime.append(data.toInt());
    }


    QStringList lines  = ui->plainTextEdit_14->toPlainText().split("\n");
    for (const QString& line : lines) {
    QStringList values = line.split(" ");
    QVector<int> row;
    for (const QString& value : values) {
        row.append(value.toInt());
    }
    CommunicationCost.append(row);
    }


    hspa_ga hspaga(tasknum);
    hspaga.taskattribute.sortExectime.assign(SoftExecTime.begin(),SoftExecTime.end());
    hspaga.taskattribute.hardExectime.assign(HardExecTime.begin(),HardExecTime.end());
    hspaga.taskattribute.hardwareArea.assign(HardArea.begin(),HardArea.end());
    hspaga.taskattribute.areaConstraints=sLimit;
    hspaga.taskattribute.relesetime.assign(releaseTime.begin(),releaseTime.end());
    hspaga.cpu.resize(cpunum);
    hspaga.fpga.resize(fpganum);
    for (const QVector<int>& subVector : CommunicationCost) {
    hspaga.taskattribute.CommunicationCost.emplace_back(subVector.begin(), subVector.end());
    }
    hspaga.run();


    haspagaresult  *hsp = new haspagaresult(nullptr,tasknum);
    hsp->setHaspa(hspaga);
    hsp->show();
}


void MainWindow::on_actionOnline_Help_triggered()
{
    QHelpEngineCore helpEngine("mycollection.qhc");

        // get all file references for the identifier
        QList<QHelpLink> links =
        helpEngine.documentsForIdentifier(QLatin1String("MyDialog::ChangeButton"));

    // If help is available for this keyword, get the help data
    // of the first file reference.
    if (links.count()) {
        QByteArray helpData = helpEngine.fileData(links.constBegin()->url);
        // show the documentation to the user
        // if (!helpData.isEmpty())
            // displayHelp(helpData);
    // }
    }
}


void MainWindow::on_actionsecond_triggered()
{
    QMessageBox::about(this, tr("关于智能嵌入式系统课程工具"),
                       tr("本工具是对陈仪香教授所著的《智能嵌入式系统设计》书中的算法进行实现并进行图形化展示"
                          "目的是更直观的得到书中算法的结果,以此能够更好地理解书中的算法\n"
                          "如有意见或建议,请通过51255902164@stu.ecnu.edu.cn联系\n!"));
}

