#include <QtTest>
#include "../circuitMaster_main/testFunctions.h"
#include "../circuitMaster_main/ioFunctions.h"
#include "../circuitMaster_main/circuitConnection.h"
#include "../circuitMaster_main/circuitElement.h"

/*!
*\file
*\brief Тесты для вычисления силы тока и напряжения в соединениях цепи
*/

class calculateCurrentAndVoltage_tests : public QObject
{
    Q_OBJECT

private slots:
    void unknownResistance();
    void onlyResistanceWithNoParent();

    void knownVoltage();
    void knownCurrent();

    void knownCurrentFrom_SeqParent();
    void knownVoltageFrom_ParParent();

    void parAndSeq_ParParent();
    void parAndPar_ParParent();
    void seqAndSeq_ParParent();

    void parAndSeq_SeqParent();
    void parAndPar_SeqParent();
    void seqAndSeq_SeqParent();

    void complexTest_1();

};

void calculateCurrentAndVoltage_tests::unknownResistance()
{
    CircuitConnection connection(CircuitConnection::ConnectionType::sequential);
    try {
        connection.calculateCurrentAndVoltage();
        QVERIFY2(false, "No exception is thrown");
    } catch (...)
    {
        QVERIFY(true);
    }
}

void calculateCurrentAndVoltage_tests::onlyResistanceWithNoParent()
{
    CircuitConnection connection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 5));
    connection.calculateResistance();
    try {
        connection.calculateCurrentAndVoltage();
        QVERIFY2(false, "No exception is thrown");
    } catch (...)
    {
        QVERIFY(true);
    }
}

void calculateCurrentAndVoltage_tests::knownVoltage()
{
    CircuitConnection connection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 10));
    connection.calculateResistance();
    connection.setVoltage(100);

    connection.calculateCurrentAndVoltage();

    std::complex<double> actualCurrent = connection.getCurrent();
    std::complex<double> expectedCurrent(10, 0);

    COMPARE_COMPLEX(expectedCurrent, actualCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::knownCurrent()
{
    CircuitConnection connection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 10));
    connection.calculateResistance();
    connection.setCurrent(10);

    connection.calculateCurrentAndVoltage();

    std::complex<double> actualVoltage = connection.getVoltage();
    std::complex<double> expectedVoltage(100, 0);

    COMPARE_COMPLEX(expectedVoltage, actualVoltage, 0.001);
}

