#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H
#include <complex>
#include <QString>
#include <QStringList>
#include <QtXml/QDomDocument>

/*!
*\file
*\brief Переменные, заголовки конструкторов и функций класса CircuitElement
*/

/*!
*\class CircuitElement
*\brief Элемент цепи переменного тока
*
* Элемент цепи имеет тип и сопротивление.
*/
class CircuitElement
{
    friend void COMPARE_ELEMENTS(CircuitElement expectedElement, CircuitElement actualElement); /*!< Функция для сравнения элементов при тестировании */

    public:
    enum class ElemType
    {
        invalid, /*!< Неверный тип элемента */
        R, /*!< Резистор */
        L, /*!< Катушка индуктивности */
        C /*!< Конденсатор */
    };

    /*!
    * \brief Конструктор элемента определенного типа с известным сопротивлением
    * \param[in] startType - тип элемента
    * \param[in] startValue - сопротивление элемента
    */
    CircuitElement(ElemType startType, std::complex<double> startValue);

    /*!
    * \brief Конструктор соединения на основе элемента xml файла
    * \param[in] node - элемент xml файла тэга \c <elem>
    * \param[in] frequency - частота переменного тока, если указана индуктивность или емкость. Если частота неизвестна, передавать -1
    */
    CircuitElement(QDomNode node, double frequency);

    private:
    std::complex<double> resistance; /*!< Комплексное сопротивление элемента */
    ElemType type; /*!< Тип элемента */

    CircuitElement resistorFromNode(QDomNode node)
    {
        QDomElement typeElem = node.firstChildElement("type");
        QDomElement resistanceElem = node.firstChildElement("res");

        QString lineNumStr = QString::number(node.lineNumber());
        QStringList unexpectedTags;
        auto children = node.childNodes();
        for (int i = 0; i < children.count(); i++)
        {
            QString childTag = children.at(i).toElement().tagName();
            if (childTag != "type" && childTag != "res")
                unexpectedTags.a
        }

        if (unexpectedTags.count() > 0)
            throw QString("У резистора на строке %1 присутствуют недопустимые тэги %2. "
                          "Для резистора допустимо указание типа \"<type>\" и сопротивления \"<res>\"").arg(lineNumStr);
    }

    CircuitElement coilFromNode(QDomNode node, double frequency)
    {
        bool isFrequencyKnown = frequency != -1;
        QDomElement typeElem = node.firstChildElement("type");
        QDomElement resistanceElem = node.firstChildElement("res");
        QDomElement inductivityElem = node.firstChildElement("ind");
        QDomElement capacityElem = node.firstChildElement("cap");
    }

    CircuitElement capacitorFromNode(QDomNode node, double frequency)
    {
        bool isFrequencyKnown = frequency != -1;
        QDomElement typeElem = node.firstChildElement("type");
        QDomElement resistanceElem = node.firstChildElement("res");
        QDomElement inductivityElem = node.firstChildElement("ind");
        QDomElement capacityElem = node.firstChildElement("cap");
    }


    public:
    /*!
    * \brief Получить комплексное сопротивление элемента
    * \return - комплексное сопротивление элемента
    */
    std::complex<double> getElemResistance() const;

    /*!
    * \brief Получить тип элемента на основе его текстового представления
    * \param[in] typeStr - строка, содержащая название типа
    * \return - тип элемента
    */
    static ElemType elemTypeFromStr(QString typeStr);
};

#endif // CIRCUITELEMENT_H
