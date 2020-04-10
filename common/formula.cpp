/* работа с логическими выражениями */

#include "formula.h"
#include <deque>

enum  {
    _CI = -3,
    _CO = -4,
    _AN = -5,
    _OR = -6,
    _EQ = -7,
    _NT = -8,
    _PL = -10,
    _MN = -11,
    _MU = -12,
    _DV = -13,
    _BG = -14,
    _LT = -15
};

void TFormulaElement::clear()
{
    N=0;Tip=_pt_none;
    ElStr.clear();
    R=0;
    for (int i=0;i<2;i++){
    tag_object[i]=nullptr;
    tag_int[i]=0;
    tag_str[i].clear();
    tag_ptr[i]=nullptr;
}
}

void TFormulaElement::assign(TFormulaElement *fe)
{
    N=fe->N;             // Номер по порядку
    Tip=fe->Tip;           // <0 операция
    ElStr=fe->ElStr;    // переменная
    for (int i=0;i<2;i++){
        tag_object[i]=fe->tag_object[i];
        tag_int[i]=fe->tag_int[i];
        tag_str[i]=fe->tag_str[i];
        tag_ptr[i]=fe->tag_ptr[i];
    }

    R=fe->R;             // Состояние переменной
}



Formula::Formula()
{
    vFormulaElement.clear();
}

Formula::~Formula()
{
    clear();

}

void Formula::clear()
{
    for (auto el : vFormulaElement) {
        delete el;
    }
    vFormulaElement.clear();
}

void Formula::fromString(QString _stFormula)
{
    error=false;
    stFormula=_stFormula;
    QVector<TFormulaElement*> _vFormulaElement;
    TFormulaElement fe;
    vFormulaElement.clear();
    int N = 1;
    QString st1 = "";
    fe.clear();
    fe.Tip=0;
    _stFormula=_stFormula.replace(" ","");
    for (auto && i : _stFormula) {
        if (_stf(i.toLatin1()) < 0) { // операция
            // если предидущий не команда или скобки вносим
            if (fe.Tip == 1) {
                fe.ElStr=st1.trimmed();
                auto *newfe=new TFormulaElement();
                newfe->assign(&fe);
                _vFormulaElement.push_back(newfe);
                fe.ElStr.clear();
                N++;st1 = "";
            }
            fe.Tip = _stf(i.toLatin1());
            fe.ElStr = i;
            fe.N = N;
            auto *newfe=new TFormulaElement();
            newfe->assign(&fe);
            _vFormulaElement.push_back(newfe);
            N++;st1 = "";
            fe.ElStr.clear();
        } else {
            fe.Tip = 1;
            fe.N = N;
            st1 += i;
        }
    }
    if (fe.Tip == 1) {
        fe.ElStr=st1.trimmed();
        auto *newfe=new TFormulaElement();
        newfe->assign(&fe);
        _vFormulaElement.push_back(newfe);
    }

    // переводим в польскую нотацию
    vFormulaElement=vFormulaElement2PolskaNota(_vFormulaElement);

}

QString Formula::convertedFormula()
{
    QString S;
    foreach (TFormulaElement *el, vFormulaElement) {
        if (el->Tip<0){
            S=S+_fst(el->Tip);
        } else {
            S=S+"["+el->ElStr+"]";
        }
    }
    return S;
}


// ставим значения
bool Formula::setElementsR()
{
    foreach (TFormulaElement *el, vFormulaElement) {
        if (!FormulaElementSetR(el)) return false;
    }
    return true;
}
// выявление скобок и команд
int Formula::_stf(char ch)
{
    switch (ch) {
    case '(': return _CI;
    case ')': return _CO;
    case '&': return _AN;
    case '|': return _OR;
    case '!': return _NT;
    case '=': return _EQ;
    case '>': return _BG;
    case '<': return _LT;

    case '+': return _PL;
    case '-': return _MN;
    case '*': return _MU;
    case '/': return _DV;
    default :
        return 1;
    }
}

char Formula::_fst(int f)
{
    switch (f) {
    case _CI: return '(';
    case _CO: return ')';
    case _AN: return '&';
    case _OR: return '|';
    case _NT: return '!';
    case _EQ: return '=';
    case _BG: return '>';
    case _LT: return '<';

    case _PL: return '+';
    case _MN: return '-';
    case _MU: return '*';
    case _DV: return '/';
    default :
        return '?';
    }
}

// уровень операции
int Formula::_levst(long v)
{
    switch (v) {
    case _CI: return 1;//"(";
    case _AN: return 3;//"&";
    case _OR: return 2;//"^";
    case _EQ: return 2;//"=";
    case _NT: return 4;//"!";
    case _BG: return 2;//">";
    case _LT: return 2;//"<";

    case _PL: return 2;//"+";
    case _MN: return 2;//"-";
    case _MU: return 3;//"*";
    case _DV: return 3;//"*";
    default :
        return 99;
    }
}

