/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerで複数のアニメーションを連結して再生したりするためのシーケンサを  */
/* 扱うためのクラスです。                                                     */
/* データだけでなくEntityそのものを制御対象とします（一応Entityと組み合わせて */
/* 使う並列コンポーネントに見えますが、実態としてはEntityを制御するための上位 */
/* コンポーネントになります）。                                               */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include "../Library/ss6player_data.h"
#include "../Library/ss6player_data_project.h"
#include "../Entity/ss6player_entity.h"

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* シーケンサ（シーケンスデータ再生）管理 */
class Sequencer
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	struct DataStep
	{
		int IndexAnimationPack;						/* 再生アニメーションパック番号 */
		int IndexAnimation;							/* 再生アニメーション番号 */
		int CountPlay;								/* 再生回数 */

		/* ********************************************************* */
		//! 本クラスが有効か？
		/*!
		@param	なし

		@retval	なし

		本クラスが再生可能な状態かを返します。
		*/
		inline void CleanUp(void)
		{
			IndexAnimationPack = -1;
			IndexAnimation = -1;
			CountPlay = 0;
		}

		/* ********************************************************* */
		//! 本クラスが有効か？
		/*!
		@param	なし

		@retval	関数
			true == 有効（有効なアニメーション設定）
			false == 無効（再生しないアニメーション設定）

		本クラスに設定されている値が再生可能な状態かを返します。
		ただし、設定されている値がアニメーションデータに対して正しいか
			（例えばパック番号やアニメーション番号が有効値範囲内にある
			か……など）については、頓着しませんので、注意してください。
		*/
		inline bool IsValid(void)
		{
			if(	(0 > IndexAnimationPack)
				|| (0 > IndexAnimation)
				|| (0 == CountPlay)
			)	{
				return(false);
			}

			return(true);
		}
	};

	/* ********************************************************* */
	//! 再生終了コールバック
	/*!
	@param	sequencer
		再生終了したシーケンサオブジェクト
	@param	pointer
		コールバック設定時に設定した任意ポインタ

	@retval	なし

	本クラスで指定されたシーケンスが再生終了すると発生するコール
		バックの関数型です。

	コールバック関数の中でアニメーションの再設定などは行わないで
		ください（再生状態が不安定になる場合があります）。

	コールバックのタイミングは、Sequencer::Updateの中です。
	*/
	typedef void (*TypeFunctionPlayEnd)(Sequencer& sequencer, void* pointer);

	/* ********************************************************* */
	//! シーケンスデコード時コールバック
	/*!
	@param	data
		デコードされた再生情報（入出力）
	@param	sequencer
		再生終了したシーケンサオブジェクト
	@param	step
		シーケンスデータ内の何番目のデータか？
	@param	pointer
		コールバック設定時に設定した任意ポインタ

	@retval	関数値
		ジャンプするステップ番号
		-1 == ステップを変更しない

	シーケンスの新しいステップをデコードした際に、新しい再生情報が
		適用される寸前でコールバックを行います。

	コールバック関数内でdataの内容を変更して返すことで、次に再生する
		アニメーションの内容を強制変更することができます。
	dataをCleanUp()して返すと、その時点でアニメーションの再生を停止
		することができます（一時停止ではないので……復帰するには、
		Play関数から呼び出しなおす必要があります）。
	dataを変更した場合、その有効性（正しく再生可能な情報であるか？）
		の内容については頓着しませんので、不正な値を設定した場合
		例外などが出る可能性が高いので、その旨注意してください。

	コールバック関数が0以上の関数値を返すと、そのステップに強制的
		に移動します。
	dataの内容を変更すると同時に・関数値も返した場合、関数値の方が
		有線され・dataへの変更設定は無視されます。

	コールバックのタイミングは、Sequencer::Updateの中です。
	*/
	typedef int (*TypeFunctionDecodeStep)(DataStep& data, Sequencer& sequencer, int step, void* pointer);

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 定数群 */
	enum Constant	{
		COUNT_ENTITY = 2,							/* 内部的に使用するエンティティ個数 *//* ※廃棄タイミングの関係からダブルバッファになっています */
	};

	/* 再生時ステータス */
	enum FlagBitStatus : Uint32	{
		VALID = 0x40000000,
		PLAYING = 0x20000000,
		PAUSING = 0x10000000,

		STEP_UPDATE = 0x08000000,

		CLEAR = 0x00000000,
	};

	/* ----------------------------------------------- Variables */
private:
	Uint32 /* FlagBitStatus */ Status;										/* 再生ステータス */

