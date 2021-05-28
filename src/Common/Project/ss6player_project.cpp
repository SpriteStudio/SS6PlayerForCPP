/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_project.h"

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                    Project */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Project::CleanUp(void)
{
	Raw = nullptr;
	InstanceTexture = nullptr;

//	Data
//	CellMap
	TableIndexTexture = nullptr;
	Animation = nullptr;
	Effect = nullptr;
	Sequence = nullptr;

	Texture = nullptr;
}

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Project::Project(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Project::~Project(void)
{
//	CleanUp();
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Project::IsValid(void) const

/* ********************************************************* */
//! 本クラスの起動（実行用初期化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Project::BootUp(const void* data)
{
	CleanUp();

	/* プロジェクトにデータを設定 */
	if(false == Data.BootUp(data))	{
		return(false);
	}
	int count;

	/* 生データ位置をキャッシュ */
	/* MEMO: 本来であれば初期化が完全に終了したタイミングで入れるのが常道ですが、 */
	/*       内部実装で多用しているIsValidの関係から、最初に値を入れる。          */
	Raw = data;

	/* セルマップ情報を取得 */
	Data.DataCellMap(&CellMap);

	/* テクスチャ情報を設置 */
	/* MEMO: ここでCountGetTextureを使わないこと。 */
	/* MEMO: 実は実体を持っても良いのではないかと思ったり……。 */
	count = CellMap.CountGetTableCellMap();
	if(0 < count)	{
		int countTexture = CountGetTexture();

		/* テクスチャ管理クラスの実体の初期化 */
		/* MEMO: 領域としては無駄になりますが、枚数分だけ実体を取っておきます。 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		InstanceTexture = new SpriteStudio6::Texture[countTexture];
		if(nullptr == InstanceTexture)	{
			ShutDown();
			return(false);
		}
		/* MEMO: 未使用状態にするのはコントラクタにかませています。 */
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: アロケート時のアライメントを自動にしているので、必ずハンドルとヒープの先頭ポインタは同じ値になります。 */
		void* handleInstanceTexture;
		InstanceTexture = reinterpret_cast<SpriteStudio6::Texture*>(Memory::Allocate(&handleInstanceTexture, sizeof(SpriteStudio6::Texture) * countTexture));
		if(nullptr == InstanceTexture)	{
			ShutDown();
			return(false);
		}
		for(int i=0; i<countTexture; i++)	{
			/* コンストラクタを実行 */
			new(&InstanceTexture[i]) SpriteStudio6::Texture();
		}
#endif

		/* テクスチャポインタテーブルの初期化 */
		/* MEMO: この時点では実体と結び付けません（TextureSet関数で割り当てています）。 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		Texture = new SpriteStudio6::Texture*[countTexture];
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: アロケート時のアライメントを自動にしているので、必ずハンドルとヒープの先頭ポインタは同じ値になります。 */
		void* handleTexture;
		Texture = reinterpret_cast<SpriteStudio6::Texture**>(Memory::Allocate(&handleTexture, sizeof(SpriteStudio6::Texture*) * countTexture));
#endif
		if(nullptr == Texture)	{
			ShutDown();
			return(false);
		}
		for(int i=0; i<count; i++)	{
			Texture[i] = nullptr;
		}

		/* セルマップ-使用テクスチャのテーブル */
		/* MEMO: セルマップ数と使用テクスチャ数は必ずしも一致しません（複数のSSCEで単一の */
		/*       テクスチャを使いまわしているような場合）。                               */
		/*       SS6Player for UnityではScript_SpriteStudio6_Rootに設定されているマテリア */
		/*       ルテーブルの並びで解決していましたが、C++版はマテリアルテーブルを形成し  */
		/*       ていないため、本情報を別個に持っています。                               */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		Sint16* tableIndexTexture = new Sint16[count];
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: アロケート時のアライメントを自動にしているので、必ずハンドルとヒープの先頭ポインタは同じ値になります。 */
		void* handleTableIndexTexture;
		Sint16* tableIndexTexture = reinterpret_cast<Sint16*>(Memory::Allocate(&handleTableIndexTexture, sizeof(Sint16) * count));
#endif
		if(nullptr == tableIndexTexture)	{
			ShutDown();
			return(false);
		}
		for(int i=0; i<count; i++)	{
			tableIndexTexture[i] = CellMap.IndexGetTexture(i);
		}
		TableIndexTexture = tableIndexTexture;
	}

	/* アニメーション情報を取得 */
	count = Data.CountDataAnimation();
	if(0 < count)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		Animation = new Library::Data::Project::Animation[count];
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: アロケート時のアライメントを自動にしているので、必ずハンドルとヒープの先頭ポインタは同じ値になります。 */
		void* handleAnimation;
		Animation = reinterpret_cast<Library::Data::Project::Animation*>(Memory::Allocate(&handleAnimation, sizeof(Library::Data::Project::Animation) * count));
#endif
		if(nullptr == Animation)	{
			ShutDown();
			return(false);
		}
		for(int i=0; i<count; i++)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
			/* コンストラクタを実行 */
			new(&Animation[i]) Library::Data::Project::Animation;
#endif

			/* MEMO: アニメーション情報を取得・アトリビュート取得クラスを設定 */
			Data.DataAnimation(&Animation[i], i);
			Animation[i].AttributeExpanderSetUp();
		}
	}

	/* エフェクト情報を取得 */
	count = Data.CountDataEffect();
	if(0 < count)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		Effect = new Library::Data::Project::Effect[count];
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: アロケート時のアライメントを自動にしているので、必ずハンドルとヒープの先頭ポインタは同じ値になります。 */
		void* handleEffect;
		Effect = reinterpret_cast<Library::Data::Project::Effect*>(Memory::Allocate(&handleEffect, sizeof(Library::Data::Project::Effect) * count));
#endif
		if(nullptr == Effect)	{
			ShutDown();
			return(false);
		}
		for(int i=0; i<count; i++)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
			/* コンストラクタを実行 */
			new(&Effect[i]) Library::Data::Project::Effect;
#endif

			Data.DataEffect(&Effect[i], i);
		}
	}

	/* シーケンス情報を取得 */
	count = Data.CountDataSequence();
	if(0 < count)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		Sequence = new Library::Data::Project::Sequence[count];
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: アロケート時のアライメントを自動にしているので、必ずハンドルとヒープの先頭ポインタは同じ値になります。 */
		void* handleSequence;
		Sequence = reinterpret_cast<Library::Data::Project::Sequence*>(Memory::Allocate(&handleSequence, sizeof(Library::Data::Project::Sequence) * count));
