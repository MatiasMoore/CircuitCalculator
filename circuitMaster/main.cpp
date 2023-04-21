#include <QCoreApplication>
#include <iostream>
#include <complex>
#include <QFile>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>
#include <stdio.h>

#include <windows.h>

class CircuitElement
{
    public:
    enum class ElemType
    {
        invalid, R, L, C
    };
    CircuitElement()
    {

    }
    CircuitElement(ElemType startType, double startValue)
    {
        this->type = startType;
        this->simpleResistance = startValue;
    }
    public:
    double simpleResistance;
    ElemType type;
    public:
    std::complex<double> calculateElemResistance(double frequency)
    {
        std::complex<double> res;
        switch (this->type) {
        case ElemType::R:
            res = {this->simpleResistance, 0};
            break;
        case ElemType::L:
            res = {0, this->simpleResistance};
            break;
        case ElemType::C:
            res = {0, -this->simpleResistance};
            break;
        }
        return res;
    }
    static ElemType elemTypeFromStr(QString typeStr)
    {
        if (typeStr == "R")
            return ElemType::R;
        else if (typeStr == "L")
            return ElemType::L;
        else if (typeStr == "C")
            return ElemType::C;
        else
            return ElemType::invalid;
    }
};

class CircuitConnection
{
    public:
    enum class ConnectionType
    {
        invalid, parallel, sequential, sequentialComplex
    };
    CircuitConnection()
    {

    }
    CircuitConnection(ConnectionType startType)
    {
        this->type = startType;
    }
    public:
    QString name;
    int id;
    ConnectionType type;
    QList<CircuitElement> elements;
    QList<CircuitConnection*> children;
    CircuitConnection* parent = NULL;
    std::complex<double> resistance = 0;
    std::complex<double> voltage;
    std::complex<double> current;
    bool isVoltageSet = false;
    bool isCurrentSet = false;
    public:
    void setVoltage(std::complex<double> newVolt)
    {
        this->voltage = newVolt;
        isVoltageSet = true;
    }

    void setCurrent(std::complex<double> newCurr)
    {
        this->current = newCurr;
        isCurrentSet = true;
    }

    std::complex<double> calculateResistance(double frequency)
    {
        // Считаем сопротивление равным нулю
        this->resistance = 0;

        // Для простого последовательного соединения
        if (this->type == ConnectionType::sequential)
        {
            // Сопротивление цепи равно сумме сопротивлений её элементов
            for (int i = 0; i < this->elements.count(); i++)
            {
                this->resistance += this->elements[i].calculateElemResistance(frequency);
            }
        }
        // Для сложного последовательного соединения
        else if(this->type == ConnectionType::sequentialComplex)
        {
            // Сопротивление цепи равно сумме сопротивлений её соединений-детей
            for (int i = 0; i < this->children.count(); i++)
            {
                this->resistance += this->children[i]->calculateResistance(frequency);
            }
        }
        // Для параллельного соединения
        else if (this->type == ConnectionType::parallel)
        {
            // Находим сумму обратных значений сопротивления соединений-детей
            std::complex<double> reverseSum = 0;
            for (int i = 0; i < this->children.count(); i++)
            {
                reverseSum += 1.0 / this->children[i]->calculateResistance(frequency);
            }
            // Находим сопротивление параллельной цепи
            this->resistance = 1.0 / reverseSum;
        }
        return this->resistance;
    }

    void calculateCurrentAndVoltage()
    {
        // Если есть соединение-родитель - "наследуем" значения тока или напряжения
        if (this->parent != NULL)
        {
            // Наследуем силу тока если родитель - последовательное соединение
            if (this->parent->type == CircuitConnection::ConnectionType::sequentialComplex)
                //this->current = parent->current;
                this->setCurrent(parent->current);
            // Наследуем напряжение если родитель - параллельное соединение
            else if (this->parent->type == CircuitConnection::ConnectionType::parallel)
                //this->voltage = parent->voltage;
                this->setVoltage(this->parent->voltage);
        }

        if (!this->isCurrentSet && !this->isVoltageSet)
        {
            qDebug() << "no voltage and current in" << this->id;
        }

        // Вычисляем оставшуюся неизвестную величину
        if (!this->isCurrentSet)
            this->current = this->voltage / this->resistance;
        else if (!this->isVoltageSet)
            this->voltage = this->current * this->resistance;


        // Рекурсивно вычисляем силу тока и напряжение всех детей, если они имеются
        bool hasChildren = this->children.count() != 0;
        if (hasChildren)
        {
            for(int i = 0; i < this->children.count(); i++)
            {
                CircuitConnection* child = this->children[i];
                child->calculateCurrentAndVoltage();
            }
        }

    }


