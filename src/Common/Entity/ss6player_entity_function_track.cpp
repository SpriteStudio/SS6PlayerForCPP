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
/*                                                           Entity (Partial) */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 再生トラックの再生状態を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::StatusGetPlay(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(false);
	}
	if(0 > indexTrack)	{
		return(0 != (Status & FlagBitStatus::PLAYING));	/* ? true : false */
	}

	if(CountTrack <= indexTrack)	{
		return(false);
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	return(controlTrack->StatusGetIsPlaying());
}

/* ********************************************************* */
//! 再生トラックの一時停止状態を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::StatusGetPause(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(false);
	}

	Library::Control::Animation::Track* controlTrack;
	if(0 > indexTrack)	{
		for(int i=0; i<CountTrack; i++)	{
			controlTrack = &TableControlTrack[i];
			if(true == controlTrack->StatusGetIsPlaying())	{
				if(true == controlTrack->StatusGetIsPausing())	{
					return(true);
				}
			}
		}

		return(false);
	}

	if(CountTrack <= indexTrack)	{
		return(false);
	}

	controlTrack = &TableControlTrack[indexTrack];
	return(controlTrack->StatusGetIsPausing());
}

/* ********************************************************* */
//! 再生トラック数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::CountGetTrack(void)
{
	return((nullptr != TableControlTrack) ? CountTrack : -1);
}

/* ********************************************************* */
//! 再生トラック数の変更（再生トラックの再起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::TrackReboot(int count)
{
	/* MEMO: 現在未対応（対応途中）。 */
	/* MEMO: Projectのワークエリアの構成を変更して・転送する必要がある。 */
	return(false);
}

/* ********************************************************* */
//! トラックにパーツを接続
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::TrackConnectParts(int idParts, int indexTrack, bool flagChildParts)
{

	int countParts = CountParts;
	if(0 > idParts)	{	/* 全パーツ対象 */
		/* MEMO: この場合は「flagChildParts」の指定は無視されます。 */
		for(int i=0; i<countParts; i++)	{
			TableControlParts[i].IndexControlTrack = indexTrack;
		}
	} else {	/* 特定パーツ対象 */
		if(countParts <= idParts)	{	/* パーツ数が異常 */
			return(false);
		}
		if(true == flagChildParts)	{	/* 子も連動 */
			TrackConnectPartsInvolveChildren(idParts, indexTrack);
		} else {	/* 自身のみ */
			TableControlParts[idParts].IndexControlTrack = indexTrack;
		}
	}

	return(true);
}
void Entity::TrackConnectPartsInvolveChildren(int idParts, int indexTrack)
{
	TableControlParts[idParts].IndexControlTrack = indexTrack;

	Library::Data::Parts::Animation dataPartsAnimation;
	DataAnimation->TableParts(&dataPartsAnimation, idParts);

	int countPartsChild = dataPartsAnimation.CountTableIDChild();
	for(int i=0; i<countPartsChild; i++)	{
		/* MEMO: 自身で再帰しています。 */
		TrackConnectPartsInvolveChildren(dataPartsAnimation.IDGetChild(i), indexTrack);
	}
}

