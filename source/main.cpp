#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QIcon>
//这个适用于打包的标志位 如果是编译程序无需打开
// #define PACKAGE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qssFile("D:/workspace/QT-SoftwareHardwaretool/QSS/Ubuntu.qss");//1.
    if(qssFile.open(QFile::ReadOnly)){//2.
        a.setStyleSheet(qssFile.readAll());//3.
        qDebug() << "test";
    }
    qssFile.close();//4


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "test_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
//    QApplication::setAttribute(Qt::AA_Use96Dpi); qputenv("QT_SCALE_FACTOR", QString::number(1.0).toLatin1());
    MainWindow w;
    w.show();

    return a.exec();
}
