/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerでテクスチャを扱う場合の基礎機能です。                            */
/* SS6Playerからは本機能を通してプラットフォームのテクスチャを扱っています。  */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_types.h"

/* 前方宣言 */
namespace SpriteStudio6	{
class Renderer;
}	/* SpriteStudio6 */

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
typedef const void* TypeKeyCacheTexture;									/* テクスチャのキャッシュキーに用いる型（メモリアドレスなのでvoid*） */
extern const TypeKeyCacheTexture KeyCacheTextureInvalid;					/* テクスチャキーの不正値 *//* ※本来は変数ですが解り易いのでここで定義しています */

typedef void* TypeViewTextureRenderTarget;									/* レンダリングターゲット用のテクスチャビュー型 */
extern const TypeViewTextureRenderTarget ViewTextureRenderInvalid;			/* レンダリングターゲット用テクスチャビューの不正値 *//* ※本来は変数ですが解り易いのでここで定義しています */

/* テクスチャ管理 */
class Texture
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 補間種別 */
	enum KindInterpolation	{
		NEAREST_NEIGHBOR = 0,						/* Nearest-Neighbor: 最近傍補間 */
		BILINEAR,									/* Bilinear: 4点加重平均 */

		INTERPOLATION_TERMINATOR,					/* 番兵（使用禁止） */
	};

	/* ラップ種別 */
	enum KindWrap	{
		CLAMP = 0,									/* クランプ */
		REPEAT,										/* 繰り返し */
		MIRROR,										/* 反転繰り返し */

		WRAP_TERMINATOR,							/* 番兵（使用禁止） */
	};

	/* ピクセル型式 */
	enum KindTypePixel	{
		BUFFER_TEST_MASTER = -3,					/* （深度・ステンシル）テスト用バッファ マスタターゲット *//* アプリケーションからは使用しないでください。 */
		COLOR_MASTER = -2,							/* マスタターゲット（色用） *//* アプリケーションからは使用しないでください。 */
		_ERROR = -1,								/* エラー */

		BUFFER_TEST = 0,							/* （深度・ステンシル）テスト用バッファ */
		COLOR_SDR,									/* 色ピクセルSDR用（通常はR8G8B8A8） */
		COLOR_HDR,									/* 色ピクセルHDR用（通常はFloat-RGBA） */
	};

	/* 定数ラベル関係 */
	enum Constants	{
		CACHE_MAX = 2048,							/* キャッシュ最大数 */
	};

	/* ステータスビット */
	enum FlagBitStatus : Uint32	{
		VALID = 0x40000000,							/* 使用可能 */

		USAGE_TARGET = 0x08000000,					/* レンダリングターゲットとして使用可能 *//* ※レンダテクスチャ */
		USAGE_TARGET_MASTER = 0x02000000,			/* マスタターゲット（色・テストとも） */
		USAGE_TARGET_TEST = 0x04000000,				/* レンダリングターゲット時に（深度・ステンシル）バッファとして使用 */

		CLEAR = 0x00000000,
	};

	/* ----------------------------------------------- Variables */
private:
	/* MEMO: privateについてはプラットフォーム毎で内容が変わっても問題ありません。 */
	/*       ※機種非依存部からはアクセスしません。                                */

protected:
public:
	/* MEMO: publicについてはプラットフォーム問わず同じである必要があります。 */
	Uint32 /* FlagBitStatus */ Status;				/* ステータス */
	KindTypePixel TypePixel;						/* ピクセル種別 */
	Sint32 SizePixelX;								/* ピクセルサイズ横 */
	Sint32 SizePixelY;								/* ピクセルサイズ縦 */
	TypeKeyCacheTexture KeyCache;					/* キャッシュキー */

	/* 標準設置リソース関係（static） */
	/* MEMO: マスタターゲットはターゲット設定時の簡便化のために便宜的にテクスチャとして設置してはいますが、 */
	/*       純粋にテクスチャとして見た時には欠落した情報が多いためテクスチャとしては扱えないことがあること */
	/*       に注意してください。                                                                           */
	static Texture TargetMasterColor;				/* マスタターゲット（色）のテクスチャ */
	static Texture TargetMasterTest;				/* マスタターゲット（深度・ステンシル）のテクスチャ */

	/* ----------------------------------------------- Functions */
