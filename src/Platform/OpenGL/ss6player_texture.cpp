/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_texture.h"

#include "ss6player_gpu_resource_cache.h"
#include "ss6player_memory.h"
#include "ss6player_system.h"

/* OSS関係 */
/* MEMO: stbを使用しているのはこのモジュールだけなので、直接読み込んでしまう。 */
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/* ========================================================================== */
/* MEMO: 以下、SpriteStudio6名前空間直下に定義するものです。                  */
/* ========================================================================== */
namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */
/* テクスチャのGPUリソース追加情報 */
/* MEMO: （内容が同じなので）機種依存テクスチャの情報を受け取るためのクラスとしても */
/*       流用しています（もし、後に受け取る情報量が増えるような場合、キャッシメモリ */
/*       増加に直結するので、その場合はクラスを分けること。                         */
class DataCacheIndivisualTexture	{
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
	/* テクスチャサイズ */
	/* MEMO: キャッシュ内に持っているのは、個々のTextureクラスで持つと情報の祖語が出る可能性があるので……。 */
	Sint16 SizePixelX;
	Sint16 SizePixelY;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline void CleanUpIndivisual(void)
	{
		SizePixelX = -1;
		SizePixelY = -1;
	}

	DataCacheIndivisualTexture(void)
	{
		CleanUpIndivisual();
	}

	~DataCacheIndivisualTexture(void)
	{
	}

/* ----------------------------------------------- Operators */

/* ----------------------------------------------- Friends */
};

/* MEMO: 記載が若干冗長になるので簡易記載として型定義してあるだけです。 */
typedef Platform::GPU::ManagerResourceCache<TypeKeyCacheTexture, DataCacheIndivisualTexture, Texture::Constants::CACHE_MAX> TypeManagerCacheTexture;

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */
const TypeKeyCacheTexture KeyCacheTextureInvalid = nullptr;					/* テクスチャキーの不正値 */
const TypeViewTextureRenderTarget ViewTextureRenderInvalid = nullptr;		/* レンダリングターゲット用テクスチャビューの不正値 */

