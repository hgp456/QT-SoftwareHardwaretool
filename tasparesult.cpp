#include "tasparesult.h"
#include "ui_tasparesult.h"
#include "taspa.h"
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

tasparesult::tasparesult(QWidget *parent,int taspaValue) :
    QWidget(parent),
    t(taspaValue),
    ui(new Ui::tasparesult)
{
    ui->setupUi(this);

    ui->plainTextEdit->insertPlainText("Task  PriorityValue\n");


}


void tasparesult::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    ui->plainTextEdit->insertPlainText(t.order_print());
//    ui->graphicsView->
    std::vector<std::vector<int>> array = t.array;
    QFile dotFile("graph.dot");
    if (!dotFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open dot file.";
        return;
    }
    QTextStream out(&dotFile);
    out << "digraph G {\n";
    int n = array.size();
    bool all_in=false;
    for (int i = 0; i < n; ++i) {
        all_in=false;
        for (int j = 0; j < n; ++j) {
            if (array[i][j] != 0) {
                out << "  \"T" << i+1 <<"," << t.val[i]  <<  " \" " << " -> " << " \"T" << j+1 << "," << t.val[j] << " \" "  <<";\n";
                all_in=true;
            }
        }
        if(all_in==false&&i!=n-1)
            out << " \"T"<<i+1 << "," << t.val[i] << " \" ";

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
//    QImage image;
//    image.load("graph.svg");

//    QGraphicsScene *scene = new QGraphicsScene(this);
//    scene->addPixmap(QPixmap::fromImage(image));
//    graphicsView->setScene(scene);
//    graphicsView->setRenderHint(QPainter::Antialiasing);
//    graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
////    graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
//    // Set the size of the QGraphicsView to match the desired size
//    graphicsView->setTransform(QTransform().scale(x, y));
//    // Center the image within the QGraphicsView
//    graphicsView->setAlignment(Qt::AlignCenter);

//    qDebug() << "111111.";
}


tasparesult::~tasparesult()
{
    delete ui;
}

void tasparesult::on_pushButton_clicked()
{
    x=x-0.25;
    y=y-0.25;
    if(x<=0) x=0.25;
    if(y<=0) y=0.25;
   graphicsView->setTransform(QTransform().scale(x, y));
}


void tasparesult::on_pushButton_2_clicked()
{
   x=x+0.25;
   y=y+0.25;
   graphicsView->setTransform(QTransform().scale(x, y));
}

