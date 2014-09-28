#include <QApplication>

#include "MainApplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainApplication m;

    return a.exec();
}
