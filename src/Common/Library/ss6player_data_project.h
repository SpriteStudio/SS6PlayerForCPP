/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerの持っているデータの内、プロジェクト周りの大区分データ構造の定義  */
/* です。                                                                     */
/* 定義順などの問題もあって、data.hから分割して記述されています。             */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include "ss6player_data.h"
#include "ss6player_pack.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Data	{
namespace Project	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 前方宣言 */
class Project;

/* セルマップデータ */
/* MEMO: SS6PUでのScript_SpriteStudio6_DataCellMapクラスと等価です。 */
class CellMap
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* データバージョンコード */
	/* MEMO: SS6のバージョンと対応しているのではなく、SSFB2内で閉じているバージョンコードです。 */
	/* MEMO: 基本的にSS6PUのScript_SpriteStudio6_DataCellMapのバージョンコードと同じです。      */
	enum KindVersion : Uint32	{
		SS5PU = 0x00000000,	/* Before SS5PU *//* (Reserved) */
		CODE_010000 = 0x00010000,	/* SS6PU Ver.0.8.0 */
		CODE_010001 = 0x00010001,	/* SS6PU Ver.0.9.0 */
			/* MEMO: Change Members "Library_SpriteStudio6.Data.CellMap.Cell.DataMesh" */

		SUPPORT_EARLIEST = CODE_010000,
		SUPPORT_LATEST = CODE_010001,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* セルマップデータ本体 */
	const ss::ssfb2::DataCellMap* Value;

	/* ----------------------------------------------- Functions */
private:
	inline void CleanUp(void)
	{
		Value = nullptr;
	}

protected:
public:
	CellMap(void)
	{
		CleanUp();
	}

	inline KindVersion Version(void)
	{
		return((KindVersion)(Value->version()));
	}

	inline bool IsValid(void)
	{
		return(nullptr != Value);	/* ? true : false */
	}

	inline int CountGetTableCellMap(void)
	{
		return((int)((Value->table_cell_map())->size()));
	}
	inline const Library::Data::CellMap::Body* TableCellMap(Library::Data::CellMap::Body* cellMap, int index)
	{
		/* MEMO: 存在するなら常に1つ。 */
		cellMap->Value = (*(Value->table_cell_map()))[index];

		return(cellMap);
	}

	inline int CountGetTableIndexTexture(void)
	{
		return((int)((Value->index_texture())->size()));
	}
	inline const Sint16* TableIndexTexture(void)
	{
		return((const Sint16*)((Value->index_texture())->data()));
	}
	inline Sint16 IndexGetTexture(int indexCellMap)
	{
		/* MEMO: 都度テーブルを取得したくない場合に、本関数を使用してください。 */
		return((*(Value->index_texture()))[indexCellMap]);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* エフェクトデータ */
/* MEMO: SS6PUでのScript_SpriteStudio6_DataEffectクラスと等価です。 */
class Effect
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* データバージョンコード */
	/* MEMO: SS6のバージョンと対応しているのではなく、SSFB2内で閉じているバージョンコードです。 */
	/* MEMO: 基本的にSS6PUのScript_SpriteStudio6_DataEffectのバージョンコードと同じです。       */
	enum KindVersion : Uint32	{
		SS5PU = 0,	/* Before SS5PU *//* (Reserved) */
		CODE_010000,	/* = 0x00010000 (Miss) */	/* SS6PU Ver.1.0.0 */
		CODE_010001 = 0x00010001,	/* SS6PU Ver.1.0.26 */
			/* MEMO: Shaders changed to integrated type */
		CODE_010002 = 0x00010002,	/* SS6PU Ver.1.0.27 */
			/* MEMO: Shader's stencil-test are changed.  */

		SUPPORT_EARLIEST = CODE_010000,
		SUPPORT_LATEST = CODE_010001,
	};

	/* ステータスビット */
	enum FlagBit : Uint32	{
		SEEDRANDOM_LOCK = 0x00000001,

		CLEAR = 0x00000000
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* アニメーションデータ本体 */
	const ss::ssfb2::DataEffect* Value;

	/* ----------------------------------------------- Functions */
private:
	inline void CleanUp(void)
	{
		Value = nullptr;
	}

protected:
public:
	Effect(void)
	{
		CleanUp();
	}

	inline KindVersion Version(void)
	{
		return((KindVersion)(Value->version()));
	}
	inline const char* Name(void)
	{
		return((Value->name())->c_str());
	}
	inline FlagBit FlagData(void)
	{
		return((FlagBit)(Value->flag_data()));
	}

	inline bool IsValid(void)
	{
		/* MEMO: SS6Player for UnityではデータのScriptableObjectが起動したかは特殊なフラグを */
		/*       持っていたが、C++版ではSSFB2が割り当たってるかでチェックする。              */
		return(nullptr != Value);	/* ? true : false */
	}

	inline bool StatusIsLockSeedRandom(void)
	{
		return(0 != (FlagData() & FlagBit::SEEDRANDOM_LOCK));
	}
	inline int SeedRandom(void)
	{
		return((int)(Value->seed_random()));
	}
	inline int VersionRenderer(void)
	{
		return((int)(Value->version_renderer()));
	}
	inline int CountMaxParticle(void)
	{
		return((int)(Value->count_max_particle()));
	}
	inline int CountFramePerSecond(void)
	{
		return((int)(Value->count_frame_per_second()));
	}
	inline CPU::Vector2* ScaleLayout(CPU::Vector2* scaleLayout)
	{
		const ss::ssfb2::Vector2* original = Value->scale_layout();
		scaleLayout->Set(original->x(), original->y());

		return(scaleLayout);
	}
	inline int CountGetTableParts(void)
	{
		return((int)(Value->table_parts()->size()));
	}
	inline Library::Data::Parts::Effect* TableParts(Library::Data::Parts::Effect* effect, int index)
	{
		effect->Value = (*(Value->table_parts()))[index];

		return(effect);
	}
	inline int CountGetTableEmitter(void)
	{
		return((int)(Value->table_emitter()->size()));
	}
	inline Library::Data::Effect::Emitter* TableEmitter(Library::Data::Effect::Emitter* emitter, int index)
	{
		emitter->Value = (*(Value->table_emitter()))[index];

		return(emitter);
	}
	inline int CountTableIndexEmitterOrderDraw(void)
	{
		return((int)(Value->table_index_emitter_order_draw()->size()));
	}
	inline const int* TableIndexEmitterOrderDraw(void)
	{
		return((const int*)(Value->table_index_emitter_order_draw()->data()));
	}

	/* ユーティリティ */
	int IDGetParts(const char* name, size_t length=0);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* アニメーションデータ */
/* MEMO: SS6PUでのScript_SpriteStudio6_DataAnimationクラスと等価です。 */
/* MEMO: 基本的にはFlatBuffersで圧縮されている対象データと、格納されているアトリビュートを展開するための */
/*       展開クラス群を持ちます。                                                                        */
class Animation
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* セットアップアニメーション設定データ */
	/* MEMO: SS6Player for Unityには全データを格納するコンパイルモードがありますが、 */
	/*       C++版にはありません（基本無駄なのでSS6ConverterVer2が出力しません）。   */
	class DataSetup
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
		/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
		const ss::ssfb2::DataSetup* Value;				/* 情報へのポインタ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline bool IsUserData(void) const
		{
			ss::ssfb2::UserDataFlagBit flags = Value->user_data()->flags();
			return(0 != (flags & ss::ssfb2::UserDataFlagBit::UserDataFlagBit_VALID));	/* ? true : false */
		}
		inline Library::Data::Animation::Attribute::UserData* UserData(Library::Data::Animation::Attribute::UserData* userData) const
		{
			const ss::ssfb2::UserData* value = Value->user_data();
			userData->Flags = (Library::Data::Animation::Attribute::UserData::FlagBit)(value->flags());
			userData->NumberInt = (Sint32)(value->number_int());

			const ss::ssfb2::Rect* valueRect = value->rectangle();
			userData->Rectangle.SetLURD(valueRect->x_min(), valueRect->y_min(), valueRect->x_max(), valueRect->y_max());

			const ss::ssfb2::Vector2* valueCoordinate = value->coordinate();
			userData->Coordinate.Set(valueCoordinate->x(), valueCoordinate->y());

			userData->Text = value->text()->c_str();

			return(userData);
		}

		inline bool IsSignal(void) const
		{
			size_t countCommand = ((Value->signal())->TableCommand())->size();
			return(0 < countCommand);	/* ? true : false */
		}
		inline Library::Data::Animation::Attribute::Signal* Signal(Library::Data::Animation::Attribute::Signal* signal) const
		{
			signal->Value = Value->signal();
			return(signal);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* データバージョンコード */
	/* MEMO: SS6のバージョンと対応しているのではなく、SSFB2内で閉じているバージョンコードです。 */
	/* MEMO: 基本的にSS6PUのScript_SpriteStudio6_DataAnimationのバージョンコードと同じです。 */
	enum KindVersion : Uint32	{
		SS5PU = 0,	/* Before SS5PU *//* (Reserved) */
		CODE_010000 = 0x00010000,	/* SS6PU Ver.0.8.0 */
		CODE_010001 = 0x00010001,	/* SS6PU Ver.0.9.0 */
			/* MEMO: Add "Library_SpriteStudio6.Data.Parts.Animation.CountMesh" */
			/* MEMO: Add "Library_SpriteStudio6.Data.Parts.Animation.Catalog" */
		CODE_010002 = 0x00010002,	/* SS6PU Ver.0.9.8 */
			/* MEMO: Add "PackAttribute's Dictionaries" */
		CODE_010003 = 0x00010003,	/* SS6PU Ver.1.0.23 */
			/* MEMO: Add "DataSetup" */
		CODE_010004 = 0x00010004,	/* SS6PU Ver.1.0.26 */
			/* MEMO: Shaders changed to integrated type */
		CODE_010005 = 0x00010005,	/* SS6PU Ver.1.0.27 */
			/* MEMO: Shader's stencil-test are changed.  */
			/* MEMO: "Mask" parts' materials are separated for masking and nonmasking. */

		/* MEMO: Before "CODE_010005", can not play on SS6PU Ver.1.1.0. */
		CODE_010100 = 0x00010100,	/* SS6PU Ver.1.1.0 */
			/* MEMO: Abolition of (NORMAL/MASK)_TRIANGLE2" "_ TRIANGLE4".                                                     */
			/*       Always divide "Normal (Sprite)" and "Mask" into 4 triangles according to SpriteStudio6's specifications. */
			/* MEMO: Support "Mesh Deformation" */
			/* MEMO: Support "Z-Position" Priority */

		SUPPORT_EARLIEST = CODE_010100,
		SUPPORT_LATEST = CODE_010100,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* アニメーションデータ本体 */
	const ss::ssfb2::DataAnimation* Value;

	/* アトリビュート群展開機能群 */
	const Data::Animation::Pack::AttributeExpandStatus* ExpanderStatus;

	const Data::Animation::Pack::AttributeExpandCell* ExpanderCell;

	const Data::Animation::Pack::AttributeExpandVector3* ExpanderPosition;
	const Data::Animation::Pack::AttributeExpandVector3* ExpanderRotation;
	const Data::Animation::Pack::AttributeExpandVector2* ExpanderScaling;
	const Data::Animation::Pack::AttributeExpandVector2* ExpanderScalingLocal;

	const Data::Animation::Pack::AttributeExpandFloat* ExpanderRateOpacity;
	const Data::Animation::Pack::AttributeExpandInt* ExpanderPriority;
	const Data::Animation::Pack::AttributeExpandPartsColor* ExpanderPartsColor;
	const Data::Animation::Pack::AttributeExpandVertexCorrection* ExpanderVertexCorrection;

	const Data::Animation::Pack::AttributeExpandVector2* ExpanderOffsetPivot;
	const Data::Animation::Pack::AttributeExpandVector2* ExpanderPositionAnchor;	/* ※現在未使用（非対応機能） */
	const Data::Animation::Pack::AttributeExpandVector2* ExpanderSizeForce;
	const Data::Animation::Pack::AttributeExpandVector2* ExpanderPositionTexture;
	const Data::Animation::Pack::AttributeExpandFloat* ExpanderRotationTexture;
	const Data::Animation::Pack::AttributeExpandVector2* ExpanderScalingTexture;

	const Data::Animation::Pack::AttributeExpandFloat* ExpanderRadiusCollision;

	const Data::Animation::Pack::AttributeExpandUserData* ExpanderUserData;
	const Data::Animation::Pack::AttributeExpandInstance* ExpanderInstance;
	const Data::Animation::Pack::AttributeExpandEffect* ExpanderEffect;
	const Data::Animation::Pack::AttributeExpandDeform* ExpanderDeform;
	const Data::Animation::Pack::AttributeExpandShader* ExpanderShader;
	const Data::Animation::Pack::AttributeExpandSignal* ExpanderSignal;

	/* ----------------------------------------------- Functions */
private:
	inline void CleanUp(void)
	{
		Value = nullptr;

		ExpanderStatus = nullptr;

		ExpanderCell = nullptr;

		ExpanderPosition = nullptr;
		ExpanderRotation = nullptr;
		ExpanderScaling = nullptr;
		ExpanderScalingLocal = nullptr;

		ExpanderRateOpacity = nullptr;
		ExpanderPriority = nullptr;
		ExpanderPartsColor = nullptr;
		ExpanderVertexCorrection = nullptr;

		ExpanderOffsetPivot = nullptr;
		ExpanderPositionAnchor = nullptr;
		ExpanderSizeForce = nullptr;
		ExpanderPositionTexture = nullptr;
		ExpanderRotationTexture = nullptr;
		ExpanderScalingTexture = nullptr;

		ExpanderRadiusCollision = nullptr;

		ExpanderUserData = nullptr;
		ExpanderInstance = nullptr;
		ExpanderEffect = nullptr;
		ExpanderDeform = nullptr;
		ExpanderShader = nullptr;
		ExpanderSignal = nullptr;
	}

protected:
public:
	Animation(void)
	{
		CleanUp();
	}

	inline KindVersion Version(void)
	{
		return((KindVersion)(Value->version()));
	}

	inline const char* Name(void)
	{
		return((Value->name())->c_str());
	}

	inline int CountTableParts(void)
	{
		return((int)(Value->table_parts()->size()));
	}
	inline Library::Data::Parts::Animation* TableParts(Library::Data::Parts::Animation* parts, int idParts)
	{
		parts->Value = (*(Value->table_parts()))[idParts];

		return(parts);
	}

	inline const Library::Data::Parts::Animation::Catalog* CatalogParts(Library::Data::Parts::Animation::Catalog* catalog)
	{
		catalog->Value = Value->catalog_parts();

		return(catalog);
	}

	inline int CountTableAnimation(void)
	{
		return((int)(Value->table_animation()->size()));
	}
	inline const Library::Data::Animation::Body* TableAnimation(Library::Data::Animation::Body* animation, int index)
	{
		animation->Value = (*(Value->table_animation()))[index];

		return(animation);
	}

	inline bool IsAnimationSetup(void)
	{
		/* MEMO: セットアップアニメーションは0（なし）か1（あり）しかない。 */
		return(0 < Value->table_animation_parts_setup()->size());	/* ? true : false */
	}
	inline const DataSetup* TableAnimationPartsSetup(DataSetup* dataSetUp)
	{
		dataSetUp->Value = (*(Value->table_animation_parts_setup()))[0];

		return(dataSetUp);
	}

	inline bool IsValid(void)
	{
		/* MEMO: SS6Player for UnityではデータのScriptableObjectが起動したかは特殊なフラグを */
		/*       持っていたが、C++版ではSSFB2が割り当たってるかでチェックする。              */
		return(nullptr != Value);	/* ? true : false */
	}

	/* MEMO: 本関数は、原則外部から使用しないでください。 */
	bool AttributeExpanderSetUp(void);

	/* ユーティリティ */
	/* MEMO: 主に本クラスの子・孫から情報を直接取るための機能群です。 */
	/*       ※基本的にSS6Player内部処理で使用しますので、外部からは  */
	/*         なるだけ常用しないでください。                         */
	inline int IDGetPartsParent(int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);

		return(parts.IDParent());
	}
	inline const char* NameGetParts(int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);

		return(parts.Name());
	}
	inline const Sint32* TableGetPartsChild(int* count, int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);
		if(nullptr != count)	{
			*count = parts.CountTableIDChild();
		}

		return(parts.TableIDChild());
	}
	inline Library::Data::Parts::Animation::KindFeature FeatureGetParts(int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);

		return(parts.Feature());
	}
	inline Library::KindOperationBlend::Kind OperationGetPartsBlendTarget(int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);

		return(parts.OperationBlendTarget());
	}
	inline int IndexGetPartsAnimationPackUnderControl(int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);

		return(parts.IndexAnimationPackUnderControl());
	}
	inline const char* NameGetPartsAnimationUnderControl(int idParts)
	{
		Library::Data::Parts::Animation parts;
		TableParts(&parts, idParts);

		return(parts.NameAnimationUnderControl());
	}

	int IndexGetAnimation(const char* name);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* シーケンスデータ */
