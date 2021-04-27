#include "finalinterface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FinalInterFace w;
    w.show();
    return a.exec();
}
