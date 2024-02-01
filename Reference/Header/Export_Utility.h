#pragma once

#include "CSceneMgr.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "LightMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "RvRcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "CustomTex.h"

#include "Texture.h"

#include "Transform.h"
#include "Camera.h"
#include "Calculator.h"

namespace Engine {

	// SceneMgr
	namespace scenemgr
	{
		inline HRESULT			Create_SceneMgr(LPDIRECT3DDEVICE9 pGraphicDev, CSceneMgr** ppManagementInstance);

		inline HRESULT			Change_CurScene(CScene* pScene);
		inline CScene*			Get_CurScene();

		inline _int				Update_Scene(const _float& fTimeDelta);
		inline void				LateUpdate_Scene();
		inline void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	}

	// ProtoMgr
	namespace proto
	{

		inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
		inline CComponent*		Clone_Proto(const _tchar* pProtoTag);
	}

	// Renderer
	namespace renderer
	{
		inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
		inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
		inline void		Clear_RenderGroup();
	}



	// LightMgr
	namespace light
	{
		inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
		inline CLight* Get_Light(_int _iIndex);
		inline _int Change_Light(const _float& fTimeDelta, _int _iIndex);
		inline TIME_STATE Get_TimeIndex();
	}



	// Release
	inline void				Release_Utility();


#include "Export_Utility.inl"

}