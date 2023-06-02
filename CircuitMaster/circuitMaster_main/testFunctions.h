#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H
#include <QtTest>
#include <complex>
#include "ioFunctions.h"

void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon);

#endif // TESTFUNCTIONS_H
