#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

class CItemObject :
	public Engine::CGameObject
{
private:
	explicit CItemObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItemObject(const CItemObject& rhs);
	virtual ~CItemObject();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT Add_Component();
	void Load_Object(const _tchar* _path);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;
public:
	static CItemObject* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
private:
	wstring m_strPath;
	_float	m_fFrame;


};

