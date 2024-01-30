#pragma once

#include "Scene.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev,wstring _strSceneName);
	virtual ~CStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:

	HRESULT			Ready_Layer_Environment();
	HRESULT			Ready_Layer_GameLogic();
	HRESULT			Ready_Layer_UI();
	HRESULT			Ready_LightInfo();

private:
	HRESULT Load_Data();

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring _strSceneName);

private:
	virtual void Free() override;

};

