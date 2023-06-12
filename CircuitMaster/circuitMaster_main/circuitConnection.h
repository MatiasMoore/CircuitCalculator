#ifndef CIRCUITCONNECTION_H
#define CIRCUITCONNECTION_H
#include <QFile>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>
#include "circuitElement.h"

/*!
*\file
*\brief Переменные, заголовки конструкторов и функций класса CircuitConnection
*/

/*!
*\class CircuitConnection
*\brief Соединение цепи переменного тока
*
* Соединение цепи имеет название, тип, напряжение, силу тока и сопротивление. Оно также
* может содержать элементы класса CircutElement, указатели на вложенные соединения
* и указатель на соединения-родителя
*/
class CircuitConnection
{
    public:
    enum class ConnectionType
    {
        invalid, /*!< Неверный тип соединения */
        parallel, /*!< Параллельное соединение */
        sequential, /*!< Последовательное соединение */
        sequentialComplex /*!< Последовательное соединение со вложенными соединениями */
    };

    /*!
    * \brief Конструктор по умолчанию
    * \param[in] newVolt - новое значение напряжения
    */
    CircuitConnection();

    /*!
    * \brief Конструктор соединения определенного типа
    * \param[in] startType - тип соединения
    */
    CircuitConnection(ConnectionType startType);

    /*!
    * \brief Конструктор соединения определенного типа и имени
    * \param[in] startType - тип соединения
    * \param[in] startName - название соединения
    */
    CircuitConnection(ConnectionType startType, QString const & startName);

    /*!
    * \brief Конструктор соединения определенного типа с элементом
    * \param[in] startType - тип соединения
    * \param[in] startElem - элемент соединения
    */
    CircuitConnection(ConnectionType startType, CircuitElement const & startElem);

    /*!
    * \brief Конструктор соединения определенного типа с известным сопротивлением
    * \param[in] startType - тип соединения
    * \param[in] startResistance - сопротивление соединения
    */
    CircuitConnection(ConnectionType startType, std::complex<double> startResistance);

    public:
    int id; /*!< id соединения */
    QString name; /*!< Название соединения */    
    ConnectionType type; /*!< Тип соединения */
    QList<CircuitElement> elements; /*!< QList элементов соединения */
    QList<CircuitConnection*> children; /*!< QList указателей на соединения-детей */
    CircuitConnection* parent = NULL; /*!< Указатель на соединение-родителя */
    std::complex<double> resistance; /*!< Комплексное сопротивление соединения */
    std::complex<double> voltage; /*!< Комплексное напряжение соединения */
    std::complex<double> current; /*!< Комплексная сила тока соединения */
    bool hasCustomName = false; /*!< Указано ли имя пользователем */
    bool isVoltageSet = false; /*!< Известно ли напряжение */
    bool isCurrentSet = false; /*!< Известна ли сила тока */
    public:

    /*!
    * \brief Установить значение напряжения соединения
    * \param[in] newVolt - новое значение напряжения
    */
    void setVoltage(std::complex<double> newVolt);

    /*!
    * \brief Установить значение силы тока соединения
    * \param[in] newCurr - новое значение силы тока
    */
    void setCurrent(std::complex<double> newCurr);

    /*!
    * \brief Рассчитывает сопротивление соединения цепи в виде комплексного числа
    * \return - полученное сопротивление
    */
    std::complex<double> calculateResistance();

    /*!
    * \brief Рассчитывает силу тока и напряжение в виде комплексного числа для соединеия и всех его вложенных соединений
    * \return - результаты записываются в объекты класса
    */
    void calculateCurrentAndVoltage();

    /*!
    * \brief Добавить элемент в соединение
    * \param[in] newElem - новый элемент
    */
    void addElement(CircuitElement const & newElem);

    /*!
    * \brief Добавить соединение-ребенка
    * \param[in] newChildCircuit - указатель на соединение-ребенка
    */
    void addChild(CircuitConnection* newChildCircuit);

    /*!
    * \brief Получить тип соединение на основе его текстового представления
    * \param[in] strType - строка, содержащая название типа
    * \return - тип соединения
    */
    static ConnectionType strToConnectionType(QString const & strType);

    /*!
    * \brief Получить объекты класса из корневого элемента документа и записать в контейнер
    * \param[in,out] map - контейнер для записи соединений
    * \param[in] node - тэг элемента по кторому создается запись
    * \param[in] frequency - частота перемнного тока, если неизвестна передать значение -1
    * \return - указатель на созданный в map объект класса
    */
    static CircuitConnection* connectionFromDocElement(QMap<int, CircuitConnection>& map, QDomNode const & node, double frequency);

};

#endif // CIRCUITCONNECTION_H
