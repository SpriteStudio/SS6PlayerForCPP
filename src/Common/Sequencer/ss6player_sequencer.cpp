/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "../Entity/ss6player_entity.h"
#include "../Project/ss6player_project.h"
#include "ss6player_sequencer.h"

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
/*                                                        Sequencer::DataStep */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Sequencer::DataStep::CleanUp(void)

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Sequencer::DataStep::IsValid(void)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                  Sequencer */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Sequencer::CleanUp(void)
{
	Status = FlagBitStatus::CLEAR;
//	Entity

	DataProject = nullptr;

	DataSequencePack = nullptr;
	DataSequence.Value = nullptr;
	IndexStep = -1;

	FlagHideForce = false;
	RateTime = 0.0f;

	CallBackPlayEnd.CleanUp();
	CallBackDecodeStep.CleanUp();
}

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Sequencer::Sequencer(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Sequencer::~Sequencer(void)
{
	/* MEMO: 念のため終了処理を呼んでおきます。 */
	ShutDown();
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::IsValid(void)
{
	/* MEMO: DataSequenceはPlay時に与えられるのでチェックしない。 */
	if(	(0 == (Status & FlagBitStatus::VALID))
		|| (nullptr == DataProject)
		|| (nullptr == DataSequencePack)
	)	{
		return(false);
	}
	if(false == DataProject->IsValid())	{
		return(false);
	}
	if(false == DataSequencePack->IsValid())	{
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! 本クラスの起動（実行用初期化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::BootUp(SpriteStudio6::Project& project, int countTrack)
{
	CleanUp();

	/* プロジェクトを設定 */
	if(false == project.IsValid())	{
		return(false);
	}
	DataProject = &project;

	/* 再生情報の初期設定 */
	FlagHideForce = false;
	RateTime = 1.0f;

	/* エンティティの起動 */
	if(false == Entity.BootUp(*DataProject, countTrack))	{	/* 失敗 */
		return(false);
	}

	/* ステータス変更 */
	Status |= FlagBitStatus::VALID;

	return(true);
}

/* ********************************************************* */
//! 本クラスの終了（未使用化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Sequencer::ShutDown(void)
{
	/* エンティティの終了 */
	Entity.ShutDown();

	CleanUp();
}

/* ********************************************************* */
//! 再生状態の定常更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::Update(float timeElapsed)
{
	if(false == IsValid())	{
		return(true);
	}
	/* MEMO: ここでPLAYINGを判定するのは若干微妙に思える（判定を考え直すこと）。 */
	if(0 == (Status & FlagBitStatus::PLAYING))	{
		/* MEMO: Updateをしたことにして何もしないのでtrueを返しています。 */
		return(true);
	}

	if(0 != (Status & FlagBitStatus::STEP_UPDATE))	{	/* ステップが変更要請が出ている */
		/* 更新したステップの内容をエンティティに反映 */
		StepProgress();
	}

	/* MEMO: 強制非表示状態でもアニメーションは進行させています。 */
	/* MEMO: rateTimeはエンティティに与えずに自前で計算するようにしています。 */
	/*       ※シーケンスの再生速度とエンティティの再生速度は目的を分けておい */
	/*         た方が良いと（現在は）判断しています。                         */
	Entity.FlagHideForce = FlagHideForce;	/* 非表示状態 */
	timeElapsed *= RateTime;
	if(false == Entity.Update(timeElapsed, Library::KindMasking::FOLLOW_DATA))	{
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! 描画関数
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Sequencer::Draw(	TypeDrawCommandList commandList,
										const CPU::Matrix4x4& matrixProjection,
										const CPU::Matrix4x4& matrixView,
										const CPU::Matrix4x4& matrixWorld
								)
{
	if(false == IsValid())	{
		return(commandList);
	}
	/* MEMO: ここでPLAYINGを判定するのは若干微妙に思える（判定を考え直すこと）。 */
	if(0 == (Status & FlagBitStatus::PLAYING))	{
		return(commandList);
	}
	TypeDrawCommandList commandListNew = Entity.Draw(	commandList,
														matrixProjection,
														matrixView,
														matrixWorld
												);

	if(nullptr == commandListNew)	{
		/* MEMO: エンティティの描画からエラーが返ってきた時は、明確にエラーを上位に伝えておく。 */
		return(DrawCommandListInvalid);
	}

	return(commandList);
}

/* ********************************************************* */
//! シーケンスパック数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Sequencer::CountGetPack(void)
{
	/* MEMO: ここはIsValidでは判定しないこと。 */
	if(nullptr == DataProject)	{
		return(-1);
	}
	if(false == DataProject->IsValid())	{
		return(-1);
	}

	return(DataProject->CountGetSequencePack());
}

/* ********************************************************* */
//! 名前からシーケンスパックのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Sequencer::IndexGetPack(const char* name)
{
	/* MEMO: ここはIsValidでは判定しないこと。 */
	if(nullptr == DataProject)	{
		return(-1);
	}
	if(false == DataProject->IsValid())	{
		return(-1);
	}

	return(DataProject->IndexGetSequencePack(name));
}

/* ********************************************************* */
//! シーケンスパックを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::PackSet(int index)
{
	/* MEMO: ここはIsValidでは判定しないこと。 */
	if(nullptr == DataProject)	{
		return(false);
	}
	if(false == DataProject->IsValid())	{
		return(false);
	}
	if((0 > index) || (DataProject->CountGetSequencePack() <= index))	{
		return(false);
	}

	Library::Data::Project::Sequence* sequence = &DataProject->Sequence[index];
	if(false == sequence->IsValid())	{
		return(false);
	}
	DataSequencePack = sequence;

	return(true);
}

/* ********************************************************* */
//! シーケンスパック内のシーケンス数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Sequencer::CountGetSequence(void)
{
	/* MEMO: ここはIsValidでは判定しないこと。 */
	if(nullptr == DataProject)	{
		return(false);
	}
	if(false == DataProject->IsValid())	{
		return(false);
	}


	if(nullptr == DataSequencePack)	{
		return(false);
	}
	if(false == DataSequencePack->IsValid())	{
		return(false);
	}

	return(DataSequencePack->CountGetSequence());
}

/* ********************************************************* */
//! 名前からシーケンスのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Sequencer::IndexGetSequence(const char* name)
{
	if(false == IsValid())	{
		return(false);
	}

	return(DataSequencePack->IndexGetSequence(name));
}

/* ********************************************************* */
//! 指定シーケンスのステップ数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Sequencer::CountGetStep(int indexSequence)
{
	if(false == IsValid())	{
		return(false);
	}

	/* MEMO: 再生中に呼ばれることもあるのと、使い捨てなのでDataSequenceを使用しないこと。 */
	Library::Data::Sequence::Body sequence;
	DataSequencePack->SequenceGet(&sequence, indexSequence);

	return(sequence.CountGetStep());
}

/* ********************************************************* */
//! シーケンスの再生開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::Play(int indexSequence, float rateTime, int step)
{
	if(false == IsValid())	{
		return(false);
	}

	/* 再生情報の設定 */
	DataSequencePack->SequenceGet(&DataSequence, indexSequence);
	RateSetTime(RateTime);
	StepSet(step);
	Status |= FlagBitStatus::PLAYING;

	/* MEMO: ここではエンティティに直接反映を行いません（Updateで行っています）。 */

	return(true);
}
bool Sequencer::StepProgress(void)
{
	DataStep dataStepCallBack;
	dataStepCallBack.CleanUp();

	/* ステップの再生情報を取得 */
	if(false == DataGetStep(&dataStepCallBack, IndexStep))	{
		/* MEMO: このエラーの場合どうしたものか……（エンティティを組んだ後SSAEを削除するとありえる）。*/
		return(false);
	}

	/* コールバックを発行 */
	if(true == CallBackDecodeStep.IsValid())	{
		int indexStepNext = CallBackDecodeStep.Function(	dataStepCallBack, 
															*this,
															IndexStep,
															CallBackDecodeStep.Pointer
													);
		if(0 <= indexStepNext)	{	/* ステップ強制移動 */
			if(false == DataGetStep(&dataStepCallBack, indexStepNext))	{
				/* MEMO: このエラーの場合どうしたものか……（エンティティを組んだ後SSAEを削除するとありえる）。*/
				return(false);
			}
			IndexStep = indexStepNext;
		} else {	/* ステップは移動せず */
			if(false == dataStepCallBack.IsValid())	{	/* 再生停止が返ってきた */
				/* MEMO: 現時点位置で停止する。 */
				Entity.AnimationStop(-1);

				return(true);
			}
		}
	}

	/* エンティティにアニメーションを設定 */
	/* MEMO: 同じアニメーションパック（パーツ構成）の場合、PackSetは何もしないで */
	/*       帰ってくるので無造作に呼び出しています。                            */
	/* MEMO: 再生速度はシーケンサ側で計算しているので、エンティティ側は等速再生を指定しています。 */
	Entity.PackSet(dataStepCallBack.IndexAnimationPack);
	Entity.CallBackPlayEnd.Set(FunctionCallBackPlayEnd, reinterpret_cast<void*>(this));	/* 再生終了コールバックを設定 */
	if(false == Entity.AnimationPlay(	-1,									/* 全トラック指定（トラック0） */
										dataStepCallBack.IndexAnimation,	/* 再生アニメーション */
										dataStepCallBack.CountPlay,			/* 再生回数 */
										0,									/* 先頭フレーム強制オフセット（未使用） */
										1.0f								/* 再生速度 */
								)
	)	{	/* 失敗 */
		return(false);
	}

	/* 一時停止状態の再設定 */
	/* MEMO: AnimationPlayを行うと一時停止状態が解除されるので……。 */
	if(0 != (Status & FlagBitStatus::PAUSING))	{
		Entity.AnimationPause(-1, true);
//	} else {
//		Entity.AnimationPause(-1, false);
	}

	/* ステータスを変更 */
	Status &= ~FlagBitStatus::STEP_UPDATE;

	return(true);
}
bool Sequencer::DataGetStep(DataStep* data, int indexStep)
{
	Library::Data::Sequence::Step dataStep;
	DataSequence.Step(&dataStep, indexStep);

	const char* name;
	name = dataStep.NameAnimationPack();	/* アニメーションパック名 */
	data->IndexAnimationPack = DataProject->IndexGetAnimationPack(name);
	if(0 > data->IndexAnimationPack)	{
		/* MEMO: このエラーの場合どうしたものか……（エンティティを組んだ後SSAEを削除するとありえる）。*/
		return(false);
	}

	name = dataStep.NameAnimation();	/* アニメーション名 */
	data->IndexAnimation = DataProject->Animation[data->IndexAnimationPack].IndexGetAnimation(name);
	if(0 > data->IndexAnimation)	{
		/* MEMO: このエラーの場合どうしたものか……（エンティティを組んだ後SSAE/アニメーションを削除するとありえる）。*/
		return(false);
	}

	data->CountPlay = dataStep.CountRepeat();	/* 再生回数 */

	return(true);
}

/* ********************************************************* */
//! シーケンスの再生停止
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Sequencer::Stop(bool flagJumpEndStep, bool flagJumpEndFrame)
{
	/* MEMO: 今はflagJumpEndStepを見ていない。 */

	/* エンティティを停止 */
	Entity.AnimationPause(-1, false);	/* ポーズを解除しておく */
	Entity.AnimationStop(flagJumpEndFrame);

	/* ステータスを変更 */
	Status &= ~(	FlagBitStatus::PLAYING
					| FlagBitStatus::PAUSING
					| FlagBitStatus::STEP_UPDATE
			);
}

/* ********************************************************* */
//! シーケンスの再生一時停止状態の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::PauseSet(bool flagSwitch)
{
	if(false == IsValid())	{
		return(false);
	}
	if(false == StatusGetPlaying())	{	/* 再生中ではない */
		return(false);
	}

	Status |= FlagBitStatus::PAUSING;

	return(Entity.AnimationPause(-1, flagSwitch));
}

/* ********************************************************* */
//! シーケンスの再生速度の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::RateSetTime(float rateTime)
{
	if(false == IsValid())	{
		return(false);
	}

	/* MEMO: NaNや0.0fチェックを入れるか？ */
	RateTime =  rateTime;

	return(true);
}

/* ********************************************************* */
//! シーケンスの再生ステップの設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Sequencer::StepSet(int step)
{
	if(false == IsValid())	{
		return(false);
	}

	int countStep = DataSequence.CountGetStep();
	if(0 > step)	{	/* 末尾指定 */
		step = countStep - 1;
	} else {	/* 通常指定 */
		if(countStep <= step)	{	/* 不正ステップ */
			DataSequence.Value = nullptr;
			return(false);
		}
	}
	IndexStep = step;

	Status |= FlagBitStatus::STEP_UPDATE;

	return(true);
}

/* ********************************************************* */
//! 再生中かのステータスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Sequencer::StatusGetPlaying(void)

/* ********************************************************* */
//! 一時停止中かのステータスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Sequencer::StatusGetPausing(void)

/* ********************************************************* */
//! エンティティからの再生終了コールバック受領関数
/* --------------------------------------------------------- */
/* MEMO:
 * pointerには、Sequencerのインスタンスのポインタが入ってきます。
 * EntityのCallBackPlayEndは再生が終了した再生状況が安定した状態
 * でコールバックされるので、この中でアニメーションの再生状態を
 * 変更しても構わない。
 * ※CallBackPlayEndTrackやCallBackUserは再生状態の更新中にコー
 *   ルバックが発生するので、コールバック受領関数の中で再生状態
 *   を変更すると挙動が不安定になるので注意してください。
 */
void Sequencer::FunctionCallBackPlayEnd(SpriteStudio6::Entity& entity, void* pointer)
{
	/* シーケンサのインスタンスを取得 */
	Sequencer& sequencer = *(reinterpret_cast<Sequencer*>(pointer));

	/* MEMO: エンティティは（再生終了コールバックなので）停止しているので、 */
	/*       特段何も考えずに放置で構わない。                               */

	/* ステップ数のチェック */
	/* MEMO: 再生方向を見ながら+1するか-1するか決める（今は単方向）。 */
	sequencer.IndexStep++;
	int countStep = sequencer.DataSequence.CountGetStep();
	if(countStep <= sequencer.IndexStep)	{	/* 範囲外（=終了） */
		/* 再生終了コールバック */
		/* MEMO: ここで発行する場合、終了後挙動指定がKEEP以外だと */
		/*       何度もかかってしまうが……どうしたものか。       */
		if(true == sequencer.CallBackPlayEnd.IsValid())	{
			sequencer.CallBackPlayEnd.Function(	sequencer,
												sequencer.CallBackPlayEnd.Pointer
											);
		}

		/* 再生終了後の挙動 */
		switch(sequencer.DataSequence.TypeGetPlayEnd())	{
			case Library::Data::Sequence::Body::KindTypePlayEed::LAST:
				sequencer.IndexStep = countStep - 1;
				break;

			case Library::Data::Sequence::Body::KindTypePlayEed::TOP:
				sequencer.IndexStep = 0;
				break;

			case Library::Data::Sequence::Body::KindTypePlayEed::KEEP:
			default:
				/* MEMO: 再生状態を変更しない。 */
				return;
		}
	}

	/* ステータスを更新 */
	sequencer.Status |= FlagBitStatus::STEP_UPDATE;
}

}	/* SpriteStudio6 */
