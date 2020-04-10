#ifndef FORMULA_H
#define FORMULA_H

/* абстраткный класс для работы с логическими выражениям */

#include <QString>
#include <QVector>
#include <QObject>
#include <QPointer>


/* работа с логическими выражениями */

enum {_pt_none=0,_pt_number=1};

struct TFormulaElement {

    int N;             // Номер по порядку
    int Tip;           // <0 операция
    QString ElStr;    //  переменная
    int R;             // Состояние переменной
    void clear();
    void assign(TFormulaElement *fe);

    QPointer<QObject>tag_object[2];
    int              tag_int[2];
    QString          tag_str[2];
    void            *tag_ptr[2];


};

class Formula
{
public:


    explicit Formula();
    explicit Formula(QString _stFormula);
    virtual ~Formula();
    void clear();
    void fromString(QString _stFormula);
    bool setElementsR();
    int  result();
    bool isEmpty() const;
    bool error{};
    QStringList errorLog;

protected:
    QString stFormula;
    virtual int _levst(long v);
    virtual int _stf(char ch);
    virtual char _fst(int f);
    virtual int _feOP(int OPTip, int R1, int R2);
    QVector<TFormulaElement*> vFormulaElement;
    QVector<TFormulaElement*> vFormulaElement2PolskaNota(const QVector<TFormulaElement*>& vFormulaElement);
    bool FormulaElementAccept(TFormulaElement* el);// определяем тип  и выставляем параметр
    virtual bool FormulaElementSetR(TFormulaElement* el);// выставляем значение
    int resultFormula(QVector<TFormulaElement*> &vFormulaElementP, int &N);
    virtual void err(QString st);
    QString convertedFormula();
};

#endif // FORMULA_H
