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

CircuitConnection::CircuitConnection(ConnectionType startType, QString const & startName)
{
    this->type = startType;
    this->name = startName;
    this->hasCustomName = true;
}

CircuitConnection::CircuitConnection(ConnectionType startType, CircuitElement const & startElem)
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

        // Ошибка, если обратное сопротивление меньше 0
        if (reverseSum.real() == 0 && reverseSum.imag() == 0)
            throw QString("При расчете сопротивления параллельного соединения %1 получено недопустимое значение. "
                          "Проверьте правильность входных данных.").arg(this->name);

        // Находим сопротивление параллельной цепи
        this->resistance = 1.0 / reverseSum;
    }

    // Ошибка, если сопротивление меньше 0
    if (this->resistance.real() == 0 && this->resistance.imag() == 0)
        throw QString("При расчете сопротивления соединения %1 был получен 0. Проверьте правильность входных данных.").arg(this->name);

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
        throw QString("Недостаточно данных для вычисления силы тока и напряжения в соединении %1.").arg(this->name);

    // Вычисляем оставшуюся неизвестную величину
    if (!this->isCurrentSet)
        this->setCurrent(this->voltage / this->resistance);
    else if (!this->isVoltageSet)
        this->setVoltage(this->current * this->resistance);

    // Рекурсивно вычисляем силу тока и напряжение всех детей, если они имеются
    bool hasChildren = this->children.count() != 0;
    if (hasChildren)
    {
        for (auto iter = this->children.begin(); iter != this->children.end(); iter++)
            (*iter)->calculateCurrentAndVoltage();
    }

}

void CircuitConnection::addElement(CircuitElement const & newElem)
{
    this->elements.append(newElem);
}

void CircuitConnection::addChild(CircuitConnection* newChildCircuit)
{
    this->children.append(newChildCircuit);
    newChildCircuit->parent = this;
}

CircuitConnection::ConnectionType CircuitConnection::strToConnectionType(QString const & strType)
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

CircuitConnection* CircuitConnection::connectionFromDocElement(QMap<int, CircuitConnection>& map, QDomNode const & node, double frequency)
{
    // Основные переменные
    QDomElement element = node.toElement();
    QString nodeType = element.tagName();
    QDomNodeList children = node.childNodes();
    const QString elementLineNumStr = QString::number(element.lineNumber());

    //Обработка ошибок
    if (nodeType == "elem")
        throw QString("Неверное расположение элемента цепи на строке %1. Элементы могут "
                      "располагаться только внутри простых последовательных соединений.").arg(elementLineNumStr);

    if (nodeType != "seq" && nodeType != "par")
        throw QString("Неизвестный тэг на строке %1.").arg(elementLineNumStr);

    // Создаём новый объект соединения
    CircuitConnection newConnection;

    // Присваиваем идентификатор
    int newId = map.keys().count() + 1;
    newConnection.id = newId;

    // Получаем название соединения
    QString newName = element.attribute("name", "");
    // Создаем имя, если не указано пользователем
    if (newName == "")
    {
        newName = QString("%1_%2 на строке %3").arg(nodeType, QString::number(newId), elementLineNumStr);
        newConnection.hasCustomName = false;
    }
    else
    {
        newConnection.hasCustomName = true;
    }
    newConnection.name = newName;

    // Получаем значение напряжения, если указано
    double voltageAtr = element.attribute("voltage", "-1").toDouble();
     if (voltageAtr != -1)
     {
         // Ошибка, если значение меньше нуля
         if (voltageAtr <= 0)
            throw QString("Недопустимое значение напряжения у соединения на строке %1. Значение напряжения должно "
                          "быть больше 0.").arg(elementLineNumStr);

         newConnection.setVoltage(voltageAtr);
     }

    // Добавляем объект в QMap всех соединений цепи
    map.insert(newId, newConnection);

    // Указатель на новый объект соединения в QMap для дальнейшего его заполнения
    CircuitConnection* newConnectionPtr = &map[newId];

    // Определить тип соединения
    CircuitConnection::ConnectionType circuitType = CircuitConnection::strToConnectionType(nodeType);
    bool hasSeqInside = !node.firstChildElement("seq").isNull();
    bool hasParInside = !node.firstChildElement("par").isNull();
    if (circuitType == CircuitConnection::ConnectionType::sequential && (hasSeqInside || hasParInside))
        circuitType = CircuitConnection::ConnectionType::sequentialComplex;

    newConnectionPtr->type = circuitType;

    // Для сложного последовательного соединения
    if (circuitType == CircuitConnection::ConnectionType::sequentialComplex || circuitType == CircuitConnection::ConnectionType::parallel)
    {
        if (children.isEmpty())
        {
            throw QString("Пустое соединение на строке %1.").arg(elementLineNumStr);
        }
        // Рекурсивно обрабатываем каждого ребёнка текущей цепи
        for(int i = 0; i < children.count(); i++)
            newConnectionPtr->addChild(connectionFromDocElement(map, children.at(i), frequency));
    }
    // Для простого последовательного соединения
    else if (circuitType == CircuitConnection::ConnectionType::sequential)
    {
        if (children.isEmpty())
        {
            throw QString("Отсутсвуют элементы соединения на строке %1.").arg(elementLineNumStr);
        }
        // Добавляем все элементы в соединение
        for(int i = 0; i < children.count(); i++)
        {
            newConnectionPtr->addElement(CircuitElement(children.at(i), frequency));
        }
    }
    return newConnectionPtr;
}

