#include <QtTest>
#include "../circuitMaster_main/testFunctions.h"
#include "../circuitMaster_main/ioFunctions.h"
#include "../circuitMaster_main/circuitConnection.h"
#include "../circuitMaster_main/circuitElement.h"

// add necessary includes here

class calculateElemResistance_tests : public QObject
{
    Q_OBJECT

private slots:
    void invalidResistance();

    void intValueR();
    void intValueL();
    void intValueC();

    void floatValueR();
    void floatValueL();
    void floatValueC();

};

void calculateElemResistance_tests::invalidResistance()
{
    CircuitElement elem(CircuitElement::ElemType::R, -5);

    try {
        std::complex<double> actualRes = elem.calculateElemResistance();
        std::complex<double> expectedRes(5, 0);
        QVERIFY2(false, "No exception is thrown");
    } catch (...)
    {
        QVERIFY(true);
    }

//    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void calculateElemResistance_tests::intValueR()
{
    CircuitElement elem(CircuitElement::ElemType::R, 5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(5, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void calculateElemResistance_tests::intValueL()
{
    CircuitElement elem(CircuitElement::ElemType::L, 5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, 5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void calculateElemResistance_tests::intValueC()
{
    CircuitElement elem(CircuitElement::ElemType::C, 5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, -5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void calculateElemResistance_tests::floatValueR()
{
    CircuitElement elem(CircuitElement::ElemType::R, 5.5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(5.5, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void calculateElemResistance_tests::floatValueL()
{
    CircuitElement elem(CircuitElement::ElemType::L, 5.5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, 5.5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void calculateElemResistance_tests::floatValueC()
{
    CircuitElement elem(CircuitElement::ElemType::C, 5.5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, -5.5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

QTEST_APPLESS_MAIN(calculateElemResistance_tests)

#include "tst_calculateelemresistance_tests.moc"
