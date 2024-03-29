#include "circuitElement.h"

/*!
*\file
*\brief Реализация конструкторов и функций класса CircuitElement
*/

CircuitElement::CircuitElement(ElemType startType, std::complex<double> startValue)
{
    this->type = startType;
    this->resistance = startValue;
}

CircuitElement::CircuitElement(QDomNode const & node, double frequency)
{
    // Инициализация основных переменных
    bool isFrequencyKnown = frequency != -1;
    QDomElement typeElem = node.firstChildElement("type");
    QDomElement resistanceElem = node.firstChildElement("res");
    QDomElement inductivityElem = node.firstChildElement("ind");
    QDomElement capacityElem = node.firstChildElement("cap");

    // Определение типа элемента
    this->type = elemTypeFromStr(typeElem.text());

    // Обработка ошибок ввода
    QString lineNumStr = QString::number(node.lineNumber());

    // Ошибка, если полученный тэг не является тэгом элемента
    QString elemTag = node.toElement().tagName();
    if (elemTag != "elem")
        throw QString("На строке %1 ожидался тэг элемента \"<elem>\", а был получен тэг \"<%2>\".").arg(lineNumStr, elemTag);

    // Ошибки указания данных
    switch (this->type){
    // Неверный тип элемента
    case CircuitElement::ElemType::invalid:
        throw QString("Неверный тип элемента на строке %1. Допустимые типы: \"R\", \"L\", \"C\".").arg(lineNumStr);
        break;

    // Для резистора
    case CircuitElement::ElemType::R:
        if (!inductivityElem.isNull())
            throw QString("Для резистора на строке %1 недопустимо указание индуктивности \"<ind>\". "
                          "Допускается только указание сопротивления \"<res>\".").arg(lineNumStr);
        if (!capacityElem.isNull())
            throw QString("Для резистора на строке %1 недопустимо указание емкости \"<cap>\". "
                          "Допускается только указание сопротивления \"<res>\".").arg(lineNumStr);
        if (resistanceElem.isNull())
            throw QString("Для резистора на строке %1 не указаны данные о сопротивлении. "
                          "Необходимо указание сопротивления \"<res>\".").arg(lineNumStr);
        break;

    // Для катушки
    case CircuitElement::ElemType::L:
        if (!capacityElem.isNull())
            throw QString("Для катушки индуктивности на строке %1 недопустимо указание емкости \"<cap>\". "
                          "Допускается указание сопротивления \"<res>\" или индуктивности \"<ind>\".").arg(lineNumStr);
        if (!inductivityElem.isNull() && !resistanceElem.isNull())
            throw QString("Для катушки индуктивности на строке %1 указано и сопротивление \"<res>\", и индуктивность \"<ind>\". "
                          "Допускается указание только одного из них.").arg(lineNumStr);
        if (inductivityElem.isNull() && resistanceElem.isNull())
            throw QString("Для катушки индуктивности на строке %1 не указаны данные о сопротивлении. "
                          "Необходимо указание сопротивления \"<res>\" или индуктивности \"<ind>\".").arg(lineNumStr);
        if (!isFrequencyKnown && !inductivityElem.isNull())
            throw QString("Для катушки индуктивности на строке %1 указана индуктивность \"<ind>\", однако "
                          "неизвестна частота переменного тока. Укажите сопротивления элемента \"<res>\" или "
                          "частоту \"frequency\" как атрибут корневого элемента цепи.").arg(lineNumStr);
        break;

    // Для конденсатора
    case CircuitElement::ElemType::C:
        if (!inductivityElem.isNull())
            throw QString("Для конденсатора на строке %1 недопустимо указание индуктивности \"<ind>\". "
                          "Допускается указание сопротивления \"<res>\" или емкости \"<cap>\".").arg(lineNumStr);
        if (!capacityElem.isNull() && !resistanceElem.isNull())
            throw QString("Для конденсатора индуктивности на строке %1 указано и сопротивление \"<res>\", и индуктивность \"<ind>\". "
                          "Допускается указание только одного из них.").arg(lineNumStr);
        if (capacityElem.isNull() && resistanceElem.isNull())
            throw QString("Для конденсатора на строке %1 не указаны данные о сопротивлении. Необходимо указание "
                          "сопротивления \"<res>\" или емкости \"<cap>\".").arg(lineNumStr);
        if (!isFrequencyKnown && !capacityElem.isNull())
            throw QString("Для конденсатора на строке %1 указана емкость \"<cap>\", однако неизвестна частота "
                          "переменного тока. Укажите сопротивления элемента \"<res>\" или частоту \"frequency\" "
                          "как атрибут корневого элемента цепи.").arg(lineNumStr);
        break;
    }

    // Проверка на наличие лишних тэгов
    int expectedChildCount = !typeElem.isNull() + !resistanceElem.isNull() + !inductivityElem.isNull() + !capacityElem.isNull();
    if (node.childNodes().count() != expectedChildCount)
        throw QString("Количество тэгов элемента на строке %1 не соответсвует ожидаемому. "
                      "Возможно использованы неизвестные тэги или какой-то из тэгов написан несколько раз.").arg(lineNumStr);

    // Получение значений и обработка ошибок конвертации
    double inductivity = 0, capacity = 0, resistance = 0;
    // Для индуктивности
    if (!inductivityElem.isNull())
    {
        bool indCorrectValue;
        inductivity = inductivityElem.text().toDouble(&indCorrectValue);
        QString indLineStr = QString::number(inductivityElem.lineNumber());
        if (!indCorrectValue)
            throw QString("Неверный формат значения индуктивности на строке %1.").arg(indLineStr);
        if (inductivity <= 0)
            throw QString("Недопустимое значение индуктивности на строке %1. Значение индуктивности должно быть больше 0.").arg(indLineStr);
    }
    // Для емкости
    else if (!capacityElem.isNull())
    {
        bool capCorrectValue;
        capacity = capacityElem.text().toDouble(&capCorrectValue);
        QString capLineStr = QString::number(capacityElem.lineNumber());
        if (!capCorrectValue)
            throw QString("Неверный формат значения емкости на строке %1.").arg(capLineStr);
        if (capacity <= 0)
            throw QString("Недопустимое значение емкости на строке %1. Значение емкости должно быть больше 0.").arg(capLineStr);
    }
    // Для сопротивления
    else if (!resistanceElem.isNull())
    {
        bool resCorrectValue;
        resistance = resistanceElem.text().toDouble(&resCorrectValue);
        QString resLineStr = QString::number(resistanceElem.lineNumber());
        if (!resCorrectValue)
            throw QString("Неверный формат значения сопротивления на строке %1.").arg(resLineStr);
        if (resistance <= 0)
            throw QString("Недопустимое значение сопротивления на строке %1. Значение сопротивления должно быть больше 0.").arg(resLineStr);
    }

    // Рассчёт активного сопротивления
    float activeResistance = 0;
    if (!resistanceElem.isNull())
    {
        activeResistance = resistance;
    }
    else
    {
        switch (this->type){
        case CircuitElement::ElemType::L:
            activeResistance = 2 * 3.14 * frequency * inductivity;
            break;
        case CircuitElement::ElemType::C:
            activeResistance = 1 / (2 * 3.14 * frequency * capacity);
            break;
        }
    }

    // Преобразование в комплексное сопротивление, в зависимости от типа элемента
    switch (this->type) {
    case ElemType::R:
        this->resistance = {activeResistance, 0};
        break;
    case ElemType::L:
        this->resistance = {0, activeResistance};
        break;
    case ElemType::C:
        this->resistance = {0, -activeResistance};
        break;
    }
}

std::complex<double> CircuitElement::getElemResistance() const
{
    return this->resistance;
}

CircuitElement::ElemType CircuitElement::elemTypeFromStr(QString const & typeStr)
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
