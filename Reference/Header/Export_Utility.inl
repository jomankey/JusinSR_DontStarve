#include "Export_Utility.h"

CComponent * Get_Component(COMPONENTID eID, const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

HRESULT Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagementInstance)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagementInstance = pManagement;

	return S_OK;
}

HRESULT Set_Scene(CScene * pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int Update_Scene(const _float & fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

void LateUpdate_Scene()
{
	CManagement::GetInstance()->LateUpdate_Scene();
}

void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent*		Clone_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}


void Release_Utility()
{
	CProtoMgr::DestroyInstance();
	CManagement::DestroyInstance();
}
