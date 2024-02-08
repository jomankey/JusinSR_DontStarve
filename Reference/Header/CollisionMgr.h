#pragma once
#include "Base.h"
#include "Engine_Define.h"

namespace Engine 
{
	class ENGINE_DLL CCollisionMgr :
		public CBase
	{
		DECLARE_SINGLETON(CCollisionMgr)
	private:
		explicit CCollisionMgr();
		virtual ~CCollisionMgr();

	public:
		_bool Collision_Monster(
			_vec3 vPlayerPos,
			_vec3* vPlayerAxis,
			_vec3 vMonsterPos,
			_vec3* vMonsterAxis,
			_vec3 vPlayerScale,
			_vec3 vMonsterScale);
		_bool Collision_Item(_vec3 pPlayerPos, _vec3 pItemPos, _vec3 vPlayerScale, _vec3 vItemScale);

		_bool Collision_Mouse(_vec2 vMousePos, _float fX, _float fY, _float fSizeX, _float fSizeY);

	private:
		virtual void Free();
	};
}
