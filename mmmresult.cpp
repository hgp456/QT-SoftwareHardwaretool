#include "mmmresult.h"
#include "ui_mmmresult.h"
#include "mmm.h"
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
std::vector<QString> Color {
    "RED",
    "BLUE",
    "GREEN",
    "YELLOW",
    "ORANGE",
    "PURPLE",
    "PINK",
    "CYAN",
    "BROWN",
    "GOLD",
    "SILVER",
    "BLACK",
    "WHITE",
    "GRAY",
    "MAROON",
    "NAVY",
    "TEAL",
    "LIME",
    "AQUA",
    "OLIVE",
    "FUCHSIA",
    "INDIGO",
    "CORAL",
    "TURQUOISE",
    "VIOLET",
    "MAGENTA",
    "SAFFRON",
    "CRIMSON",
    "LAVENDER",
    "PLUM",
    "KHAKI"
};

std::vector<QString> Shape {
    "circle",
    "square",
    "triangle",
    "rectangle",
    "hexagon",
    "pentagon",
    "ellipse",
    "diamond",
    "star",
    "octagon",
    "heart",
    "crescent",
    "cross",
    "arrow",
    "spiral",
    "cloud",
    "sun",
    "moon",
    "flower",
    "leaf",
    "wave",
    "mountain",
    "tree",
    "house",
    "car",
    "boat",
    "plane",
    "rocket",
    "clock",
    "book"
};

MMMresult::MMMresult(QWidget *parent,int  value) :
    QWidget(parent),
    m(value),
    ui(new Ui::MMMresult)
{
    ui->setupUi(this);
}


void MMMresult::showEvent(QShowEvent *event) {
    ui->plainTextEdit->insertPlainText(m.result_print());

    std::vector<std::vector<int>> array = m.clusterarrayOri;
    qDebug() << QDir::currentPath();
    QFile dotFile("graph.dot");
    if (!dotFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open dot file.";
        return;
    }
    QTextStream out(&dotFile);
    out << "graph G {\n";
    int n = array.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (array[i][j] !=0) {
                out << "  T" << i+1 << " -- T" << j+1 <<   " [label=\"" << array[i][j]   <<  "\""  << "]" << ";\n";
            }
       }
    }

    out << "edge [fontname=\"Helvetica\", fontsize=10];\n";

    qDebug() <<"Test1";
    qDebug() <<m.Kc.size();
    for(int i=0;i<m.Kc.size();i++)
    {
       for(int j=0;j<m.Kc[i].size();j++)
       {
            out << "T" <<m.Kc[i][j]+1 << " [shape=" << Shape[i] <<", " << "color=" << Color[i] << ", " <<"penwidth=2];\n" ;
       }
    }


    out << "}\n";



    dotFile.close();

//    // Run the dot command to generate the graph
    QProcess process;
    process.setProgram("dot");
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


MMMresult::~MMMresult()
{
    delete ui;
}


void MMMresult::on_pushButton_clicked()
{
    x=x-0.25;
    y=y-0.25;
    if(x<=0) x=0.25;
    if(y<=0) y=0.25;
    graphicsView->setTransform(QTransform().scale(x, y));
}


void MMMresult::on_pushButton_2_clicked()
{
    x=x+0.25;
    y=y+0.25;
    graphicsView->setTransform(QTransform().scale(x, y));
}
