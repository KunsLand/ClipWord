#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://dictionary-icon.png"));
    a.quitOnLastWindowClosed();
    Widget w;
    w.show();

    return a.exec();
}
