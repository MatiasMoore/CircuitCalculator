#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H
#include <QtTest>
#include <complex>
#include "ioFunctions.h"

/*!
* \Сделать QVerify для комплексного числа
* \param[in] expected - ожидаемое комплексное число
* \param[in] actual - полученное комплексное число
* \param[in] epsilon - допустимая погрешность
*/
void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon);

#endif // TESTFUNCTIONS_H
