/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_entity_effect.h"

#include "../Library/ss6player_utility.h"
#include <cmath>

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
/*                                                     EntityEffect (Partial) */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! アニメーションの再生開始
bool EntityEffect::AnimationPlay(int timesPlay, float rateTime, int framePerSecond)
{
	if(0 == (Status & FlagBitStatus::VALID))	{	/* 未初期化 */
		/* MEMO: C++版の場合は未初期化でここに来ることは考えづらいのでエラーで帰っておきます。 */
		/*       ※C#（for Unity）では、初期値が設定されているので、強制初期化しています。     */
		return(false);
	}
	if((nullptr == DataCellMap) || (nullptr == DataEffect))	{	/* データが設定されていない */
		return(false);
	}

	/* 再生速度・再生状態の確定 */
	/* MEMO: Do not specify 0 to framePerSecond, when control from animation. */
	RateTime = (true == std::isnan(rateTime)) ? RateTime : rateTime;
	if(0 == framePerSecond)	{
		framePerSecond = DataEffect->CountFramePerSecond();
	}
	FramePerSecond = (Float32)framePerSecond;
	TimePerFrame = 1.0f / FramePerSecond;
//	Frame = 0.0f;
	TimeElapsed = (0.0f > RateTime) ? (FrameRange * TimePerFrame) : 0.0f; 

	/* エンティティ事態のステータスの設定 */
	Status |= FlagBitStatus::PLAYING;
	Status &= ~(FlagBitStatus::CHANGE_TABLEMATERIAL | FlagBitStatus::CHANGE_CELLMAP);

	/* 再生ステータスの設定 */
	StatusPlaying |= (	Library::Control::Animation::Track::FlagBitStatus::PLAYING
						| Library::Control::Animation::Track::FlagBitStatus::PLAYING_START
					);
	StatusPlaying &= ~Library::Control::Animation::Track::FlagBitStatus::PAUSING;

	return(true);
}

/* ********************************************************* */
//! アニメーションの停止
void EntityEffect::AnimationStop(bool flagJumpEnd)
{
	if(false == IsValid())	{
		return;
	}
	if(0 == (Status & FlagBitStatus::PLAYING))	{
		return;
	}

	/* エンティティ自体のステータスの設定 */
	Status &= ~FlagBitStatus::PLAYING;

	/* 再生ステータスを設定 */
	StatusPlaying &= ~(	Library::Control::Animation::Track::FlagBitStatus::PLAYING
						| Library::Control::Animation::Track::FlagBitStatus::PLAYING_START
						| Library::Control::Animation::Track::FlagBitStatus::PAUSING
					);
}

/* ********************************************************* */
//! アニメーションの一時停止状態の設定
bool EntityEffect::AnimationPause(bool flagSwitch)
{
	if(false == IsValid())	{
		return(false);
	}
	if(0 == (StatusPlaying & Library::Control::Animation::Track::FlagBitStatus::PLAYING))	{
		return(false);
	}

	if(true == flagSwitch)	{
		if(0 == (StatusPlaying & Library::Control::Animation::Track::FlagBitStatus::PAUSING))	{
			StatusPlaying |= Library::Control::Animation::Track::FlagBitStatus::PAUSING;
		}
	} else {
		if(0 != (StatusPlaying & Library::Control::Animation::Track::FlagBitStatus::PAUSING))	{
			StatusPlaying &= ~Library::Control::Animation::Track::FlagBitStatus::PAUSING;
		}
	}

	return(true);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
