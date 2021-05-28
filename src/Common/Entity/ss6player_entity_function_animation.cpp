/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_entity.h"
#include "../Library/ss6player_utility.h"

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
/*                                                           Entity (Partial) */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! アニメーションの個数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Entity::CountGetAnimation(void)

/* ********************************************************* */
//! 名前からアニメーションのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::IndexGetAnimation(const char* name)
{
	if(nullptr == DataAnimation)	{
		return(-1);
	}
	return(DataAnimation->IndexGetAnimation(name));
}

/* ********************************************************* */
//! アニメーション番号から名前を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
const char* Entity::NameGetAnimation(int indexAnimation)
{
	if(nullptr == DataAnimation)	{
		return(nullptr);
	}
	if((0 > indexAnimation) || (CountGetAnimation() <= indexAnimation))	{
		return(nullptr);
	}

	Library::Data::Animation::Body dataAnimationBody;
	DataAnimation->TableAnimation(&dataAnimationBody, indexAnimation);
	return(dataAnimationBody.Name());
}

/* ********************************************************* */
//! アニメーションの再生開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::AnimationPlay(	int indexTrack,
							int indexAnimation,
							int timesPlay,
							int frameOffset,
							float rateTime,
							Library::KindStylePlay::Kind style,
							const char* labelRangeStart,
							int frameRangeOffsetStart,
							const char* labelRangeEnd,
							int frameRangeOffsetEnd,
							int framePerSecond,
							Library::Data::Project::Animation* dataAnimation
						)
{
	if(0 == (Status & FlagBitStatus::VALID))	{	/* 未起動 */
		return(false);
	}

	/* エンティティの状態を確定 */
	int countTrack = CountTrack;
	if(0 > indexTrack)	{	/* 全トラック対象 */
		/* MEMO: 全トラックの再生とトランジションを停止・全てのパーツをトラック0に接続します。 */
		for(int i=0; i<countTrack; i++)	{
			TableControlTrack[i].Stop();
			TableControlTrack[i].Transition(-1, 0.0f);
		}
		TrackConnectParts(-1, 0);
		indexTrack = 0;
	} else {	/* 特定トラック */
		/* MEMO: 指定トラックの状態のみ変更します。 */
		if(countTrack <= indexTrack)	{
		 	return(false);
		}
		TableControlTrack[indexTrack].Stop();
		TableControlTrack[indexTrack].Transition(-1, 0.0f);
	}

	/* 再生するアニメーションパックを確定 */
	if(nullptr == dataAnimation)	{
		dataAnimation = TableInformationPlay[indexTrack].DataAnimation;
	}
	if(nullptr == dataAnimation)	{
		dataAnimation = DataAnimation;
	}

	/* 再生アニメーション番号を確定 */
	indexAnimation = (0 > indexAnimation) ? TableInformationPlay[indexTrack].IndexAnimation : indexAnimation;
	if((0 > indexAnimation) || (CountGetAnimation() <= indexAnimation))	{
	 	return(false);
	}
	Library::Data::Animation::Body dataAnimationBody;
	DataAnimation->TableAnimation(&dataAnimationBody, indexAnimation);

	/* 再生回数を確定 */
	timesPlay = (0 > timesPlay) ? TableInformationPlay[indexTrack].TimesPlay : timesPlay;

	/* 再生開始オフセット位置を確定 */
	frameOffset = (INT_MIN == frameOffset) ? TableInformationPlay[indexTrack].Frame : frameOffset;

	/* 再生速度を確定 */
	rateTime = (true == isnan(rateTime)) ? TableInformationPlay[indexTrack].RateTime : rateTime;

	/* 再生方向を確定 */
	bool flagPingPong = false;
	switch(style)	{
		case Library::KindStylePlay::NORMAL:
			flagPingPong = false;
			break;

		case Library::KindStylePlay::PINGPONG:
			flagPingPong = true;
			break;

		case Library::KindStylePlay::NO_CHANGE:
		default:	/* ClangのWarning避け */
			flagPingPong = TableInformationPlay[indexTrack].FlagPingPong;
			break;
	}

	/* 開始・終了位置ラベルの確定 */
	if(nullptr == labelRangeStart)	{
		labelRangeStart = TableInformationPlay[indexTrack].LabelStart.c_str();
	}
	if(true == Library::Utility::Text::IsNullOrEmpty(labelRangeStart))	{	/* 空文字 */
		/* MEMO: アニメーションを先頭から再生 */
		labelRangeStart = Library::Data::Animation::Label::TableNameLabelReserved[Library::Data::Animation::Label::KindLabelReserved::START];
	}
	frameRangeOffsetStart = (INT_MIN == frameRangeOffsetStart) ? TableInformationPlay[indexTrack].FrameOffsetStart : frameRangeOffsetStart;

	if(nullptr == labelRangeEnd)	{
		labelRangeEnd = TableInformationPlay[indexTrack].LabelEnd.c_str();
	}
	if(true == Library::Utility::Text::IsNullOrEmpty(labelRangeEnd))	{
		/* MEMO: アニメーションを終端まで再生 */
		labelRangeEnd = Library::Data::Animation::Label::TableNameLabelReserved[Library::Data::Animation::Label::KindLabelReserved::END];
	}
	frameRangeOffsetEnd = (INT_MAX == frameRangeOffsetEnd) ? TableInformationPlay[indexTrack].FrameOffsetEnd : frameRangeOffsetEnd;

	/* 再生FPSの確定 */
	if(0 >= framePerSecond)	{
		framePerSecond = dataAnimationBody.FramePerSecond();
	}

	/* Update play-Information */
//	TableInformationPlay[indexTrack].FlagSetInitial = 
//	TableInformationPlay[indexTrack].FlagStopInitial = 
	TableInformationPlay[indexTrack].DataAnimation = dataAnimation;
	TableInformationPlay[indexTrack].NameAnimation = dataAnimationBody.Name();	/* std::stringへの代入なのでディープコピーです */
	TableInformationPlay[indexTrack].IndexAnimation = indexAnimation;
	TableInformationPlay[indexTrack].FlagPingPong = flagPingPong;
	TableInformationPlay[indexTrack].LabelStart = labelRangeStart;	/* std::stringへの代入なのでディープコピーです */
	TableInformationPlay[indexTrack].FrameOffsetStart = frameRangeOffsetStart;
	TableInformationPlay[indexTrack].LabelEnd = labelRangeEnd;	/* std::stringへの代入なのでディープコピーです */
	TableInformationPlay[indexTrack].FrameOffsetEnd = frameRangeOffsetEnd;
	TableInformationPlay[indexTrack].Frame = frameOffset;
	TableInformationPlay[indexTrack].TimesPlay = timesPlay;
	TableInformationPlay[indexTrack].RateTime = rateTime;

	/* Get range */
	int frameRangeStart = 0;
	int frameRangeEnd = 0;
	dataAnimationBody.FrameRangeGet(	&frameRangeStart, &frameRangeEnd,
										labelRangeStart,
										frameRangeOffsetStart,
										labelRangeEnd,
										frameRangeOffsetEnd
								);

	/* Adjust top rame */
	int frameTop = frameRangeStart + frameOffset;
	if((frameRangeStart > frameTop) || (frameRangeEnd < frameTop))
	{	/* Range Error */
		frameTop = frameRangeStart;
	}

	/* Update Status */
	Status |= FlagBitStatus::PLAYING;
	Status &= ~(FlagBitStatus::CHANGE_TABLEMATERIAL | FlagBitStatus::CHANGE_CELLMAP);

	/* Refresh Control-Parts */
	int countControlParts = CountParts;
	for(int i=0; i<countControlParts; i++)	{
		if(TableControlParts[i].IndexControlTrack == indexTrack)	{
			TableControlParts[i].AnimationChange(*this);
		}
	}

	/* Start Playing */
	return(TableControlTrack[indexTrack].Start(	*this,
												*dataAnimation,
												indexAnimation,
												frameRangeStart,
												frameRangeEnd,
												frameTop,
												framePerSecond,
												rateTime,
												0.0f,
												flagPingPong,
												timesPlay
											)
		);
}

