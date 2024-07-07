#include "hasparesult.h"
#include "ui_hasparesult.h"
#include <QString>
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
#include <QFileDialog>
hasparesult::hasparesult(QWidget *parent,int tasknum) :
    QWidget(parent),
    hsp(tasknum),
    ui(new Ui::hasparesult)
{
    ui->setupUi(this);
}


void hasparesult::showEvent(QShowEvent *event) {
     ui->plainTextEdit->insertPlainText(hsp.output+"\n");
     ui->plainTextEdit->insertPlainText(hsp.result_output());

    std::vector<std::vector<int>> array = hsp.taskattribute.CommunicationCost;
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
                out << "  \"T" << i+1 <<"(" << hsp.taskattribute.sortExectime[i] <<  "," << hsp.taskattribute.hardExectime[i] <<"," << hsp.taskattribute.hardwareArea[i] << ")" <<  " \" " << " -> " << " \"T" << j+1 << "(" << hsp.taskattribute.sortExectime[j] <<"," <<hsp.taskattribute.hardExectime[j] <<"," << hsp.taskattribute.hardwareArea[j]  << ")" << " \"" << "[label=" <<"\"" << hsp.taskattribute.CommunicationCost[i][j] << "\"" <<",penwidth=\"3\"]" <<";\n";
                all_in=true;
            }
        }
        if(all_in==false&&hsp.TaskList[i].pre.size()==0)
            out << " \"T"<<i+1 << "(" << hsp.taskattribute.sortExectime[i]<<  "," << hsp.taskattribute.hardExectime[i] << "," << hsp.taskattribute.hardwareArea[i] << ")" << " \" ";

    }

    //涂色
    for (int i = 0; i < n; ++i) {
        if(hsp.TaskList[i].hardwareuse==true)
        {
            out << "  \"T" << i+1 <<"(" << hsp.taskattribute.sortExectime[i] <<  "," << hsp.taskattribute.hardExectime[i] <<"," << hsp.taskattribute.hardwareArea[i] <<")" <<  " \" " << "[fillcolor=\"Red\", style=\"filled\",penwidth=\"3\"] "  <<";\n";
        }
        else
        {
            out << "  \"T" << i+1 <<"(" << hsp.taskattribute.sortExectime[i] <<  "," << hsp.taskattribute.hardExectime[i] <<"," << hsp.taskattribute.hardwareArea[i] <<")"  <<  " \" " << "[fillcolor=\"Orange\", style=\"filled\",penwidth=\"3\"] "  <<";\n";
        }
    }

    out << "}\n";
    dotFile.close();

    // Run the dot command to generate the graph
    QProcess process;
    //用于打包程序
//    process.setProgram("Graphviz/bin/dot.exe");
    #ifdef PACKAGE
        process.setProgram("Graphviz/bin/dot.exe");
    #else
        process.setProgram("dot.exe");
    #endif

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
    //
}

hasparesult::~hasparesult()
{
    delete ui;
}

void hasparesult::on_pushButton_clicked()
{
    x=x-0.25;
    y=y-0.25;
    if(x<=0) x=0.25;
    if(y<=0) y=0.25;
    graphicsView->setTransform(QTransform().scale(x, y));
}


void hasparesult::on_pushButton_2_clicked()
{
    x=x+0.25;
    y=y+0.25;
    graphicsView->setTransform(QTransform().scale(x, y));
}


void hasparesult::on_pushButton_4_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", "", "SVG Files (*.svg)");

    if (!filePath.isEmpty()) {
        QFile::copy("graph.svg", filePath);
    }
}


void hasparesult::on_pushButton_3_clicked()
{
    #ifdef PACKAGE
        QProcess::execute("test_hspa.exe");
    #else
        QProcess::startDetached("python", QStringList() << "test_hspa.py");
    #endif

}