#endif
		if(nullptr == Sequence)	{
			ShutDown();
			return(false);
		}
		for(int i=0; i<count; i++)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
			/* コンストラクタを実行 */
			new(&Sequence[i]) Library::Data::Project::Sequence;
#endif

			/* MEMO: アニメーション情報を取得・アトリビュート取得クラスを設定 */
			Data.DataSequence(&Sequence[i], i);
		}
	}

	return(true);
}

/* ********************************************************* */
//! 本クラスの終了（未使用化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Project::ShutDown(void)
{
	int countTexture = CellMap.CountGetTableCellMap();

	/* 生データ位置をキャッシュ */
	/* MEMO: 一応終了開始タイミングで消しておきます。 */
	Raw = nullptr;

	/* テクスチャ情報を終了 */
	if(nullptr != Texture)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete[] Texture;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		Memory::Free(Texture);
#endif
		Texture = nullptr;
	}
	if(nullptr != InstanceTexture)	{
		for(int i=0; i<countTexture; i++)	{
			if(true == InstanceTexture[i].IsValid())	{
				/* MEMO: キャッシュからも削除します。 */
				InstanceTexture[i].ShutDown(true);

#if 0	/* MEMO: 直接newで書いてしまう場合 */
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
				InstanceTexture[i].~Texture();
#endif
			}
		}

#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete[] InstanceTexture;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		Memory::Free(InstanceTexture);
#endif
		InstanceTexture = nullptr;
	}
	if(nullptr != TableIndexTexture)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete[] TableIndexTexture;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		Memory::Free(TableIndexTexture);
#endif
		TableIndexTexture = nullptr;
	}

	/* セルマップ情報を終了 */
//	CellMap.CleanUp();

	/* アニメーション情報を終了 */
	if(nullptr != Animation)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete[] Animation;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: 単なるFlatBuffersを覗くための情報構造体なので、デストラクトしないで直接解放しています。 */
		Memory::Free(Animation);
#endif
		Animation = nullptr;
	}

	/* エフェクト情報を終了 */
	if(nullptr != Effect)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete[] Effect;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: 単なるFlatBuffersを覗くための情報構造体なので、デストラクトしないで直接解放しています。 */
		Memory::Free(Effect);
