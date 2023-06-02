#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H
#include <complex>
#include <QString>

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
    CircuitElement(ElemType startType, double startValue);

    public:
    double simpleResistance;
    ElemType type;

    public:
    /*!
    * \Рассчитывает сопротивление элемента цепи в виде комплексного числа
    * \return - полученное сопротивление
    */
    std::complex<double> calculateElemResistance();

    /*!
    * \Получить тип элемента на основе его текстового представления
    * \param[in] typeStr - строка, содержащая название типа
    * \return - тип элемента
    */
    static ElemType elemTypeFromStr(QString typeStr);
};

#endif // CIRCUITELEMENT_H
