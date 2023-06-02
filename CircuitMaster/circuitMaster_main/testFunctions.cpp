#include "testFunctions.h"

void COMPARE_COMPLEX(std::complex<double> expected, std::complex<double> actual, double epsilon)
{
    double realDelta = abs(expected.real() - actual.real());
    double imagDelta = abs(expected.imag() - actual.imag());
    char message[500];
    sprintf(message, "Expected = %s Got = %s", complexToStr(expected).toStdString().c_str(), complexToStr(actual).toStdString().c_str());
    QVERIFY2(realDelta < epsilon && imagDelta < epsilon, message);
}