/* ********************************************************* */
//! アニメーションの停止
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::AnimationStop(int indexTrack, bool flagJumpEnd, bool flagEngageTransition)
{
	int countTrack = CountTrack;
	if(0 > indexTrack)	{	/* 全トラック対象 */
		for(int i=0; i<countTrack; i++)	{
			if(true == AnimationStopMain(i, flagJumpEnd, flagEngageTransition))	{
				Status &= ~FlagBitStatus::PLAYING;
				break;
			}
		}
	} else {	/* 特定トラック対象 */
		if(countTrack <= indexTrack)	{	/* エラー */
			return;
		}

		if(true == AnimationStopMain(indexTrack, flagJumpEnd, flagEngageTransition))	{
			Status &= ~FlagBitStatus::PLAYING;
		}
	}
}
bool Entity::AnimationStopMain(int indexTrack, bool flagJumpEnd, bool flagEngageTransition)
{
	/* マスタを停止 */
	if(true == flagJumpEnd)	{	/* 末尾フレームにジャンプ */
		Float32 timeTotal;
		Float32 timeInRange;
		if(true == TableControlTrack[indexTrack].TimeGetRemain(&timeTotal, &timeInRange))	{
			TableControlTrack[indexTrack].Stop();
			TableControlTrack[indexTrack].TimeElapseReplacement = timeTotal;
			TableControlTrack[indexTrack].TimeElapseInRangeReplacement = timeInRange;
		}
	} else {	/* 現在位置にとどまる */
		TableControlTrack[indexTrack].Stop();
	}

	/* スレーブを停止 */
	if(true == flagEngageTransition)	{	/* トランジションも終了 */
		int indexTrackSlave = TableControlTrack[indexTrack].IndexTrackSlave;
		if(0 <= indexTrackSlave)	{
			TrackChangeSlaveToMaster(indexTrack, indexTrackSlave);
			TableControlTrack[indexTrackSlave].Stop();
		}
	}

	/* 再生状態をチェック */
	int countTrack = CountTrack;
	bool flagStopAll = true;
	for(int i=0; i<countTrack; i++)	{
		if(true == TableControlTrack[i].StatusGetIsPlaying())	{
			flagStopAll = false;
			break;	/* i-Loop */
		}
	}

	return(flagStopAll);
}

