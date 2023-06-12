#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H
#include <QtTest>
#include <complex>
#include "ioFunctions.h"
#include "circuitConnection.h"
#include "circuitElement.h"

/*!
*\file
*\brief Заголовки функций для упрощения тестирования функций, связанных с цепью переменного тока
*/

/*!
* \brief Аналог QCOMPARE для комплексного числа
* \param[in] expected - ожидаемое комплексное число
* \param[in] actual - полученное комплексное число
* \param[in] epsilon - допустимая погрешность
*/
void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon);

/*!
* \brief Аналог QCOMPARE для элементов
* \param[in] expected - ожидаемый элемент
* \param[in] actual - полученный элемент
*/
void COMPARE_ELEMENTS(CircuitElement const & expected, CircuitElement const & actual);

/*!
* \brief Аналог QCOMPARE для соединений
* \param[in] expected - ожидаемое соединение
* \param[in] actual - полученное соединение
*/
void COMPARE_CONNECTION(CircuitConnection const & expected, CircuitConnection const & actual);

/*!
* \brief Аналог QCOMPARE для дерева соединений
* \param[in] expected - ожидаемое дерево соединений
* \param[in] actual - полученное дерево соединений
*/
void COMPARE_CONNECTION_TREE(CircuitConnection const & expected, CircuitConnection const & actual);

#endif // TESTFUNCTIONS_H
