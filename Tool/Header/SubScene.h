#pragma once
#include "Export_Utility.h"
#include "ToolCubeTerrain.h"

class CSubScene :
    public CScene
{
private:
    explicit CSubScene(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSubScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT Input_Mouse();
	_vec3 Picking_Terrain();

private:
	HRESULT			Ready_Layer_Environment();
	HRESULT			Ready_Layer_GameLogic();
	HRESULT			Ready_LightInfo();

private:
	void Save_File();
	HRESULT Load_File();

	HRESULT Create_Object(const _tchar* pName, _vec3 vPos);

private:
	CToolCubeTerrain* m_pCubeTerrain;
	Engine::CCalculator* m_pCalculatorCom;

	//_vec3 vLight[3] = { { 1.f, 1.f, 1.f} };

public:
	static CSubScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
    
};

