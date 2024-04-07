#include "klresult.h"
#include "ui_klresult.h"
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
std::vector<QString> Color1 {
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

std::vector<QString> Shape1 {
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

KLResult::KLResult(QWidget *parent,int tasknum) :
    QWidget(parent),
    k(tasknum),
    ui(new Ui::KLResult)
{
    ui->setupUi(this);
}



void KLResult::showEvent(QShowEvent *event) {
    ui->plainTextEdit->insertPlainText(k.output1);

    std::vector<std::vector<int>> array = k.commCost;
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
//                out << "  T" << i+1 << " -- T" << j+1 <<   " [label=\"" << array[i][j]   <<  "\""  << "]" << ";\n";
                out << char('a'+i) << " -- " << char('a'+j) <<   " [label=\"" << array[i][j]   <<  "\""  << "]" << ";\n";
            }
        }
    }

    out << "edge [fontname=\"Helvetica\", fontsize=10];\n";

//    qDebug() <<"Test1";
//    qDebug() <<k.Kc.size();

    for(int j=0;j<k.union1.size()-1;j++)
    {
        qDebug() << k.union1[j];
        out << char('a'+k.union1[j]) << ',';
    }
    out <<char('a'+k.union1[k.union1.size()-1]) <<" [shape=" << Shape1[0] <<", " << "color=" << Color1[0] << ", " <<"penwidth=2];\n" ;



    for(int j=0;j<k.union2.size()-1;j++)
    {
        out << char('a'+k.union2[j]) << ',';
    }
    out <<char('a'+k.union2[k.union2.size()-1]) <<" [shape=" << Shape1[1] <<", " << "color=" << Color1[1] << ", " <<"penwidth=2];\n" ;


    out << "}\n";



    dotFile.close();

    //    // Run the dot command to generate the graph
    QProcess process;
    //用于打包程序
//    process.setProgram("Graphviz/bin/dot.exe");
     process.setProgram("dot.exe");
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


KLResult::~KLResult()
{
    delete ui;
}
