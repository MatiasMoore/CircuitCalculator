#include <QtTest>
#include "../circuitMaster_main/testFunctions.h"
#include "../circuitMaster_main/ioFunctions.h"
#include "../circuitMaster_main/circuitConnection.h"
#include "../circuitMaster_main/circuitElement.h"

/*!
*\file
*\brief Тесты для создания соединения на основе xml файла
*/

const QString testsDirPath = "..\\..\\testFiles\\connectionFromDocElement_tests";

class connectionFromDocElement_tests : public QObject
{
    Q_OBJECT

private slots:
    void incompleteTag();
    void invalidElemType();
    void incorrectValueFormat();
    void noVoltage();
    void elementsInParConnection();
    void invalidElemResistance();
//    void test_case1();

    void simpleSeq();
    void simpleSeqOneR();
    void simpleSeqOneL();
    void simpleSeqOneC();
    void simpleSeqRLC();

    void floatResistance();
    void floatVoltage();

    void simplePar();
    void triplePar();
    void twoSeqIn_complexSeq();
    void parAndSeqIn_complexSeq();
    void parAndParIn_Par();

    void complexTest1();
    void complexTest2();
};

//void connectionFromDocElement_tests::test_case1()
//{
//    QString inputPath = testsDirPath + "\\test1.xml";
//    qDebug() << inputPath;
//    QMap<int, CircuitConnection> circuitMap;
//    readInputFromFile(inputPath, circuitMap);
//}

void connectionFromDocElement_tests::incompleteTag()
{
    QString inputPath = testsDirPath + "\\incompleteTag.xml";
    QMap<int, CircuitConnection> circuitMap;
    try {
        readInputFromFile(inputPath, circuitMap);
        QVERIFY2(false, "No exception is thrown");
    } catch (QString) {
        QVERIFY(true);
    }

}

void connectionFromDocElement_tests::invalidElemType()
{
    QString inputPath = testsDirPath + "\\invalidElemType.xml";
    QMap<int, CircuitConnection> circuitMap;
    try {
        readInputFromFile(inputPath, circuitMap);
        QVERIFY2(false, "No exception is thrown");
    } catch (QString) {
        QVERIFY(true);
    }
}

void connectionFromDocElement_tests::incorrectValueFormat()
{
    QString inputPath = testsDirPath + "\\incorrectValueFormat().xml";
    QMap<int, CircuitConnection> circuitMap;
    try {
        readInputFromFile(inputPath, circuitMap);
        QVERIFY2(false, "No exception is thrown");
    } catch (QString) {
        QVERIFY(true);
    }
}

void connectionFromDocElement_tests::noVoltage()
{
    QString inputPath = testsDirPath + "\\noVoltage.xml";
    QMap<int, CircuitConnection> circuitMap;
    try {
        readInputFromFile(inputPath, circuitMap);
        QVERIFY2(false, "No exception is thrown");
    } catch (QString) {
        QVERIFY(true);
    }
}

void connectionFromDocElement_tests::elementsInParConnection()
{
    QString inputPath = testsDirPath + "\\elementsInParConnection.xml";
    QMap<int, CircuitConnection> circuitMap;
    try {
        readInputFromFile(inputPath, circuitMap);
        QVERIFY2(false, "No exception is thrown");
    } catch (QString) {
        QVERIFY(true);
    }
}

void connectionFromDocElement_tests::invalidElemResistance()
{
    QString inputPath = testsDirPath + "\\invalidElemResistance.xml";
    QMap<int, CircuitConnection> circuitMap;
    try {
        readInputFromFile(inputPath, circuitMap);
        QVERIFY2(false, "No exception is thrown");
    } catch (QString) {
        QVERIFY(true);
    }
}

void connectionFromDocElement_tests::simpleSeq()
{
    QString inputPath = testsDirPath + "\\simpleSeq.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::R, 1));
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());

//    CircuitConnection& toCheck = *circuitMap.begin();
//    COMPARE_COMPLEX(std::complex<double>(20, 0), toCheck.voltage, 0.001);
}

void connectionFromDocElement_tests::simpleSeqOneR()
{
    QString inputPath = testsDirPath + "\\simpleSeqOneR.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());

}

void connectionFromDocElement_tests::simpleSeqOneL()
{
    QString inputPath = testsDirPath + "\\simpleSeqOneL.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 1)));

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());

}

void connectionFromDocElement_tests::simpleSeqOneC()
{
    QString inputPath = testsDirPath + "\\simpleSeqOneC.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -1)));

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());

}

void connectionFromDocElement_tests::simpleSeqRLC()
{
    QString inputPath = testsDirPath + "\\simpleSeqRLC.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::R, 1));
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 1)));
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -1)));

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

void connectionFromDocElement_tests::floatResistance()
{
    QString inputPath = testsDirPath + "\\floatResistance.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::R, 1.8));
    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

void connectionFromDocElement_tests::floatVoltage()
{
    QString inputPath = testsDirPath + "\\floatVoltage.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    expectedSeq.setVoltage(20.4);
    expectedSeq.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

void connectionFromDocElement_tests::simplePar()
{
    QString inputPath = testsDirPath + "\\simplePar.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedPar = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par1");
    expectedPar.setVoltage(20);

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    expectedPar.addChild(&seq1);
    expectedPar.addChild(&seq2);

    COMPARE_CONNECTION_TREE(expectedPar, *circuitMap.begin());
}

void connectionFromDocElement_tests::triplePar()
{
    QString inputPath = testsDirPath + "\\triplePar.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedPar = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par1");
    expectedPar.setVoltage(20);

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    CircuitConnection seq3 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq3");
    seq3.addElement(CircuitElement(CircuitElement::ElemType::R, 3));

    expectedPar.addChild(&seq1);
    expectedPar.addChild(&seq2);
    expectedPar.addChild(&seq3);

    COMPARE_CONNECTION_TREE(expectedPar, *circuitMap.begin());
}

