/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: fileFinder
 *** Description: Example for a Simple Function Block Type
 *** Version:
 ***     1.0: 2025-06-30/user -
 *************************************************************************/

#ifndef _FILEFINDER_H_
#define _FILEFINDER_H_

#include "devlog.h"
#include "simplefb.h"

#include "forte_string.h"
#include "forte_int.h"
#include "forte_bool.h"

#include "forte_array_at.h"

class FORTE_fileFinder : public CSimpleFB {
    DECLARE_FIRMWARE_FB(FORTE_fileFinder)

private:
    // Имена входов
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];

    // Имена выходов
    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

    // Имена событий
    static const TEventID scm_nEventREQID = 0;
    static const TEventID scm_nEventCNFID = 1;

    static const TDataIOID scm_anEIWith[];
    static const TForteInt16 scm_anEIWithIndexes[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TDataIOID scm_anEOWith[];
    static const TForteInt16 scm_anEOWithIndexes[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    virtual void setInitialValues();

    // Входы
    CIEC_STRING& st_DP() {
        return *static_cast<CIEC_STRING*>(getDI(0));
    }

    CIEC_STRING& st_FE() {
        return *static_cast<CIEC_STRING*>(getDI(1));
    }

    // Выходы
    CIEC_STRING& st_LF() {
        return *static_cast<CIEC_STRING*>(getDO(0));
    }

    CIEC_INT& st_Out() {
        return *static_cast<CIEC_INT*>(getDO(1));
    }

    CIEC_DATE_AND_TIME& st_LC() {
        return *static_cast<CIEC_DATE_AND_TIME*>(getDO(2));
    }

    CIEC_BOOL& st_TR() {
        return *static_cast<CIEC_BOOL*>(getDO(3));
    }


    // Основной алгоритм
    void alg_REQ(void);

    // Переменная для хранения предыдущего количества файлов
    TForteInt32 lastFileCount = -1;
    FORTE_BASIC_FB_DATA_ARRAY(1, 2, 4, 0, 0);
public:
    FORTE_fileFinder(CStringDictionary::TStringId pa_nInstanceNameId, CResource* pa_poSrcRes) :
        CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr, m_anFBConnData, m_anFBVarsData) {
    }
    virtual ~FORTE_fileFinder() = default;
};

#endif // _FILEFINDER_H_
