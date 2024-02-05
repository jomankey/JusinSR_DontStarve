#pragma once
#include "VIBuffer.h"
#include "Engine_Define.h"

namespace Engine
{
    class CTerrainCubeTex :
        public CVIBuffer
    {
    private:
        explicit CTerrainCubeTex() = delete;
        explicit CTerrainCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
        explicit CTerrainCubeTex(const CTerrainCubeTex& rhs);
        virtual ~CTerrainCubeTex();

    public:
        virtual HRESULT		Ready_Buffer();
        virtual void		Render_Buffer();

    private:
        //        

    public:
        static CTerrainCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
        // CVIBuffer을(를) 통해 상속됨
        CComponent* Clone() override;

    private:
        virtual void	Free();
    };
}

