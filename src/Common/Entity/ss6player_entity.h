/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerでアニメーションオブジェクトを扱うためのクラスです。              */
/* 通常、アプリケーションからは本クラスを使用して個々のアニメーションオブジェ */
/* クトを操作・描画します。                                                   */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include "../Library/ss6player_control.h"
#include "../Library/ss6player_data_project.h"
#include "../Library/ss6player_utility.h"
#include "../PrimitiveBatcher/ss6player_primitive_batcher.h"

#include <vector>

/* 前方宣言 */
namespace SpriteStudio6	{
class Project;
class Sequencer;
class EntityEffect;
}	/* SpriteStudio6 */

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* エンティティ（アニメーションオブジェクト）管理 */
class Entity :
	public Library::Control::Root
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
	/* 基底クラス型 */
	typedef Library::Control::Root base;

	/* 前回再生情報キャッシュ */
	class InformationPlay	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		Library::Data::Project::Animation* DataAnimation;	/* 現在パーツ構成の元になっているアニメーションデータ */

		bool FlagSetInitial;
		bool FlagStopInitial;

		std::string NameAnimation;
		int IndexAnimation;
		bool FlagPingPong;
		std::string LabelStart;
		int FrameOffsetStart;
		std::string LabelEnd;
		int FrameOffsetEnd;
		int Frame;
		int TimesPlay;
		Float32 RateTime;

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			DataAnimation = nullptr;

			FlagSetInitial = false;
			FlagStopInitial = false;

			NameAnimation.clear();
			IndexAnimation = -1;
			FlagPingPong = false;
			LabelStart.clear();
			FrameOffsetStart = 0;
			LabelEnd.clear();
			FrameOffsetEnd = 0;
			Frame = 0;
			TimesPlay = 0;
			RateTime = 1.0f;
		}

		InformationPlay(void)
		{
			CleanUp();
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

protected:
public:
	/* 各種コールバック関数型 */
	/* MEMO: SS6Player for Unityでは、Library::CallBack::Function～で定義されていたものです。 */
	/*       C++版では現時点では各Entityクラス内でで定義しています。                          */
	/* ********************************************************* */
	//! 再生終了コールバック
	/*!
	@param	entity
		再生終了したアニメーションオブジェクト
	@param	pointer
		コールバック設定時に指定した任意ポインタ

	@retval	なし

	アニメーションオブジェクト（エンティティ）の全てのトラックが
		アニメーション再生を終了したタイミングで発生するコール
		バックの関数型です。
	※SS6Player for Unityの再生終了コールバックと同義ですが、関数
		の戻り値でのオブジェクトの自己削除機能はなくなっています。
		これは、C++版の場合、描画スレッドやリソース管理の絡みから
		各アプリケーションで解放の都合が変わると思われるためです。
	*/
	typedef void (*TypeFunctionPlayEnd)(Entity& entity, void* pointer);

	/* ********************************************************* */
	//! トラック再生終了（トラックトランジションの終了含む）コールバック
	/*!
	@param	entity
		再生終了したアニメーションオブジェクト
	@param	indexTrackPlay
		再生終了したトラック番号（マスタ）
	@param	indexTrackSlave
		再生終了したトラック番号（スレーブ）
	@param	indexAnimation
		再生終了したアニメーション番号（マスタ）
	@param	indexAnimationSlave
		再生終了したアニメーション番号（スレーブ）

	@retval	なし

	アニメーションオブジェクト（エンティティ）の再生トラックで
		再生していたアニメーションが終了した時に発生するコール
		バックの関数型です。
	複数トラックを使用しての、「アニメーション合成」「トラック
		トランジション」を使用している場合の、特定トラックの
		再生終了を検知する場合に使用します。
	indexTrackSlave・indexAnimationSlaveは、トラックトランジション
		を使用している際にトランジション終了のコールバックとして
		発生した場合に、トランジションのスレーブ側トラック情報が
		格納されてきます（通常のトラック再生終了コールバックの
		場合は、この2値は常に-1です）。
	*/
	typedef void (*TypeFunctionControlEndTrackPlay)(Entity& entity, void* pointer, int indexTrackPlay, int indexTrackSlave, int indexAnimation, int indexAnimationSlave);

	/* ********************************************************* */
	//! ユーザデータコールバック
	/*!
	@param	entity
		ユーザデータを検知したアニメーションオブジェクト
	@param	nameParts
		ユーザーデータを検知したパーツ名
	@param	idParts
		ユーザーデータを検知したパーツID
	@param	indexAnimation
		再生中のアニメーション番号
	@param	frameDecode
		デコードした時の再生フレーム
	@param	frameKeyData
		ユーザデータが設置されているフレーム
	@param	userData
		検知したユーザデータ
	@param	flagWayBack
		アニメーションが往復再生されていた場合の往復情報
		false == 往路
		true == 復路

	@retval	なし

	アニメーション再生中にユーザデータを検知した時に発生する
		コールバックの関数型です。

	前後の処理フレーム状態によってフレームスキップが起こって
		いた場合、framdeDecodeとframeKeyDataが異なる場合があ
		ります。

	フレームスキップが起こっていた場合、フレームスキップ区間
		内に設置されているコールバックを全て別々に通知します。
	複数パーツにユーザデータが設置されていた場合も、設置され
		ている全てのパーツで別々に通知します。
	通知の順序仕様の整頓仕様は下記になります。
	1. パーツIDが若い順
	1. パーツIDが若い順
	2. 同一パーツ内の、再生対象フレーム区間内に存在するユーザ
		データ（再生方向に準じてフレーム順）
	※検知フレーム位置順優先ではないので、その旨注意してください。

	また、本コールバック関数内では、原則としてアニメーションの
		切替など再生状態に影響を与える処理をしないでください
		（アニメーションの再生更新中に発生するため、アニメー
			ションの状態が不安定です）。
	*/
	typedef void (*TypeFunctionUserData)(Entity& entity, void* pointer, const char* nameParts, int idParts, int indexAnimation, int frameDecode, int frameKeyData, const Library::Data::Animation::Attribute::UserData& userData, bool flagWayBack);

	/* ********************************************************* */
	//! シグナルコールバック
	/*!
	@param	entity
		シグナルを検知したアニメーションオブジェクト
	@param	nameParts
		シグナルを検知したパーツ名
	@param	idParts
		シグナルを検知したパーツID
	@param	indexAnimation
		再生中のアニメーション番号
	@param	frameDecode
		デコードした時の再生フレーム
	@param	frameKeyData
		シグナルが設置されているフレーム
	@param	signal
		検知したシグナル
	@param	flagWayBack
		アニメーションが往復再生されていた場合の往復情報
		false == 往路
		true == 復路

	@retval	なし

	アニメーション再生中にシグナルを検知した時に発生するコー
		ルバックの関数型です。

	前後の処理フレーム状態によってフレームスキップが起こって
		いた場合、framdeDecodeとframeKeyDataが異なる場合があ
		ります。

	フレームスキップが起こっていた場合、フレームスキップ区間
		内に設置されているコールバックを全て別々に通知します。
	複数パーツにシグナルが設置されていた場合も、設置されている
		全てのパーツで別々に通知します。
	通知の順序仕様の整頓仕様は下記になります。
	1. パーツIDが若い順
	2. 同一パーツ内の、再生対象フレーム区間内に存在するシグナ
		ル（再生方向に準じてフレーム順）
	※検知フレーム位置順優先ではないので、その旨注意してください。

	また、本コールバック関数内では、原則としてアニメーションの
		切替など再生状態に影響を与える処理をしないでください
		（アニメーションの再生更新中に発生するため、アニメー
			ションの状態が不安定です）。

	signalは（ユーザーデータのように）格納データからアクセス用
		に値のバックアップを取らず、直接アニメーションデータの
		内容を覗いているので、値の取得方法がすべて関数になって
		います。
	※シグナルはデータそのものが大きいため、複製のためのオーバ
		ヘッドを忌避したためです。
	*/
	typedef void (*TypeFunctionSignal)(Entity& entity, void* pointer, const char* nameParts, int idParts, int indexAnimation, int frameDecode, int frameKeyData, const Library::Data::Animation::Attribute::Signal& signal, bool flagWayBack);

	/* MEMO: アニメーションの再生時間上書用コールバックは削除（C++版だとUpdateに更新時間を渡すので必要がないため）。 */
	/* MEMO: コライダ関係は未対応 */

	/* ----------------------------------------------- Enums & Constants */
private:
	/* 再生時ステータス */
	enum FlagBitStatus : Uint32	{
		VALID = 0x40000000,
		PLAYING = 0x20000000,

		UPDATE_RATE_SCALELOCAL = 0x08000000,
		UPDATE_RATE_OPACITY = 0x04000000,

		CHANGE_TABLEMATERIAL = 0x00800000,
		CHANGE_CELLMAP = 0x00400000,

		ANIMATION_SYNTHESIZE = 0x00080000,

		CLEAR = 0x00000000,
	};

protected:
public:
	/* 各種初期値 */
	enum Defaults	{
		LIMIT_TRACK = 1,							/* 最初に設置するアニメーション再生トラック数 */
		COUNT_TEXTURE_OFFSCREEN = 0,				/* オフスクリーンテクスチャ数 *//* ※予約 */
	};

	/* ----------------------------------------------- Variables */
private:
	Uint32 /* FlagBitStatus */ Status;													/* 再生ステータス */

	Library::Data::Project::Animation* DataAnimation;									/* アニメーションデータ管理 */
	int IndexPack;																		/* 現在のアニメーショパック番号（=パーツモデル） */

	/* 同一プロジェクト内共通ワーク */
	/* MEMO: 下記の実体もWorkAreaProjectの中に含まれています。                                    */
	/*       - SpriteStudio6::Texture** Texture;                                                  */
	/*       - Library::Control::CallBack<TypeFunctionControlEndTrackPlay>* CallBackPlayEndTrack; */
	void* WorkAreaProject;																/* ヒープハンドル */
	int CountTrack;																		/* 再生トラック数 */
	InformationPlay* TableInformationPlay;												/* 前回再生情報キャッシュ */
	Library::Control::Animation::Track* TableControlTrack;								/* 再生トラックコントロール */

	/* アニメーションパック依存ワーク */
	void* WorkAreaPack;																	/* ヒープハンドル */
	int CountParts;
	Library::Control::Animation::Parts* TableControlParts;								/* パーツ制御 */
	int CountPartsDraw;
	Library::Control::Animation::Draw* TableControlPartsDraw;							/* 描画パーツワーク */
	void* WorkAreaPackAdditional;														/* 描画パーツ用追加ワーク（WorkAreaPack内に存在） */

	int CountEntityChildInstance;
	Entity* ListEntityChildInstance;													/* 子アニメーション: インスタンス */
	int CountEntityChildEffect;
	EntityEffect* ListEntityChildEffect;												/* 子アニメーション: エフェクト */

	/* MEMO: DrawとRenderは異なるスレッドで扱われるため、スレッドセーフ的に別領域として */
	/*       随時値を移し替えています。                                                 */
	/*       ※ダブルバッファにしても良いかも……。                                     */
	/* MEMO: 静的配列にしてワークの中に含みたいのだが、std::sortがかなり高速なので、現在はstd::vectorにしてあります。 */
	/*       ※後日、高速なクィックソートを自作するのも手かとは思いますが……。                                       */
	std::vector<int> ListPartsDraw;														/* 描画処理するパーツリスト（優先度と混ぜ込まれたキー値） */
	std::vector<int> ListPartsPreDraw;													/* 描画前描画処理するパーツリスト（優先度と混ぜ込まれたキー値） *//* ※マスク対応での描画前ステンシルへの描画 */

protected:
public:
	/* コールバック関係 */
	/* MEMO: トラック再生終了コールバックについては、本来トラック制御クラス内に入れておく */
	/*       方が実装構造としてはスマートですが、外部からの設定のしやすさなどの点で意図的 */
	/*       に外部（本エンティティ直下で他のコールバック群と同位）に設置しています。     */
	Library::Control::CallBack<TypeFunctionUserData> CallBackUserData;					/* ユーザデータ検出コールバック情報 */
	Library::Control::CallBack<TypeFunctionSignal> CallBackSignal;						/* シグナル検出コールバック情報 */
	Library::Control::CallBack<TypeFunctionPlayEnd> CallBackPlayEnd;					/* 再生終了コールバック情報 */
	Library::Control::CallBack<TypeFunctionControlEndTrackPlay>* CallBackPlayEndTrack;	/* トラック再生終了コールバック情報 *//* ※順序は再生トラック順です */

	/* 静的変数群 */
	static int LimitTrack;																/* デフォルトで使用される再生トラック数 */

	/* ----------------------------------------------- Functions */
private:
protected:
	/* MEMO: 継承体からは見えるようにしておきます。 */
	void CleanUp(void);
	void CleanUpWorkAreaProject(void);
	void CleanUpWorkAreaPack(void);
	bool BootUpWorkAreaProject(int countTrack, int countTexture, int countTextureOffscreen);
	bool BootUpWorkAreaPack(void);
	void ShutDownWorkAreaProject(void);
	void ShutDownWorkAreaPack(void);
	bool UpdateMain(	float timeElapsed,
						bool flagHideDefault,
						Library::KindMasking::Kind masking,
						Library::Control::Animation::ArgumentContainer& argumentUpdate,
						bool flagInitializeMatrixCorrection,
						bool flagPlanarization,
						const FPU::Matrix4x4& matrixParent
				);
	bool TrackChangeSlaveToMaster(int indexTrackMaskter, int indexTrackSlave);
	void RateSetScaleLocal(float scaleX, float scaleY);
	void RateSetOpacity(float rate);

	void PackRelease(void);

	bool ModelSetDraw(void);

public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Entity(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	本クラスを継承する場合を考慮して、仮想デストラクタになっています。
	*/
	virtual ~Entity(void);

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効

	本クラスが描画用として使用可能かを返します。
	具体的には、
	- BootUpが済んでいる
	- PackSetされている
	ことがtrueを返す条件になります。
	*/
	bool IsValid(void);

	/* ********************************************************* */
	//! 本クラスの起動（初期起動）
	/*!
	@param	project
		プロジェクトデータ（の管理クラス）
	@param	countTrack
		初期状態で設置する再生トラック数
		省略時: 1
	@param	entityParent
		親のエンティティ
		nullptr == 自分が最親エンティティ（通常使用時）
		省略時: nullptr

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスにプロジェクトデータを設定して・起動します。
	本関数を使用後に下記の処理を追加で行うことでアニメーションが
		（AnimationPlay関数で）再生可能になります。
	- CountGetTexture関数とNameGetTexture関数を使用してテクスチャ
		を別途に読み込んでから、TextureSet関数でそれらを本クラス
		に割り当てる
	- PackSet関数で再生するアニメーションパック（ssaeに相当）を
		設定する

	通常では、entityParentは省略して（記述しないで）使用してくだ
		さい。
	※本値に有効なポインタが入る場合は、原則として子アニメーション
		の場合で、内部処理から呼ばれる場合専用です。
	*/
	bool BootUp(Project& project, int countTrack=1, Entity* entityParent=nullptr);

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
		描画するためのプロジェクション行列
	@param	matrixView
		描画するためのビュー行列
	@param	matrixWorld
		描画するためのローカル→ワールド変換行列

	@retval	関数値
		更新されたコマンドリストの値
		※殆どのプラットフォームの場合引数と変わらないです。
		DrawCommandListInvalid == 失敗

	本クラスで再生中のアニメーションの状態を描画します。
	Updateの結果を描画コマンドリストに出力します。
	コマンドリスト生成の処理スレッドから呼び出してください。

	一見、本関数を複数回呼び出すことで1つのEntityを複数回描画する
		ことができますが……その（増殖描画したい）場合については、
		EntityReplicateクラスを使用するようにしてください。
	※シェーダ定数の関係から本関数を直接呼び出すと、描画不良やGPU
		とのメモリ競合を起こしたりしますので、本関数を1描画区間で
		複数回呼ぶことは行わないでください。

	関数値でDrawCommandListInvalidが返ってくる場合は、余程の場合で
		あり・描画しないでエラーを回避できる状態については（全体的な
		描画のパイプラインを阻害しないために）commandListの内容を
		そのまま返します（例えば未起動状態の本クラスに対して本関数を
		しようしたような場合については、nullptrを返さずに・コマンド
		リストを更新せずに・commandListをそのまま返します）。
	この場合の余程……の程度ですが、データや再生の不整合などで、描画
		コマンドの生成に支障があるような場合です。
	*/
	TypeDrawCommandList Draw(	TypeDrawCommandList commandList,
								const CPU::Matrix4x4& matrixProjection,
								const CPU::Matrix4x4& matrixView,
								const CPU::Matrix4x4& matrixWorld
						);

	/* ********************************************************* */
	//! アニメーションパックを設定（パーツモデルの設定）
	/*!
	@param	index
		アニメーションパック番号
		-1 == 現在割り当たっているパックの解放

	@retval	関数値
		true == 成功
		false == 失敗

	エンティティで再生管理を行うアニメーションパック（SS6のSSAEに
		相当）を設定します。

	実態として、アニメーションパックに定義されているアニメーション
		パーツの管理情報（モデル情報）を元に本エンティティのアニ
		メーションパーツ構造を設定します。
	本関数はエンティティ内部の構成を再構成しますので、メモリの再
		配置と多少のランニングコストがかかることに注意してくださ
		い。
	*/
	bool PackSet(int index);

	/* ********************************************************* */
	//! アニメーションパックを取得
	/*!
	@param	なし

	@retval	関数値
		現在割り当たっているアニメーションパック番号
		-1 == 設定されていない

	現在エンティティに割り当たっているアニメーションパック（SS6の
		SSAEに相当）を取得します。
	*/
	inline int PackGet(void)
	{
		return(IndexPack);
	}

	/* ********************************************************* */
	//! アニメーションパックで使用する最大描画頂点数を取得
	/*!
	@param	countFragmentMax
		最大マテリアル（PrimitiveBatcher::Fragment）数の格納数
	@param	index
		アニメーションパック番号

	@retval	関数値
		最大頂点数
		-1 == 失敗
	@retval	countFragmentMax
		最大マテリアル（PrimitiveBatcher::Fragment）数
		※関数値が-1の場合、値は不定になります。

	現在設定されているアニメーションプロジェクト内の指定アニメー
		ションパックで描画使用される最大の頂点数を計算します。
	子のインスタンス・エフェクトの分も加算して計算します。

	publicにはなっていますが、原則として外部から本関数を使用しない
		でください。
	※PackSetの際のPrimitiveBatcherの初期化処理用に特化した算出方法
		であることと・本関数の内部処理ですでに踏んでなくてはならない
		初期化工程があるため、それらを満たしていない場合には正常動作
		しません。
	*/
	int CountGetVertexDraw(int* countFragmentMax, int index);

	/* 各機能部位ユーティリティ群 */
	/* MEMO: Entityクラスは規模が大きいので見通しを良くする為に、各機能群でソース  */
	/*       を分けて実装しています。                                              */
	/*       各部位のファイルを間違ってincludeした場合の影響を最小限にするために、 */
	/*       「__SS6PLAYER_ENTITY_CLASS__」ラベル定義時以外は内容が無効（空ファイ  */
	/*       ルと同等）になるようにしてありますが……下記ファイル群は単独でinclude */
	/*       しないでください。                                                    */
	#define __SS6PLAYER_ENTITY_CLASS__
	#include <ss6player_entity_function_animation.h>	/* アニメーション操作関係 */
	#include <ss6player_entity_function_cell.h>			/* セル操作関係 */
	#include <ss6player_entity_function_parts.h>		/* パーツ操作関係 */
	#include <ss6player_entity_function_track.h>		/* 再生トラック操作関係 */
	#include <ss6player_entity_function_misc.h>			/* その他（分類前含む）関係 */
	#undef __SS6PLAYER_ENTITY_CLASS__

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:

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
