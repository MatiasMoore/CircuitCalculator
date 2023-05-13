#include <QtTest>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitConnection.h>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitConnection.cpp>

#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitElement.h>
#include <F:\aeyy\a_lab_projects\KiNPO\circuitMaster\circuitElement.cpp>
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
    void test_case1();
    void test_case2();

};

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

void simpleTest::test_case1()
{
    QVERIFY2(CircuitElement::elemTypeFromStr("R") != CircuitElement::ElemType::R, "Failed to convert R element");
}

void simpleTest::test_case2()
{
    QVERIFY2(true, "It is false");
}

QTEST_APPLESS_MAIN(simpleTest)

#include "tst_simpletest.moc"
