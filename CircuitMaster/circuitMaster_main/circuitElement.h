#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H
#include <complex>
#include <QString>
#include <QtXml/QDomDocument>

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
    CircuitElement(ElemType startType, std::complex<double> startValue);

    CircuitElement(QDomNode node, double frequency);

    public:
    std::complex<double> resistance;
    ElemType type;

    public:
    /*!
    * \Получить тип элемента на основе его текстового представления
    * \param[in] typeStr - строка, содержащая название типа
    * \return - тип элемента
    */
    static ElemType elemTypeFromStr(QString typeStr);
};

#endif // CIRCUITELEMENT_H
