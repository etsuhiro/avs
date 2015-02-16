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
