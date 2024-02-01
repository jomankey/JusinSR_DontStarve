#include "Export_Utility.h"





//SceneMgr
HRESULT scenemgr::Create_SceneMgr(LPDIRECT3DDEVICE9 pGraphicDev, CSceneMgr** ppManagementInstance)
{
	CSceneMgr* pManagement = CSceneMgr::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagementInstance = pManagement;

	return S_OK;
}

HRESULT scenemgr::Change_CurScene(CScene * pScene)
{
	return CSceneMgr::GetInstance()->Change_Scene(pScene);
}

CScene* scenemgr::Get_CurScene()
{
	return CSceneMgr::GetInstance()->Get_CurScene();
}

_int scenemgr::Update_Scene(const _float & fTimeDelta)
{
	return CSceneMgr::GetInstance()->Update_Scene(fTimeDelta);
}

void scenemgr::LateUpdate_Scene()
{
	CSceneMgr::GetInstance()->LateUpdate_Scene();
}

void scenemgr::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSceneMgr::GetInstance()->Render_Scene(pGraphicDev);
}



//ProtoMgr
HRESULT	proto::Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent* proto::Clone_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

// Renderer
void		renderer::Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		renderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		renderer::Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

// LightMgr
HRESULT		light::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

CLight* light::Get_Light(_int _iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(_iIndex);
}

_int light::Change_Light(const _float& fTimeDelta, _int _iIndex)
{
	return CLightMgr::GetInstance()->Change_Light(fTimeDelta, _iIndex);
}

TIME_STATE light::Get_TimeIndex()
{
	return CLightMgr::GetInstance()->Get_TimeIndex();
}

_int light::Get_Day()
{
	return CLightMgr::GetInstance()->Get_Day();
}

void Release_Utility()
{
	CLightMgr::DestroyInstance();
	CRenderer::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CSceneMgr::DestroyInstance();
}
