#include "ioFunctions.h"

QString complexToStr(std::complex<double> num)
{
    QString str;
    double real = num.real();
    double imag = num.imag();
    if (imag == 0)
        str = QString::number(real);
    else
    {
        QString sign = imag > 0 ? "+" : "-";
        str = QString("%1 %2 %3i").arg(QString::number(real), sign, QString::number(abs(imag)));
    }
    return str;
}

void readInputFromFile(QString inputPath, QMap<QString, CircuitConnection>& circuitMap)
{
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        throw QString("Неверно указан файл для входных данных. Возможно указанного расположения не существует или нет прав на запись.");
    }

    // Setup DomDocument
    QDomDocument domDocument;
    QString errorMes;
    int errorLine;
    if (!domDocument.setContent(&xmlFile, &errorMes, &errorLine))
    {
        throw QString("%1 at line %2").arg(errorMes, QString::number(errorLine));
    }

    xmlFile.close();

    QDomElement rootElement = domDocument.documentElement();

    auto seqConnections = domDocument.elementsByTagName("seq");
    auto parConnections = domDocument.elementsByTagName("par");
    auto conns = { seqConnections, parConnections};
    int voltageAttrCount = 0;
    for (auto i = conns.begin(); i != conns.end(); i++)
    {
        for (int j = 0; j < i->count(); j++)
        {
            QDomNode node = i->at(j);
            if (node.attributes().contains("voltage"))
                voltageAttrCount++;
            if (voltageAttrCount > 1)
                throw QString("Повторное указание напряжения цепи на строке %1. Напряжение указывается лишь один раз для всей схемы").arg(QString::number(node.lineNumber()));
        }
    }
    if (voltageAttrCount == 0)
        throw QString("Не указано значение напряжения. Значение напряжения должно быть указано");
    CircuitConnection::connectionFromDocElement(circuitMap, rootElement);
}

void writeOutputToFile(QString outputPath, QMap<QString, CircuitConnection>& circuitMap)
{
    QFile outFile(outputPath);
    if (!outFile.open(QFile::WriteOnly | QFile::Text)) {
        throw QString("Неверно указан файл для выходных данных. Возможно указанного расположения не существует или нет прав на запись.");
    }

    auto keyIter = circuitMap.keyBegin();

    while (keyIter != circuitMap.keyEnd())
    {
        CircuitConnection& currCirc = circuitMap[*keyIter];
        if (currCirc.hasCustomName)
        {
            QString outLine = QString("%1 = %2\n").arg(currCirc.name, complexToStr(currCirc.current));
            outFile.write(outLine.toStdString().c_str());
        }
        keyIter++;
    }

    outFile.close();
}
