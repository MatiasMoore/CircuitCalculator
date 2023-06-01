#include <QtTest>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitConnection.h>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitConnection.cpp>

#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitElement.h>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitElement.cpp>

#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\ioFunctions.h>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\ioFunctions.cpp>

// add necessary includes here

class simpleTest : public QObject
{
    Q_OBJECT

public:
    simpleTest();
    ~simpleTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    //calculateElemResistance
    void invalidResistance();

    void intValueR();
    void intValueL();
    void intValueC();

    void floatValueR();
    void floatValueL();
    void floatValueC();

    //calculateResistance
    void sequential_R();
    void sequential_L();
    void sequential_C();
    void sequential_RC();
    void sequential_LC();
    void sequential_RL();

    void parallel_R();
    void parallel_RC();
    void parallel_LC();
    void parallel_RL();

    void parAndPar_sequentially();
    void parAndSeq_sequentially();

    void parAndPar_parallel();
    void parAndSeq_parallel();

    void complexTest1();
    void complexTest2();

};

#define COMPARE_COMPLEX(expected, actual, epsilon) \
double realDelta = abs(expectedRes.real() - actualRes.real()); \
double imagDelta = abs(expectedRes.imag() - actualRes.imag()); \
char message[500]; \
sprintf(message, "Expected = %s Got = %s", complexToStr(expected).toStdString().c_str(), complexToStr(actual).toStdString().c_str()); \
QVERIFY2(realDelta < epsilon && imagDelta < epsilon, message);

simpleTest::simpleTest()
{

}

simpleTest::~simpleTest()
{

}

void simpleTest::initTestCase()
{

}

void simpleTest::cleanupTestCase()
{

}