/* ********************************************************* */
//! 再生アニメーションを合成遷移
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::TrackTransition(int indexTrack, int indexTrackSlave, float time, bool flagCancelPauseAfterTransition)
{
	if(nullptr == TableControlTrack)	{
		return(false);
	}

	Library::Control::Animation::Track* controlTrack;
	if(0 > indexTrack)	{	/* 全トラック対象 */
		if(0 <= indexTrackSlave)	{	/* スレーブが指定されている */
			return(false);
		}

		/* 全トラックのトランジションを停止 */
		for(int i=0; i<CountTrack; i++)	{
			controlTrack = &TableControlTrack[i];
			indexTrackSlave = controlTrack->IndexTrackSlave;
			if(0 <= indexTrackSlave)	{
				TableControlTrack[indexTrackSlave].Stop();
				controlTrack->Transition(-1, 0.0f);
			}
		}

		return(true);
	}

	if(CountTrack <= indexTrack)	{	/* マスタ指定が不正 */
		return(false);
	}
	controlTrack = &TableControlTrack[indexTrack];
	if(0 > indexTrackSlave)	{	/* トランジションのキャンセル */
		indexTrackSlave = controlTrack->IndexTrackSlave;
		TableControlTrack[indexTrackSlave].Stop();
		controlTrack->Transition(-1, 0.0f);

		return(true);
	}
	if(CountTrack <= indexTrackSlave)	{	/* スレーブ指定が不正 */
		return(false);
	}

	if(false == TableControlTrack[indexTrackSlave].StatusGetIsPlaying())	{	/* スレーブが再生中でない */
		return(false);
	}

	if(0 <= controlTrack->IndexTrackSlave)	{	/* マスタがトランジション中 */
		return(false);
	}
	if(0.0f >= time)	{	/* 時間指定が不正 */
		return(false);
	}

	/* マスタをトランジションモードに設定 */
	controlTrack->StatusSetIsTransitionStart(true);
	controlTrack->StatusSetIsTransitionCancelPause(flagCancelPauseAfterTransition);

	return(controlTrack->Transition(indexTrackSlave, time));
}

/* ********************************************************* */
//! 再生中のアニメーション番号を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::IndexGetAnimation(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(-1);
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(-1);
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	return(controlTrack->ArgumentContainer.IndexAnimation);
}

/* ********************************************************* */
//! パーツに接続されている再生トラック番号を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::IndexGetTrack(int idParts)
{
	if((nullptr == TableControlTrack) || (nullptr == TableControlParts))	{
		return(-1);
	}

	if((0 > idParts) || (CountParts <= idParts))	{
		return(-1);
	}

	return(TableControlParts[idParts].IndexControlTrack);
}

/* ********************************************************* */
//! スレーブ再生トラックの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::IndexGetTrackSlave(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(-1);
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(-1);
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	return(controlTrack->IndexTrackSlave);
}

/* ********************************************************* */
//! 再生中のアニメーションのFPSを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::FramePerSecondGet(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(-1);
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(-1);
	}

	return(TableControlTrack[indexTrack].FramePerSecond);
}

/* ********************************************************* */
//! 再生中のアニメーション再生速度の変更
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::RateTimeSet(int indexTrack, float rateTime)
{
	if(nullptr == TableControlTrack)	{
		return(false);
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(0 > indexTrack)	{	/* 全トラック対象 */
		for(int i=0; i<CountTrack; i++)	{
			controlTrack = &TableControlTrack[i];
			if(true == controlTrack->StatusGetIsPlaying())	{
				controlTrack->RateTime = rateTime;
			}
		}
	} else {	/* 特定トラック */
		Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];

		if(CountTrack <= indexTrack)	{
			return(false);
		}
		if(false == controlTrack->StatusGetIsPlaying())	{	/* 再生していない */
			return(false);
		}
		controlTrack->RateTime = rateTime;
	}

	return(true);
}

/* ********************************************************* */
//! 再生中のアニメーション再生速度の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
float Entity::RateTimeGet(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
//	if(true == controlTrack->StatusGetIsPlaying())	{
//		return(std::numeric_limits<float>::quiet_NaN());
//	}

	return(controlTrack->RateTime);
}

/* ********************************************************* */
//! 再生中のアニメーション再生の残り再生回数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::PlayTimesGetRemain(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(-1);
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(-1);
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(false == controlTrack->StatusGetIsPlaying())	{
		return(-1);
	}

	return(controlTrack->TimesPlay);
}

/* ********************************************************* */
//! 再生カーソル位置最大値の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
float Entity::RangeGetCursor(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(false == controlTrack->StatusGetIsPlaying())	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	float timeRange = controlTrack->TimeRange;
	if(true == controlTrack->StatusGetIsPlayStylePingpong())	{	/* 往復再生 */
		timeRange *= 2.0f;
	}

	return(timeRange);
}