class Sequence
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* データバージョンコード */
	/* MEMO: SS6のバージョンと対応しているのではなく、SSFB2内で閉じているバージョンコードです。 */
	/* MEMO: 基本的にSS6PUのScript_SpriteStudio6_DataSequenceのバージョンコードと同じです。     */
	enum KindVersion : Uint32	{
		SS5PU = 0x00000000,	/* Before SS5PU *//* (Reserved) */
		CODE_010000 = 0x00010000,	/* SS6PU Ver.1.2.0 */

		SUPPORT_EARLIEST = CODE_010000,
		SUPPORT_LATEST = CODE_010000,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* アニメーションデータ本体 */
	const ss::ssfb2::SequencePack* Value;

	/* ----------------------------------------------- Functions */
private:
	inline void CleanUp(void)
	{
		Value = nullptr;
	}

protected:
public:
	Sequence(void)
	{
		CleanUp();
	}

	inline bool IsValid(void)
	{
		return(nullptr != Value);	/* ? true : false */
	}

	inline KindVersion Version(void)
	{
		return((KindVersion)(Value->version()));
	}

	inline const char* Name(void)
	{
		return((Value->name())->c_str());
	}

	inline int CountGetSequence(void)
	{
		return((int)((Value->sequenceList())->size()));
	}
	int IndexGetSequence(const char* name);
	inline Library::Data::Sequence::Body* SequenceGet(Library::Data::Sequence::Body* sequence, int index)
	{
		sequence->Value = (*(Value->sequenceList()))[index];

		return(sequence);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* プロジェクトデータ */
class Project
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
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::ProjectData* Value;			/* 情報へのポインタ */

	const void* Data;								/* 元データ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	bool BootUp(const void* data)
	{
		Data = data;
		Value = ss::ssfb2::GetProjectData(data);

		return(nullptr != Value);	/* ? true : false */
	}

	void ShutDown(void)
	{
		Data = nullptr;
		Value = nullptr;
	}

	bool IsValid(void)
	{
		return((nullptr != Data) && (nullptr != Value));	/* ? true : false */
	}

	inline const char* Name(void)
	{
		return((Value->name())->c_str());
	}

	inline CellMap* DataCellMap(CellMap* cellMap)
	{
		/* MEMO: セルマップが存在していないプロジェクトは基本的にないと思われるのと、 */
		/*       存在していない場合は、セルマップ情報の内容が空扱いになります。       */
		cellMap->Value = Value->cell_map();

		return(cellMap);
	}

	inline int CountDataAnimation(void)
	{
		return((int)(Value->animation()->size()));
	}
	int IndexGetDataAnimation(const char* name);
	inline Animation* DataAnimation(Animation* animation, int index)
	{
		animation->Value = (*(Value->animation()))[index];

		return(animation);
	}

	inline int CountDataEffect(void)
	{
		return((int)(Value->effect()->size()));
	}
	int IndexGetDataEffect(const char* name);
	inline Effect* DataEffect(Effect* effect, int index)
	{
		effect->Value = (*(Value->effect()))[index];

		return(effect);
	}

	inline int CountDataSequence(void)
	{
		return((int)(Value->sequence()->size()));
	}
	int IndexGetDataSequence(const char* name);
	inline Sequence* DataSequence(Sequence* sequence, int index)
	{
		sequence->Value = (*(Value->sequence()))[index];

		return(sequence);
	}

	inline int CountNameTexture(void)
	{
		return((int)(Value->name_texture()->size()));
	}
	inline const char* NameGetTexture(int index)
	{
		return((*(Value->name_texture()))[index]->c_str());
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

}	/* Project */
}	/* Data */
}	/* Library */
}	/* SpriteStudio6 */
