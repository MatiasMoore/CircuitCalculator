#include <QtTest>
#include "../circuitMaster_main/testFunctions.h"
#include "../circuitMaster_main/ioFunctions.h"
#include "../circuitMaster_main/circuitConnection.h"
#include "../circuitMaster_main/circuitElement.h"

/*!
*\file
*\brief Тесты для вычисления сопротивления соединений цепи
*/

class calculateResistance_tests : public QObject
{
    Q_OBJECT

private slots:
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

void calculateResistance_tests::sequential_R()
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

void calculateResistance_tests::sequential_L()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 10))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, 10);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::sequential_C()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::C, std::complex<double>( 0, -15))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, -15);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::sequential_RC()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::R, 5),
        CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -15))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(5, -15);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::sequential_LC()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 10)),
        CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -15))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, -5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::sequential_RL()
{
    auto connectionType = CircuitConnection::ConnectionType::sequential;
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::R, 5),
        CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 10))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
        connection.addElement(elements[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(5, 10);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::parallel_R()
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

void calculateResistance_tests::parallel_RC()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -15)))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(4.5, -1.5);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::parallel_LC()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 10))),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -15)))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(0, 30);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::parallel_RL()
{
    auto connectionType = CircuitConnection::ConnectionType::parallel;
    CircuitConnection children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 10)))
    };

    CircuitConnection connection(connectionType);
    for (int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
        connection.addChild(&children[i]);

    std::complex<double> actualRes = connection.calculateResistance();
    std::complex<double> expectedRes(4, 2);

    COMPARE_COMPLEX(expectedRes, actualRes, 0.001);

}

void calculateResistance_tests::parAndPar_sequentially()
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

void calculateResistance_tests::parAndSeq_sequentially()
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

void calculateResistance_tests::parAndPar_parallel()
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

void calculateResistance_tests::parAndSeq_parallel()
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

void calculateResistance_tests::complexTest1()
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

void calculateResistance_tests::complexTest2()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel);

    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential);
    CircuitElement elements[] = {
        CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 3)),
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
        CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -6)),
        CircuitElement(CircuitElement::ElemType::R, 7)
    };
    for (int i = 0; i < sizeof(elements2) / sizeof(elements2[0]); i++)
        seq2.addElement(elements2[i]);

    CircuitConnection seqComplex(CircuitConnection::ConnectionType::sequentialComplex);
    CircuitConnection seq_1(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 8));

    CircuitConnection par_1(CircuitConnection::ConnectionType::parallel);
    CircuitConnection par_1children[] = {
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 9)),
        CircuitConnection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -10)))
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


QTEST_APPLESS_MAIN(calculateResistance_tests)

#include "tst_calculateresistance_tests.moc"
