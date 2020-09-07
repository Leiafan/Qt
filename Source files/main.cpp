#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    QSplashScreen *scr = new QSplashScreen;
    MainWindow auth;

    scr->setPixmap(QPixmap(":/mw/img/logo_ksu.jpg"));
    scr->show();
    QTimer::singleShot(3000, scr, SLOT(close()));
    QTimer::singleShot(3300, &auth,SLOT(show()));

    return a.exec();
}