/* ********************************************************* */
//! 再生カーソル位置の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
float Entity::CursorGet(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(false == controlTrack->StatusGetIsPlaying())	{
		return(std::numeric_limits<float>::quiet_NaN());
	}
	if(0.0f < controlTrack->TimeDelay)	{	/* 再生待機中 */
		return(-1.0f);
	}

	float timeCursor = controlTrack->TimeElapsed;
	if(true == controlTrack->StatusGetIsPlayStylePingpong())	{	/* 往復再生 */
		if(controlTrack->StatusGetIsPlayingReverse() != controlTrack->StatusGetIsPlayStyleReverse())	{	/* 復路 */
			timeCursor += controlTrack->TimeRange;
		}
	}

	return(timeCursor);
}

/* ********************************************************* */
//! 再生カーソル位置の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
float Entity::CursorSet(int indexTrack, float cursor)
{
	if((true == std::isnan(cursor)) || (0.0f > cursor))	{	/* カーソル位置が不正 */
		return(false);
	}

	if(nullptr == TableControlTrack)	{
		return(false);
	}

	if(0 > indexTrack)	{	/* 全トラック対象 */
		bool flagSucceed = false;
		for(int i=0; i<CountTrack; i++)	{
			flagSucceed |= CursorSetMain(i, cursor);
		}

		return(flagSucceed);
	}

	if(CountTrack <= indexTrack)	{
		return(false);
	}
	CursorSetMain(indexTrack, cursor);

	return(true);
}
bool Entity::CursorSetMain(int indexTrack, float cursor)
{
	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(false == controlTrack->StatusGetIsPlaying())	{
		return(false);
	}

	float rangeCursor = controlTrack->TimeRange;
	if(true == controlTrack->StatusGetIsPlayStylePingpong())	{	/* 往復再生 */
		bool flagReverse = controlTrack->StatusGetIsPlayStyleReverse();
		if(rangeCursor <= cursor)	{	/* 復路 */
			cursor -= rangeCursor;
			flagReverse = !flagReverse;
		}
		if(true == flagReverse)	{	/* 逆再生 */
			controlTrack->Status |= Library::Control::Animation::Track::FlagBitStatus::PLAYING_REVERSE;
		} else {	/* 順再生 */
			controlTrack->Status &= ~Library::Control::Animation::Track::FlagBitStatus::PLAYING_REVERSE;
		}
	}

	if(rangeCursor < cursor)	{	/* 範囲オーバ */
		return(false);
	}

	/* MEMO: （通常のフレームスキップと違い）フレーム位置がジャンプするため時間経過範囲が不正になることから、 */
	/*       ユーザデータやシグナルの検知は行わない。                                                         */
	controlTrack->Status |= Library::Control::Animation::Track::FlagBitStatus::IGNORE_NEXTUPDATE_USERDATA
							| Library::Control::Animation::Track::FlagBitStatus::IGNORE_NEXTUPDATE_SIGNAL;
	controlTrack->TimeElapsed = cursor;

	return(true);
}

/* ********************************************************* */
//! 再生カーソル位置のフレームを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Entity::CursorGetFrame(int indexTrack)
{
	if(nullptr == TableControlTrack)	{
		return(-1);
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(-1);
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(false == controlTrack->StatusGetIsPlaying())	{
		return(-1);
	}

	return(controlTrack->ArgumentContainer.Frame);
}

/* ********************************************************* */
//! 再生カーソル位置をフレームにスナップ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
float Entity::CursorSnapFrame(int indexTrack, float cursor)
{
	if((true == std::isnan(cursor)) || (0.0f > cursor))	{	/* カーソル位置が不正 */
		return(std::numeric_limits<float>::quiet_NaN());
	}

	if(nullptr == TableControlTrack)	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	if((0 > indexTrack) || (CountTrack <= indexTrack))	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	Library::Control::Animation::Track* controlTrack = &TableControlTrack[indexTrack];
	if(false == controlTrack->StatusGetIsPlaying())	{
		return(std::numeric_limits<float>::quiet_NaN());
	}

	float frameFloat = cursor * (float)controlTrack->FramePerSecond;
	int frame = (int)((true == controlTrack->StatusGetIsPlayingReverse()) ? std::ceil(frameFloat) : std::floor(frameFloat));

	return((float)frame * controlTrack->TimePerFrame);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
