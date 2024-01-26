#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRvRcTex : public CVIBuffer
{
private:
	explicit CRvRcTex();
	explicit CRvRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRvRcTex(const CRvRcTex& rhs);
	virtual ~CRvRcTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	static CRvRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone();

private:
	virtual void	Free();
};

END