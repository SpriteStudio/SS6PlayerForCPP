/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* アニメーションのアトリビュートの圧縮展開関係の定義群です。                 */
/* SSFB2データの外郭仕様は下記になっています。                                */
/* 1. シリアライズにはGoogle FlatBuffersを使用しています。                    */
/* 2. アニメーションアトリビュートの圧縮                                      */
/*   SS6Player for Unity（SS6PU）と同様にアニメーションデータの各アトリビュー */
/*   トに専用の圧縮がかかっています。                                         */
/*   圧縮方式は下記の4種類になりますが、a（非圧縮）とc（CPE+Flyweight）は使用 */
/*   していません（一応、仕様として下記に記載してはありますが、SS6Player for  */
/*   Unityのみでの使用になっており、aとcはSS6ConverterVer2の時点で出力不能に  */
/*   なっています）。                                                         */
/*   a. 非圧縮（Uncompressed）                                                */
/*      全てのフレームの値を展開し・一切省略や圧縮をしません。                */
/*      ※廃止の理由は、そもそもデータが大きいのと毎フレームデコードするため  */
/*        利点がなく、専ら他の圧縮のための元データを作成する際程度しか有用性  */
/*        がないためです（SS6ConverterVer2の内部処理でのみ使用）。            */
/*   b. 変化点抽出圧縮（CPE: Changing Point Extract）                         */
/*      非圧縮のデータの中で「値が変化している」部分のみを抽出します。        */
/*      また、同じ値が現れた場合には「各アニメーション内の同じパーツの同じア  */
/*      トリビュート」の中で既出データを辞書として使用します。                */
/*   c. 変化点抽出圧縮＋GOF Flyweight（CPE + Flyweight）                      */
/*      圧縮そのものはbと変わりませんが、辞書を全アニメーション・全パーツ共用 */
/*      の大域辞書化したものです。                                            */
/*      ※廃止の理由は、大きなアニメーションになるとすぐに大域辞書がオーバー  */
/*        フローしてしまうことと、圧縮情報が多次元配列化するために実装が複雑  */
/*        なりランタイム負荷が高いためです。                                  */
/*   d. 変化点抽出圧縮＋補間式（CPE + Interpolate）                           */
/*      実行負荷が小さい「直線」「加速」「減速」の補間はキーフレームのみ格納  */
/*      してランタイムで計算展開し・実行負荷が高い「エルミート」「ベジエ」補  */
/*      間は展開後にCPEで圧縮します（「補間なし」も「展開後CPE」ですが、CPEで */
/*      重複フレームが削られるため、データ量としてキーフレーム数以下になるこ  */
/*      とは担保されています）。                                              */
/*                                                                            */
/* 本部位は上記の内2を展開するための機能で、SS6PUでは「PackAttribute」となっ  */
/* ていた部位です。                                                           */
/* SS6PUではインポータ部と機能を共用していたため実装が複雑だったため、深い階  */
/* 層に沈める形で半隠蔽していましたが、C++版ではその必要もないので比較的平易  */
/* な実装になっています。                                                     */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include "ss6player_data.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Data	{
namespace Animation	{
namespace Pack	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */
namespace KindTypePack	{
enum Kind	{
	STANDARD_UNCOMPRESSED = 0,	/* ※未使用（仕様上予約） */	/* 非圧縮（Uncompressed） */
	STANDARD_CPE,												/* 変化点圧縮（Changing-Point Extracting: CPE） */
	CPE_FLYWEIGHT,				/* ※未使用（仕様上予約） */	/* 変化点圧縮＆GoF-Flyweight（CPEの辞書を大域辞書化） */
	CPE_INTERPOLATE,											/* 変化点圧縮＆補間式計算（コストの低い補間は数式で・それ以外をCPE） */