private:
	inline void CleanUp(void)
	{
		Status = FlagBitStatus::CLEAR;
		TypePixel = KindTypePixel::_ERROR;
		SizePixelX = 0;
		SizePixelY = 0;
		KeyCache = KeyCacheTextureInvalid;
	}

protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Texture(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	（あまりないと思いますが）本クラスを継承することを考慮して、
		仮想デストラクタになっています。
	*/
	virtual ~Texture(void);

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効（値が設定されていない）

	本クラスが描画用として使用可能かを返します。
	*/
	inline bool IsValid(void) const
	{
		return(0 != (Status & FlagBitStatus::VALID));	/* ? true : false */
	}

	/* ********************************************************* */
	//! テクスチャを設定（起動）
	/*!
	@param	keyTexture
		テクスチャのキャッシュキー

	@retval	関数値
		true == 成功
		false == 失敗

	指定されたテクスチャキャッシュのキーを使用して本クラスと実際の
		テクスチャを結びつけます。
	テクスチャキャッシュのキーと本クラスの実体とは1対1である必要は
		なく、同一のテクスチャキャッシュのキーを複数の本クラスの
		実体と共用してもかまいません。
	テクスチャキャッシュのキーは必ずCacheStore静的関数を使用して
		取得されたテクスチャキーを使用してください。
	*/
	bool BootUp(TypeKeyCacheTexture keyTexture);

	/* ********************************************************* */
	//! レンダリングターゲットテクスチャを設定（起動）
	/*!
	@param	type
		ピクセルの種別
	@param	sizePixelX
		作成するテクスチャのピクセル幅横
	@param	sizePixelY
		作成するテクスチャのピクセル幅縦

	@retval	関数値
		true == 成功
		false == 失敗

	指定されたピクセルサイズ・ピクセル種別のレンダリングターゲット
		用のテクスチャを作成して、本クラスと結び付けます。
	本関数を使用してテクスチャを作成する場合、（BootUp関数で行う
		ような）先にテクスチャのキャッシュキーを取得する必要は
		ありません（本クラス内でテクスチャを作成し・テクスチャ
		キャッシュに登録します）。

	また、sizePixelX・sizePixelYは原則正の2のn乗（POT）の必要が
		あります。

	typeにKindTypePixel::BUFFER_TESTを指定した場合、（深度・ステン
		シル）テスト用バッファを作成しますので、そのテクスチャを
		色ターゲット用には（原則として）使用不可能です。
	また、KindTypePixel::COLOR_SDRとKindTypePixel::COLOR_HDRの差は
		下記のようになっています。
	- ピクセルのデータが整数（SDR）であるか浮動小数（HDR）である。
	- 各輝度範囲の最小～最大の幅でクリッピングされるか否か（SDRは
		値領域でクリッピングされますが、HDRはクリッピングされない
		ため、特に加算・減算時で値領域を超えた場合の挙動が異なり
		ます）。
	- メモリの使用サイズ

	※後に本関数の仕様を（キャッシュ登録時にテクスチャを生成する
		手順に）変更するかもしれません。
	*/
	bool BootUpTarget(KindTypePixel type, Sint16 sizePixelX, Sint16 sizePixelY);

	/* ********************************************************* */
	//! テクスチャを解放（終了）
	/*!
	@param	flagReleaseCache
		true == キャッシュも解放する
		false == キャッシュは解放しない

	@retval	なし

	本クラスと実際のテクスチャとの連動を切ります（本クラスを未使用
		状態にします）。
	flagReleaseCacheをtrueにした場合、本クラスが持っているキー（Set
		関数実行時に与えたkeyTexture）をテクスチャキャッシュからも
		解放します（内部的にCacheReleaseを呼び出します）。
	※同一のテクスチャキーを複数の本クラスの実体で共有している場合
		には注意してください（ManagerResourceCacheクラスの実装を変更
		していない限り、複数からの参照を考慮しないで解放するため）。
	※また、描画使用中のテクスチャをflagReleasseCache=trueで解放した
		場合には例外が発生したり（最悪ハングアップ）しますので、注意
		してください。
	*/
	void ShutDown(bool flagReleaseCache=false);

	/* ********************************************************* */
	//! テクスチャの横ピクセルサイズを取得
	/*!
	@param	なし

	@retval	関数値
		テクスチャの横ピクセルサイズ
		-1 == エラー

	本クラスが管理しているテクスチャの横ピクセルサイズを取得します。
	*/
	Sint16 SizeGetPixelX(void) const;

	/* ********************************************************* */
	//! テクスチャの縦ピクセルサイズを取得
	/*!
	@param	なし

	@retval	関数値
		テクスチャの横ピクセルサイズ
		-1 == エラー

	本クラスが管理しているテクスチャの横ピクセルサイズを取得します。
	*/
	Sint16 SizeGetPixelY(void) const;

	/* ********************************************************* */
	//! ディスクリプタスロット番号を取得
	/*!
	@param	なし

	@retval	関数値
		ディスクリプタスロット番号
		-1 == エラー

	本クラスのテクスチャが定義されているディスクリプタスロットを
		返します。
	*/
	Sint32 IndexGetSlotDiscriptor(void) const;

	/* ********************************************************* */
	//! レンダリング用テクスチャビューを取得
	/*!
	@param	なし

	@retval	関数値
		ディスクリプタスロット番号
		-1 == エラー

	テクスチャが持つレンダリングターゲット用のビューを取得します。
	このビュー情報は各プラットフォームで扱いが異なる、レンダラに
		ビュー情報を渡すための情報体になります（つまり実体が何に
		なるかはプラットフォーム毎で異なり、元型情報のポインタを
		reinterpret_castした値になっています）。

	※様々な都合から、publicになっていますが、本来的にはprivate
		で宣言する方が妥当ではある関数です。
		Commonの中から使用しておらず、原則Renderer::SectionTarget
		からしか使われない関数なので、Renderer::SectionTargetの
		実装で本関数を使用しない場合は、常にViewTextureRenderInvalid
		を返すような空関数実装で構いません。	
	*/
	TypeViewTextureRenderTarget ViewGetRenderTarget(void);

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:
	/* ********************************************************* */
	//! テクスチャデータのメモリポインタからキャッシュキーを取得
	/*!
	@param	dataTexture
		テクスチャデータのメモリポインタ

	@retval	関数値
		キャッシュキー
		KeyCacheTextureInvalid == そのメモリポインタは未キャッシュ

	キャッシュから与えられたメモリポインタを持つテクスチャのキー
		を検索してキーを返します。
	*/
	static TypeKeyCacheTexture KeyGetCache(const void* dataTexture);

	/* ********************************************************* */
	//! メモリポインタをキャッシュに登録（テクスチャデータのGPU資産化）
	/*!
	@param	flagExistCache
		定義された際にキャッシュに既存だったかの状態格納先
		nullptr == 本値はいらない（返さないための指定）
	@param	dataTexture
		テクスチャデータのメモリポインタ

	@retval	関数値
		キャッシュキー
		KeyCacheTextureInvalid == 登録失敗/データが受付不可テクスチャ
	@param	flagExistCache
		定義された際にキャッシュに既存だったかの状態
		true == 既存だった（多重定義しようとした）
		false == 初回登録

	キャッシュにテクスチャのメモリポインタをキャッシュします。
	同時にdataTextureをGPUが描画に使用可能な状態に初期化します。

	同じメモリポインタがキャッシュされていた場合、処理を終了した
		ふりをして既存キーを返して正常終了します（その時には
		flagExistCacheの値がtrueになります）。

	dataTextureがconst void*でないのは、本メモリに格納されている
		テクスチャは（ライブラリが）直接データの一部をクラスの
		実体としているため、内容が書き換えられるためconstが保障
		されないためです。
	dataTextureはSpriteStudio::AlignmentMemory::FILE_BUFFERでアライ
		メントされている必要があります。
	*/
	static TypeKeyCacheTexture CacheStore(bool* flagExistCache, void* dataTexture);

	/* ********************************************************* */
	//! キャッシュから削除（テクスチャデータのGPUから奪還）
	/*!
	@param	dataTexture
		テクスチャデータのキャッシュキー

	@retval	なし

	指定されたキャッシュキーに対応すメモリポインタにあるテクスチャ
		をGPU使用状態から奪還してメモリから消去可能にします。

	描画使用中のテクスチャを解放した場合には例外が発生したり（最悪
		ハングアップ）しますので、注意してください。
	*/
	static void CacheRelease(TypeKeyCacheTexture keyCache);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class Renderer;
};

/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
