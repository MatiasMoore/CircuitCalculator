#include <QCoreApplication>
#include <iostream>
#include <complex>
#include <QFile>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>

#include <windows.h>

class CircuitElement
{
    public:
    enum class ElemType
    {
        R, L, C
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
    double calculateElemResistance(double frequency)
    {
        return this->value;
    }
    static ElemType elemTypeFromStr(QString typeStr)
    {
        if (typeStr == "R")
            return ElemType::R;
        else if (typeStr == "L")
            return ElemType::L;
        else if (typeStr == "C")
            return ElemType::C;
    }
};

class CircuitConnection
{
    public:
    enum class ConnectionType
    {
        parallel, sequential, sequentialComplex
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
    QString id;
    ConnectionType type;
    QList<CircuitElement> elements;
    QList<CircuitConnection*> children;
    CircuitConnection* parent = NULL;
    CircuitConnection* next = NULL;
    double resistance = 0;
    double voltage = -1;
    double current = -1;
    public:
    double calculateResistance(double frequency)
    {
        this->resistance = 0;
        if (this->type == ConnectionType::sequential)
        {
            for (int i = 0; i < this->elements.count(); i++)
            {
                this->resistance += this->elements[i].calculateElemResistance(frequency);
            }
        }
        else if(this->type == ConnectionType::sequentialComplex)
        {
            for (int i = 0; i < this->children.count(); i++)
            {
                this->resistance += this->children[i]->calculateResistance(frequency);
            }
        }
        else
        {
            double reverseSum = 0;
            for (int i = 0; i < this->children.count(); i++)
            {
                reverseSum += 1.0 / this->children[i]->calculateResistance(frequency);
            }
            this->resistance = 1.0 / reverseSum;
        }
        return this->resistance;
    }
    double calculateCurrentAndVoltage()
    {
        this->current = 0;

        if (this->parent != NULL)
        {
            if (this->parent->type == CircuitConnection::ConnectionType::sequentialComplex)
            {
                this->current = parent->current;
            }
            else if (this->parent->type == CircuitConnection::ConnectionType::parallel)
            {
                this->voltage = parent->voltage;
            }
        }

        if (this->current == -1 && this->voltage == -1)
        {
            qDebug() << "no voltage and current";
        }

        if (this->current == -1)
        {
            this->current = this->voltage / this->resistance;
        }
        else if (this->voltage == -1)
        {
            this->voltage = this->current * this->resistance;
        }

        this->current = this->voltage / this->resistance;

        //bool hasNext = this->next != NULL;
        //if (hasNext)
        //{
        //    next->voltage = this->current * next->resistance;
        //}

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
};

CircuitConnection* parseChildren(QMap<int, CircuitConnection>& map, QDomNode node, CircuitConnection* parentPtr)
{
    QDomElement element = node.toElement();
    QString nodeType = element.tagName();
    QDomNodeList children = node.childNodes();

    if (nodeType == "volt")
    {
        if (parentPtr != NULL)
        {
            QDomElement voltageNode = node.firstChildElement("value");
            if (!voltageNode.isNull())
                parentPtr->voltage = voltageNode.toElement().text().toDouble();
        }
        return NULL;
    }

    CircuitConnection newCircuit;

    int newId = map.keys().count() + 1;
    newCircuit.id = QString::number(newId);
    newCircuit.parent = parentPtr;
    newCircuit.name = element.attribute("name", "");
    map.insert(newId, newCircuit);
    qDebug() << "Added " << element.tagName();

    CircuitConnection* circuit = &map[newId];

    if (nodeType == "seq")
    {
        circuit->type = CircuitConnection::ConnectionType::sequential;
        bool isComplex = !node.firstChildElement("seq").isNull() || !node.firstChildElement("par").isNull();
        if (isComplex)
        {
            circuit->type = CircuitConnection::ConnectionType::sequentialComplex;
            CircuitConnection* prevChild = NULL;
            for(int i = 0; i < children.count(); i++)
            {
                CircuitConnection* currChildPtr = parseChildren(map, children.at(i), circuit);
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
        else
        {
            for(int i = 0; i < children.count(); i++)
            {
                QDomElement currElem = children.at(i).toElement();
                if (currElem.tagName() == "elem")
                {
                    CircuitElement::ElemType type = CircuitElement::elemTypeFromStr(currElem.firstChildElement("type").text());
                    double value = currElem.firstChildElement("value").text().toDouble();
                    circuit->addElement(CircuitElement(type, value));
                }
            }
        }
    }
    else if (nodeType == "par")
    {
        circuit->type = CircuitConnection::ConnectionType::parallel;
        for(int i = 0; i < children.count(); i++)
        {
            circuit->addChild(parseChildren(map, children.at(i), circuit));
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
    CircuitConnection::ConnectionType type = circ.type;
    QString typeStr;
    if (type == CircuitConnection::ConnectionType::parallel)
        typeStr = "parallel";
    else if (type == CircuitConnection::ConnectionType::sequential)
        typeStr = "sequential";
    else if (type == CircuitConnection::ConnectionType::sequentialComplex)
        typeStr = "sequentialComplex";
    qDebug() << prefix + "Type =" << typeStr;
    qDebug() << prefix + "Resistance =" << circ.resistance;
    qDebug() << prefix + "Voltage =" << circ.voltage;
    qDebug() << prefix + "Current =" << circ.current;
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
    domDocument.setContent(&xmlFile);
    QDomElement rootElement = domDocument.documentElement();
    QDomNode domNode = rootElement;
    QDomNodeList list = domNode.childNodes();

    parseChildren(connects, rootElement, NULL);
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
            qDebug() << currCirc.name + " = " + QString::number(currCirc.current);
        }
        keyIter++;
    }

    return 0;





    return 0;

    QDomElement domElement = domNode.toElement();

    qDebug() << domElement.tagName() + " | " + domElement.text();

    return 0;
    if (domElement.isNull())
         qDebug() << "last sibling";
    qDebug() << domElement.text();
    return 0;
    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        if (!domElement.isNull()) {
            //qDebug() << domElement.tagName();
            if (domElement.tagName() == "HEADER") {
                QDomNode node = domElement.firstChild();
                while (!node.isNull()) {
                    QDomElement element = node.toElement();
                    if (!element.isNull()) {
                        const QString tagName(element.tagName());
                        if (tagName == "NAME") {
                            qDebug() << "Name is:" << element.text();
                        } else if (tagName == "SEX") {
                            qDebug() << "Sex is:" << element.text();
                        }
                    }
                    node = node.nextSibling();
                }
            }
        }
        domNode = domNode.nextSibling();
    }
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
