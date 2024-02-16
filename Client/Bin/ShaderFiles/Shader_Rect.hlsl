/* VertexShader, PixelShader */

// D3DDECLUSAGE

// float2x2, float3x3, float1x3, float4x4 == matrix
/* 셰이더의 전역변수 == 상수테이블 */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float g_fData = 0.0f;
texture g_Texture;
texture g_Texture1;

/* 각 텍스쳐마다 값을 어떻게 얻어와야하는지에 대해서 설정을 해줘야한다. */
sampler DefaultSampler = sampler_state
{
    texture = g_Texture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler DefaultSampler1 = sampler_state
{
    texture = g_Texture1;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};



struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

/* 정점셰이더 : 정점의 변환(월드, 뷰, 투영) + 정점의 구성정보를 변경한다. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    vector vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vWorldPos = vPosition;

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

/* /W 연산을 수행한다. (투영) */
/* 뷰포트 변환 (윈도웆좌표로) */
/* 래스탈ㄹ이ㅡㅈ -> 정점정보를 기반으로하여 둘러쌓여진 픽셀이 만들어짐. */

struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};



/* 픽셀셰이더 : 픽셀의 색을 결정하낟. */
vector PS_MAIN(PS_IN In) : COLOR0
{
    vector vColor = (vector) 0;

	
    vColor = tex2D(DefaultSampler, In.vTexcoord);

    In.vWorldPos.y;

    vColor = vector(vColor.r, vColor.g , vColor.b , vColor.a - 0.1f);



	// vColor = vector(1.0f, 0.f, 0.f, In.vTexcoord.y);

    return vColor;
}



/* 그래픽 카드에서 지원하는 셰이더 버젼에 따라 다른 방식으로 셰이더를 시작한다. */
technique DefaultTechnique
{
	/* Pass : 셰이더 기능의 캡슐화. */
	/* 명암 + 노멀맵핑 + 림라이트 + PBR + SSAO */
    pass DefaultPass
    {
		// fillmode = wireframe;
        //AlphaBlendEnable = true;
        //SrcBlend = SrcAlpha;
        //DestBlend = InvSrcAlpha;
        //bLENDoP = aDD;

		/* 위에서 작성한 정점 쉐이더의 버젼을 지정하고 진입점 함수를 지정한다. */
        VertexShader = compile vs_3_0 VS_MAIN();
		/* 위에서 작성한 픽셀 쉐이더의 버젼을 지정하고 진입점 함수를 지정한다. */
        PixelShader = compile ps_3_0 PS_MAIN();
    }

	/* 림라이트 + 디스토션 + 알파블렌딩 */
	//pass DefaultPass2
	//{
	//	/* 위에서 작성한 정점 쉐이더의 버젼을 지정하고 진입점 함수를 지정한다. */
	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	/* 위에서 작성한 픽셀 쉐이더의 버젼을 지정하고 진입점 함수를 지정한다. */
	//	PixelShader = compile ps_3_0 PS_MAIN();
	//}

}

