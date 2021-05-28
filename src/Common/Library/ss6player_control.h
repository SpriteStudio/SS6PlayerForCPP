/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* アニメーション再生の根底制御関係の処理群です。                             */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>

#include "ss6player_data.h"
#include "ss6player_pack.h"
#include "ss6player_data_project.h"
#include "../PrimitiveBatcher/ss6player_primitive_batcher.h"

/* 前方宣言 */
namespace SpriteStudio6	{
class Entity;
class Project;
}	/* SpriteStudio6 */

namespace SpriteStudio6	{
namespace Library	{
namespace Control	{
namespace Animation	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 再生トラック管理 */
/* MEMO: プレーヤでの再生トラックは「アニメーション再生1つ」につき1つ使用し、アニメーションの */
/*       経過時間や再生速度・再生方向などの管理を行うとともに、最終的に描画やタイムラインなど */
/*       のフレーム単位に変換するところまでの役割です。                                       */
class Track	{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum FlagBitStatus	{
		VALID = 0x40000000,
		PLAYING = 0x20000000,
		PAUSING = 0x10000000,

		STYLE_PINGPONG = 0x02000000,
		STYLE_REVERSE = 0x01000000,

		PLAYING_START = 0x00800000,
		PLAYING_REVERSE = 0x00400000,
		PLAYING_REVERSEPREVIOUS = 0x00200000,
		PLAYING_TURN = 0x00100000,

		DECODE_ATTRIBUTE = 0x0008000,

		IGNORE_SKIPLOOP = 0x00001000,
		IGNORE_USERDATA = 0x00000800,
		IGNORE_SIGNAL = 0x00000400,
		IGNORE_NEXTUPDATE_USERDATA = 0x00000200,
		IGNORE_NEXTUPDATE_SIGNAL = 0x00000100,

		TRANSITION_START = 0x00000080,
		TRANSITION_CANCEL_PAUSE = 0x00000040,

		REQUEST_PLAYEND = 0x00000008,
		REQUEST_TRANSITIONEND = 0x00000004,

		CLEAR = 0x00000000,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	Uint32 /* FlagBitStatus */ Status;

	Library::Data::Animation::Pack::ArgumentContainer ArgumentContainer;

	Float32 TimeDelay;
	Float32 TimeElapsed;
	Float32 TimeElapsedNow;
	Float32 RateTime;
	
	int TimesPlay;
	int CountLoop;
	int CountLoopNow;
	
	int FramePerSecond;
	Float32 TimePerFrame;
	Float32 TimePerFrameConsideredRateTime;
	
	int FrameStart;
	int FrameEnd;
	int FrameRange;
	Float32 TimeRange;
	
	Float32 TimeElapsedTransition;
	Float32 TimeLimitTransition;
	Float32 RateTransition;
	int IndexTrackSlave;

	/* MEMO: This value is applicable even in a stopping.                            */
	/*       Mainly used for shifting display-frame in cycle that executed "Stop()". */
	Float32 TimeElapseReplacement;
	Float32 TimeElapseInRangeReplacement;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* MEMO: StatusGet/Set～の関数群はアクセスの関係上現時点ではpublicになっていますが、 */
	/*       プレーヤ外部からアクセスしないでください（あくまで内部処理用で、実装が予告  */
	/*       なく変更する可能性も低くありません）。                                      */
	/*       ※そのため、意図的に注釈を省いています。                                    */
	/*       ※そもそも、元来的に本クラスを直接叩くことは推奨しません。                  */
	inline bool StatusGetIsValid(void) const
	{
		return(0 != (Status & FlagBitStatus::VALID));
	}
	inline bool StatusGetIsPlaying(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING)));
	}
	inline bool StatusGetIsPausing(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PAUSING) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PAUSING)));
	}
	inline bool StatusGetIsPlayStylePingpong(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::STYLE_PINGPONG) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::STYLE_PINGPONG)));
	}
	inline bool StatusGetIsPlayStyleReverse(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::STYLE_REVERSE) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::STYLE_REVERSE)));
	}
	inline bool StatusGetIsPlayingStart(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_START) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_START)));
	}
	inline bool StatusGetIsPlayingReverse(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_REVERSE) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_REVERSE)));
	}
	inline bool StatusGetIsPlayingReversePrevious(void) const
	{
//		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_REVERSEPREVIOUS) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_REVERSEPREVIOUS)));
		return(0 != (Status & FlagBitStatus::PLAYING_REVERSEPREVIOUS));
	}
	inline bool StatusGetIsPlayingTurn(void) const
	{
		return((FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_TURN) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING | FlagBitStatus::PLAYING_TURN)));
	}
	inline bool StatusGetIsDecodeAttribute(void) const
	{
		return(0 != (Status & FlagBitStatus::DECODE_ATTRIBUTE));
	}
	inline bool StatusGetIsIgnoreSkipLoop(void) const
	{
		return(0 != (Status & FlagBitStatus::IGNORE_SKIPLOOP));
	}
	inline bool StatusGetIsIgnoreUserData(void) const
	{
		return(0 != (Status & FlagBitStatus::IGNORE_USERDATA));
	}
	inline bool StatusGetIsIgnoreSignal(void) const
	{
		return(0 != (Status & FlagBitStatus::IGNORE_SIGNAL));
	}
	inline bool StatusGetIsIgnoreNextUpdateUserData(void) const
	{
		return(0 != (Status & FlagBitStatus::IGNORE_NEXTUPDATE_USERDATA));
	}
	inline bool StatusGetIsIgnoreNextUpdateSignal(void) const
	{
		return(0 != (Status & FlagBitStatus::IGNORE_NEXTUPDATE_SIGNAL));
	}
	inline bool StatusGetIsTransitionStart(void) const
	{
		return(0 != (Status & FlagBitStatus::TRANSITION_START));
	}
	inline void StatusSetIsTransitionStart(bool value)
	{
		if(true == value)	{
			Status |= FlagBitStatus::TRANSITION_START;
		} else {
			Status &= ~FlagBitStatus::TRANSITION_START;
		}
	}
	inline bool StatusGetIsTransitionCancelPause(void) const
	{
		return(0 != (Status & FlagBitStatus::TRANSITION_CANCEL_PAUSE));
	}
	inline void StatusSetIsTransitionCancelPause(bool value)
	{
		if(true == value)	{
			Status |= FlagBitStatus::TRANSITION_CANCEL_PAUSE;
		} else {
			Status &= ~FlagBitStatus::TRANSITION_CANCEL_PAUSE;
		}
	}
	inline bool StatusGetIsRequestPlayEnd(void) const
	{
		return(0 != (Status & FlagBitStatus::REQUEST_PLAYEND));
	}
	inline void StatusSetIsRequestPlayEnd(bool value)
	{
		if(true == value)	{
			Status |= FlagBitStatus::REQUEST_PLAYEND;
		} else {
			Status &= ~FlagBitStatus::REQUEST_PLAYEND;
		}
	}
	inline bool StatusGetIsRequestTransitionEnd(void) const
	{
		return(0 != (Status & FlagBitStatus::REQUEST_TRANSITIONEND));
	}
	inline void StatusSetIsRequestTransitionEnd(bool value)
	{
		if(true == value)	{
			Status |= FlagBitStatus::REQUEST_TRANSITIONEND;
		} else {
			Status &= ~FlagBitStatus::REQUEST_TRANSITIONEND;
		}
	}

	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態にします。
	*/
	void CleanUp(void);

	/* ********************************************************* */
	//! 起動
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスの内容を使用可能に初期化します（再生トラック管理を使用
		可能にします）。
	*/
	bool BootUp(void);

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態に戻して廃棄可能にします。
	*/
	void ShutDown(void);

	/* ********************************************************* */
	//! トラック再生開始
	/*!
	@param	instanceRoot
		アニメーション再生オブジェクト
	@param	dataAnimation
		アニメーションデータ
	@param	indexAnimation
		再生アニメーション番号
	@param	frameRangeStart
		再生フレーム範囲開始フレーム
	@param	frameRangeEnd
		再生フレーム範囲終了フレーム
	@param	frame
		再生フレーム開始フレーム
	@param	framePerSecond
		再生FPS
	@param	rateTime
		再生速度倍率
	@param	timeDelay
		再生までの待機時間（1.0fで1秒）
	@param	flagPingpong
		true == ピンポン再生
		false == 通常再生
	@param	timesPlay
		再生回数
		0 == 無限ループ

	@retval	関数値
		true == 成功
		false == 失敗

	トラックの再生を開始します。
	*/
	bool Start(	Entity& entity,
				const Library::Data::Project::Animation& dataAnimation,
				int indexAnimation,
				int frameRangeStart,
				int frameRangeEnd,
				int frame,
				int framePerSecond,
				float rateTime,
				float timeDelay,
				bool flagPingpong,
				int timesPlay
			);

	/* ********************************************************* */
	//! トラックトランジションを開始
	/*!
	@param	indexTrackSlave
		スレーブ再生トラック
	@param	time
		遷移時間（1.0fで1秒）

	@retval	関数値
		true == 成功
		false == 失敗

	本トラックをメイン再生トラックとして、トラックトランジション
		（TRSアニメーションのブレンド）を開始します。
	*/
	bool Transition(int indexTrackSlave, float time);

	/* ********************************************************* */
	//! トラック再生終了
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	トラックの再生を停止します。
	*/
	bool Stop(void);

	/* ********************************************************* */
	//! トラック再生一時停止状態の設定
	/*!
	@param	flagSwitch
		true == 一時停止状態
		false == 一時停止解除

	@retval	関数値
		true == 成功
		false == 失敗

	トラックの再生一時停止状態を設定します。
	*/
	bool Pause(bool flagSwitch);

	/* ********************************************************* */
	//! トラック再生状態の更新
	/*!
	@param	timeElapsed
		前の処理から今回までに経過した時間（1.0fが1秒）

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスの状態を更新します。
	原則、メインループ1回に1度呼び出してください。
	*/
	bool Update(float timeElapsed);

	/* MEMO: 以下は、publicであるものの、内部処理専用関数です。 */
	/*       ※そのため、意図的に注釈を省いています。           */
	void StatusClearTransient(bool flagDecodeNextForce)
	{	/* MEMO: SS6Player for Unityでは呼出コストを嫌ってEntityに直書きしていましたが、C++版ではinline関数での実装に戻しました。 */
		if((FlagBitStatus::VALID | FlagBitStatus::PLAYING) == (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING)))	{	/* Not-Playing */
			Status &= ~(	FlagBitStatus::PLAYING_START
							| FlagBitStatus::DECODE_ATTRIBUTE
							| FlagBitStatus::TRANSITION_START
							| FlagBitStatus::IGNORE_NEXTUPDATE_USERDATA
							| FlagBitStatus::IGNORE_NEXTUPDATE_SIGNAL
						);
		}
		if(true == flagDecodeNextForce)	{
			Status |= FlagBitStatus::DECODE_ATTRIBUTE;
		}
		TimeElapseReplacement = 0.0f;
	}
	void TimeSkip(float time, bool flagReverseParent, bool flagWrapRange);
	bool TimeGetRemain(float* timeRemainTotal, float* timeRemainInRange);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 更新処理用引数 */
