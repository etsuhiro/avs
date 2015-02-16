//入力用
struct vertexIn
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

//出力用
struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

//頂点シェーダー
vertexOut vs_main(vertexIn IN)
{
	vertexOut OUT;

	OUT.pos = IN.pos;
	OUT.col = IN.col;

	return OUT;
}

float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	return Pos;
}

struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

//ピクセルシェーダー
float4 ps_main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

	//カラーをそのまま返す
	OUT.col = IN.col;

	return OUT.col;
}

float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

technique Technique1
{
	pass Pass1
	{
		// TODO: set renderstates here.

		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
}