void calculateCurrentAndVoltage_tests::knownCurrentFrom_SeqParent()
{
    CircuitConnection parent(CircuitConnection::ConnectionType::sequentialComplex);
    parent.setCurrent(10);

    CircuitConnection connection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 10));
    connection.setParent(&parent);
    connection.calculateResistance();

    connection.calculateCurrentAndVoltage();

    std::complex<double> actualVoltage = connection.getVoltage();
    std::complex<double> expectedVoltage(100, 0);

    COMPARE_COMPLEX(expectedVoltage, actualVoltage, 0.001);

    std::complex<double> actualCurrent = connection.getCurrent();
    std::complex<double> expectedCurrent(10, 0);

    COMPARE_COMPLEX(expectedCurrent, actualCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::knownVoltageFrom_ParParent()
{
    CircuitConnection parent(CircuitConnection::ConnectionType::parallel);
    parent.setVoltage(100);

    CircuitConnection connection(CircuitConnection::ConnectionType::sequential, CircuitElement(CircuitElement::ElemType::R, 10));
    connection.setParent(&parent);
    connection.calculateResistance();

    connection.calculateCurrentAndVoltage();

    std::complex<double> actualVoltage = connection.getVoltage();
    std::complex<double> expectedVoltage(100, 0);

    COMPARE_COMPLEX(expectedVoltage, actualVoltage, 0.001);

    std::complex<double> actualCurrent = connection.getCurrent();
    std::complex<double> expectedCurrent(10, 0);

    COMPARE_COMPLEX(expectedCurrent, actualCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::parAndSeq_ParParent()
{
    CircuitConnection par(CircuitConnection::ConnectionType::parallel, std::complex<double>(5, 0));
    CircuitConnection seq(CircuitConnection::ConnectionType::sequential, std::complex<double>(15, 0));

    CircuitConnection parent(CircuitConnection::ConnectionType::parallel, std::complex<double>(3.75, 0));
    parent.setVoltage(100);
    parent.addChild(&par);
    parent.addChild(&seq);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualSeqVoltage = seq.getVoltage();
    std::complex<double> expectedSeqVoltage(100, 0);
    COMPARE_COMPLEX(expectedSeqVoltage, actualSeqVoltage, 0.001);

    std::complex<double> actualSeqCurrent = seq.getCurrent();
    std::complex<double> expectedSeqCurrent(6.666, 0);
    COMPARE_COMPLEX(expectedSeqCurrent, actualSeqCurrent, 0.001);

    std::complex<double> actualParVoltage = par.getVoltage();
    std::complex<double> expectedParVoltage(100, 0);
    COMPARE_COMPLEX(expectedParVoltage, actualParVoltage, 0.001);

    std::complex<double> actualParCurrent = par.getCurrent();
    std::complex<double> expectedParCurrent(20, 0);
    COMPARE_COMPLEX(expectedParCurrent, actualParCurrent, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(26.666, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::parAndPar_ParParent()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel, std::complex<double>(5, 0));
    CircuitConnection par2(CircuitConnection::ConnectionType::parallel, std::complex<double>(15, 0));

    CircuitConnection parent(CircuitConnection::ConnectionType::parallel, std::complex<double>(3.75, 0));
    parent.setVoltage(100);
    parent.addChild(&par1);
    parent.addChild(&par2);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualPar1Voltage = par1.getVoltage();
    std::complex<double> expectedPar1Voltage(100, 0);
    COMPARE_COMPLEX(expectedPar1Voltage, actualPar1Voltage, 0.001);

    std::complex<double> actualPar1Current = par1.getCurrent();
    std::complex<double> expectedPar1Current(20, 0);
    COMPARE_COMPLEX(expectedPar1Current, actualPar1Current, 0.001);

    std::complex<double> actualPar2Voltage = par2.getVoltage();
    std::complex<double> expectedPar2Voltage(100, 0);
    COMPARE_COMPLEX(expectedPar2Voltage, actualPar2Voltage, 0.001);

    std::complex<double> actualPar2Current = par2.getCurrent();
    std::complex<double> expectedPar2Current(6.666, 0);
    COMPARE_COMPLEX(expectedPar2Current, actualPar2Current, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(26.666, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::seqAndSeq_ParParent()
{
    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential, std::complex<double>(5, 0));
    CircuitConnection seq2(CircuitConnection::ConnectionType::sequential, std::complex<double>(15, 0));

    CircuitConnection parent(CircuitConnection::ConnectionType::parallel, std::complex<double>(3.75, 0));
    parent.setVoltage(100);
    parent.addChild(&seq1);
    parent.addChild(&seq2);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualSeq1Voltage = seq1.getVoltage();
    std::complex<double> expectedSeq1Voltage(100, 0);
    COMPARE_COMPLEX(expectedSeq1Voltage, actualSeq1Voltage, 0.001);

    std::complex<double> actualSeq1Current = seq1.getCurrent();
    std::complex<double> expectedSeq1Current(20, 0);
    COMPARE_COMPLEX(expectedSeq1Current, actualSeq1Current, 0.001);

    std::complex<double> actualSeq2Voltage = seq2.getVoltage();
    std::complex<double> expectedSeq2Voltage(100, 0);
    COMPARE_COMPLEX(expectedSeq2Voltage, actualSeq2Voltage, 0.001);

    std::complex<double> actualSeq2Current = seq2.getCurrent();
    std::complex<double> expectedSeq2Current(6.666, 0);
    COMPARE_COMPLEX(expectedSeq2Current, actualSeq2Current, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(26.666, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::parAndSeq_SeqParent()
{
    CircuitConnection par(CircuitConnection::ConnectionType::parallel, std::complex<double>(5, 0));
    CircuitConnection seq(CircuitConnection::ConnectionType::sequential, std::complex<double>(15, 0));

    CircuitConnection parent(CircuitConnection::ConnectionType::sequentialComplex, std::complex<double>(20, 0));
    parent.setVoltage(100);
    parent.addChild(&par);
    parent.addChild(&seq);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualSeqVoltage = seq.getVoltage();
    std::complex<double> expectedSeqVoltage(75, 0);
    COMPARE_COMPLEX(expectedSeqVoltage, actualSeqVoltage, 0.001);

    std::complex<double> actualSeqCurrent = seq.getCurrent();
    std::complex<double> expectedSeqCurrent(5, 0);
    COMPARE_COMPLEX(expectedSeqCurrent, actualSeqCurrent, 0.001);

    std::complex<double> actualParVoltage = par.getVoltage();
    std::complex<double> expectedParVoltage(25, 0);
    COMPARE_COMPLEX(expectedParVoltage, actualParVoltage, 0.001);

    std::complex<double> actualParCurrent = par.getCurrent();
    std::complex<double> expectedParCurrent(5, 0);
    COMPARE_COMPLEX(expectedParCurrent, actualParCurrent, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(5, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::parAndPar_SeqParent()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel, std::complex<double>(5, 0));
    CircuitConnection par2(CircuitConnection::ConnectionType::parallel, std::complex<double>(15, 0));

    CircuitConnection parent(CircuitConnection::ConnectionType::sequentialComplex, std::complex<double>(20, 0));
    parent.setVoltage(100);
    parent.addChild(&par1);
    parent.addChild(&par2);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualPar1Voltage = par1.getVoltage();
    std::complex<double> expectedPar1Voltage(25, 0);
    COMPARE_COMPLEX(expectedPar1Voltage, actualPar1Voltage, 0.001);

    std::complex<double> actualPar1Current = par1.getCurrent();
    std::complex<double> expectedPar1Current(5, 0);
    COMPARE_COMPLEX(expectedPar1Current, actualPar1Current, 0.001);

    std::complex<double> actualPar2Voltage = par2.getVoltage();
    std::complex<double> expectedPar2Voltage(75, 0);
    COMPARE_COMPLEX(expectedPar2Voltage, actualPar2Voltage, 0.001);

    std::complex<double> actualPar2Current = par2.getCurrent();
    std::complex<double> expectedPar2Current(5, 0);
    COMPARE_COMPLEX(expectedPar2Current, actualPar2Current, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(5, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::seqAndSeq_SeqParent()
{
    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential, std::complex<double>(5, 0));
    CircuitConnection seq2(CircuitConnection::ConnectionType::sequential, std::complex<double>(15, 0));

    CircuitConnection parent(CircuitConnection::ConnectionType::sequentialComplex, std::complex<double>(20, 0));
    parent.setVoltage(100);
    parent.addChild(&seq1);
    parent.addChild(&seq2);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualSeq1Voltage = seq1.getVoltage();
    std::complex<double> expectedSeq1Voltage(25, 0);
    COMPARE_COMPLEX(expectedSeq1Voltage, actualSeq1Voltage, 0.001);

    std::complex<double> actualSeq1Current = seq1.getCurrent();
    std::complex<double> expectedSeq1Current(5, 0);
    COMPARE_COMPLEX(expectedSeq1Current, actualSeq1Current, 0.001);

    std::complex<double> actualSeq2Voltage = seq2.getVoltage();
    std::complex<double> expectedSeq2Voltage(75, 0);
    COMPARE_COMPLEX(expectedSeq2Voltage, actualSeq2Voltage, 0.001);

    std::complex<double> actualSeq2Current = seq2.getCurrent();
    std::complex<double> expectedSeq2Current(5, 0);
    COMPARE_COMPLEX(expectedSeq2Current, actualSeq2Current, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(5, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);
}

void calculateCurrentAndVoltage_tests::complexTest_1()
{
    CircuitConnection par1(CircuitConnection::ConnectionType::parallel, std::complex<double>(5, 0));

    CircuitConnection par2(CircuitConnection::ConnectionType::parallel, std::complex<double>(15, 0));
    CircuitConnection seq1(CircuitConnection::ConnectionType::sequential, std::complex<double>(20, 0));
    CircuitConnection parComplex(CircuitConnection::ConnectionType::parallel, std::complex<double>(8.571, 0));
    parComplex.addChild(&par2);
    parComplex.addChild(&seq1);

    CircuitConnection parent(CircuitConnection::ConnectionType::parallel, std::complex<double>(3.16, 0));
    parent.setVoltage(100);
    parent.addChild(&par1);
    parent.addChild(&parComplex);

    parent.calculateCurrentAndVoltage();

    std::complex<double> actualPar1Voltage = par1.getVoltage();
    std::complex<double> expectedPar1Voltage(100, 0);
    COMPARE_COMPLEX(expectedPar1Voltage, actualPar1Voltage, 0.001);

    std::complex<double> actualPar1Current = par1.getCurrent();
    std::complex<double> expectedPar1Current(20, 0);
    COMPARE_COMPLEX(expectedPar1Current, actualPar1Current, 0.001);

    std::complex<double> actualPar2Voltage = par2.getVoltage();
    std::complex<double> expectedPar2Voltage(100, 0);
    COMPARE_COMPLEX(expectedPar2Voltage, actualPar2Voltage, 0.001);

    std::complex<double> actualPar2Current = par2.getCurrent();
    std::complex<double> expectedPar2Current(6.666, 0);
    COMPARE_COMPLEX(expectedPar2Current, actualPar2Current, 0.001);

    std::complex<double> actualSeq1Voltage = seq1.getVoltage();
    std::complex<double> expectedSeq1Voltage(100, 0);
    COMPARE_COMPLEX(expectedSeq1Voltage, actualSeq1Voltage, 0.001);

    std::complex<double> actualSeq1Current = seq1.getCurrent();
    std::complex<double> expectedSeq1Current(5, 0);
    COMPARE_COMPLEX(expectedSeq1Current, actualSeq1Current, 0.001);

    std::complex<double> actualParentCurrent = parent.getCurrent();
    std::complex<double> expectedParentCurrent(31.645, 0);
    COMPARE_COMPLEX(expectedParentCurrent, actualParentCurrent, 0.001);

}

QTEST_APPLESS_MAIN(calculateCurrentAndVoltage_tests)

#include "tst_calculatecurrentandvoltage_tests.moc"