// обработка команды
int Formula::_feOP(int OPTip, int R1, int R2)
{
    switch (OPTip) {
    case _AN: return R1 && R2;//"&";
    case _OR: return R1 || R2;;//"^";
    case _NT: if (R1==0) return 1; else  return 0;//"!";
    case _EQ: if (R1==R2) return 1; else  return 0;//"=";

    case _BG: if (R1>R2) return 1; else  return 0;//">";
    case _LT: if (R1<R2) return 1; else  return 0;//"<";

    case _PL: return R1 + R2;;//"+";
    case _MN: return R2 - R1;;//"-";
    case _MU: return R1 * R2;;//"*";
    case _DV: {if (R1!=0) return R2 / R1; return 0;}//"/";


    default :
        return 33;
    }

}

QVector<TFormulaElement *> Formula::vFormulaElement2PolskaNota(const QVector<TFormulaElement *>& v)
{
    std::deque<TFormulaElement*>  vStack;
    bool bSCok;
    QVector<TFormulaElement*> vFormulaElementP;
    vFormulaElementP.clear();
    for (auto i : v){
        if (i->Tip < _pt_none) {
            // Если очеpедной символ - ')'то выталкиваем из стека в выходную стpоку
            // все знаки опеpаций до ближайшей откpывающей скобки
            if (i->Tip == _CO) {
                bSCok = false;
                while (!vStack.empty()) {
                    TFormulaElement *fe = vStack.back();
                    if (fe->Tip == _CI) {
                        vStack.pop_back();
                        bSCok = true;
                        break;
                    }
                    vFormulaElementP.push_back(fe);
                    vStack.pop_back();
                }
                if (!bSCok) {
                    err("несоотв скобок");
                    return vFormulaElementP;
                }

            } else
                // Если очеpедной символ - '(' , то заталкиваем её в стек
                if (i->Tip == _CI) {
                    vStack.push_back(i);
                } else
                    //если стек пуст, то опеpация из входной стpоки пеpеписывается в стек;
                    if (vStack.empty()) {
                        vStack.push_back(i);
                    } else {
                        // если пpиоpитет поступившей опеpации больше пpиоpитета опеpации на веpшине стека
                        // заталкиваем поступившую опеpацию на стек
                        TFormulaElement *fe = vStack.back();
                        if (_levst(fe->Tip) < _levst(i->Tip)) {
                            vStack.push_back(i);
                        } else {
                            // если пpиоpитет меньше пеpеписываем в выходную стpоку все опеpации
                            // с большим или pавным пpиоpитетом
                            while (!vStack.empty()) {
                                TFormulaElement *fe = vStack.back();
                                if (_levst(fe->Tip) < _levst(i->Tip)) break;
                                vFormulaElementP.push_back(fe);
                                vStack.pop_back();
                            }
                            //записываем в стек поступившую  опеpацию
                            vStack.push_back(i);

                        }
                    }
        } else {
            // Если очеpедной символ - буква , то пеpеписываем её в выходную стpоку
            vFormulaElementP.push_back(i);
        }
    }
    // после pассмотpения всего выpажения Пеpеписываем все опеpации из стека в выходную стpоку
    while (!vStack.empty()) {
        TFormulaElement *fe = vStack.back();
        if (fe->Tip == _CI) return vFormulaElementP;
        vFormulaElementP.push_back(fe);
        vStack.pop_back();
    }
    return vFormulaElementP;
}

bool Formula::FormulaElementAccept(TFormulaElement *el)
{
    // число то
    bool ok;
    int n=el->ElStr.toInt(&ok);
    if (ok){
        el->tag_int[0]=n;
        el->Tip=_pt_number;
        return true;
    }
    return false;
}

bool Formula::FormulaElementSetR(TFormulaElement *el)
{
    if (el->Tip==_pt_number) {
        el->R=el->tag_int[0];
        return true;
    }
    return false;
}

int Formula::resultFormula(QVector<TFormulaElement*> &vFormulaElementP, int &N)
{
    int R1, R2;
    if (N < 0)
        return 33;
    const TFormulaElement *feOp = vFormulaElementP.at(N) ;
    N--;
    // если текущий значение - выдаем значение
    if (feOp->Tip >= 1) return feOp->R;
    // если текущий цифра - выдаем значение
//    if (feOp->Tip == 2) return feOp->R;
    // если текущий цифра - выдаем значение
//    if (feOp->Tip == 3) return feOp->R;
    // если операция - собираем переменные
    R1 = resultFormula(vFormulaElementP,N);
    if (feOp->Tip == _NT) return _feOP(feOp->Tip, R1, 0);
    R2 = resultFormula(vFormulaElementP,N);
    return _feOP(feOp->Tip, R1, R2);
}

void Formula::err(QString st)
{
    error=true;
    errorLog.push_back(QString("%1 %2").arg(st).arg(convertedFormula()));
}



int  Formula::result()
{
    if (vFormulaElement.empty()) return 0;
    setElementsR();
    int N = vFormulaElement.size() - 1;
    return resultFormula(vFormulaElement, N);
}

bool Formula::isEmpty() const
{
    return vFormulaElement.isEmpty();
}
