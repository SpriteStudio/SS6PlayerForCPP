/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* プラットフォームに依存した変数型定義群です（ただ、少し変数型以外のものも混 */
/* じっています）。                                                           */
/* 本機能（.h/.cpp）の実装を変更することで、プラットフォーム用に対応した基本  */
/* 型を定義・実装します。                                                     */
/*                                                                            */
/* 基本的にタプル系（ベクトル・行列など）は、CPU用とFPU用に分けて定義してあり */
/* ます。                                                                     */
/* FPU用については、要素に対する入出力をクラスとしてフォローしておいて、メン  */
/* バ変数の形態でアクセスできないようになっています（このあたり、FPU用につい  */
/* ては各機種での制限が多かったりするので、それらでのトラブル抑止です）。     */
/* タプルの計算については、原則数式で行わずに（ラッピング用の）関数とマクロを */
/* 経由した関数での計算実装になっています（これについては、後に演算子をオーバ */
/* ライドするかもしれませんが……環境によって行列のベクトル格納方向などの違い */
/* があり、かえって混乱しないかが若干不安なため迷っています）。               */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
/* for OpenGL(GLFW+GLEW) */
#define GLEW_STATIC

#include <cstring>
#include <cstddef>
#include <cstdint>

#if _WIN32 || _WIN64
/* MEMO: Windowsの時 */
#include <Windows.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/gl.h>

#define SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS
#elif __APPLE__
/* MEMO: MacOSの時 */
#include <OpenGL/gl3.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif	/* _WIN32 || _WIN64 */

/* コンパイルオプション: シェーダは分割型（頂点・ピクセルで個別のソース） */
/* MEMO: プラットフォームによってシェーダの扱いが異なるため、Shader::BootUpのプロトタイプ宣言などが変わります。 */
#define SS6PLAYER_COMPILEOPTION_SHADER_SEPALATE

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */
enum AlignmentMemory	{
	/* 配列などで安全なアライメント ※念のため */
	SAFETY = 8,																/* 配列などのサイズを自力で求めたりする時のアライメント用 */

	/* FPUに計算させるベクトル・行列・四元数などのアライメント */
	TUPPLE_VARIABLE = 16,													/* ※SIMDなどを使用する時にアライメント制限が付いている場合は仕様に合わせて設定する必要があります。 */

	/* GPU関係のクラスのアライメント */
	CLASSES_GPU = 16,

	/* ファイルをメモリに読む際のアライメント */
	FILE_BUFFER = 16,														/* ※テクスチャやシェーダのファイル読込などにアライメント制限が付いている環境では仕様に合わせて設定する必要があります。 */
};

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 整数型 */
typedef int8_t Sint8;														/*!< 8ビット整数（符号あり） */
typedef uint8_t Uint8;														/*!< 8ビット整数（符号なし） */
typedef int16_t Sint16;														/*!< 16ビット整数（符号あり） */
typedef uint16_t Uint16;													/*!< 16ビット整数（符号なし） */
typedef int32_t Sint32;														/*!< 32ビット整数（符号あり） */
typedef uint32_t Uint32;													/*!< 32ビット整数（符号なし） */
typedef int64_t Sint64;														/*!< 64ビット整数（符号あり） */
typedef uint64_t Uint64;													/*!< 64ビット整数（符号なし） */

/* 浮動小数型 */
// typedef float Float16;
typedef float Float32;														/*!< 32ビット浮動小数（単精度） */
typedef double Float64;														/*!< 64ビット浮動小数（倍精度） */

/* クラス・構造体関係前方宣言 */
namespace CPU	{
class Vector2;
class Vector3;
class Vector4;
class Color;
class Color32;
class Rect;
class Matrix4x3;
class Matrix4x4;
}	/* CPU */
namespace FPU	{
class Vector2;
class Vector3;
class Vector4;
class Color;
class Matrix4x3;
class Matrix4x4;
}	/* FPU */

