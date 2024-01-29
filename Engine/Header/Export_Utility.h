#pragma once

#include "Management.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "LightMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "RvRcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Texture.h"

#include "Transform.h"
#include "Camera.h"
#include "Pipeline.h"
#include "Calculator.h"

BEGIN(Engine)

// Management

inline CComponent*		Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
inline CLayer* Get_Layer(const _tchar* pLayerTag);

inline HRESULT			Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementInstance);

inline HRESULT			Set_Scene(CScene* pScene);
inline CScene*			Get_Scene();

inline _int				Update_Scene(const _float& fTimeDelta);
inline void				LateUpdate_Scene();
inline void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

// ProtoMgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_Proto(const _tchar* pProtoTag);

// Renderer
inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup();

// LightMgr
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
								const D3DLIGHT9* pLightInfo,
								const _uint& iIndex);

inline CLight* Get_Light(_int _iIndex);
inline _int Change_Light(const _float& fTimeDelta, _int _iIndex);


// Release
inline void				Release_Utility();


#include "Export_Utility.inl"

END