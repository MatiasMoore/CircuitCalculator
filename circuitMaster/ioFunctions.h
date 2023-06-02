#ifndef IOFUNCTIONS_H
#define IOFUNCTIONS_H
#include <complex>
#include <QString>
#include <QFile>
#include "circuitConnection.h"

QString complexToStr(std::complex<double> num);

bool readInputFromFile(QString inputPath, QMap<int, CircuitConnection>& circuitMap);

bool writeOutputToFile(QString outputPath, QMap<int, CircuitConnection>& circuitMap);

#endif // IOFUNCTIONS_H
