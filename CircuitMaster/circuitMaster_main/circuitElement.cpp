#include "circuitElement.h"

CircuitElement::CircuitElement(ElemType startType, double startValue)
{
    this->type = startType;
    this->simpleResistance = startValue;
}

std::complex<double> CircuitElement::calculateElemResistance()
{
    if (this->simpleResistance <= 0)
        throw QString("Invalid resistance value");
    std::complex<double> res;
    switch (this->type) {
    case ElemType::R:
        res = {this->simpleResistance, 0};
        break;
    case ElemType::L:
        res = {0, this->simpleResistance};
        break;
    case ElemType::C:
        res = {0, -this->simpleResistance};
        break;
    }
    return res;
}
CircuitElement::ElemType CircuitElement::elemTypeFromStr(QString typeStr)
{
    if (typeStr == "R")
        return ElemType::R;
    else if (typeStr == "L")
        return ElemType::L;
    else if (typeStr == "C")
        return ElemType::C;
    else
        return ElemType::invalid;
}
