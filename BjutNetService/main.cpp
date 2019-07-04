
#include <QCoreApplication>
#include "ServiceInterface.h"

int main(int argc, char *argv[])
{
    bna::ServiceInterface si(argc, argv);
    return si.Run();
}
