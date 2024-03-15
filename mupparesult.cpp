#include "mupparesult.h"
#include "ui_mupparesult.h"
#include "muppa.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <qdir.h>
#include <QImage>
#include <QGraphicsSvgItem>
#include <QtSvg/QSvgRenderer>
#include <Qtsvg/QtSvg>
#include <QGraphicsScene>
#include <QDesktopServices>
#include <QFileDialog>


MuppaResult::MuppaResult(QWidget *parent,int value) :
    QWidget(parent),
    m(value),
    ui(new Ui::MuppaResult)
{
    ui->setupUi(this);
}

void MuppaResult::showEvent(QShowEvent *event) {

   ui->plainTextEdit->insertPlainText(m.t.order_print()+"\n");
   ui->plainTextEdit->insertPlainText(m.result_output());
    std::vector<std::vector<int>> array = m.t.array;
    qDebug() << QDir::currentPath();
    QFile dotFile("graph.dot");
    if (!dotFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open dot file.";
        return;
    }
    QTextStream out(&dotFile);
    out << "digraph G {\n";
    int n = array.size();
    qDebug()<<"nsize=" << n;
    bool all_in=false;
    for (int i = 0; i < n; ++i) {
        all_in=false;
        for (int j = 0; j < n; ++j) {
            if (array[i][j] != 0) {
                out << "  \"T" << i+1 <<"," << m.t.val[i]  <<  " \" " << " -> " << " \"T" << j+1 << "," << m.t.val[j] << " \" "  <<";\n";
                all_in=true;
            }
        }
        if(all_in==false&&i!=n-1)
            out << " \"T"<<i+1 << "," << m.t.val[i] << " \" ";

    }
    out << "}\n";
    dotFile.close();

    // Run the dot command to generate the graph
    QProcess process;
    process.setProgram("Graphviz/bin/dot.exe");
    QStringList arguments;
    arguments << "-Tsvg" << "graph.dot" << "-o" << "graph.svg";
    process.setArguments(arguments);

    process.start();
    if (process.waitForFinished(-1)) {
        QByteArray output = process.readAllStandardOutput();
        QByteArray error = process.readAllStandardError();

        qDebug() << "Output:" << output;
        qDebug() << "Error:" << error;
    } else {
        qDebug() << "Process execution failed.";
    }



    // Load the graph image and display it in the QGraphicsView
    //    QPixmap pixmap("graph.svg");
    //    QGraphicsView *graphicsView = new QGraphicsView;
    ui->scrollArea->setWidget(graphicsView);

    QSvgRenderer *renderer = new QSvgRenderer(QLatin1String("graph.svg"));

    QGraphicsScene* scene = new QGraphicsScene(this);
    //    scene->setSceneRect(renderer.defaultSize());
    QGraphicsSvgItem* svgItem = new QGraphicsSvgItem();

    svgItem->setSharedRenderer(renderer);
    scene->addItem(svgItem);
    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->setTransform(QTransform().scale(x, y));
    graphicsView->setAlignment(Qt::AlignCenter);
    graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::Window);




}

MuppaResult::~MuppaResult()
{
    delete ui;
}

void MuppaResult::on_pushButton_clicked()
{
    x=x-0.25;
    y=y-0.25;
    if(x<=0) x=0.25;
    if(y<=0) y=0.25;
    graphicsView->setTransform(QTransform().scale(x, y));
}


void MuppaResult::on_pushButton_2_clicked()
{
    x=x+0.25;
    y=y+0.25;
    graphicsView->setTransform(QTransform().scale(x, y));
}


void MuppaResult::on_pushButton_5_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", "", "SVG Files (*.svg)");

    if (!filePath.isEmpty()) {
        QFile::copy("graph.svg", filePath);
    }
}


void MuppaResult::on_pushButton_6_clicked()
{

    QProcess::startDetached("python", QStringList() << "test.py");
//      QProcess::execute("test_ori.exe"); // 用于打包版本
//      QProcess::startDetached("start test_ori.exe");
//      QDesktopServices::openUrl(QUrl::fromLocalFile("output.svg"));
//    QProcess process1;
//    process1.setProgram("python");
//    QStringList arguments1;
//    arguments1 << "test.py";
//    process1.setArguments(arguments1);
//    process1.start();
//    if (process1.waitForFinished(-1)) {
//        QByteArray output = process1.readAllStandardOutput();
//        QByteArray error = process1.readAllStandardError();

//        qDebug() << "Output:" << output;
//        qDebug() << "Error:" << error;
//    } else {
//        qDebug() << "Process execution failed.";
//    }
//    QDesktopServices::openUrl(QUrl::fromLocalFile("output.svg"));
}

