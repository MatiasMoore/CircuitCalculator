#include "testFunctions.h"

/*!
*\file
*\brief Реализация функций для упрощения тестирования функций, связанных с цепью переменного тока
*/

void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon)
{
    double realDelta = abs(expected.real() - actual.real());
    double imagDelta = abs(expected.imag() - actual.imag());
    char message[500];
    sprintf(message, "Expected = %s Got = %s", complexToStr(expected).toStdString().c_str(), complexToStr(actual).toStdString().c_str());
    QVERIFY2(realDelta < epsilon && imagDelta < epsilon, message);
}

void COMPARE_ELEMENTS(CircuitElement const & expectedElement, CircuitElement const & actualElement)
{
    QCOMPARE(actualElement.type, expectedElement.type);
    COMPARE_COMPLEX(actualElement.resistance, expectedElement.resistance, 0.001);
}

void COMPARE_CONNECTION(CircuitConnection const & expectedConnection, CircuitConnection const & actualConnection)
{
    QCOMPARE(actualConnection.name, expectedConnection.name);
    QCOMPARE(actualConnection.type, expectedConnection.type);
    QCOMPARE(actualConnection.voltage, expectedConnection.voltage);
    QCOMPARE(actualConnection.elements.count(), expectedConnection.elements.count());
    for (int i = 0; i < expectedConnection.elements.count(); i++)
    {
        COMPARE_ELEMENTS(expectedConnection.elements[i], actualConnection.elements[i]);
    }
}

void COMPARE_CONNECTION_TREE(CircuitConnection const & expected, CircuitConnection const & actual)
{
    COMPARE_CONNECTION(expected, actual);
    char mess[500];
    sprintf(mess, "Connection \"%s\" doesn't have the expected amount of children", actual.name.toStdString().c_str());
    QVERIFY2(expected.children.count() == actual.children.count(), mess);
    for (int i = 0; i < expected.children.count(); i++)
    {
        COMPARE_CONNECTION_TREE(*expected.children[i], *actual.children[i]);
    }
}