/* CPU型タプル */
/* MEMO: CPUでの計算で使用するためのタプルクラスです。            */
/*       下記の場合に使用します。                                 */
/*       - FPUを使用しないで、CPUのみで計算する場合。             */
/*       - 静的データから値を引っ張る場合。                       */
/*       - 内包する各成分を個別で操作することが多い場合。         */
/*       - std140形式などでシェーダにブロックを転送する場合。     */
/*                                                                */
/*       特徴として、下記があります。                             */
/*       - 各成分を単独で操作可能。                               */
/*       - メモリアライメントに（CPU規定以外の）制限がない。      */
/*       - シェーダブロック以外に外部ユニットに対して使用すること */
/*         ができない。                                           */
/* MEMO: 本当はCPUは静的クラスにしておきたい（usingで省略できないようにしたい）のですが、 */
/*       インナクラスは前方宣言できないので仕方なく……。                                 */
namespace CPU	{
/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
/* 2次元ベクトル */
class Vector2
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	enum KindElement	{
		X = 0,
		Y,

		TERMINATOR,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	GLfloat Element[TERMINATOR];

	/* 定数群 */
	static const Vector2 Zero;						/* 全要素0.0f */
	static const Vector2 One;						/* 全要素1.0f */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Vector2(void)
	{
		Element[KindElement::X] = 
		Element[KindElement::Y] = (GLfloat)0.0f;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値

	@retval	なし
	*/
	Vector2(float valueX, float valueY)
	{
		Element[KindElement::X] = (GLfloat)valueX;
		Element[KindElement::Y] = (GLfloat)valueY;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector2

	@retval	なし
	*/
	Vector2(const Vector2& value)
	{
		Element[KindElement::X] = value.Element[KindElement::X];
		Element[KindElement::Y] = value.Element[KindElement::Y];
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期化に使うFPU::Vector2

	@retval	なし
	*/
	Vector2(const FPU::Vector2& value);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Vector2(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のVector2

	@retval	なし
	*/
	inline void Set(const Vector2& value)
	{
		Element[KindElement::X] = value.Element[KindElement::X];
		Element[KindElement::Y] = value.Element[KindElement::Y];
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のFPU::Vector2

	@retval	なし
	*/
	void Set(const FPU::Vector2& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値

	@retval	なし
	*/
	void Set(float valueX, float valueY)
	{
		Element[KindElement::X] = (GLfloat)valueX;
		Element[KindElement::Y] = (GLfloat)valueY;
	}

	/* ********************************************************* */
	//! X成分値の設定
	/*!
	@param	value
		X成分値

	@retval	なし
	*/
	inline void SetX(float value)
	{
		Element[KindElement::X] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! X成分値の取得
	/*!
	@param	なし

	@retval	X成分値
	*/
	inline float GetX(void) const
	{
		return((float)Element[KindElement::X]);
	}

	/* ********************************************************* */
	//! Y成分値の設定
	/*!
	@param	value
		Y成分値

	@retval	なし
	*/
	inline void SetY(float value)
	{
		Element[KindElement::Y] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! Y成分値の取得
	/*!
	@param	なし

	@retval	Y成分値
	*/
	inline float GetY(void) const
	{
		return((float)Element[KindElement::Y]);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 3次元ベクトル */
class Vector3
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	enum KindElement	{
		X = 0,
		Y,
		Z,

		TERMINATOR,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	GLfloat Element[TERMINATOR];

	/* 定数群 */
	static const Vector3 Zero;						/* 全要素0.0f */
	static const Vector3 One;						/* 全要素1.0f */
	static const Vector3 AxisX;						/* X要素のみ1.0f（Y,Zは0.0f） */
	static const Vector3 AxisY;						/* Y要素のみ1.0f（X,Zは0.0f） */
	static const Vector3 AxisZ;						/* Z要素のみ1.0f（X,Yは0.0f） */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Vector3(void)
	{
		Element[KindElement::X] = 
		Element[KindElement::Y] = 
		Element[KindElement::Z] = (GLfloat)0.0f;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値

	@retval	なし
	*/
	Vector3(float valueX, float valueY, float valueZ)
	{
		Element[KindElement::X] = (GLfloat)valueX;
		Element[KindElement::Y] = (GLfloat)valueY;
		Element[KindElement::Z] = (GLfloat)valueZ;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector3

	@retval	なし
	*/
	Vector3(const Vector3& value)
	{
		Element[KindElement::X] = value.Element[KindElement::X];
		Element[KindElement::Y] = value.Element[KindElement::Y];
		Element[KindElement::Z] = value.Element[KindElement::Z];
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うFPU::Vector3

	@retval	なし
	*/
	Vector3(const FPU::Vector3& value);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Vector3(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のVector3

	@retval	なし
	*/
	inline void Set(const Vector3& value)
	{
		Element[KindElement::X] = value.Element[KindElement::X];
		Element[KindElement::Y] = value.Element[KindElement::Y];
		Element[KindElement::Z] = value.Element[KindElement::Z];
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のFPU::Vector3

	@retval	なし
	*/
	void Set(const FPU::Vector3& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値

	@retval	なし
	*/
	void Set(float valueX, float valueY, float valueZ)
	{
		Element[KindElement::X] = (GLfloat)valueX;
		Element[KindElement::Y] = (GLfloat)valueY;
		Element[KindElement::Z] = (GLfloat)valueZ;
	}

	/* ********************************************************* */
	//! X成分値の設定
	/*!
	@param	value
		X成分値

	@retval	なし
	*/
	inline void SetX(float value)
	{
		Element[KindElement::X] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! X成分値の取得
	/*!
	@param	なし

	@retval	X成分値
	*/
	inline float GetX(void) const
	{
		return((float)Element[KindElement::X]);
	}

	/* ********************************************************* */
	//! Y成分値の設定
	/*!
	@param	value
		Y成分値

	@retval	なし
	*/
	inline void SetY(float value)
	{
		Element[KindElement::Y] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! Y成分値の取得
	/*!
	@param	なし

	@retval	Y成分値
	*/
	inline float GetY(void) const
	{
		return((float)Element[KindElement::Y]);
	}

	/* ********************************************************* */
	//! Z成分値の設定
	/*!
	@param	value
		Z成分値

	@retval	なし
	*/
	inline void SetZ(float value)
	{
		Element[KindElement::Z] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! Z成分値の取得
	/*!
	@param	なし

	@retval	Z成分値
	*/
	inline float GetZ(void) const
	{
		return((float)Element[KindElement::Z]);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 4次元ベクトル */
class Vector4
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	enum KindElement	{
		X = 0,
		Y,
		Z,
		W,

		TERMINATOR,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	GLfloat Element[TERMINATOR];

	/* 定数群 */
	static const Vector4 Zero;						/* 全要素0.0f */
	static const Vector4 One;						/* 全要素1.0f */
	static const Vector4 Origin;					/* 同次原点（X,Y,Z = 0.0f, W=1.0f） */
	static const Vector4 AxisX;						/* X要素のみ1.0f（Y,Z,Wは0.0f） */
	static const Vector4 AxisY;						/* Y要素のみ1.0f（X,Z,Wは0.0f） */
	static const Vector4 AxisZ;						/* Z要素のみ1.0f（X,Y,Wは0.0f） */
	static const Vector4 AxisW;						/* W要素のみ1.0f（X,Y,Zは0.0f） *//* 同次座標用 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Vector4(void)
	{
		Element[KindElement::X] = 
		Element[KindElement::Y] = 
		Element[KindElement::Z] = 
		Element[KindElement::W] = (GLfloat)0.0f;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値
	@param	valueW
		Z成分値

	@retval	なし
	*/
	Vector4(float valueX, float valueY, float valueZ, float valueW)
	{
		Element[KindElement::X] = (GLfloat)valueX;
		Element[KindElement::Y] = (GLfloat)valueY;
		Element[KindElement::Z] = (GLfloat)valueZ;
		Element[KindElement::W] = (GLfloat)valueW;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector4

	@retval	なし
	*/
	Vector4(const Vector4& value)
	{
		Element[KindElement::X] = value.Element[KindElement::X];
		Element[KindElement::Y] = value.Element[KindElement::Y];
		Element[KindElement::Z] = value.Element[KindElement::Z];
		Element[KindElement::W] = value.Element[KindElement::W];
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うFPU::Vector4

	@retval	なし
	*/
	Vector4(const FPU::Vector4& value);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Vector4(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のVector4

	@retval	なし
	*/
	inline void Set(const Vector4& value)
	{
		Element[KindElement::X] = value.Element[KindElement::X];
		Element[KindElement::Y] = value.Element[KindElement::Y];
		Element[KindElement::Z] = value.Element[KindElement::Z];
		Element[KindElement::W] = value.Element[KindElement::W];
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のFPU::Vector4

	@retval	なし
	*/
	void Set(const FPU::Vector4& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のFPU::Vector4

	@retval	なし
	*/
	void Set(float valueX, float valueY, float valueZ, float valueW)
	{
		Element[KindElement::X] = (GLfloat)valueX;
		Element[KindElement::Y] = (GLfloat)valueY;
		Element[KindElement::Z] = (GLfloat)valueZ;
		Element[KindElement::W] = (GLfloat)valueW;
	}

	/* ********************************************************* */
	//! X成分値の設定
	/*!
	@param	value
		X成分値

	@retval	なし
	*/
	inline void SetX(float value)
	{
		Element[KindElement::X] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! X成分値の取得
	/*!
	@param	なし

	@retval	X成分値
	*/
	inline float GetX(void) const
	{
		return((float)Element[KindElement::X]);
	}

	/* ********************************************************* */
	//! Y成分値の設定
	/*!
	@param	value
		Y成分値

	@retval	なし
	*/
	inline void SetY(float value)
	{
		Element[KindElement::Y] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! Y成分値の取得
	/*!
	@param	なし

	@retval	Y成分値
	*/
	inline float GetY(void) const
	{
		return((float)Element[KindElement::Y]);
	}

	/* ********************************************************* */
	//! Z成分値の設定
	/*!
	@param	value
		Z成分値

	@retval	なし
	*/
	inline void SetZ(float value)
	{
		Element[KindElement::Z] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! Z成分値の取得
	/*!
	@param	なし

	@retval	Z成分値
	*/
	inline float GetZ(void) const
	{
		return((float)Element[KindElement::Z]);
	}

	/* ********************************************************* */
	//! W成分値の設定
	/*!
	@param	value
		W成分値

	@retval	なし
	*/
	inline void SetW(float value)
	{
		Element[KindElement::W] = (GLfloat)value;
	}

	/* ********************************************************* */
	//! W成分値の取得
	/*!
	@param	なし

	@retval	W成分値
	*/
	inline float GetW(void) const
	{
		return((float)Element[KindElement::W]);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 色 */
/* MEMO: 色は最終的にVector4にしてシェーダに送ったりすることが多いため、 */
/*       最初からVector4の継承体になっています。                         */
/*       その上でVector4を隠蔽していますので、メンバに直接触れることがで */
/*       きませんので、かならずGet?・Set?経由で操作してください。        */
class Color :
	private Vector4
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* 定数群 */
	static const Color Clear;						/* 全要素0.0f */
	static const Color White;						/* 純白（全要素1.0f） */
	static const Color Black;						/* 純黒（R,G,B = 0.0f, A=1.0f） */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Color(void) :
		Vector4()
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		R成分値
	@param	valueY
		G成分値
	@param	valueZ
		B成分値
	@param	valueW
		A成分値

	@retval	なし
	*/
	Color(float valueR, float valueG, float valueB, float valueA)
	{
		SetX(valueR);
		SetY(valueG);
		SetZ(valueB);
		SetW(valueA);
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のCOlor
	@retval	なし
	*/
	Color(const Color& value)
	{
		SetX(value.GetR());
		SetY(value.GetG());
		SetZ(value.GetB());
		SetW(value.GetA());
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うFPU::Color

	@retval	なし
	*/
	Color(const FPU::Color& value);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のColor32

	@retval	なし

	RGBAの変換計算が疑似的なため、少し誤差が出ます。
	*/
	Color(const Color32& value);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Color(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のColor

	@retval	なし
	*/
	inline void Set(const Color& value)
	{
		SetX(value.GetR());
		SetY(value.GetG());
		SetZ(value.GetB());
		SetW(value.GetA());
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueX
		R成分値
	@param	valueY
		G成分値
	@param	valueZ
		B成分値
	@param	valueW
		A成分値

	@retval	なし
	*/
	inline void Set(float valueR, float valueG, float valueB, float valueA)
	{
		SetX(valueR);
		SetY(valueG);
		SetZ(valueB);
		SetW(valueA);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のFPU::Color

	@retval	なし
	*/
	void Set(const FPU::Color& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のColor32

	@retval	なし
	*/
	void Set(const Color32& value);

	/* ********************************************************* */
	//! R成分値の設定
	/*!
	@param	value
		R成分値

	@retval	なし
	*/
	inline void SetR(float value)
	{
		SetX(value);
	}

	/* ********************************************************* */
	//! R成分値の取得
	/*!
	@param	なし

	@retval	R成分値
	*/
	inline float GetR(void) const
	{
		return(GetX());
	}

	/* ********************************************************* */
	//! G成分値の設定
	/*!
	@param	value
		G成分値

	@retval	なし
	*/
	inline void SetG(float value)
	{
		SetY(value);
	}

	/* ********************************************************* */
	//! G成分値の取得
	/*!
	@param	なし

	@retval	G成分値
	*/
	inline float GetG(void) const
	{
		return(GetY());
	}

	/* ********************************************************* */
	//! B成分値の設定
	/*!
	@param	value
		B成分値

	@retval	なし
	*/
	inline void SetB(float value)
	{
		SetZ(value);
	}

	/* ********************************************************* */
	//! B成分値の取得
	/*!
	@param	なし

	@retval	B成分値
	*/
	inline float GetB(void) const
	{
		return(GetZ());
	}

	/* ********************************************************* */
	//! A成分値の設定
	/*!
	@param	value
		A成分値

	@retval	なし
	*/
	inline void SetA(float value)
	{
		SetW(value);
	}

	/* ********************************************************* */
	//! A成分値の取得
	/*!
	@param	なし

	@retval	A成分値
	*/
	inline float GetA(void) const
	{
		return(GetW());
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 色（32bit） */
/* MEMO: Color32は一部データで使っているので定義してありますが、基本的にColorを実用するようにしてください。 */
/*       ※Colorに対して32bit形式で取り出す時などの変換クラスとして使用するだけの目的で設置しています。     */
/*       ※ですので、基本的に各種機能を積んでいません。                                                     */
class Color32
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Color32(void) :
		r(0),
		g(0),
		b(0),
		a(0)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueR
		R成分値
	@param	valueG
		G成分値
	@param	valueB
		B成分値
	@param	valueA
		A成分値

	@retval	なし
	*/
	Color32(Uint8 valueR, Uint8 valueG, Uint8 valueB, Uint8 valueA) :
		r(valueR),
		g(valueG),
		b(valueB),
		a(valueA)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueR
		R成分値
	@param	valueG
		G成分値
	@param	valueB
		B成分値
	@param	valueA
		A成分値

	@retval	なし

	RGBAの変換計算が疑似的なため、少し誤差が出ます。
	*/
	Color32(Color& color) : 
		r((Uint8)(color.GetR() * 255.0f)),
		g((Uint8)(color.GetG() * 255.0f)),
		b((Uint8)(color.GetB() * 255.0f)),
		a((Uint8)(color.GetA() * 255.0f))
	{
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 矩形範囲 */
class Rect
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	Float32 xMin;
	Float32 yMin;
	Float32 xMax;
	Float32 yMax;

	/* 定数群 */
	static const Rect Zero;							/* 座標(0.0f, 0.0f),大きさ(0.0f, 0,0f) */

	/* ----------------------------------------------- Variables */
private:
protected:
public:

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Rect(void) :
		xMin(0.0f),
		yMin(0.0f),
		xMax(0.0f),
		yMax(0.0f)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値

	@retval	なし
	*/
	Rect(float valueX, float valueY, float valueWidth, float valueHeight) :
		xMin(valueX),
		yMin(valueY),
		xMax(valueX + valueWidth),
		yMax(valueY + valueHeight)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector2

	@retval	なし
	*/
	Rect(const Rect& value)
	{
		*this = value;
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Rect(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元のRect

	@retval	なし
	*/
	inline void Set(const Rect& value)
	{
		*this = value;
	}

	/* ********************************************************* */
	//! 値の設定（左上座標＆縦横幅）
	/*!
	@param	valueX
		左上X値
	@param	valueY
		左上Y値
	@param	valueWidth
		X幅
	@param	valueHeight
		Y幅

	@retval	なし
	*/
	inline void SetXYWH(float valueX, float valueY, float valueWidth, float valueHeight)
	{
		xMin = valueX;
		yMin = valueY;
		xMax = valueX + valueWidth;
		yMax = valueY + valueHeight;
	}

	/* ********************************************************* */
	//! 値の設定（対角座標）
	/*!
	@param	valueXMin
		左上X値
	@param	valueYMin
		左上Y値
	@param	valueXMax
		右下X値
	@param	valueYMax
		右下Y値

	@retval	なし
	*/
	inline void SetLURD(float valueXMin, float valueYMin, float valueXMax, float valueYMax)
	{
		xMin = valueXMin;
		yMin = valueYMin;
		xMax = valueXMax;
		yMax = valueYMax;
	}

	/* ********************************************************* */
	//! 位置の取得（左上）
	/*!
	@param	position
		結果出力先

	@retval	関数値
		なし
	@param	position
		左上の位置
	*/
	inline void PositionGetLU(CPU::Vector2* position)
	{
		position->Set(xMin, yMin);
	}

	/* ********************************************************* */
	//! サイズの取得
	/*!
	@param	size
		結果出力先

	@retval	関数値
		なし
	@param	size
		サイズ
	*/
	inline void SizeGet(CPU::Vector2* size)
	{
		size->Set(xMax - xMin, yMax - yMin);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 4x3行列 */
class Matrix4x3
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	enum KindElement
	{
		M00 = 0,
		M10,
		M20,
		M30,

		M01,
		M11,
		M21,
		M31,

		M02,
		M12,
		M22,
		M32,

		TERMINATOR,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	/* MEMO: 格納はRaw-Majorです。 */
	GLfloat Element[KindElement::TERMINATOR];

	/* 定数群 */
	static const Matrix4x3 Zero;					/* 全要素0.0f */
	static const Matrix4x3 Unit;					/* 単位行列 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Matrix4x3(void);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	_m00～\m33
		各成分値

	@retval	なし

	表記順は行優先（行で4次元ベクトルが表記される）なので注意してください。
	*/
	Matrix4x3(	float _m00, float _m10, float _m20, float _m30,
				float _m01, float _m11, float _m21, float _m31,
				float _m02, float _m12, float _m22, float _m32
			);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	Matrix4x3(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, const Vector3& axisW);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のMatrix4x4

	@retval	なし
	*/
	Matrix4x3(const Matrix4x3& value);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うFPU::Matrix4x4

	@retval	なし
	*/
	Matrix4x3(const FPU::Matrix4x3& value);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Matrix4x3(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のMatrix4x3

	@retval	なし
	*/
	void Set(const Matrix4x3& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のFPU::Matrix4x3

	@retval	なし
	*/
	void Set(const FPU::Matrix4x3& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値（平行移動成分）

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	void Set(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, const Vector3& axisW);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 4x4行列 */
class Matrix4x4
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	enum KindElement
	{
		M00 = 0,
		M10,
		M20,
		M30,

		M01,
		M11,
		M21,
		M31,

		M02,
		M12,
		M22,
		M32,

		M03,
		M13,
		M23,
		M33,

		TERMINATOR,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	/* MEMO: 格納はRaw-Majorです。 */
	GLfloat Element[KindElement::TERMINATOR];

	/* 定数群 */
	static const Matrix4x4 Zero;					/* 全要素0.0f */
	static const Matrix4x4 Unit;					/* 単位行列 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Matrix4x4(void);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	_m00～\m33
		各成分値

	@retval	なし

	表記順は行優先（行で4次元ベクトルが表記される）なので注意してください。
	*/
	Matrix4x4(	float _m00, float _m10, float _m20, float _m30,
				float _m01, float _m11, float _m21, float _m31,
				float _m02, float _m12, float _m22, float _m32,
				float _m03, float _m13, float _m23, float _m33
			);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値（平行移動成分）

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	Matrix4x4(const Vector4& axisX, const Vector4& axisY, const Vector4& axisZ, const Vector4& axisW);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のMatrix4x4

	@retval	なし
	*/
	Matrix4x4(const Matrix4x4& value);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うFPU::Matrix4x4

	@retval	なし
	*/
	Matrix4x4(const FPU::Matrix4x4& value);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Matrix4x4(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のMatrix4x4

	@retval	なし
	*/
	void Set(const Matrix4x4& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のFPU::Matrix4x4

	@retval	なし
	*/
	void Set(const FPU::Matrix4x4& value);

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値（平行移動成分）

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	void Set(const Vector4& axisX, const Vector4& axisY, const Vector4& axisZ, const Vector4& axisW);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* ----------------------------------------------- Enums & Constants */

/* ----------------------------------------------- Variables */

/* ----------------------------------------------- Functions */

/* ----------------------------------------------- Operators */
}	/* CPU */

/* FPU型タプル */
/* MEMO: FPUでの計算で使用するためのタプルクラスです。              */
/*       下記の場合に使用します。                                   */
/*       - CPUで直接計算しないで、FPUを使用して高速計算する場合。   */
/*                                                                  */
/*       特徴として、下記があります。                               */
/*       - 各成分を単独で操作することはできません（実装は隠蔽されて */
/*         います）。                                               */
/*       - メモリアライメントに（CPUの規定以外に加えて）FPUの制限が */
/*         つく（違反するとハード的に誤動作する場合もあります）。   */
/*       - 専用ユニット（FPU）で処理する分だけ高速です（ただしあま  */
/*         り細切れに計算するとCPU-FPU間連絡でのオーバヘッドがかか  */
/*         る場合があります）。                                     */
/*                                                                  */
/*       この型を構造体のメンバなどで持つ場合、念のためメンバ宣言側 */
/*       にも「alignas(AlignmentMemory::TUPPLE_VARIABLE)」を付与し  */
/*       てメモリアライメント設定を行ってください。                 */
/*       Switchに限ってはnn::util::Vector?fをpublic継承すれば良いの */
/*       ですが……プラットフォーム別での最低限の実装がわかるよう、 */
/*       包含体にしています。                                       */
/* MEMO: 本当はFPUは静的クラスにしておきたい（usingで省略できないようにしたい）のですが、 */
/*       インナクラスは前方宣言できないので仕方なく……。                                 */
namespace FPU	{
/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
/* 2次元ベクトル */
class alignas(AlignmentMemory::TUPPLE_VARIABLE) Vector2
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	/* MEMO: SIMDを使わない場合、原則CPU側の値を保持しておきます。 */
	CPU::Vector2 Value;

	/* 定数群 */
	static const Vector2 Zero;						/* 全要素0.0f */
	static const Vector2 One;						/* 全要素1.0f */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Vector2(void) :
		Value()
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値

	@retval	なし
	*/
	Vector2(float valueX, float valueY) :
		Value(valueX, valueY)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector2

	@retval	なし
	*/
	Vector2(const Vector2& value) :
		Value(value.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うCPU::Vector2

	@retval	なし
	*/
	Vector2(const CPU::Vector2& value) :
		Value(value)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Vector2(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値

	@retval	なし
	*/
	inline void Set(float valueX, float valueY)
	{
		Value.Set(valueX, valueY);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のVector2

	@retval	なし
	*/
	inline void Set(const Vector2& value)
	{
		Value.Set(value.Value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のCPU::Vector2

	@retval	なし
	*/
	inline void Set(const CPU::Vector2& value)
	{
		Value.Set(value);
	}

	/* ********************************************************* */
	//! 値の取得
	/*!
	@param	valueX
		X成分値格納先
	@param	valueY
		Y成分値格納先

	@retval	なし
	@retval	valueX
		X成分値
	@retval	valueY
		Y成分値
	*/
	inline void Get(float* valueX, float* valueY) const
	{
		*valueX = Value.GetX();
		*valueY = Value.GetY();
	}

	/* ********************************************************* */
	//! X成分値の設定
	/*!
	@param	value
		X成分値

	@retval	なし
	*/
	inline void SetX(float value)
	{
		Value.SetX(value);
	}

	/* ********************************************************* */
	//! X成分値の取得
	/*!
	@param	なし

	@retval	X成分値
	*/
	inline float GetX(void) const
	{
		return(Value.GetX());
	}

	/* ********************************************************* */
	//! Y成分値の設定
	/*!
	@param	value
		Y成分値

	@retval	なし
	*/
	inline void SetY(float value)
	{
		Value.SetY(value);
	}

	/* ********************************************************* */
	//! Y成分値の取得
	/*!
	@param	なし

	@retval	Y成分値
	*/
	inline float GetY(void) const
	{
		return(Value.GetY());
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 3次元ベクトル */
class alignas(AlignmentMemory::TUPPLE_VARIABLE) Vector3
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	/* MEMO: SIMDを使わない場合、原則CPU側の値を保持しておきます。 */
	CPU::Vector3 Value;

	/* 定数群 */
	static const Vector3 Zero;						/* 全要素0.0f */
	static const Vector3 One;						/* 全要素1.0f */
	static const Vector3 AxisX;						/* X要素のみ1.0f（Y,Zは0.0f） */
	static const Vector3 AxisY;						/* Y要素のみ1.0f（X,Zは0.0f） */
	static const Vector3 AxisZ;						/* Z要素のみ1.0f（X,Yは0.0f） */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Vector3(void) :
		Value()
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値

	@retval	なし
	*/
	Vector3(float valueX, float valueY, float valueZ) :
		Value(valueX, valueY, valueZ)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector3

	@retval	なし
	*/
	Vector3(const Vector3& value) :
		Value(value.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うCPU::Vector3

	@retval	なし
	*/
	Vector3(const CPU::Vector3& value) :
		Value(value)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Vector3(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値

	@retval	なし
	*/
	inline void Set(float valueX, float valueY, float valueZ)
	{
		Value.Set(valueX, valueY, valueZ);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のVector3

	@retval	なし
	*/
	inline void Set(const Vector3& value)
	{
		Value.Set(value.Value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のCPU::Vector3

	@retval	なし
	*/
	inline void Set(const CPU::Vector3& value)
	{
		Value.Set(value);
	}

	/* ********************************************************* */
	//! 値の取得
	/*!
	@param	valueX
		X成分値格納先
	@param	valueY
		Y成分値格納先
	@param	valueZ
		Z成分値格納先

	@retval	なし
	@retval	valueX
		X成分値
	@retval	valueY
		Y成分値
	@retval	valueZ
		Z成分値
	*/
	inline void Get(float* valueX, float* valueY, float* valueZ) const
	{
		*valueX = Value.GetX();
		*valueY = Value.GetY();
		*valueZ = Value.GetZ();
	}

	/* ********************************************************* */
	//! X成分値の設定
	/*!
	@param	value
		X成分値

	@retval	なし
	*/
	inline void SetX(float value)
	{
		Value.SetX(value);
	}

	/* ********************************************************* */
	//! X成分値の取得
	/*!
	@param	なし

	@retval	X成分値
	*/
	inline float GetX(void) const
	{
		return(Value.GetX());
	}

	/* ********************************************************* */
	//! Y成分値の設定
	/*!
	@param	value
		Y成分値

	@retval	なし
	*/
	inline void SetY(float value)
	{
		Value.SetY(value);
	}

	/* ********************************************************* */
	//! Y成分値の取得
	/*!
	@param	なし

	@retval	Y成分値
	*/
	inline float GetY(void) const
	{
		return(Value.GetY());
	}

	/* ********************************************************* */
	//! Z成分値の設定
	/*!
	@param	value
		Z成分値

	@retval	なし
	*/
	inline void SetZ(float value)
	{
		Value.SetZ(value);
	}

	/* ********************************************************* */
	//! Z成分値の取得
	/*!
	@param	なし

	@retval	Z成分値
	*/
	inline float GetZ(void) const
	{
		return(Value.GetZ());
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 4次元ベクトル */
class alignas(AlignmentMemory::TUPPLE_VARIABLE) Vector4
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	/* MEMO: SIMDを使わない場合、原則CPU側の値を保持しておきます。 */
	CPU::Vector4 Value;

	/* 定数群 */
	static const Vector4 Zero;						/* 全要素0.0f */
	static const Vector4 One;						/* 全要素1.0f */
	static const Vector4 Origin;					/* 同次原点（X,Y,Z = 0.0f, W=1.0f） */
	static const Vector4 AxisX;						/* X要素のみ1.0f（Y,Z,Wは0.0f） */
	static const Vector4 AxisY;						/* Y要素のみ1.0f（X,Z,Wは0.0f） */
	static const Vector4 AxisZ;						/* Z要素のみ1.0f（X,Y,Wは0.0f） */
	static const Vector4 AxisW;						/* W要素のみ1.0f（X,Y,Zは0.0f） *//* 同次座標用 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Vector4(void) :
		Value()
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値
	@param	valueW
		W成分値

	@retval	なし
	*/
	Vector4(float valueX, float valueY, float valueZ, float valueW) :
		Value(valueX, valueY, valueZ, valueW)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector4

	@retval	なし
	*/
	Vector4(const Vector4& value) :
		Value(value.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うCPU::Vector4

	@retval	なし
	*/
	Vector4(const CPU::Vector4& value) :
		Value(value)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Vector4(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値
	@param	valueW
		W成分値

	@retval	なし
	*/
	inline void Set(float valueX, float valueY, float valueZ, float valueW)
	{
		Value.Set(valueX, valueY, valueZ, valueW);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のVector4

	@retval	なし
	*/
	inline void Set(const Vector4& value)
	{
		Value.Set(value.Value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のCPU::Vector4

	@retval	なし
	*/
	inline void Set(const CPU::Vector4& value)
	{
		Value.Set(value);
	}

	/* ********************************************************* */
	//! 値の取得
	/*!
	@param	valueX
		X成分値格納先
	@param	valueY
		Y成分値格納先
	@param	valueZ
		Z成分値格納先
	@param	valueW
		W成分値格納先

	@retval	なし
	@retval	valueX
		X成分値
	@retval	valueY
		Y成分値
	@retval	valueZ
		Z成分値
	@retval	valueW
		W成分値
	*/
	inline void Get(float* valueX, float* valueY, float* valueZ, float* valueW) const
	{
		*valueX = Value.GetX();
		*valueY = Value.GetY();
		*valueZ = Value.GetZ();
		*valueW = Value.GetW();
	}

	/* ********************************************************* */
	//! X成分値の設定
	/*!
	@param	value
		X成分値

	@retval	なし
	*/
	inline void SetX(float value)
	{
		Value.SetX(value);
	}

	/* ********************************************************* */
	//! X成分値の取得
	/*!
	@param	なし

	@retval	X成分値
	*/
	inline float GetX(void) const
	{
		return(Value.GetX());
	}

	/* ********************************************************* */
	//! Y成分値の設定
	/*!
	@param	value
		Y成分値

	@retval	なし
	*/
	inline void SetY(float value)
	{
		Value.SetY(value);
	}

	/* ********************************************************* */
	//! Y成分値の取得
	/*!
	@param	なし

	@retval	Y成分値
	*/
	inline float GetY(void) const
	{
		return(Value.GetY());
	}

	/* ********************************************************* */
	//! Z成分値の設定
	/*!
	@param	value
		Z成分値

	@retval	なし
	*/
	inline void SetZ(float value)
	{
		Value.SetZ(value);
	}

	/* ********************************************************* */
	//! Z成分値の取得
	/*!
	@param	なし

	@retval	Z成分値
	*/
	inline float GetZ(void) const
	{
		return(Value.GetZ());
	}

	/* ********************************************************* */
	//! W成分値の設定
	/*!
	@param	value
		W成分値

	@retval	なし
	*/
	inline void SetW(float value)
	{
		Value.SetW(value);
	}

	/* ********************************************************* */
	//! W成分値の取得
	/*!
	@param	なし

	@retval	W成分値
	*/
	inline float GetW(void) const
	{
		return(Value.GetW());
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 色 */
/* MEMO: 滅多に使用しないかもしれませんが、CPU上で色の合成計算などを行う時のために */
/*       最初からVector4の継承体になっています。                                   */
class Color :
	private Vector4
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* 定数群 */
	static const Color Clear;						/* 全要素0.0f */
	static const Color White;						/* 純白（全要素1.0f） */
	static const Color Black;						/* 純黒（R,G,B = 0.0f, A=1.0f） */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Color(void) :
		Vector4()
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	valueX
		X成分値
	@param	valueY
		Y成分値
	@param	valueZ
		Z成分値
	@param	valueW
		Z成分値

	@retval	なし
	*/
	Color(float valueR, float valueG, float valueB, float valueA) :
		Vector4(valueR, valueG, valueB, valueA)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のVector4

	@retval	なし
	*/
	Color(const Color& value):
		Vector4(value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うCPU::Vector4

	@retval	なし
	*/
	Color(const CPU::Color& value) :
		Vector4(value.GetR(), value.GetG(), value.GetB(), value.GetA())
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Color(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	valueR
		R成分値
	@param	valueG
		G成分値
	@param	valueB
		B成分値
	@param	valueA
		A成分値

	@retval	なし
	*/
	inline void Set(float valueR, float valueG, float valueB, float valueA)
	{
		Value.Set(valueR, valueG, valueB, valueA);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のColor

	@retval	なし
	*/
	inline void Set(const Color& value)
	{
		Value.Set(value.Value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		元値のCPU::Color

	@retval	なし
	*/
	inline void Set(const CPU::Color& value)
	{
		Value.Set(value.GetR(), value.GetG(), value.GetB(), value.GetA());
	}

	/* ********************************************************* */
	//! 値の取得
	/*!
	@param	valueR
		R成分値格納先
	@param	valueG
		G成分値格納先
	@param	valueB
		B成分値格納先
	@param	valueA
		A成分値格納先

	@retval	なし
	@retval	valueR
		R成分値
	@retval	valueG
		G成分値
	@retval	valueB
		B成分値
	@retval	valueA
		A成分値
	*/
	inline void Get(float* valueR, float* valueG, float* valueB, float* valueA) const
	{
		*valueR = Value.GetX();
		*valueG = Value.GetY();
		*valueB = Value.GetZ();
		*valueA = Value.GetW();
	}

	/* ********************************************************* */
	//! R成分値の設定
	/*!
	@param	value
		R成分値

	@retval	なし
	*/
	inline void SetR(float value)
	{
		Value.SetX(value);
	}

	/* ********************************************************* */
	//! R成分値の取得
	/*!
	@param	なし

	@retval	R成分値
	*/
	inline float GetR(void) const
	{
		return(Value.GetX());
	}

	/* ********************************************************* */
	//! G成分値の設定
	/*!
	@param	value
		G成分値

	@retval	なし
	*/
	inline void SetG(float value)
	{
		Value.SetY(value);
	}

	/* ********************************************************* */
	//! G成分値の取得
	/*!
	@param	なし

	@retval	G成分値
	*/
	inline float GetG(void) const
	{
		return(Value.GetY());
	}

	/* ********************************************************* */
	//! B成分値の設定
	/*!
	@param	value
		B成分値

	@retval	なし
	*/
	inline void SetB(float value)
	{
		Value.SetZ(value);
	}

	/* ********************************************************* */
	//! B成分値の取得
	/*!
	@param	なし

	@retval	B成分値
	*/
	inline float GetB(void) const
	{
		return(Value.GetZ());
	}

	/* ********************************************************* */
	//! A成分値の設定
	/*!
	@param	value
		A成分値

	@retval	なし
	*/
	inline void SetA(float value)
	{
		Value.SetW(value);
	}

	/* ********************************************************* */
	//! A成分値の取得
	/*!
	@param	なし

	@retval	A成分値
	*/
	inline float GetA(void) const
	{
		return(Value.GetW());
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 4x3行列 */
class Matrix4x3
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	CPU::Matrix4x3 Value;

	/* 定数群 */
	static const Matrix4x3 Zero;					/* 全要素0.0f */
	static const Matrix4x3 Unit;					/* 単位行列 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Matrix4x3(void) :
		Value(Unit.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	_m00～\m33
		各成分値

	@retval	なし

	表記順は行優先（行で4次元ベクトルが表記される）なので注意してください。
	*/
	Matrix4x3(	float _m00, float _m10, float _m20, float _m30,
				float _m01, float _m11, float _m21, float _m31,
				float _m02, float _m12, float _m22, float _m32
			) :
		Value(	_m00, _m10, _m20, _m30,
				_m01, _m11, _m21, _m31,
				_m02, _m12, _m22, _m32
			)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	Matrix4x3(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, const Vector3& axisW) :
		Value(axisX.Value, axisY.Value, axisZ.Value, axisW.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のMatrix4x4

	@retval	なし
	*/
	Matrix4x3(const Matrix4x3& value) :
		Value(value.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うCPU::Matrix4x4

	@retval	なし
	*/
	Matrix4x3(const CPU::Matrix4x3& value) :
		Value(value)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Matrix4x3(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のMatrix4x3

	@retval	なし
	*/
	void Set(const Matrix4x3& value)
	{
		Value.Set(value.Value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のCPU::Matrix4x3

	@retval	なし
	*/
	void Set(const CPU::Matrix4x3& value)
	{
		Value.Set(value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値（平行移動成分）

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	void Set(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ, const Vector3& axisW)
	{
		Value.Set(axisX.Value, axisY.Value, axisZ.Value, axisW.Value);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class CPU::Matrix4x3;
};

/* 4x4行列 */
class Matrix4x4
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: 内部実装で他のクラスなどから覗くためpublicにしていますが、原則外部から使用しないでください。 */
	/*       ※プラットフォーム依存のため、他プラットフォームとの互換性がありません。                     */
	/* MEMO: SIMDを使わない場合、原則CPU側の値を保持しておきます。 */
	CPU::Matrix4x4 Value;

	/* 定数群 */
	static const Matrix4x4 Zero;					/* 全要素0.0f */
	static const Matrix4x4 Unit;					/* 単位行列 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Matrix4x4(void) :
		Value(Unit.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	_m00～\m33
		各成分値

	@retval	なし

	表記順は行優先（行で4次元ベクトルが表記される）なので注意してください。
	*/
	Matrix4x4(	float _m00, float _m10, float _m20, float _m30,
				float _m01, float _m11, float _m21, float _m31,
				float _m02, float _m12, float _m22, float _m32,
				float _m03, float _m13, float _m23, float _m33
			) :
		Value(	_m00, _m10, _m20, _m30,
				_m01, _m11, _m21, _m31,
				_m02, _m12, _m22, _m32,
				_m03, _m13, _m23, _m33
			)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値（平行移動成分）

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	Matrix4x4(const Vector4& axisX, const Vector4& axisY, const Vector4& axisZ, const Vector4& axisW) :
		Value(axisX.Value, axisY.Value, axisZ.Value, axisW.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使う別のMatrix4x4

	@retval	なし
	*/
	Matrix4x4(const Matrix4x4& value) :
		Value(value.Value)
	{
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	value
		初期値に使うCPU::Matrix4x4

	@retval	なし
	*/
	Matrix4x4(const CPU::Matrix4x4& value) :
		Value(value)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~Matrix4x4(void)
	{
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のMatrix4x4

	@retval	なし
	*/
	void Set(const Matrix4x4& value)
	{
		Value.Set(value.Value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	value
		設定する元のCPU::Matrix4x4

	@retval	なし
	*/
	void Set(const CPU::Matrix4x4& value)
	{
		Value.Set(value);
	}

	/* ********************************************************* */
	//! 値の設定
	/*!
	@param	axisX
		X軸成分値
	@param	axisY
		Y軸成分値
	@param	axisZ
		Z軸成分値
	@param	axisW
		W軸成分値（平行移動成分）

	@retval	なし

	各ベクトルが変形行列内の各軸要素になるように設定します。
	*/
	void Set(const Vector4& axisX, const Vector4& axisY, const Vector4& axisZ, const Vector4& axisW)
	{
		Value.Set(axisX.Value, axisY.Value, axisZ.Value, axisW.Value);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class CPU::Matrix4x4;
};

/* ----------------------------------------------- Enums & Constants */

/* ----------------------------------------------- Variables */

/* ----------------------------------------------- Functions */

/* ----------------------------------------------- Operators */
}	/* FPU */

/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */
namespace Platform	{
extern const char* SignatureNamePlatform;
extern const char* SignatureVersionPlatform;
extern const char* SignatureAuthorPlatform;
}	/* Platform */

extern const char* TextEmpty;					/* 空文字 *//* ※いちいち空文字を記載するのもメモリの無駄ではあるので……。 */

extern const float PI;							/* 円周率 */
extern const float CoefficientRadianToDegree;	/* 変換係数: ラジアン→角度 */
extern const float CoefficientDegreeToRadian;	/* 変換係数: 角度→ラジアン */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */
/* 数式ラッピング関数系 */
/* MEMO: 基本的に本関数群を使用してベクトルや行列の演算子を積んでいます。 */
/*       ※ライブラリの演算子を直接使ってしまうと、共通コード部で色々問題 */
/*         が出てしまうため。                                             */
/* ********************************************************* */
//! ベクトル同士の加算（ベクトル・CPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を加算（result = lhs + rhs）します。
*/
inline void VectorAdd(CPU::Vector2* result, const CPU::Vector2& lhs, const CPU::Vector2& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Yに分けないこと。 */
	result->Set(	lhs.GetX() + rhs.GetX(),
					lhs.GetY() + rhs.GetY()
			);
}
inline void VectorAdd(CPU::Vector3* result, const CPU::Vector3& lhs, const CPU::Vector3& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Zに分けないこと。 */
	result->Set(	lhs.GetX() + rhs.GetX(),
					lhs.GetY() + rhs.GetY(),
					lhs.GetZ() + rhs.GetZ()
			);
}
inline void VectorAdd(CPU::Vector4* result, const CPU::Vector4& lhs, const CPU::Vector4& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Z/Wに分けないこと。 */
	result->Set(	lhs.GetX() + rhs.GetX(),
					lhs.GetY() + rhs.GetY(),
					lhs.GetZ() + rhs.GetZ(),
					lhs.GetW() + rhs.GetW()
			);
}

/* ********************************************************* */
//! ベクトル同士の加算（ベクトル・FPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を加算（result = lhs + rhs）します。
*/
inline void VectorAdd(FPU::Vector2* result, const FPU::Vector2& lhs, const FPU::Vector2& rhs)
{
	VectorAdd(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorAdd(FPU::Vector3* result, const FPU::Vector3& lhs, const FPU::Vector3& rhs)
{
	VectorAdd(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorAdd(FPU::Vector4* result, const FPU::Vector4& lhs, const FPU::Vector4& rhs)
{
	VectorAdd(&result->Value, lhs.Value, rhs.Value);
}

/* ********************************************************* */
//! ベクトル同士の減算（ベクトル・CPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を減算（result = lhs - rhs）します。
*/
inline void VectorSub(CPU::Vector2* result, const CPU::Vector2& lhs, const CPU::Vector2& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Yに分けないこと。 */
	result->Set(	lhs.GetX() - rhs.GetX(),
					lhs.GetY() - rhs.GetY()
			);
}
inline void VectorSub(CPU::Vector3* result, const CPU::Vector3& lhs, const CPU::Vector3& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Zに分けないこと。 */
	result->Set(	lhs.GetX() - rhs.GetX(),
					lhs.GetY() - rhs.GetY(),
					lhs.GetZ() - rhs.GetZ()
			);
}
inline void VectorSub(CPU::Vector4* result, const CPU::Vector4& lhs, const CPU::Vector4& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Z/Wに分けないこと。 */
	result->Set(	lhs.GetX() - rhs.GetX(),
					lhs.GetY() - rhs.GetY(),
					lhs.GetZ() - rhs.GetZ(),
					lhs.GetW() - rhs.GetW()
			);
}

/* ********************************************************* */
//! ベクトル同士の減算（ベクトル・FPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を減算（result = lhs - rhs）します。
*/
inline void VectorSub(FPU::Vector2* result, const FPU::Vector2& lhs, const FPU::Vector2& rhs)
{
	VectorSub(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorSub(FPU::Vector3* result, const FPU::Vector3& lhs, const FPU::Vector3& rhs)
{
	VectorSub(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorSub(FPU::Vector4* result, const FPU::Vector4& lhs, const FPU::Vector4& rhs)
{
	VectorSub(&result->Value, lhs.Value, rhs.Value);
}

/* ********************************************************* */
//! ベクトル同士の乗算（ベクトル・CPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を乗算（result = lhs * rhs）します。
*/
inline void VectorMul(CPU::Vector2* result, const CPU::Vector2& lhs, const CPU::Vector2& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Yに分けないこと。 */
	result->Set(	lhs.GetX() * rhs.GetX(),
					lhs.GetY() * rhs.GetY()
			);
}
inline void VectorMul(CPU::Vector3* result, const CPU::Vector3& lhs, const CPU::Vector3& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Zに分けないこと。 */
	result->Set(	lhs.GetX() * rhs.GetX(),
					lhs.GetY() * rhs.GetY(),
					lhs.GetZ() * rhs.GetZ()
			);
}
inline void VectorMul(CPU::Vector4* result, const CPU::Vector4& lhs, const CPU::Vector4& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Z/Wに分けないこと。 */
	result->Set(	lhs.GetX() * rhs.GetX(),
					lhs.GetY() * rhs.GetY(),
					lhs.GetZ() * rhs.GetZ(),
					lhs.GetW() * rhs.GetW()
			);
}

/* ********************************************************* */
//! ベクトル同士の乗算（ベクトル・FPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を乗算（result = lhs * rhs）します。
*/
inline void VectorMul(FPU::Vector2* result, const FPU::Vector2& lhs, const FPU::Vector2& rhs)
{
	VectorMul(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorMul(FPU::Vector3* result, const FPU::Vector3& lhs, const FPU::Vector3& rhs)
{
	VectorMul(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorMul(FPU::Vector4* result, const FPU::Vector4& lhs, const FPU::Vector4& rhs)
{
	VectorMul(&result->Value, lhs.Value, rhs.Value);
}

/* ********************************************************* */
//! ベクトル同士の除算（ベクトル・CPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を除算（result = lhs / rhs）します。
*/
inline void VectorDiv(CPU::Vector2* result, const CPU::Vector2& lhs, const CPU::Vector2& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Yに分けないこと。 */
	result->Set(	lhs.GetX() / rhs.GetX(),
					lhs.GetY() / rhs.GetY()
			);
}
inline void VectorDiv(CPU::Vector3* result, const CPU::Vector3& lhs, const CPU::Vector3& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Zに分けないこと。 */
	result->Set(	lhs.GetX() / rhs.GetX(),
					lhs.GetY() / rhs.GetY(),
					lhs.GetZ() / rhs.GetZ()
			);
}
inline void VectorDiv(CPU::Vector4* result, const CPU::Vector4& lhs, const CPU::Vector4& rhs)
{
	/* MEMO: resultとlhs/rhsの実体が同じ可能性があるので、SetX/Y/Z/Wに分けないこと。 */
	result->Set(	lhs.GetX() / rhs.GetX(),
					lhs.GetY() / rhs.GetY(),
					lhs.GetZ() / rhs.GetZ(),
					lhs.GetW() / rhs.GetW()
			);
}

/* ********************************************************* */
//! ベクトル同士の除算（ベクトル・FPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を除算（result = lhs / rhs）します。
*/
inline void VectorDiv(FPU::Vector2* result, const FPU::Vector2& lhs, const FPU::Vector2& rhs)
{
	VectorDiv(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorDiv(FPU::Vector3* result, const FPU::Vector3& lhs, const FPU::Vector3& rhs)
{
	VectorDiv(&result->Value, lhs.Value, rhs.Value);
}
inline void VectorDiv(FPU::Vector4* result, const FPU::Vector4& lhs, const FPU::Vector4& rhs)
{
	VectorDiv(&result->Value, lhs.Value, rhs.Value);
}

/* ********************************************************* */
//! ベクトルをスカラ乗算（CPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を乗算（result = lhs * rhs）します。
*/
inline void VectorMul(CPU::Vector2* result, const CPU::Vector2& lhs, Float32 rhs)
{
	/* MEMO: resultとlhsの実体が同じ可能性があるので、SetX/Yに分けないこと。 */
	result->Set(	lhs.GetX() * rhs,
					lhs.GetY() * rhs
			);
}
inline void VectorMul(CPU::Vector3* result, const CPU::Vector3& lhs, Float32 rhs)
{
	/* MEMO: resultとlhsの実体が同じ可能性があるので、SetX/Y/Zに分けないこと。 */
	result->Set(	lhs.GetX() * rhs,
					lhs.GetY() * rhs,
					lhs.GetZ() * rhs
			);
}
inline void VectorMul(CPU::Vector4* result, const CPU::Vector4& lhs, Float32 rhs)
{
	/* MEMO: resultとlhsの実体が同じ可能性があるので、SetX/Y/Z/Wに分けないこと。 */
	result->Set(	lhs.GetX() * rhs,
					lhs.GetY() * rhs,
					lhs.GetZ() * rhs,
					lhs.GetW() * rhs
			);
}

/* ********************************************************* */
//! ベクトルをスカラ乗算（FPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

ベクトル同士を乗算（result = lhs * rhs）します。
*/
inline void VectorMul(FPU::Vector2* result, const FPU::Vector2& lhs, Float32 rhs)
{
	VectorMul(&result->Value, lhs.Value, rhs);
}
inline void VectorMul(FPU::Vector3* result, const FPU::Vector3& lhs, Float32 rhs)
{
	VectorMul(&result->Value, lhs.Value, rhs);
}
inline void VectorMul(FPU::Vector4* result, const FPU::Vector4& lhs, Float32 rhs)
{
	VectorMul(&result->Value, lhs.Value, rhs);
}

/* MEMO: 行列関係は（コストが高いので）さすがにCPU側ではやらないと思うので、（間違いの防止のために）実装していません。 */
/* ********************************************************* */
//! 行列同士の乗算（4x4行列・FPU用）
/*!
@param	result
	結果
@param	lhs
	左項
@param	rhs
	右項

@retval	なし

4x4行列同士を乗算（result = lhs * rhs）します。
lhsとrhsの乗算順序については、後ろからの乗算になります（そのため
	アプリケーション側の行列実装と計算順序が合わない可能性があります）。

実装時のための検算用情報を下記します（Matrix4x4の格納方向は実装によって
	変わると思われるので、コンストラクタでの並びで配列要素を記載します）。

FPU::Matrix4x4 matrix01
(      0.0f,    1.0f,    2.0f,    3.0f,
       4.0f,    5.0f,    6.0f,    7.0f,
       8.0f,    9.0f,   10.0f,   11.0f,
      12.0f,   13.0f,   14.0f,   15.0f   );
FPU::Matrix4x4 matrix02
(    100.0f,  101.0f,  102.0f,  103.0f,
     104.0f,  105.0f,  106.0f,  107.0f,
     108.0f,  109.0f,  110.0f,  111.0f,
     112.0f,  113.0f,  114.0f,  115.0f   );
の時に、本関数
MatrixMul(&matrixResult, matrix01, matrix02);
……の結果は、下記の内容である必要があります。

FPU::Matrix4x4 matrixResult
(   2456.0f, 2862.0f, 3268.0f, 3674.0f, 
    2552.0f, 2974.0f, 3396.0f, 3818.0f, 
    2648.0f, 3086.0f, 3524.0f, 3962.0f, 
    2744.0f, 3198.0f, 3652.0f, 4106.0f   );

※浮動小数の丸め誤差などもあるので精度の厳密性は問いません。
*/
extern void MatrixMul(FPU::Matrix4x4* result, const FPU::Matrix4x4& lhs, const FPU::Matrix4x4& rhs);

/* ********************************************************* */
//! 逆行列の取得（4x4行列・FPU用）
/*!
@param	result
	結果
@param	value
	元値

@retval	関数値
	true == 逆行列が有効
	false == 逆行列が無効（行列式が0）

4x4行列の逆行列を取得（result = value ^ -1）します。

実装時のための検算用情報を下記します（Matrix4x4の格納方向は実装によって
	変わると思われるので、コンストラクタでの並びで配列要素を記載します）。

FPU::Matrix4x4 matrix03
(   3.0f, 1.0f, 1.0f, 2.0f,
    5.0f, 1.0f, 3.0f, 4.0f,
    2.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 3.0f, 2.0f, 1.0f  );
の時に本関数
MatrixInverse(&matrixResult, matrix03);
……の結果は、下記の内容である必要があります。

FPU::Matrix4x4 matrixResult
(    0.5f, -0.2272727f,  0.3636363f, -0.0909090f,
     0.5f, -0.3181818f, -0.0909090f,  0.2727272f,   
    -1.0f,  0.4545454f,  0.2727272f,  0.1818181f, 
     0.0f,  0.2727272f, -0.6363636f, -0.0909090f
);

※浮動小数の丸め誤差などもあるので精度の厳密性は問いません。
*/
extern bool MatrixInverse(FPU::Matrix4x4* result, const FPU::Matrix4x4& value);

/* ********************************************************* */
//! 転置行列の取得（4x4行列・FPU用）
/*!
@param	result
	結果
@param	value
	元値

@retval	なし

4x4行列の転置行列を取得（result = value ^ T）します。
*/
extern void MatrixTranspose(FPU::Matrix4x4* result, const FPU::Matrix4x4& value);

/* ********************************************************* */
//! 変形行列の設定
/*!
@param	result
	結果
@param	translate
	平行移動量
@param	rotation
	各軸回転角度（度）
@param	scaling
	スケーリング

@retval	なし

4x4行列に指定された変形を行う行列を作成します。
回転順序はX-Y-Z回転（SpriteStudio6の回転順序）です。
*/
extern void MatrixCreateTRS(FPU::Matrix4x4* result, const FPU::Vector3& translate, const FPU::Vector3& rotation, const FPU::Vector3& scaling);

/* ********************************************************* */
//! ベクトルの変形（4次元ベクトル＆4x4行列・FPU用）
/*!
@param	result
	結果
@param	vector
	元ベクトル
@param	matrix
	変形行列

@retval	なし

matrixでvectorを変形します。
vectorのW要素が0.0fで方向ベクトル・1.0fで同次座標ベクトルと
	して計算します。
*/
extern void VectorTransform(FPU::Vector4* result, const FPU::Vector4& vector, const FPU::Matrix4x4& matrix);

/* ********************************************************* */
//! 平行投影のプロジェクション行列の取得
/*!
@param	result
	結果
@param	xLeft
	画面の左端のX座標（ピクセル単位）
@param	xRight
	画面の右端のX座標（ピクセル単位）
@param	yTop
	画面の上端のY座標（ピクセル単位）
@param	yBottom
	画面の下端のY座標（ピクセル単位）
@param	zNear
	画面に対して最前のZ座標
@param	zFar
	画面に対して最奥のZ座標

@retval	なし

平行投影のプロジェクション行列を作成します。
本行列は、SpriteStudio6::Entityオブジェクトに渡して、シェーダ内
	でGPU視錐台空間に座標を投影する目的のもののため、プラット
	フォームの基底ライブラリ（及びそれらに依存して作られたシェーダ）
	が要求するプロジェクション行列とは異なる可能性があります。
※具体的には、左手座標系のZ空間が0.0～1.0の空間に投影するための行列です。
	左手系ですので各軸方向は下記になります。
	- X座標は右が+
	- Y座標は上が+
	- Z座標は奥が+

具体的には本関数は、下記の内容を出力します。

width = xRight - xLeft;
height = yTop - yBottom;
depth = zFar - zNear;
offsetX = xRight + xLeft;
offsetY = yTop + yBottom;
offsetZ = zFar + zNear;

CPU::Matrix4x4 result
(    2.0f / width,   0.0f,          0.0f,         -offsetX / width,	
     0.0f,           2.0f / height, 0.0f,         -offsetY / height,	
     0.0f,           0.0f,          1.0f / depth, -offsetZ / depth,	
     0.0f,           0.0f,          0.0f,         1.0f 
);
*/
extern void MatrixGetProjectionOrtho(CPU::Matrix4x4* result, float xLeft, float xRight, float yTop, float yBottom, float zNear, float zFar);

/* ********************************************************* */
//! 遠近法投影のプロジェクション行列の取得
/*!
@param	result
	結果
@param	radianFOV
	視野角（ラジアン単位）
@param	retioAspect
	画面の縦横比（横/縦）
@param	zNear
	画面に対して最前のZ座標
@param	zFar
	画面に対して最奥のZ座標

@retval	なし

遠近法投影のプロジェクション行列を作成します。
本行列は、SpriteStudio6::Entityオブジェクトに渡して、シェーダ内
	でGPU視錐台空間に座標を投影する目的のもののため、プラット
	フォームの基底ライブラリ（及びそれらに依存して作られたシェーダ）
	が要求するプロジェクション行列とは異なる可能性があります。
※具体的には、左手座標系のZ空間が0.0～1.0の空間に投影するための行列です。
	左手系ですので各軸方向は下記になります。
	- X座標は右が+
	- Y座標は上が+
	- Z座標は奥が+

具体的には本関数は、下記の内容を出力します。

invHalfTanAngle = 1.0f / tanf(radianFOV * 0.5f);
invRatioAspect = 1.0f / ratioAspect;
invDepth = 1.0f / (zFar - zNear);

CPU::Matrix4x4 result
(   invRatioAspect * invHalfTanAngle, 0.0f,            0.0f,                0.0f,
    0.0f,                             invHalfTanAngle, 0.0f,                0.0f,
    0.0f,                             0.0f,            zFar * invDepth,     -(zFar * zNear) * invDepth,
    0.0f,                             0.0f,            -1.0f,               1.0f
);
*/
extern void MatrixGetProjectionPerspective(CPU::Matrix4x4* result, float radianFOV, float ratioAspect, float zNear, float zFar);

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */
}	/* SpriteStudio6 */
