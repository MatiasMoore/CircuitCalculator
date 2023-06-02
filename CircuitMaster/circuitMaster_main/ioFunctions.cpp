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

bool readInputFromFile(QString inputPath, QMap<int, CircuitConnection>& circuitMap)
{
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        throw QString("Input not found");
        return 0;
    }

    // Setup DomDocument
    QDomDocument domDocument;
    QString errorMes;
    int errorLine;
    if (!domDocument.setContent(&xmlFile, &errorMes, &errorLine))
    {
        char buff[90];
        sprintf(buff, "%s at line %d\n", errorMes.toStdString().c_str(), errorLine);
        std::string errorStr = buff;
        throw errorStr;
    }

    xmlFile.close();

    QDomElement rootElement = domDocument.documentElement();

    CircuitConnection::connectionFromDocElement(circuitMap, rootElement);

    return 1;
}

bool writeOutputToFile(QString outputPath, QMap<int, CircuitConnection>& circuitMap)
{
    QFile outFile(outputPath);
    if (!outFile.open(QFile::WriteOnly | QFile::Text)) {
        throw QString("Output not found");
        return 0;
    }

    auto keyIter = circuitMap.keyBegin();

    while (keyIter != circuitMap.keyEnd())
    {
        CircuitConnection& currCirc = circuitMap[*keyIter];
        if (currCirc.name.length() > 0)
        {
            QString outLine;// = currCirc.name + " = " + QString::number(currCirc.current.real()) + " " + QString::number(currCirc.current.imag());
            outLine = QString("%1 = %2\n").arg(currCirc.name, complexToStr(currCirc.current));
            outFile.write(outLine.toStdString().c_str());
        }
        keyIter++;
    }

    outFile.close();
}