Texture Texture::TargetMasterColor;											/* マスタターゲット（色）のテクスチャ */
Texture Texture::TargetMasterTest;											/* マスタターゲット（深度・ステンシル）のテクスチャ */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
/* キャッシュマネージャ */
static TypeManagerCacheTexture ManagerCacheTexture;	/* テクスチャ */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
static bool ValueCheckPow2(int value);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                             Texture::InformationViewTarget */
/* -------------------------------------------------------------------------- */
// Texture::InformationViewTarget::InformationViewTarget(void)
// Texture::InformationViewTarget::~InformationViewTarget(void)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                    Texture */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Texture::Texture(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Texture::~Texture(void)
{
	/* MEMO: 念のため解放しておく。 */
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Texture::CleanUp(void)

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Texture::IsValid(void) const

/* ********************************************************* */
//! テクスチャを設定（起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Texture::BootUp(TypeKeyCacheTexture keyTexture)
{
	if(KeyCacheTextureInvalid == keyTexture)	{	/* キーが不正 */
		return(false);
	}

	/* 定義をチェック */
	TypeManagerCacheTexture::DataCache* cache = ManagerCacheTexture.DataGet(keyTexture);
	if(nullptr == cache)	{	/* 定義されていない */
		return(false);
	}

	/* 情報を設定 */
	CleanUp();

	/* MEMO: 下記を全て設定すること */
	Status |= FlagBitStatus::VALID;

	SizePixelX = cache->SizePixelX;
	SizePixelY = cache->SizePixelY;
	KeyCache = keyTexture;

	return(true);
}

/* ********************************************************* */
//! レンダリングターゲットテクスチャを設定（起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Texture::BootUpTarget(KindTypePixel type, Sint16 sizePixelX, Sint16 sizePixelY)
{
	switch(type)	{
		case KindTypePixel::BUFFER_TEST_MASTER:
		case KindTypePixel::COLOR_MASTER:
			break;

		case KindTypePixel::BUFFER_TEST:
		case KindTypePixel::COLOR_SDR:
		case KindTypePixel::COLOR_HDR:
			if((0 >= sizePixelX) || (0 >= sizePixelY))	{	/* ピクセルサイズが不正 */
				return(false);
			}
			break;

		default:
			return(false);
	}

	CleanUp();
	Status |= FlagBitStatus::USAGE_TARGET;

	/* 作成するテクスチャ毎の特性確定 */
	GLenum format;
	GLenum typePixel;
	switch(type)	{
		case KindTypePixel::BUFFER_TEST_MASTER:
			Status |= FlagBitStatus::USAGE_TARGET_MASTER;
			Status |= FlagBitStatus::USAGE_TARGET_TEST;
			break;

		case KindTypePixel::COLOR_MASTER:
			Status |= FlagBitStatus::USAGE_TARGET_MASTER;
			Status &= ~FlagBitStatus::USAGE_TARGET_TEST;
			break;

		case KindTypePixel::BUFFER_TEST:
			Status &= ~FlagBitStatus::USAGE_TARGET_MASTER;
			Status |= FlagBitStatus::USAGE_TARGET_TEST;

			format = GL_DEPTH_STENCIL;
			typePixel = GL_UNSIGNED_INT_24_8;
			break;

		case KindTypePixel::COLOR_SDR:
			Status &= ~FlagBitStatus::USAGE_TARGET_MASTER;
			Status &= ~FlagBitStatus::USAGE_TARGET_TEST;

			format = GL_RGBA;
			typePixel = GL_UNSIGNED_BYTE;
			break;

		case KindTypePixel::COLOR_HDR:
			Status &= ~FlagBitStatus::USAGE_TARGET_MASTER;
			Status &= ~FlagBitStatus::USAGE_TARGET_TEST;

			/* MEMO: 実用上困らないと思うので半精度にしていますが、精度的に問題があればGL_FLOATでも構わない。 */
			format = GL_RGBA;
			typePixel = GL_HALF_FLOAT;
			break;

		default:
			return(false);
	}

	/* テクスチャの作成 */
	if(0 != (Status & FlagBitStatus::USAGE_TARGET_MASTER))	{	/* マスタターゲット */
		/* MEMO: マスタターゲットの場合は、現在内容は空のままです（ステータスだけがあれば良い）。 */
		/*       ※後にもう少し厳密に設定するかもしれませんが、現在は必要ないと思われる。         */
	} else {	/* テクスチャターゲット */
		/* テクスチャを作成 */
		GLuint idTexture;
		glGenTextures(1, &idTexture);
		if(0 == idTexture)	{	/* 失敗 */
			return(false);
		}
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D(	GL_TEXTURE_2D,
						0,						/* レベル（MIPマップは作成しない） */
						(GLint)format,			/* 内部形式 */
						sizePixelX, sizePixelY,	/* ピクセルサイズX, ピクセルサイズY */
						0,						/* ボーダー（0固定） */
						format,					/* 形式 */
						typePixel,				/* ピクセルタイプ */
						0						/* 転送する画像（何も転送しない） */
					);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		/* テクスチャをキャッシュに登録 */
		/* MEMO: 現状はキーはクラスのポインタにしておく。 */
		KeyCache = KeyGetCache((void*)this);
		if(KeyCacheTextureInvalid != KeyCache)	{	/* 既存 */
			/* MEMO: 既存していた場合は一応そのまま帰る。                                  */
			/*       ※同じTextureでShutDown前に再度本関数を呼ばない限りは起こらないはず。 */
			return(false);
		}
		KeyCache = (void*)this;
		TypeManagerCacheTexture::DataCache cacheTemp;
		cacheTemp.Data = (void*)this;	/* データの実体がCPUメモリ上にないため、クラスのポインタを入れておく */
		if(0 == ManagerCacheTexture.Store(KeyCache, cacheTemp))	{	/* エラー */
			return(KeyCacheTextureInvalid);
		}
		TypeManagerCacheTexture::DataCache* cache = ManagerCacheTexture.DataGet(KeyCache);
		cache->SizePixelX = sizePixelX;
		cache->SizePixelY = sizePixelY;
		cache->IndexResource = 0;	/* リソースのインデックスはない（常に0） */
		cache->IndexSlotDescriptor = (int)idTexture - 1;	/* 通常のテクスチャと同様にID-1を入れておく *//* ※ディスクリプタのエラーが-1なので-1しておくこと */
		if(-1 == cache->IndexSlotDescriptor)	{	/* エラー */
			/* MEMO: テクスチャを解放する */

			/* MEMO: キャッシュを消去して帰る。 */
			ManagerCacheTexture.Release(KeyCache);

			return(false);
		}
	}

	/* テクスチャを有効化 */
	Status |= FlagBitStatus::VALID;

	return(true);
}

/* ********************************************************* */
//! テクスチャを解放（終了）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Texture::ShutDown(bool flagReleaseCache)
{
	if(true == IsValid())	{	/* 有効 */
		if(true == flagReleaseCache)	{	/* 削除指定あり */
			/* キャッシュからの削除 */
			ManagerCacheTexture.Release(KeyCache);
		}
	}

	/* 情報をクリア */
	CleanUp();
}

/* ********************************************************* */
//! テクスチャの横ピクセルサイズを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Sint16 Texture::SizeGetPixelX(void) const
{
	if(false == IsValid())	{	/* 無効 */
		return(-1);
	}
	if(KeyCacheTextureInvalid == KeyCache)	{	/* キャッシュ上にない */
		return(-1);
	}

	TypeManagerCacheTexture::DataCache* cache = ManagerCacheTexture.DataGet(KeyCache);
	return(cache->SizePixelX);
}

/* ********************************************************* */
//! テクスチャの縦ピクセルサイズを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Sint16 Texture::SizeGetPixelY(void) const
{
	if(false == IsValid())	{	/* 無効 */
		return(-1);
	}
	if(KeyCacheTextureInvalid == KeyCache)	{	/* キャッシュ上にない */
		return(-1);
	}

	TypeManagerCacheTexture::DataCache* cache = ManagerCacheTexture.DataGet(KeyCache);
	return(cache->SizePixelY);
}

/* ********************************************************* */
//! テクスチャのディスクリプタを取得
/* --------------------------------------------------------- */
/* MEMO:
 * OpenGLではテクスチャのIDが返ります。
 */
Sint32 Texture::IndexGetSlotDiscriptor(void) const
{
	TypeManagerCacheTexture::DataCache* dataCache = ManagerCacheTexture.DataGet(KeyCache);	/* テクスチャキャッシュ */
	if(KeyCacheTextureInvalid == KeyCache)	{	/* キャッシュ上にない */
		return(-1);
	}

	if(0 <= dataCache->IndexSlotDescriptor)	{
		/* MEMO: ディスクリプタとして入れている値は「テクスチャID-1」なことに注意。 */
		return(dataCache->IndexSlotDescriptor + 1);
	}
	return(-1);
}

/* ********************************************************* */
//! レンダリング用テクスチャビューを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeViewTextureRenderTarget Texture::ViewGetRenderTarget(void)
{
	/* MEMO: OpenGL版では、本関数は呼び出されいない（Renderer::SectionTargetの */
	/*       実装がテクスチャビューを必要としていない）ために、空関数です。    */
	return(ViewTextureRenderInvalid);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Texture [static] */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! テクスチャデータのメモリポインタからキャッシュキーを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeKeyCacheTexture Texture::KeyGetCache(const void* dataTexture)
{
	/* MEMO: （最適化されるはずだが）一応++を前置しておく。 */
	/* MEMO: ループのスコープ中でcacheを確保していないのは、毎度コンストラクタに */
	/*       走られる懸念除けなだけなので……単に念のため。                      */
	TypeManagerCacheTexture::DataCache* cache;
	for(auto iterator = ManagerCacheTexture.Cache.begin(); iterator != ManagerCacheTexture.Cache.end(); ++iterator)	{
		cache = &(iterator->second);
		/* MEMO: 今のキャッシュのtemplateの実装だとキャッシュのデータとキーが同じ */
		/*       実値をとるので検索の必要もないのだが……念のため検索しておく。   */
		if(cache->Data == dataTexture)	{
			return(iterator->first);
		}
	}

	return(KeyCacheTextureInvalid);
}

/* ********************************************************* */
//! メモリポインタをキャッシュに登録（テクスチャデータのGPU資産化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeKeyCacheTexture Texture::CacheStore(bool* flagExistCache, void* dataTexture)
{
	TypeKeyCacheTexture keyCache = KeyGetCache(dataTexture);
	if(KeyCacheTextureInvalid != keyCache)	{	/* 既存 */
		/* MEMO: 情報をそのまま返してしまう。 */
		*flagExistCache = true;

		return(keyCache);
	}
	*flagExistCache = false;

	/* 定義 */
	/* MEMO: 現状はキーはメモリアドレスと同じにしておく。 */
	/* MEMO: キャッシュを先に確保する。 */
	keyCache = dataTexture;
	TypeManagerCacheTexture::DataCache cacheTemp;
	cacheTemp.Data = dataTexture;
	/* MEMO: ディスクリプタはテクスチャの初期化時に返ってきます。 */
	if(0 == ManagerCacheTexture.Store(keyCache, cacheTemp))	{	/* エラー */
		return(KeyCacheTextureInvalid);
	}

	/* 定義されたキャッシュのバッファを取得 */
	TypeManagerCacheTexture::DataCache* cache = ManagerCacheTexture.DataGet(keyCache);

	/* テクスチャデータを解析（非機種依存形式） */
	int widthImage;
	int heightImage;
	int bppImage;
	stbi_uc* result = stbi_load_from_memory(	reinterpret_cast<stbi_uc const*>(dataTexture),
												((int)(UINTPTR_MAX - reinterpret_cast<uintptr_t>(dataTexture)) & INT_MAX),	/* ※実質無制限 */
												&widthImage, &heightImage, &bppImage, 0
										);

	if(nullptr != result)	{	/* stb_imageが対応している形式 */
		if((false == ValueCheckPow2(widthImage)) || (false == ValueCheckPow2(heightImage)))	{	/* NPOT */
			/* MEMO: イメージ解析とキャッシュを消して帰る。 */
			stbi_image_free(reinterpret_cast<void*>(result));
			ManagerCacheTexture.Release(keyCache);

			return(KeyCacheTextureInvalid);
		}

		/* テクスチャを取得 */
		GLuint idTexture = 0;
		glGenTextures(1, &idTexture);
		if(0 == idTexture)	{	/* エラー */
			/* MEMO: イメージ解析とキャッシュを消して帰る。 */
			stbi_image_free(reinterpret_cast<void*>(result));
			ManagerCacheTexture.Release(keyCache);

			return(KeyCacheTextureInvalid);
		}

		GLenum targetType = GL_TEXTURE_2D;
		glBindTexture(targetType, idTexture);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		/* MEMO: テクスチャには補間とラップモードを設定していません。     */
		/*       それらの設定は、Renderer内にあるサンプラが持っています。 */

		switch(bppImage)	{
			case 3:	/* RGB */
				glTexImage2D(targetType, 0, GL_RGBA, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const GLvoid*>(result));
				break;

			case 4:	/* RGBA */
				glTexImage2D(targetType, 0, GL_RGBA, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLvoid*>(result));
				break;

			default:	/* Other */
				/* MEMO: イメージ解析とキャッシュを消して帰る。 */
				stbi_image_free(reinterpret_cast<void*>(result));
				ManagerCacheTexture.Release(keyCache);

				return(KeyCacheTextureInvalid);
		}

		/* キャッシュに情報を設定 */
		cache->SizePixelX = widthImage;
		cache->SizePixelY = heightImage;
		cache->IndexResource = 0;	/* 複数テクスチャではないので常に0 */
		cache->IndexSlotDescriptor = (int)idTexture - 1;	/* ディスクリプタとしてテクスチャIDを入れておく *//* ※ディスクリプタのエラーが-1なので-1しておくこと */

		stbi_image_free(reinterpret_cast<void*>(result));

		return(keyCache);
	}

	/* MEMO: 圧縮テクスチャは一旦読まない。 */
	/* MEMO: キャッシュを消去して帰る。 */
	ManagerCacheTexture.Release(keyCache);

	return(KeyCacheTextureInvalid);
}

/* ********************************************************* */
//! キャッシュから削除（テクスチャデータのGPUから奪還）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Texture::CacheRelease(TypeKeyCacheTexture keyCache)
{
	TypeManagerCacheTexture::DataCache* cache = ManagerCacheTexture.DataGet(keyCache);
	if(nullptr == cache)	{	/* 定義されていない */
		/* MEMO: そのまま帰っておく。 */
		return;
	}

	/* テクスチャを終了 */
	if(0 <= cache->IndexSlotDescriptor)	{
		GLuint textureID = (GLuint)(cache->IndexSlotDescriptor + 1);	/* ディスクリプタに入れる時に-1しているので復帰 */
		glDeleteTextures(1, &textureID);
	}

	/* キャッシュから削除 */
	cache->CleanUpIndivisual();
	cache->CleanUp();
	ManagerCacheTexture.Release(keyCache);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 値が2の階乗かをチェック
/*!
@param	value
	チェックする値
 
@return 関数値
	true == 2のn乗
	false == 2のn乗ではない

現状、65536未満の値しかチェックしません。
*/
static bool ValueCheckPow2(int value)
{
	static const int tablePow2[] = {
		0x0001,	0x0002,	0x0004,	0x0008,
		0x0010,	0x0020,	0x0040,	0x0080,
		0x0100,	0x0200,	0x0400,	0x0800,
		0x1000,	0x2000,	0x4000,	0x8000,
		0x0000
	};

	int pow2 = tablePow2[0];
	for(int i=1; pow2!=0; i++)	{
		if(pow2 == value)	{
			return(true);
		}
		pow2 = tablePow2[i];
	}
	return(false);
}


/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                       [Exceptional-static] */
/* -------------------------------------------------------------------------- */
/* この関数群は、システム全体の初期化・終了に関わる処理のため、プラットフォー */
/* ムやフレームワーク毎で実装が変わるので、呼出側でexternしてください。       */
/* ※ヘッダに含むのが適切でないプロトタイプを持っています。                   */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! テクスチャ管理のシステムを起動
/*!
@param	setting
	システム初期化設定

@retval	関数値
	true == 成功
	false == 失敗

テクスチャを管理するための基底処理を初期化します。
※主にプラットフォームライブラリなどの初期化のための関数なので、
	別途で起動処理が確定しているような場合には本関数は空（や
	呼ばれなくても）問題はありません。

instanceFrameworkGFX/deviceGFXはtexture処理のローカル内でキャッ
	シュ（保持）されます。
*/
bool TextureSystemBootUp(const SettingSystem& setting)
{
	/* フレームワーク・デバイスのキャッシュ */
	/* MEMO: この変数群はRendererで初期化します。 */
//	InstanceFrameworkGFX = instanceFrameworkGFX;
//	DeviceGFX = deviceGFX;

	/* キャッシュマネージャの初期化 */
	/* MEMO: 処理としては暫定（パージの方法を考えないとリソースがそのまま残される）。 */
	ManagerCacheTexture.Cache.clear();	/* テクスチャ */

	/* マスタターゲットの取得 */
	Texture::TargetMasterColor.BootUpTarget(Texture::KindTypePixel::COLOR_MASTER, -1, -1);
	Texture::TargetMasterTest.BootUpTarget(Texture::KindTypePixel::BUFFER_TEST_MASTER, -1, -1);

	return(true);
}

/* ********************************************************* */
//! テクスチャ管理のシステムを終了
/*!
@param	なし

@retval	なし

SystemBootUp関数で起動した基底処理を終了します。
※主にプラットフォームライブラリなどの終了処理のための関数なので、
	別途で終了処理が確定しているような場合には本関数は空（や呼ば
	れなくても）問題はありません。
*/

void TextureSystemShutDown(void)
{
	/* キャッシュマネージャの終了 */
	/* MEMO: 処理としては暫定（パージの方法を考えないとリソースがそのまま残される）。 */
	ManagerCacheTexture.Cache.clear();	/* テクスチャ */

	/* フレームワーク・デバイスのクリア */
	/* MEMO: この変数群はRendererで消去します。 */
//	DeviceGFX = nullptr;
//	InstanceFrameworkGFX = nullptr;
}

}	/* SpriteStudio6 */
