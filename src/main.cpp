#include <QCoreApplication>

#include <QNetworkAccessManager>

#include "resolver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QNetworkAccessManager m;
    YouTubeFinder f(m, 0, "Tove Lo");
    Resolver r;

    //return EXIT_SUCCESS;
    return app.exec();
}
