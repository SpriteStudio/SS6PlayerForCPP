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
/*                                                                      Input */
/* -------------------------------------------------------------------------- */
/* MEMO: 頂点シェーダの出力と同一である必要があります。 */
in OutputShaderVertex
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
} IN;


/* -------------------------------------------------------------------------- */
/*                                                                     Output */
/* -------------------------------------------------------------------------- */
layout(location = 0) out vec4 o_Color;


/* -------------------------------------------------------------------------- */
/*                                                   Sampler-Texture (Unique) */
/* -------------------------------------------------------------------------- */
/* MEMO: サンプラ名は変えないでください（C++側から名称で検索しています）。 */
uniform sampler2D TexureAlbedo0;


/* -------------------------------------------------------------------------- */
/*                                                                     Macros */
/* -------------------------------------------------------------------------- */
/* 描画対象外のピクセルを捨てる（関数型マクロ） */
/* MEMO: マクロの中でdiscardしているので注意してください。 */
/* MEMO: ピクセルシェーダ入力のFlagsをダイレクトに使用しているので注意してください。        */
/*       ※本フラグはここでしか使わないはずなので、外部から設定させない方が良いと思います。 */
 #define SS6PlayerPixelCheckDiscard(_alpha,_threshold)	\
	if(0.5f < IN.Flags.x)	{							\
		if((_threshold) >= (_alpha))	{				\
			discard;									\
		}												\
	}

/* -------------------------------------------------------------------------- */
/*                                               Shader-Code (Pixel/Fragment) */
/* -------------------------------------------------------------------------- */
void main()
{
	/* ピクセルをデコード */
	vec4 pixel;
	pixel = texture(TexureAlbedo0, IN.Texture00UV.xy);
	SS6PlayerPixelCheckDiscard(pixel.a, IN.ColorMain.a);	/* ※描画しないピクセルはこの中でdiscardされます（ステンシルも操作されない） */

	/* MEMO: ステンシルにのみ影響を与えたいので、通常合成で結果に影響のない色を出力しています。 */
	o_Color = vec4(0.0, 0.0, 0.0, 0.0);
}
