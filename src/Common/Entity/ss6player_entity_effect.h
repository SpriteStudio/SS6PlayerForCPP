/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerでエフェクトオブジェクトを扱うためのクラスです。                  */
/* 通常、アプリケーションからは本クラスを使用して個々のエフェクトオブジェクト */
/* を操作・描画します（とは言え、エフェクトオブジェクトを単体で操作する機会は */
/* そんなに多くないと思うので、原則はアニメーションのエンティティから子として */
/* 呼ばれる場合が大半のはずです）。                                           */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include "../Library/ss6player_control.h"
#include "../Library/ss6player_control_effect.h"
#include "../Library/ss6player_data_project.h"
#include "../Library/ss6player_utility.h"

#include "../Library/ss6player_utility_random_xorshift32.h"

#include <vector>
#include <time.h>

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* エンティティ（アニメーションオブジェクト）管理 */
class EntityEffect :
	public Library::Control::Root
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
	/* 基底クラス型 */
	typedef Library::Control::Root base;

protected:
public:
	/* 使用する乱数型 */
	typedef Library::Utility::Random::XorShift32 RandomGenerator;

	/* 各種コールバック関数型 */
	/* MEMO: SS6Player for Unityでは、Library::CallBack::Function～で定義されていたものです。 */
	/*       C++版では現時点では各Entityクラス内でで定義しています。                          */
	/* ********************************************************* */
	//! 再生終了コールバック
	/*!
	@param	entity
		再生終了したエフェクトオブジェクト
	@param	pointer
		コールバック設定時に指定した任意ポインタ

	@retval	なし

	エフェクトオブジェクト（エンティティ）の再生を終了したタイミング
		で発生するコールバックの関数型です。
	*/
	typedef bool (*TypeFunctionPlayEnd)(EntityEffect& entity, void* pointer);

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum Constants	{
		LIMIT_SUBEMITTER_DEPTH = 2,					/* 子エミッタ保持限界階層数 */
		LIMIT_SUBEMITTER_COUNT = 10,				/* 子エミッタ保持限界数 */
	};
	enum Defaults	{
		LIMIT_PARTICLEDRAW = 1024,					/* デフォルトのパーティクル最大描画数 */
	};
	enum FlagBitStatus	{
		VALID = 0x40000000,
		PLAYING = 0x20000000,
		PLAYING_INFINITY = 0x1000000,

		UPDATE_RATE_SCALELOCAL = 0x08000000,
		UPDATE_RATE_OPACITY = 0x04000000,

		CHANGE_TABLEMATERIAL = 0x00800000,
		CHANGE_CELLMAP = 0x00400000,

		CLEAR = 0x00000000,
	};

	/* ----------------------------------------------- Variables */
private:
	static Uint32 RandomKeyMakeID;

protected:
public:
	Library::Data::Project::Effect* DataEffect;										/* エフェクトデータ管理 */
	int IndexData;																	/* 現在のエフェクトデータ番号 */

	void* WorkAreaProject;															/* 追加ワークエリア（プロジェクト用）: ヒープハンドル */
	void* WorkAreaData;																/* 追加ワークエリア（データ用）: ヒープハンドル */

	/* MEMO: 「エフェクト」にはマルチトラック再生機能はありません。 */
	Library::Control::Effect ControlEffect;											/* 再生状態管理 */

	int LimitParticleDraw;															/* 最大パーティクル描画数 */
	int CountParticleDraw;															/* 描画中パーティクル数 */

	Uint32 /* FlagBitStatus */ Status;												/* 再生ステータス */

	/* MEMO: 経過時間管理がアニメーションほど複雑でないため、（Control::Trackを使用しないで）最低限の情報のみで実装しています。       */
	/*       ただし、アニメーション再生トラックのステータス情報を流用しています（同じ内容のステータスを多重定義するのは無駄なため）。 */
	Uint32 /* Library::Control::Animation::Track::FlagBitStatus */ StatusPlaying;
	Float32 TimePerFrame;															/* 1フレームの時間（1.0f==1秒） *//* ※FramePerSecondの逆値 */
	Float32 TimeElapsed;															/* 経過時間 */
	Float32 RateTime;																/* 時間経過倍率 */

	Float32 Frame;																	/* 現在フレーム位置 */
	Float32 FrameRange;																/* 再生対象フレーム範囲 */
	Float32 FramePerSecond;															/* 1秒あたりのフレーム数 */

	/* コールバック関係 */
	Library::Control::CallBack<TypeFunctionPlayEnd> CallBackPlayEnd;				/* 再生終了コールバック情報 */

	/* ----------------------------------------------- Functions */