#endif
		Effect = nullptr;
	}

	/* エフェクト情報を終了 */
	if(nullptr != Sequence)	{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete[] Sequence;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		/* MEMO: 単なるFlatBuffersを覗くための情報構造体なので、デストラクトしないで直接解放しています。 */
		Memory::Free(Sequence);
#endif
		Sequence = nullptr;
	}

	/* プロジェクトにデータを終了 */
	Data.ShutDown();

	/* 未使用化 */
	CleanUp();
}

/* ********************************************************* */
//! テクスチャ枚数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::CountGetTexture(void)
{
	if(false == IsValid())	{
		return(-1);
	}

	return(Data.CountNameTexture());
}

/* ********************************************************* */
//! テクスチャ名を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
const char* Project::NameGetTexture(int index)
{
	int countTexture = CountGetTexture();
	if((0 > index) || (countTexture <= index))	{
		return(nullptr);
	}

	return(Data.NameGetTexture(index));
}

/* ********************************************************* */
//! テクスチャを設定（初期起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Project::TextureBootUp(SpriteStudio6::Texture** tableTexture, Sint16* tableIndexTexture)
{
	int countTexture = CountGetTexture();
	for(int i=0; i<countTexture; i++)	{
		tableTexture[i] = TextureGet(i);
		tableIndexTexture[i] = TableIndexTexture[i];
	}

	return(true);
}

/* ********************************************************* */
//! テクスチャを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Project::TextureSet(int index, SpriteStudio6::Texture& texture)
{
	int countTexture = CountGetTexture();
	if((0 > index) || (countTexture <= index))	{
		return(false);
	}

	/* テクスチャをテーブルに設定 */
	Texture[index] = &texture;

	return(true);
}

/* ********************************************************* */
//! テクスチャを設定（本クラス内部生成テクスチャ）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Project::TextureSet(int index, void* dataTexture)
{
	int countTexture = CountGetTexture();
	if((0 > index) || (countTexture <= index))	{
		return(false);
	}

	/* テクスチャをキャッシュに登録＋テクスチャ管理クラスを起動 */
	bool flagExistCacheTexture;
	TypeKeyCacheTexture keyCacheTexture = Texture::CacheStore(&flagExistCacheTexture, dataTexture);
	if(false == InstanceTexture[index].BootUp(keyCacheTexture))	{	/* 失敗 */
		return(false);
	}

	/* テクスチャをテーブルに設定 */
	Texture[index] = &InstanceTexture[index];

	return(true);
}

/* ********************************************************* */
//! 現在割り当たっているテクスチャを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
SpriteStudio6::Texture* Project::TextureGet(int index)
{
	int countTexture = CountGetTexture();
	if((0 > index) || (countTexture <= index))	{
		return(nullptr);
	}

	return(Texture[index]);
}

/* ********************************************************* */
//! セルマップ数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::CountGetCellMap(void)
{
	return(CellMap.CountGetTableCellMap());
}

/* ********************************************************* */
//! 名前からセルマップのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::IndexGetCellMap(const char* name)
{
	int count = CountGetCellMap();
	Library::Data::CellMap::Body dataCellMap;
	for(int i=0; i<count; i++)	{
		CellMap.TableCellMap(&dataCellMap, i);
		if(true == Library::Utility::Text::Compare(name,  dataCellMap.Name()))	{
			return(i);
		}
	}

	return(-1);
}

/* ********************************************************* */
//! アニメーションパック数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::CountGetAnimationPack(void)
{
	if(false == IsValid())	{
		return(-1);
	}

	return((int)(Data.CountDataAnimation()));
}

/* ********************************************************* */
//! 名前からアニメーションパックのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::IndexGetAnimationPack(const char* name)
{
	if(false == IsValid())	{
		return(-1);
	}

	return(Data.IndexGetDataAnimation(name));
}


/* ********************************************************* */
//! シーケンスパック数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::CountGetSequencePack(void)
{
	if(false == IsValid())	{
		return(-1);
	}

	return((int)(Data.CountDataSequence()));
}

/* ********************************************************* */
//! 名前からシーケンスパックのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Project::IndexGetSequencePack(const char* name)
{
	if(false == IsValid())	{
		return(-1);
	}

	return(Data.IndexGetDataSequence(name));
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Texture [static] */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
