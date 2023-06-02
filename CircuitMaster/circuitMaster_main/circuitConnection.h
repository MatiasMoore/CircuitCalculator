#ifndef CIRCUITCONNECTION_H
#define CIRCUITCONNECTION_H
#include <QFile>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>
#include <qdebug.h>
#include "circuitElement.h"

class CircuitConnection
{
    public:
    enum class ConnectionType
    {
        invalid, parallel, sequential, sequentialComplex
    };
    CircuitConnection();

    CircuitConnection(ConnectionType startType);

    CircuitConnection(ConnectionType startType, CircuitElement startElem);

    CircuitConnection(ConnectionType startType, std::complex<double> startResistance);

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
    bool hasCustomName = false;
    bool isVoltageSet = false;
    bool isCurrentSet = false;
    public:
    /*!
    * \Установить значение напряжения соединения
    * \param[in] newVolt - новое значение напряжения
    */
    void setVoltage(std::complex<double> newVolt);

    /*!
    * \Установить значение силы тока соединения
    * \param[in] newCurr - новое значение силы тока
    */
    void setCurrent(std::complex<double> newCurr);

    /*!
    * \Рассчитывает сопротивление соединения цепи в виде комплексного числа
    * \return - полученное сопротивление
    */
    std::complex<double> calculateResistance();

    /*!
    * \Рассчитывает силу тока и напряжение в виде комплексного числа для соединеия и всех его вложенных соединений
    * \return - результаты записываются в объекты класса
    */
    void calculateCurrentAndVoltage();

    /*!
    * \Добавить элемент в соединение
    * \param[in] newElem - новый элемент
    */
    void addElement(CircuitElement newElem);

    /*!
    * \Добавить соединение-ребенка
    * \param[in] newChildCircuit - указатель на соединение-ребенка
    */
    void addChild(CircuitConnection* newChildCircuit);

    /*!
    * \Получить тип соединение на основе его текстового представления
    * \param[in] strType - строка, содержащая название типа
    * \return - тип соединения
    */
    static ConnectionType strToConnectionType(QString strType);

    /*!
    * \Получить объекты класса из корневого элемента документа
    * \param[in|out] map - контейнер для записи соединений
    * \param[in] node - тэг элемента по кторому создается запись
    * \param[in] parentPtr - указатель на соединение-родителя
    * \return - указатель на созданный в map объект класса
    */
    static CircuitConnection* connectionFromDocElement(QMap<QString, CircuitConnection>& map, QDomNode node);

};

#endif // CIRCUITCONNECTION_H
