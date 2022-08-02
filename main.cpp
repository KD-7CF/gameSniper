#include <QApplication>
#include "game.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    game G;
    G.setMinimumSize(QSize(600, 600));
    G.setMaximumSize(QSize(600, 600));
    G.setWindowFlags(Qt::FramelessWindowHint);
    G.show();

    return app.exec();
}
