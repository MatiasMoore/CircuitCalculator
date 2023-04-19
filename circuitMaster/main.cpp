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
        this->value = startValue;
    }
    public:
    double value;
    ElemType type;
    public:
    std::complex<double> calculateElemResistance(double frequency)
    {
        std::complex<double> res;
        switch (this->type) {
        case ElemType::R:
            res = {this->value, 0};
            break;
        case ElemType::L:
            //jXl1
            res = {0, this->value};
            break;
        case ElemType::C:
            //-jXc1
            res = {0, -this->value};
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
    CircuitConnection* next = NULL;
    std::complex<double> resistance = 0;
    std::complex<double> voltage = -1;
    std::complex<double> current = -1;
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

    double calculateCurrentAndVoltage()
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

        return 0;
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

};



CircuitConnection* parseConnectionChildren(QMap<int, CircuitConnection>& map, QDomNode node, CircuitConnection* parentPtr)
{
    QDomElement element = node.toElement();
    QString nodeType = element.tagName();
    QDomNodeList children = node.childNodes();

    // Если это элемент источник напряжения - сохранить напряжение и частоту
    if (nodeType == "volt")
    {
        if (parentPtr != NULL)
        {
            QDomElement voltageNode = node.firstChildElement("value");
            if (!voltageNode.isNull())
                parentPtr->setVoltage(voltageNode.toElement().text().toDouble());
        }
        return NULL;
    }


    // Создаём новый объект соединения
    CircuitConnection newCircuit;

    // Присваеваем ему уникальный id
    int newId = map.keys().count() + 1;
    newCircuit.id = newId;

    // Его родителем является объект, который рекурсивно вызвал функцию
    newCircuit.parent = parentPtr;
    newCircuit.name = element.attribute("name", "");

    if (newCircuit.name == "")
    {
        newCircuit.name = "unnamed #" + QString::number(newCircuit.id);
    }

    // Добавляем объект в QMap всех соединений цепи
    map.insert(newId, newCircuit);
    qDebug() << "Added " << element.tagName();

    // Указатель на новый объект соединения в QMap для дальнейшего его заполнения
    CircuitConnection* circuit = &map[newId];

    // Определить тип соединения
    CircuitConnection::ConnectionType circuitType = CircuitConnection::strToConnectionType(nodeType);
    if (circuitType == CircuitConnection::ConnectionType::sequential && (!node.firstChildElement("seq").isNull() || !node.firstChildElement("par").isNull()))
        circuitType = CircuitConnection::ConnectionType::sequentialComplex;

    circuit->type = circuitType;

    // Для простого последовательного соединения
    if (circuitType == CircuitConnection::ConnectionType::sequential)
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
                    qDebug() << "no elem type at" << currElem.lineNumber();
                }
                CircuitElement::ElemType type = CircuitElement::elemTypeFromStr(elem.text());
                if (type == CircuitElement::ElemType::invalid)
                {
                    qDebug() << "invalid elem type at" << currElem.lineNumber();
                }
                double value = currElem.firstChildElement("value").text().toDouble();
                circuit->addElement(CircuitElement(type, value));
            }
        }
    }
    // Для сложного последовательного соединения
    else if (circuitType == CircuitConnection::ConnectionType::sequentialComplex)
    {
        // Рекурсивно обрабатываем каждого ребёнка текущей цепи
        CircuitConnection* prevChild = NULL;
        for(int i = 0; i < children.count(); i++)
        {
            CircuitConnection* currChildPtr = parseConnectionChildren(map, children.at(i), circuit);
            if (prevChild != NULL)
            {
                prevChild->next = currChildPtr;
            }
            if (currChildPtr != NULL)
            {
                prevChild = currChildPtr;
                circuit->addChild(currChildPtr);
            }
        }
    }
    else if (circuitType == CircuitConnection::ConnectionType::parallel)
    {
        // Рекурсивно обрабатываем каждого ребёнка текущей цепи
        for(int i = 0; i < children.count(); i++)
        {
            circuit->addChild(parseConnectionChildren(map, children.at(i), circuit));
        }
    }

    return circuit;
}

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
    qDebug() << prefix + "Value =" << elem.value;
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
    bool hasNext = circ.next != NULL;
    if (hasNext)
    {
        qDebug() << prefix + "Next Id =" << circ.next->id;
    }

    setConsoleColor(7);
}

int main(int argc, char *argv[])
{
    /*
    try
    {
        throw "Invalid index";
    } catch (char const* exceptionStr)
    {
        printf("%s\n", exceptionStr);
    }
    return 0;
    */
    QMap<int, CircuitConnection> connects;


    // Load the input file
    QString inputPath = "F:\\aeyy\\a_lab_projects\\KiNPO\\circuitMaster\\test.xml";
    qDebug() << inputPath;
    QFile xmlFile(inputPath);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Input not found";
        return 0;
    }

    // Setup DomDocument
    QDomDocument domDocument;
    QString errorMes;
    int errorLine, errorColumn;
    if (!domDocument.setContent(&xmlFile, &errorMes, &errorLine, &errorColumn))
    {
        printf("%s at %d in %d\n", errorMes.toStdString().c_str(), errorLine, errorColumn);
        return 0;
    }
    QDomElement rootElement = domDocument.documentElement();

    try
    {
        parseConnectionChildren(connects, rootElement, NULL);
    }
    catch(std::string error)
    {
        puts(error.c_str());
        return 0;
    }

    qDebug() << "Num of connections ="<< connects.count();
    qDebug() << "-----------------------------------------";


    connects[*connects.keyBegin()].calculateResistance(1);
    connects[*connects.keyBegin()].calculateCurrentAndVoltage();

    printConnection(connects[*connects.keyBegin()], "  ");

    auto keyIter = connects.keyBegin();

    while (keyIter != connects.keyEnd())
    {
        CircuitConnection& currCirc = connects[*keyIter];
        if (currCirc.name.length() > 0)
        {
            qDebug() << currCirc.name + " = " + QString::number(currCirc.current.real()) + " " + QString::number(currCirc.current.imag());
        }
        keyIter++;
    }

    return 0;

}
//complex< double > z( 1.0, 2.0 );     // z = 1 + 2i
//cout << z              << std::endl; // Комплексное число выводится в виде вектора: (1, 2)
//cout << std::conj( z ) << std::endl; // Комплексно-сопряженное: (1, -2)
//cout << z.real()       << std::endl; // Действительная часть комплексного числа: 1
//cout << z.imag()       << std::endl; // Мнимая часть комплексного числа: 2
//printf("Hello world!\n");
//scanf("%d", &z);

//complex<double> a(2.0, 3.0);
//complex<double> b(5.0, -7.0);
//complex<double> res = a * b;
//printf("a = %f | %f\n", a.real(), a.imag());
//printf("b = %f | %f\n", b.real(), b.imag());
//printf("res = %f | %f\n", res.real(), res.imag());
