/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                                   Cautions */
/* -------------------------------------------------------------------------- */
/* MEMO: 本ファイルを記述して・ランタイムでコンパイルする際には、下記に注意し */
/*       てください。                                                         */
/*       - 文字エンコード: UTF-8（BOMなし）                                   */
/*       - 改行コード: LF                                                     */
/*                                                                            */
/*       上記を満たしていない場合、シェーダのコンパイルが失敗する可能性が非常 */
/*       に高いです。                                                         */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*                                                            Compile-Options */
/* -------------------------------------------------------------------------- */
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

// #define COMPILEOPTION_CONVERT_SRGB_TO_RGB

/* -------------------------------------------------------------------------- */
/*                                                        Input (Vertex-Data) */
/* -------------------------------------------------------------------------- */
/* 頂点バインド */
/* MEMO: i_UV0の.zと.wにはUV座標以外のパラメータが入っていますので注意してください。 */
/*       .x: U座標                                                                   */
/*       .y: V座標                                                                   */
/*       .z: パーツカラーの合成種別                                                  */
/*       .w: 描画不透明度                                                            */
/* MEMO: i_Flagsの内容は下記になります。                                                */
/*       .x: 透明ピクセルの廃棄フラグ（0.0f:廃棄しない・1.0f:廃棄）                     */
/*       .y: プリマルチプライドアルファ計算フラグ（0.0f:計算しない・1.0f:計算する）     */
/*       .z: パーツカラーを頂点色として扱うかのフラグ（0.0f:パーツカラー・1.0f:頂点色） */
/*       .w: （未使用）                                                                 */
layout(location = 0)  in vec3 i_Position;
layout(location = 1)  in vec4 i_UV0;
layout(location = 2)  in vec4 i_ColorParts;
layout(location = 3)  in vec4 i_Flags;
layout(location = 4)  in vec4 i_ArgumentVs00;
layout(location = 5)  in vec4 i_ArgumentFs00;
layout(location = 6)  in vec4 i_ArgumentFs01;
layout(location = 7)  in vec4 i_ArgumentFs02;
layout(location = 8)  in vec4 i_ArgumentFs03;
layout(location = 9)  in vec4 i_ParameterFs00;
layout(location = 10) in vec4 i_ParameterFs01;
layout(location = 11) in vec4 i_ParameterFs02;
layout(location = 12) in vec4 i_ParameterFs03;


/* -------------------------------------------------------------------------- */
/*                                                                     Output */
/* -------------------------------------------------------------------------- */
/* MEMO: ピクセル（フラグメント）シェーダの入力と同一である必要があります。 */
out OutputShaderVertex
{
	vec4 ColorMain;
	vec4 ColorParts;
	vec4 Texture00UV;
	vec4 PositionDraw;
	vec4 Flags;
	vec4 ArgumentVs;
	vec4 ArgumentFs00;
	vec4 ArgumentFs01;
	vec4 ArgumentFs02;
	vec4 ArgumentFs03;
	vec4 ParameterFs00;
	vec4 ParameterFs01;
	vec4 ParameterFs02;
	vec4 ParameterFs03;
} OUT;


/* -------------------------------------------------------------------------- */
/*                                                          Constant (Unique) */
/* -------------------------------------------------------------------------- */
/* シェーダ定数 */
layout(std140) uniform View
{
    vec4 projMtx[4];
    vec4 cameraMtx[3];
    vec4 worldMtx[3];
};


/* -------------------------------------------------------------------------- */
/*                                                                     Macros */
/* -------------------------------------------------------------------------- */
/* 位置ベクトルを変形（関数型マクロ） */
#define SS6PlayerPositionTransform(_matrix_, _position_) vec3( \
    dot(_matrix_[0].xyzw, _position_.xyzw), \
    dot(_matrix_[1].xyzw, _position_.xyzw), \
    dot(_matrix_[2].xyzw, _position_.xyzw))

/* 位置ベクトルをプロジェクション投影（関数型マクロ） */
#define SS6PlayerProjectionTransform(_matrix_, _position_) vec4( \
    dot(_matrix_[0].xyzw, _position_.xyzw), \
    dot(_matrix_[1].xyzw, _position_.xyzw), \
    dot(_matrix_[2].xyzw, _position_.xyzw), \
    dot(_matrix_[3].xyzw, _position_.xyzw))

/* 変形計算後のZ座標に対する補正（関数型マクロ）                                     */
/* MEMO: 基本、同次正規化のために行います。                                          */
/*       通常はGPUが頂点シェーダの出力を自動的に正規化するので何も必要ありませんが、 */
/*       自動で正規化されると困る場合などに補正計算で使用します（例えば深度変化を直  */
/*       線変移にしたりする場合に、自動正規化とは異なる手順で正規化しなくてはならな  */
/*       いケースなどが生じます）。                                                  */
/* MEMO: 現在は空処理です。 */
#define SS6PlayerPositionCorrect(_pos) { }


/* -------------------------------------------------------------------------- */
/*                                                       Shader-Code (Vertex) */
/* -------------------------------------------------------------------------- */
void main()
{
	/* 頂点座標を算出 */
	/* MEMO: 変形計算がマクロ実装なので、念のためテンポラリを使ってスワップして使用しています */
	vec4 position = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 temp = vec4(i_Position, 1.0);
	position.xyz = SS6PlayerPositionTransform(worldMtx, temp);
	temp.xyz = SS6PlayerPositionTransform(cameraMtx, position);
	position = SS6PlayerProjectionTransform(projMtx, temp);
	SS6PlayerPositionCorrect(position);

	/* UVを算出 */
	/* MEMO: UVは現在CPU側で行列計算してしまっているので、ここでは上下反転だけ行う。 */
	vec4 uv0 = i_UV0;
	uv0.y = 1.0 - uv0.y;
//	uv0.z = i_UV0.z;
//	uv0.w = i_UV0.w;

	/* 頂点色 */
	/* MEMO: 不透明度が2つあるように見えますが、uv0.wはSSのデータを解釈した結果の不透明度・colorDiffuse.wは最終的な不透明度です。 */
	/*       ※ピクセルシェーダが使用している値は、colorDiffuse.wになります。                                                     */
	vec4 colorDiffuse = vec4(1.0, 1.0, 1.0, uv0.w);
	vec4 colorParts = i_ColorParts;
	if(0.5 < i_Flags.z)	{
		/* MEMO: i_Flags.zが1.0に設定されていた場合、パーツカラーデータは頂点色として機能します。 */
		/*       そのため、計算用のパーツカラーは完全透明色にして影響しないようにします。         */
		colorDiffuse = colorParts;
		colorDiffuse.a *= uv0.w;
		uv0.z = 0.0;
		colorParts = vec4(0.0, 0.0, 0.0, 0.0);
	}

	/* 出力 */
	gl_Position = position;
	OUT.ColorMain = colorDiffuse;
	OUT.ColorParts = colorParts;
	OUT.Texture00UV = uv0;
	OUT.PositionDraw = position;
	OUT.Flags = i_Flags;
	OUT.ArgumentVs = i_ArgumentVs00;
	OUT.ArgumentFs00 = i_ArgumentFs00;
	OUT.ArgumentFs01 = i_ArgumentFs01;
	OUT.ArgumentFs02 = i_ArgumentFs02;
	OUT.ArgumentFs03 = i_ArgumentFs03;
	OUT.ParameterFs00 = i_ParameterFs00;
	OUT.ParameterFs01 = i_ParameterFs01;
	OUT.ParameterFs02 = i_ParameterFs02;
	OUT.ParameterFs03 = i_ParameterFs03;
}

