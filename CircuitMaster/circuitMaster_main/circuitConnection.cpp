#include "circuitConnection.h"

/*!
*\file
*\brief Реализация конструкторов и функций класса CircuitConnection
*/

CircuitConnection::CircuitConnection()
{

}

CircuitConnection::CircuitConnection(ConnectionType startType)
{
    this->type = startType;
}

CircuitConnection::CircuitConnection(ConnectionType startType, QString startName)
{
    this->type = startType;
    this->name = startName;
    this->hasCustomName = true;
}

CircuitConnection::CircuitConnection(ConnectionType startType, CircuitElement startElem)
{
    this->type = startType;
    this->addElement(startElem);
}

CircuitConnection::CircuitConnection(ConnectionType startType, std::complex<double> startResistance)
{
    this->type = startType;
    this->resistance = startResistance;
}

void CircuitConnection::setVoltage(std::complex<double> newVolt)
{
    this->voltage = newVolt;
    isVoltageSet = true;
}

void CircuitConnection::setCurrent(std::complex<double> newCurr)
{
    this->current = newCurr;
    isCurrentSet = true;
}

std::complex<double> CircuitConnection::calculateResistance()
{
    // Считаем сопротивление равным нулю
    this->resistance = 0;

    // Для простого последовательного соединения
    if (this->type == ConnectionType::sequential)
    {
        // Сопротивление цепи равно сумме сопротивлений её элементов
        for (auto iter = this->elements.cbegin(); iter != this->elements.cend(); iter++)
            this->resistance += iter->getElemResistance();
    }
    // Для сложного последовательного соединения
    else if(this->type == ConnectionType::sequentialComplex)
    {
        // Сопротивление цепи равно сумме сопротивлений её соединений-детей
        for (auto iter = this->children.begin(); iter != this->children.end(); iter++)
            this->resistance += (*iter)->calculateResistance();
    }
    // Для параллельного соединения
    else if (this->type == ConnectionType::parallel)
    {
        // Находим сумму обратных значений сопротивления соединений-детей
        std::complex<double> reverseSum = 0;
        for (auto iter = this->children.begin(); iter != this->children.end(); iter++)
            reverseSum += 1.0 / (*iter)->calculateResistance();

        // Находим сопротивление параллельной цепи
        this->resistance = 1.0 / reverseSum;
    }
    return this->resistance;
}

void CircuitConnection::calculateCurrentAndVoltage()
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
        throw QString("Unknown voltage and current without parent element");
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
        for (auto iter = this->children.begin(); iter != this->children.end(); iter++)
            (*iter)->calculateCurrentAndVoltage();
    }

}

void CircuitConnection::addElement(CircuitElement newElem)
{
    this->elements.append(newElem);
}

void CircuitConnection::addChild(CircuitConnection* newChildCircuit)
{
    this->children.append(newChildCircuit);
    newChildCircuit->parent = this;
}

CircuitConnection::ConnectionType CircuitConnection::strToConnectionType(QString strType)
{
    CircuitConnection::ConnectionType type;
    if (strType == "seq")
        type = CircuitConnection::ConnectionType::sequential;
    else if (strType == "par")
        type = CircuitConnection::ConnectionType::parallel;
    else
        type = CircuitConnection::ConnectionType::invalid;
    return type;
}

CircuitConnection* CircuitConnection::connectionFromDocElement(QMap<QString, CircuitConnection>& map, QDomNode node, double frequency)
{
    QDomElement element = node.toElement();
    QString nodeType = element.tagName();
    QDomNodeList children = node.childNodes();

    //Обработка ошибок
    if (nodeType == "elem")
        throw QString("Неверное расположение элемента цепи на строке %1. Элементы могут "
                      "располагаться только внутри простых последовательных соединений").arg(QString::number(element.lineNumber()));

    if (nodeType != "seq" && nodeType != "par")
        throw QString("Неизвестный тэг на строке %1").arg(QString::number(element.lineNumber()));

    // Создаём новый объект соединения
    CircuitConnection newCircuit;    

    // Получаем название соединения
    QString newName = element.attribute("name", "");
    // Создаем имя, если не указано пользователем
    if (newName == "")
    {
        int newId = map.keys().count() + 1;
        newName = QString::number(newId);
        newCircuit.hasCustomName = false;
    }
    else
    {
        newCircuit.hasCustomName = true;
    }
    newCircuit.name = newName;

    // Проверяем имя на уникальность
    QList<QString> usedNames = map.keys();
    for (int i = 0; i < usedNames.count(); i++)
    {
        if (usedNames[i] == newName)
            throw QString("Имя соединения должно быть уникальным. Повтор имени "
                          "соединения на строке %1.").arg(QString::number(element.lineNumber()));
    }

    // Получаем значение напряжения, если указано
    double voltageAtr = element.attribute("voltage", "-1").toDouble();
     if (voltageAtr != -1)
         newCircuit.setVoltage(voltageAtr);

    // Добавляем объект в QMap всех соединений цепи
    map.insert(newName, newCircuit);

    // Указатель на новый объект соединения в QMap для дальнейшего его заполнения
    CircuitConnection* circuit = &map[newName];

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
        if (children.isEmpty())
        {
            throw QString("Пустое соединение на строке %1").arg(QString::number(element.lineNumber()));
        }
        // Рекурсивно обрабатываем каждого ребёнка текущей цепи
        for(int i = 0; i < children.count(); i++)
            circuit->addChild(connectionFromDocElement(map, children.at(i), frequency));
    }
    // Для простого последовательного соединения
    else if (circuitType == CircuitConnection::ConnectionType::sequential)
    {
        if (children.isEmpty())
        {
            throw QString("Отсутсвуют элементы соединения на строке %1").arg(QString::number(element.lineNumber()));
        }
        // Добавляем все элементы в соединение
        for(int i = 0; i < children.count(); i++)
        {
            circuit->addElement(CircuitElement(children.at(i), frequency));
        }
    }
    return circuit;
}

