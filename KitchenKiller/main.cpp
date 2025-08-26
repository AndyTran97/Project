#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //set app icon
    QIcon icon(":/sprites/icons/Kitchen.PNG");
    a.setWindowIcon(icon);
    w.show();
    return a.exec();

}