    bool addElement(CircuitElement newElem)
    {
        this->elements.append(newElem);
    }

    bool addChild(CircuitConnection* newChildCircuit)
    {
        this->children.append(newChildCircuit);
    }

    static ConnectionType strToConnectionType(QString strType)
    {
        ConnectionType type;
        if (strType == "seq")
            type = ConnectionType::sequential;
        else if (strType == "par")
            type = ConnectionType::parallel;
        else
            type = ConnectionType::invalid;
        return type;
    }

    static CircuitConnection* connectionFromDocElement(QMap<int, CircuitConnection>& map, QDomNode node, CircuitConnection* parentPtr)
    {
        QDomElement element = node.toElement();
        QString nodeType = element.tagName();
        QDomNodeList children = node.childNodes();

        // Создаём новый объект соединения
        CircuitConnection newCircuit;

        // Присваеваем ему уникальный id
        int newId = map.keys().count() + 1;
        newCircuit.id = newId;

        // Его родителем является объект, который рекурсивно вызвал функцию
        newCircuit.parent = parentPtr;
        newCircuit.name = element.attribute("name", "");

        // Получаем значение напряжения, если указано
        double voltageAtr = element.attribute("voltage", "-1").toDouble();
         if (voltageAtr != -1)
             newCircuit.setVoltage(voltageAtr);

        // Добавляем объект в QMap всех соединений цепи
        map.insert(newId, newCircuit);
        qDebug() << "Added " << element.tagName();

        // Указатель на новый объект соединения в QMap для дальнейшего его заполнения
        CircuitConnection* circuit = &map[newId];

        // Определить тип соединения
        CircuitConnection::ConnectionType circuitType = CircuitConnection::strToConnectionType(nodeType);
        bool hasSeqInside = !node.firstChildElement("seq").isNull();
        bool hasParInside = !node.firstChildElement("par").isNull();
        if (circuitType == CircuitConnection::ConnectionType::sequential && (hasSeqInside || hasParInside))
            circuitType = CircuitConnection::ConnectionType::sequentialComplex;

        circuit->type = circuitType;

        // Для сложного последовательного соединения
        if (circuitType == CircuitConnection::ConnectionType::sequentialComplex || circuitType == CircuitConnection::ConnectionType::parallel)
        {
            // Рекурсивно обрабатываем каждого ребёнка текущей цепи
            for(int i = 0; i < children.count(); i++)
            {
                CircuitConnection* currChildPtr = connectionFromDocElement(map, children.at(i), circuit);
                if (currChildPtr != NULL)
                {
                    circuit->addChild(currChildPtr);
                }
            }
        }
        // Для простого последовательного соединения
        else if (circuitType == CircuitConnection::ConnectionType::sequential)
        {
            if (children.isEmpty())
            {
                std::string message;
                message.append("No elements inside connection at line ").append(std::to_string(element.lineNumber()));
                throw message;
            }
            // Добавляем все элементы в соединение
            for(int i = 0; i < children.count(); i++)
            {
                QDomElement currElem = children.at(i).toElement();
                if (currElem.tagName() == "elem")
                {
                    QDomElement elem = currElem.firstChildElement("type");
                    if (elem.isNull())
                    {
                        std::string message;
                        message.append("No element type at line ").append(std::to_string(currElem.lineNumber()));
                        throw message;
                    }
                    CircuitElement::ElemType type = CircuitElement::elemTypeFromStr(elem.text());
                    if (type == CircuitElement::ElemType::invalid)
                    {
                        std::string message;
                        message.append("Invalid elem type at line ").append(std::to_string(currElem.lineNumber()));
                        throw message;
                    }
                    double value = currElem.firstChildElement("value").text().toDouble();
                    circuit->addElement(CircuitElement(type, value));
                }
            }
        }
        return circuit;
    }

};





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

bool readInputFromFile(QString inputPath, QMap<int, CircuitConnection>& circuitMap)
{
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Input not found";
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

    CircuitConnection::connectionFromDocElement(circuitMap, rootElement, NULL);

    return 1;
}

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

bool writeOutputToFile(QString outputPath, QMap<int, CircuitConnection>& circuitMap)
{
    QFile outFile(outputPath);
    if (!outFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Input not found";
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


    circuitMap[*circuitMap.keyBegin()].calculateResistance(1);
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
