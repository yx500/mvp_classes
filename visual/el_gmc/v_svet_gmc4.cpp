#include "v_svet_gmc4.h"

#include "mvp_objectfactory.h"
REGISTERELEMENT(v_Svet_GMC4,"Светофор ГМЦ4 ДБК","VISUAL Горка ГМЦ")


v_Svet_GMC4::v_Svet_GMC4(v_Base *parent) : v_Svet_Gor_4N(parent)
{
    FSTATE_DBK_ERROR=false;
}


void v_Svet_GMC4::updateStatesFromModel()
{
    v_Svet::updateStatesFromModel();
    SET_STATE_MODEL_BOOL(STATE_K);
    SET_STATE_MODEL_BOOL(STATE_J);
    SET_STATE_MODEL_BOOL(STATE_Z);
    SET_STATE_MODEL_BOOL(STATE_B);
    setSTATE_NAZAD(false);
    bool _FSTATE_DBK_ERROR=modelPropertyBool("STATE_DBK_ERROR0") |
                     modelPropertyBool("STATE_DBK_ERROR1") |
                     modelPropertyBool("STATE_DBK_ERROR2") |
                     modelPropertyBool("STATE_DBK_ERROR3");
    setSTATE_DBK_ERROR(_FSTATE_DBK_ERROR);
}
