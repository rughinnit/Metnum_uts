#include "metnum_uts.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    metnum_uts w;
    w.show();
    return a.exec();
}
