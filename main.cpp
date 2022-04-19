#include "mainwindow.h"
#include <QApplication>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>

#include <string>
using std::string;
using std::getline;

#include <vector>
using std::vector;

#include <limits>
using std::numeric_limits;

#include <stdexcept>

#include "argvparser.h"
using namespace CommandLineProcessing;

#include "Kuznyechik.hpp"
#include "mycrypto.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
