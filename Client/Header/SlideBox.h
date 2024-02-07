#pragma once
#include "CUI.h"
#include "Export_Utility.h"
#include <ExplainPanel.h>

class CSlideBox :public CGameObject
{

protected:
	explicit CSlideBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CSlideBox(const CSlideBox& rhs);
	virtual ~CSlideBox();

public:
	void Set_Item(CItem* pItem);
	
	_float Get_fX() { return m_fX; }
	_float Get_fY() { return m_fY; }

	void SetShow(bool _bIsShow) { m_bIsShow = _bIsShow; }

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT			Add_Component();

public:
	static CSlideBox* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free();

private:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

	const _tchar* ItemName;
	BOOL m_bIsShow = true;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;

	CItem* m_pItem;
	CExplainPanel* m_pPanel;
};

