#pragma once
#include "Export_Utility.h"

class CTerrainScene :
    public CScene
{
private:
    explicit CTerrainScene(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CTerrainScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT Input_Mouse();
	_vec3 Picking_Terrain();

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();

private:
	void Save_File();
	HRESULT Load_File();

private:
	Engine::CCalculator* m_pCalculatorCom;

public:
	static CTerrainScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

