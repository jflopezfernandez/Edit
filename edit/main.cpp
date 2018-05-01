#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QCoreApplication::setOrganizationName("FreeAndOpenSoftware");
    QCoreApplication::setApplicationName("Edit");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("<file>", "The file to open.");
    parser.process(application);

    MainWindow mainWindow;

    mainWindow.show();
    return application.exec();
}
