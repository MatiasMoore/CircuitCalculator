#include <QtTest>
#include "../circuitMaster_main/testFunctions.h"
#include "../circuitMaster_main/ioFunctions.h"
#include "../circuitMaster_main/circuitConnection.h"
#include "../circuitMaster_main/circuitElement.h"

// add necessary includes here
const QString testsDirPath = "tests";

class calculateElemResistance_tests : public QObject
{
    Q_OBJECT

private slots:
    void invalidResistance();

    void capacityForR();
    void inductivityForR();
    void noResistanceInfoForR();

    void capacityForL();
    void inductivityForL();
    void noResistanceInfoForL();

    void capacityForC();
    void inductivityForC();
    void noResistanceInfoForC();

    void bigExponentialR();
    void smallExponentialR();

    void intValueR();
    void intValueL();
    void intValueC();

    void floatValueR();
    void floatValueL();
    void floatValueC();

};

void calculateElemResistance_tests::invalidResistance()
{
    QString inputPath = testsDirPath + "\\invalidResistance.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::capacityForR()
{
    QString inputPath = testsDirPath + "\\capacityForR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), 50);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::inductivityForR()
{
    QString inputPath = testsDirPath + "\\inductivityForR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), 50);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::noResistanceInfoForR()
{
    QString inputPath = testsDirPath + "\\noResistanceInfoForR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::capacityForL()
{
    QString inputPath = testsDirPath + "\\capacityForL.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), 50);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::inductivityForL()
{
    QString inputPath = testsDirPath + "\\inductivityForL.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), 50);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 628));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::noResistanceInfoForL()
{
    QString inputPath = testsDirPath + "\\noResistanceInfoForL.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::capacityForC()
{
    QString inputPath = testsDirPath + "\\capacityForC.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), 50);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -0.318));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::inductivityForC()
{
    QString inputPath = testsDirPath + "\\inductivityForC.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), 50);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}

void calculateElemResistance_tests::noResistanceInfoForC()
{
    QString inputPath = testsDirPath + "\\noResistanceInfoForC.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    try {
        CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
        QVERIFY2(false, "Нет ожидаемого исключения");
    } catch (QString) {
        QVERIFY(true);
    }
}


void calculateElemResistance_tests::bigExponentialR()
{
    QString inputPath = testsDirPath + "\\bigExponentialR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::R, std::complex<double>(56000000, 0));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::smallExponentialR()
{
    QString inputPath = testsDirPath + "\\smallExponentialR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::R, std::complex<double>(0.00000056, 0));
    COMPARE_COMPLEX(expectedElem.getElemResistance(), actualElem.getElemResistance(), 0.0000000001);
}

void calculateElemResistance_tests::intValueR()
{
    QString inputPath = testsDirPath + "\\intValueR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::R, std::complex<double>(2, 0));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::intValueL()
{
    QString inputPath = testsDirPath + "\\intValueL.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 2));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::intValueC()
{
    QString inputPath = testsDirPath + "\\intValueC.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -2));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::floatValueR()
{
    QString inputPath = testsDirPath + "\\floatValueR.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::R, std::complex<double>(2.5, 0));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::floatValueL()
{
    QString inputPath = testsDirPath + "\\floatValueL.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::L, std::complex<double>(0, 2.5));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

void calculateElemResistance_tests::floatValueC()
{
    QString inputPath = testsDirPath + "\\floatValueC.xml";
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
        QVERIFY2(false, "Файл с тестами не найден");

    QDomDocument domDocument;
    if (!domDocument.setContent(&xmlFile))
        QVERIFY2(false, "Не удалось создать QDomDocument");

    CircuitElement actualElem = CircuitElement(domDocument.documentElement(), -1);
    CircuitElement expectedElem = CircuitElement(CircuitElement::ElemType::C, std::complex<double>(0, -2.5));
    COMPARE_ELEMENTS(expectedElem, actualElem);
}

/*
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
*/
QTEST_APPLESS_MAIN(calculateElemResistance_tests)

#include "tst_calculateelemresistance_tests.moc"
