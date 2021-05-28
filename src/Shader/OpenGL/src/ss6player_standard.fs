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
uniform sampler2D TexureAlbedo0;


/* -------------------------------------------------------------------------- */
/*                                                                     Macros */
/* -------------------------------------------------------------------------- */
/* カラーブレンド計算（関数型マクロ） */
/* MEMO: マクロの都合上、_pixel（ピクセル色入出力）・_partscolor（パーツカラー色） */
/*       には計算式などを与えないでください。                                      */
/*       ※必ず単体のvec4を与えてください。                                        */
#define SS6PlayerPartsColor(_pixel,_partscolor,_blend)							\
	{																			\
		vec4 color[4];															\
		float rate = _partscolor.a;												\
		float rateInverse = 1.0f - rate;										\
		color[0] = (_pixel * rateInverse) + (_partscolor * rate);				\
		color[1] = _pixel + (_partscolor * rate);								\
		color[2] = _pixel - (_partscolor * rate);								\
		color[3] = (_pixel * rateInverse) + ((_pixel * _partscolor) * rate);	\
		_pixel = color[int(_blend)];											\
	}

/* 描画対象外のピクセルを捨てる（関数型マクロ） */
/* MEMO: マクロの中でdiscardしているので注意してください。 */
/* MEMO: ピクセルシェーダ入力のFlagsをダイレクトに使用しているので注意してください。        */
/*       ※本フラグはここでしか使わないはずなので、外部から設定させない方が良いと思います。 */
#define SS6PlayerPixelCheckDiscard(_alpha,_threshold)	\
	if(0.5f < IN.Flags.x)	{				\
		if((_threshold) >= (_alpha))	{	\
			discard;						\
		}									\
	}

/* 色域クランプ（関数型マクロ） */
/* MEMO: HDRの時の飽和時特性の調整のために入っていますが……フレームバッファ */
/*       への（GPUのラスタライザ段での）合成時に飽和クリップが行われないため */
/*       に、結局飽和状態を制御できないので、あまり意味がないのが実情です。  */
#define SS6PlayerColorClamp(_pixel)	(clamp((_pixel), 0.0, 1.0))

/* プリマルチプライドアルファの追加計算（関数型マクロ） */
/* MEMO: ピクセルシェーダ入力のFlagsをダイレクトに使用しているので注意してください。        */
/*       ※本フラグはここでしか使わないはずなので、外部から設定させない方が良いと思います。 */
/* MEMO: _pixelは入出力なので注意してください（.xyzが書き換わります）。 */
/*       ※そのため、式などを与えないでください。                       */
#define SS6PlayerPixelColorRGBCorrectPremultipliedAlpha(_pixel,_alpha)	\
	if(0.5f < IN.Flags.y)	{											\
		_pixel.xyz *= (_alpha);											\
	}

/* ピクセルの最終色を決定するための補正（関数型マクロ） */
/* MEMO: 自乗は、sRGB→ストレートRGBの変換である2.2乗の近似計算です。 */
#ifdef COMPILEOPTION_CONVERT_SRGB_TO_RGB
#define SS6PlayerColorRGBCorrectRGB(_pixel) ((_pixel) * (_pixel))
#define SS6PlayerColorRGBCorrectA(_pixel) (_pixel)
#else
#define SS6PlayerColorRGBCorrectRGB(_pixel) (_pixel)
#define SS6PlayerColorRGBCorrectA(_pixel) (_pixel)
#endif


/* -------------------------------------------------------------------------- */
/*                                               Shader-Code (Pixel/Fragment) */
/* -------------------------------------------------------------------------- */
void main()
{
	/* ピクセルをデコード */
	vec4 pixel;
	pixel = texture(TexureAlbedo0, IN.Texture00UV.xy);
	pixel *= IN.ColorMain;
	SS6PlayerPixelCheckDiscard(pixel.a, 0.0f);	/* ※描画しないピクセルはこの中でdiscardされます */

	/* ピクセル色計算 */
	float pixelA = pixel.a;	/* 計算でアルファが壊れるので値を保全 */
	SS6PlayerPartsColor(pixel, IN.ColorParts, IN.Texture00UV.z);	/* パーツカラーを計算 */
	pixel = SS6PlayerColorClamp(pixel);	/* 色域をクリップ */
	SS6PlayerPixelColorRGBCorrectPremultipliedAlpha(pixel,pixelA);	/* プリマルチプライドアルファ対処 */
	pixel.a = pixelA;	/* 壊したアルファを復帰 */

	/* 出力 */
	o_Color.rgb = SS6PlayerColorRGBCorrectRGB(pixel.rgb);
	o_Color.a = SS6PlayerColorRGBCorrectA(pixel.a);
}