/* MEMO: 更新で取りまとめた描画に必要な情報をまとめておくための構造体です。 */
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
	/* SpriteStudio6機能としてのカメラ行列 */
	alignas(SpriteStudio6::AlignmentMemory::TUPPLE_VARIABLE) FPU::Matrix4x4 MatrixCamera;
	FPU::Matrix4x4* MatrixTransformPartsCamera;	/* テンポラリ・適用するカメラパーツの持つ変形行列 */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline void CleanUp(void)
	{
		MatrixCamera.Set(FPU::Matrix4x4::Unit);
		MatrixTransformPartsCamera = nullptr;
	}

	inline void Duplicate(ArgumentContainer& original)
	{
		MatrixCamera.Set(original.MatrixCamera);
		MatrixTransformPartsCamera = original.MatrixTransformPartsCamera;
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* アトリビュートのキャッシュバッファ */
/* MEMO: パーツ制御・パーツ描画制御の中で共用しています。 */
template<typename _Type> class BufferAttribute
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
	int FrameKey;
	_Type Value;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline void CleanUp(void)
	{
		FrameKey = -1;
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* 再生パーツ管理 */
/* MEMO: プレーヤでの再生している各パーツのデータを管理します。 */
/*       アトリビュート更新のキャッシュの役割もあります。       */
class Parts	{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* TRS（座標・スケール・回転）のアトリビュートキャッシュ */
	/* MEMO: TRSはほぼひとまとめで管理しないと色々面倒なので……。 */
	class BufferTRS
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
		BufferAttribute<CPU::Vector3> Position;
		BufferAttribute<CPU::Vector3> Rotation;
		BufferAttribute<CPU::Vector2> Scaling;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			Position.CleanUp();
			Position.Value = CPU::Vector3::Zero;
			Rotation.CleanUp();
			Rotation.Value = CPU::Vector3::Zero;
			Scaling.CleanUp();
			Scaling.Value = CPU::Vector2::One;
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum FlagBitStatus	{
		VALID = 0x40000000,
		HIDE_FORCE = 0x20000000,
		HIDE = 0x10000000,

		CHANGE_TRANSFORM_SCALING = 0x04000000,
		CHANGE_TRANSFORM_ROTATION = 0x02000000,
		CHANGE_TRANSFORM_POSITION = 0x01000000,

		REFRESH_TRANSFORM_SCALING = 0x00400000,
		REFRESH_TRANSFORM_ROTATION = 0x00200000,
		REFRESH_TRANSFORM_POSITION = 0x00100000,

		UPDATE_SCALELOCAL = 0x00080000,
		UPDATE_RATEOPACITY = 0x00040000,
		UPDATE__RESERVED_01_ = 0x00020000,	/* Reserved */

		CHANGE_CELL_UNREFLECTED = 0x00000800,
		CHANGE_CELL_IGNORE_ATTRIBUTE = 0x00000200,
		CHANGE_CELL_IGNORE_NEWANIMATION = 0x00000100,

		INSTANCE_PLAY_INDEPENDENT = 0x00000080,
		INSTANCE_IGNORE_EXCEPT_NEXTDATA = 0x00000040,
		INSTANCE_IGNORE_ATTRIBUTE = 0x00000020,
		INSTANCE_IGNORE_NEWANIMATION = 0x00000010,

		EFFECT_PLAY_INDEPENDENT = 0x00000008,
		EFFECT_IGNORE_EXCEPT_NEXTDATA = 0x00000004,
		EFFECT_IGNORE_ATTRIBUTE = 0x00000002,
		EFFECT_IGNORE_NEWANIMATION = 0x00000001,

		CLEAR = 0x00000000
	};

	/* ----------------------------------------------- Variables */
private:
	int FrameKeyStatusAnimationFrame;
	SpriteStudio6::Library::Data::Animation::Attribute::Status StatusAnimationFrame;

protected:
public:
	int IDParts;																						/* パーツID */
	Uint32 /* FlagBitStatus */ Status;																	/* 動作ステータス */
	Uint32 /* SpriteStudio6::Library::Data::Animation::Parts::FlagBitStatus */ StatusAnimationParts;	/* アニメーションデータの更新情報など */
	int IndexControlTrack;																				/* アニメーション再生トラック番号 */
	int IndexControlDraw;																				/* 描画情報 *//* ※SS6Player for Unityでは固定で持っていましたがC++版では別に配列を持っています */

	int IndexEntityUnderControl;																		/* 制御している子エンティティ番号 *//* インスタンス・エフェクト以外では-1 */
	int IndexPackUnderControl;																			/* 制御している子エンティティのアニメーションパック番号（エフェクトパーツの場合はエフェクトデータ番号） */
	int IndexAnimationUnderControl;																		/* 制御している子エンティティのアニメーション番号 */

	/* アトリビュートのキャッシュ関係 */
	BufferTRS TRSMaster;																				/* TRSのマスタ */
	BufferTRS TRSSlave;																					/* TRSのスレーブ *//* ※トランジション時にしか使いません */
	BufferAttribute<CPU::Vector2> ScaleLocal;															/* ローカルスケール */
	BufferAttribute<Float32> RateOpacity;																/* 不透明度（ローカル不透明度・マスク強度共用） */
	BufferAttribute<int> Priority;																		/* 優先度 */

	int FramePreviousUpdateUnderControl;																/* インスタンスの前回キー位置 *//* ※インスタンス・エフェクトはキーの検知が特殊なのでBufferAttributeを使っていません。 */
	Library::Data::Animation::Attribute::Instance DataInstance;											/* インスタンス */

	/* MEMO: 行列計算などが高速になるようにFPU変数型にしています。 */
	/* MEMO: アライメントしなくてはいけない連中は一応最後に置いておきます。 */
	/* MEMO: SS6Player for Unityで別個に存在したMatrixBoneWorld（ワールド座標系でのボーン行列） */
	/*       は、C++版では補正計算をしなくて良いのでMatrixTransformに統一しました。             */
	/* MEMO: 行列系はワールド行列を持っておくとよさそうな「気になり」ますが、持っていません。 */
	/*       ローカル行列からのワールド行列への変換は頂点シェーダ側で行います。               */
	/*       理由としては下記が挙げられます。                                                 */
	/*       - CPUでの乗算回数を減らすため（メッシュ関係で結構乗算をしているので、それ以外は  */
	/*         節約したい）。                                                                 */
	/*       - 後に擬似的なインスタンス描画（nVIDIAの擬似インスタンシングでもなく・GPUインス  */
	/*         タンシングでもなく……計算が終わったアニメーションを、ワールド行列だけ変えて複 */
	/*         数回Entity::Renderだけをまわして描画増殖する方法）を可能にした時にワールド行列 */
	/*         が混入していると面倒になるため。                                               */
	alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Vector3 TransformPosition;							/* （ローカル）座標 */
	alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Vector3 TransformRotation;							/* （ローカル）回転 */
	alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Vector3 TransformScaling;							/* （ローカル）スケール *//* ※否「（アトリビュートの）ローカルスケール」 */
	alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Matrix4x4 MatrixTransform;							/* ローカル座標系でのパーツの（継承される）ローカル行列 *//* InstanceGameObject/InstanceTransform */
	alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Matrix4x4 MatrixTransformLocal;						/* ローカル座標系でのパーツの（自身のみの）ローカル行列 */

	/* ----------------------------------------------- Functions */
private:
	bool BootUpInstance(Entity& entity, int idParts, int& indexControlDraw, int& indexEntityUnderControlInstance, void*& workArea);
	bool BootUpEffect(Entity& entity, int idParts, int& indexControlDraw, int& indexEntityUnderControlEffect, void*& workArea);

	void ShutDownInstance(Entity& entity, int idParts);
	void ShutDownEffect(Entity& entity, int idParts);

	void UpdateMatrixTransform(	Entity& entity,
								int idParts,
								ArgumentContainer& argument,
								const Library::Data::Animation::Parts& dataAnimationParts,
								Control::Animation::Track& controlTrack,
								const FPU::Matrix4x4& matrixParent
							);
	void UpdateNormal(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Control::Animation::Track& controlTrack
					);
	void UpdateInstance(	Entity& entity,
							int idParts,
							ArgumentContainer& argument,
							const Library::Data::Animation::Parts& dataAnimationParts,
							Control::Animation::Track& controlTrack
					);
	void UpdateEffect(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Control::Animation::Track& controlTrack
					);
	void UpdateMesh(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Control::Animation::Track& controlTrack
					);
	void UpdateSetPartsDraw(	Entity& entity,
								int idParts,
								bool flagHideDefault,
								bool flagSetForce,
								bool flagMask,
								int indexTrackRoot
							);
	void UpdateUserData(Entity& entity, int idParts, const Library::Data::Animation::Parts& dataAnimationParts, Control::Animation::Track& controlTrack);
	void UpdateUserDataFoward(	Entity& entity,
								int idParts,
								const Library::Data::Animation::Parts& dataAnimationParts,
								Control::Animation::Track& controlTrack,
								int frameRangeStart,
								int frameRangeEnd,
								int frameDecode,
								bool flagTurnBack
							);
	void UpdateUserDataReverse(	Entity& entity,
								int idParts,
								const Library::Data::Animation::Parts& dataAnimationParts,
								Control::Animation::Track& controlTrack,
								int frameRangeEnd,
								int frameRangeStart,
								int frameDecode,
								bool flagTurnBack
							);
	void UpdateSignal(Entity& entity, int idParts, const Library::Data::Animation::Parts& dataAnimationParts, Control::Animation::Track& controlTrack);
	void UpdateSignalFoward(	Entity& entity,
								int idParts,
								const Library::Data::Animation::Parts& dataAnimationParts,
								Control::Animation::Track& controlTrack,
								int frameRangeStart,
								int frameRangeEnd,
								int frameDecode,
								bool flagTurnBack
							);
	void UpdateSignalReverse(	Entity& entity,
								int idParts,
								const Library::Data::Animation::Parts& dataAnimationParts,
								Control::Animation::Track& controlTrack,
								int frameRangeEnd,
								int frameRangeStart,
								int frameDecode,
								bool flagTurnBack
							);

	bool DrawNormal(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					);
	bool DrawInstance(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					);
	bool DrawEffect(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					);
	bool DrawMask(	Entity& entity,
					int idParts,
					ArgumentContainer& argument,
					const Library::Data::Animation::Parts& dataAnimationParts,
					Library::Control::Animation::Track& controlTrack,
					bool flagPreDraw,
					bool flagHideDefault,
					bool flagPlanarization,
					Library::KindMasking::Kind masking
				);
	bool DrawMesh(	Entity& entity,
					int idParts,
					ArgumentContainer& argument,
					const Library::Data::Animation::Parts& dataAnimationParts,
					Library::Control::Animation::Track& controlTrack,
					bool flagPreDraw,
					bool flagHideDefault,
					bool flagPlanarization,
					Library::KindMasking::Kind masking
				);

protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Parts(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	※このクラスは継承したりしないので、仮想化していません。
	*/
	~Parts(void);

	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態にします。
	*/
	void CleanUp(void);

	/* ********************************************************* */
	//! パーツで使用するワークエリアのサイズを取得
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID

	@retval	関数値
		ワークエリアのサイズ
		0 == 追加のワークエリアを必要としない
		~0（-1） == 失敗

	パーツが描画パーツである場合に、Drawクラスの実体以外に使用する
		追加のワークエリアのサイズを返します。
	※具体的には、パーツがペアになるDrawクラスを持つ時に、そのDraw
		クラスが使用するワークエリアのサイズになります。
	※GPU転送バッファ（Buffer::GPU～）が使うバッファプールのサイズ
		は本関数では考慮されません（使用するメモリ特性が異なるため
		です。本関数はあくまでCPUとFPUが使うためのワーク長です）。
	*/
	static size_t SizeGetWorkArea(Entity& entity, int idParts);

	/* ********************************************************* */
	//! 起動
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	indexControlDraw
		連携するDraw制御のインデックスバッファ
	@param	indexEntityUnderControlInstance
		連携する子エンティティのインデックスバッファ（インスタンス用）
	@param	indexEntityUnderControlEffect
		連携する子エンティティのインデックスバッファ（エフェクト用）
	@param	workArea
		追加ワークエリアの先頭

	@retval	関数値
		true == 成功
		false == 失敗
	@retval	indexControlDraw
		次のパーツで使用するDraw制御のインデックス
	@retval	indexEntityUnderControlInstance
		次のインスタンスパーツで使用する子エンティティのインデックス
	@retval	indexEntityUnderControlEffect
		次のエフェクトパーツで使用する子エンティティのインデックス
	@retval	workArea
		次のパーツで使用する追加ワークエリアの先頭

	本クラスの内容を使用可能に初期化します（アニメーションパーツ
		を使用可能にします）。
	indexControlDraw・indexEntityChildInstance・indexEntityChildEffect
		は最初は0に初期化しておいてください。
	*/
	bool BootUp(Entity& entity, int idParts, int& indexControlDraw, int& indexEntityUnderControlInstance, int& indexEntityUnderControlEffect, void*& workArea);

	/* ********************************************************* */
	//! 終了
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）

	本クラスの内容を使用終了状態にして、廃棄可能にします。
	*/
	void ShutDown(Entity& entity, int idParts);

	/* ********************************************************* */
	//! 更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	argument
		処理用引数群（変形行列など）
	@param	flagHideDefault
		デフォルトの非表示状態（原則アニメーションオブジェクト自体の非表示状態）
	@param	indexTrackRoot
		アニメーションのルートパーツが再生に使用している再生トラック番号
	@param	matrixParent
		親アニメーションから継承する姿勢行列

	@retval	なし

	パーツの状態を更新します。
	argumentのMatrixTransformPartsCameraは本処理の中で収集されるので、
		純粋に更新のための引数というわけではありませんので注意して
		ください（さらに、MatrixCameraはMatrixTransformPartsCameraの
		確定後に最終決定されます）。
	*/
	void Update(Entity& entity, int idParts, ArgumentContainer& argument, bool flagHideDefault, int indexTrackRoot, const FPU::Matrix4x4& matrixParent);

	/* ********************************************************* */
	//! 描画前描画用情報の更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	argument
		処理用引数群（変形行列など）
	@param	flagHideDefault
		デフォルトの非表示状態（原則アニメーションオブジェクト自体の非表示状態）
	@param	flagPlanarization
		平面描画フラグ
	@param	masking
		マスキング種別

	@retval	関数値
		true == パーツを実際に描画する
		false == パーツを実際に描画しない

	パーツの描画前描画（PreDraw）のための情報を計算します。
	※「描画前描画」は初期のマスク状態を描画前にステンシルに設定する
		ためのもので、マスクパーツのみを描画する工程です。
	*/
	bool PreDraw(	Entity& entity,
					int idParts,
					ArgumentContainer& argument,
					bool flagHideDefault,
					bool flagPlanarization,
					Library::KindMasking::Kind masking
				);

	/* ********************************************************* */
	//! 描画用情報の更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	argument
		処理用引数群（変形行列など）
	@param	flagHideDefault
		デフォルトの非表示状態（原則アニメーションオブジェクト自体の非表示状態）
	@param	flagPlanarization
		平面描画フラグ
	@param	masking
		マスキング種別

	@retval	関数値
		true == パーツを実際に描画する
		false == パーツを実際に描画しない

	パーツの描画前描画（PreDraw）のための情報を計算します。
	*/
	bool Draw(	Entity& entity,
				int idParts,
				ArgumentContainer& argument,
				bool flagHideDefault,
				bool flagPlanarization,
				Library::KindMasking::Kind masking
			);

	/* ********************************************************* */
	//! 次に描画するパーツを取得
	/*!
	@param	なし

	@retval	なし

	データ内に記録された描画パーツチェインの次のパーツを取得します。
	※ただ、これはベイクデータなので、パーツ合成などを行うと正常に
		機能しないため、動的配列でのソートのパフォーマンスによっては
		削除します（というよりおそらく最終的に抹消予定関数）。
	*/
	inline int IDPartsNextDraw(void)
	{
		if(false == StatusAnimationFrame.IsValid())	{
			return(-1);
		}
		return(StatusAnimationFrame.IDGetPartsNextDraw());
	}

	/* ********************************************************* */
	//! 次に描画前描画するパーツを取得
	/*!
	@param	なし

	@retval	なし

	データ内に記録された描画前描画（マスクのステンシル描画）パーツ
		チェインの次のパーツを取得します。
	※ただ、これはベイクデータなので、パーツ合成などを行うと正常に
		機能しないため、動的配列でのソートのパフォーマンスによっては
		削除します（というよりおそらく最終的に抹消予定関数）。
	*/
	inline int IDPartsNextPreDraw(void)
	{
		if(false == StatusAnimationFrame.IsValid())	{
			return(-1);
		}
		return(StatusAnimationFrame.IDGetPartsNextPreDraw());
	}

	/* ********************************************************* */
	//! アニメーション変更時の定型処理
	/*!
	外部から使用しないでください（publicになっていますが、内部処理
		用関数です）
	*/
	void AnimationChange(Entity& entity);

	/* ********************************************************* */
	//! アトリビュートのキャッシュ消去
	/*!
	外部から使用しないでください（publicになっていますが、内部処理
		用関数です）
	*/
	void CacheClearAttribute(Entity& entity, bool flagClearCellApply);

	/* ********************************************************* */
	//! インスタンスの再生開始
	/*!
	外部から使用しないでください（publicになっていますが、内部処理
		用関数です）
	*/
	bool InstancePlayStart(Entity& entity, float rateTime);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* パーツ描画制御 */
/* MEMO: 描画するパーツの描画情報です。                                                     */
/*       ※SS6Player for Unityではパーツ制御の中に入っていましたが、C++では使用メモリ削減の */
/*         ために別に分けてあります。                                                       */
class Draw
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum FlagBitStatus : Uint32	{
		/* Common (Accumulated until drawed) */
		NO_DRAW = 0x40000000,	/* Not "Hide" ... for when no cell designation */

		UPDATE_COORDINATE = 0x08000000,
		UPDATE_UVTEXTURE = 0x04000000,
		UPDATE_PARAMETERBLEND = 0x02000000,
		UPDATE_COLORPARTS = 0x01000000,

		UPDATE_MASKING = 0x00800000,
		UPDATE_DEFORM = 0x00400000,	/* Mesh only */

		/* Common (One-time) */
		UPDATE_COORDINATE_NOWFRAME = 0x00080000,
		UPDATE_COLLIDERRECTANGLE_NOWFRAME = 0x00040000,

		/* for Plain (Normal/Mask) */
		UPDATE_TRANSFORM_TEXTURE = 0x00008000,
		UPDATE_FLIP_COEFFICIENT = 0x00004000,	/* FLIP_X,FLIP_Y,FLIP_TEXTURE_X or FLIP_TEXTURE_Y are updated. */

		USE_ADDITIONALCOLOR_PREVIOUS = 0x00000800,
		USE_ADDITIONALCOLOR = 0x00000400,
		/* MEMO: Bit-order should be the same as "Library_SpriteStudio6.Data.Animation.Attribute.Status.FlagBit.FLIP_*". */
		FLIP_COEFFICIENT_X = 0x00000080,	/* Normal(Sprite) Only */
		FLIP_COEFFICIENT_Y = 0x00000040,	/* Normal(Sprite) Only */
		FLIP_COEFFICIENT_TEXTURE_X = 0x00000020,	/* Normal(Sprite) Only */
		FLIP_COEFFICIENT_TEXTURE_Y = 0x00000010,	/* Normal(Sprite) Only */

		CLEAR = 0x00000000,
	};

	enum FlagShiftStatus	{
		FLIP_COEFFICIENT_TO_ATTRIBUTE = 20,	/* FlagBitStatus::Shift から Library::Data::Animation::Attribute::Status::FlagBit へのシフト幅 */
	};

	enum Constant	{
		COUNT_ARGUMENT = 16,	/* Argumentの配列長 */
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	Uint32 /* FlagBitStatus */ Status;

	Library::Data::Animation::Attribute::Status DataStatusPrevious;

	Library::KindMasking::Kind Masking;

	CPU::Vector2 RateScaleMesh;
	CPU::Vector2 RateScaleTexture;

	CPU::Vector2 SizeSprite;
	CPU::Vector2 PivotSprite;
	CPU::Vector2 SizeTexture;
	CPU::Vector2 SizeCell;
	CPU::Vector2 PivotCell;
	CPU::Vector2 PositionCell;
	Library::Data::Animation::Attribute::Cell DataCellApply;
	int IndexVertexCorrectionTable;

	BufferAttribute<Library::Data::Animation::Attribute::Cell> DataCell;
	BufferAttribute<Library::Data::Animation::Attribute::PartsColor> PartsColor;
	BufferAttribute<Library::Data::Animation::Attribute::VertexCorrection> VertexCorrection;
	BufferAttribute<Library::Data::Animation::Attribute::Deform> Deform;
	BufferAttribute<Library::Data::Animation::Attribute::Shader> Shader;
	BufferAttribute<Library::Data::Animation::Attribute::Signal> Signal;
	BufferAttribute<CPU::Vector2> OffsetPivot;
	BufferAttribute<CPU::Vector2> SizeForce;
	BufferAttribute<CPU::Vector2> ScalingTexture;
	BufferAttribute<Float32> RotationTexture;
	BufferAttribute<CPU::Vector2> PositionTexture;

	/* MEMO: パーツカラーの最終的な適用テーブル */
	/*       ※このポインタ群については、実体はアトリビュートのキャッシュか動的パーツ */
	/*         カラーのワークを指すので、動的にワークを取る必要はありません。         */
	Library::KindOperationBlend::Kind OperationPartsColor;
	const CPU::Color* TableVertexColorPartsColor;
	const Float32* TableRateAlphaPartsColor;

	/* MEMO: 各配列の実体は頂点数分だけ要素を持ちます。 */
	/* MEMO: 本来の型はCPU::～で統一するのが推奨ですが、計算量の最適化の都合上FPU::～になっているものがあります。 */
	/*       ※主にメッシュの頂点座標関係です（対ベクトル・対行列の計算が直接的に絡む関係から、FPU::～でないと都  */
	/*         合が悪いためです）。                                                                               */
	/*                                                                                                            */
	/*       また、FPU::～型は、メモリアライメントが必要なので、注意してください。                                */
	/*       さらにCPU::～型であってもシェーダ定数の領域の場合は、std140でのアライメントの問題が出るので、Vector4 */
	/*       以外にするとシェーダ側でアライメントが変わる可能性が高いので、その点も注意してください。             */
	/* MEMO: GPUに転送するための頂点バッファは各エンティティが持つModelDraw（PrimitiveBatcherクラス）に集積 */
	/*       されているので、各パーツが持つのはCPU計算用のキャッシュのみです。                              */
	int CountVertex;
	int CountVertexIndex;
	Uint16* IndexVertexDraw;	/* const Uint16* */
	CPU::Color* ColorPartsDraw;
	CPU::Vector4* UVTextureDraw;
	FPU::Vector4* CoordinateDraw;
	CPU::Vector3* CoordinateTransformedDraw;
	FPU::Vector4* DeformDraw;
	CPU::Vector4* ArgumentVs00Draw;
	/* MEMO: 下記は全頂点同じデータ(パーツで)違うが頂点では違わないので、 */
	/*       頂点データ転送時に一斉に複製します。                         */
	CPU::Vector4 FlagsDraw;
	/* MEMO: 下記のArgumentFs**DrawとParameterFs**Drawの00～03は、「Constant::COUNT_ARGUMENT / 4」分だけ必要になります。 */
	CPU::Vector4 ArgumentFs00Draw;
	CPU::Vector4 ArgumentFs01Draw;
	CPU::Vector4 ArgumentFs02Draw;
	CPU::Vector4 ArgumentFs03Draw;
	CPU::Vector4 ParameterFs00Draw;
	CPU::Vector4 ParameterFs01Draw;
	CPU::Vector4 ParameterFs02Draw;
	CPU::Vector4 ParameterFs03Draw;

	/* MEMO: 行列計算などが高速になるようにFPU変数型にしています。 */
	/* MEMO: アライメントしなくてはいけない連中は一応最後に置いておきます。 */
	alignas(SpriteStudio6::AlignmentMemory::TUPPLE_VARIABLE) FPU::Matrix4x4 MatrixTexture;

	/* ----------------------------------------------- Functions */
private:
	bool BootUpCommon(int countPartsColorBuffer, bool flagMask, void*& workArea);
	bool TextureSetTarget(	PrimitiveBatcher::SettingRender* settingRender,
							bool* flagBreakBatching,
							Entity& entity,
							int idParts,
							Parts& controlParts,
							ArgumentContainer& argument,
							int indexCellMap
						);
	bool TextureSetCellMap(	PrimitiveBatcher::SettingRender* settingRender,
							Entity& entity,
							int idParts,
							Parts& controlParts,
							ArgumentContainer& argument,
							int indexCellMap,
							bool flagBindTexture
						);

protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	Draw(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	※このクラスは継承したりしないので、仮想化していません。
	*/
	~Draw(void);

	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態にします。
	*/
	void CleanUp(void);

	/* ********************************************************* */
	//! アニメーションが変更された時の定型処理
	/*!
	@param	なし

	@retval	なし

	アニメーションが変更された時に必要な本クラスの再初期化を行います。
	*/
	void AnimationChange(bool flagClearDataCellApply);

	/* ********************************************************* */
	//! 描画情報で使用するワークエリアのサイズを取得
	/*!
	@param	entity
		エンティティ
	@param	idParts
		パーツID
	@param	countVertex
		矩形の持つ頂点数
	@param	flagMesh
		true == メッシュパーツとしての算出
		false == 通常（否メッシュパーツ）として算出

	@retval	関数値
		ワークエリアのバイトサイズ
		0 == ワークエリアは不要
		~0(-1) == 失敗

	指定された頂点数で使用するワークエリアのサイズを取得します。

	インスタンス・エフェクトパーツの場合はcountVertex=0で呼び出して
		ください。
	※NULLパーツやボーン系パーツなどの描画情報を持たないパーツの場合は
		本関数を呼ばないでください。

	flagMeshは、メッシュパーツの場合だけは頂点インデックスの配列を持つ
		必要があるため、trueとfalseの場合で返すワーク長が変わります。
	※ただ、現在だと頂点インデックスの領域を保持する必要性がほとんど
		ない（動的変化しないのと、Buffer::SingleGPUの中に持ってしまって
		いるため）ので、後に頂点インデックスの配列もろとも削除するかも
		しれません。

	本関数は静的関数です（注意してください）。
	*/
	static size_t SizeGetWorkArea(Entity& entity, int idParts, int countVertex, bool flagMesh);

	/* ********************************************************* */
	//! 矩形（通常・マスク）パーツ用描画情報の起動
	/*!
	@param	countVertex
		矩形の持つ頂点数
		※矩形なのでLibrary::KindVertex::TERMINATOR2かTERMINATOR4です。
	@param	flagMask
		true == マスクパーツ
		false == 描画用パーツ
	@param	workArea
		追加ワークエリアの先頭
	@param	flagIsPartsMask
		true == マスクパーツ
		false == マスクパーツ以外

	@retval	関数値
		true == 成功
		false == 失敗（エラー）
	@retval	workArea
		追加ワークエリアの先頭

	矩形パーツ用の初期化を行います。
	countVertexは
	- アニメーション用ならLibrary::KindVertex::TERMINATOR4
	- エフェクト用ならLibrary::KindVertex::TERMINATOR2

	flagIsPartsMaskは呼び出し元のパーツの種別がマスクパーツか否かと
		正しく合致していなくてはなりません（SizeGetWorkArea関数で
		求めたワークエリア長と不整合を起こしてメモリオーバーランなど
		が起こります）。
	*/
	bool BootUp(int countVertex, bool flagMask, void*& workArea, bool flagIsPartsMask);

	/* ********************************************************* */
	//! メッシュパーツ用描画情報の起動
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	flagMask
		true == マスクパーツ
		false == 描画用パーツ
	@param	workArea
		追加ワークエリアの先頭

	@retval	関数値
		true == 成功
		false == 失敗（エラー）
	@retval	workArea
		追加ワークエリアの先頭

	メッシュパーツ用の初期化を行います。
	flagMaskは現状無視されますが、予約として残しておきます。
	*/
	bool BootUpMesh(Entity& entity, int idParts, bool flagMask, void*& workArea);

	/* ********************************************************* */
	//! インスタンスパーツ用描画情報の起動
	/*!
	@param	flagMask
		true == マスクパーツ
		false == 描画用パーツ
	@param	workArea
		追加ワークエリアの先頭

	@retval	関数値
		true == 成功
		false == 失敗（エラー）
	@retval	workArea
		追加ワークエリアの先頭

	インスタンスパーツ用の初期化を行います。
	flagMaskは現状無視されますが、予約として残しておきます。
	*/
	bool BootUpInstance(bool flagMask, void*& workArea);

	/* ********************************************************* */
	//! エフェクトパーツ用描画情報の起動
	/*!
	@param	flagMask
		true == マスクパーツ
		false == 描画用パーツ
	@param	workArea
		追加ワークエリアの先頭

	@retval	関数値
		true == 成功
		false == 失敗（エラー）
	@retval	workArea
		追加ワークエリアの先頭

	エフェクトパーツ用の初期化を行います。
	flagMaskは現状無視されますが、予約として残しておきます。
	*/
	bool BootUpEffect(bool flagMask, void*& workArea);

	/* ********************************************************* */
	//! 矩形パーツ用描画情報の終了
	/*!
	@param	なし

	@retval	なし

	矩形パーツ用描画情報を終了します。
	*/
	void ShutDown(void);

	/* ********************************************************* */
	//! メッシュパーツ用描画情報の終了
	/*!
	@param	なし

	@retval	なし

	メッシュパーツ用描画情報を終了します。
	*/
	void ShutDownMesh(void);

	/* ********************************************************* */
	//! インスタンスパーツ用描画情報の終了
	/*!
	@param	なし

	@retval	なし

	インスタンスパーツ用描画情報を終了します。
	*/
	void ShutDownInstance(void);

	/* ********************************************************* */
	//! エフェクトパーツ用描画情報の終了
	/*!
	@param	なし

	@retval	なし

	エフェクトパーツ用描画情報を終了します。
	*/
	void ShutDownEffect(void);

	/* ********************************************************* */
	//! 反転ステータス群の設定
	/*!
	@param	status
		ステータスアトリビュートのデータ

	@retval	なし

	ステータスアトリビュートのデータから反転情報を解析して本クラス
		に設定します。
	*/
	void StatusSetFlip(Library::Data::Animation::Attribute::Status& status);

	/* ********************************************************* */
	//! 描画情報の更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	controlParts
		パーツ制御
	@param	argument
		再生トラックの情報コンテナ
	@param	dataAnimationParts
		パーツのモデル情報
	@param	statusControlParts
		パーツ制御のステータスの参照
	@param	controlTrack
		再生トラック制御

	@retval	関数値
		true == 成功
		false == 失敗（エラー）

	パーツの描画状態を更新します。
	EntityのUpdateから呼び出してください（それ以外からは呼ばないで
		ください）。
	*/
	bool Update(	Entity& entity,
					int idParts,
					Parts& controlParts,
					ArgumentContainer& argument,
					const Library::Data::Animation::Parts& dataAnimationParts,
					Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
					Control::Animation::Track& controlTrack
				);

	/* ********************************************************* */
	//! 矩形パーツの描画情報の更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	controlParts
		パーツ制御
	@param	argument
		再生トラックの情報コンテナ
	@param	dataAnimationParts
		パーツのモデル情報
	@param	flagPreDraw
		true == 描画前描画（マスク初期化用）
		false == 通常描画
	@param	flagPlanarization
		true == Z値を殺す（全パーツ・全頂点をZ座標値0）
		false == Z値を生かす（頂点のZ値はデータと変形計算結果を使用）
	@param	masking
		パーツに適用するマスク手法
	@param	statusControlParts
		パーツ制御のステータスの参照
	@param	controlTrack
		再生トラック制御

	@retval	関数値
		true == 成功
		false == 失敗（エラー）

	パーツの描画状態を更新します。
	EntityのUpdateから呼び出してください（それ以外からは呼ばないで
		ください）。

	本関数を使用する描画パーツ種別は下記になります。
	- 通常パーツ
	- マスクパーツ（描画前描画: PreDraw時……のみ）

	※通常パーツは過去2三角分割がありましたが現在は全て4三角形分割
		なので、将来的に処理中の2三角形分割の判定を削除する予定です。
	※エフェクトでは2三角形分割を使用していますが専用処理関数を持っ
		ていますので、本関数を使用していません。
	※マスクパーツは、PreDraw（描画前描画）とDraw（本描画）とで同じ
		頂点情報を使用するため、Drawは処理を最小限にした専用関数を
		持っています（DrawMask関数）。

	本関数で実際にGPUに送られる頂点が決定されます。
	※実際にGPUに送られるのは、本関数で算出された頂点群が定義された
		PrimitiveBatcherクラス（Entity::ModelDraw）のDraw関数である
		ことに注意してください。
	*/
	bool DrawNormal(	Entity& entity,
						int idParts,
						Parts& controlParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						bool flagPreDraw,
						bool flagPlanarization,
						Library::KindMasking::Kind masking,
						Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
						Control::Animation::Track& controlTrack
					);

	/* ********************************************************* */
	//! マスクパーツの描画情報の更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	controlParts
		パーツ制御
	@param	argument
		再生トラックの情報コンテナ
	@param	dataAnimationParts
		パーツのモデル情報
	@param	flagPreDraw
		true == 描画前描画（マスク初期化用）
		false == 通常描画
	@param	flagPlanarization
		true == Z値を殺す（全パーツ・全頂点をZ座標値0）
		false == Z値を生かす（頂点のZ値はデータと変形計算結果を使用）
	@param	masking
		パーツに適用するマスク手法
	@param	statusControlParts
		パーツ制御のステータスの参照
	@param	controlTrack
		再生トラック制御

	@retval	関数値
		true == 成功
		false == 失敗（エラー）

	パーツの描画状態を更新します。
	EntityのUpdateから呼び出してください（それ以外からは呼ばないで
		ください）。

	本関数を使用する描画パーツ種別は下記になります。
	- マスクパーツ（本描画: Draw時……のみ）

	※本関数を呼び出す場合、必ずそれ以前に（同じマスクパーツで）
		PreDraw時にDrawNormal関数が呼ばれている必要があります
		（本関数はPreDraw時に求めた描画情報をそのまま使用して、
		本描画時の処理負荷を軽減するために存在しています）。
	※関数のプロトタイプとしてDrawNormalと同じ引数群を取って
		いますが、かなりの引数を使用していません。
		殆どの引数が「DrawNormalと揃えるため」と「後にPreDrawと
		Drawで処理内容が変わる可能性が捨てきれない」ために予備と
		して受け取っている値群です。

	本関数で実際にGPUに送られる頂点が決定されます。
	※実際にGPUに送られるのは、本関数で算出された頂点群が定義された
		PrimitiveBatcherクラス（Entity::ModelDraw）のDraw関数である
		ことに注意してください。
	*/
	bool DrawMask(	Entity& entity,
					int idParts,
					Parts& controlParts,
					ArgumentContainer& argument,
					const Library::Data::Animation::Parts& dataAnimationParts,
					bool flagPreDraw,
					bool flagPlanarization,
					Library::KindMasking::Kind masking,
					Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
					Control::Animation::Track& controlTrack
				);

	/* ********************************************************* */
	//! メッシュパーツの描画情報の更新
	/*!
	@param	entity
		再生エンティティ（アニメーションオブジェクトの実体）
	@param	idParts
		パーツID
	@param	controlParts
		パーツ制御
	@param	argument
		再生トラックの情報コンテナ
	@param	dataAnimationParts
		パーツのモデル情報
	@param	flagPreDraw
		true == 描画前描画（マスク初期化用）
		false == 通常描画
	@param	flagPlanarization
		true == Z値を殺す（全パーツ・全頂点をZ座標値0）
		false == Z値を生かす（頂点のZ値はデータと変形計算結果を使用）
	@param	masking
		パーツに適用するマスク手法
	@param	statusControlParts
		パーツ制御のステータスの参照
	@param	controlTrack
		再生トラック制御

	@retval	関数値
		true == 成功
		false == 失敗（エラー）

	パーツの描画状態を更新します。
	EntityのUpdateから呼び出してください（それ以外からは呼ばないで
		ください）。

	本関数を使用する描画パーツ種別は下記になります。
	- メッシュパーツ

	本関数で実際にGPUに送られる頂点が決定されます。
	※実際にGPUに送られるのは、本関数で算出された頂点群が定義された
		PrimitiveBatcherクラス（Entity::ModelDraw）のDraw関数である
		ことに注意してください。
	*/
	bool DrawMesh(	Entity& entity,
					int idParts,
					Parts& controlParts,
					ArgumentContainer& argument,
					const Library::Data::Animation::Parts& dataAnimationParts,
					bool flagPreDraw,
					bool flagPlanarization,
					Library::KindMasking::Kind masking,
					Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
					Control::Animation::Track& controlTrack
				);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};


/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */
/* MEMO: プライオリティキーにパーツIDが混ぜ込まれている理由ですが、 */
/*       SS6の描画規則として、描画順同着の場合にパーツIDが後の（SS6 */
/*       のタイムライン上で下に位置している）のパーツが後に描画され */
/*       る規則をソートに一緒に混ぜ込むためです。                   */
extern const int CountShiftSortKeyPriority;
extern const int MaskSortKeyPriority;
extern const int MaskSortKeyIDParts;

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */
}	/* Animation */
}	/* Control */
}	/* Library */
}	/* SpriteStudio6 */

namespace SpriteStudio6	{
namespace Library	{
namespace Control	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 動的パーツカラー */
/* MEMO: 基本的にパーツカラーと同じではあるのですが、RateAlphaがなかったりなど */
/*       の違いがあるため、パーツカラーとは異なる内容になっています。          */
/*       ※SS6のパーツカラーにRateAlphaがある理由は、後方互換のためなどもあり  */
/*         補佐的な内部パラメータとして残存しているためです。                  */
/*         その補佐的な仕様のため、動的に扱えてしまうと色々と問題が起こること  */
/*         から、動的パーツカラーではRateAlphaの変更はサポートしていません。   */
class AdditionalColor
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum FlagBitStatus	{
		VALID = 0x40000000,
		CHANGE = 0x20000000,

		CLEAR = 0
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	Uint32 /* FlagBitStatus */ Status;
	Library::KindOperationBlend::Kind Operation;
	CPU::Color VertexColor[Library::KindVertex::TERMINATOR2];

	static const CPU::Color ColorClear[Library::KindOperationBlend::TERMINATOR_PARTSCOLOR];

	/* ----------------------------------------------- Functions */
private:
	inline void CleanUp(void)
	{
		Status = FlagBitStatus::CLEAR;
		Operation = Library::KindOperationBlend::NON;
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			VertexColor[i].Set(CPU::Color::White);
		}
	}

protected:
public:
	/* ********************************************************* */
	//! 起動
	/*!
	@param	なし

	@retval	なし

	本クラスを起動します。
	原則、Entity系クラスの内部処理から実行しますので、アプリケー
		ションから直接実行しないでください。
	*/
	bool BootUp(void)
	{
#if 0	/* MEMO: SS6Player for Unityの初期化パラメータ（SS6PUは動的設置で起動直後から効果）。 */
		Operation = Library::KindOperationBlend::MIX;
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			VertexColor[i].Set(ColorClear[Library::KindOperationBlend::MIX]);
		}
#else	/* MEMO: C++版は常にバッファが設置されているので、機能しない内容（ブレンドしない指定）で初期化する。 */
		CleanUp();
#endif

		Status |= FlagBitStatus::VALID;

		return(true);
	}

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし

	本クラスを終了します。
	原則、Entity系クラスの内部処理から実行しますので、アプリケー
		ションから直接実行しないでください。
	*/
	void ShutDown(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! 全頂点で同じ色を設定
	/*!
	@param	operationBlend
		ブレンド種別(NON/MIX/ADD/SUB/MUL)
		NON == アニメーションデータを使用する（動的パーツカラー
			を一時的に使用しないようにする）
	@param	color
		設定する色
		ColorClear[operationBlend]: 効果なしの色を設定

	@retval	なし

	全ての頂点に同じ色を設定します。

	operationBlendはNON・MIX・ADD・SUB・MULのみ使用可能です。
	それ以外の合成方法を与えると設定を無視しますので、注意
		してください。

	また、ColorClearを使用する場合の配列添字にはMIX・ADD・
		SUB・MUL以外を与えないでください（不正メモリを参照
		します）。
	*/
	void SetOverall(Library::KindOperationBlend::Kind operation, const CPU::Color& color);

	/* ********************************************************* */
	//! 四隅の頂点に異なる色を指定
	/*!
	@param	operationBlend
		ブレンド種別(NON/MIX/ADD/SUB/MUL)
		NON == アニメーションデータを使用する（動的パーツカラー
			を一時的に使用しないようにする）
	@param	colorLU
		左上の頂点色
	@param	colorRU
		右上の頂点色
	@param	colorRD
		右下の頂点色
	@param	colorLD
		左下の頂点色

	@retval	なし

	四隅の頂点に別々の色を設定します。

	operationBlendはNON・MIX・ADD・SUB・MULのみ使用可能です。
	それ以外の合成方法を与えると設定を無視しますので、注意
		してください。

	また、ColorClearを使用する場合の配列添字にはMIX・ADD・
		SUB・MUL以外を与えないでください（不正メモリを参照
		します）。
	*/
	void SetVertex(	Library::KindOperationBlend::Kind operation,
					const CPU::Color& colorLU,
					const CPU::Color& colorRU,
					const CPU::Color& colorRD,
					const CPU::Color& colorLD
				);

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	AdditionalColor(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	仮想関数やコンストラクタを通していないといけないメンバを持って
		いないので必須ではないのですが……念のため。
	*/
	~AdditionalColor(void)
	{
		CleanUp();
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* コールバック情報クラス */
/* MEMO: 殆ど構造体のような扱いしかしていません。 */
template<typename _TypeFunction> class CallBack
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
	_TypeFunction Function;
	void* Pointer;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし

	本クラスを未使用状態にします。
	*/
	inline void CleanUp(void)
	{
		Function = nullptr;
		Pointer = nullptr;
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	CallBack(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	※このクラスは継承したりしないので、仮想化していません。
	*/
	~CallBack(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! コールバックが発行可能か？
	/*!
	@param	なし

	@retval	関数値
		true == 可能（コールバック関数が設定されている）
		false == 不可能（コールバック関数が設定されていない）

	本クラスの内容でコールバックを発行できるかどうかを取得します。
	*/
	inline bool IsValid(void)
	{
		return(nullptr != Function);	/* ? true : false */
	}

	/* ********************************************************* */
	//! コールバックの設定
	/*!
	@param	function
		コールバック受領関数
	@param	pointer
		コールバックポインタ

	@retval	なし

	本クラスにコールバックを設定します。
	pointerはコールバック受領関数に渡される任意のポインタで、
		本クラス含めたSS6Playerの機能は本ポインタに頓着しません。
	*/
	inline void Set(_TypeFunction function, void* pointer)
	{
		Function = function;
		Pointer = pointer;
	}

	/* ********************************************************* */
	//! コールバックの取得
	/*!
	@param	function
		コールバック受領関数の格納先
	@param	pointer
		コールバックポインタの格納先

	@retval	function
		コールバック受領関数
	@retval	pointer
		コールバックポインタ

	本クラスに設定されているコールバック情報を取得します。
	*/
	inline void Get(_TypeFunction* function, void** pointer)
	{
		*function = Function;
		*pointer = Pointer;
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* Entity基底（Root） */
/* MEMO: アニメーションとエフェクトのエンティティの基底クラスです。 */
/*       殆ど何をするわけでもないクラスですが、各種別のエンティティ */
/*       機能での共通データなどを定義してあります。                 */
/* MEMO: 名称がEntityではなくRootになっている理由は、SS6Player for  */
/*       Unityがそうだったという以上に、Entityだと名称の衝突が激し  */
/*       くなってしまい、混乱をきたすためです。                     */
class Root
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
	/* MEMO: 親オブジェクトを本クラス（SpriteStudio6::Library::Control::Entity）ではなく、 */
	/*       派生クラスのSpriteStudio6::Entityクラスで持っているのが少々気持ち悪いのですが */
	/*       ……アニメーションやエフェクトの親オブジェクトは「必ずアニメーションオブジェ  */
	/*       クト」である必要があるために、基底である本クラスで（ポリモーフする必要もなけ  */
	/*       れば）アクセスされても困るので……仕様明示性込みで、この指定にしてあります。  */
	SpriteStudio6::Entity* EntityParent;				/* 親アニメーションオブジェクトのエンティティ *//* ※自身が他のインスタンスの子である場合に!=nullptr。 */

	SpriteStudio6::Project* DataProject;				/* プロジェクトデータ管理 */
	Library::Data::Project::CellMap* DataCellMap;		/* セルマップ情報 */

	bool FlagHideForce;									/* 非表示設定 */
	bool FlagPlanarization;								/* 平面化設定 *//* true==パーツのZ軸値を無視してRootと同じZ座標で描画する */

	Float32 RateOpacityForce;							/* 外部から指定するアニメーション全体にかかる不透明度（ほぼインスタンス用・全パーツへの不透明度係数） *//* 1.0f==不透明 */
	CPU::Vector2 RateScaleLocalForce;					/* 外部から指定するローカルスケール（インスタンス用） */

	Library::Control::AdditionalColor* AdditionalColor;	/* 動的パーツカラー */
	void* HandleAdditionalColor;						/* 動的パーツカラー（メモリハンドル） */

	CPU::Vector3 Position;								/* オブジェクトの位置 */
	CPU::Vector3 Rotation;								/* オブジェクトの回転 *//* 回転順序が関係するのでクォータニオンで持っていた方が良いか？ */
	CPU::Vector3 Scaling;								/* オブジェクトのスケール */

	int CountTexture;									/* テクスチャ個数 */
	SpriteStudio6::Texture** Texture;					/* 使用しているテクスチャ群 */
	Sint16* IndexTextureCellMap;						/* セルマップとテクスチャインデックスの変換テーブル */

	/* MEMO: 現状にシェーダ定数バッファは1エンティティに1組で足りるので、エンティティに固定で */
	/*       持って処理しています。                                                           */
	/* MEMO: 現状、パーツごとに必要な定数は頂点データに持っています（シェーダ変更でキャッシュ */
	/*       パージした時のロック期間発生を抑制するため）。                                   */
	ShaderData* ConstantShader;							/* シェーダ定数 */

	/* 更新・描画用の行列バッファ */
	/* MEMO: まともに処理関数群に渡すと引数が多くなるので、構造体にまとめて参照渡しにするためのワークです。 */
	Library::Control::Animation::ArgumentContainer ArgumentContainerControl;

	/* プリミティブバッチング関連 */
	/* MEMO: 描画用モデルはアニメーション・エフェクト共通で使用するので定義自体はここに入っていますが、 */
	/*       扱いそのものは（アニメーション・エフェクトによって）特化せざるを得ないので、継承先の各エン */
	/*       ティティクラスで行ってください（基底である本クラスでは関知していません）。                 */
	PrimitiveBatcher* ModelDraw;						/* メッシュバッチングされたプリミティブ（描画用モデル） */

	/* ----------------------------------------------- Functions */
private:
protected:
	inline void CleanUp(void)
	{
		EntityParent = nullptr;

		DataProject = nullptr;	/* 暫定 */
		DataCellMap = nullptr;	/* 暫定 */

		FlagHideForce = false;
		FlagPlanarization = false;

		RateOpacityForce = 1.0f;
		RateScaleLocalForce.Set(CPU::Vector2::One);

		AdditionalColor = nullptr;
		HandleAdditionalColor = nullptr;

		Position.Set(CPU::Vector3::Zero);
		Rotation.Set(CPU::Vector3::Zero);
		Scaling.Set(CPU::Vector3::One);

		CountTexture = -1;
		Texture = nullptr;
		IndexTextureCellMap = nullptr;

		ConstantShader = nullptr;

//		ArgumentContainerControl

//		ModelDraw = nullptr;
	}

	static inline size_t SizeGetWorkAreaTexture(int countTexture)
	{
		size_t sizeWorkArea = 0;
		sizeWorkArea += Memory::Alignment(sizeof(SpriteStudio6::Texture*) * countTexture, AlignmentMemory::SAFETY);
		sizeWorkArea += Memory::Alignment(sizeof(Sint16*) * countTexture, AlignmentMemory::SAFETY);

		return(sizeWorkArea);
	}

	inline void WorkAreaSetTexture(int countTexture, void* buffer)
	{
		CountTexture = countTexture;
		Uint8* bufferNow = reinterpret_cast<Uint8*>(buffer);
		if(0 >= countTexture)	{
			Texture = nullptr;
			IndexTextureCellMap = nullptr;
		} else {
			/* MEMO: 最終的にbuffer==nullptrである場合は、ここでアロケートできるようにするのも手。 */
			/*       ※ただその場合には、解放時の関数とどこでアロケートしたのかの判別が必要になる  */
			/*         ので注意。                                                                  */
			Texture = reinterpret_cast<SpriteStudio6::Texture**>(bufferNow);
			bufferNow += Memory::Alignment(sizeof(SpriteStudio6::Texture*) * CountTexture, AlignmentMemory::SAFETY);

			IndexTextureCellMap = reinterpret_cast<Sint16*>(bufferNow);
			bufferNow += Memory::Alignment(sizeof(Sint16*) * CountTexture, AlignmentMemory::SAFETY);

			for(int i=0; i<countTexture; i++)	{
				Texture[i] = nullptr;
				IndexTextureCellMap[i] = -1;
			}
		}
	}

public:
	/* MEMO: 本クラスにコンストラクタ・デストラクタはありません。 */
	/*       ※初期化・終了の処理は、派生側に任せています。       */

	/* ********************************************************* */
	//! 本クラスの起動（実行用初期化）
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	派生クラスの同種関数から呼び出してください。
	*/
	inline bool BootUp(void)
	{
		CleanUp();

		return(true);
	}

	/* ********************************************************* */
	//! 本クラスの終了
	/*!
	@param	なし

	@retval	なし

	派生クラスの同種関数から呼び出してください。
	*/
	inline void ShutDown(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! 本クラスの更新
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	派生クラスの同種関数から呼び出してください。
	*/
	inline bool Update(void)
	{
		/* 現時点、何もしていません。 */
		return(true);
	}

	/* ********************************************************* */
	//! 最親エンティティを取得
	/*!
	@param	なし

	@retval	関数値
		最親エンティティのポインタ
		nullptr == 呼出元が最親エンティティ

	エンティティが所属する最親エンティティを取得します。
	※呼出元が最親エンティティである場合にthisを返さないのは、
		EntityEffectからも本関数を呼び出せるため、（Rootを返すこと
		で）キャストミスが起こることを避けるためです。
	*/
	Entity* EntityGetParentHighest(void);

	/* ********************************************************* */
	//! セルマップ数の取得
	/*!
	@param	flagInUse
		false == 現在アニメーションに設定されているプロジェクト
					（元データ）からデータを取得
		true == 現在アニメーションが実際に使用しているセルマップ
					テーブルからデータを取得
		省略時: true

	@retval	関数値
		セルマップ数

	現在設定されているセルマップデータのセルマップ数を取得します。

	※現在flagInUseについては、機能していません（後にDataSetCellMap
		関数の処理が実装された時に、同時に効果対応します）、
		現在は常にfalse指定と同じ挙動をします。
	*/
	int CountGetCellMap(bool flagInUse=true);

	/* ********************************************************* */
	//! セルマップのインデックスを取得
	/*!
	@param	name
		セルマップ名
	@param	flagInUse
		false == 現在アニメーションに設定されているプロジェクト
					（元データ）からデータを取得
		true == 現在アニメーションが実際に使用しているセルマップ
					テーブルからデータを取得
		省略時: true

	@retval	関数値
		セルマップのインデックス
		-1 == 見つからなかった

	指定名称のセルマップのインデックスを返します。

	flagInUseは現在対応していません（常にfalseと同じ動作状態になって
		います）。
	※DataSetCellMap関数を本実装した際に、本フラグも効果するように
		対処します。
	*/
	int IndexGetCellMap(const char* name, bool flagInUse=true);

	/* ********************************************************* */
	//! セルマップ番号から名前を取得
	/*!
	@param	indexCellMap
		セルマップ番号
	@param	flagInUse
		false == 現在アニメーションに設定されているプロジェクト
					（元データ）からデータを取得
		true == 現在アニメーションが実際に使用しているセルマップ
					テーブルからデータを取得
		省略時: true

	@retval	関数値
		セルマップ名
		nullptr == エラー

	セルマップ番号からセルマップ名を返します。

	flagInUseは現在対応していません（常にfalseと同じ動作状態になって
		います）。
	※DataSetCellMap関数を本実装した際に、本フラグも効果するように
		対処します。
	*/
	const char* NameGetCellMap(int indexCellMap, bool flagInUse=true);

	/* ********************************************************* */
	//! セルマップ情報の取得
	/*!
	@param	dataCellMap
		セルマップ情報格納先
	@param	indexCellMap
		セルマップ番号

	@retval	関数値
		セルマップ情報（※dataCellMapと同値です）
		nullptr == 失敗
	@param	dataCellMap
		セルマップ情報

	セルマップの情報を扱うための情報をcellMapに格納します。
	indexCellMapは必ず、「0 ～ (CountGetCellMap()-1)」の値である
		必要があります。

	※現在flagInUseについては、機能していません（後にDataSetCellMap
		関数の処理が実装された時に、同時に効果対応します）、
		現在は常にfalse指定と同じ挙動をします。
	*/
	inline const Library::Data::CellMap::Body* DataGetCellMap(Library::Data::CellMap::Body* dataCellMap, int indexCellMap, bool flagInUse=true)
	{
		if(true == flagInUse)	{
			/* MEMO: 現在未実装。後に積むこと。 */
		}
		/* MEMO: ここから下は元データから取得するための処理。 */

		return(((0 > indexCellMap) || (DataCellMap->CountGetTableCellMap() <= indexCellMap)) ? nullptr : DataCellMap->TableCellMap(dataCellMap, indexCellMap));
	}

	/* ********************************************************* */
	//! セルマップデータを設定
	/*!
	@param	indexCellMap
		セルマップ番号
	@param	dataCellMap
		セルマップデータ

	@retval	関数値
		true == 成功
		false == 失敗

	指定番号のセルマップ（スロット）にセルマップデータを設定します。
	本関数を実行することでアニメーションで使用しているセルマップ
		データを変更することができます。
	※セルマップで使用しているテクスチャまで変更する場合は、別途
		TextureSet関数も使用する必要があります。

	本関数で設定したセルマップの変更は、設定されているプロジェクト
		（元データ）にまで変更が及ぶことはありません（つまり、変更
		が適用されるのは、実行したEntityのみに限られます）。

	※本関数は現在機能しません（常にfalseを返します）。予約関数です。
	*/
	bool DataSetCellMap(int indexCellMap, Library::Data::CellMap::Body& dataCellMap);

	/* ********************************************************* */
	//! セル数の取得
	/*!
	@param	indexCellMap
		セルマップ番号

	@retval	関数値
		セル数

	現在設定されているセルマップデータの指定セルマップに格納されて
		いるセル数を取得します。

	※現在flagInUseについては、機能していません（後にDataSetCellMap
		関数の処理が実装された時に、同時に効果対応します）、
		現在は常にfalse指定と同じ挙動をします。
	*/
	inline int CountGetCell(int indexCellMap, bool flagInUse=true)
	{
		Library::Data::CellMap::Body dataCellMap;
		DataGetCellMap(&dataCellMap, indexCellMap, flagInUse);

		return(dataCellMap.CountTableCell());
	}

	/* ********************************************************* */
	//! セルのインデックス取得
	/*!
	@param	dataCellMap
		セルマップ情報
	@param	nameCellMap
		セル名

	@retval	関数値
		セルのインデックス
		-1 == 失敗

	cellMap中に格納されているセルから指定名のセルを検索し、番号を
		前します。
	*/
	int IndexGetCell(Library::Data::CellMap::Body& dataCellMap, const char* nameCell);

	/* ********************************************************* */
	//! セル情報の取得
	/*!
	@param	dataCell
		セル情報格納先
	@param	dataCellMap
		セルマップ情報
	@param	indexCell
		セル番号

	@retval	関数値
		セル情報（※dataCellと同値です）
		nullptr == 失敗
	@param	dataCell
		セル情報

	cellMap中に格納されているセルの情報群を取得するための情報をcell
		に格納します。
	indexは必ず、「0 ～ (CountGetCellMap()-1)」の値である必要があり
		ます。
	*/
	inline const Library::Data::CellMap::Cell* DataGetCell(Library::Data::CellMap::Cell* dataCell, Library::Data::CellMap::Body& dataCellMap, int indexCell)
	{
		return(((0 > indexCell) || (dataCellMap.CountTableCell() <= indexCell)) ? nullptr : dataCellMap.TableCell(dataCell, indexCell));
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

}	/* Control */
}	/* Library */
}	/* SpriteStudio6 */