//calculateElemResistance
void simpleTest::invalidResistance()
{
    CircuitElement elem(CircuitElement::ElemType::R, -5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(5, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void simpleTest::intValueR()
{
    CircuitElement elem(CircuitElement::ElemType::R, 5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(5, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void simpleTest::intValueL()
{
    CircuitElement elem(CircuitElement::ElemType::L, 5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, 5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void simpleTest::intValueC()
{
    CircuitElement elem(CircuitElement::ElemType::C, 5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, -5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void simpleTest::floatValueR()
{
    CircuitElement elem(CircuitElement::ElemType::R, 5.5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(5.5, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void simpleTest::floatValueL()
{
    CircuitElement elem(CircuitElement::ElemType::L, 5.5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, 5.5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

void simpleTest::floatValueC()
{
    CircuitElement elem(CircuitElement::ElemType::C, 5.5);

    std::complex<double> actualRes = elem.calculateElemResistance();
    std::complex<double> expectedRes(0, -5.5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);
}

//calculateResistance
void simpleTest::sequential_R()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::R, 5)
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(5, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::sequential_L()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::L, 10)
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, 10);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::sequential_C()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::C, 15)
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, -15);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::sequential_RC()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::R, 5),
        CircuitElement(CircuitElement::ElemType::C, 15)
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(5, -15);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::sequential_LC()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::L, 10),
        CircuitElement(CircuitElement::ElemType::C, 15)
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, -5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::sequential_RL()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::R, 5),
        CircuitElement(CircuitElement::ElemType::L, 10)
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(5, 10);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parallel_R()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 4)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(2.222, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parallel_RC()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::C, 15))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(4.5, -1.5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parallel_LC()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::L, 10)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::C, 15))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, 30);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parallel_RL()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::L, 10))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(4, 2);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parAndPar_sequentially()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children1[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 3)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 4))
    };
    for (int i = 0; i < sizeof(children1) / sizeof(children1[0]); i++)
        par1.addChild(&children1[i]);

    CircuitConnection par2(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children2[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 6))
    };
    for (int i = 0; i < sizeof(children2) / sizeof(children2[0]); i++)
        par2.addChild(&children2[i]);

    auto connectionType = CircuitConnection::ConnectionType::sequentialComplex;
    CircuitConnection connection(connectionType);
    connection.addChild(&par1);
    connection.addChild(&par2);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(4.441, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parAndSeq_sequentially()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children1[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 3)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 4))
    };
    for (int i = 0; i < sizeof(children1) / sizeof(children1[0]); i++)
        par1.addChild(&children1[i]);

    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5));

    auto connectionType = CircuitConnection::ConnectionType::sequentialComplex;
    CircuitConnection connection(connectionType);
    connection.addChild(&par1);
    connection.addChild(&seq1);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(6.714, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parAndPar_parallel()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children1[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 3)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 4))
    };
    for (int i = 0; i < sizeof(children1) / sizeof(children1[0]); i++)
        par1.addChild(&children1[i]);

    CircuitConnection par2(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children2[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 6))
    };
    for (int i = 0; i < sizeof(children2) / sizeof(children2[0]); i++)
        par2.addChild(&children2[i]);

    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection connection(connectionType);
    connection.addChild(&par1);
    connection.addChild(&par2);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(1.053, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::parAndSeq_parallel()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children1[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 3)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 4))
    };
    for (int i = 0; i < sizeof(children1) / sizeof(children1[0]); i++)
        par1.addChild(&children1[i]);

    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5));

    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection connection(connectionType);
    connection.addChild(&par1);
    connection.addChild(&seq1);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(1.277, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::complexTest1()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children1[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 3)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 4))
    };
    for (int i = 0; i < sizeof(children1) / sizeof(children1[0]); i++)
        par1.addChild(&children1[i]);

    CircuitConnection par2(CircuitConnection::ConnectionType::parallel);
    CircuitConnection children2[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 6))
    };
    for (int i = 0; i < sizeof(children2) / sizeof(children2[0]); i++)
        par2.addChild(&children2[i]);

    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential);
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::R, 7),
        CircuitElement(CircuitElement::ElemType::R, 8)
    };
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        seq1.addElement(elements[i]);

    CircuitConnection parComplex(CircuitConnection::ConnectionType::parallel);
    parComplex.addChild(&par2);
    parComplex.addChild(&seq1);

    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection connection(connectionType);
    connection.addChild(&par1);
    connection.addChild(&parComplex);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0.984, 0);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void simpleTest::complexTest2()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);

    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential);
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::L, 3),
        CircuitElement(CircuitElement::ElemType::R, 4)
    };
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        seq1.addElement(elements[i]);

    CircuitConnection children1[] = {
        seq1,
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5))
    };
    for (int i = 0; i < sizeof(children1) / sizeof(children1[0]); i++)
        par1.addChild(&children1[i]);



    CircuitConnection par2(CircuitConnection::ConnectionType::parallel);

    CircuitConnection seq2(CircuitConnection::ConnectionType::sequential);
    CircuitElement elements2[] = {
        CircuitElement(CircuitElement::ElemType::C, 6),
        CircuitElement(CircuitElement::ElemType::R, 7)
    };
    for (int i = 0; i < sizeof(elements2) / sizeof(elements2[0]); i++)
        seq2.addElement(elements2[i]);

    CircuitConnection seqComplex(CircuitConnection::ConnectionType::sequentialComplex);
    CircuitConnection seq_1(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 8));

    CircuitConnection par_1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection par_1children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 9)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::C, 10))
    };
    for (int i = 0; i < sizeof(par_1children) / sizeof(par_1children[0]); i++)
        par_1.addChild(&par_1children[i]);

    seqComplex.addChild(&seq_1);
    seqComplex.addChild(&par_1);

    CircuitConnection children2[] = {
        seq2,
        seqComplex
    };
    for (int i = 0; i < sizeof(children2) / sizeof(children2[0]); i++)
        par2.addChild(&children2[i]);



    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection connection(connectionType);
    connection.addChild(&par1);
    connection.addChild(&par2);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(1.951, 0.098);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

QTEST_APPLESS_MAIN(simpleTest)

#include "tst_simpletest.moc"