private:
	/* MEMO: 継承体からは見えるようにしておきます。 */
	void CleanUp(void);
	void CleanUpWorkAreaProject(void);
	void CleanUpWorkAreaData(void);
	bool BootUpWorkAreaProject(int countTexture);
	bool BootUpWorkAreaData(void);
	void ShutDownWorkAreaProject(void);
	void ShutDownWorkAreaData(void);

	bool UpdateMain(	float timeElapsed,
						bool flagHideDefault,
						Library::KindMasking::Kind masking,
						Library::Control::Animation::ArgumentContainer& argumentUpdate,
						bool flagInitializeMatrixCorrection,
						bool flagPlanarization,
						const FPU::Matrix4x4& matrixParent
					);

	void RateSetScaleLocal(float scaleX, float scaleY);
	void RateSetOpacity(float rate);

	void DataRelease(void);

	bool ModelSetDraw(void);

protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	EntityEffect(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	本クラスを継承する場合を考慮して、仮想デストラクタになっています。
	*/
	virtual ~EntityEffect(void);

	/* ********************************************************* */
	//! ステータス関連
	/*!
	原則、外部からステータスを取得・設定する場合、本関数群を使用
		して行うようにしてください。
	※ただ、SS6Playerの内部処理以外からステータスを取得して何か
		特殊処理を行うことは推奨していません。
	*/
	inline bool StatusGetIsValid(void)
	{
		return(0 != (Status & FlagBitStatus::VALID));	/* ? true * false */
	}
	inline bool StatusGetIsPlaying(void)
	{
		return(0 != (Status & FlagBitStatus::PLAYING));	/* ? true * false */
	}
	inline bool StatusGetIsPlayingInfinity(void)
	{
		return(0 != (Status & FlagBitStatus::PLAYING_INFINITY));	/* ? true * false */
	}
	inline void StatusSetIsPlayingInfinity(bool flagSwitch)
	{
		Status = (true == flagSwitch) ? (FlagBitStatus)(Status | FlagBitStatus::PLAYING_INFINITY) : (FlagBitStatus)(Status & ~FlagBitStatus::PLAYING_INFINITY);
	}
	inline bool StatusGetIsUpdateRateScaleLocal(void)
	{
		return(0 != (Status & FlagBitStatus::UPDATE_RATE_SCALELOCAL));	/* ? true * false */
	}
	inline bool StatusGetIsUpdateRateOpacity(void)
	{
		return(0 != (Status & FlagBitStatus::UPDATE_RATE_OPACITY));	/* ? true * false */
	}
	inline bool StatusGetIsChangeTableMaterial(void)
	{
		return(0 != (Status & FlagBitStatus::CHANGE_TABLEMATERIAL));	/* ? true * false */
	}
	inline bool StatusGetIsChangeCellMap(void)
	{
		return(0 != (Status & FlagBitStatus::CHANGE_CELLMAP));	/* ? true * false */
	}

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効

	本クラスが描画用として使用可能かを返します。
	*/
	bool IsValid(void);

	/* ********************************************************* */
	//! 本クラスの起動（初期起動）
	/*!
	@param	project
		プロジェクトデータ（の管理クラス）
	@param	entityParent
		親のエンティティ
		nullptr == 自分が最親エンティティ（通常使用時）
		省略時: nullptr

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスにプロジェクトデータを設定して・起動します。
	本関数を使用後に下記の処理を追加で行うことでエフェクトが
		（AnimationPlay関数で）再生可能になります。
	- CountGetTexture関数とNameGetTexture関数を使用してテクスチャ
		を別途に読み込んでから、TextureSet関数でそれらを本クラス
		に割り当てる
	- DataSet関数で再生するエフェクトデータ（sseeに相当）を設定
		する

	通常では、entityParentは省略して（記述しないで）使用してくだ
		さい。
	※本値に有効なポインタが入る場合は、原則として子アニメーション
		の場合で、内部処理から呼ばれる場合専用です。
	*/
	bool BootUp(Project& project, Entity* entityParent=nullptr);

	/* ********************************************************* */
	//! 本クラスの終了（未使用化）
	/*!
	@param	なし

	@retval	なし

	本クラスの使用を終了し・解放可能な内部のワークなどを全て解放
		します。
	本関数終了後は（再度BootUpからの手順を行うことなしに）アニメー
		ションを扱うことはできません。
	また、本クラスをShutDown後であっても、使用していたテクスチャ群
		は裏で並列動作中の描画スレッドでGPUが使用している可能性が
		高いため、テクスチャなどの解放タイミングには注意してください。
		※描画で使用しているテクスチャを解放した場合、最悪GPUがハング
			アップします。
	*/
	void ShutDown(void);

	/* ********************************************************* */
	//! 再生状態の定常更新
	/*!
	@param	timeElapsed
		前のフレームからの時間経過
		本クラスに与える変形行列
		何も変形させない場合は単位行列を与えてください。
	@param	masking
		適用するマスキング種別

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスで再生中のアニメーションの状態を更新します。
	アニメーションを再生していない場合でも実行しても問題ありません
		（何もしないだけで、例外などが出ることはありません）。
	*/
	bool Update(float timeElapsed, Library::KindMasking::Kind masking);

	/* ********************************************************* */
	//! 描画関数
	/*!
	@param	commandList
		描画情報を書き込むためのコマンドリスト
	@param	matrixProjection
		描画に使用するプロジェクション行列
	@param	matrixView
		描画に使用するビュー行列
	@param	matrixWorld
		描画に使用するローカル→ワールド変換行列

	@retval	関数値
		更新されたコマンドリストの値
		※殆どのプラットフォームの場合引数と変わらないです。
		DrawCommandListInvalid == 失敗

	本クラスで再生中のアニメーションの状態を更新します。
	Updateの結果を描画コマンドリストに出力します。
	コマンドリスト生成の処理スレッドから呼び出してください。
	*/
	TypeDrawCommandList Draw(	TypeDrawCommandList commandList,
								const CPU::Matrix4x4& matrixProjection,
								const CPU::Matrix4x4& matrixView,
								const CPU::Matrix4x4& matrixWorld
						);

	/* ********************************************************* */
	//! エフェクトデータを設定（パーツモデルの設定）
	/*!
	@param	index
		エフェクトデータ番号
		-1 == 現在割り当たっているデータの解放

	@retval	関数値
		true == 成功
		false == 失敗

	エンティティで再生管理を行うエフェクトデータ（SS6のSSEEに相当）
		を設定します。

	実態として、エフェクトデータに定義されているエフェクトパーツ
		の管理情報（モデル情報）を元に本エンティティのエフェクト
		再生管理構造を設定します。
	本関数はエンティティ内部の構成を再構成しますので、メモリの再
		配置と多少のランニングコストがかかることに注意してくださ
		い。
	*/
	bool DataSet(int index);

	/* ********************************************************* */
	//! エフェクトデータを取得
	/*!
	@param	なし

	@retval	関数値
		現在割り当たっているエフェクトデータ番号
		-1 == 設定されていない

	現在エンティティに割り当たっているエフェクトデータ（SS6のSSAE
		に相当）を取得します。
	*/
	inline int DataGet(void)
	{
		return(IndexData);
	}

	/* ********************************************************* */
	//! 現在設定しているエフェクトで使用する最大描画頂点数を取得
	/*!
	@param	countFragmentMax
		最大マテリアル（PrimitiveBatcher::Fragment）数の格納数

	@retval	関数値
		最大頂点数
		-1 == 失敗
	@retval	countFragmentMax
		最大マテリアル（PrimitiveBatcher::Fragment）数
		※関数値が-1の場合、値は不定になります。

	現在設定されているエフェクトデータで描画使用される最大の頂点数
		を計算します。
	※現在仕様として、エフェクトの最大頂点数はLimitParticleDrawの
		設定値を元に計算されますので、データを計算して求めていま
		せん。
		LimitParticleDrawが0の場合は設定がデフォルトとみなして、
		Defaults::LIMIT_PARTICLEDRAWを返します。

	publicにはなっていますが、原則として外部から本関数を使用しない
		でください。
	※PackSetの際のPrimitiveBatcherの初期化処理用に特化した算出方法
		であることと・本関数の内部処理ですでに踏んでなくてはならない
		初期化工程があるため、それらを満たしていない場合には正常動作
		しません。
	*/
	int CountGetVertexDraw(int* countFragmentMax);

	/* MEMO: 以下の関数群はpublicにしてありますが、原則としてLibrary::Control::Animation::Partsの内部処理のみから使用してください（後にprivateにするかも）。 */
	inline void TimeSkip(Float32 time, bool flagReverseParent)
	{
		TimeElapsed = time;
	}
	inline Float32 TimeGetFramePosition(int frame)
	{
		return((Float32)frame * TimePerFrame);
	}
	inline void SeedOffsetSet(Uint32 seed)
	{
		ControlEffect.SeedOffsetSet(seed);
	}

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:
	/* ********************************************************* */
	//! 乱数発生器の作成
	/*!
	@param	なし

	@retval	関数値
		乱数発生器のインスタンス（ポインタ）
		nullptr == エラー

	エフェクトの内部処理で使用する乱数発生器を新しく作成します。
	乱数発生器はLibrary::Utility::Random::Generatorの継承体である
		必要があり、同クラスの継承体であればエフェクトの諸計算で
		使用している乱数の規則性を変更することが可能です。
	※Library::Utility::Random::Generatorは純粋仮想クラスですので、
		個々の継承クラスで仕様を実装する必要があります。
	*/
	static Library::Utility::Random::Generator* InstanceCreateRandom(void);

	/* ********************************************************* */
	//! 乱数発生器の解放
	/*!
	@param	random
		乱数発生器のポインタ

	@retval	なし

	InstanceCreateRandom関数で作成した乱数発生器を解放します。
	*/
	static void InstanceReleaseRandom(Library::Utility::Random::Generator* random);

	/* ********************************************************* */
	//! 乱数キーの作成
	/*!
	@param	なし

	@retval	関数値
		乱数キー

	エフェクトで使用する乱数キーを作成します（特に「乱数種を固定
		していないエフェクトのデータ」で使用され、設置時やループ時
		などにキーが更新され、再生に揺らぎを持たせています）。
	*/
	static inline Uint32 KeyCreateRandom(void)
	{
		RandomKeyMakeID++;

		/* MEMO: 現在のUTC秒数（起点はエポック秒）から乱数キーを作成しています。 */
		time_t timeNow;
		timeNow = time(nullptr);
		
		return(RandomKeyMakeID + (Uint32)timeNow);
	}

	/* 各機能部位ユーティリティ群 */
	/* MEMO: EntityEffectクラスは規模はさほど大きくはならないはずではありますが、  */
	/*       Entityクラスと実装を合わせています。                                  */
	/*       各部位のファイルを間違ってincludeした場合の影響を最小限にするために、 */
	/*       「__SS6PLAYER_ENTITY_CLASS__」ラベル定義時以外は内容が無効（空ファイ  */
	/*       ルと同等）になるようにしてありますが……下記ファイル群は単独でinclude */
	/*       しないでください。                                                    */
	#define __SS6PLAYER_ENTITY_CLASS__
	#include "ss6player_entity_effect_function_animation.h"	/* アニメーション操作関係 */
	#include "ss6player_entity_effect_function_cell.h"		/* セル操作関係 */
	#include "ss6player_entity_effect_function_parts.h"		/* パーツ操作関係 */
	#include "ss6player_entity_effect_function_misc.h"		/* その他（分類前含む）関係 */
	#undef __SS6PLAYER_ENTITY_CLASS__

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class SpriteStudio6::Library::Control::Animation::Parts;
	friend class SpriteStudio6::Library::Control::Animation::Draw;
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
