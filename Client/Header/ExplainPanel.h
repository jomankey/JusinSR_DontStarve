#pragma once
#include "Export_Utility.h"

class CExplainPanel : public CGameObject
{
protected:
	explicit CExplainPanel(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, wstring strItemKey);
	explicit CExplainPanel(const CExplainPanel& rhs);
	virtual ~CExplainPanel();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

public:
	void Set_Show(bool _bShow) { m_bShow = _bShow; }

private:
	HRESULT			Add_Component();

public:
	static CExplainPanel* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 vPos, wstring strItemKey);
	virtual void Free() override;

private:
	_bool m_bShow = false;

	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;

	CREATEINFO m_tCreateInfo;
	wstring m_strItemKey;
};

