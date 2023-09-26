#include <QCoreApplication>

#include <QNetworkAccessManager>

#include "bot.h"
#include "downloadhandler.h"
#include "resolver.h"
#include "searchhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QNetworkAccessManager m;
    Bot bot;
    Database db;
    Resolver r(bot);
    r.appendHandler(std::make_unique<SearchHandler>(db, m, bot));
    r.appendHandler(std::make_unique<DownloadHandler>(db, m, bot));

    // return EXIT_SUCCESS;
    return app.exec();
}
