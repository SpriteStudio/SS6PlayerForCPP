/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_pack.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Data	{
namespace Animation	{
namespace Pack	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
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
/*                                                          ArgumentContainer */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void ArgumentContainer::CleanUp(void)

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// ArgumentContainer::ArgumentContainer(void)

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// ArgumentContainer::ArgumentContainer(const void* dataAnimation, int indexAnimation, int idParts, int frame, int framePrevious)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// ArgumentContainer::~ArgumentContainer(void)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          CapacityContainer */
/* -------------------------------------------------------------------------- */
const CapacityContainer CapacityContainer::Capacity[KindTypePack::Kind::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	CapacityContainer(	false,	/* Status */
													false,	/* Cell */
													false,	/* Position */
													false,	/* Rotation */
													false,	/* Scaling */
													false,	/* ScalingLocal */
													false,	/* RateOpacity */
													false,	/* Priority */
													false,	/* PartsColor */
													false,	/* VertexCorrection */
													false,	/* OffsetPivot */
													false,	/* PositionAnchor */
													false,	/* SizeForce */
													false,	/* PositionTexture */
													false,	/* RotationTexture */
													false,	/* ScalingTexture */
													false,	/* RadiusCollision */
													false,	/* UserData (Trigger) */
													false,	/* Instance (Trigger) */
													false,	/* Effect (Trigger) */
													false	/* Deform */
												), 
	/* STANDARD_CPE */	CapacityContainer(	true,	/* Status */
											true,	/* Cell */
											true,	/* Position */
											true,	/* Rotation */
											true,	/* Scaling */
											true,	/* ScalingLocal */
											true,	/* RateOpacity */
											true,	/* Priority */
											true,	/* PartsColor */
											true,	/* VertexCorrection */
											true,	/* OffsetPivot */
											true,	/* PositionAnchor */
											true,	/* SizeForce */
											true,	/* PositionTexture */
											true,	/* RotationTexture */
											true,	/* ScalingTexture */
											true,	/* RadiusCollision */
											true,	/* UserData (Trigger) */
											true,	/* Instance (Trigger) */
											true,	/* Effect (Trigger) */
											true	/* Deform */
										),
	/* CPE_FLYWEIGHT */	CapacityContainer(	false,	/* Status */
											false,	/* Cell */
											false,	/* Position */
											false,	/* Rotation */
											false,	/* Scaling */
											false,	/* ScalingLocal */
											false,	/* RateOpacity */
											false,	/* Priority */
											false,	/* PartsColor */
											false,	/* VertexCorrection */
											false,	/* OffsetPivot */
											false,	/* PositionAnchor */
											false,	/* SizeForce */
											false,	/* PositionTexture */
											false,	/* RotationTexture */
											false,	/* ScalingTexture */
											false,	/* RadiusCollision */
											false,	/* UserData (Trigger) */
											false,	/* Instance (Trigger) */
											false,	/* Effect (Trigger) */
											false	/* Deform */
										),
	/* CPE_INTERPOLATE */	CapacityContainer(	false,	/* Status */
												false,	/* Cell */
												true,	/* Position */
												true,	/* Rotation */
												true,	/* Scaling */
												true,	/* ScalingLocal */
												true,	/* RateOpacity */
												true,	/* Priority */
												true,	/* PartsColor */
												true,	/* VertexCorrection */
												true,	/* OffsetPivot */
												true,	/* PositionAnchor */
												true,	/* SizeForce */
												true,	/* PositionTexture */
												true,	/* RotationTexture */
												true,	/* ScalingTexture */
												true,	/* RadiusCollision */
												false,	/* UserData (Trigger) */
												false,	/* Instance (Trigger) */
												false,	/* Effect (Trigger) */
												true	/* Deform */
											),
};

// CapacityContainer::CapacityContainer(	bool status,
// 											bool cell,
// 											bool position,
// 											bool rotation,
// 											bool scaling,
// 											bool scalingLocal,
// 											bool rateOpacity,
// 											bool priority,
// 											bool partsColor,
// 											bool vertexCorrection,
// 											bool offsetPivot,
// 											bool positionAnchor,
// 											bool sizeForce,
// 											bool positionTexture,
// 											bool rotationTexture,
// 											bool scalingTexture,
// 											bool radiusCollision,
// 											bool userData,
// 											bool instance,
// 											bool effect,
// 											bool deform
// 										)
// bool CapacityContainer::Status(void)
// bool CapacityContainer::Cell(void)
// bool CapacityContainer::Position(void)
// bool CapacityContainer::Rotation(void)
// bool CapacityContainer::Scaling(void)
// bool CapacityContainer::ScalingLocal(void)
// bool CapacityContainer::RateOpacity(void)
// bool CapacityContainer::Priority(void)
// bool CapacityContainer::PartsColor(void)
// bool CapacityContainer::VertexCorrection(void)
// bool CapacityContainer::OffsetPivot(void)
// bool CapacityContainer::PositionAnchor(void)
// bool CapacityContainer::SizeForce(void)
// bool CapacityContainer::PositionTexture(void)
// bool CapacityContainer::RotationTexture(void)
// bool CapacityContainer::ScalingTexture(void)
// bool CapacityContainer::RadiusCollision(void)
// bool CapacityContainer::UserData(void)
// bool CapacityContainer::Instance(void)
// bool CapacityContainer::Effect(void)
// bool CapacityContainer::Deform(void)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            AttributeExpand */
/* -------------------------------------------------------------------------- */
/* MEMO: テンプレートクラスにつき実装記述はなし。 */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                              Static: AttributeExpand [int] */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                        圧縮データ展開 [Uncompress: 非圧縮] */
/* -------------------------------------------------------------------------- */
/* MEMO: SS6Player for UnityとSS6ConverterVer2のみでの実装です。 */
/*       ※非圧縮なので、実装するメリットがないためです。        */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                  圧縮データ展開 [Standard-CPE: 変化点抽出] */
/* -------------------------------------------------------------------------- */
/* MEMO: ソース中で混乱しないようソースローカルのクラス内に分類しています。 */
class StandardCPE
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 圧縮ステータスの内包ビット */
	enum FlagBit : Uint32	{
		FRAMEKEY = 0x00007fff,
		INDEX = 0x3fff8000,

		CLEAR = 0x00000000,
	};
	enum FlagBitShift	{
		SHIFT_FRAMEKEY = 15,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:

	/* ----------------------------------------------- Functions */
private:
	static inline int IndexGetFrame(int frame, const Uint32* tableStatus, int countTableStatus)
	{
		/* フレームに対応したデータ位置を発見（二分木） */
		int frameKey = -1;
		int indexMinimum = 0;
		int indexMaximum = countTableStatus - 1;
		int index;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}
	
		return(indexMinimum);
	}

	/* 実展開部関数群 */
	/* MEMO: これらの関数をそれぞれの実際のValueGet～系関数から呼び出しています。 */
	/* MEMO: SS6Player for Unityではジェネリックで書いていましたが、C++版ではFlatBuffersが絡むので */
	/*       templateで書ききろうとすると記述が複雑になるため、展開してザラ書きしています。        */
	/*       ※変化があるのは末尾だけなのですが……今時マクロ展開とかもないだろうと……。          */
	static inline bool ValueExpandInt(	Sint32* outValue,
										int* outFrameKey,
										int frame,
										const Uint32* tableStatus,
										int countTableStatus,
										const flatbuffers::Vector<int32_t>* tableValue
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		*outValue = (Sint32)((*tableValue)[index]);

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandFloat(	Float32* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<float>* tableValue
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		*outValue = (Float32)((*tableValue)[index]);

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandVector2(	CPU::Vector2* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector2>>* tableValue
										)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->Set((*tableValue)[index]->x(), (*tableValue)[index]->y());

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandVector3(	CPU::Vector3* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector3>>* tableValue
										)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->Set((*tableValue)[index]->x(), (*tableValue)[index]->y(), (*tableValue)[index]->z());

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandStatus(	Library::Data::Animation::Attribute::Status* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<int32_t>* tableValue
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->Flags = (Uint32)((*tableValue)[index]);

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandCell(	Library::Data::Animation::Attribute::Cell* outValue,
										int* outFrameKey,
										int frame,
										const Uint32* tableStatus,
										int countTableStatus,
										const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeCell>>* tableValue
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->IndexCellMap = (int)((*tableValue)[index]->IndexCellMap());
		outValue->IndexCell = (int)((*tableValue)[index]->IndexCell());

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandPartsColor(	Library::Data::Animation::Attribute::PartsColor* outValue,
												int* outFrameKey,
												int frame,
												const Uint32* tableStatus,
												int countTableStatus,
												const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributePartsColor>>* tableValue
										)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->Bound = (Library::KindBoundBlend::Kind)((*tableValue)[index]->Bound());
		outValue->Operation = (Library::KindOperationBlend::Kind)((*tableValue)[index]->Operation());
		const ss::ssfb2::Color* data;
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			data = (*((*tableValue)[index]->VertexColor()))[i];
			outValue->VertexColor[i].Set(data->r(), data->g(), data->b(), data->a());
			outValue->RateAlpha[i] = (*((*tableValue)[index]->RateAlpha()))[i];
		}

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandVertexCorrection(	Library::Data::Animation::Attribute::VertexCorrection* outValue,
													int* outFrameKey,
													int frame,
													const Uint32* tableStatus,
													int countTableStatus,
													const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::VertexCorrection>>* tableValue
												)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			outValue->Coordinate[i].Set((*((*tableValue)[index]->Coordinate()))[i]->x(), (*((*tableValue)[index]->Coordinate()))[i]->y());
		}

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandUserData(	Library::Data::Animation::Attribute::UserData* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::UserData>>* tableValue
										)
	{
		/* MEMO: 本関数は使用されませんので、伏せておきます。具体的なデコードの実装については、ValueGetIndexUserDataを参照してください。 */
		return(false);
	}

	static inline bool ValueExpandInstance(	Library::Data::Animation::Attribute::Instance* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeInstance>>* tableValue
										)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->Flags = (Uint32)((*tableValue)[index]->Flags());
		outValue->PlayCount = (int)((*tableValue)[index]->PlayCount());
		outValue->RateTime = (Float32)((*tableValue)[index]->RateTime());
		outValue->OffsetStart = (int)((*tableValue)[index]->OffsetStart());
		outValue->OffsetEnd = (int)((*tableValue)[index]->OffsetEnd());
		outValue->LabelStart = ((*tableValue)[index]->LabelStart())->c_str();
		outValue->LabelEnd = ((*tableValue)[index]->LabelEnd())->c_str();

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandEffect(	Library::Data::Animation::Attribute::Effect* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeEffect>>* tableValue
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->Flags = (Uint32)((*tableValue)[index]->Flags());
		outValue->FrameStart = (int)((*tableValue)[index]->FrameStart());
		outValue->RateTime = (Float32)((*tableValue)[index]->RateTime());

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandDeform(	Library::Data::Animation::Attribute::Deform* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeDeform>>* tableValue,
											const ss::ssfb2::ContainerDeform* container
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		/* MEMO: デフォームは無変化情報が省略されているため、出力の値に上書きして累積しています。 */
		/*       ※つまりoutValueが時系列累積での全頂点変化情報になっています。                   */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->TableCoordinate.clear();
		int countVertexChange = ((*tableValue)[index]->TableCoordinate())->size();
		const int* tableIndexVertex = (const int*)(container->TableIndexVertex()->data());
		for(int i=0; i<countVertexChange; i++)	{
			outValue->TableCoordinate[tableIndexVertex[i]].Set((*((*tableValue)[index]->TableCoordinate()))[i]->x(), (*((*tableValue)[index]->TableCoordinate()))[i]->y());
		}

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandShader(	Library::Data::Animation::Attribute::Shader* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeShader>>* tableValue
									)
	{
		/* フレームに対応したデータ位置を発見 */
		int index = IndexGetFrame(frame, tableStatus, countTableStatus);

		/* ステータスからキーフレームと辞書インデックスを取得 */
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		if(*outFrameKey == frameKey)	{
			return(false);	/* outValueは上書きされていない */
		}
		*outFrameKey = frameKey;

		/* 辞書から値を返す */
		index = (int)((status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY);
		outValue->ID = ((*tableValue)[index]->ID())->c_str();

		const flatbuffers::Vector<float>* listParameter = (*tableValue)[index]->Parameter();
		for(int i=0; i<Library::Data::Animation::Attribute::Shader::Constant::COUNT_PARAMETER; i++)	{
			outValue->Parameter[i] = (*listParameter)[i];
		}

		return(true);	/* outValueは新規値 */
	}

	static inline bool ValueExpandSignal(	Library::Data::Animation::Attribute::Signal* outValue,
											int* outFrameKey,
											int frame,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeSignal>>* tableValue
									)
	{
		/* MEMO: 本関数は使用されませんので、伏せておきます。具体的なデコードの実装については、ValueGetIndexSignalを参照してください。 */
		return(true);	/* outValueは新規値 */
	}

protected:
public:
	/* ValueGet系関数 */
	/* MEMO: 本関数をAttributeExpand派生クラスのValueGet関数に設定します。 */
	/* MEMO: 返値がfalseの場合に、outFrameKeyはいじらないこと（outFrameKeyは最後にキーをデコードしたフレームのワークなので、更新時以外に操作しない）。 */
	static bool ValueGetInt(Sint32* outValue, int* outFrameKey, const ss::ssfb2::ContainerInt* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandInt(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetFloat(Float32* outValue, int* outFrameKey, const ss::ssfb2::ContainerFloat* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandFloat(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetVector2(CPU::Vector2* outValue, int* outFrameKey, const ss::ssfb2::ContainerVector2* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandVector2(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetVector3(CPU::Vector3* outValue, int* outFrameKey, const ss::ssfb2::ContainerVector3* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandVector3(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetStatus(Library::Data::Animation::Attribute::Status* outValue, int* outFrameKey, const ss::ssfb2::ContainerStatus* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandStatus(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetCell(Library::Data::Animation::Attribute::Cell* outValue, int* outFrameKey, const ss::ssfb2::ContainerCell* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandCell(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetPartsColor(Library::Data::Animation::Attribute::PartsColor* outValue, int* outFrameKey, const ss::ssfb2::ContainerPartsColor* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandPartsColor(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetVertexCorrection(Library::Data::Animation::Attribute::VertexCorrection* outValue, int* outFrameKey, const ss::ssfb2::ContainerVertexCorrection* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandVertexCorrection(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetUserData(Library::Data::Animation::Attribute::UserData* outValue, int* outFrameKey, const ss::ssfb2::ContainerUserData* container, const ArgumentContainer& argument)
	{
		/* MEMO: ユーザデータは圧縮展開をしないので、本関数は実質使いません（インデックスで取得してくる）。 */
		/* MEMO: 実はユーザデータは圧縮がかかっていません（トリガデータなので圧縮がかかると困るため）。 */
		/*       SSAEのキーフレーム状態がそのまま（データ型式だけ変換されて）入っています。             */

		/* MEMO: 間違って展開エンコードしないようにfalseを返しておきます。 */
		return(false);
	}
	static bool ValueGetInstance(Library::Data::Animation::Attribute::Instance* outValue, int* outFrameKey, const ss::ssfb2::ContainerInstance* container, const ArgumentContainer& argument)
	{
		/* MEMO: 実はインスタンスデータは圧縮がかかっていません（トリガデータなので圧縮がかかると困るため）。 */
		/*       SSAEのキーフレーム状態がそのまま（データ型式だけ変換されて）入っています。             */
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandInstance(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetEffect(Library::Data::Animation::Attribute::Effect* outValue, int* outFrameKey, const ss::ssfb2::ContainerEffect* container, const ArgumentContainer& argument)
	{
		/* MEMO: 実はエフェクトデータは圧縮がかかっていません（トリガデータなので圧縮がかかると困るため）。 */
		/*       SSAEのキーフレーム状態がそのまま（データ型式だけ変換されて）入っています。             */
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandEffect(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetDeform(Library::Data::Animation::Attribute::Deform* outValue, int* outFrameKey, const ss::ssfb2::ContainerDeform* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandDeform(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue(), container));
	}
	static bool ValueGetShader(Library::Data::Animation::Attribute::Shader* outValue, int* outFrameKey, const ss::ssfb2::ContainerShader* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		return(ValueExpandShader(outValue, outFrameKey, frame, (Uint32*)(tableCode->data()), countCode, container->TableValue()));
	}
	static bool ValueGetSignal(Library::Data::Animation::Attribute::Signal* outValue, int* outFrameKey, const ss::ssfb2::ContainerSignal* container, const ArgumentContainer& argument)
	{
		/* MEMO: シグナルは圧縮展開をしないので、本関数は実質使いません（インデックスで取得してくる）。 */
		/* MEMO: 実はシグナルデータは圧縮がかかっていません（トリガデータなので圧縮がかかると困るため）。 */
		/*       SSAEのキーフレーム状態がそのまま（データ型式だけ変換されて）入っています。               */
		/*       ※再生時の扱いはデータ型式が異なるだけのユーザデータと考えれば、ほぼ正解です。           */

		/* MEMO: 間違って展開エンコードしないようにfalseを返しておきます。 */
		return(false);
	}

	/* ValueGetIndex系関数 */
	/* MEMO: 本関数をAttributeExpand派生クラスのValueGetIndex関数に設定します。 */
	/* MEMO: 返値がfalseの場合に、outFrameKeyはいじらないこと（outFrameKeyは最後にキーをデコードしたフレームのワークなので、更新時以外に操作しない）。 */
	/* MEMO: 基本的にUserData・Signalしか対応していません。            */
	/*       SS6Player for Unityでは対応しているものもありはするものの */
	/*       間違って値を取り出すと困るので、C++版では一旦非対応にして */
	/*       あります。                                                */
	static bool ValueGetIndexInt(Sint32* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerInt* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexFloat(Float32* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerFloat* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexVector2(CPU::Vector2* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerVector2* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexVector3(CPU::Vector3* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerVector3* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexStatus(Library::Data::Animation::Attribute::Status* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerStatus* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexCell(Library::Data::Animation::Attribute::Cell* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerCell* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexPartsColor(Library::Data::Animation::Attribute::PartsColor* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerPartsColor* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexVertexCorrection(Library::Data::Animation::Attribute::VertexCorrection* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerVertexCorrection* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexUserData(Library::Data::Animation::Attribute::UserData* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerUserData* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		if((0 > index) || ((int)(((*(container->TableCodeValue()))[0]->TableCode())->size()) <= index))	{
			return(false);
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		Uint32* tableStatus = (Uint32*)(tableCode->data());
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		int indexValue = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY;

		/* 辞書から値を返す */
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::UserData>>* tableValue = container->TableValue();
		const ss::ssfb2::UserData* dataValue = (*tableValue)[indexValue];
		outValue->Flags = (Uint32)(dataValue->flags());
		outValue->NumberInt = (Sint32)(dataValue->number_int());
		outValue->Rectangle.SetLURD(	(dataValue->rectangle())->x_min(),
										(dataValue->rectangle())->y_min(),
										(dataValue->rectangle())->x_max(),
										(dataValue->rectangle())->y_max()
								);
		outValue->Coordinate.Set((dataValue->coordinate())->x(), (dataValue->coordinate())->y());
		outValue->Text = (dataValue->text())->c_str();
		*outFrameKey = frameKey;

		return(true);	/* outValueは新規値 */
	}
	static bool ValueGetIndexInstance(Library::Data::Animation::Attribute::Instance* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerInstance* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexEffect(Library::Data::Animation::Attribute::Effect* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerEffect* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexDeform(Library::Data::Animation::Attribute::Deform* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerDeform* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexShader(Library::Data::Animation::Attribute::Shader* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerShader* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexSignal(Library::Data::Animation::Attribute::Signal* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerSignal* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		if((0 > index) || ((int)(((*(container->TableCodeValue()))[0]->TableCode())->size()) <= index))	{
			return(false);
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		Uint32* tableStatus = (Uint32*)(tableCode->data());
		Uint32 status = tableStatus[index];
		int frameKey = status & FlagBit::FRAMEKEY;
		int indexValue = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_FRAMEKEY;

		/* 辞書から値を返す */
		/* MEMO: シグナルはデータが大きいので、FlatBuffersに直接アクセスして値を取得するようにする。 */
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeSignal>>* tableValue = container->TableValue();
		outValue->Value = (*tableValue)[indexValue];
		*outFrameKey = frameKey;

		return(true);	/* outValueは新規値 */
	}

	/* CountGetValue系関数 */
	/* MEMO: 本関数をAttributeExpand派生クラスのCountGetValue関数に設定します。 */
	/* MEMO: 基本的にUserData・Signalしか対応していません。 */
	static int CountGetValueInt(const ss::ssfb2::ContainerInt* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueFloat(const ss::ssfb2::ContainerFloat* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueVector2(const ss::ssfb2::ContainerVector2* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueVector3(const ss::ssfb2::ContainerVector3* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueStatus(const ss::ssfb2::ContainerStatus* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueCell(const ss::ssfb2::ContainerCell* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValuePartsColor(const ss::ssfb2::ContainerPartsColor* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueVertexCorrection(const ss::ssfb2::ContainerVertexCorrection* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueUserData(const ss::ssfb2::ContainerUserData* container)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(-1);
		}
		if(0 >= tableCodeValue->size())	{
			return(-1);
		}

		return((int)((((*(container->TableCodeValue()))[0]->TableCode())->size())));
	}
	static int CountGetValueInstance(const ss::ssfb2::ContainerInstance* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueEffect(const ss::ssfb2::ContainerEffect* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueDeform(const ss::ssfb2::ContainerDeform* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueShader(const ss::ssfb2::ContainerShader* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueSignal(const ss::ssfb2::ContainerSignal* container)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(-1);
		}
		if(0 >= tableCodeValue->size())	{
			return(-1);
		}

		return((int)((((*(container->TableCodeValue()))[0]->TableCode())->size())));
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*         圧縮データ展開 [CPE_Flyweight: CPE+GoF Flyweight（CPE辞書大域化）] */
/* -------------------------------------------------------------------------- */
/* MEMO: SS6Player for Unityのみでの実装です。                             */
/*       ※大域辞書あふれが頻発して汎用がきついため、C++版では非実装です。 */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                     圧縮データ展開 [CPE_Interpolate: CPE+リアルタイム補間] */
/* -------------------------------------------------------------------------- */
/* MEMO: ソース中で混乱しないようソースローカルのクラス内に分類しています。 */
class CPE_Interpolate
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 圧縮ステータスの内包ビット */
	enum FlagBit : Uint32	{
		FRAMEKEY = 0x00007fff,
		INDEX = 0x07ff8000,
		FORMULA = 0x38000000,

		CLEAR = 0x00000000,
	};
	enum FlagBitShift	{
		SHIFT_FRAMEKEY = 0,
		SHIFT_INDEX = 15,
		SHIFT_FORMULA = 27,
	};
	enum KindFormula	{
		FORMULA_LINEAR = 0,
		FORMULA_ACCELERATE,
		FORMULA_DECELERATE,

		FORMULA__RESERVED_04,
		FORMULA__RESERVED_03,
		FORMULA__RESERVED_02,
		FORMULA__RESERVED_01,

		FORMULA_CPE = 7,	/* -1 (Raw) */
	};
	enum KindElementVector	{
		ELEMENT_X = 0,
		ELEMENT_Y,
		ELEMENT_Z,
		ELEMENT_W
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:

	/* ----------------------------------------------- Functions */
private:
	/* 実展開部関数群 */
	/* MEMO: これらの関数をそれぞれの実際のValueGet～系関数から呼び出しています。 */
	/* MEMO: SS6Player for Unityではジェネリックで書いていましたが、C++版ではFlatBuffersが絡むので */
	/*       templateで書ききろうとすると記述が複雑になるため、展開してザラ書きしています。        */
	/*       ※変化があるのは末尾だけなのですが……今時マクロ展開とかもないだろうと……。          */
	/* MEMO: フレームから配列インデックスを求めるロジックは定数以外StandardCPEと同じなのですが、 */
	/*       その後の処理で中途で使用した値などを使用していたりする関係から、共通化などをしてい  */
	/*       ません（後に構造化するかもしれませんが、今はベタ書きです）。                        */
	static inline bool IntExpandInt(	Sint32* outValue,
										int frame,
										int framePrevious,
										const Uint32* tableStatus,
										int countTableStatus,
										const flatbuffers::Vector<int32_t>* tableValue
									)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		float value;
		float valueEnd;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				value = (float)((*tableValue)[index]);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = (float)((*tableValue)[indexEnd]);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				*outValue = (Sint32)(((valueEnd - value) * rate) + value);

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				value = (float)((*tableValue)[index]);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = (float)((*tableValue)[indexEnd]);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				*outValue = (Sint32)(((valueEnd - value) * rate) + value);

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				value = (float)((*tableValue)[index]);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift.FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = (float)((*tableValue)[indexEnd]);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				*outValue = (Sint32)(((valueEnd - value) * rate) + value);

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				*outValue = (Sint32)((*tableValue)[index]);

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline bool FloatExpandFloat(	Float32* outValue,
											int frame,
											int framePrevious,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<float>* tableValue
									)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		float value;
		float valueEnd;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				value = (*tableValue)[index];

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = (*tableValue)[indexEnd];

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				*outValue = (Float32)(((valueEnd - value) * rate) + value);

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				value = (*tableValue)[index];

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = (*tableValue)[indexEnd];

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				*outValue = (Float32)(((valueEnd - value) * rate) + value);

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				value = (*tableValue)[index];

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift.SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = (*tableValue)[indexEnd];

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				*outValue = (Float32)(((valueEnd - value) * rate) + value);

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				*outValue = (*tableValue)[index];

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline float FloatExtractVector2(const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector2>>* tableValue, int index)
	{
		switch(index % 2)	{
			case 0:
				return((*tableValue)[(index / 2)]->x());
			case 1:
				return((*tableValue)[(index / 2)]->y());
		}

		/* MEMO: ここにはきません。 */
		return(0.0f);
	}
	static inline bool FloatExpandVector2(	Float32* outValue,
											int frame,
											int framePrevious,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector2>>* tableValue
										)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		float value;
		float valueEnd;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				value = FloatExtractVector2(tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = FloatExtractVector2(tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				*outValue = ((valueEnd - value) * rate) + value;

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				value = FloatExtractVector2(tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = FloatExtractVector2(tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				*outValue = ((valueEnd - value) * rate) + value;

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				value = FloatExtractVector2(tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = FloatExtractVector2(tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				*outValue = ((valueEnd - value) * rate) + value;

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				/* MEMO: Always outputs a value since Vector2 also is tupple. */
				*outValue = FloatExtractVector2(tableValue, index);

				if(framePrevious == frameKey)
				{
					return(false);	/* outValueは上書きされていない */
				}
				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline float FloatExtractVector3(const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector3>>* tableValue, int index)
	{
		switch(index % 3)	{
			case 0:
				return((*tableValue)[(index / 3)]->x());
			case 1:
				return((*tableValue)[(index / 3)]->y());
			case 2:
				return((*tableValue)[(index / 3)]->z());
		}

		/* MEMO: ここにはきません。 */
		return(0.0f);
	}
	static inline bool FloatExpandVector3(	Float32* outValue,
											int frame,
											int framePrevious,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector3>>* tableValue
										)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		float value;
		float valueEnd;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				value = FloatExtractVector3(tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = FloatExtractVector3(tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				*outValue = ((valueEnd - value) * rate) + value;

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				value = FloatExtractVector3(tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = FloatExtractVector3(tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				*outValue = ((valueEnd - value) * rate) + value;

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				value = FloatExtractVector3(tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				valueEnd = FloatExtractVector3(tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				*outValue = ((valueEnd - value) * rate) + value;

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				/* MEMO: Vector2と同様に必ず書き込んでおかなくて良いのか？？（ひょっとするとSS6PUのバグ？） */
				*outValue = FloatExtractVector3(tableValue, index);

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline void ValueExtractPartsColor(	Library::Data::Animation::Attribute::PartsColor* outValue,
												const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributePartsColor>>* tableValue,
												int index
											)
	{
		outValue->Bound = (Library::KindBoundBlend::Kind)((*tableValue)[index]->Bound());
		outValue->Operation = (Library::KindOperationBlend::Kind)((*tableValue)[index]->Operation());
		const ss::ssfb2::Color* data;
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			data = (*((*tableValue)[index]->VertexColor()))[i];
			outValue->VertexColor[i].Set(data->r(), data->g(), data->b(), data->a());
			outValue->RateAlpha[i] = (*((*tableValue)[index]->RateAlpha()))[i];
		}
	}
	static inline bool ValueExpandPartsColor(	Library::Data::Animation::Attribute::PartsColor* outValue,
												int frame,
												int framePrevious,
												const Uint32* tableStatus,
												int countTableStatus,
												const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributePartsColor>>* tableValue
											)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		Library::Data::Animation::Attribute::PartsColor value;
		Library::Data::Animation::Attribute::PartsColor valueEnd;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				ValueExtractPartsColor(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractPartsColor(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				outValue->Operation = value.Operation;
				outValue->Bound = value.Bound;

				outValue->VertexColor[Library::KindVertex::LU].Set(	
					((valueEnd.VertexColor[Library::KindVertex::LU].GetR() - value.VertexColor[Library::KindVertex::LU].GetR()) * rate) + value.VertexColor[Library::KindVertex::LU].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetG() - value.VertexColor[Library::KindVertex::LU].GetG()) * rate) + value.VertexColor[Library::KindVertex::LU].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetB() - value.VertexColor[Library::KindVertex::LU].GetB()) * rate) + value.VertexColor[Library::KindVertex::LU].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetA() - value.VertexColor[Library::KindVertex::LU].GetA()) * rate) + value.VertexColor[Library::KindVertex::LU].GetA()
				);
				outValue->VertexColor[Library::KindVertex::RU].Set(	
					((valueEnd.VertexColor[Library::KindVertex::RU].GetR() - value.VertexColor[Library::KindVertex::RU].GetR()) * rate) + value.VertexColor[Library::KindVertex::RU].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetG() - value.VertexColor[Library::KindVertex::RU].GetG()) * rate) + value.VertexColor[Library::KindVertex::RU].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetB() - value.VertexColor[Library::KindVertex::RU].GetB()) * rate) + value.VertexColor[Library::KindVertex::RU].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetA() - value.VertexColor[Library::KindVertex::RU].GetA()) * rate) + value.VertexColor[Library::KindVertex::RU].GetA()
				);
				outValue->VertexColor[Library::KindVertex::RD].Set(	
					((valueEnd.VertexColor[Library::KindVertex::RD].GetR() - value.VertexColor[Library::KindVertex::RD].GetR()) * rate) + value.VertexColor[Library::KindVertex::RD].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetG() - value.VertexColor[Library::KindVertex::RD].GetG()) * rate) + value.VertexColor[Library::KindVertex::RD].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetB() - value.VertexColor[Library::KindVertex::RD].GetB()) * rate) + value.VertexColor[Library::KindVertex::RD].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetA() - value.VertexColor[Library::KindVertex::RD].GetA()) * rate) + value.VertexColor[Library::KindVertex::RD].GetA()
				);
				outValue->VertexColor[Library::KindVertex::LD].Set(	
					((valueEnd.VertexColor[Library::KindVertex::LD].GetR() - value.VertexColor[Library::KindVertex::LD].GetR()) * rate) + value.VertexColor[Library::KindVertex::LD].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetG() - value.VertexColor[Library::KindVertex::LD].GetG()) * rate) + value.VertexColor[Library::KindVertex::LD].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetB() - value.VertexColor[Library::KindVertex::LD].GetB()) * rate) + value.VertexColor[Library::KindVertex::LD].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetA() - value.VertexColor[Library::KindVertex::LD].GetA()) * rate) + value.VertexColor[Library::KindVertex::LD].GetA()
				);

				outValue->RateAlpha[Library::KindVertex::LU] = ((valueEnd.RateAlpha[Library::KindVertex::LU] - value.RateAlpha[Library::KindVertex::LU]) * rate) + value.RateAlpha[Library::KindVertex::LU];
				outValue->RateAlpha[Library::KindVertex::RU] = ((valueEnd.RateAlpha[Library::KindVertex::RU] - value.RateAlpha[Library::KindVertex::RU]) * rate) + value.RateAlpha[Library::KindVertex::RU];
				outValue->RateAlpha[Library::KindVertex::RD] = ((valueEnd.RateAlpha[Library::KindVertex::RD] - value.RateAlpha[Library::KindVertex::RD]) * rate) + value.RateAlpha[Library::KindVertex::RD];
				outValue->RateAlpha[Library::KindVertex::LD] = ((valueEnd.RateAlpha[Library::KindVertex::LD] - value.RateAlpha[Library::KindVertex::LD]) * rate) + value.RateAlpha[Library::KindVertex::LD];

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				ValueExtractPartsColor(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractPartsColor(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				outValue->Operation = value.Operation;
				outValue->Bound = value.Bound;

				outValue->VertexColor[Library::KindVertex::LU].Set(	
					((valueEnd.VertexColor[Library::KindVertex::LU].GetR() - value.VertexColor[Library::KindVertex::LU].GetR()) * rate) + value.VertexColor[Library::KindVertex::LU].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetG() - value.VertexColor[Library::KindVertex::LU].GetG()) * rate) + value.VertexColor[Library::KindVertex::LU].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetB() - value.VertexColor[Library::KindVertex::LU].GetB()) * rate) + value.VertexColor[Library::KindVertex::LU].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetA() - value.VertexColor[Library::KindVertex::LU].GetA()) * rate) + value.VertexColor[Library::KindVertex::LU].GetA()
				);
				outValue->VertexColor[Library::KindVertex::RU].Set(	
					((valueEnd.VertexColor[Library::KindVertex::RU].GetR() - value.VertexColor[Library::KindVertex::RU].GetR()) * rate) + value.VertexColor[Library::KindVertex::RU].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetG() - value.VertexColor[Library::KindVertex::RU].GetG()) * rate) + value.VertexColor[Library::KindVertex::RU].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetB() - value.VertexColor[Library::KindVertex::RU].GetB()) * rate) + value.VertexColor[Library::KindVertex::RU].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetA() - value.VertexColor[Library::KindVertex::RU].GetA()) * rate) + value.VertexColor[Library::KindVertex::RU].GetA()
				);
				outValue->VertexColor[Library::KindVertex::RD].Set(	
					((valueEnd.VertexColor[Library::KindVertex::RD].GetR() - value.VertexColor[Library::KindVertex::RD].GetR()) * rate) + value.VertexColor[Library::KindVertex::RD].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetG() - value.VertexColor[Library::KindVertex::RD].GetG()) * rate) + value.VertexColor[Library::KindVertex::RD].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetB() - value.VertexColor[Library::KindVertex::RD].GetB()) * rate) + value.VertexColor[Library::KindVertex::RD].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetA() - value.VertexColor[Library::KindVertex::RD].GetA()) * rate) + value.VertexColor[Library::KindVertex::RD].GetA()
				);
				outValue->VertexColor[Library::KindVertex::LD].Set(	
					((valueEnd.VertexColor[Library::KindVertex::LD].GetR() - value.VertexColor[Library::KindVertex::LD].GetR()) * rate) + value.VertexColor[Library::KindVertex::LD].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetG() - value.VertexColor[Library::KindVertex::LD].GetG()) * rate) + value.VertexColor[Library::KindVertex::LD].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetB() - value.VertexColor[Library::KindVertex::LD].GetB()) * rate) + value.VertexColor[Library::KindVertex::LD].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetA() - value.VertexColor[Library::KindVertex::LD].GetA()) * rate) + value.VertexColor[Library::KindVertex::LD].GetA()
				);

				outValue->RateAlpha[Library::KindVertex::LU] = ((valueEnd.RateAlpha[Library::KindVertex::LU] - value.RateAlpha[Library::KindVertex::LU]) * rate) + value.RateAlpha[Library::KindVertex::LU];
				outValue->RateAlpha[Library::KindVertex::RU] = ((valueEnd.RateAlpha[Library::KindVertex::RU] - value.RateAlpha[Library::KindVertex::RU]) * rate) + value.RateAlpha[Library::KindVertex::RU];
				outValue->RateAlpha[Library::KindVertex::RD] = ((valueEnd.RateAlpha[Library::KindVertex::RD] - value.RateAlpha[Library::KindVertex::RD]) * rate) + value.RateAlpha[Library::KindVertex::RD];
				outValue->RateAlpha[Library::KindVertex::LD] = ((valueEnd.RateAlpha[Library::KindVertex::LD] - value.RateAlpha[Library::KindVertex::LD]) * rate) + value.RateAlpha[Library::KindVertex::LD];

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				ValueExtractPartsColor(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractPartsColor(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				outValue->Operation = value.Operation;
				outValue->Bound = value.Bound;

				outValue->VertexColor[Library::KindVertex::LU].Set(	
					((valueEnd.VertexColor[Library::KindVertex::LU].GetR() - value.VertexColor[Library::KindVertex::LU].GetR()) * rate) + value.VertexColor[Library::KindVertex::LU].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetG() - value.VertexColor[Library::KindVertex::LU].GetG()) * rate) + value.VertexColor[Library::KindVertex::LU].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetB() - value.VertexColor[Library::KindVertex::LU].GetB()) * rate) + value.VertexColor[Library::KindVertex::LU].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::LU].GetA() - value.VertexColor[Library::KindVertex::LU].GetA()) * rate) + value.VertexColor[Library::KindVertex::LU].GetA()
				);
				outValue->VertexColor[Library::KindVertex::RU].Set(	
					((valueEnd.VertexColor[Library::KindVertex::RU].GetR() - value.VertexColor[Library::KindVertex::RU].GetR()) * rate) + value.VertexColor[Library::KindVertex::RU].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetG() - value.VertexColor[Library::KindVertex::RU].GetG()) * rate) + value.VertexColor[Library::KindVertex::RU].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetB() - value.VertexColor[Library::KindVertex::RU].GetB()) * rate) + value.VertexColor[Library::KindVertex::RU].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::RU].GetA() - value.VertexColor[Library::KindVertex::RU].GetA()) * rate) + value.VertexColor[Library::KindVertex::RU].GetA()
				);
				outValue->VertexColor[Library::KindVertex::RD].Set(	
					((valueEnd.VertexColor[Library::KindVertex::RD].GetR() - value.VertexColor[Library::KindVertex::RD].GetR()) * rate) + value.VertexColor[Library::KindVertex::RD].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetG() - value.VertexColor[Library::KindVertex::RD].GetG()) * rate) + value.VertexColor[Library::KindVertex::RD].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetB() - value.VertexColor[Library::KindVertex::RD].GetB()) * rate) + value.VertexColor[Library::KindVertex::RD].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::RD].GetA() - value.VertexColor[Library::KindVertex::RD].GetA()) * rate) + value.VertexColor[Library::KindVertex::RD].GetA()
				);
				outValue->VertexColor[Library::KindVertex::LD].Set(	
					((valueEnd.VertexColor[Library::KindVertex::LD].GetR() - value.VertexColor[Library::KindVertex::LD].GetR()) * rate) + value.VertexColor[Library::KindVertex::LD].GetR(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetG() - value.VertexColor[Library::KindVertex::LD].GetG()) * rate) + value.VertexColor[Library::KindVertex::LD].GetG(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetB() - value.VertexColor[Library::KindVertex::LD].GetB()) * rate) + value.VertexColor[Library::KindVertex::LD].GetB(),
					((valueEnd.VertexColor[Library::KindVertex::LD].GetA() - value.VertexColor[Library::KindVertex::LD].GetA()) * rate) + value.VertexColor[Library::KindVertex::LD].GetA()
				);

				outValue->RateAlpha[Library::KindVertex::LU] = ((valueEnd.RateAlpha[Library::KindVertex::LU] - value.RateAlpha[Library::KindVertex::LU]) * rate) + value.RateAlpha[Library::KindVertex::LU];
				outValue->RateAlpha[Library::KindVertex::RU] = ((valueEnd.RateAlpha[Library::KindVertex::RU] - value.RateAlpha[Library::KindVertex::RU]) * rate) + value.RateAlpha[Library::KindVertex::RU];
				outValue->RateAlpha[Library::KindVertex::RD] = ((valueEnd.RateAlpha[Library::KindVertex::RD] - value.RateAlpha[Library::KindVertex::RD]) * rate) + value.RateAlpha[Library::KindVertex::RD];
				outValue->RateAlpha[Library::KindVertex::LD] = ((valueEnd.RateAlpha[Library::KindVertex::LD] - value.RateAlpha[Library::KindVertex::LD]) * rate) + value.RateAlpha[Library::KindVertex::LD];

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				ValueExtractPartsColor(outValue, tableValue, index);

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline void ValueExtractVertexCorrection(	Library::Data::Animation::Attribute::VertexCorrection* outValue,
														const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::VertexCorrection>>* tableValue,
														int index
													)
	{
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			outValue->Coordinate[i].Set((*((*tableValue)[index]->Coordinate()))[i]->x(), (*((*tableValue)[index]->Coordinate()))[i]->y());
		}
	}
	static inline bool ValueExpandVertexCorrection(	Library::Data::Animation::Attribute::VertexCorrection* outValue,
													int frame,
													int framePrevious,
													const Uint32* tableStatus,
													int countTableStatus,
													const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::VertexCorrection>>* tableValue
												)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		Library::Data::Animation::Attribute::VertexCorrection value;
		Library::Data::Animation::Attribute::VertexCorrection valueEnd;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				ValueExtractVertexCorrection(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractVertexCorrection(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
					outValue->Coordinate[i].Set(
						((valueEnd.Coordinate[i].GetX() - value.Coordinate[i].GetX()) * rate) + value.Coordinate[i].GetX(),
						((valueEnd.Coordinate[i].GetY() - value.Coordinate[i].GetY()) * rate) + value.Coordinate[i].GetY()
						);
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				ValueExtractVertexCorrection(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractVertexCorrection(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
					outValue->Coordinate[i].Set(
						((valueEnd.Coordinate[i].GetX() - value.Coordinate[i].GetX()) * rate) + value.Coordinate[i].GetX(),
						((valueEnd.Coordinate[i].GetY() - value.Coordinate[i].GetY()) * rate) + value.Coordinate[i].GetY()
						);
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				ValueExtractVertexCorrection(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractVertexCorrection(&valueEnd, tableValue, indexEnd);

				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
					outValue->Coordinate[i].Set(
						((valueEnd.Coordinate[i].GetX() - value.Coordinate[i].GetX()) * rate) + value.Coordinate[i].GetX(),
						((valueEnd.Coordinate[i].GetY() - value.Coordinate[i].GetY()) * rate) + value.Coordinate[i].GetY()
						);
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				/* MEMO: Even if has reference to array, since caller manages buffer, no problem to shallow copy. */
				ValueExtractVertexCorrection(outValue, tableValue, index);

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline bool ValueExpandDeform(	Library::Data::Animation::Attribute::Deform* outValue,
											int frame,
											int framePrevious,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeDeform>>* tableValue,
											const ss::ssfb2::ContainerDeform* container
										)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		/* MEMO: デフォームの場合は格納情報が差分なこともあり、Attribute::Deformに変換しているとコストも */
		/*       バカにならないので、FlatBuffersから直接値を取り出して計算してしまっています。           */
		const int countVertexChange = (int)(container->TableIndexVertex()->size());
		const int* tableIndexVertex = container->TableIndexVertex()->data();
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector2>>* tableCoordinateStart;
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector2>>* tableCoordinateEnd;
		CPU::Vector2* outCoordinate;
		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				tableCoordinateStart = (*tableValue)[index]->TableCoordinate();

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				tableCoordinateEnd = (*tableValue)[indexEnd]->TableCoordinate();

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				for(int i=0; i<countVertexChange; i++)	{
					outCoordinate = &outValue->TableCoordinate[tableIndexVertex[i]];
					outCoordinate->Set(
						(((*tableCoordinateEnd)[i]->x() - (*tableCoordinateStart)[i]->x()) * rate) + (*tableCoordinateStart)[i]->x(),
						(((*tableCoordinateEnd)[i]->y() - (*tableCoordinateStart)[i]->y()) * rate) + (*tableCoordinateStart)[i]->y()
						);
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				tableCoordinateStart = (*tableValue)[index]->TableCoordinate();

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				tableCoordinateEnd = (*tableValue)[indexEnd]->TableCoordinate();

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				for(int i=0; i<countVertexChange; i++)	{
					outCoordinate = &outValue->TableCoordinate[tableIndexVertex[i]];
					outCoordinate->Set(
						(((*tableCoordinateEnd)[i]->x() - (*tableCoordinateStart)[i]->x()) * rate) + (*tableCoordinateStart)[i]->x(),
						(((*tableCoordinateEnd)[i]->y() - (*tableCoordinateStart)[i]->y()) * rate) + (*tableCoordinateStart)[i]->y()
						);
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				tableCoordinateStart = (*tableValue)[index]->TableCoordinate();

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				tableCoordinateEnd = (*tableValue)[indexEnd]->TableCoordinate();

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				for(int i=0; i<countVertexChange; i++)	{
					outCoordinate = &outValue->TableCoordinate[tableIndexVertex[i]];
					outCoordinate->Set(
						(((*tableCoordinateEnd)[i]->x() - (*tableCoordinateStart)[i]->x()) * rate) + (*tableCoordinateStart)[i]->x(),
						(((*tableCoordinateEnd)[i]->y() - (*tableCoordinateStart)[i]->y()) * rate) + (*tableCoordinateStart)[i]->y()
						);
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				tableCoordinateStart = (*tableValue)[index]->TableCoordinate();
				for(int i=0; i<countVertexChange; i++)	{
					outCoordinate = &outValue->TableCoordinate[tableIndexVertex[i]];
					outCoordinate->Set(
						(*tableCoordinateStart)[i]->x(),
						(*tableCoordinateStart)[i]->y()
						);
				}

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

	static inline void ValueExtractShader(	Library::Data::Animation::Attribute::Shader* outValue,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeShader>>* tableValue,
											int index
										)
	{
		outValue->ID = ((*tableValue)[index]->ID())->c_str();
		const flatbuffers::Vector<float>* listParameter = (*tableValue)[index]->Parameter();
		for(int i=0; i<Library::Data::Animation::Attribute::Shader::Constant::COUNT_PARAMETER; i++)	{
			outValue->Parameter[i] = (*listParameter)[i];
		}
	}
	static inline bool ValueExpandShader(	Library::Data::Animation::Attribute::Shader* outValue,
											int frame,
											int framePrevious,
											const Uint32* tableStatus,
											int countTableStatus,
											const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::AnimationAttributeShader>>* tableValue
										)
	{
		if(0 >= countTableStatus)	{
			return(false);
		}

		/* フレームに対応したデータ位置を発見 */
		KindFormula formula;
		int frameKey = -1;
		int frameKeyEnd;
		int indexStatusLast = countTableStatus - 1;
		int status;
		int statusEnd;
		int indexMinimum = 0;
		int indexMaximum = indexStatusLast;
		int index;
		int indexEnd;
		float rate;
		Library::Data::Animation::Attribute::Shader value;
		Library::Data::Animation::Attribute::Shader valueEnd;
		Float32 parameterEffectStart;
		while(indexMinimum != indexMaximum)	{
			index = indexMinimum + indexMaximum;
			index = (index >> 1) + (index & 1);	/* (index / 2) + (index % 2) */
			frameKey = tableStatus[index] & FlagBit::FRAMEKEY;
			if(frame == frameKey)	{
				indexMinimum = indexMaximum = index;
			} else {
				if((frame < frameKey) || (-1 == frameKey))	{
					indexMaximum = index - 1;
				} else {
					indexMinimum = index;
				}
			}
		}

		/* ステータスからキーフレームと辞書インデックスを取得 */
		status = tableStatus[indexMinimum];
		frameKey = status & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
		index = (status & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
		if(indexStatusLast <= indexMinimum)	{	/* 最終キーなら補間しない */
			formula = KindFormula::FORMULA_CPE;
		} else {
			formula = (KindFormula)((status & FlagBit::FORMULA) >> FlagBitShift::SHIFT_FORMULA);
		}

		switch(formula)	{
			case KindFormula::FORMULA_LINEAR:
				ValueExtractShader(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractShader(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				outValue->ID = value.ID;
				for(int i=0; i<Library::Data::Animation::Attribute::Shader::Constant::COUNT_PARAMETER; i++)	{
					parameterEffectStart = value.Parameter[i];
					outValue->Parameter[i] = ((valueEnd.Parameter[i] - parameterEffectStart) * rate) + parameterEffectStart;
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_ACCELERATE:
				ValueExtractShader(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> (int)FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractShader(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate *= rate;
				outValue->ID = value.ID;
				for(int i=0; i<Library::Data::Animation::Attribute::Shader::Constant::COUNT_PARAMETER; i++)	{
					parameterEffectStart = value.Parameter[i];
					outValue->Parameter[i] = ((valueEnd.Parameter[i] - parameterEffectStart) * rate) + parameterEffectStart;
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA_DECELERATE:
				ValueExtractShader(&value, tableValue, index);

				indexMinimum++;
				statusEnd = tableStatus[indexMinimum];
				frameKeyEnd = statusEnd & FlagBit::FRAMEKEY;	/* >> FlagBitShift::SHIFT_FRAMEKEY; */
				indexEnd = (statusEnd & FlagBit::INDEX) >> FlagBitShift::SHIFT_INDEX;
				ValueExtractShader(&valueEnd, tableValue, indexEnd);

				/* MEMO: 展開関数を呼ぶとコストがかかるので、コードを展開しています。 */
				/* MEMO: ここ、後にFPU計算にした方が速いかもしれません。 */
				rate = (float)(frame - frameKey) / (float)(frameKeyEnd - frameKey);
				rate = 1.0f - rate;
				rate *= rate;
				rate = 1.0f - rate;
				outValue->ID = value.ID;
				for(int i=0; i<Library::Data::Animation::Attribute::Shader::Constant::COUNT_PARAMETER; i++)	{
					parameterEffectStart = value.Parameter[i];
					outValue->Parameter[i] = ((valueEnd.Parameter[i] - parameterEffectStart) * rate) + parameterEffectStart;
				}

				return(true);	/* outValueは新規値 */

			case KindFormula::FORMULA__RESERVED_04:
				break;
			case KindFormula::FORMULA__RESERVED_03:
				break;
			case KindFormula::FORMULA__RESERVED_02:
				break;
			case KindFormula::FORMULA__RESERVED_01:
				break;

			case KindFormula::FORMULA_CPE:
				if(framePrevious == frameKey)	{
					return(false);	/* outValueは上書きされていない */
				}

				ValueExtractShader(outValue, tableValue, index);

				return(true);	/* outValueは新規値 */
		}

		return(false);	/* outValueは上書きされていない */
	}

protected:
public:
	/* ValueGet系関数 */
	/* MEMO: 本関数をAttributeExpand派生クラスのValueGet関数に設定します。 */
	/* MEMO: 返値がfalseの場合に、outFrameKeyはいじらないこと（outFrameKeyは最後にキーをデコードしたフレームのワークなので、更新時以外に操作しない）。 */
	static bool ValueGetInt(Sint32* outValue, int* outFrameKey, const ss::ssfb2::ContainerInt* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		if(true == IntExpandInt(outValue, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode,  container->TableValue()))	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	static bool ValueGetFloat(Float32* outValue, int* outFrameKey, const ss::ssfb2::ContainerFloat* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		if(true == FloatExpandFloat(outValue, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	static bool ValueGetVector2(CPU::Vector2* outValue, int* outFrameKey, const ss::ssfb2::ContainerVector2* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		/* MEMO: countCodeが0より大きい場合に圧縮データから値を受け取っているので、条件文が他の関数とは逆になっていますので注意してください。 */
		/*       ※他の関数はcountCodeが0以下の場合には値を変更しないで帰るようになっています。                                               */
		bool flagUpdated = false;
		int countCode;
		const flatbuffers::Vector<int32_t>* tableCode;
		tableCode = (*(container->TableCodeValue()))[KindElementVector::ELEMENT_X]->TableCode();
		countCode = tableCode->size();
		float value;
		if(0 < countCode)	{
			if(true == FloatExpandVector2(&value, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
				flagUpdated = true;
				outValue->SetX(value);
			}
		}
		tableCode = (*(container->TableCodeValue()))[KindElementVector::ELEMENT_Y]->TableCode();
		countCode = tableCode->size();
		if(0 < countCode)	{
			if(true == FloatExpandVector2(&value, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
				flagUpdated = true;
				outValue->SetY(value);
			}
		}
		if(true == flagUpdated)	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	static bool ValueGetVector3(CPU::Vector3* outValue, int* outFrameKey, const ss::ssfb2::ContainerVector3* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		/* MEMO: countCodeが0より大きい場合に圧縮データから値を受け取っているので、条件文が他の関数とは逆になっていますので注意してください。 */
		/*       ※他の関数はcountCodeが0以下の場合には値を変更しないで帰るようになっています。                                               */
		bool flagUpdated = false;
		int countCode;
		const flatbuffers::Vector<int32_t>* tableCode;
		tableCode = (*(container->TableCodeValue()))[KindElementVector::ELEMENT_X]->TableCode();
		countCode = tableCode->size();
		float value;
		if(0 < countCode)	{
			if(true == FloatExpandVector3(&value, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
				flagUpdated = true;
				outValue->SetX(value);
			}
		}
		tableCode = (*(container->TableCodeValue()))[KindElementVector::ELEMENT_Y]->TableCode();
		countCode = tableCode->size();
		if(0 < countCode)	{
			if(true == FloatExpandVector3(&value, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
				flagUpdated = true;
				outValue->SetY(value);
			}
		}
		tableCode = (*(container->TableCodeValue()))[KindElementVector::ELEMENT_Z]->TableCode();
		countCode = tableCode->size();
		if(0 < countCode)	{
			if(true == FloatExpandVector3(&value, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
				flagUpdated = true;
				outValue->SetZ(value);
			}
		}
		if(true == flagUpdated)	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	/* MEMO: Statusは非対応 */
	/* MEMO: Cellは非対応 */
	static bool ValueGetPartsColor(Library::Data::Animation::Attribute::PartsColor* outValue, int* outFrameKey, const ss::ssfb2::ContainerPartsColor* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		if(true == ValueExpandPartsColor(outValue, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	static bool ValueGetVertexCorrection(Library::Data::Animation::Attribute::VertexCorrection* outValue, int* outFrameKey, const ss::ssfb2::ContainerVertexCorrection* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		if(true == ValueExpandVertexCorrection(outValue, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	/* MEMO: UserDataは非対応 */
	/* MEMO: Instanceは非対応 */
	/* MEMO: Effectは非対応 */
	static bool ValueGetDeform(Library::Data::Animation::Attribute::Deform* outValue, int* outFrameKey, const ss::ssfb2::ContainerDeform* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(0 >= countCode)	{
			return(false);
		}
		if(true == ValueExpandDeform(outValue, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue(), container))	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}
	static bool ValueGetShader(Library::Data::Animation::Attribute::Shader* outValue, int* outFrameKey, const ss::ssfb2::ContainerShader* container, const ArgumentContainer& argument)
	{
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::CodeValueContainer>>* tableCodeValue = container->TableCodeValue();
		if(nullptr == tableCodeValue)	{
			return(false);
		}
		if(0 >= tableCodeValue->size())	{
			return(false);
		}
		int frame = argument.Frame;
		if((0 <= *outFrameKey) && (frame == argument.FramePrevious))	{
			return(false);
		}

		const flatbuffers::Vector<int32_t>* tableCode = (*(container->TableCodeValue()))[0]->TableCode();
		int countCode = tableCode->size();
		if(true == ValueExpandShader(outValue, frame, *outFrameKey, (Uint32*)(tableCode->data()), countCode, container->TableValue()))	{
			*outFrameKey = frame;
			return(true);
		}

		return(false);
	}

	/* MEMO: Signalは非対応 */

	/* ValueGetIndex系関数 */
	/* MEMO: 本関数をAttributeExpand派生クラスのValueGetIndex関数に設定します。 */
	/* MEMO: 返値がfalseの場合に、outFrameKeyはいじらないこと（outFrameKeyは最後にキーをデコードしたフレームのワークなので、更新時以外に操作しない）。 */
	/* MEMO: 基本的にUserData・Signalしか対応していません（本クラスはUserData・Signalに対応していないので、全ての機能が非対応）。 */
	static bool ValueGetIndexInt(Sint32* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerInt* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexFloat(Float32* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerFloat* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexVector2(CPU::Vector2* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerVector2* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexVector3(CPU::Vector3* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerVector3* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexPartsColor(Library::Data::Animation::Attribute::PartsColor* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerPartsColor* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexVertexCorrection(Library::Data::Animation::Attribute::VertexCorrection* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerVertexCorrection* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexDeform(Library::Data::Animation::Attribute::Deform* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerDeform* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}
	static bool ValueGetIndexShader(Library::Data::Animation::Attribute::Shader* outValue, int* outFrameKey, int index, const ss::ssfb2::ContainerShader* container, const ArgumentContainer& argument)
	{
		/* MEMO: 非対応 */
		return(false);
	}

	/* CountGetValue系関数 */
	/* MEMO: 本関数をAttributeExpand派生クラスのCountGetValue関数に設定します。 */
	/* MEMO: 基本的にUserData・Signalしか対応していません（本クラスはUserData・Signalに対応していないので、全ての機能が非対応）。 */
	static int CountGetValueInt(const ss::ssfb2::ContainerInt* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueFloat(const ss::ssfb2::ContainerFloat* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueVector2(const ss::ssfb2::ContainerVector2* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueVector3(const ss::ssfb2::ContainerVector3* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValuePartsColor(const ss::ssfb2::ContainerPartsColor* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueVertexCorrection(const ss::ssfb2::ContainerVertexCorrection* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueDeform(const ss::ssfb2::ContainerDeform* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}
	static int CountGetValueShader(const ss::ssfb2::ContainerShader* container)
	{
		/* MEMO: 非対応 */
		return(-1);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */
/* MEMO: 各関数を内部クラスで実装したため、宣言順序の関係からここで定義。 */
static const AttributeExpandInt AttributeExpanderInt[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandInt(	nullptr,						nullptr,							nullptr								),
	/* STANDARD_CPE */			AttributeExpandInt(	StandardCPE::ValueGetInt,		StandardCPE::ValueGetIndexInt,		StandardCPE::CountGetValueInt		),
	/* CPE_FLYWEIGHT */			AttributeExpandInt(	nullptr,						nullptr,							nullptr								),
	/* CPE_INTERPOLATE */		AttributeExpandInt(	CPE_Interpolate::ValueGetInt,	CPE_Interpolate::ValueGetIndexInt,	CPE_Interpolate::CountGetValueInt	),
};
static const AttributeExpandFloat AttributeExpanderFloat[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandFloat(	nullptr,						nullptr,								nullptr								),
	/* STANDARD_CPE */			AttributeExpandFloat(	StandardCPE::ValueGetFloat,		StandardCPE::ValueGetIndexFloat,		StandardCPE::CountGetValueFloat		),
	/* CPE_FLYWEIGHT */			AttributeExpandFloat(	nullptr,						nullptr,								nullptr								),
	/* CPE_INTERPOLATE */		AttributeExpandFloat(	CPE_Interpolate::ValueGetFloat,	CPE_Interpolate::ValueGetIndexFloat,	CPE_Interpolate::CountGetValueFloat	),
};
static const AttributeExpandVector2 AttributeExpanderVector2[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandVector2(	nullptr,							nullptr,								nullptr									),
	/* STANDARD_CPE */			AttributeExpandVector2(	StandardCPE::ValueGetVector2,		StandardCPE::ValueGetIndexVector2,		StandardCPE::CountGetValueVector2		),
	/* CPE_FLYWEIGHT */			AttributeExpandVector2(	nullptr,							nullptr,								nullptr									),
	/* CPE_INTERPOLATE */		AttributeExpandVector2(	CPE_Interpolate::ValueGetVector2,	CPE_Interpolate::ValueGetIndexVector2,	CPE_Interpolate::CountGetValueVector2	),
};
static const AttributeExpandVector3 AttributeExpanderVector3[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandVector3(	nullptr,						nullptr,									nullptr									),
	/* STANDARD_CPE */			AttributeExpandVector3(	StandardCPE::ValueGetVector3,	StandardCPE::ValueGetIndexVector3,			StandardCPE::CountGetValueVector3		),
	/* CPE_FLYWEIGHT */			AttributeExpandVector3(	nullptr,						nullptr,									nullptr									),
	/* CPE_INTERPOLATE */		AttributeExpandVector3(	CPE_Interpolate::ValueGetVector3,	CPE_Interpolate::ValueGetIndexVector3,	CPE_Interpolate::CountGetValueVector3	),
};
static const AttributeExpandStatus AttributeExpanderStatus[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandStatus(	nullptr,						nullptr,							nullptr								),
	/* STANDARD_CPE */			AttributeExpandStatus(	StandardCPE::ValueGetStatus,	StandardCPE::ValueGetIndexStatus,	StandardCPE::CountGetValueStatus	),
	/* CPE_FLYWEIGHT */			AttributeExpandStatus(	nullptr,						nullptr,							nullptr								),
	/* CPE_INTERPOLATE */		AttributeExpandStatus(	nullptr,						nullptr,							nullptr								),
};
static const AttributeExpandCell AttributeExpanderCell[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandCell(	nullptr,					nullptr,						nullptr							),
	/* STANDARD_CPE */			AttributeExpandCell(	StandardCPE::ValueGetCell,	StandardCPE::ValueGetIndexCell,	StandardCPE::CountGetValueCell	),
	/* CPE_FLYWEIGHT */			AttributeExpandCell(	nullptr,					nullptr,						nullptr							),
	/* CPE_INTERPOLATE */		AttributeExpandCell(	nullptr,					nullptr,						nullptr							),
};
static const AttributeExpandPartsColor AttributeExpanderPartsColor[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandPartsColor(	nullptr,								nullptr,									nullptr										),
	/* STANDARD_CPE */			AttributeExpandPartsColor(	StandardCPE::ValueGetPartsColor,		StandardCPE::ValueGetIndexPartsColor,		StandardCPE::CountGetValuePartsColor		),
	/* CPE_FLYWEIGHT */			AttributeExpandPartsColor(	nullptr,								nullptr,									nullptr										),
	/* CPE_INTERPOLATE */		AttributeExpandPartsColor(	CPE_Interpolate::ValueGetPartsColor,	CPE_Interpolate::ValueGetIndexPartsColor,	CPE_Interpolate::CountGetValuePartsColor	),
};
static const AttributeExpandVertexCorrection AttributeExpanderVertexCorrection[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandVertexCorrection(	nullptr,									nullptr,										nullptr											),
	/* STANDARD_CPE */			AttributeExpandVertexCorrection(	StandardCPE::ValueGetVertexCorrection,		StandardCPE::ValueGetIndexVertexCorrection,		StandardCPE::CountGetValueVertexCorrection		),
	/* CPE_FLYWEIGHT */			AttributeExpandVertexCorrection(	nullptr,									nullptr,										nullptr											),
	/* CPE_INTERPOLATE */		AttributeExpandVertexCorrection(	CPE_Interpolate::ValueGetVertexCorrection,	CPE_Interpolate::ValueGetIndexVertexCorrection,	CPE_Interpolate::CountGetValueVertexCorrection	),
};
static const AttributeExpandUserData AttributeExpanderUserData[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandUserData(	nullptr,						nullptr,							nullptr								),
	/* STANDARD_CPE */			AttributeExpandUserData(	StandardCPE::ValueGetUserData,	StandardCPE::ValueGetIndexUserData, StandardCPE::CountGetValueUserData	),
	/* CPE_FLYWEIGHT */			AttributeExpandUserData(	nullptr,						nullptr,							nullptr								),
	/* CPE_INTERPOLATE */		AttributeExpandUserData(	nullptr,						nullptr,							nullptr								),
};
static const AttributeExpandInstance AttributeExpanderInstance[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandInstance(	nullptr,						nullptr,							nullptr								),
	/* STANDARD_CPE */			AttributeExpandInstance(	StandardCPE::ValueGetInstance,	StandardCPE::ValueGetIndexInstance,	StandardCPE::CountGetValueInstance	),
	/* CPE_FLYWEIGHT */			AttributeExpandInstance(	nullptr,						nullptr,							nullptr								),
	/* CPE_INTERPOLATE */		AttributeExpandInstance(	nullptr,						nullptr,							nullptr								),
};
static const AttributeExpandEffect AttributeExpanderEffect[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandEffect(	nullptr,						nullptr,							nullptr								),
	/* STANDARD_CPE */			AttributeExpandEffect(	StandardCPE::ValueGetEffect,	StandardCPE::ValueGetIndexEffect,	StandardCPE::CountGetValueEffect	),
	/* CPE_FLYWEIGHT */			AttributeExpandEffect(	nullptr,						nullptr,							nullptr								),
	/* CPE_INTERPOLATE */		AttributeExpandEffect(	nullptr,						nullptr,							nullptr								),
};
static const AttributeExpandDeform AttributeExpanderDeform[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandDeform(	nullptr,							nullptr,								nullptr									),
	/* STANDARD_CPE */			AttributeExpandDeform(	StandardCPE::ValueGetDeform,		StandardCPE::ValueGetIndexDeform,		StandardCPE::CountGetValueDeform		),
	/* CPE_FLYWEIGHT */			AttributeExpandDeform(	nullptr,							nullptr,								nullptr									),
	/* CPE_INTERPOLATE */		AttributeExpandDeform(	CPE_Interpolate::ValueGetDeform,	CPE_Interpolate::ValueGetIndexDeform,	CPE_Interpolate::CountGetValueDeform	),
};
static const AttributeExpandShader AttributeExpanderShader[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandShader(	nullptr,							nullptr,								nullptr									),
	/* STANDARD_CPE */			AttributeExpandShader(	StandardCPE::ValueGetShader,		StandardCPE::ValueGetIndexShader,		StandardCPE::CountGetValueShader		),
	/* CPE_FLYWEIGHT */			AttributeExpandShader(	nullptr,							nullptr,								nullptr									),
	/* CPE_INTERPOLATE */		AttributeExpandShader(	CPE_Interpolate::ValueGetShader,	CPE_Interpolate::ValueGetIndexShader,	CPE_Interpolate::CountGetValueShader	),
};
static const AttributeExpandSignal AttributeExpanderSignal[KindTypePack::TERMINATOR] = {
	/* STANDARD_UNCOMPRESSED */	AttributeExpandSignal(	nullptr,							nullptr,								nullptr									),
	/* STANDARD_CPE */			AttributeExpandSignal(	StandardCPE::ValueGetSignal,		StandardCPE::ValueGetIndexSignal,		StandardCPE::CountGetValueSignal		),
	/* CPE_FLYWEIGHT */			AttributeExpandSignal(	nullptr,							nullptr,								nullptr									),
	/* CPE_INTERPOLATE */		AttributeExpandSignal(	nullptr,							nullptr,								nullptr									),
};

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
const AttributeExpandInt* AttributeExpanderGetInt(KindTypePack::Kind kindCompress)
{
	const AttributeExpandInt& expander = AttributeExpanderInt[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandFloat* AttributeExpanderGetFloat(KindTypePack::Kind kindCompress)
{
	const AttributeExpandFloat& expander = AttributeExpanderFloat[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandVector2* AttributeExpanderGetVector2(KindTypePack::Kind kindCompress)
{
	const AttributeExpandVector2& expander = AttributeExpanderVector2[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandVector3* AttributeExpanderGetVector3(KindTypePack::Kind kindCompress)
{
	const AttributeExpandVector3& expander = AttributeExpanderVector3[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandStatus* AttributeExpanderGetStatus(KindTypePack::Kind kindCompress)
{
	const AttributeExpandStatus& expander = AttributeExpanderStatus[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandCell* AttributeExpanderGetCell(KindTypePack::Kind kindCompress)
{
	const AttributeExpandCell& expander = AttributeExpanderCell[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandPartsColor* AttributeExpanderGetPartsColor(KindTypePack::Kind kindCompress)
{
	const AttributeExpandPartsColor& expander = AttributeExpanderPartsColor[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandVertexCorrection* AttributeExpanderGetVertexCorrection(KindTypePack::Kind kindCompress)
{
	const AttributeExpandVertexCorrection& expander = AttributeExpanderVertexCorrection[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandUserData* AttributeExpanderGetUserData(KindTypePack::Kind kindCompress)
{
	const AttributeExpandUserData& expander = AttributeExpanderUserData[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandInstance* AttributeExpanderGetInstance(KindTypePack::Kind kindCompress)
{
	const AttributeExpandInstance& expander = AttributeExpanderInstance[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandEffect* AttributeExpanderGetEffect(KindTypePack::Kind kindCompress)
{
	const AttributeExpandEffect& expander = AttributeExpanderEffect[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandDeform* AttributeExpanderGetDeform(KindTypePack::Kind kindCompress)
{
	const AttributeExpandDeform& expander = AttributeExpanderDeform[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandShader* AttributeExpanderGetShader(KindTypePack::Kind kindCompress)
{
	const AttributeExpandShader& expander = AttributeExpanderShader[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
const AttributeExpandSignal* AttributeExpanderGetSignal(KindTypePack::Kind kindCompress)
{
	const AttributeExpandSignal& expander = AttributeExpanderSignal[kindCompress];

	return((true == expander.IsValid()) ? &expander : nullptr);
}
}	/* Pack */
}	/* Animation */
}	/* Data */
}	/* Library */
}	/* SpriteStudio6 */
