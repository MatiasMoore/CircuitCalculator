#include <QtTest>
#include "../circuitMaster_main/testFunctions.h"
#include "../circuitMaster_main/ioFunctions.h"
#include "../circuitMaster_main/circuitConnection.h"
#include "../circuitMaster_main/circuitElement.h"
// add necessary includes here
const QString testsDirPath = "F:\\aeyy\\a_lab_projects\\CircuitMaster\\tests";
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
    void test_case1();

    void simpleSeq();
    void simlpeSeqOneR();
    void simlpeSeqOneL();
    void simlpeSeqOneC();
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

void connectionFromDocElement_tests::test_case1()
{
    QString inputPath = testsDirPath + "\\test1.xml";
    qDebug() << inputPath;
    QMap<QString, CircuitConnection> circuitMap;
    readInputFromFile(inputPath, circuitMap);
}

void connectionFromDocElement_tests::incompleteTag()
{
    QString inputPath = testsDirPath + "\\incompleteTag.xml";
    QMap<QString, CircuitConnection> circuitMap;
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
    QMap<QString, CircuitConnection> circuitMap;
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
    QMap<QString, CircuitConnection> circuitMap;
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
    QMap<QString, CircuitConnection> circuitMap;
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
    QMap<QString, CircuitConnection> circuitMap;
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
    QMap<QString, CircuitConnection> circuitMap;
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
    QMap<QString, CircuitConnection> circuitMap;

    readInputFromFile(inputPath, circuitMap);
    qDebug() << complexToStr(circuitMap["seq1"].voltage);
}

void connectionFromDocElement_tests::simlpeSeqOneR()
{

}

void connectionFromDocElement_tests::simlpeSeqOneL()
{

}

void connectionFromDocElement_tests::simlpeSeqOneC()
{

}

void connectionFromDocElement_tests::simpleSeqRLC()
{

}

void connectionFromDocElement_tests::floatResistance()
{

}

void connectionFromDocElement_tests::floatVoltage()
{

}

void connectionFromDocElement_tests::simplePar()
{

}

void connectionFromDocElement_tests::triplePar()
{

}

void connectionFromDocElement_tests::twoSeqIn_complexSeq()
{

}

void connectionFromDocElement_tests::parAndSeqIn_complexSeq()
{

}

void connectionFromDocElement_tests::parAndParIn_Par()
{

}

void connectionFromDocElement_tests::complexTest1()
{

}

void connectionFromDocElement_tests::complexTest2()
{

}

QTEST_APPLESS_MAIN(connectionFromDocElement_tests)

#include "tst_connectionfromdocelement_tests.moc"
