#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H
#include <QtTest>
#include <complex>
#include "ioFunctions.h"
#include "circuitConnection.h"
#include "circuitElement.h"

/*!
* \Сделать QVerify для комплексного числа
* \param[in] expected - ожидаемое комплексное число
* \param[in] actual - полученное комплексное число
* \param[in] epsilon - допустимая погрешность
*/
void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon);

void COMPARE_ELEMENTS(CircuitElement expected, CircuitElement actual);

void COMPARE_CONNECTION(CircuitConnection expected, CircuitConnection actual);

void COMPARE_CONNECTION_TREE(CircuitConnection expected, CircuitConnection actual);

#endif // TESTFUNCTIONS_H
