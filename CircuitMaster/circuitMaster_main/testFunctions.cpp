#include "testFunctions.h"

void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon)
{
    double realDelta = abs(expected.real() - actual.real());
    double imagDelta = abs(expected.imag() - actual.imag());
    char message[500];
    sprintf(message, "Expected = %s Got = %s", complexToStr(expected).toStdString().c_str(), complexToStr(actual).toStdString().c_str());
    QVERIFY2(realDelta < epsilon && imagDelta < epsilon, message);
}

void COMPARE_ELEMENTS(CircuitElement expected, CircuitElement actual)
{
    QVERIFY2(expected.type == actual.type, "Element types don't match");
    QVERIFY2(expected.simpleResistance == actual.simpleResistance, "Element resistance values don't match");
}

void COMPARE_CONNECTION(CircuitConnection expected, CircuitConnection actual)
{
    QVERIFY2(expected.name == actual.name, "Connection names don't match");
    QVERIFY2(expected.voltage == actual.voltage, "Connection voltage values don't match");
    QVERIFY2(expected.elements.count() == actual.elements.count(), "Connections don't have the same amount of elements");
    for (int i = 0; i < expected.elements.count(); i++)
    {
        COMPARE_ELEMENTS(expected.elements[i], actual.elements[i]);
    }
}

void COMPARE_CONNECTION_TREE(CircuitConnection expected, CircuitConnection actual)
{
    COMPARE_CONNECTION(expected, actual);
    QVERIFY2(expected.children.count() == actual.children.count(), "Connections trees don't have the same amount of children");
    for (int i = 0; i < expected.children.count(); i++)
    {
        COMPARE_CONNECTION(*expected.children[i], *actual.children[i]);
    }
}