void connectionFromDocElement_tests::twoSeqIn_complexSeq()
{
    QString inputPath = testsDirPath + "\\twoSeqIn_complexSeq.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequentialComplex, "seqComplex1");
    expectedSeq.setVoltage(20);

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    expectedSeq.addChild(&seq1);
    expectedSeq.addChild(&seq2);

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

void connectionFromDocElement_tests::parAndSeqIn_complexSeq()
{
    QString inputPath = testsDirPath + "\\parAndSeqIn_complexSeq.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequentialComplex, "seqComplex1");
    expectedSeq.setVoltage(20);

    CircuitConnection par1 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par1");

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    par1.addChild(&seq1);
    par1.addChild(&seq2);

    CircuitConnection seq3 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq3");
    seq3.addElement(CircuitElement(CircuitElement::ElemType::R, 3));

    expectedSeq.addChild(&par1);
    expectedSeq.addChild(&seq3);

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

void connectionFromDocElement_tests::parAndParIn_Par()
{
    QString inputPath = testsDirPath + "\\parAndParIn_Par.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedPar = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par1");
    expectedPar.setVoltage(20);

    CircuitConnection par2 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par2");

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    par2.addChild(&seq1);
    par2.addChild(&seq2);

    CircuitConnection par3 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par3");

    CircuitConnection seq3 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq3");
    seq3.addElement(CircuitElement(CircuitElement::ElemType::R, 3));

    CircuitConnection seq4 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq4");
    seq4.addElement(CircuitElement(CircuitElement::ElemType::R, 4));

    par3.addChild(&seq3);
    par3.addChild(&seq4);

    expectedPar.addChild(&par2);
    expectedPar.addChild(&par3);

    COMPARE_CONNECTION_TREE(expectedPar, *circuitMap.begin());
}

void connectionFromDocElement_tests::complexTest1()
{
    QString inputPath = testsDirPath + "\\complexTest1.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequentialComplex, "seqComplex1");
    expectedSeq.setVoltage(20);

    CircuitConnection par1 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par1");

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));

    CircuitConnection par2 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par2");

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 2));

    CircuitConnection seq3 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq3");
    seq3.addElement(CircuitElement(CircuitElement::ElemType::R, 3));

    par2.addChild(&seq2);
    par2.addChild(&seq3);

    par1.addChild(&seq1);
    par1.addChild(&par2);

    CircuitConnection seqComplex2 = CircuitConnection(CircuitConnection::ConnectionType::sequentialComplex, "seqComplex2");

    CircuitConnection par3 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par3");

    CircuitConnection seq4 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq4");
    seq4.addElement(CircuitElement(CircuitElement::ElemType::R, 4));

    CircuitConnection seq5 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq5");
    seq5.addElement(CircuitElement(CircuitElement::ElemType::R, 5));

    par3.addChild(&seq4);
    par3.addChild(&seq5);

    CircuitConnection seq6 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq6");
    seq6.addElement(CircuitElement(CircuitElement::ElemType::R, 6));

    seqComplex2.addChild(&par3);
    seqComplex2.addChild(&seq6);

    expectedSeq.addChild(&par1);
    expectedSeq.addChild(&seqComplex2);

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

void connectionFromDocElement_tests::complexTest2()
{
    QString inputPath = testsDirPath + "\\complexTest2.xml";
    QMap<int, CircuitConnection> circuitMap;

    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (QString str) {
        QVERIFY2(false, str.toStdString().c_str());
    }

    CircuitConnection expectedSeq = CircuitConnection(CircuitConnection::ConnectionType::sequentialComplex, "seqComplex1");
    expectedSeq.setVoltage(20);

    CircuitConnection par1 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par1");

    CircuitConnection par2 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par2");

    CircuitConnection seq1 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq1");
    seq1.addElement(CircuitElement(CircuitElement::ElemType::R, 1));
    seq1.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 2)));

    CircuitConnection par3 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par3");

    CircuitConnection seq2 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq2");
    seq2.addElement(CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -4)));
    seq2.addElement(CircuitElement(CircuitElement::ElemType::R, 6));

    CircuitConnection seq3 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq3");
    seq3.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 5)));

    par3.addChild(&seq2);
    par3.addChild(&seq3);

    par2.addChild(&seq1);
    par2.addChild(&par3);

    CircuitConnection seq4 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq4");
    seq4.addElement(CircuitElement(CircuitElement::ElemType::R, 3));

    par1.addChild(&par2);
    par1.addChild(&seq4);

    CircuitConnection par4 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par4");

    CircuitConnection seq5 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq5");
    seq5.addElement(CircuitElement(CircuitElement::ElemType::R, 1.2));
    seq5.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 3.4)));
    seq5.addElement(CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -8)));

    CircuitConnection par5 = CircuitConnection(CircuitConnection::ConnectionType::parallel, "par5");

    CircuitConnection seq6 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq6");
    seq6.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 8)));

    CircuitConnection seq7 = CircuitConnection(CircuitConnection::ConnectionType::sequential, "seq7");
    seq7.addElement(CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 6)));

    par5.addChild(&seq6);
    par5.addChild(&seq7);

    par4.addChild(&seq5);
    par4.addChild(&par5);

    expectedSeq.addChild(&par1);
    expectedSeq.addChild(&par4);

    COMPARE_CONNECTION_TREE(expectedSeq, *circuitMap.begin());
}

QTEST_APPLESS_MAIN(connectionFromDocElement_tests)

#include "tst_connectionfromdocelement_tests.moc"
