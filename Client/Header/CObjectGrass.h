#pragma once
#include "GameObject.h"
#include "ResObject.h"

namespace Engine {
	class CRcTex;
	class CTexture;
}

class CObjectGrass :
	public CResObject
{
private:
	explicit CObjectGrass(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CObjectGrass(const CObjectGrass& rhs);
	virtual ~CObjectGrass();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

private:
	HRESULT			Add_Component();
	void			Check_FrameState();
	void Ready_Stat();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[RESOBJECT_END];

	RESOBJECTSTATE m_eCurState;
	RESOBJECTSTATE m_ePreState;

	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;

	OBJSTAT m_Stat;


public:
	static CObjectGrass* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};