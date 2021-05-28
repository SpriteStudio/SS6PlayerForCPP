/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* GPU資産のキャッシュ管理です。                                              */
/*                                                                            */
/* Switchのライブラリ仕様に依存するもので、テクスチャ・モデルなどのGPU資産が  */
/* 一度初期起動してしまうとGPU管理下に置かれ・初期化済かどうかなどを取得する  */
/* 方法がないため、それらの管理を行うための機能です。                         */
/* ※このあたりは載せるタイトルによって変わったりもするため、なるだけプラット */
/*   フォーム依存のGPU処理内でのみ使用して、プレーヤ本体側へもこの処理は隠蔽  */
/*   しておく方が良いだろうと思います（だもので、Platform.h等でもこのファイル */
/*   はできるだけインクルードしたりしないようにしてください）。               */
/*                                                                            */
/* 現在使っているのはテクスチャだけですが、今後でマルチテクスチャやモデルなど */
/* が増えてきた時のために、テンプレート化してあります。                       */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_types.h"
#include <unordered_map>

namespace SpriteStudio6	{
namespace Platform	{
namespace GPU	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 初期化済GPUリソースキャッシュ */
/* MEMO: 内部的に管理形態を変更可能なようにテンプレートクラス化しています。 */
template<typename _typeKey, typename _typeDataCacheIndivisual, size_t _initialCapacityCache=2048> class ManagerResourceCache
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* 内部クラスの型 */
	typedef _typeKey TypeKey;

	/* 初期化済みリソース情報 */
	/* MEMO: ManagerResourceCacheクラスの内部情報クラスです。                                       */
	/*       キャッシュの単位は原則リソースアーカイブ毎で、複数データを内包しているものは初期化時に */
	/*       一斉に定義され・各内包データは格納インデックスを同一性の担保とします。                 */
	/* MEMO: 現在参照回数などの自動解放などは管理していません（参照回数などの管理はタイトル側の都合 */
	/*       に依存するので、かえって都合が悪くなる場合もあるため。）                               */
	/*       だもので、解放処理は明示的に呼ばれる必要があります。                                   */
	class DataCache : public _typeDataCacheIndivisual
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
		void* Data;																		/* リソースデータ格納ポインタ */
		int IndexResource;																/* リソースのインデックス *//* ※通常は0。テクスチャ等のリソースファイル内に複数データを格納できるものの識別時に使います。 */
		int IndexSlotDescriptor;														/* 各内包リソースのディスクリプタプールスロット */

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
		DataCache(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		~DataCache(void)
		{
		}

		/* ********************************************************* */
		//! 未使用化
		/*!
		@param	なし

		@retval	なし
		*/
		void CleanUp(void)
		{
			Data = nullptr;
			IndexResource = -1;
			IndexSlotDescriptor = -1;
		}

		/* ********************************************************* */
		//! 情報が有効かの判定
		/*!
		@param	なし

		@retval	関数値
			true == 有効
			false == 無効情報
		*/
		inline bool IsValid()
		{	/* MEMO: リソースが割りあたっているかで判定 */
			return(nullptr != Data);	/* ? true : false */
		}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	std::unordered_map<TypeKey, DataCache>	Cache;

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
	ManagerResourceCache(void)
	{
		Cache.reserve(_initialCapacityCache);
		Cache.clear();
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	*/
	~ManagerResourceCache(void)
	{
		Cache.clear();	/* 念のため */
	}

	/* ********************************************************* */
	//! キャッシュデータを取得
	/*!
	@param	key
		キー

	@retval	関数値
		キャッシュのポインタ
		nullptr == キーに対応する情報が存在しない

	指定されたキーのキャッシュ情報を取得します。
	※キャッシュが既存かどうかのチェックに使用できます。
	*/
	DataCache* DataGet(TypeKey key)
	{
		if(0 >= Cache.count(key))	{	/* キーが存在しない */
			return(nullptr);
		}

		return(&Cache[key]);
	}

	/* ********************************************************* */
	//! キャッシュデータを取得
	/*!
	@param	key
		キー
	@param	dataCache
		キャッシュに登録する内容

	@retval	関数値
		現在のキャッシュ登録数
		0 == エラー

	指定されたキーで情報を登録します。
	同じキーでの多重登録は認めません（先行優先）。
	本関数終了後にdataCacheの内容は破棄して構いません（コピーされます）。
	*/
	size_t Store(TypeKey key, DataCache& dataCache)
	{
		if(nullptr != DataGet(key))	{	/* 念のため */
			return(0);
		}

#if 0
		std::pair<TypeKey, TypeDataCache> pair;
		pair.first = key;
		pair.second = dataCache;
		Cache.insert(pair);
#else
		Cache[key] = dataCache;
#endif

		return(Cache.size());
	}

	/* ********************************************************* */
	//! キャッシュの解放
	/*!
	@param	key
		キー

	@retval	なし

	指定したキーの情報をキャッシュから削除します。
	*/
	void Release(TypeKey key)
	{
		if(nullptr != DataGet(key))	{	/* キーに対する情報が存在する */
			Cache.erase(key);
		}
	}
	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
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

}	/* GPU */
}	/* Platform */
}	/* SpriteStudio6 */
