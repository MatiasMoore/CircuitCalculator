#include <QCoreApplication>
#include <iostream>
#include <complex>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>
#include <stdio.h>

#include <windows.h>
#include "circuitElement.h"
#include "circuitConnection.h"
#include "ioFunctions.h"


void printElement(CircuitElement& elem, QString prefix)
{
    CircuitElement::ElemType type = elem.type;
    QString typeStr;
    if (type == CircuitElement::ElemType::R)
        typeStr = "R";
    else if (type == CircuitElement::ElemType::L)
        typeStr = "L";
    else if (type == CircuitElement::ElemType::C)
        typeStr = "C";
    qDebug() << prefix + "Type =" << typeStr;
    qDebug() << prefix + "Value =" << elem.simpleResistance;
}

QString increasePrefix(QString& prefix)
{
    QString increased = prefix;
    for (int i = 0; i < 3; i++)
    {
        increased = increased + prefix[0];
    }
    return increased;
}

QList<int> colors({1, 2, 3, 4, 5, 6, 9, 0xa, 0xb, 0xc, 0xd, 0xe});
int lastColorIndex = 0;
//0 = Black 8 = Gray
//1 = Blue 9 = Light Blue
//2 = Green a = Light Green
//3 = Aqua b = Light Aqua
//4 = Red c = Light Red
//5 = Purple d = Light Purple
//6 = Yellow e = Light Yellow
//7 = White f = Bright White
void setConsoleColor(int c)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
}
void printConnection(CircuitConnection& circ, QString prefix)
{
    int connectionColor = colors[lastColorIndex];
    if (lastColorIndex == colors.count() - 1)
        lastColorIndex = 0;
    else
        lastColorIndex++;

    setConsoleColor(connectionColor);

    bool hasParent = circ.parent != NULL;
    if (hasParent)
        qDebug() << prefix + "Parent Id =" << circ.parent->id;
    else
        qDebug() << "ROOT ELEMENT";
    qDebug() << prefix + "ID =" << circ.id;
    qDebug() << prefix + "name =" << circ.name;
    CircuitConnection::ConnectionType type = circ.type;
    QString typeStr;
    if (type == CircuitConnection::ConnectionType::parallel)
        typeStr = "parallel";
    else if (type == CircuitConnection::ConnectionType::sequential)
        typeStr = "sequential";
    else if (type == CircuitConnection::ConnectionType::sequentialComplex)
        typeStr = "sequentialComplex";
    qDebug() << prefix + "Type =" << typeStr;
    qDebug() << prefix + "Resistance =" << circ.resistance.real() << circ.resistance.imag();
    qDebug() << prefix + "Voltage =" << circ.voltage.real() << circ.voltage.imag();
    qDebug() << prefix + "Current =" << circ.current.real() << circ.current.imag();
    bool hasElements = circ.elements.count() != 0;
    if (hasElements)
    {
        qDebug() << prefix + "Elements--------------";
        for(int i = 0; i < circ.elements.count(); i++)
        {
            qDebug() << prefix + "Element #" + QString::number(i+1) + ":" ;
            printElement(circ.elements[i], increasePrefix(prefix));
        }
        qDebug() << prefix + "----------------------";
    }
    bool hasChildren = circ.children.count() != 0;
    if (hasChildren)
    {
        qDebug() << prefix + "Children==============";
        for(int i = 0; i < circ.children.count(); i++)
        {
            qDebug() << prefix + "Child #" + QString::number(i+1) + ":" ;
            printConnection(*circ.children[i], increasePrefix(prefix));
            setConsoleColor(connectionColor);
        }
        qDebug() << prefix + "======================";
    }
    //bool hasNext = circ.next != NULL;
    //if (hasNext)
    //{
    //    qDebug() << prefix + "Next Id =" << circ.next->id;
    //}

    setConsoleColor(7);
}

int main(int argc, char *argv[])
{
    QMap<int, CircuitConnection> circuitMap;

    // Load the input file
    QString inputPath = "F:\\aeyy\\a_lab_projects\\KiNPO\\circuitMaster\\test.xml";
    try {
        readInputFromFile(inputPath, circuitMap);
    } catch (std::string str) {
        puts(str.c_str());
        return 0;
    }


    qDebug() << "Num of connections ="<< circuitMap.count();
    qDebug() << "-----------------------------------------";


    circuitMap[*circuitMap.keyBegin()].calculateResistance();
    circuitMap[*circuitMap.keyBegin()].calculateCurrentAndVoltage();

    printConnection(circuitMap[*circuitMap.keyBegin()], "  ");

    writeOutputToFile("F:\\aeyy\\a_lab_projects\\KiNPO\\circuitMaster\\testOut.txt", circuitMap);

    auto keyIter = circuitMap.keyBegin();

    while (keyIter != circuitMap.keyEnd())
    {
        CircuitConnection& currCirc = circuitMap[*keyIter];
        if (currCirc.name.length() > 0)
        {
            qDebug() << currCirc.name + " = " + complexToStr(currCirc.current);
        }
        keyIter++;
    }

    return 0;

}