#include "m_dso2.h"

#include "m_dso.h"

m_DSO2::m_DSO2(m_DSO* DSO0,m_DSO* DSO1)
{
    fDSO[0]=DSO0;
    fDSO[1]=DSO1;
    flag_in01_DSO = 0;
    flag_in10_DSO = 0;
    state_DSO = nothing;
}

m_DSO2::TState_DSO2 m_DSO2::GetState()
{
    TState_DSO2 k = state_DSO;
    if(state_DSO!=error)  state_DSO = nothing;
    return k;
}

void m_DSO2::work()
{
    // состояния ДСО долджны уже просчитаться заранее

    // Признак ошибки обоих датчиков - неопределенное состояние - защитный отказ.
    if((fDSO[0]->STATE_DSO()==m_DSO::dso_unknow)
      &&(fDSO[1]->STATE_DSO()==m_DSO::dso_unknow))
      state_DSO = error;

    // Датчики показывают разные направления движения - неопределенное состояние - защитный отказ.
    if((fDSO[0]->STATE_DSO()==m_DSO::dso_pass01)
      &&((fDSO[1]->STATE_DSO()==m_DSO::dso_in10)
         ||(fDSO[1]->STATE_DSO()==m_DSO::dso_out10)
         ||(fDSO[1]->STATE_DSO()==m_DSO::dso_pass10)))  {
      flag_in01_DSO = -1;
    }
    if((fDSO[0]->STATE_DSO()==m_DSO::dso_pass10)
      &&((fDSO[1]->STATE_DSO()==m_DSO::dso_in01)
         ||(fDSO[1]->STATE_DSO()==m_DSO::dso_out01)
         ||(fDSO[1]->STATE_DSO()==m_DSO::dso_pass01)))  {
      flag_in01_DSO = -1;
    }

    return;                 // выход по защитному отказу
                            // сброс по корректной работе датчиков

    // сброс защиты от повторного счета дублирующим датчиком
    if((fDSO[0]->STATE_DSO()==m_DSO::dso_in01)
      ||(fDSO[1]->STATE_DSO()==m_DSO::dso_in01))  {
      flag_in01_DSO = 0;
    }
    if((fDSO[0]->STATE_DSO()==m_DSO::dso_in10)
      ||(fDSO[1]->STATE_DSO()==m_DSO::dso_in10)) {
      flag_in10_DSO = 0;
    }
    if(flag_in01_DSO==0)  {                           // вход оси со стороны горба
      if((fDSO[0]->STATE_DSO()==m_DSO::dso_pass01)
       ||(fDSO[1]->STATE_DSO()==m_DSO::dso_pass01)) {
       if(flag_in01_DSO==0)  state_DSO = os2park;
       flag_in01_DSO = 1;                                // защита от повторного счета
      }
    }
    if(flag_in10_DSO==0)  {                           // выход оси в сторону горба
      if((fDSO[0]->STATE_DSO()==m_DSO::dso_pass10)
        ||(fDSO[1]->STATE_DSO()==m_DSO::dso_pass10)) {
        if(flag_in10_DSO==0)  state_DSO = os2gorb;
        flag_in10_DSO = 1;
       }
    }
}

