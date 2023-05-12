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
    std::complex<double> calculateElemResistance(double frequency);

    static ElemType elemTypeFromStr(QString typeStr);
};

#endif // CIRCUITELEMENT_H