protected:
public:
	SpriteStudio6::Project* DataProject;									/* 使用中のプロジェクト */

	Library::Data::Project::Sequence* DataSequencePack;						/* 使用中のシーケンスパック */
	Library::Data::Sequence::Body DataSequence;								/* 使用中のシーケンス */
	int IndexStep;															/* 現在の再生中のデータカーソル */

	bool FlagHideForce;														/* 強制非表示 */
	Float32 RateTime;														/* 再生速度 */

	/* MEMO: 一見するとエンティティを捨ててしまうと描画資産が捨てられてしまい */
	/*       GPUハングアップなどを招きかねないように見えますが……            */
	/*       描画バッファに使用しているBuffer::GPU～が、基底処理でバッファ毎  */
	/*       遅延解放処理を行うので、現実として問題ありません。               */
	SpriteStudio6::Entity Entity;											/* 再生用エンティティ */

	Library::Control::CallBack<TypeFunctionPlayEnd> CallBackPlayEnd;		/* 再生終了時コールバック関数 */
	Library::Control::CallBack<TypeFunctionDecodeStep> CallBackDecodeStep;	/* シーケンスデコード時コールバック関数 */

	/* ----------------------------------------------- Functions */
private:
	void CleanUp(void);
	bool StepProgress(void);
	bool DataGetStep(DataStep* data, int indexStep);

	static void FunctionCallBackPlayEnd(SpriteStudio6::Entity& entity, void* pointer);

protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Sequencer(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	本クラスを継承する場合を考慮して、仮想デストラクタになっています。
	*/
	virtual ~Sequencer(void);

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効

	本クラスが再生可能な状態かを返します。
	*/
	bool IsValid(void);

	/* ********************************************************* */
	//! 本クラスの起動（実行用初期化）
	/*!
	@param	project
		プロジェクト
	@param	countTrack
		再生用のEntityに持たせる再生トラック数
		省略時: 1
		※通常は省略して使用してください。

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスにアニメーションデータを設定して・起動します。
	正確には、本関数を使用後にCountGetTexture関数とNameGetTexture関数
		を使用してテクスチャを別途に読み込んでから、TextureSet関数で
		それらを本クラスに割り当ててやることでアニメーションが再生可能
		になります。
	*/
	bool BootUp(SpriteStudio6::Project& project, int countTrack=1);

	/* ********************************************************* */
	//! 本クラスの終了（未使用化）
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスの使用を終了し・解放可能な内部のワークなどを全て解放
		します。
	本関数終了後は（再度BootUpからの手順を行うことなしに）シーケン
		ス再生を扱うことはできません。
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

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスで再生中のシーケンスアニメーションの状態を更新します。
	アニメーションを再生していない場合でも実行しても問題ありません
		（何もしないだけで、例外などが出ることはありません）。

	本関数を実行した場合、本クラスが保持している再生用のエンティ
		ティを個別にUpdateする必要はありません（むしろ行わないで
		ください）。
	*/
	bool Update(float timeElapsed);

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

	本関数を実行した場合、本クラスが保持している再生用のエンティ
		ティを個別にDrawする必要はありません（むしろ行わないで
		ください）。
	*/
	TypeDrawCommandList Draw(	TypeDrawCommandList commandList,
								const CPU::Matrix4x4& matrixProjection,
								const CPU::Matrix4x4& matrixView,
								const CPU::Matrix4x4& matrixWorld
						);

	/* ********************************************************* */
	//! シーケンスパック数の取得
	/*!
	@param	なし

	@retval	関数値
		シーケンスパック数
		-1 == 失敗

	現在設定されているプロジェクトに含まれるシーケンスパック
		（SpriteStudio6上のSSQEに相当）数を取得します。
	*/
	int CountGetPack(void);

	/* ********************************************************* */
	//! 名前からシーケンスパックのインデックスを取得
	/*!
	@param	name
		シーケンスパック名

	@retval	関数値
		シーケンスパックの番号
		-1 == 失敗

	現在設定されているプロジェクトに含まれるシーケンスパック
		（SpriteStudio6上のSSQEに相当）から指定名を持つシーケンス
		パックのインデックスを取得します。

	インデックスは「0～(CountGetPack() - 1)」の範囲の値になります。
	*/
	int IndexGetPack(const char* name);

	/* ********************************************************* */
	//! シーケンスパックを設定
	/*!
	@param	index
		シーケンスパック番号

	@retval	関数値
		true == 成功
		false == 失敗

	現在設定されているプロジェクトに含まれるシーケンスパック
		の指定番号のシーケンスパックを本クラスに設定します。

	実際のシーケンスを再生（Play関数を実行）する以前に、本関数で
		使用するシーケンスパックを設定しておく必要があります。
	*/
	bool PackSet(int index);

	/* ********************************************************* */
	//! シーケンスパック内のシーケンス数を取得
	/*!
	@param	なし

	@retval	関数値
		シーケンス数
		-1 == 失敗

	現在設定されているプロジェクト内シーケンスパックに含まれる
		シーケンス数を取得します（SSQE内の各シーケンスに相当）。
	*/
	int CountGetSequence(void);

	/* ********************************************************* */
	//! 名前からシーケンスのインデックスを取得
	/*!
	@param	name
		シーケンス名

	@retval	関数値
		シーケンスの番号
		-1 == 失敗

	現在設定されているプロジェクト内シーケンスパックに含まれる
		指定名を持つシーケンスのインデックスを取得します。

	本関数で得られた値は、Play関数などに与えるシーケンス番号に
		なります。
	本関数で得られる値は、あくまでシーケンスパック内のインデック
		スとなります（設定しているシーケンスパックを本値取得後に
		切り替えた場合、Play関数で再生されるシーケンスは、新規に
		設定したシーケンスパック内の該当インデックスのシーケンス
		になります）。

	インデックスは「0～(CountGetSequence() - 1)」の範囲の値になり
		ます。
	*/
	int IndexGetSequence(const char* name);

	/* ********************************************************* */
	//! 指定シーケンスのステップ数を取得
	/*!
	@param	indexSequence
		シーケンス番号

	@retval	関数値
		ステップ数
		-1 == 失敗

	現在設定されているプロジェクト内シーケンスパックに含まれる
		指定シーケンスの持つステップ数（データ中にいくつの再生命令
		が記載されているか）を取得します。
	*/
	int CountGetStep(int indexSequence);

	/* ********************************************************* */
	//! シーケンスの再生開始
	/*!
	@param	indexSequence
		シーケンス番号
	@param	rateTime
		再生速度係数（1.0f: 等速）
	@param	step
		開始するステップ番号

	@retval	関数値
		true == 成功
		false == 失敗

	シーケンスを再生開始します。
	*/
	bool Play(int indexSequence, float rateTime=1.0f, int step=0);

	/* ********************************************************* */
	//! シーケンスの再生停止
	/*!
	@param	flagJumpEndStep
		true == 再生停止状態をシーケンスの末尾のシーケンスにする
		false == 現状位置で停止する
	@param	flagJumpEndFrame
		true == 再生停止状態をステップの末尾にする
		false == 現状位置で停止する

	@retval	なし

	再生しているシーケンスの再生を停止します。
	（一時停止ではないので）再生停止からの復帰はできません。

	本関数で再生を停止した場合、再生終了コールバックは発行され
		ません（FunctionPlayEndに設定されている関数は呼ばれません）。

	flagJumpEndStepがtrueの場合、flagJumpEndFrameの指定は無視され、
		末尾ステップの末尾フレームまでジャンプします。
	*/
	void Stop(bool flagJumpEndStep=false, bool flagJumpEndFrame=false);

	/* ********************************************************* */
	//! シーケンスの再生一時停止状態の設定
	/*!
	@param	flagSwitch
		true == 再生を一時停止する
		false == 再生の一時停止を解除する（再開）

	@retval	関数値
		true == 成功
		false == 失敗

	現在再生しているシーケンスの再生の一時停止状態を設定します。
	*/
	bool PauseSet(bool flagSwitch);

	/* ********************************************************* */
	//! シーケンスの再生速度の設定
	/*!
	@param	rateTime
		再生速度係数（1.0f: 等速）

	@retval	関数値
		true == 成功
		false == 失敗

	現在再生しているシーケンスの再生速度の係数（再生速度）を変更
		します。
	*/
	bool RateSetTime(float rateTime);

	/* ********************************************************* */
	//! シーケンスの再生ステップの設定
	/*!
	@param	step
		再生ステップ
		-1 == 末尾ステップ

	@retval	関数値
		true == 成功
		false == 失敗

	現在再生しているシーケンスの再生位置を変更します。
	*/
	bool StepSet(int step);

	/* ********************************************************* */
	//! 再生中かのステータスを取得
	/*!
	@param	なし

	@retval	関数値
		true == 再生中
		false == 再生中ではない

	現在シーケンスを再生中かのステータスを取得します。
	*/
	inline bool StatusGetPlaying(void)
	{
		return(0 != (Status & FlagBitStatus::PLAYING));	/* ? true : false */
	}

	/* ********************************************************* */
	//! 一時停止中かのステータスを取得
	/*!
	@param	なし

	@retval	関数値
		true == 一時停止中
		false == 一時停止中ではない

	現在シーケンスの再生が一時停止中かのステータスを取得します。
	*/
	inline bool StatusGetPausing(void)
	{
		return(	(true == StatusGetPlaying())
				&& (0 != (Status & FlagBitStatus::PAUSING))
			);	/* ? true : false */
	}

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:

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

}	/* SpriteStudio6 */