/* ********************************************************* */
//! アニメーションの一時停止状態の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::AnimationPause(int indexTrack, bool flagSwitch)
{
	int countTrack = CountTrack;
	bool flagSuccess = true;
	if(0 > indexTrack)	{	/* 全トラック */
		for(int i=0; i<countTrack; i++)	{
			if(true == TableControlTrack[i].StatusGetIsPlaying())	{
				flagSuccess &= AnimationPauseMain(i, flagSwitch);
			}
		}
	} else {	/* 特定トラック */
		if(countTrack <= indexTrack)	{
			return(false);
		}
		if(false == TableControlTrack[indexTrack].StatusGetIsPlaying())	{
			return(false);
		}

		return(AnimationPauseMain(indexTrack, flagSwitch));
	}

	return(flagSuccess);
}
bool Entity::AnimationPauseMain(int indexTrack, bool flagSwitch)
{
	bool flagSuccess = true;

	/* マスタトラック */
	if(true == TableControlTrack[indexTrack].StatusGetIsPlaying())	{
		flagSuccess &= TableControlTrack[indexTrack].Pause(flagSwitch);
	}

	/* スレーブトラック */
	int indexTrackSlave = TableControlTrack[indexTrack].IndexTrackSlave;
	if(0 <= indexTrackSlave)	{
		flagSuccess &= TableControlTrack[indexTrackSlave].Pause(flagSwitch);
	}

	return(flagSuccess);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