	TERMINATOR,
};
}	/* KindTypePack */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* アトリビュートコンテナへの引数群 */
/* MEMO: 基本的にはアトリビュートのデータから取り出すフレーム番号などを格納したワークです。 */
/*       本クラスの実体は再生トラック制御クラスが持っています。                             */
/*       ※基本的に全部バラで管理すると引数で渡すコストもバカにならないのでまとめているだけ */
/*         です。                                                                           */
class ArgumentContainer
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
	const void* /* const Library::Data::Project::Animation* */ DataAnimation;	/* アニメーションデータ *//* ※宣言順序などの関係でvoid*に…… */
	int IndexAnimation;															/* アニメーション番号 */
	int IDParts;																/* パーツID *//* ※ほぼ予備データ */
	int Frame;																	/* 再生フレーム（0インデックス） */
	int FramePrevious;															/* 前回処理で再生したフレーム（0インデックス・初回再生時は-1） */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態にします。
	*/
	inline void CleanUp(void)
	{
		DataAnimation = nullptr;
		IndexAnimation = -1;
		IDParts = -1;
		Frame = -1;
		FramePrevious = -1;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態でコンストラクトします。
	*/
	ArgumentContainer(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	dataAnimation
		アニメーションデータのポインタ
	@param	indexAnimation
		アニメーション番号
	@param	idParts
		パーツID
	@param	frame
		再生フレーム
	@param	framePrevious
		前回処理で再生したフレーム
		-1 == 初回再生

	@retval	なし

	本クラスの内容を指定内容でコンストラクトします。
	*/
	ArgumentContainer(const void* dataAnimation, int indexAnimation, int idParts, int frame, int framePrevious) :
		DataAnimation(dataAnimation),
		IndexAnimation(indexAnimation),
		IDParts(idParts),
		Frame(frame),
		FramePrevious(framePrevious)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~ArgumentContainer(void)
	{
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 能力コンテナ */
/* MEMO: 各圧縮種別の対応アトリビュート情報を格納しているクラスです。 */
/*       インポータで必要になりますが、念のためランタイムでも持ってお */
/*       くようにします。                                             */
class CapacityContainer
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
	enum FlagBit : Uint32	{
		STATUS = 0x00000001,
		CELL = 0x00000002,
		POSITION = 0x00000004,
		ROTATION = 0x00000008,
		SCALING = 0x00000010,
		SCALING_LOCAL = 0x00000020,
		RATE_OPACITY = 0x00000040,
		PRIORITY = 0x00000080,
		PARTS_COLOR = 0x00000100,
		VERTEX_CORRECTION = 0x00000200,
		OFFSET_PIVOT = 0x00000400,
		POSITION_ANCHOR = 0x00000800,
		SIZE_FORCE = 0x00001000,
		POSITION_TEXTURE = 0x00002000,
		ROTATION_TEXTURE = 0x00004000,
		SCALING_TEXTURE = 0x00008000,
		RADIUS_COLLISION = 0x00010000,
		USER_DATA = 0x00020000,
		INSTANCE = 0x00040000,
		EFFECT = 0x00080000,
		DEFORM = 0x00100000,
	};

protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
	Uint32 /* FlagBit */ Flags;

	/* 各圧縮型式の能力 */
	/* MEMO: SS6Player for Unityでは各圧縮クラスの中にありましたが、 */
	/*       C++版は各圧縮クラスがソース内スコープで隠蔽されているの */
	/*       で、ここに入れておきます。                              */
	/* MEMO: UncompressedとCPE_Flyweightは仕様上入れてありますが、内容は */
	/*       全てfalse（非対応）です。                                   */
	static const CapacityContainer Capacity[KindTypePack::Kind::TERMINATOR];

protected:
public:

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	CapacityContainer(	bool status,
						bool cell,
						bool position,
						bool rotation,
						bool scaling,
						bool scalingLocal,
						bool rateOpacity,
						bool priority,
						bool partsColor,
						bool vertexCorrection,
						bool offsetPivot,
						bool positionAnchor,
						bool sizeForce,
						bool positionTexture,
						bool rotationTexture,
						bool scalingTexture,
						bool radiusCollision,
						bool userData,
						bool instance,
						bool effect,
						bool deform
					)
	{
		Flags = 0;
		Flags |= (true == status) ? FlagBit::STATUS : 0;

		Flags |= (true == cell) ? FlagBit::CELL : 0;

		Flags |= (true == position) ? FlagBit::POSITION : 0;
		Flags |= (true == rotation) ? FlagBit::ROTATION : 0;
		Flags |= (true == scaling) ? FlagBit::SCALING : 0;
		Flags |= (true == scalingLocal) ? FlagBit::SCALING_LOCAL : 0;

		Flags |= (true == rateOpacity) ? FlagBit::RATE_OPACITY : 0;
		Flags |= (true == priority) ? FlagBit::PRIORITY : 0;
		Flags |= (true == partsColor) ? FlagBit::PARTS_COLOR : 0;
		Flags |= (true == vertexCorrection) ? FlagBit::VERTEX_CORRECTION : 0;

		Flags |= (true == offsetPivot) ? FlagBit::OFFSET_PIVOT : 0;
		Flags |= (true == positionAnchor) ? FlagBit::POSITION_ANCHOR : 0;
		Flags |= (true == sizeForce) ? FlagBit::SIZE_FORCE : 0;
		Flags |= (true == positionTexture) ? FlagBit::POSITION_TEXTURE : 0;
		Flags |= (true == rotationTexture) ? FlagBit::ROTATION_TEXTURE : 0;
		Flags |= (true == scalingTexture) ? FlagBit::SCALING_TEXTURE : 0;

		Flags |= (true == radiusCollision) ? FlagBit::RADIUS_COLLISION : 0;

		Flags |= (true == userData) ? FlagBit::USER_DATA : 0;
		Flags |= (true == instance) ? FlagBit::INSTANCE : 0;
		Flags |= (true == effect) ? FlagBit::EFFECT : 0;
		Flags |= (true == deform) ? FlagBit::DEFORM : 0;
	}

	inline bool Status(void)
	{
		return(0 != (Flags & FlagBit::STATUS));
	}
	inline bool Cell(void)
	{
		return(0 != (Flags & FlagBit::CELL));
	}
	inline bool Position(void)
	{
		return(0 != (Flags & FlagBit::POSITION));
	}
	inline bool Rotation(void)
	{
		return(0 != (Flags & FlagBit::ROTATION));
	}
	inline bool Scaling(void)
	{
		return(0 != (Flags & FlagBit::SCALING));
	}
	inline bool ScalingLocal(void)
	{
		return(0 != (Flags & FlagBit::SCALING_LOCAL));
	}
	inline bool RateOpacity(void)
	{
		return(0 != (Flags & FlagBit::RATE_OPACITY));
	}
	inline bool Priority(void)
	{
		return(0 != (Flags & FlagBit::PRIORITY));
	}
	inline bool PartsColor(void)
	{
		return(0 != (Flags & FlagBit::PARTS_COLOR));
	}
	inline bool VertexCorrection(void)
	{
		return(0 != (Flags & FlagBit::VERTEX_CORRECTION));
	}
	inline bool OffsetPivot(void)
	{
		return(0 != (Flags & FlagBit::OFFSET_PIVOT));
	}
	inline bool PositionAnchor(void)
	{
		return(0 != (Flags & FlagBit::POSITION_ANCHOR));
	}
	inline bool SizeForce(void)
	{
		return(0 != (Flags & FlagBit::SIZE_FORCE));
	}
	inline bool PositionTexture(void)
	{
		return(0 != (Flags & FlagBit::POSITION_TEXTURE));
	}
	inline bool RotationTexture(void)
	{
		return(0 != (Flags & FlagBit::ROTATION_TEXTURE));
	}
	inline bool ScalingTexture(void)
	{
		return(0 != (Flags & FlagBit::SCALING_TEXTURE));
	}
	inline bool RadiusCollision(void)
	{
		return(0 != (Flags & FlagBit::RADIUS_COLLISION));
	}
	inline bool UserData(void)
	{
		return(0 != (Flags & FlagBit::USER_DATA));
	}
	inline bool Instance(void)
	{
		return(0 != (Flags & FlagBit::INSTANCE));
	}
	inline bool Effect(void)
	{
		return(0 != (Flags & FlagBit::EFFECT));
	}
	inline bool Deform(void)
	{
		return(0 != (Flags & FlagBit::DEFORM));
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 圧縮展開クラス */
/* MEMO: 各圧縮型式展開クラスは全て本クラスを型指定で展開したものです。 */
/*       抽象仮想テンプレートクラス的に扱うために、関数は関数ポインタで */
/*       の実装になっています。                                         */
/*       型と展開型式毎の組み合わせになるため、実体はかなりの数になりま */
/*       すが……クラスのインスタンスは静的配置になるために容量などを圧 */
/*       迫する心配はありません（各データはその実体へのポインタを持つこ */
/*       とになります）。                                               */
template<typename _TypeValue, typename _TypeContainer> class AttributeExpand
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* ********************************************************* */
	//! 値取得関数型
	/*!
	@param	outValue
		取得した値の格納先
	@param	outFrameKey
		取得した値が配置されているフレーム格納先
	@param	container
		アトリビュートコンテナの配列
	@param	argument
		デコード指定

	@retval	関数値
		true == 成功
		false == 失敗
	@retval	outValue
		取得した値
	@retval	outFrameKey
		取得した値が配置されているフレーム

	argumentで指定された位置のデータを取得するための関数です。
	outFrameKeyは、取得したデータが「本来配置されているフレーム」を
		指します（2フレーム目と1フレーム目のデータが同じだった時に
		2フレーム目を取得しようとした場合は1が返ります）。

	本方式での値取得に対応していない場合、falseを返す関数を割り当て
		てください（nullptrは許可しません）。
	*/
	typedef bool (*FunctionValueGet)(	_TypeValue* outValue,
										int* outFrameKey,
										const _TypeContainer* container,
										const ArgumentContainer& argument
									);

	/* ********************************************************* */
	//! インデックスでの値取得関数型
	/*!
	@param	outValue
		取得した値の格納先
	@param	outFrameKey
		取得した値が配置されているフレーム格納先
	@param	index
		データのインデックス
	@param	container
		アトリビュートコンテナの配列
	@param	argument
		デコード指定

	@retval	関数値
		true == 成功
		false == 失敗
	@retval	outValue
		取得した値
	@retval	outFrameKey
		取得した値が配置されているフレーム

	containerのindexで指定された位置のデータをフレーム数に関係なく
		取得します。
	ユーザーデータなどの特殊な取得方法を行うもので使用します。

	本方式での値取得に対応していない場合、falseを返す関数を割り当て
		てください（nullptrは許可しません）。
	*/
	typedef bool (*FunctionValueGetIndex)(	_TypeValue* outValue,
											int* outFrameKey,
											int index,
											const _TypeContainer* container,
											const ArgumentContainer& argument
										);

	/* ********************************************************* */
	//! コンテナに格納されているデータ数を取得
	/*!
	@param	container
		アトリビュートコンテナの配列

	@retval	関数値
		アトリビュートコンテナの配列要素数
		-1 == エラー

	インデックスでの値取得を行う場合には必ず本関数を使用して、最大
		のインデックスを超えないようにしてください。

	本方式での値取得に対応していない場合、-1を返す関数を割り当てて
		ください（nullptrは許可しません）。
	*/
	typedef int (*FunctionCountGetValue)(const _TypeContainer* container);

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* アクセス用の関数ポインタ群 */
	FunctionValueGet ValueGet;
	FunctionValueGetIndex ValueGetIndex;
	FunctionCountGetValue CountGetValue;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	functionValuGet
		ValueGet関数のポインタ
	@param	functionValueGetIndex
		ValueGetIndex関数のポインタ
	@param	functionCountGetValue
		CountGetValue関数のポインタ
	@param	frame

	@retval	なし

	本クラスの内容を指定内容でコンストラクトします。
	※本クラスは引数無しのコンストラクタは持っていません。
	*/
	AttributeExpand(	FunctionValueGet functionValuGet,
						FunctionValueGetIndex functionValueGetIndex,
						FunctionCountGetValue functionCountGetValue
				) :
		ValueGet(functionValuGet),
		ValueGetIndex(functionValueGetIndex),
		CountGetValue(functionCountGetValue)
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~AttributeExpand(void)
	{
	}

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効（呼び出せる状態）
		false == 無効（非対応）
	*/
	inline bool IsValid(void) const
	{
		/* MEMO: 現時点では対応している場合、全関数をとりあえず設定する仕様なので、 */
		/*       ValueGetだけチェックしておく（そもそもユーザデータ以外はValueGetが */
		/*       ないと正常動作できないので）。                                     */
		return(nullptr != ValueGet);	/* ? true : false */
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 各格納型用のアトリビュート展開クラス */
typedef AttributeExpand<Sint32, ss::ssfb2::ContainerInt> AttributeExpandInt;
typedef AttributeExpand<Float32, ss::ssfb2::ContainerFloat> AttributeExpandFloat;
typedef AttributeExpand<CPU::Vector2, ss::ssfb2::ContainerVector2> AttributeExpandVector2;
typedef AttributeExpand<CPU::Vector3, ss::ssfb2::ContainerVector3> AttributeExpandVector3;
typedef AttributeExpand<Library::Data::Animation::Attribute::Status, ss::ssfb2::ContainerStatus> AttributeExpandStatus;
typedef AttributeExpand<Library::Data::Animation::Attribute::Cell, ss::ssfb2::ContainerCell> AttributeExpandCell;
typedef AttributeExpand<Library::Data::Animation::Attribute::PartsColor, ss::ssfb2::ContainerPartsColor> AttributeExpandPartsColor;
typedef AttributeExpand<Library::Data::Animation::Attribute::VertexCorrection, ss::ssfb2::ContainerVertexCorrection> AttributeExpandVertexCorrection;
typedef AttributeExpand<Library::Data::Animation::Attribute::UserData, ss::ssfb2::ContainerUserData> AttributeExpandUserData;
typedef AttributeExpand<Library::Data::Animation::Attribute::Instance, ss::ssfb2::ContainerInstance> AttributeExpandInstance;
typedef AttributeExpand<Library::Data::Animation::Attribute::Effect, ss::ssfb2::ContainerEffect> AttributeExpandEffect;
typedef AttributeExpand<Library::Data::Animation::Attribute::Deform, ss::ssfb2::ContainerDeform> AttributeExpandDeform;
typedef AttributeExpand<Library::Data::Animation::Attribute::Shader, ss::ssfb2::ContainerShader> AttributeExpandShader;
typedef AttributeExpand<Library::Data::Animation::Attribute::Signal, ss::ssfb2::ContainerSignal> AttributeExpandSignal;

/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */
const AttributeExpandInt* AttributeExpanderGetInt(KindTypePack::Kind kindCompress);
const AttributeExpandFloat* AttributeExpanderGetFloat(KindTypePack::Kind kindCompress);
const AttributeExpandVector2* AttributeExpanderGetVector2(KindTypePack::Kind kindCompress);
const AttributeExpandVector3* AttributeExpanderGetVector3(KindTypePack::Kind kindCompress);
const AttributeExpandStatus* AttributeExpanderGetStatus(KindTypePack::Kind kindCompress);
const AttributeExpandCell* AttributeExpanderGetCell(KindTypePack::Kind kindCompress);
const AttributeExpandPartsColor* AttributeExpanderGetPartsColor(KindTypePack::Kind kindCompress);
const AttributeExpandVertexCorrection* AttributeExpanderGetVertexCorrection(KindTypePack::Kind kindCompress);
const AttributeExpandUserData* AttributeExpanderGetUserData(KindTypePack::Kind kindCompress);
const AttributeExpandInstance* AttributeExpanderGetInstance(KindTypePack::Kind kindCompress);
const AttributeExpandEffect* AttributeExpanderGetEffect(KindTypePack::Kind kindCompress);
const AttributeExpandDeform* AttributeExpanderGetDeform(KindTypePack::Kind kindCompress);
const AttributeExpandShader* AttributeExpanderGetShader(KindTypePack::Kind kindCompress);
const AttributeExpandSignal* AttributeExpanderGetSignal(KindTypePack::Kind kindCompress);

}	/* Pack */
}	/* Animation */
}	/* Data */
}	/* Library */
}	/* SpriteStudio6 */
