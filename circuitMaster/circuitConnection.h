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
    void setVoltage(std::complex<double> newVolt);

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

    bool addElement(CircuitElement newElem);

    bool addChild(CircuitConnection* newChildCircuit);

    static ConnectionType strToConnectionType(QString strType);

    /*!
    * \Получить объекты класса из корневого элемента документа
    * \param[in|out] map - контейнер для записи соединений
    * \param[in] node - тэг элемента по кторому создается запись
    * \param[in] parentPtr - указатель на соединение-родителя
    * \return - указатель на созданный в map объект класса
    */
    static CircuitConnection* connectionFromDocElement(QMap<int, CircuitConnection>& map, QDomNode node, CircuitConnection* parentPtr);

};

#endif // CIRCUITCONNECTION_H
