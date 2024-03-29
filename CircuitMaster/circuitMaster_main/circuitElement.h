#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H
#include <complex>
#include <QString>
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
    friend void COMPARE_ELEMENTS(CircuitElement const & expectedElement, CircuitElement const & actualElement); /*!< Функция для сравнения элементов при тестировании */

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
    CircuitElement(QDomNode const & node, double frequency);

    private:
    std::complex<double> resistance; /*!< Комплексное сопротивление элемента */
    ElemType type; /*!< Тип элемента */

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
    static ElemType elemTypeFromStr(QString const & typeStr);
};

#endif // CIRCUITELEMENT_H
