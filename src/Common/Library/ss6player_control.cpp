/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_control.h"
#include "../Library/ss6player_utility.h"
#include "../Entity/ss6player_entity.h"
#include "../Entity/ss6player_entity_effect.h"
#include "../Project/ss6player_project.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Control	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
/* パーツ関係の定数 */
/* MEMO: 今は外部から使わないだろうと思うので、ファイルスコープに沈めています。  */
/*       外部で使用する場合には、Partsクラスの静的メンバ変数にするのが正しいかと */
/*       思います。                                                              */
static const CPU::Vector2 SizeTextureDefault(64.0f, 64.0f);	/* テクスチャが存在しない場合のダミー値 */

static const Library::KindVertex::Kind TableIndexVertexCorrectionOrder[4][Library::KindVertex::TERMINATOR2] = {	/* 反転状態を加味した頂点処理順序 */
	{	/* Normal */
		Library::KindVertex::LU,
		Library::KindVertex::RU,
		Library::KindVertex::RD,
		Library::KindVertex::LD,
	}, {	/* Flip-X */
		Library::KindVertex::RU,
		Library::KindVertex::LU,
		Library::KindVertex::LD,
		Library::KindVertex::RD,
	}, {	/* Flip-Y */
		Library::KindVertex::LD,
		Library::KindVertex::RD,
		Library::KindVertex::RU,
		Library::KindVertex::LU,
	}, {	/* FlipX&Y */
		Library::KindVertex::RD,
		Library::KindVertex::LD,
		Library::KindVertex::LU,
		Library::KindVertex::RU,
	}
};

static Float32 TableRateAlphaPartsColorDefault[Library::KindVertex::TERMINATOR2] =	{
	1.0f,
	1.0f,
	1.0f,
	1.0f,
};

/* 通常パーツ・パーティクルのモデル元データ */
static const int TableCountVertexIndex_Triangle2 = 6;	/* 2面 * 3頂点 */
static const int TableCountVertexIndex_Triangle4 = 12;	/* 4面 * 3頂点 */

/* MEMO: 頂点データ群は4分割三角形のもので持っています（2分割三角の時はTERMINATOR2までを使用） */
/*       ※座標とUVがVector4で・同次がついているのは、FPUで行列計算を行うためです。            */
static const FPU::Vector4 TableUVMappingModel[Library::KindVertex::TERMINATOR4] = {	/* UV */
	/* LU */	FPU::Vector4(-0.5f, 0.5f, 0.0f, 1.0f),
	/* RU */	FPU::Vector4(0.5f, 0.5f, 0.0f, 1.0f),
	/* RD */	FPU::Vector4(0.5f, -0.5f, 0.0f, 1.0f),
	/* LD */	FPU::Vector4(-0.5f, -0.5f, 0.0f, 1.0f),
	/* C */		FPU::Vector4(0.0f, 0.0f, 0.0f, 1.0f)
};
static const FPU::Vector4 TableCoordinateModel[Library::KindVertex::TERMINATOR4] = {	/* 座標 */
	/* LU */	FPU::Vector4(-0.5f, 0.5f, 0.0f, 1.0f),
	/* RU */	FPU::Vector4(0.5f, 0.5f, 0.0f, 1.0f),
	/* RD */	FPU::Vector4(0.5f, -0.5f, 0.0f, 1.0f),
	/* LD */	FPU::Vector4(-0.5f, -0.5f, 0.0f, 1.0f),
	/* C */		FPU::Vector4(0.0f, 0.0f, 0.0f, 1.0f)
};
static const FPU::Color TableColorModel[Library::KindVertex::TERMINATOR4] = {
	/* LU */	FPU::Color(1.0f, 1.0f, 1.0f, 1.0f),
	/* RU */	FPU::Color(1.0f, 1.0f, 1.0f, 1.0f),
	/* RD */	FPU::Color(1.0f, 1.0f, 1.0f, 1.0f),
	/* LD */	FPU::Color(1.0f, 1.0f, 1.0f, 1.0f),
	/* C */		FPU::Color(1.0f, 1.0f, 1.0f, 1.0f)
};
static const CPU::Vector4 TableVArgModel[Library::KindVertex::TERMINATOR4] = {	/* 頂点シェーダ用パラメータ */
	/* LU */	FPU::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
	/* RU */	FPU::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
	/* RD */	FPU::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
	/* LD */	FPU::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
	/* C */		FPU::Vector4(1.0f, 0.0f, 0.0f, 0.0f)
};

/* MEMO: ちょっとインチキしてここに記述しています。 */
namespace Animation	{
/* MEMO: プライオリティキーにパーツIDが混ぜ込まれている理由ですが、 */
/*       SS6の描画規則として、描画順同着の場合にパーツIDが後の（SS6 */
/*       のタイムライン上で下に位置している）のパーツが後に描画され */
/*       る規則をソートに一緒に混ぜ込むためです。                   */
#if 0	/* MEMO: Before Ver.1.0.x */
const int CountShiftSortKeyPriority = 16;
const int MaskSortKeyPriority = 0x7fff0000;	/* -16384 to 16383 */
const int MaskSortKeyIDParts = 0x0000ffff;	/* -32788 to 32767 */
#else	/* MEMO: After Ver.1.1.x */
/* MEMO: SS6PUのVer.1.1.x以降でZ座標ソートにも対応したため、プライオリティ部分の精度を上げる必要が出たため */
/*       パーツID部分を4ビット削った関係から、扱えるパーツ数が少なくなっています。                         */
const int CountShiftSortKeyPriority = 12;
const int MaskSortKeyPriority = 0x7ffff000;	/* -524288 to 524287 */
const int MaskSortKeyIDParts = 0x00000fff;	/* 0 to 4095 */
#endif
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 対角線交点座標
/*!
@param	intersection
	交点座標格納先
@param	LU
	左上座標
@param	RU
	右上座標
@param	LD
	左下座標
@param	RD
	右下座標

@retval	関数値
	なし
@retval	intersection
	交点座標

線分[LU, RD]と線分[RU, LD]の交点座標を返します。
入力と出力の型がいびつなので注意してください。
*/
inline void CoordinateGetDiagonalIntersection(	FPU::Vector4* intersection,
												const CPU::Vector2& LU,
												const CPU::Vector2& RU,
												const CPU::Vector2& LD,
												const CPU::Vector2& RD
											)
{
	/* MEMO: (.z, .w)は(0.0, 1.0f)に固定されます。 */
	/* MEMO: エラー時（交点が見つからない場合）のことを考えて、原点を入れています。 */
	intersection->Set(FPU::Vector4::Origin);

	float c1 = (LD.GetY() - RU.GetY()) * (LD.GetX() - LU.GetX()) - (LD.GetX() - RU.GetX()) * (LD.GetY() - LU.GetY());
	float c2 = (RD.GetX() - LU.GetX()) * (LD.GetY() - LU.GetY()) - (RD.GetY() - LU.GetY()) * (LD.GetX() - LU.GetX());
	float c3 = (RD.GetX() - LU.GetX()) * (LD.GetY() - RU.GetY()) - (RD.GetY() - LU.GetY()) * (LD.GetX() - RU.GetX());
	float ca = c1 / c3;
	float cb = c2 / c3;

	if(((0.0f <= ca) && (1.0f >= ca)) && ((0.0f <= cb) && (1.0f >= cb)))	{
		intersection->SetX(LU.GetX() + ca * (RD.GetX() - LU.GetX()));
		intersection->SetY(LU.GetY() + ca * (RD.GetY() - LU.GetY()));
	}
}

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
/* 描画コマンド割込生成関数群 */
static TypeDrawCommandList CallBackFunctionCommandInsertFragment(TypeDrawCommandList commandList, void* parameterPointer, int parameterInteger);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Animation::Track */
/* -------------------------------------------------------------------------- */
namespace Animation	{
/* ********************************************************* */
//! 再生ステータス系関数群
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Track::StatusGetIsValid(void) const
// bool Track::StatusGetIsPlaying(void) const
// bool Track::StatusGetIsPausing(void) const
// bool Track::StatusGetIsPlayStylePingpong(void) const
// bool Track::StatusGetIsPlayStyleReverse(void) const
// bool Track::StatusGetIsPlayingStart(void) const
// bool Track::StatusGetIsPlayingReverse(void) const
// bool Track::StatusGetIsPlayingReversePrevious(void) const
// bool Track::StatusGetIsPlayingTurn(void) const
// bool Track::StatusGetIsDecodeAttribute(void) const
// bool Track::StatusGetIsIgnoreUserData(void) const
// bool Track::StatusGetIsIgnoreSkipLoop(void) const
// bool Track::StatusGetIsIgnoreNextUpdateUserData(void) const
// bool Track::StatusGetIsTransitionStart(void) const
// void Track::StatusSetIsTransitionStart(bool value)
// bool Track::StatusGetIsTransitionCancelPause(void) const
// void Track::StatusSetIsTransitionCancelPause(bool value)
// bool Track::StatusGetIsRequestPlayEnd(void) const
// void Track::StatusSetIsRequestPlayEnd(bool value)
// bool Track::StatusGetIsRequestTransitionEnd(void) const
// void Track::StatusSetIsRequestTransitionEnd(bool value)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Track::CleanUp(void)
{
	Status = FlagBitStatus::CLEAR;

	ArgumentContainer.CleanUp();

	TimeDelay = 0.0f;
	TimeElapsed = 0.0f;
	RateTime = 1.0f;
	TimeElapsedNow = 0.0f;

	TimesPlay = -1;
	CountLoop = -1;
	CountLoopNow = -1;

	FramePerSecond = 60;
	TimePerFrame = 0.0f;
	TimePerFrameConsideredRateTime = 0.0f;

	FrameStart = -1;
	FrameEnd = -1;
	FrameRange = 0;
	TimeRange = 0.0f;

	TimeElapsedTransition = 0.0f;
	TimeLimitTransition = 0.0f;
	RateTransition = 0.0f;
	IndexTrackSlave = -1;

	TimeElapseReplacement = 0.0f;
	TimeElapseInRangeReplacement = 0.0f;
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Track::BootUp(void)
{
	CleanUp();

	Status |= FlagBitStatus::VALID;

	return(true);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Track::ShutDown(void)
{
	CleanUp();
}

/* ********************************************************* */
//! トラック再生開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Track::Start(	SpriteStudio6::Entity& instanceRoot,
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
				)
{
	/* Check booted-up */
	if(0 == (Status & FlagBitStatus::VALID))	{
		if(false == BootUp())	{
			return(false);
		}
	}

	/* Reset Animation */
	ArgumentContainer.DataAnimation = &dataAnimation;
	ArgumentContainer.IndexAnimation = indexAnimation;
	ArgumentContainer.FramePrevious = -1;

	/* Set datas */
	Status &= FlagBitStatus::VALID;	/* Clear */

	Status = (true == flagPingpong) ? (Status | FlagBitStatus::STYLE_PINGPONG) : (Status & ~FlagBitStatus::STYLE_PINGPONG);
	Status &= ~FlagBitStatus::STYLE_REVERSE;
	RateTime = rateTime;
	if(0.0f > RateTime)	{
		Status |= (FlagBitStatus::STYLE_REVERSE | FlagBitStatus::PLAYING_REVERSE);
		RateTime *= -1.0f;
	}
	FramePerSecond = framePerSecond;
	TimePerFrame = 1.0f / (float)FramePerSecond;

	FrameStart = frameRangeStart;
	FrameEnd = frameRangeEnd;
	if(0 != (Status & FlagBitStatus::PLAYING_REVERSE))	{	/* Play-Reverse */
		if(FrameStart >= frame)
		{
			frame = FrameEnd + 1;
		}
	} else {	/* Play-Foward */
		if((FrameEnd + 1) <= frame)
		{
			frame = FrameStart - 1;
		}
	}
	ArgumentContainer.Frame = frame;

	TimesPlay = timesPlay;
	TimeDelay = timeDelay;

	CountLoop = 0;
	TimeElapsed = (ArgumentContainer.Frame - FrameStart) * TimePerFrame;
	TimeElapsedNow = 0.0f;
//	ArgumentContainer.Frame = Mathf.Clamp(ArgumentContainer.Frame, FrameStart, FrameEnd);

	Status |= FlagBitStatus::PLAYING;
	Status |= FlagBitStatus::PLAYING_START;
	Status &= ~(	FlagBitStatus::IGNORE_NEXTUPDATE_USERDATA
					| FlagBitStatus::IGNORE_NEXTUPDATE_USERDATA
			);

	FrameRange = (FrameEnd - FrameStart) + 1;
	TimeRange = (Float32)FrameRange * TimePerFrame;
	TimePerFrameConsideredRateTime = TimePerFrame * RateTime;

	TimeElapseReplacement = 0.0f;
	TimeElapseInRangeReplacement = 0.0f;

	return(true);
}

/* ********************************************************* */
//! トラックトランジションを開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Track::Transition(int indexTrackSlave, float time)
{
	if(0 == (Status & FlagBitStatus::VALID))	{
		return(false);
	}

	IndexTrackSlave = indexTrackSlave;

	TimeElapsedTransition = 0.0f;
	TimeLimitTransition = time;
	RateTransition = 0.0f;

	return(true);
}

/* ********************************************************* */
//! トラック再生終了
bool Track::Stop(void)
{
	if(0 == (Status & FlagBitStatus::VALID))	{
		return(false);
	}

	if(0 == (Status & FlagBitStatus::PLAYING))	{
		return(true);
	}

	Status &= ~(FlagBitStatus::PLAYING | FlagBitStatus::PAUSING);

	return(true);
}

/* ********************************************************* */
//! トラック再生一時停止状態の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Track::Pause(bool flagSwitch)
{
	if(0 == (Status & FlagBitStatus::VALID))	{
		return(false);
	}

	if(true == flagSwitch)	{
		Status |= FlagBitStatus::PAUSING;
	} else {
		Status &= ~FlagBitStatus::PAUSING;
	}

	return(true);
}

/* ********************************************************* */
//! トラック再生状態の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Track::Update(float timeElapsed)
{
	Float32 timeElapsedAnimation = timeElapsed * RateTime;
	bool flagStopJumpTime = (0.0f != TimeElapseReplacement);
	bool FlagRangeOverPrevious = false;
	if(0 == (Status & FlagBitStatus::PLAYING))	{	/* Stoping */
		if(true == flagStopJumpTime)	{
			/* MEMO: In frame-Jump, Elapsed time is absolute. */
			timeElapsedAnimation = TimeElapseReplacement;
		} else {
			timeElapsedAnimation = 0.0f;
		}
	}
	TimeElapsedNow = timeElapsedAnimation;

	if(0 == (Status & FlagBitStatus::VALID))	{
		return(false);
	}

	/* Reset status */
	CountLoopNow = 0;
	Status &= ~FlagBitStatus::PLAYING_TURN;
	if(0 != (Status & FlagBitStatus::PLAYING_REVERSE))	{
		Status |= FlagBitStatus::PLAYING_REVERSEPREVIOUS;
	} else {
		Status &= ~FlagBitStatus::PLAYING_REVERSEPREVIOUS;
	}

	/* Check Playng status */
	if(0 == (Status & FlagBitStatus::PLAYING))	{	/* Not-Playing */
		/* MEMO: Even if the animation has ended, there are cases when you are transitioning. */
		if(false == flagStopJumpTime)	{
			goto Update_UpdateTransition;
		}
	}

	if(0 == (Status & FlagBitStatus::PLAYING_START))	{	/* (Not Right-After-Starting) */
		if(0 != (Status & FlagBitStatus::PAUSING))	{	/* Play & Pausing */
#if 0
			/* MEMO: Transition does not progress during paused. */
			return(true);
#else
			goto Update_UpdateTransition;
#endif
		}
	}

	if(0.0f > TimeDelay)	{	/* Wait Infinite */
		TimeDelay = -1.0f;
		Status &= ~(FlagBitStatus::PLAYING_START | FlagBitStatus::DECODE_ATTRIBUTE);	/* Cancel Start & Decoding Attribute */
		return(true);
	} else {	/* Wait Limited-Time */
		if(0.0f < TimeDelay)	{	/* Waiting */
			TimeDelay -= timeElapsedAnimation;
			if(0.0f < TimeDelay)	{
				Status &= ~(FlagBitStatus::PLAYING_START | FlagBitStatus::DECODE_ATTRIBUTE);	/* Cancel Start & Decoding Attribute */
				return(true);
			}

			/* Start */
			timeElapsedAnimation += -TimeDelay * ((0 == (Status & FlagBitStatus::PLAYING_REVERSE)) ? 1.0f : -1.0f);
			TimeDelay = 0.0f;
			ArgumentContainer.FramePrevious = -1;
			Status |= (FlagBitStatus::PLAYING_START | FlagBitStatus::DECODE_ATTRIBUTE);
		}
	}
	if(0 != (Status & FlagBitStatus::PLAYING_START))	{	/* Play & Right-After-Starting */
		Status |= (FlagBitStatus::PLAYING_START | FlagBitStatus::DECODE_ATTRIBUTE);
		timeElapsedAnimation = 0.0f;
		goto Update_End;	/* Display the first frame, force */
	}

	/* Calculate New-Frame */
	ArgumentContainer.FramePrevious = ArgumentContainer.Frame;
#if 0	/* MEMO: 元（C#）はここにあったが、Clangではローカル変数の初期化タイミングの問題でエラーになるので、関数先頭に移動。 */
	bool FlagRangeOverPrevious = false;
#endif
	if(0 != (Status & FlagBitStatus::STYLE_PINGPONG))	{	/* Play-Style: PingPong */
		if(0 != (Status & FlagBitStatus::PLAYING_REVERSE))	{
			FlagRangeOverPrevious = (0.0f > TimeElapsed) ? true : false;
			TimeElapsed -= timeElapsedAnimation;
			if((0.0f > TimeElapsed) && (true == FlagRangeOverPrevious))	{	/* Still Range-Over */
				goto Update_End;
			}
		} else {
			FlagRangeOverPrevious = (TimeRange <= TimeElapsed) ? true : false;
			TimeElapsed += timeElapsedAnimation;
			if((TimeRange <= TimeElapsed) && (true == FlagRangeOverPrevious))
			{	/* Still Range-Over */
				goto Update_End;
			}
		}

		if(0 != (Status & FlagBitStatus::STYLE_REVERSE))	{	/* Play-Style: PingPong & Reverse */
			while((TimeRange <= TimeElapsed) || (0.0f > TimeElapsed))	{
				if(0 != (Status & FlagBitStatus::PLAYING_REVERSE))	{	/* Now: Reverse */
					if(TimeRange <= TimeElapsed)	{	/* MEMO: Follow "FlagRangeOverPrevious" */
						break;
					}
					if(0.0f > TimeElapsed)	{	/* Frame-Over: Turn */
						TimeElapsed += TimeRange;
						TimeElapsed = TimeRange - TimeElapsed;
						Status |= FlagBitStatus::PLAYING_TURN;
						Status &= ~FlagBitStatus::PLAYING_REVERSE;
					}
				} else {   /* Now: Foward */
					if(true == FlagRangeOverPrevious)	{
						FlagRangeOverPrevious = false;
						Status |= FlagBitStatus::PLAYING_TURN;
						break;
					} else {
						CountLoop++;
						if(TimeRange <= TimeElapsed)	{	/* Frame-Over: Loop/End */
							if(0 < TimesPlay)	{	/* Limited-Count Loop */
								TimesPlay--;
								if(0 >= TimesPlay)	{	/* End */
									goto Update_PlayEnd_Foward;
								}
							}

							/* Not-End */
							TimeElapsed -= TimeRange;
							TimeElapsed = TimeRange - TimeElapsed;
							Status |= FlagBitStatus::PLAYING_REVERSE;
							Status |= FlagBitStatus::PLAYING_TURN;
							CountLoopNow++;
						}
					}
				}
			}
		} else {	/* Play-Style: PingPong & Foward */
			while((TimeRange <= TimeElapsed) || (0.0f > TimeElapsed))	{
				if(0 != (Status & FlagBitStatus::PLAYING_REVERSE))	{	/* Now: Reverse */
					if(true == FlagRangeOverPrevious)	{
						FlagRangeOverPrevious = false;
						Status |= FlagBitStatus::PLAYING_TURN;
						break;
					} else {
						CountLoop++;
						if(0.0f > TimeElapsed)	{	/* Frame-Over: Loop/End */
							if(0 < TimesPlay)	{	/* Limited-Count Loop */
								TimesPlay--;
								if(0 >= TimesPlay)	{	/* End */
									goto AnimationUpdate_PlayEnd_Reverse;
								}
							}

							/* Not-End */
							TimeElapsed += TimeRange;
							TimeElapsed = TimeRange - TimeElapsed;
							Status &= ~FlagBitStatus::PLAYING_REVERSE;
							Status |= FlagBitStatus::PLAYING_TURN;
							CountLoopNow++;
						}
					}
				} else {	/* Now: Foward */
					if(0.0f > TimeElapsed)	{	/* MEMO: Follow "FlagRangeOverPrevious" */
						break;
					}
					if(TimeRange <= TimeElapsed)	{	/* Frame-Over: Turn */
						TimeElapsed -= TimeRange;
						TimeElapsed = TimeRange - TimeElapsed;
						Status |= FlagBitStatus::PLAYING_TURN;
						Status |= FlagBitStatus::PLAYING_REVERSE;
					}
				}
			}
		}
	} else {	/* Play-Style: OneWay */
		if(0 != (Status & FlagBitStatus::STYLE_REVERSE))	{	/* Play-Style: OneWay & Reverse */
			FlagRangeOverPrevious = (0.0f > TimeElapsed) ? true : false;
			TimeElapsed -= timeElapsedAnimation;
			if((0.0f > TimeElapsed) && (true == FlagRangeOverPrevious))	{	/* Still Range-Over */
				goto Update_End;
			}

			while(0.0f > TimeElapsed)	{
				TimeElapsed += TimeRange;
				if(true == FlagRangeOverPrevious)	{
					FlagRangeOverPrevious = false;
					Status |= FlagBitStatus::PLAYING_TURN;
				} else {
					CountLoop++;
					if(0 < TimesPlay)	{	/* Limited-Count Loop */
						TimesPlay--;
						if(0 >= TimesPlay)	{	/* End */
							goto AnimationUpdate_PlayEnd_Reverse;
						}
					}

					/* Not-End */
					CountLoopNow++;
					Status |= FlagBitStatus::PLAYING_TURN;
				}
			}
		} else {	/* Play-Style: OneWay & Foward */
			FlagRangeOverPrevious = (TimeRange <= TimeElapsed) ? true : false;
			TimeElapsed += timeElapsedAnimation;
			if((TimeRange <= TimeElapsed) && (true == FlagRangeOverPrevious))	{	/* Still Range-Over */
				goto Update_End;
			}

			while(TimeRange <= TimeElapsed)	{
				TimeElapsed -= TimeRange;
				if(true == FlagRangeOverPrevious)	{
					FlagRangeOverPrevious = false;
					Status |= FlagBitStatus::PLAYING_TURN;
				} else {
					CountLoop++;
					if(0 < TimesPlay)	{	/* Limited-Count Loop */
						TimesPlay--;
						if(0 >= TimesPlay)	{	/* End */
							goto Update_PlayEnd_Foward;
						}
					}

					/* Not-End */
					Status |= FlagBitStatus::PLAYING_TURN;
					CountLoopNow++;
				}
			}
		}
	}

Update_End:;
	/* MEMO: 終了処理がらみが（トラックトランジションが絡んでいる関係で）gotoの */
	/*       使い方が本来忌避すべき実装である点について、申し訳ありません。     */
	/*       ※ただ、この処理形態にしておかないと、同じ決着処理がロジック中に重 */
	/*         複して現れる形になりメンテナンスなどを経た時の確実性がかなり落ち */
	/*         るため、現時点での苦肉の策です（関数呼出コストを下げて高速化しよ */
	/*         うとしているSS6Player for UnityでのC#実装の名残です）。          */
	{
		int frame = (int)(TimeElapsed / TimePerFrame);
//		frame = Mathf.Clamp(frame, 0, (FrameRange - 1));
		frame += FrameStart;
		if(	((frame != ArgumentContainer.FramePrevious) || (0 != (Status & FlagBitStatus::PLAYING_TURN)))
			|| (true == flagStopJumpTime)
			)	{
			Status |= FlagBitStatus::DECODE_ATTRIBUTE;
		}
		ArgumentContainer.Frame = frame;
	}
	/* Fall-Through */
Update_UpdateTransition:;
	Status &= ~FlagBitStatus::REQUEST_TRANSITIONEND;
	if(0 <= IndexTrackSlave)	{
		TimeElapsedTransition += timeElapsed;	/* Transition's elapsed time exclude RateTime */
		if(TimeLimitTransition <= TimeElapsedTransition)	{	/* End */
			RateTransition = 1.0f;	/* Clip */
			Status |= FlagBitStatus::REQUEST_TRANSITIONEND;
		} else {
			RateTransition = TimeElapsedTransition / TimeLimitTransition;
		}
	}
	return(true);

Update_PlayEnd_Foward:;
	TimesPlay = 0;	/* Clip */
	Status |= (FlagBitStatus::REQUEST_PLAYEND | FlagBitStatus::DECODE_ATTRIBUTE);
	TimeElapsed = TimeRange;
	ArgumentContainer.Frame = FrameEnd;
	goto Update_UpdateTransition;

AnimationUpdate_PlayEnd_Reverse:;
	TimesPlay = 0;	/* Clip */
	Status |= (FlagBitStatus::REQUEST_PLAYEND | FlagBitStatus::DECODE_ATTRIBUTE);
	TimeElapsed = 0.0f;
	ArgumentContainer.Frame = FrameStart;
	goto Update_UpdateTransition;
}

/* ********************************************************* */
//! トラック再生ステータスのクリア
/* --------------------------------------------------------- */
/* MEMO:
 * トラック再生ステータスのフレーム毎のステータスフラグを落と
 *	します（次ループでの再生のため）。
 */
// void Track::StatusClearTransient(bool flagDecodeNextForce)

/* ********************************************************* */
//! トラック再生時間のスキップ（カーソルジャンプ）
/* --------------------------------------------------------- */
/* MEMO:
 * 原則的に、インスタンスのアニメーション描画から呼ぶに留めて
 * ください。
 */
void Track::TimeSkip(float time, bool flagReverseParent, bool flagWrapRange)
{
	if(0.0f > time)	{	/* Wait Infinity */
		TimeDelay = -1.0f;
		return;
	}

	bool flagPongPong = (0 != (Status & FlagBitStatus::STYLE_PINGPONG));	/* ? true : false */
	bool flagReverseStyle = (0 != (Status & FlagBitStatus::STYLE_REVERSE));	/* ? true : false */
	Float32 timeLoop = TimeRange * ((true == flagPongPong) ? 2.0f : 1.0f);
	Float32 timeCursor = time;
	int countLoop = 0;

	/* Loop-Count Get */
	while(timeLoop <= timeCursor)	{
		timeCursor -= timeLoop;
		countLoop++;
	}

	/* Solving Play-Count */
	if(0 >= TimesPlay)	{	/* Infinite-Loop */
		/* MEMO: "TimesPlay" does not change. */
		countLoop = 0;
		TimeDelay = 0.0f;
	} else {	/* Limited-Loop */
		if(0 >= countLoop)	{	/* No-Wrap-Around */
			/* MEMO: "TimesPlay" does not change. */
			countLoop = 0;
			TimeDelay = 0.0f;
		} else {	/* Wrap-Around */
			if(TimesPlay <= countLoop)	{	/* Over */
				if(true == flagReverseParent)	{	/* Reverse ... Play-Delay */
					/* MEMO: "TimesPlay" does not change. */
					TimeDelay = ((float)(countLoop - TimesPlay) * timeLoop) + timeCursor;
					timeCursor = timeLoop;
				} else {	/* Foward ... Play-End */
					TimeDelay = 0.0f;
					TimesPlay = 0;

					if(true == flagPongPong)	{	/* Play-Style: PingPong */
						timeCursor = (0 != (Status & FlagBitStatus::STYLE_REVERSE)) ? TimeRange : 0.0f;
					} else {	/* Play-Style: OneWay */
						timeCursor = (0 != (Status & FlagBitStatus::STYLE_REVERSE)) ? 0.0f : TimeRange;
					}

					Stop();
				}
			} else {   /* In-Range */
				TimesPlay -= countLoop;
			}
		}
	}

	/* Time Adjust */
	if(true == flagPongPong)	{	/* Play-Style: PingPong */
		Status &= ~FlagBitStatus::PLAYING_REVERSE;
		if(true == flagReverseStyle)	{	/* Play-Stype: PingPong & Reverse */
			if(TimeRange <= timeCursor)	{	/* Start: Foward */
				timeCursor -= TimeRange;
//				Status &= ~FlagBitStatus.PLAYING_REVERSE;
			} else {	/* Start: Reverse */
				Status |= FlagBitStatus::PLAYING_REVERSE;
			}
		} else {	/* Play-Style: PingPong & Foward */
			if(TimeRange <= timeCursor)	{	/* Start: Reverse */
				timeCursor -= TimeRange;
				timeCursor = TimeRange - timeCursor;
				Status |= FlagBitStatus::PLAYING_REVERSE;
			} else {	/* Start: Foward */
//				Status &= ~FlagBitStatus.PLAYING_REVERSE;
			}
		}
	} else {	/* Play-Style: One-Way */
		if(true == flagReverseStyle)	{	/* Play-Stype: One-Way & Reverse */
			Status |= FlagBitStatus::PLAYING_REVERSE;
			if(false == flagReverseParent)	{
				timeCursor = TimeRange - timeCursor;
			}
		} else {	/* Play-Stype: One-Way & Foward */
			Status &= ~FlagBitStatus::PLAYING_REVERSE;
			if(true == flagReverseParent)	{
				timeCursor = TimeRange - timeCursor;
			}
		}
	}

	TimeElapsed = timeCursor;
	int frame = (int)(TimeElapsed / TimePerFrame);
//	frame = Mathf.Clamp(frame, 0, (FrameRange - 1));
	frame += FrameStart;
	ArgumentContainer.Frame = frame;
}

/* ********************************************************* */
//! トラック再生の残り時間の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Track::TimeGetRemain(float* timeRemainTotal, float* timeRemainInRange)
{
	*timeRemainTotal = 0.0f;
	*timeRemainInRange = 0.0f;

	if((FlagBitStatus::VALID | FlagBitStatus::PLAYING) != (Status & (FlagBitStatus::VALID | FlagBitStatus::PLAYING)))	{
		return(false);
	}

	int timesPlayRemain = 0;
	if(0 < TimesPlay)	{	/* Limited-Loop */
		timesPlayRemain = TimesPlay - 1;
	}

	if(0 != (Status & FlagBitStatus::STYLE_PINGPONG))	{	/* Style-PingPong */
		*timeRemainTotal = TimeRange * (float)(timesPlayRemain * 2);

		if(0 != (Status & FlagBitStatus::STYLE_REVERSE))	{	/* Style-Reverse */
			if(0 != (Status & FlagBitStatus::PLAYING_REVERSE))	{	/* timeRemain-Reverse (Out-bound) */
				*timeRemainInRange = TimeRange + TimeElapsed;
			} else {	/* Play-Normal (In-bound) */
				*timeRemainInRange = (TimeRange - TimeElapsed);
			}
		} else {	/* Style-Normal */
			if(0 == (Status & FlagBitStatus::PLAYING_REVERSE))	{	/* Play-Normal (Out-bound) */
				*timeRemainInRange = TimeRange + (TimeRange - TimeElapsed);
			} else {	/* Play-Reverse (In-bound) */
				*timeRemainInRange = TimeElapsed;
			}
		}

		*timeRemainTotal += *timeRemainInRange;
	} else {
		*timeRemainTotal += TimeRange * (float)timesPlayRemain;

		if(0 != (Status & FlagBitStatus::STYLE_REVERSE))	{	/* Style-Reverse */
			*timeRemainInRange = TimeElapsed;
		} else {	/* Style-Normal */
			*timeRemainInRange = (TimeRange - TimeElapsed);
		}

		*timeRemainTotal += *timeRemainInRange;
	}

	return(true);
}
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                               Animation::ArgumentContainer */
/* -------------------------------------------------------------------------- */
namespace Animation	{
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void ArgumentContainer::CleanUp(void)

/* ********************************************************* */
//! 複製
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void ArgumentContainer::Duplicate(ArgumentContainer& original)
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                 Animation::BufferAttribute */
/* -------------------------------------------------------------------------- */
namespace Animation	{
/* MEMO: テンプレートクラスにつき実装記述はなし。 */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Animation::Parts */
/* -------------------------------------------------------------------------- */
namespace Animation	{
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Parts::Parts(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Parts::~Parts(void)
{
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Parts::CleanUp(void)
{
	IDParts = -1;
	Status = FlagBitStatus::CLEAR;
	StatusAnimationParts = FlagBitStatus::CLEAR;
	IndexControlTrack = 0;
	IndexControlDraw = -1;

	IndexEntityUnderControl = -1;
	IndexAnimationUnderControl = -1;

	FrameKeyStatusAnimationFrame = -1;
	StatusAnimationFrame.CleanUp();

	TRSMaster.CleanUp();
	TRSSlave.CleanUp();
	ScaleLocal.CleanUp();	ScaleLocal.Value.Set(CPU::Vector2::One);
	RateOpacity.CleanUp();	RateOpacity.Value = 1.0f;
	Priority.CleanUp();	Priority.Value = 0;

	FramePreviousUpdateUnderControl = -1;
	DataInstance.CleanUp();

	TransformPosition.Set(FPU::Vector3::Zero);
	TransformRotation.Set(FPU::Vector3::Zero);
	TransformScaling.Set(FPU::Vector3::One);
	MatrixTransform.Set(FPU::Matrix4x4::Unit);
	MatrixTransformLocal.Set(FPU::Matrix4x4::Unit);
}

/* ********************************************************* */
//! パーツで使用するワークエリアのサイズを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 本関数は静的関数なので注意してください。
 */
size_t Parts::SizeGetWorkArea(Entity& entity, int idParts)
{
	size_t sizeWorkArea = 0;
	if((0 > idParts) || (entity.CountGetParts() <= idParts))	{
		/* MEMO: エラーを返すべきか？ */
		return(sizeWorkArea);
	}

	switch(entity.DataAnimation->FeatureGetParts(idParts))	{
		case Library::Data::Parts::Animation::KindFeature::ROOT:
		case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
			/* MEMO: Drawクラスを併用しない（描画パーツでない）ので、追加ワークエリアは存在しない。 */
			break;

		case Library::Data::Parts::Animation::KindFeature::NORMAL:
			/* MEMO: 通常パーツは全て4分割メッシュ（5頂点）。 */
			sizeWorkArea += Draw::SizeGetWorkArea(entity, idParts, (int)Library::KindVertex::TERMINATOR4, false);
			break;

		case Library::Data::Parts::Animation::KindFeature::INSTANCE:
			/* MEMO: インスタンスは描画情報のみ持つ（実際の描画ワークについては子のEntityが持っている）。 */
			sizeWorkArea += Draw::SizeGetWorkArea(entity, idParts, 0, false);
			break;

		case Library::Data::Parts::Animation::KindFeature::EFFECT:
			/* MEMO: エフェクトは描画情報のみ持つ（実際の描画ワークについては子のEntityが持っている）。 */
			sizeWorkArea += Draw::SizeGetWorkArea(entity, idParts, 0, false);
			break;

		case Library::Data::Parts::Animation::KindFeature::MASK:
			/* MEMO: マスクパーツは（通常パーツと同じで）全て4分割メッシュ（5頂点）。 */
			sizeWorkArea += Draw::SizeGetWorkArea(entity, idParts, (int)Library::KindVertex::TERMINATOR4, false);
			break;

		case Library::Data::Parts::Animation::KindFeature::JOINT:
		case Library::Data::Parts::Animation::KindFeature::BONE:
		case Library::Data::Parts::Animation::KindFeature::MOVENODE:
		case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
			/* MEMO: Drawクラスを併用しない（描画パーツでない）ので、追加ワークエリアは存在しない。 */
			break;

		case Library::Data::Parts::Animation::KindFeature::MESH:
			{
				/* MEMO: メッシュパーツの頂点数はメッシュセルではなく頂点バインドから取得する。 */
				Library::Data::Parts::Animation dataPartsAnimation;
				entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);
				Library::Data::Parts::Animation::BindMesh dataBindMesh;
				dataPartsAnimation.Mesh(&dataBindMesh);

				sizeWorkArea += Draw::SizeGetWorkArea(entity, idParts, dataBindMesh.CountVertex(), true);
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::CAMERA:
			/* MEMO: Drawクラスを併用しない（描画パーツでない）ので、追加ワークエリアは存在しない。 */
			break;

		case Library::Data::Parts::Animation::KindFeature::TERMINATOR:	/* ※ClangのWarning除け */
			break;
	}

	/* MEMO: アライメントしておく。 */
	sizeWorkArea = Memory::Alignment(sizeWorkArea);

	return(sizeWorkArea);
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Parts::BootUp(Entity& entity, int idParts, int& indexControlDraw, int& indexEntityUnderControlInstance, int& indexEntityUnderControlEffect, void*& workArea)
{
	/* 領域初期化 */
	CleanUp();

	IDParts = idParts;
	Status = FlagBitStatus::CLEAR;

	/* パーツ種別毎での専用初期化 */
	/* MEMO: 描画パーツの場合はIndexControlDrawを設定すること。 */
	switch(entity.FeatureGetParts(idParts))	{
		case Library::Data::Parts::Animation::KindFeature::ROOT:
		case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
			break;

		case Library::Data::Parts::Animation::KindFeature::NORMAL:
			{
//				IndexEntityChild = -1;

				IndexControlDraw = indexControlDraw;
				indexControlDraw++;
				Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];

				if(false == controlDraw->BootUp(Library::KindVertex::TERMINATOR4, false, workArea, false))	{
					goto BootUp_ErrorEnd;
				}
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::INSTANCE:
			/* MEMO: インスタンスは専用の初期化処理を追加で呼び出します。 */
			/*       ※caseの中に書くには処理が大きいため。               */
			if(false == BootUpInstance(entity, idParts, indexControlDraw, indexEntityUnderControlInstance, workArea))	{
				goto BootUp_ErrorEnd;
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::EFFECT:
			/* MEMO: インスタンスは専用の初期化処理を追加で呼び出します。 */
			/*       ※caseの中に書くには処理が大きいため。               */
			if(false == BootUpEffect(entity, idParts, indexControlDraw, indexEntityUnderControlEffect, workArea))	{
				goto BootUp_ErrorEnd;
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::MASK:
			{
//				IndexEntityChild = -1;

				IndexControlDraw = indexControlDraw;
				indexControlDraw++;
				Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];

				if(false == controlDraw->BootUp(Library::KindVertex::TERMINATOR4, true, workArea, true))	{
					goto BootUp_ErrorEnd;
				}
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::JOINT:
		case Library::Data::Parts::Animation::KindFeature::BONE:
		case Library::Data::Parts::Animation::KindFeature::MOVENODE:
		case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
			break;

		case Library::Data::Parts::Animation::KindFeature::MESH:
			{
//				IndexEntityChild = -1;

				IndexControlDraw = indexControlDraw;
				indexControlDraw++;
				Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];

				if(false == controlDraw->BootUpMesh(entity, idParts, false, workArea))	{
					goto BootUp_ErrorEnd;
				}
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::CAMERA:
			break;

		default:	/* MEMO: Clangの警告除け */
			break;
	}

//	AnimationChange();
	Status |= FlagBitStatus::VALID;
	/* MEMO: 起動直後は変形行列をリフレッシュするようにしておく */
	Status |= (	FlagBitStatus::CHANGE_TRANSFORM_POSITION
				| FlagBitStatus::CHANGE_TRANSFORM_ROTATION
				| FlagBitStatus::CHANGE_TRANSFORM_SCALING
			);

	return(true);

	/* エラー時 */
BootUp_ErrorEnd:;
	CleanUp();
	Status = FlagBitStatus::CLEAR;

	return(false);
}
bool Parts::BootUpInstance(Entity& entity, int idParts, int& indexControlDraw, int& indexEntityUnderControlInstance, void*& workArea)
{
	Entity* entityUnderControl = nullptr;
	Library::Control::Animation::Draw* controlDraw = nullptr;

	/* 直前のキーのデコードフレームをクリア */
	FramePreviousUpdateUnderControl = -1;

	/* 子エンティティのインスタンス再生情報を取得 */
	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);

	const char* nameAnimation = dataPartsAnimation.NameAnimationUnderControl();
	IndexPackUnderControl= dataPartsAnimation.IndexAnimationPackUnderControl();
	if(0 > IndexPackUnderControl)	{	/* 不正 */
		goto BootUpInstance_ErrorEnd;
	}

	/* 子エンティティを作成・起動 */
	IndexEntityUnderControl = indexEntityUnderControlInstance;
	indexEntityUnderControlInstance++;

	entityUnderControl = &entity.ListEntityChildInstance[IndexEntityUnderControl];

	/* MEMO: 子エンティティの再生トラックは必ず1つ。 */
	/* MEMO: 親エンティティを指定しないと描画モデルなど設定で齟齬が起こるので注意してください。 */
	if(false == entityUnderControl->BootUp(*entity.DataProject, 1, &entity))	{	/* 失敗 */
		goto BootUpInstance_ErrorEnd;
	}

	/* 子エンティティのアニメーションパック設定 */
	if(false == entityUnderControl->PackSet(IndexPackUnderControl))	{	/* 失敗 */
		goto BootUpInstance_ErrorEnd;
	}
	IndexAnimationUnderControl = entityUnderControl->IndexGetAnimation(nameAnimation);
	if(0 > IndexAnimationUnderControl)	{
		goto BootUpInstance_ErrorEnd;
	}
	/* MEMO: 再生しないままだとインスタンス側が何も表示しないので、一回再生してから停止しています。 */
	entityUnderControl->AnimationPlay(-1, IndexAnimationUnderControl);	/* 一旦アニメーションを設定 */
	entityUnderControl->AnimationStop(-1);	/* アニメーションを停止 */

	/* 描画の作成 */
	IndexControlDraw = indexControlDraw;
	indexControlDraw++;

	controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
	if(false == controlDraw->BootUpInstance(false, workArea))	{	/* 失敗 */
		goto BootUpInstance_ErrorEnd;
	}

	return(true);

BootUpInstance_ErrorEnd:;
	if(nullptr != controlDraw)	{
		/* MEMO: 値を巻き戻しておく。 */
		indexControlDraw = IndexControlDraw;
		IndexControlDraw = -1;
	}

	if(nullptr != entityUnderControl)	{
		/* MEMO: 値を巻き戻しておく。 */
		indexEntityUnderControlInstance = IndexEntityUnderControl;
		IndexEntityUnderControl = -1;

		entityUnderControl->ShutDown();
	}

	return(false);
}
bool Parts::BootUpEffect(Entity& entity, int idParts, int& indexControlDraw, int& indexEntityUnderControlEffect, void*& workArea)
{
	EntityEffect* entityUnderControl = nullptr;
	Library::Control::Animation::Draw* controlDraw = nullptr;

	/* 直前のキーのデコードフレームをクリア */
	FramePreviousUpdateUnderControl = -1;

	/* 子エンティティのエフェクト再生情報を取得 */
	/* MEMO: IndexPackUnderControlはエフェクトパーツではエフェクト番号になります。                                                       */
	/*       ※IndexAnimationUnderControlはエフェクトパーツの場合は常に-1です（エフェクトは複数のアニメーションを内部に持たないので）。  */
	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);

	IndexPackUnderControl = dataPartsAnimation.IndexEffectDataUnderControl();
	if(0 > IndexPackUnderControl)	{	/* 不正 */
		goto BootUpEffect_ErrorEnd;
	}
	IndexAnimationUnderControl = -1;

	/* 子エンティティを作成・起動 */
	IndexEntityUnderControl = indexEntityUnderControlEffect;
	indexEntityUnderControlEffect++;

	entityUnderControl = &entity.ListEntityChildEffect[IndexEntityUnderControl];
	/* MEMO: 親エンティティを指定しないと描画モデルなど設定で齟齬が起こるので注意してください。 */
	if(false == entityUnderControl->BootUp(*entity.DataProject, &entity))	{	/* エフェクトエンティティの起動（プロジェクトの設定） */
		goto BootUpEffect_ErrorEnd;
	}

	if(false == entityUnderControl->DataSet(IndexPackUnderControl))	{	/* エフェクトエンティティにエフェクトデータを設定 */
		goto BootUpEffect_ErrorEnd;
	}

	/* 描画の作成 */
	IndexControlDraw = indexControlDraw;
	indexControlDraw++;

	controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
	if(false == controlDraw->BootUpEffect(false, workArea))	{	/* 失敗 */
		goto BootUpEffect_ErrorEnd;
	}

	return(true);

BootUpEffect_ErrorEnd:;
	if(nullptr != controlDraw)	{
		/* MEMO: 値を巻き戻しておく。 */
		indexControlDraw = IndexControlDraw;
		IndexControlDraw = -1;
	}

	if(nullptr != entityUnderControl)	{
		/* MEMO: 値を巻き戻しておく。 */
		indexEntityUnderControlEffect = IndexEntityUnderControl;
		IndexEntityUnderControl = -1;

		entityUnderControl->ShutDown();
	}

	return(false);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Parts::ShutDown(Entity& entity, int idParts)
{
	/* パーツ種別毎での専用初期化 */
	/* MEMO: 描画パーツの場合はIndexControlDrawを設定すること。 */
	switch(entity.FeatureGetParts(idParts))	{
		case Library::Data::Parts::Animation::KindFeature::ROOT:
		case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
			break;

		case Library::Data::Parts::Animation::KindFeature::NORMAL:
			{
				Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
				controlDraw->ShutDown();
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::INSTANCE:
			ShutDownInstance(entity, idParts);
			break;

		case Library::Data::Parts::Animation::KindFeature::EFFECT:
			ShutDownEffect(entity, idParts);
			break;

		case Library::Data::Parts::Animation::KindFeature::MASK:
			{
				Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
				controlDraw->ShutDown();
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::JOINT:
		case Library::Data::Parts::Animation::KindFeature::BONE:
		case Library::Data::Parts::Animation::KindFeature::MOVENODE:
		case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
			break;

		case Library::Data::Parts::Animation::KindFeature::MESH:
			{
				Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
				controlDraw->ShutDownMesh();
			}
			break;

		case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::CAMERA:
			break;

		default:	/* MEMO: Clangの警告除け */
			break;
	}

	/* 領域未使用 */
	CleanUp();
}
void Parts::ShutDownInstance(Entity& entity, int idParts)
{
	/* 子エンティティを終了 */
	/* MEMO: ここで子エンティティのデストラクタを走らせてはいけない。 */
	if(0 <= IndexEntityUnderControl)	{
		Entity& entityUnderControl = entity.ListEntityChildInstance[IndexEntityUnderControl];
		entityUnderControl.ShutDown();
	}

	/* 描画制御を終了 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
	controlDraw->ShutDownInstance();
}
void Parts::ShutDownEffect(Entity& entity, int idParts)
{
	/* 子エンティティを終了 */
	/* MEMO: ここで子エンティティのデストラクタを走らせてはいけない。 */
	if(0 <= IndexEntityUnderControl)	{
		EntityEffect& entityUnderControl = entity.ListEntityChildEffect[IndexEntityUnderControl];
		entityUnderControl.ShutDown();
	}

	/* 描画制御を終了 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
	controlDraw->ShutDownEffect();
}

/* ********************************************************* */
//! 更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Parts::Update(Entity& entity, int idParts, ArgumentContainer& argument, bool flagHideDefault, int indexTrackRoot, const FPU::Matrix4x4& matrixParent)
{
	/* MEMO: トラックトランジションと（停止時の）フレームジャンプの処理を行う必要があるので、 */
	/*       再生状態だけで判定して処理をスキップするべきではないことに注意。                 */

	int indexTrack = IndexControlTrack;
	if(0 > indexTrack)	{	/* トラックに割り当たっていない */
		return;
	}

	Library::Control::Animation::Track& controlTrack = entity.TableControlTrack[indexTrack];
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;
	if(0 > indexAnimation)	{	/* 再生していない */
		return;
	}

	/* MEMO: 一応再生中のアニメーションなので、パーツIDはないわけはないと思うのでチェックをしていません。 */
	Library::Data::Animation::Body dataAnimationBody;
	Library::Data::Animation::Parts dataAnimationParts;
	entity.DataAnimation->TableAnimation(&dataAnimationBody, indexAnimation);
	dataAnimationBody.TableParts(&dataAnimationParts, idParts);

	/* 更新: ローカル変形行列 */
	/* MEMO: アニメーションのリフレッシュなどの関係もあるので、常に本処理は行われること。 */
	UpdateMatrixTransform(entity, idParts, argument, dataAnimationParts, controlTrack, matrixParent);

	/* 未使用パーツなら後の処理をしない */
	if(0 != (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NOT_USED))	{	/* アニメーションで未使用 */
		return;
	}

	/* 更新: ユーザデータ・シグナル */
	/* MEMO: ユーザデータとシグナルのデコードは（スキップフレームが絡むため）重いので、 */
	/*       それぞれ定められた条件が全て成立した時しかデコードしません。               */
	if(true == controlTrack.StatusGetIsPlaying())	{	/* 再生中 */
		/* ユーザーデータ */
		/* MEMO: ユーザーデータのデコード条件は下記です。                                                                 */
		/*       - パーツの再生トラックが再生中。                                                                         */
		/*       - エンティティにユーザデータ検知コールバック関数が設定されている。                                       */
		/*       - 自身のエンティティが最親（インスタンスのエンティティではユーザデータは無効になるプレーヤ共通の仕様）。 */
		/*       - アニメーションデータ中の自身パーツがユーザデータを持っている。                                         */
		if(nullptr == entity.EntityParent)	{	/* 自分が最親 */
			if(true == entity.CallBackUserData.IsValid())	{	/* コールバック関数が設定されている */
				if(0 == (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NO_USERDATA))	{	/* ユーザデータを持っている */
					UpdateUserData(entity, idParts, dataAnimationParts, controlTrack);
				}
			}
		}

		/* シグナル */
		/* MEMO: シグナルは下記の条件が全て成立した時にデコード（検知処理）を行います。                               */
		/*       - パーツの再生トラックが再生中。                                                                     */
		/*       - エンティティにシグナル検知コールバック関数が設定されている。                                       */
		/*       - 自身のエンティティが最親（インスタンスのエンティティではシグナルは無効になるプレーヤ共通の仕様）。 */
		/*       - アニメーションデータ中の自身パーツがシグナルを持っている。                                         */
#if 0	/* MEMO: ユーザデータとも仕様を併せた動作仕様（こちらの方が良い気がします）。 */
		if(nullptr == entity.EntityParent)	{	/* 自分が最親 */
#else	/* MEMO: シグナルは現状最親エンティティでなくてもデコードします（暫定仕様）。 */
		{
#endif
			if(true == entity.CallBackSignal.IsValid())	{	/* コールバック関数が設定されている */
				if(0 == (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NO_SIGNAL))	{	/* シグナルを持っている */
					UpdateSignal(entity, idParts, dataAnimationParts, controlTrack);
				}
			}
		}
	}

	/* 更新: パーツ機能毎の個別処理 */
	switch(entity.FeatureGetParts(idParts))	{
		case Library::Data::Parts::Animation::KindFeature::ROOT:
		case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
			break;

		case Library::Data::Parts::Animation::KindFeature::NORMAL:
			UpdateNormal(entity, idParts, argument, dataAnimationParts, controlTrack);
			UpdateSetPartsDraw(entity, idParts, flagHideDefault, false, false, indexTrackRoot);
			break;

		case Library::Data::Parts::Animation::KindFeature::INSTANCE:
			/* MEMO: UpdateInstanceは設計概念上でだけ存在し、現時点では実効しない関数です。 */
//			UpdateInstance(entity, idParts, argument, dataAnimationParts, controlTrack);
			UpdateSetPartsDraw(entity, idParts, flagHideDefault, true, false, indexTrackRoot);
			break;

		case Library::Data::Parts::Animation::KindFeature::EFFECT:
			/* MEMO: UpdateEffectは設計概念上でだけ存在し、現時点では実効しない関数です。 */
//			UpdateEffect(entity, idParts, argument, dataAnimationParts, controlTrack);
			UpdateSetPartsDraw(entity, idParts, flagHideDefault, true, false, indexTrackRoot);
			break;

		case Library::Data::Parts::Animation::KindFeature::MASK:
			/* MEMO: マスクは描画時に使用するシェーダが異なるだけで、処理そのものは通常パーツと同じです。 */
			UpdateNormal(entity, idParts, argument, dataAnimationParts, controlTrack);
			UpdateSetPartsDraw(entity, idParts, flagHideDefault, false, true, indexTrackRoot);
			break;

		case Library::Data::Parts::Animation::KindFeature::JOINT:
			break;

		case Library::Data::Parts::Animation::KindFeature::BONE:
			/* MEMO: SS6Player for Unityではボーン専用の行列を持っていましたが、 */
			/*       C++版ではMatrixTransformを使用します。                      */
			/*       ※SS6PUで専用行列が必要だったのは、行列の補正が必要だったた */
			/*         めで、C++版ではその補正が必要なくなったのが理由です。     */
//			break;
			/* Fall-Through */
		case Library::Data::Parts::Animation::KindFeature::MOVENODE:
		case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
			break;

		case Library::Data::Parts::Animation::KindFeature::MESH:
			UpdateMesh(entity, idParts, argument, dataAnimationParts, controlTrack);
			UpdateSetPartsDraw(entity, idParts, flagHideDefault, false, false, indexTrackRoot);
			break;

		case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
			break;

		case Library::Data::Parts::Animation::KindFeature::CAMERA:
			/* MEMO: 現状、カメラは子も含んだ全エンティティの中で最後に出てきたものが適用されるので */
			/*       子も親も関係なく走査順で適用されます。                                         */
			argument.MatrixTransformPartsCamera = &MatrixTransform;
			break;

		default:	/* MEMO: Clangの警告除け */
			break;
	}

	/* 更新: コライダ */
	/* MEMO: C++版はコライダをサポートしていないので、現在何も処理していません。 */
}
void Parts::UpdateMatrixTransform(	Entity& entity,
									int idParts,
									ArgumentContainer& argument,
									const Library::Data::Animation::Parts& dataAnimationParts,
									Control::Animation::Track& controlTrack,
									const FPU::Matrix4x4& matrixParent
								)
{	/* MEMO: SS6Player for UnityのUpdateGameObjectとほぼ等価の処理です。 */
	/* アトリビュート取得用の設定 */
	controlTrack.ArgumentContainer.IDParts = idParts;

	/* パーツのステータスを取得 */
	Uint32 statusParts = dataAnimationParts.StatusParts;
	StatusAnimationParts = statusParts;	/* 描画処理側で使うためにクラス内にキャッシュしています。 */

	/* トラックトランジション関連の更新 */
	int indexTrackSlave = controlTrack.IndexTrackSlave;
	bool flagNotTransition = (0 > indexTrackSlave);	/* ? true : false */	/* ※都度スレーブのインデックスでトランジションしているかを判定するのは判りづらいので…… */
	int indexAnimationSlave;
	Library::Data::Animation::Body dataAnimationBodySlave;	/* 何度も使用するので表に出しておきます */
	Float32 rateTransition;
	Float32 rateTransitionInverse;
	Uint32 statusPartsSlave;
	const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
	const Library::Data::Project::Animation* dataProjectAnimationSlave = nullptr;
	Library::Control::Animation::Track* controlTrackSlave = nullptr;	/* スレーブは参照ではないので注意（有効だったり無効だったりするので……） */
	Library::Data::Animation::Parts dataAnimationPartsSlave;

	if(true == flagNotTransition)	{	/* トランジションしていない */
		/* MEMO: dataAnimationSlaveは放置です。 */
		indexAnimationSlave = -1;
		statusPartsSlave = (	Library::Data::Animation::Parts::FlagBitStatus::NO_POSITION
								| Library::Data::Animation::Parts::FlagBitStatus::NO_ROTATION
								| Library::Data::Animation::Parts::FlagBitStatus::NO_SCALING
						);

		rateTransition = 0.0f;
		rateTransitionInverse = 1.0f;
	} else {	/* トランジション中 */
		controlTrackSlave = &entity.TableControlTrack[indexTrackSlave];
		controlTrackSlave->ArgumentContainer.IDParts = idParts;
		indexAnimationSlave = controlTrackSlave->ArgumentContainer.IndexAnimation;
		if(0 > indexAnimationSlave)	{	/* アニメーションを再生していない（不正状態） */
			/* MEMO: 不正状態の場合、スレーブは再生していないものと扱います。 */
			/* MEMO: dataAnimationSlaveは放置です。 */
			indexAnimationSlave = -1;
			statusPartsSlave = (	Library::Data::Animation::Parts::FlagBitStatus::NO_POSITION
									| Library::Data::Animation::Parts::FlagBitStatus::NO_ROTATION
									| Library::Data::Animation::Parts::FlagBitStatus::NO_SCALING
							);

			rateTransition = 0.0f;
			rateTransitionInverse = 1.0f;
		} else {	/* Valid */
			entity.DataAnimation->TableAnimation(&dataAnimationBodySlave, indexAnimationSlave);
			dataAnimationBodySlave.TableParts(&dataAnimationPartsSlave, idParts);
			statusPartsSlave = dataAnimationPartsSlave.StatusParts;

			rateTransition = controlTrack.RateTransition;
			rateTransitionInverse = 1.0f - rateTransition;

			dataProjectAnimationSlave = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrackSlave->ArgumentContainer.DataAnimation);
		}
	}

	/* MEMO: ロジックをわかり易くするためにTRSで分けて書いてありますが……  */
	/*       その分分岐が多くなっているので、後に最適化するかもしれません。 */
	bool flagUpdate;
	CPU::Vector3 valueTRSMaster;
	CPU::Vector3 valueTRSSlave;
	FPU::Vector3 valueFPUTRSMaster;
	FPU::Vector3 valueFPUTRSSlave;

	if(true == controlTrack.StatusGetIsTransitionStart())	{	/* トランジションが開始している */
		TRSSlave.CleanUp();
	}

	/* 更新: Translate（Position） */
	{
		flagUpdate = false;
		if(0 == (statusParts & Library::Data::Animation::Parts::FlagBitStatus::NO_POSITION))	{	/* Positionのアニメーションを持っている */
			if(true == dataProjectAnimation->ExpanderPosition->ValueGet(	&TRSMaster.Position.Value, &TRSMaster.Position.FrameKey,
																			dataAnimationParts.Position,
																			controlTrack.ArgumentContainer
																	)
			)	{	/* 更新 */
				flagUpdate = true;
			}
			valueTRSMaster.Set(TRSMaster.Position.Value);
		} else {	/* Positionのアニメーションを持っていない*/
			static const Uint32 flagBitStatusCheck = FlagBitStatus::CHANGE_TRANSFORM_POSITION | FlagBitStatus::REFRESH_TRANSFORM_POSITION;
			if(flagBitStatusCheck == (Status & flagBitStatusCheck))	{	/* 値のリフレッシュ */
				flagUpdate = true;
			}
			valueTRSMaster.Set(CPU::Vector3::Zero);
		}
		if(true == flagNotTransition)	{	/* トランジションしていない */
			if(true == flagUpdate)	{	/* 更新されている */
				/* 描画用情報に設定 */
				TransformPosition.Set(valueTRSMaster);
				Status |= FlagBitStatus::CHANGE_TRANSFORM_POSITION;
			}
		} else {	/* トランジション中 */
			/* スレーブ側の処理 */
			if(0 == (statusPartsSlave & Library::Data::Animation::Parts::FlagBitStatus::NO_POSITION))	{	/* Positionのアニメーションを持っている */
				if(true == dataProjectAnimation->ExpanderPosition->ValueGet(	&TRSSlave.Position.Value, &TRSSlave.Position.FrameKey,
																				dataAnimationPartsSlave.Position,
																				controlTrackSlave->ArgumentContainer
																		)
				)	{	/* 更新 */
					flagUpdate |= true;
				}
				valueTRSSlave = TRSSlave.Position.Value;
			} else {	/* Positionのアニメーションを持っていない */
				valueTRSSlave = CPU::Vector3::Zero;
			}

			/* マスタとスレーブを合成して設定 */
			/* MEMO: 合成率（rateTransition）は常に変化しているので、更新を確認していません。 */
			/* MEMO: 直線補間です。 */
			valueFPUTRSMaster.Set(valueTRSMaster);
			valueFPUTRSMaster.Set(valueTRSMaster);
			VectorMul(&valueFPUTRSMaster, valueFPUTRSMaster, rateTransitionInverse);	/* valueTRSMaster * rateTransitionInverse */
			VectorMul(&valueFPUTRSMaster, valueFPUTRSMaster, rateTransition);	/* valueTRSSlave * rateTransition */
			VectorAdd(&TransformPosition, valueFPUTRSMaster, valueFPUTRSMaster);

			Status |= FlagBitStatus::CHANGE_TRANSFORM_POSITION;
		}
	}

	/* 更新: Rotation */
	{
		flagUpdate = false;
		if(0 == (statusParts & Library::Data::Animation::Parts::FlagBitStatus::NO_ROTATION))	{	/* Rotationのアニメーションを持っている */
			if(true == dataProjectAnimation->ExpanderRotation->ValueGet(	&TRSMaster.Rotation.Value, &TRSMaster.Rotation.FrameKey,
																			dataAnimationParts.Rotation,
																			controlTrack.ArgumentContainer
																	)
			)	{	/* 更新 */
				flagUpdate = true;
			}
			valueTRSMaster.Set(TRSMaster.Rotation.Value);
		} else {	/* Rotationのアニメーションを持っていない */
			static const Uint32 flagBitStatusCheck = FlagBitStatus::CHANGE_TRANSFORM_ROTATION | FlagBitStatus::REFRESH_TRANSFORM_ROTATION;
			if(flagBitStatusCheck == (Status & flagBitStatusCheck))	{	/* 値のリフレッシュ */
				flagUpdate = true;
			}
			valueTRSMaster.Set(CPU::Vector3::Zero);
		}
		if(true == flagNotTransition)	{	/* トランジションしていない */
			if(true == flagUpdate)	{	/* 更新されている */
				/* 描画用情報に設定 */
				TransformRotation.Set(valueTRSMaster);
				Status |= FlagBitStatus::CHANGE_TRANSFORM_ROTATION;
			}
		} else {	/* トランジション中 */
			/* スレーブ側の処理 */
			if(0 == (statusPartsSlave & Library::Data::Animation::Parts::FlagBitStatus::NO_ROTATION))	{	/* Rotationのアニメーションを持っている */
				if(true == dataProjectAnimation->ExpanderRotation->ValueGet(	&TRSSlave.Rotation.Value, &TRSSlave.Rotation.FrameKey,
																				dataAnimationPartsSlave.Rotation,
																				controlTrackSlave->ArgumentContainer
																		)
				)	{	/* 更新 */
					flagUpdate |= true;
				}
				valueTRSSlave = TRSSlave.Rotation.Value;
			} else {	/* Rotationのアニメーションを持っていない */
				valueTRSSlave = CPU::Vector3::Zero;
			}

			/* マスタとスレーブを合成して設定 */
			/* MEMO: 合成率（rateTransition）は常に変化しているので、更新を確認していません。 */
			/* MEMO: 直線補間です。 */
			valueFPUTRSMaster.Set(valueTRSMaster);
			valueFPUTRSMaster.Set(valueTRSMaster);
			VectorMul(&valueFPUTRSMaster, valueFPUTRSMaster, rateTransitionInverse);	/* valueTRSMaster * rateTransitionInverse */
			VectorMul(&valueFPUTRSMaster, valueFPUTRSMaster, rateTransition);	/* valueTRSSlave * rateTransition */
			VectorAdd(&TransformRotation, valueFPUTRSMaster, valueFPUTRSMaster);

			Status |= FlagBitStatus::CHANGE_TRANSFORM_ROTATION;
		}
	}

	/* 更新: Scaling */
	{
		flagUpdate = false;
		if(0 == (statusParts & Library::Data::Animation::Parts::FlagBitStatus::NO_SCALING))	{	/* Scalingのアニメーションを持っている */
			if(true == dataProjectAnimation->ExpanderScaling->ValueGet(	&TRSMaster.Scaling.Value, &TRSMaster.Scaling.FrameKey,
																		dataAnimationParts.Scaling,
																		controlTrack.ArgumentContainer
																	)
			)	{	/* 更新 */
				flagUpdate = true;
			}
			valueTRSMaster.Set(TRSMaster.Scaling.Value.GetX(), TRSMaster.Scaling.Value.GetY(), 1.0f);
		} else {	/* Scalingのアニメーションを持っていない */
			static const Uint32 flagBitStatusCheck = FlagBitStatus::CHANGE_TRANSFORM_SCALING | FlagBitStatus::REFRESH_TRANSFORM_SCALING;
			if(flagBitStatusCheck == (Status & flagBitStatusCheck))	{	/* 値のリフレッシュ */
				flagUpdate = true;
			}
			valueTRSMaster.Set(CPU::Vector3::One);
		}
		if(true == flagNotTransition)	{	/* トランジションしていない */
			if(true == flagUpdate)	{	/* 更新されている */
				/* 描画用情報に設定 */
				TransformScaling.Set(valueTRSMaster);
				Status |= FlagBitStatus::CHANGE_TRANSFORM_SCALING;
			}
		} else {	/* トランジション中 */
			/* スレーブ側の処理 */
			if(0 == (statusPartsSlave & Library::Data::Animation::Parts::FlagBitStatus::NO_SCALING))	{	/* Scalingのアニメーションを持っている */
				if(true == dataProjectAnimation->ExpanderScaling->ValueGet(	&TRSSlave.Scaling.Value, &TRSSlave.Scaling.FrameKey,
																			dataAnimationPartsSlave.Scaling,
																			controlTrackSlave->ArgumentContainer
																		)
				)	{	/* 更新 */
					flagUpdate |= true;
				}
				valueTRSSlave.Set(TRSSlave.Scaling.Value.GetX(), TRSSlave.Scaling.Value.GetY(), 1.0f);
			} else {	/* Scalingのアニメーションを持っていない */
				valueTRSSlave.Set(CPU::Vector3::One);
			}

			/* マスタとスレーブを合成して設定 */
			/* MEMO: 合成率（rateTransition）は常に変化しているので、更新を確認していません。 */
			/* MEMO: 直線補間です。 */
			valueFPUTRSMaster.Set(valueTRSMaster);
			valueFPUTRSMaster.Set(valueTRSMaster);
			VectorMul(&valueFPUTRSMaster, valueFPUTRSMaster, rateTransitionInverse);	/* valueTRSMaster * rateTransitionInverse */
			VectorMul(&valueFPUTRSMaster, valueFPUTRSMaster, rateTransition);	/* valueTRSSlave * rateTransition */
			VectorAdd(&TransformScaling, valueFPUTRSMaster, valueFPUTRSMaster);

			Status |= FlagBitStatus::CHANGE_TRANSFORM_SCALING;
		}
	}

	/* 何かしらの更新要求があった場合行列を再計算 */
	if(0 != (Status & (	FlagBitStatus::CHANGE_TRANSFORM_POSITION
						| FlagBitStatus::CHANGE_TRANSFORM_ROTATION
						| FlagBitStatus::CHANGE_TRANSFORM_SCALING
						| FlagBitStatus::REFRESH_TRANSFORM_POSITION
						| FlagBitStatus::REFRESH_TRANSFORM_ROTATION
						| FlagBitStatus::REFRESH_TRANSFORM_SCALING
					)
			)
	)	{	/* 再計算 */
		MatrixCreateTRS(&MatrixTransformLocal, TransformPosition, TransformRotation, TransformScaling);
	}
	/* 各種行列を求める */
	/* MEMO: ローカル行列は変化していなくても親が変化している可能性が高いので、 */
	/*       継承する行列は常に算出する。                                       */
	Parts* parent = NULL;
	{
		/* 親のパーツ情報を取得 */
		int idPartsParent = entity.DataAnimation->IDGetPartsParent(idParts);
		if(0 > idPartsParent)	{	/* 親がいない（Rootパーツ） */
			/* MEMO: ローカル行列と継承済みローカル行列は同じ。 */
			/* MEMO: ここでワールド行列を加味すると、1インスタンスの複数描画ができないので、しないこと。 */
			MatrixMul(&MatrixTransform, MatrixTransformLocal, matrixParent);	/* 親の行列を継承（ローカル行列スタック） */
		} else {	/* 親がいる */
			/* MEMO: 親の変形行列を継承（実質これが行列スタックの形成です）。 */
			/* MEMO: 親のパーツIDは、必ず自分のパーツIDより若いことはSS6の仕様として */
			/*       保障されているので、すでに親は行列の算出が終わっています。      */
			parent = &entity.TableControlParts[idPartsParent];
			MatrixMul(&MatrixTransform, MatrixTransformLocal, parent->MatrixTransform);	/* 継承済みローカル行列（ローカル行列スタック） */
		}
	}

	/* リフレッシュ要求をクリア */
	Status &= ~(FlagBitStatus::REFRESH_TRANSFORM_POSITION | FlagBitStatus::REFRESH_TRANSFORM_ROTATION | FlagBitStatus::REFRESH_TRANSFORM_SCALING);

	/* 更新: 表示状態 */
	dataProjectAnimation->ExpanderStatus->ValueGet(	&StatusAnimationFrame, &FrameKeyStatusAnimationFrame,
													dataAnimationParts.Status,
													controlTrack.ArgumentContainer
												);
	if(true == StatusAnimationFrame.IsHide())	{
		Status |= FlagBitStatus::HIDE;
	} else {
		Status &= ~FlagBitStatus::HIDE;
	}

	/* パーツの未使用状態をチェック */
	if(0 != (statusParts & Library::Data::Animation::Parts::FlagBitStatus::NOT_USED))	{	/* 未使用 */
		return;
	}

	/* 更新: ローカルスケール */
	if(true == dataProjectAnimation->ExpanderScalingLocal->ValueGet(	&ScaleLocal.Value, &ScaleLocal.FrameKey,
																		dataAnimationParts.ScalingLocal,
																		controlTrack.ArgumentContainer
																)
	)	{	/* 更新 */
		Status |= FlagBitStatus::UPDATE_SCALELOCAL;
	}

	/* 更新: 不透明度 */
	if(true == dataProjectAnimation->ExpanderRateOpacity->ValueGet(	&RateOpacity.Value, &RateOpacity.FrameKey,
																	dataAnimationParts.RateOpacity,
																	controlTrack.ArgumentContainer
																)
	)	{	/* 更新 */
		Status |= FlagBitStatus::UPDATE_RATEOPACITY;
	}

	/* 更新: 優先度 */
	dataProjectAnimation->ExpanderPriority->ValueGet(	&Priority.Value, &Priority.FrameKey,
														dataAnimationParts.Priority,
														controlTrack.ArgumentContainer
													);
}
void Parts::UpdateNormal(	Entity& entity,
							int idParts,
							ArgumentContainer& argument,
							const Library::Data::Animation::Parts& dataAnimationParts,
							Control::Animation::Track& controlTrack
						)
{	/* MEMO: 通常パーツ用の更新 */
	if(0 > IndexControlDraw)	{	/* 描画ワークが設定されていない */
		return;
	}

	/* 描画情報を更新 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
	controlDraw->StatusSetFlip(StatusAnimationFrame);
	controlDraw->Update(entity, idParts, *this, argument, dataAnimationParts, Status, controlTrack);
}
void Parts::UpdateInstance(	Entity& entity,
							int idParts,
							ArgumentContainer& argument,
							const Library::Data::Animation::Parts& dataAnimationParts,
							Control::Animation::Track& controlTrack
						)
{	/* MEMO: インスタンスパーツ用の更新 */
	/* MEMO: この関数は呼び出されません（この実体は機能明示性のためのダミーです）。                           */
	/*       本来この関数で実装されるべき処理は「DrawInstance」関数に（主に最適化の事情で）合成されています。 */
	/*       そのため、「インスタンス」パーツは、非表示状態に関係なく描画リストに登録されます。               */
}
void Parts::UpdateEffect(	Entity& entity,
							int idParts,
							ArgumentContainer& argument,
							const Library::Data::Animation::Parts& dataAnimationParts,
							Control::Animation::Track& controlTrack
						)
{	/* MEMO: エフェクトパーツ用の更新 */
	/* MEMO: この関数は呼び出されません（この実体は機能明示性のためのダミーです）。                         */
	/*       本来この関数で実装されるべき処理は「DrawEffect」関数に（主に最適化の事情で）合成されています。 */
	/*       そのため、「エフェクト」パーツは、非表示状態に関係なく描画リストに登録されます。               */
}
void Parts::UpdateMesh(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Control::Animation::Track& controlTrack
					)
{	/* MEMO: メッシュパーツ用の更新 */
	if(0 > IndexControlDraw)	{	/* 描画ワークが設定されていない */
		return;
	}

	/* 描画情報を更新 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
	controlDraw->StatusSetFlip(StatusAnimationFrame);
	controlDraw->Update(entity, idParts, *this, argument, dataAnimationParts, Status, controlTrack);
}
void Parts::UpdateSetPartsDraw(	Entity& entity,
								int idParts,
								bool flagHideDefault,
								bool flagSetForce,
								bool flagMask,
								int indexTrackRoot
							)
{
	if(	(true == flagSetForce)
		|| ((false == flagHideDefault) && (0 == (Status & (FlagBitStatus::HIDE_FORCE | FlagBitStatus::HIDE))))
	)	{
		/* MEMO: ソートキーはSS6Player for Unityでもintでしたが、本来はSint32が正しい宣言になります。 */
		/*       C++はintのビット幅が不安定なこともあるので明示しておくのが良いのですが、FlatBuffers  */
		/*       のキー定義もintになっているので、とりあえずそちらと定義を併せてあります。            */
		/* MEMO: ソートで扱えるパーツ数などの問題が起こった場合、キーをSint64に拡張して扱う方法が採れ */
		/*       るはずです（SS6PUでは、過去のAndroidのmonoでsigned longの計算を間違う場合があったた  */
		/*       めlong幅整数型の使用をなるだけ回避している側面から利用していません）。               */
		int keySort;
		int prio = Priority.Value;

		/* 描画前描画パーツリストに追加 */
		if(true == flagMask)	{	/* マスクパーツ */
			/* MEMO: マスクパーツは「PreDraw（マスク専用の事前描画）」では逆順になる。 */
			keySort = (-prio << CountShiftSortKeyPriority) | idParts;
			entity.ListPartsPreDraw.push_back(keySort);
		}

		/* 描画パーツリストに追加 */
		keySort = (prio << CountShiftSortKeyPriority) | idParts;
		entity.ListPartsDraw.push_back(keySort);

		/* アニメーション合成しているかの設定 */
		/* MEMO: アニメーション合成している場合、SS6ConverterVer2で事前に確定された */
		/*       パーツ描画順序が使えないため、リアルタイムでソートする必要がある。 */
		/* MEMO: StatusのANIMATION_SYNTHESIZEはEntityのUpdateでクリアされています。 */
		if(indexTrackRoot != IndexControlTrack)	{
			entity.Status |= Entity::FlagBitStatus::ANIMATION_SYNTHESIZE;
		}
	}
}

void Parts::UpdateUserData(Entity& entity, int idParts, const Library::Data::Animation::Parts& dataAnimationParts, Control::Animation::Track& controlTrack)
{
	if(	(true == controlTrack.StatusGetIsIgnoreUserData())
		|| (true == controlTrack.StatusGetIsIgnoreNextUpdateUserData())
		|| (false == controlTrack.StatusGetIsDecodeAttribute())
	)	{	/* ユーザーデータをデコードする必要がない */
		return;
	}

	/* MEMO: フレームスキップや逆再生なども絡んで走査領域構成が複雑になるために */
	/*       （純然とした場合分けでの）かなりパワフルな実装をしています。       */
	/*       ※下手にインテリジェンスにやるよりも、結局こちらの方が（コード自体 */
	/*         は冗長なものの実行コードステップは小さいので）軽いと思われます。 */
	int countLoop = controlTrack.CountLoopNow;
	if(true == controlTrack.StatusGetIsIgnoreSkipLoop())	{
		countLoop = 0;
	}
	bool flagLoop = (0 < countLoop);	/* ? true : false */
	bool flagFirst = controlTrack.StatusGetIsPlayingStart();
	bool flagReverse = controlTrack.StatusGetIsPlayingReverse();
	bool flagReversePrevious = controlTrack.StatusGetIsPlayingReversePrevious();
	bool flagTurn = controlTrack.StatusGetIsPlayingTurn();
	bool flagStylePingPong = controlTrack.StatusGetIsPlayStylePingpong();
	int frame = controlTrack.ArgumentContainer.Frame;
	int frameStart = controlTrack.FrameStart;
	int frameEnd = controlTrack.FrameEnd;
	int framePrevious = -1;

	/* デコードする先頭のフレームを決定 */
	if(true == flagFirst)	{
		framePrevious = frame;
	} else {
		framePrevious = controlTrack.ArgumentContainer.FramePrevious;
		if(true== flagReversePrevious)	{
			framePrevious--;
			if((false == flagTurn) && (framePrevious < frame))	{
				return;
			}
		} else {
			framePrevious++;
			if((false == flagTurn) && (framePrevious > frame))	{
				return;
			}
		}
	}

	/* ユーザデータのデコード */
	if(true == flagStylePingPong)	{	/* Play-Style: PingPong */
		bool FlagStyleReverse = controlTrack.StatusGetIsPlayStyleReverse();

		/* フレームスキップを考慮して区間を決定 */
		if(true == FlagStyleReverse)	{	/* Reverse */
			if(true == flagLoop)	{
				/* 先頭部分 */
				if(true == flagReversePrevious)	{
					framePrevious = controlTrack.ArgumentContainer.FramePrevious - 1;	/* Force */
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, false);
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, true);
				} else {
					framePrevious = controlTrack.ArgumentContainer.FramePrevious + 1;	/* Force */
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, true);
				}

				/* ループ区間 */
				for(int i=1; i<countLoop; i++)	{
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, false);
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, true);
				}

				/* 末尾区間＆現在位置 */
				if(true == flagReverse)	{	/* Now-Reverse */
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, false);
				} else {	/* Now-Foward */
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, false);
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, true);
				}
			} else {	/* Normal */
				if(true == flagTurn)	{	/* Turn-Back */
					/* MEMO: No-Loop & Turn-Back ... Always "Reverse to Foward" */
					framePrevious = controlTrack.ArgumentContainer.FramePrevious - 1;	/* Force */
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, false);
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, true);
				} else {	/* Normal */
					if(true == flagReverse)	{	/* Reverse */
						UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
					} else {	/* Foward */
						UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, true);
					}
				}
			}
		} else {	/* Normal */
			if(true == flagLoop)	{
				/* 先頭部分 */
				if(true == flagReversePrevious)	{
					framePrevious = controlTrack.ArgumentContainer.FramePrevious - 1;	/* Force */
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, true);
				} else {
					framePrevious = controlTrack.ArgumentContainer.FramePrevious + 1;	/* Force */
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, false);
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, true);
				}

				/* ループ区間 */
				for(int i=1; i<countLoop; i++)	{
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, false);
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, true);
				}

				/* 末尾区間＆現在位置 */
				if(true == flagReverse)	{	/* Now-Reverse */
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, false);
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, true);
				} else {	/* Now-Foward */
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, false);
				}
			} else {	/* Normal */
				if(true == flagTurn)	{	/* Turn-Back */
					/* MEMO: No-Loop & Turn-Back ... Always "Foward to Revese" */
					framePrevious = controlTrack.ArgumentContainer.FramePrevious + 1;	/* Force */
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, false);
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, true);
				} else {	/* Normal */
					if(true == flagReverse)	{	/* Reverse */
						UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, true);
					} else {	/* Foward */
						UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
					}
				}
			}
		}
	} else {	/* Play-Style: OneWay */
		/* フレームスキップを考慮して区間を決定 */
		if(true == flagReverse)	{	/* Backwards */
			if(true == flagTurn)	{	/* Wrap-Around */
				/* 先頭部分 */
				UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, false);

				/* ループ区間 */
				for(int j=1; j<countLoop ; j++)	{
					UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, false);
				}

				/* 末尾区間＆現在位置 */
				UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, false);
			} else {	/* Normal */
				UpdateUserDataReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
			}
		} else {	/* Foward */
			if(true == flagTurn)	{	/* Wrap-Around */
				/* 先頭部分 */
				UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, false);

				/* ループ区間 */
				for(int j=1; j<countLoop; j++)	{
					UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, false);
				}

				/* 末尾区間＆現在位置 */
				UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, false);
			}
			else
			{	/* Normal */
				UpdateUserDataFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
			}
		}
	}
}
void Parts::UpdateUserDataFoward(	Entity& entity,
									int idParts,
									const Library::Data::Animation::Parts& dataAnimationParts,
									Control::Animation::Track& controlTrack,
									int frameRangeStart,
									int frameRangeEnd,
									int frameDecode,
									bool flagTurnBack
								)
{	/* 順方向にユーザーデータを走査 */
	const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
	int countData = dataProjectAnimation->ExpanderUserData->CountGetValue(dataAnimationParts.UserData);
	const char* nameParts = entity.NameGetParts(idParts);
	int frameKey = -1;
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;
	Library::Data::Animation::Attribute::UserData userData;
	for(int i=0; i<countData; i++)	{
		if(true == dataProjectAnimation->ExpanderUserData->ValueGetIndex(&userData, &frameKey, i, dataAnimationParts.UserData, controlTrack.ArgumentContainer))	{
			if((frameRangeStart <= frameKey) && (frameRangeEnd >= frameKey))	{	/* 区間内にユーザーデータが存在 */
				entity.CallBackUserData.Function(	entity,
													entity.CallBackUserData.Pointer,
													nameParts,
													idParts,
													indexAnimation,
													frameDecode,
													frameKey,
													userData,
													flagTurnBack
												);
			}
		}
	}
}
void Parts::UpdateUserDataReverse(	Entity& entity,
									int idParts,
									const Library::Data::Animation::Parts& dataAnimationParts,
									Control::Animation::Track& controlTrack,
									int frameRangeEnd,
									int frameRangeStart,
									int frameDecode,
									bool flagTurnBack
								)
{	/* 逆方向にユーザーデータを走査 */
	const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
	int countData = dataProjectAnimation->ExpanderUserData->CountGetValue(dataAnimationParts.UserData);
	const char* nameParts = entity.NameGetParts(idParts);
	int frameKey = -1;
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;
	Library::Data::Animation::Attribute::UserData userData;
	for(int i=(countData-1); i>=0; i--)	{
		if(true == dataProjectAnimation->ExpanderUserData->ValueGetIndex(&userData, &frameKey, i, dataAnimationParts.UserData, controlTrack.ArgumentContainer))	{
			if((frameRangeStart <= frameKey) && (frameRangeEnd >= frameKey))	{	/* 区間内にユーザーデータが存在 */
				entity.CallBackUserData.Function(	entity,
													entity.CallBackUserData.Pointer,
													nameParts,
													idParts,
													indexAnimation,
													frameDecode,
													frameKey,
													userData,
													flagTurnBack
												);
			}
		}
	}
}

void Parts::UpdateSignal(Entity& entity, int idParts, const Library::Data::Animation::Parts& dataAnimationParts, Control::Animation::Track& controlTrack)
{
	if(	(true == controlTrack.StatusGetIsIgnoreSignal())
		|| (true == controlTrack.StatusGetIsIgnoreNextUpdateSignal())
		|| (false == controlTrack.StatusGetIsDecodeAttribute())
	)	{	/* シグナルをデコードする必要がない */
		return;
	}

	/* MEMO: フレームスキップや逆再生なども絡んで走査領域構成が複雑になるために */
	/*       （純然とした場合分けでの）かなりパワフルな実装をしています。       */
	/*       ※下手にインテリジェンスにやるよりも、結局こちらの方が（コード自体 */
	/*         は冗長なものの実行コードステップは小さいので）軽いと思われます。 */
	int countLoop = controlTrack.CountLoopNow;
	if(true == controlTrack.StatusGetIsIgnoreSkipLoop())	{
		countLoop = 0;
	}
	bool flagLoop = (0 < countLoop);	/* ? true : false */
	bool flagFirst = controlTrack.StatusGetIsPlayingStart();
	bool flagReverse = controlTrack.StatusGetIsPlayingReverse();
	bool flagReversePrevious = controlTrack.StatusGetIsPlayingReversePrevious();
	bool flagTurn = controlTrack.StatusGetIsPlayingTurn();
	bool flagStylePingPong = controlTrack.StatusGetIsPlayStylePingpong();
	int frame = controlTrack.ArgumentContainer.Frame;
	int frameStart = controlTrack.FrameStart;
	int frameEnd = controlTrack.FrameEnd;
	int framePrevious = -1;

	/* デコードする先頭のフレームを決定 */
	if(true == flagFirst)	{
		framePrevious = frame;
	} else {
		framePrevious = controlTrack.ArgumentContainer.FramePrevious;
		if(true== flagReversePrevious)	{
			framePrevious--;
			if((false == flagTurn) && (framePrevious < frame))	{
				return;
			}
		} else {
			framePrevious++;
			if((false == flagTurn) && (framePrevious > frame))	{
				return;
			}
		}
	}

	/* ユーザデータのデコード */
	if(true == flagStylePingPong)	{	/* Play-Style: PingPong */
		bool FlagStyleReverse = controlTrack.StatusGetIsPlayStyleReverse();

		/* フレームスキップを考慮して区間を決定 */
		if(true == FlagStyleReverse)	{	/* Reverse */
			if(true == flagLoop)	{
				/* 先頭部分 */
				if(true == flagReversePrevious)	{
					framePrevious = controlTrack.ArgumentContainer.FramePrevious - 1;	/* Force */
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, false);
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, true);
				} else {
					framePrevious = controlTrack.ArgumentContainer.FramePrevious + 1;	/* Force */
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, true);
				}

				/* ループ区間 */
				for(int i=1; i<countLoop; i++)	{
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, false);
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, true);
				}

				/* 末尾区間＆現在位置 */
				if(true == flagReverse)	{	/* Now-Reverse */
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, false);
				} else {	/* Now-Foward */
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, false);
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, true);
				}
			} else {	/* Normal */
				if(true == flagTurn)	{	/* Turn-Back */
					/* MEMO: No-Loop & Turn-Back ... Always "Reverse to Foward" */
					framePrevious = controlTrack.ArgumentContainer.FramePrevious - 1;	/* Force */
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, false);
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, true);
				} else {	/* Normal */
					if(true == flagReverse)	{	/* Reverse */
						UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
					} else {	/* Foward */
						UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, true);
					}
				}
			}
		} else {	/* Normal */
			if(true == flagLoop)	{
				/* 先頭部分 */
				if(true == flagReversePrevious)	{
					framePrevious = controlTrack.ArgumentContainer.FramePrevious - 1;	/* Force */
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, true);
				} else {
					framePrevious = controlTrack.ArgumentContainer.FramePrevious + 1;	/* Force */
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, false);
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, true);
				}

				/* ループ区間 */
				for(int i=1; i<countLoop; i++)	{
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, false);
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, true);
				}

				/* 末尾区間＆現在位置 */
				if(true == flagReverse)	{	/* Now-Reverse */
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, false);
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, true);
				} else {	/* Now-Foward */
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, false);
				}
			} else {	/* Normal */
				if(true == flagTurn)	{	/* Turn-Back */
					/* MEMO: No-Loop & Turn-Back ... Always "Foward to Revese" */
					framePrevious = controlTrack.ArgumentContainer.FramePrevious + 1;	/* Force */
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, false);
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, true);
				} else {	/* Normal */
					if(true == flagReverse)	{	/* Reverse */
						UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, true);
					} else {	/* Foward */
						UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
					}
				}
			}
		}
	} else {	/* Play-Style: OneWay */
		/* フレームスキップを考慮して区間を決定 */
		if(true == flagReverse)	{	/* Backwards */
			if(true == flagTurn)	{	/* Wrap-Around */
				/* 先頭部分 */
				UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameStart, frame, false);

				/* ループ区間 */
				for(int j=1; j<countLoop ; j++)	{
					UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frameStart, frame, false);
				}

				/* 末尾区間＆現在位置 */
				UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, frameEnd, frame, frame, false);
			} else {	/* Normal */
				UpdateSignalReverse(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
			}
		} else {	/* Foward */
			if(true == flagTurn)	{	/* Wrap-Around */
				/* 先頭部分 */
				UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frameEnd, frame, false);

				/* ループ区間 */
				for(int j=1; j<countLoop; j++)	{
					UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frameEnd, frame, false);
				}

				/* 末尾区間＆現在位置 */
				UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, frameStart, frame, frame, false);
			}
			else
			{	/* Normal */
				UpdateSignalFoward(entity, idParts, dataAnimationParts, controlTrack, framePrevious, frame, frame, false);
			}
		}
	}
}
void Parts::UpdateSignalFoward(	Entity& entity,
									int idParts,
									const Library::Data::Animation::Parts& dataAnimationParts,
									Control::Animation::Track& controlTrack,
									int frameRangeStart,
									int frameRangeEnd,
									int frameDecode,
									bool flagTurnBack
								)
{	/* 順方向にシグナルを走査 */
	const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
	int countData = dataProjectAnimation->ExpanderSignal->CountGetValue(dataAnimationParts.Signal);
	const char* nameParts = entity.NameGetParts(idParts);
	int frameKey = -1;
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;
	Library::Data::Animation::Attribute::Signal signal;
	for(int i=0; i<countData; i++)	{
		if(true == dataProjectAnimation->ExpanderSignal->ValueGetIndex(&signal, &frameKey, i, dataAnimationParts.Signal, controlTrack.ArgumentContainer))	{
			if((frameRangeStart <= frameKey) && (frameRangeEnd >= frameKey))	{	/* 区間内にシグナルが存在 */
				entity.CallBackSignal.Function(	entity,
												entity.CallBackUserData.Pointer,
												nameParts,
												idParts,
												indexAnimation,
												frameDecode,
												frameKey,
												signal,
												flagTurnBack
											);
			}
		}
	}
}
void Parts::UpdateSignalReverse(	Entity& entity,
									int idParts,
									const Library::Data::Animation::Parts& dataAnimationParts,
									Control::Animation::Track& controlTrack,
									int frameRangeEnd,
									int frameRangeStart,
									int frameDecode,
									bool flagTurnBack
								)
{	/* 逆方向にシグナルを走査 */
	const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
	int countData = dataProjectAnimation->ExpanderSignal->CountGetValue(dataAnimationParts.Signal);
	const char* nameParts = entity.NameGetParts(idParts);
	int frameKey = -1;
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;
	Library::Data::Animation::Attribute::Signal signal;
	for(int i=(countData-1); i>=0; i--)	{
		if(true == dataProjectAnimation->ExpanderSignal->ValueGetIndex(&signal, &frameKey, i, dataAnimationParts.Signal, controlTrack.ArgumentContainer))	{
			if((frameRangeStart <= frameKey) && (frameRangeEnd >= frameKey))	{	/* 区間内にシグナルが存在 */
				entity.CallBackSignal.Function(	entity,
												entity.CallBackUserData.Pointer,
												nameParts,
												idParts,
												indexAnimation,
												frameDecode,
												frameKey,
												signal,
												flagTurnBack
											);
			}
		}
	}
}

/* ********************************************************* */
//! 描画前描画用情報の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Parts::PreDraw(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
				)
{
	if(0 != (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NOT_USED))	{	/* 未使用 */
		return(false);
	}

	int indexTrack = IndexControlTrack;
	Library::Control::Animation::Track& controlTrack = entity.TableControlTrack[indexTrack];
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;

	if(0 <= indexAnimation)	{
		/* MEMO: マスクパーツしか処理しません。 */
		switch(entity.FeatureGetParts(idParts))	{
			case Library::Data::Parts::Animation::KindFeature::ROOT:
			case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
			case Library::Data::Parts::Animation::KindFeature::NORMAL:
			case Library::Data::Parts::Animation::KindFeature::INSTANCE:
			case Library::Data::Parts::Animation::KindFeature::EFFECT:
				break;

			case Library::Data::Parts::Animation::KindFeature::MASK:
				/* MEMO: マスクパーツは通常パーツと同じ描画計算を行います。 */
				/*       ※マスクパーツは機能自体は通常パーツと同じであり、 */
				/*         ピクセル描画をしないでステンシルに情報を書き込む */
				/*         ため、レンダリング設定とシェーダが異なるだけの通 */
				/*         常パーツとして（描画上は）扱っています。         */
				/* MEMO: マスクパーツの描画前描画と描画とはステンシルの扱いが */
				/*       異なる（ステンシルの操作方向が逆転する）だけで、同じ */
				/*       描画情報を使います。                                 */
				{
					Library::Data::Animation::Body dataAnimationBody;
					Library::Data::Animation::Parts dataAnimationParts;
					entity.DataAnimation->TableAnimation(&dataAnimationBody, indexAnimation);
					dataAnimationBody.TableParts(&dataAnimationParts, idParts);

					return(DrawNormal(	entity,
										idParts,
										argument,
										dataAnimationParts,
										controlTrack,
										true,	/* Pre-Draw */
										flagHideDefault,
										flagPlanarization,
										masking
								)
						);
				}
//				break;

			case Library::Data::Parts::Animation::KindFeature::JOINT:
			case Library::Data::Parts::Animation::KindFeature::BONE:
			case Library::Data::Parts::Animation::KindFeature::MOVENODE:
			case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
			case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
			case Library::Data::Parts::Animation::KindFeature::MESH:
			case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
			case Library::Data::Parts::Animation::KindFeature::CAMERA:
				break;

			default:	/* MEMO: Clangの警告除け */
				break;
		}
	}

	/* MEMO: ここまできたら描画しないパーツとする。 */
	return(false);
}

/* ********************************************************* */
//! 画用情報の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Parts::Draw(	Entity& entity,
					int idParts,
					ArgumentContainer& argument,
					bool flagHideDefault,
					bool flagPlanarization,
					Library::KindMasking::Kind masking
			)
{
	if(0 != (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NOT_USED))	{	/* 未使用 */
		return(false);
	}

	int indexTrack = IndexControlTrack;
	Library::Control::Animation::Track& controlTrack = entity.TableControlTrack[indexTrack];
	int indexAnimation = controlTrack.ArgumentContainer.IndexAnimation;

	Library::Data::Animation::Body dataAnimationBody;
	Library::Data::Animation::Parts dataAnimationParts;
	entity.DataAnimation->TableAnimation(&dataAnimationBody, indexAnimation);
	dataAnimationBody.TableParts(&dataAnimationParts, idParts);

	if(0 <= indexAnimation)	{
		switch(entity.FeatureGetParts(idParts))	{
			case Library::Data::Parts::Animation::KindFeature::ROOT:
			case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
				break;

			case Library::Data::Parts::Animation::KindFeature::NORMAL:
				return(DrawNormal(	entity,
									idParts,
									argument,
									dataAnimationParts,
									controlTrack,
									false,	/* Draw */
									flagHideDefault,
									flagPlanarization,
									masking
								)
					);

			case Library::Data::Parts::Animation::KindFeature::INSTANCE:
				return(DrawInstance(	entity,
										idParts,
										argument,
										dataAnimationParts,
										controlTrack,
										false,	/* Draw */
										flagHideDefault,
										flagPlanarization,
										masking
								)
					);

			case Library::Data::Parts::Animation::KindFeature::EFFECT:
				return(DrawEffect(	entity,
									idParts,
									argument,
									dataAnimationParts,
									controlTrack,
									false,	/* Draw */
									flagHideDefault,
									flagPlanarization,
									masking
								)
					);

			case Library::Data::Parts::Animation::KindFeature::MASK:
				/* MEMO: 子オブジェクトのマスクは無視されます（仕様）。 */
				if(nullptr == entity.EntityParent)	{	/* 自身が最親オブジェクト */
					/* MEMO: マスクパーツは「再描画」のため、DrawNormalを使用していないことに注意してください。 */
					return(DrawMask(	entity,
										idParts,
										argument,
										dataAnimationParts,
										controlTrack,
										false,	/* Draw */
										flagHideDefault,
										flagPlanarization,
										masking
								)
						);
				}
				break;

			case Library::Data::Parts::Animation::KindFeature::JOINT:
			case Library::Data::Parts::Animation::KindFeature::BONE:
			case Library::Data::Parts::Animation::KindFeature::MOVENODE:
			case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
			case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
				break;

			case Library::Data::Parts::Animation::KindFeature::MESH:
				return(DrawMesh(	entity,
									idParts,
									argument,
									dataAnimationParts,
									controlTrack,
									false,	/* Draw */
									flagHideDefault,
									flagPlanarization,
									masking
							)
					);

			case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
			case Library::Data::Parts::Animation::KindFeature::CAMERA:
				break;

			default:	/* MEMO: Clangの警告除け */
				break;
		}
	}

	/* MEMO: ここまできたら描画しないパーツとする。 */
	return(false);
}
bool Parts::DrawNormal(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					)
{
	if(0 > IndexControlDraw)	{	/* 描画ワークが設定されていない */
		return(false);
	}

	/* MEMO: この工程では本来的にはアトリビュートをデコードしないので、設定する */
	/*       必要はないのですが……インスタンスとエフェクトで再生情報を更新する */
	/*       処理（本来はUpdateの一部）が入るため、ここでも念のため設定しておき */
	/*       ます。                                                             */
	controlTrack.ArgumentContainer.IDParts = idParts;

	/* パーツそのものの非表示状態を確定 */
	bool flagHide = flagHideDefault;
	flagHide |= (0 != (Status & (FlagBitStatus::HIDE_FORCE | FlagBitStatus::HIDE)));	/* ? true : false */
	if(true == flagHide)	{	/* 非表示 */
		return(false);
	}

	/* 描画情報を更新 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];

	return(controlDraw->DrawNormal(	entity,
									idParts,
									*this,
									argument,
									dataAnimationParts,
									flagPreDraw,
									flagPlanarization,
									masking,
									Status,
									controlTrack
							)
		);
}
bool Parts::DrawInstance(	Entity& entity,
							int idParts,
							ArgumentContainer& argument,
							const Library::Data::Animation::Parts& dataAnimationParts,
							Library::Control::Animation::Track& controlTrack,
							bool flagPreDraw,
							bool flagHideDefault,
							bool flagPlanarization,
							Library::KindMasking::Kind masking
						)
{
	if(0 > IndexEntityUnderControl)	{	/* 実体がない */
		return(false);
	}
	Entity& entityUnderControl = entity.ListEntityChildInstance[IndexEntityUnderControl];
	if(0 == (entityUnderControl.Status & Entity::FlagBitStatus::VALID))	{	/* 子が初期化されていない */
		/* MEMO: 初期化してしまう？ */
		return(false);
	}

	/* インスタンス側の経過時間を取得 */
	Float32 timeElapsed = controlTrack.TimeElapsedNow;
	bool flagStopJumpTime = (0.0f != controlTrack.TimeElapseReplacement);	/* ? true : false */
	if(false == controlTrack.StatusGetIsPlaying())	{	/* 再生停止中 */
		if(true == flagStopJumpTime)	{	/* 終了時にフレームジャンプする */
			timeElapsed = controlTrack.TimeElapseInRangeReplacement;
		} else {	/* そのまま停止 */
			timeElapsed = 0.0f;
		}
	} else {
		if(true == controlTrack.StatusGetIsPausing())	{
			timeElapsed = 0.0f;
		}
	}

	bool flagDecode = true;
	flagDecode &= !(0 != (Status & FlagBitStatus::INSTANCE_IGNORE_ATTRIBUTE));	/* ? true : false */	/* Ignore Decode */
	flagDecode |= (0 != (Status & FlagBitStatus::INSTANCE_IGNORE_EXCEPT_NEXTDATA));	/* ? true : false */	/* Force Decode */
	flagDecode &= controlTrack.StatusGetIsDecodeAttribute();

	int frame = controlTrack.ArgumentContainer.Frame;
	bool flagPlayIndependentNowInstance = (0 != (Status & FlagBitStatus::INSTANCE_PLAY_INDEPENDENT));	/* ? true : false; */
	bool flagPlayReverse = controlTrack.StatusGetIsPlayingReverse();
	bool flagPlayTurn = controlTrack.StatusGetIsPlayingTurn();
	bool flagTopFrame = false;
	bool flagTimeWrap = false;
	Float32 timeOffset = 0.0f;

	/* 先頭フレームのチェック */
	/* Check top frame */
	if(true == flagPlayReverse)	{	/* 再生方向: 逆 */
		flagTopFrame = (controlTrack.TimeRange <= (controlTrack.TimeElapsed + controlTrack.TimePerFrameConsideredRateTime)) ? true : false;
	} else {	/* 再生方向: 前 */
		flagTopFrame = (0.0f > (controlTrack.TimeElapsed - controlTrack.TimePerFrameConsideredRateTime)) ? true : false;
	}

	/* 強制デコードのチェック */
	/* Check force-applying */
	if(true == flagPlayTurn)	{	/* 再生区間を折り返した（or ループした） */
		if(false == flagPlayIndependentNowInstance)	{	/* インスタンスのアニメーション再生が「独立」でない */
			FramePreviousUpdateUnderControl = -1;
		}
	}

	/* インスタンスアトリビュートを取得 */
	int frameKey = FramePreviousUpdateUnderControl;
	if((true == flagDecode) || (-1 == FramePreviousUpdateUnderControl))	{
		/* アトリビュートのデコード */
		Library::Data::Animation::Attribute::Instance dataInstance;
		const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
		if(true == dataProjectAnimation->ExpanderInstance->ValueGet(	&dataInstance, &frameKey,
																		dataAnimationParts.Instance,
																		controlTrack.ArgumentContainer
																)
		)	{	/* 更新 */
			if(0 == (Status & FlagBitStatus::INSTANCE_IGNORE_EXCEPT_NEXTDATA))	{	/* アトリビュートが有効 */
				DataInstance.Duplicate(dataInstance);
			} else {	/* 外部から無視要請 */
				FramePreviousUpdateUnderControl = -1;
			}
			Status &= ~FlagBitStatus::INSTANCE_IGNORE_EXCEPT_NEXTDATA;

			if(FramePreviousUpdateUnderControl != frameKey)	{	/* 異なるアトリビュートをデコード */
				bool flagPlayReverseInstanceData = (0.0f > DataInstance.RateTime);	/* ? true : false */
				bool flagPlayReverseInstance = flagPlayReverseInstanceData ^ flagPlayReverse;

				/* Set Animation */
				InstancePlayStart(	entity,
//									controlTrack.RateTime * ((true == flagPlayReverse) ? -1.0f : 1.0f)
									((true == flagPlayReverse) ? -1.0f : 1.0f)
								);

				/* Adjust Starting-Time */
				/* MEMO: Necessary to set time, not frame. Because parent's elapsed time has a small excess. */
				if(true == flagPlayReverse)	{	/* 逆再生 */
					flagTimeWrap = flagTopFrame & flagPlayReverseInstanceData;
					if(frameKey <= frame)	{	/* 即時再生 */
						/* MEMO: インスタンスの経過時間はフレーム範囲内。 */
						timeOffset = controlTrack.TimeElapsed - ((Float32)(frameKey - controlTrack.FrameStart) * controlTrack.TimePerFrame);
						timeOffset *= abs(DataInstance.RateTime);
						entityUnderControl.TableControlTrack[0].TimeSkip(timeOffset, flagPlayReverse, flagTimeWrap);
					} else {	/* 待機状態化 */
						if(true == flagPlayReverseInstance)	{	/* 逆再生 */
							entityUnderControl.TableControlTrack[0].TimeSkip(0.0f, flagPlayReverse, flagTimeWrap);
							entityUnderControl.TableControlTrack[0].TimeDelay = 0.0f;
							entityUnderControl.AnimationStop(-1, false);	/* ??? */
						} else {	/* 正常方向 */
							timeOffset = ((Float32)frameKey * controlTrack.TimePerFrame) - controlTrack.TimeElapsed;
							entityUnderControl.TableControlTrack[0].TimeSkip(0.0f, flagPlayReverse, flagTimeWrap);
							entityUnderControl.TableControlTrack[0].TimeDelay = timeOffset;
						}
					}
				} else {	/* 通常再生 */
					flagTimeWrap = flagTopFrame & flagPlayReverseInstanceData;
					if(frameKey <= frame)	{	/* 即時再生 */
						/* MEMO: インスタンスの経過時間はフレーム範囲内。 */
						timeOffset = controlTrack.TimeElapsed - ((Float32)(frameKey - controlTrack.FrameStart) * controlTrack.TimePerFrame);
						timeOffset *= abs(DataInstance.RateTime);
						entityUnderControl.TableControlTrack[0].TimeSkip(timeOffset, flagPlayReverse, flagTimeWrap);
					} else {	/* 待機状態化 */
						if(true == flagPlayReverseInstance)	{	/* 逆再生 */
							entityUnderControl.TableControlTrack[0].TimeSkip(0.0f, flagPlayReverse, flagTimeWrap);
							entityUnderControl.TableControlTrack[0].TimeDelay = 0.0f;
							entityUnderControl.AnimationStop(-1, false);	/* ??? */
						} else {	/* 正常方向 */
							timeOffset = ((Float32)frameKey * controlTrack.TimePerFrame) - controlTrack.TimeElapsed;
							entityUnderControl.TableControlTrack[0].TimeSkip(0.0f, flagPlayReverse, flagTimeWrap);
							entityUnderControl.TableControlTrack[0].TimeDelay = timeOffset;
						}
					}
				}

				/* Status Update */
				FramePreviousUpdateUnderControl = frameKey;
			}
		}
	}

	/* 子エンティティの更新 */
	/* MEMO: 子エンティティは非常時状態に限らずアニメーション状況は更新されなくてはならないので、 */
	/*       非表示フラグで無造作に打ち切ったりしないでください。                                 */
	bool flagHide = flagHideDefault;
	flagHide |= (0 != (Status & (FlagBitStatus::HIDE_FORCE | FlagBitStatus::HIDE)));	/* ? true : false */
	if(	(0 != (Status & Library::Control::Animation::Parts::FlagBitStatus::UPDATE_SCALELOCAL))
		|| (0 != (entity.Status & Entity::FlagBitStatus::UPDATE_RATE_SCALELOCAL))
	)	{
		entityUnderControl.RateSetScaleLocal(	entity.RateScaleLocalForce.GetX() * ScaleLocal.Value.GetX(),
												entity.RateScaleLocalForce.GetY() * ScaleLocal.Value.GetY()
										);
	}

	if(	(0 != (Status & Library::Control::Animation::Parts::FlagBitStatus::UPDATE_RATEOPACITY))
		|| (0 != (entity.Status & Entity::FlagBitStatus::UPDATE_RATE_OPACITY))
	)	{
		entityUnderControl.RateOpacityForce = entity.RateOpacityForce * RateOpacity.Value;
	}

	entityUnderControl.UpdateMain(	timeElapsed,
									flagHide,
									(0 != (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NOT_MASKING)) ? Library::KindMasking::THROUGH : Library::KindMasking::MASK,
									argument,
									false,
									flagPlanarization,
									MatrixTransform
							);

	Status &= ~(	FlagBitStatus::UPDATE_SCALELOCAL
					| FlagBitStatus::UPDATE_RATEOPACITY
			);

	return(true);
}
bool Parts::DrawEffect(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					)
{	/* MEMO: 本関数はSS5.6実装のRendererVer1には対応していません（SS5.7以降のRendererVer2のみ）。 */
	if(0 > IndexEntityUnderControl)	{	/* 実体がない */
		return(false);
	}
	EntityEffect& entityUnderControl = entity.ListEntityChildEffect[IndexEntityUnderControl];
	if(0 == (entityUnderControl.Status & Entity::FlagBitStatus::VALID))	{	/* 子が初期化されていない */
		/* MEMO: 初期化してしまう？ */
		return(false);
	}

	/* エフェクト側の経過時間を取得 */
	Float32 timeElapsed = controlTrack.TimeElapsedNow;
	bool flagStopJumpTime = (0.0f != controlTrack.TimeElapseReplacement);	/* ? true : false */
	if(false == controlTrack.StatusGetIsPlaying())	{	/* 再生停止中 */
		if(true == flagStopJumpTime)	{	/* 終了時にフレームジャンプする */
			timeElapsed = controlTrack.TimeElapseInRangeReplacement;
		} else {	/* そのまま停止 */
			timeElapsed = 0.0f;
		}
	} else {
		if(true == controlTrack.StatusGetIsPausing())	{	/* 一時停止中 */
			timeElapsed = 0.0f;
		}
	}

	int frame = controlTrack.ArgumentContainer.Frame;
	bool flagDecode = !(0 != (Status & FlagBitStatus::EFFECT_IGNORE_ATTRIBUTE));	/* (0 != (Status &= FlagBitStatus.EFFECT_IGNORE_ATTRIBUTE)) ? false : true; */
	flagDecode |= (0 != (Status & FlagBitStatus::EFFECT_IGNORE_EXCEPT_NEXTDATA));	/* ? true : false */
	flagDecode &= controlTrack.StatusGetIsDecodeAttribute();
	bool flagPlayIndependentNowInstance = (0 != (Status & FlagBitStatus::EFFECT_PLAY_INDEPENDENT));	/* ? true : false */
	bool flagPlayTurn = controlTrack.StatusGetIsPlayingTurn();
	bool flagPlayReverse = controlTrack.StatusGetIsPlayingReverse();
	float timeOffset = 0.0f;
	if(true == flagPlayTurn)	{	/* ループもしくは折り返している */
		if(false == flagPlayIndependentNowInstance)	{	/* 同期再生（独立再生でない） */
			FramePreviousUpdateUnderControl = -1;
		}
	}

	/* エフェクトアトリビュートを取得 */
	int frameKey = FramePreviousUpdateUnderControl;
	if(true == flagDecode)	{
		/* Decode data */
		Library::Data::Animation::Attribute::Effect dataEffect;
		const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);
		if(true == dataProjectAnimation->ExpanderEffect->ValueGet(	&dataEffect, &frameKey,
																	dataAnimationParts.Effect,
																	controlTrack.ArgumentContainer
															)
		)	{	/* 更新 */
			if(FramePreviousUpdateUnderControl != frameKey)	{	/* 新しいキーデータを発見 */
				/* 再生までの待機時間を設定 */
				if(frameKey <= frame)	{	/* 即時再生 */
					/* 再生開始 */
					/* MEMO: 子エフェクトのFPSは（エフェクトの持つFPSではなく）親アニメのFPSになるので注意（SS本体の仕様）。 */
					entityUnderControl.AnimationPlay(	1,
														((true == flagPlayReverse) ? -1.0f : 1.0f) * dataEffect.RateTime,
														controlTrack.FramePerSecond
													);

					/* 乱数種の設定 */
					entityUnderControl.SeedOffsetSet((Uint32)controlTrack.CountLoop);

					/* 再生時間の調整 */
					timeOffset = controlTrack.TimeElapsed - ((Float32)(frameKey - controlTrack.FrameStart) * controlTrack.TimePerFrame);
					timeOffset *= dataEffect.RateTime;	/* "Effect" does not have a negative speed. */
					timeOffset += entityUnderControl.TimeGetFramePosition(dataEffect.FrameStart);
					entityUnderControl.TimeSkip(timeOffset, false);

					/* ステータスを更新 */
					FramePreviousUpdateUnderControl = frameKey;
					Status = (0 != (dataEffect.Flags & Library::Data::Animation::Attribute::Effect::FlagBit::INDEPENDENT))
										? (Status | FlagBitStatus::EFFECT_PLAY_INDEPENDENT)
										: (Status & ~FlagBitStatus::EFFECT_PLAY_INDEPENDENT);
				}
			}
		}
	}

	/* エフェクトの更新 */
	/* MEMO: エフェクトの再生状態の更新はここで行われています（Entity::Updateのタイミングではありません）。 */
	bool flagHide = flagHideDefault;
	flagHide |= (0 != (Status & (FlagBitStatus::HIDE_FORCE | FlagBitStatus::HIDE)));	/* ? true : false */

	/* MEMO: 常に「ScaleLocal」と「RateOpacity」は更新します。 */
	CPU::Vector2 scaleLocal(entity.RateScaleLocalForce);
	VectorMul(&scaleLocal, scaleLocal, ScaleLocal.Value);
	entityUnderControl.RateSetScaleLocal(scaleLocal.GetX(), scaleLocal.GetY());

	entityUnderControl.RateSetOpacity(RateOpacity.Value * entity.RateOpacityForce);
	if(false == flagHide)	{
		/* MEMO: （Drawのタイミングだが）最適化などの都合でエフェクトのUpdateをここで呼び出していることに注意。 */
		if(false == entityUnderControl.UpdateMain(	timeElapsed,
													flagHide,
													(0 != (StatusAnimationParts & Library::Data::Animation::Parts::FlagBitStatus::NOT_MASKING)) ? Library::KindMasking::THROUGH : Library::KindMasking::MASK,
													argument,
													false,
													flagPlanarization,
													MatrixTransform
											)
		)	{	/* 失敗 */
			/* MEMO: 現在はエラー時の特殊処理はありません。 */
		}
	}

	Status &= ~(	FlagBitStatus::UPDATE_SCALELOCAL
					| FlagBitStatus::UPDATE_RATEOPACITY
			);

	return(true);
}
bool Parts::DrawMask(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					)
{
	if(0 > IndexControlDraw)	{	/* 描画ワークが設定されていない */
		return(false);
	}

	/* MEMO: この工程では本来的にはアトリビュートをデコードしないので、設定する */
	/*       必要はないのですが……インスタンスとエフェクトで再生情報を更新する */
	/*       処理（本来はUpdateの一部）が入るため、ここでも念のため設定しておき */
	/*       ます。                                                             */
	controlTrack.ArgumentContainer.IDParts = idParts;

	/* パーツそのものの非表示状態を確定 */
	bool flagHide = flagHideDefault;
	flagHide |= (0 != (Status & (FlagBitStatus::HIDE_FORCE | FlagBitStatus::HIDE)));	/* ? true : false */
	if(true == flagHide)	{	/* 非表示 */
		return(false);
	}

	/* 描画情報を更新 */
	/* MEMO: マスクの描画は描画設定以外は基本的に通常描画と同じ値を使用します。 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];

	return(controlDraw->DrawNormal(	entity,
									idParts,
									*this,
									argument,
									dataAnimationParts,
									flagPreDraw,
									flagPlanarization,
									masking,
									Status,
									controlTrack
							)
		);
}
bool Parts::DrawMesh(	Entity& entity,
						int idParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						Library::Control::Animation::Track& controlTrack,
						bool flagPreDraw,
						bool flagHideDefault,
						bool flagPlanarization,
						Library::KindMasking::Kind masking
					)
{
	if(0 > IndexControlDraw)	{	/* 描画ワークが設定されていない */
		return(false);
	}

	/* MEMO: この工程では本来的にはアトリビュートをデコードしないので、設定する */
	/*       必要はないのですが……インスタンスとエフェクトで再生情報を更新する */
	/*       処理（本来はUpdateの一部）が入るため、ここでも念のため設定しておき */
	/*       ます。                                                             */
	controlTrack.ArgumentContainer.IDParts = idParts;

	/* パーツそのものの非表示状態を確定 */
	bool flagHide = flagHideDefault;
	flagHide |= (0 != (Status & (FlagBitStatus::HIDE_FORCE | FlagBitStatus::HIDE)));	/* ? true : false */
	if(true == flagHide)	{	/* 非表示 */
		return(false);
	}

	/* 描画情報を更新 */
	Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];

	return(controlDraw->DrawMesh(	entity,
									idParts,
									*this,
									argument,
									dataAnimationParts,
									flagPreDraw,
									flagPlanarization,
									masking,
									Status,
									controlTrack
							)
		);
}

/* ********************************************************* */
//! 次に描画するパーツを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Parts::IDPartsNextDraw(void)

/* ********************************************************* */
//! 次に描画前描画するパーツを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Parts::IDPartsNextPreDraw(void)

/* ********************************************************* */
//! アニメーション変更時の定型処理
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Parts::AnimationChange(Entity& entity)
{
	Status |= (	FlagBitStatus::REFRESH_TRANSFORM_POSITION
				| FlagBitStatus::REFRESH_TRANSFORM_ROTATION
				| FlagBitStatus::REFRESH_TRANSFORM_SCALING
			);

	StatusAnimationParts = Library::Data::Animation::Parts::FlagBitStatus::CLEAR;

	bool flagClearCellApply = false;
	if(0 == (Status & FlagBitStatus::CHANGE_CELL_IGNORE_NEWANIMATION))	{
		Status &= ~(FlagBitStatus::CHANGE_CELL_IGNORE_ATTRIBUTE | FlagBitStatus::CHANGE_CELL_UNREFLECTED);
		flagClearCellApply = true;
	}

	if(0 == (Status & FlagBitStatus::INSTANCE_IGNORE_NEWANIMATION))	{
		Status &= ~(FlagBitStatus::INSTANCE_IGNORE_ATTRIBUTE | FlagBitStatus::INSTANCE_PLAY_INDEPENDENT);
	}

	if(0 == (Status & FlagBitStatus::EFFECT_IGNORE_NEWANIMATION))
	{
		Status &= ~(FlagBitStatus::EFFECT_IGNORE_ATTRIBUTE | FlagBitStatus::EFFECT_PLAY_INDEPENDENT);
	}

	TRSMaster.CleanUp();
	TRSSlave.CleanUp();

	CacheClearAttribute(entity, flagClearCellApply);
}

/* ********************************************************* */
//! アトリビュートのキャッシュ消去
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Parts::CacheClearAttribute(Entity& entity, bool flagClearCellApply)
{
	ScaleLocal.CleanUp();	ScaleLocal.Value.Set(CPU::Vector2::One);
	RateOpacity.CleanUp();	RateOpacity.Value = 1.0f;
	Priority.CleanUp();	Priority.Value = 0;

	FramePreviousUpdateUnderControl = -1;
#if 0	/* MEMO: 現在コライダ未対応 */
	FramePreviousUpdateRadiusCollision = -1;
#endif
	FrameKeyStatusAnimationFrame = -1;
	StatusAnimationFrame.Flags = Library::Data::Animation::Attribute::Status::FlagBit::INITIAL;

	if(0 <= IndexControlDraw)	{	/* 描画を持っている */
		/* 描画側のキャッシュを消去 */
		Library::Control::Animation::Draw* controlDraw = &entity.TableControlPartsDraw[IndexControlDraw];
		controlDraw->AnimationChange(flagClearCellApply);
	}
}

/* ********************************************************* */
//! インスタンスの再生開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Parts::InstancePlayStart(Entity& entity, float rateTime)
{
	if(0 > IndexEntityUnderControl)	{
		return(false);
	}

	if(0 != (DataInstance.Flags & Library::Data::Animation::Attribute::Instance::FlagBit::INDEPENDENT))	{
		Status |= FlagBitStatus::INSTANCE_PLAY_INDEPENDENT;
	} else {
		Status &= ~FlagBitStatus::INSTANCE_PLAY_INDEPENDENT;
	}

	int framePerSecond = 60;
	if(0 <= IndexControlTrack)	{
		framePerSecond = entity.TableControlTrack[IndexControlTrack].FramePerSecond;
	}

	/* MEMO: Playing target are all tracks. And TableInformationPlay[0] is always used. */
	return(entity.ListEntityChildInstance[IndexEntityUnderControl].AnimationPlay(	-1,	/* All track */
																					IndexAnimationUnderControl,
																					DataInstance.PlayCount,
																					0,
																					DataInstance.RateTime * rateTime,
																					((0 != (DataInstance.Flags & Library::Data::Animation::Attribute::Instance::FlagBit::PINGPONG)) ? Library::KindStylePlay::PINGPONG : Library::KindStylePlay::NORMAL),
																					DataInstance.LabelStart,
																					DataInstance.OffsetStart,
																					DataInstance.LabelEnd,
																					DataInstance.OffsetEnd,
																					framePerSecond
																				)
		);
}

}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                Animation::Parts::BufferTRS */
/* -------------------------------------------------------------------------- */
namespace Animation	{
// void Parts::BufferTRS::CleanUp(void)
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            Animation::Draw */
/* -------------------------------------------------------------------------- */
namespace Animation	{
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Draw::Draw(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Draw::~Draw(void)
{
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::CleanUp(void)
{
	Status = FlagBitStatus::CLEAR;

//	DataStatusPrevious
//	Masking

//	RateScaleMesh
//	RateScaleTexture

//	SizeSprite
//	PivotSprite
//	SizeTexture
//	SizeCell
//	PivotCell
//	PositionCell
//	DataCellApply
	IndexVertexCorrectionTable = 0;

//	DataCell
//	PartsColor
//	VertexCorrection
//	Deform
//	Shader
//	OffsetPivot
//	SizeForce
//	ScalingTexture
//	RotationTexture
//	PositionTexture

//	TableVertexColorPartsColor
//	TableRateAlphaPartsColor
//	TableCoordinateVertexCorrection

	CountVertex = 0;
	CountVertexIndex = 0;
//	IndexVertexDraw
//	ColorPartsDraw
//	UVTextureDraw
//	CoordinateDraw
//	CoordinateTransformedDraw
//	DeformDraw
//	ArgumentVs00Draw
//	FlagsDraw
//	ArgumentFs00Draw
//	ArgumentFs01Draw
//	ArgumentFs02Draw
//	ArgumentFs03Draw
//	ParameterFs00Draw
//	ParameterFs01Draw
//	ParameterFs02Draw
//	ParameterFs03Draw

	OperationPartsColor = Library::KindOperationBlend::NON;
	TableVertexColorPartsColor = nullptr;
	TableRateAlphaPartsColor = nullptr;

//	IndexVertexData
//	CoordinateVertexData
//	PartsColorVertexData
//	UV0VertexData
//	FlagsVertexData
//	ArgumentVs00VertexData
//	ArgumentFs00VertexData
//	ArgumentFs01VertexData
//	ArgumentFs02VertexData
//	ArgumentFs03VertexData
//	ParameterFs00VertexData
//	ParameterFs01VertexData
//	ParameterFs02VertexData
//	ParameterFs03VertexData

//	MatrixTexture

	AnimationChange(true);
}

/* ********************************************************* */
//! アニメーションが変更された時の定型処理
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::AnimationChange(bool flagClearDataCellApply)
{
#if 0	/* MEMO: 全てクリア（これはやってはいけない）。 */
	Status = FlagBitStatus.CLEAR;
#else	/* MEMO: 反転情報は消さないこと。 */
	Status &= (	FlagBitStatus::FLIP_COEFFICIENT_X
				| FlagBitStatus::FLIP_COEFFICIENT_Y
				| FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_X
				| FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_Y
			);
#endif

	DataStatusPrevious.CleanUp();

	Masking = (Library::KindMasking::Kind)(-1);

	RateScaleMesh.Set(CPU::Vector2::One);
	RateScaleTexture.Set(CPU::Vector2::One);

	SizeCell.Set(CPU::Vector2::Zero);
	PivotCell.Set(CPU::Vector2::Zero);
	SizeTexture.Set(SizeTextureDefault);
	SizeCell.Set(SizeTextureDefault);
	PivotCell.Set(CPU::Vector2::Zero);
	PositionCell.Set(CPU::Vector2::Zero);
	MatrixTexture.Set(FPU::Matrix4x4::Unit);
	if(true == flagClearDataCellApply)
	{
		DataCellApply.CleanUp();
	}
	IndexVertexCorrectionTable = 0;

	DataCell.CleanUp();	DataCell.Value.CleanUp();
	OffsetPivot.CleanUp();	OffsetPivot.Value.Set(CPU::Vector2::Zero);
	SizeForce.CleanUp();	SizeForce.Value.Set(-1.0f, -1.0f);
	ScalingTexture.CleanUp();	ScalingTexture.Value.Set(CPU::Vector2::One);
	PositionTexture.CleanUp();	PositionTexture.Value.Set(CPU::Vector2::Zero);
	RotationTexture.CleanUp();	RotationTexture.Value = 0.0f;
	PartsColor.CleanUp();	/* PartsColor.Value.CleanUp(); */
	VertexCorrection.CleanUp();	/* VertexCorrection.Value.CleanUp(); */
	Deform.CleanUp();	Deform.Value.CoordinateReset();
	Shader.CleanUp();	/* Shader.Value.CleanUp(); */
}

/* ********************************************************* */
//! 描画情報で使用するワークエリアのサイズを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
size_t Draw::SizeGetWorkArea(Entity& entity, int idParts, int countVertex, bool flagMesh)
{
	size_t sizeWorkArea = 0;

	/* 頂点数の特定 */
	/* MEMO: SS6.0では、使用しているセルの頂点数とメッシュバインドデータの頂点  */
	/*       数が異なる場合があります。                                         */
	/*       プレーヤではメッシュバインド情報を優先し・余剰情報は切り捨てます。 */
	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);
	Library::Data::Parts::Animation::BindMesh dataBindMesh;
	dataPartsAnimation.Mesh(&dataBindMesh);
	int countVertexIndex = 0;

	if(true == flagMesh)	{	/* メッシュの場合 */
		/* MEMO: 頂点インデックスは、メッシュパーツしかワークを持ちません（通常・マスクは定数配列）。      */
		/*       ※実はメッシュパーツの場合も持たないようにできます（Buffer::SingleGPUに持っているので）。 */
		countVertexIndex = dataBindMesh.CountTableIndexVertex();
		/* IndexVertexDraw */	sizeWorkArea += Memory::Alignment(sizeof(Uint16) * countVertexIndex);
	} else {	/* 通常パーツ・エフェクトの場合 */
		countVertexIndex = (Library::KindVertex::TERMINATOR4 == countVertex) ? TableCountVertexIndex_Triangle4 : TableCountVertexIndex_Triangle2;
	}

	/* 頂点データ計算用ワーク群の計上 */
	/* ColorPartsDraw */	sizeWorkArea += Memory::Alignment(sizeof(CPU::Color) * countVertex);
	/* UVTextureDraw */		sizeWorkArea += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);
	/* CoordinateDraw */	sizeWorkArea += Memory::Alignment(sizeof(FPU::Vector4) * countVertex);
	if((true == flagMesh) || (Library::Data::Parts::Animation::KindFeature::MASK == entity.FeatureGetParts(idParts)))	{
		/* MEMO: マスクパーツは2回描画するので、変形済みの頂点座標をキャッシュするためにワークを確保します。 */
		/* MEMO: メッシュパーツは頂点数が固定ではないため、コンパイル時バインディングできないため（ローカル変数で */
		/*       確保できない関係で）、変形済みの頂点座標の計s南尾ためにワークを確保します。                      */
		/* CoordinateTransformedDraw */	sizeWorkArea += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);
	} else {
		/* MEMO: メッシュ・マスク以外の描画パーツは、変形済みの頂点座標はキャッシュしません。 */
	}
	if(true == flagMesh)	{
		if(0 >= dataBindMesh.CountVertexDeform())	{	/* デフォームを使用していない */
			/* MEMO: 現在何も処理しない。 */
		} else {	/* デフォームを使用している */
			/* MEMO: 確保する領域は頂点数分なことに注意（デフォームしている頂点数ではない）。 */
			/* DeformDraw */	sizeWorkArea += Memory::Alignment(sizeof(FPU::Vector4) * countVertex);
		}
	} else {
		/* MEMO: 通常パーツにDeformはいらないので（セルメッシュでないとデフォームできない）、通常パーツ時はデフォーム用のワークは確保しません。 */
		/* DeformDraw */	sizeWorkArea += 0;
	}
	/* ArgumentVs00Draw */	sizeWorkArea += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);

	/* 頂点インデックステーブル */
	if(true == flagMesh)	{	/* メッシュの場合 */
		sizeWorkArea += Memory::Alignment(sizeof(Uint16) * countVertexIndex);
	} else {	/* 通常パーツ・エフェクトの場合 */
		/* MEMO: 通常の矩形描画の時は、GPU転送用のバッファに直接設定してしまうので専用領域はいらない。 */
	}

	return(sizeWorkArea);
}

/* ********************************************************* */
//! 矩形（通常・マスク）パーツ用描画情報の起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Draw::BootUp(int countVertex, bool flagMask, void*& workArea, bool flagIsPartsMask)
{	/* MEMO: 通常パーツ・マスクパーツの起動用関数です。 */
	CleanUp();

	CountVertex = countVertex;
//	CountVertexIndex =	/* 後から固定テーブルが割り当てられるのでここではヒープしない */

	/* 領域を初期化 */
	/* MEMO: 通常パーツにDeformはいらないので（セルメッシュでないとデフォームできない）、通常パーツ時はデフォーム用のワークは確保しません。 */
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(workArea);
//	indexVertexDraw =	/* 後から固定テーブルが割り当てられるのでここではヒープしない */
	ColorPartsDraw = reinterpret_cast<CPU::Color*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Color) * countVertex);
	UVTextureDraw = reinterpret_cast<CPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);
	CoordinateDraw = reinterpret_cast<FPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(FPU::Vector4) * countVertex);
	if(true == flagIsPartsMask)	{	/* マスクパーツ */
		/* MEMO: マスクパーツは2回描画するので、変形済みの頂点座標をキャッシュするためにワークを確保します。 */
		CoordinateTransformedDraw = reinterpret_cast<CPU::Vector3*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Vector3) * countVertex);
	} else {	/* マスクパーツ以外 */
		/* MEMO: マスクパーツ以外は描画は1回なので、変形済みの頂点座標はキャッシュしません。 */
		CoordinateTransformedDraw = nullptr;
	}
	DeformDraw = nullptr;
	ArgumentVs00Draw = reinterpret_cast<CPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);
	for(int i=0; i<countVertex; i++)	{
		new(&ColorPartsDraw[i]) CPU::Color;
		ColorPartsDraw[i].Set(TableColorModel[i]);

		new(&UVTextureDraw[i]) CPU::Vector4;
		UVTextureDraw[i].Set(TableUVMappingModel[i]);

		new(&CoordinateDraw[i]) FPU::Vector4;
		CoordinateDraw[i].Set(TableCoordinateModel[i]);

#if 0	/* MEMO: 本来はここで行うべきですが……ループ内でnullptrチェックするのもコストが高いので、別ループ（直下）に出しました。 */
		if(nullptr != CoordinateTransformedDraw)	{
			new(&CoordinateTransformedDraw[i]) CPU::Vector4;
//			CoordinateTransformedDraw[i].Set(TableCoordinateModel[i]);
		}
#endif

		new(&ArgumentVs00Draw[i]) CPU::Vector4;
		ArgumentVs00Draw[i].Set(TableVArgModel[i]);
	}
	if(nullptr != CoordinateTransformedDraw)	{
		for(int i=0; i<countVertex; i++)	{
			new(&CoordinateTransformedDraw[i]) CPU::Vector4;
//			CoordinateTransformedDraw[i].Set(TableCoordinateModel[i]);
		}
	}

	/* MEMO: これらは全頂点同じデータ(パーツで)違うが頂点では違わない）なので、 */
	/*       頂点データ転送時に一斉に複製します。                               */
//	FlagsDraw
//	ArgumentFs00Draw
//	ArgumentFs01Draw
//	ArgumentFs02Draw
//	ArgumentFs03Draw
//	ParameterFs00Draw
//	ParameterFs01Draw
//	ParameterFs02Draw
//	ParameterFs03Draw

	FlagsDraw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs00Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs01Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs02Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs03Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs00Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs01Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs02Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs03Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);

	/* 描画専用アトリビュートキャッシュの起動 */
	VertexCorrection.CleanUp();	VertexCorrection.Value.BootUp();
	Deform.CleanUp();	Deform.Value.CleanUp();	/* 未使用 */
	Shader.CleanUp();	Shader.Value.BootUp();

	/* 共通の初期設定 */
	if(false == BootUpCommon(countVertex, flagMask, workArea))	{	/* 失敗 */
		/* MEMO: 失敗してもワークを取得したままにしておく。 */
		/*       ※ShutDownしてしまった方が良いのかも……。 */
		return(false);
	}

	workArea = reinterpret_cast<void*>(pointWorkAreaNow);	/* 次のバッファ位置に更新 */

	return(true);
}

/* ********************************************************* */
//! 描画情報の起動（共用部）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Draw::BootUpCommon(int countPartsColorBuffer, bool flagMask, void*& workArea)
{
	PartsColor.Value.BootUp();
	TableVertexColorPartsColor = PartsColor.Value.VertexColor;
	TableRateAlphaPartsColor = PartsColor.Value.RateAlpha;

	Status |= (	FlagBitStatus::UPDATE_COORDINATE
				| FlagBitStatus::UPDATE_UVTEXTURE
				| FlagBitStatus::UPDATE_PARAMETERBLEND
				| FlagBitStatus::UPDATE_COLORPARTS
				| FlagBitStatus::UPDATE_MASKING
				| FlagBitStatus::UPDATE_DEFORM
				| FlagBitStatus::UPDATE_COORDINATE_NOWFRAME
			);

	return(true);
}

/* ********************************************************* */
//! メッシュパーツ用描画情報の起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Draw::BootUpMesh(Entity& entity, int idParts, bool flagMask, void*& workArea)
{	/* MEMO: メッシュパーツ用の起動関数です。 */
	CleanUp();

	/* MEMO: SS6.0では、使用しているセルの頂点数とメッシュバインドデータの頂点  */
	/*       数が異なる場合があります。                                         */
	/*       プレーヤではメッシュバインド情報を優先し・余剰情報は切り捨てます。 */
	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);
	Library::Data::Parts::Animation::BindMesh dataBindMesh;
	dataPartsAnimation.Mesh(&dataBindMesh);

	int countVertexIndex = dataBindMesh.CountTableIndexVertex();
	int countVertex = dataBindMesh.CountVertex();
//	int countTableBind = dataBindMesh.CountTableVertex();
//	int countTableUV = dataBindMesh.CountTableRateUV();
	int countVertexDeform = dataBindMesh.CountVertexDeform();

	CountVertex = countVertex;
	CountVertexIndex = countVertexIndex;

	/* 領域を初期化 */
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(workArea);
	IndexVertexDraw = reinterpret_cast<Uint16*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(Uint16) * countVertexIndex);
	ColorPartsDraw = reinterpret_cast<CPU::Color*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Color) * countVertex);
	UVTextureDraw = reinterpret_cast<CPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);
	CoordinateDraw = reinterpret_cast<FPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(FPU::Vector4) * countVertex);
	/* MEMO: メッシュパーツは頂点数が固定ではないため、コンパイル時バインディングできないため（ローカル変数で */
	/*       確保できない関係で）、変形済みの頂点座標の計s南尾ためにワークを確保します。                      */
	CoordinateTransformedDraw = reinterpret_cast<CPU::Vector3*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Vector3) * countVertex);

	if(0 >= countVertexDeform)	{	/* デフォーム未使用 */
		DeformDraw = nullptr;	/* unused */
	} else {	/* use Deform */
		DeformDraw = reinterpret_cast<FPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(FPU::Vector4) * countVertex);
		for(int i=0; i<countVertex; i++)	{
			new(&DeformDraw[i]) FPU::Vector4;
		}
	}
	ArgumentVs00Draw = reinterpret_cast<CPU::Vector4*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(CPU::Vector4) * countVertex);
	for(int i=0; i<countVertex; i++)	{
		new(&ColorPartsDraw[i]) CPU::Color;

		new(&UVTextureDraw[i]) CPU::Vector4;

		new(&CoordinateDraw[i]) FPU::Vector4;
		/* MEMO: CoordinateTransformedDrawは常に確保されるので、通常パーツのようにnullptr判定していません。 */
		new(&CoordinateTransformedDraw[i]) CPU::Vector4;

		new(&ArgumentVs00Draw[i]) CPU::Vector4(TableVArgModel[0]);	/* テーブルの先頭値で統一して設定 */
	}

	/* 頂点インデックスバッファを作成 */
	IndexVertexDraw = reinterpret_cast<Uint16*>(pointWorkAreaNow);	pointWorkAreaNow += Memory::Alignment(sizeof(Uint16) * CountVertexIndex);
	const Sint16* tableIndexVertex = dataBindMesh.TableIndexVertex();
	for(int i=0; i<CountVertexIndex; i++)	{
		IndexVertexDraw[i] = (Uint16)(tableIndexVertex[i]);
	}

	FlagsDraw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs00Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs01Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs02Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ArgumentFs03Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs00Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs01Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs02Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);
	ParameterFs03Draw.Set(0.0f, 0.0f, 0.0f, 0.0f);

	/* 描画専用アトリビュートキャッシュの起動 */
	VertexCorrection.CleanUp();	/* Disused */
	Deform.CleanUp();	Deform.Value.CleanUp();
	Shader.CleanUp();	Shader.Value.BootUp();

	if(nullptr != DeformDraw)	{	/* デフォーム使用 */
		Deform.Value.BootUp(countVertexDeform);
		Deform.Value.CoordinateReset();
	}

	if(false == BootUpCommon(0, flagMask, workArea))	{
		/* MEMO: 失敗してもワークを取得したままにしておく。 */
		/*       ※ShutDownしてしまった方が良いのかも……。 */
		return(false);
	}

	workArea = reinterpret_cast<void*>(pointWorkAreaNow);	/* 次のバッファ位置に更新 */

	return(true);
}

/* ********************************************************* */
//! インスタンスパーツ用描画情報の起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Draw::BootUpInstance(bool flagMask, void*& workArea)
{	/* MEMO: インスタンス専用の起動用関数です。 */
	CleanUp();

	CountVertex = 0;
	CountVertexIndex = 0;	/* インスタンスは描画情報を持たない */

	/* 領域を取得 */
	/* MEMO: インスタンスは描画計算をしないのでバッファを持ちません。 */
//	ColorPartsDraw = nullptr;
//	UVTextureDraw = nullptr;
//	CoordinateDraw = nullptr;
//	CoordinateTransformedDraw = nullptr;
//	DeformDraw = nullptr;
//	IndexVertexDraw = nullptr;

	/* 頂点バッファの起動 */
	/* MEMO: インスタンスは描画計算をしないのでバッファを起動しません。 */
//	CoordinateVertexData
//	PartsColorVertexData
//	UV0VertexData
	/* MEMO: 通常パーツの頂点バッファは共通で扱えるので、後からどこかに静的に設置する。 */
//	IndexVertexData

//	VArg = nullptr;
//	VArgVertexData

	/* 描画専用アトリビュートキャッシュの起動 */
	/* MEMO: インスタンスは頂点変形・デフォームも適用できないので、バッファを起動しません。 */
	VertexCorrection.CleanUp();	VertexCorrection.Value.CleanUp();	/* 未使用 */
	Deform.CleanUp();	Deform.Value.CleanUp();	/* 未使用 */
	Shader.CleanUp();	Shader.Value.CleanUp();	/* 未使用 */

	/* 共通の初期設定 */
	if(false == BootUpCommon(0, flagMask, workArea))	{	/* 失敗 */
		/* MEMO: 失敗してもワークを取得したままにしておく。 */
		/*       ※ShutDownしてしまった方が良いのかも……。 */
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! エフェクトパーツ用描画情報の起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Draw::BootUpEffect(bool flagMask, void*& workArea)
{	/* MEMO: エフェクト専用の起動用関数です。 */
	CleanUp();

	CountVertex = 0;
	CountVertexIndex = 0;	/* エフェクトは描画情報を持たない */

	/* 領域を取得 */
	/* MEMO: エフェクトは描画計算をしないのでバッファを持ちません。 */
//	ColorPartsDraw = nullptr;
//	UVTextureDraw = nullptr;
//	CoordinateDraw = nullptr;
//	CoordinateTransformedDraw = nullptr;
//	DeformDraw = nullptr;
//	IndexVertexDraw = nullptr;

	/* 頂点バッファの起動 */
	/* MEMO: エフェクトは描画計算をしないのでバッファを起動しません。 */
//	CoordinateVertexData
//	PartsColorVertexData
//	UV0VertexData
	/* MEMO: 通常パーツの頂点バッファは共通で扱えるので、後からどこかに静的に設置する。 */
//	IndexVertexData

//	VArg = nullptr;
//	VArgVertexData

	/* 描画専用アトリビュートキャッシュの起動 */
	/* MEMO: エフェクトは頂点変形・デフォームも適用できないので、バッファを起動しません。 */
	VertexCorrection.CleanUp();	VertexCorrection.Value.CleanUp();	/* 未使用 */
	Deform.CleanUp();	Deform.Value.CleanUp();	/* 未使用 */
	Shader.CleanUp();	Shader.Value.CleanUp();	/* 未使用 */

	/* 共通の初期設定 */
	if(false == BootUpCommon(0, flagMask, workArea))	{	/* 失敗 */
		/* MEMO: 失敗してもワークを取得したままにしておく。 */
		/*       ※ShutDownしてしまった方が良いのかも……。 */
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! 矩形パーツ用描画情報の終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::ShutDown(void)
{
	/* MEMO: アトリビュートキャッシュ関係（BufferAttribute）にはShutDownはありません。 */

	/* MEMO: パーツカラー関係は、ポインタになっていますが、アトリビュートキャッシュか */
	/*       外部データのバッファかのどちらかを指すため、ヒープメモリではないので解放 */
	/*       してはいけません。                                                       */
//	TableVertexColorPartsColor
//	TableRateAlphaPartsColor

	/* 頂点計算用ワークの解放 */
	/* MEMO: 明示的にデストラクタを走らせていないが……大丈夫か？ */

	/* MEMO: 自身のバッファの抹消は上位に任せています。 */

	/* 描画情報を消去 */
	CleanUp();
}

/* ********************************************************* */
//! メッシュパーツ用描画情報の終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::ShutDownMesh(void)
{
	/* MEMO: 現在は特殊な描画メモリなどがないので、通常パーツと同じ処理をしています。 */
	ShutDown();
}

/* ********************************************************* */
//! インスタンスパーツ用描画情報の終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::ShutDownInstance(void)
{
	/* MEMO: 現在は特殊な描画メモリなどがないので、通常パーツと同じ処理をしています。 */
	ShutDown();
}

/* ********************************************************* */
//! エフェクトパーツ用描画情報の終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::ShutDownEffect(void)
{
	/* MEMO: 現在は特殊な描画メモリなどがないので、通常パーツと同じ処理をしています。 */
	ShutDown();
}

/* ********************************************************* */
//! 反転ステータス群の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Draw::StatusSetFlip(Library::Data::Animation::Attribute::Status& status)
{
	/* 対象フラグ群の更新チェック */
	Library::Data::Animation::Attribute::Status statusNow = status;
	Library::Data::Animation::Attribute::Status statusUpdate;
	Uint32 /* Library::Data::Animation::Attribute::Status::FlagBit */ statusAdditionalFlip = (Status & (	FlagBitStatus::FLIP_COEFFICIENT_X
																											| FlagBitStatus::FLIP_COEFFICIENT_Y
																											| FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_X
																											| FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_Y
																										)
																								) << FlagShiftStatus::FLIP_COEFFICIENT_TO_ATTRIBUTE;
	statusNow.Flags ^= statusAdditionalFlip;
	if(false == DataStatusPrevious.IsValid())	{
		statusUpdate.Flags = Library::Data::Animation::Attribute::Status::FlagBit::HIDE
								| Library::Data::Animation::Attribute::Status::FlagBit::FLIP_X
								| Library::Data::Animation::Attribute::Status::FlagBit::FLIP_Y
								| Library::Data::Animation::Attribute::Status::FlagBit::FLIP_TEXTURE_X
								| Library::Data::Animation::Attribute::Status::FlagBit::FLIP_TEXTURE_Y;
	} else {
		/* MEMO: 変化したビットのみを抽出しています。 */
		statusUpdate.Flags = DataStatusPrevious.Flags ^ statusNow.Flags;
	}

	if(0 != (statusUpdate.Flags & (	Library::Data::Animation::Attribute::Status::FlagBit::FLIP_TEXTURE_X
									| Library::Data::Animation::Attribute::Status::FlagBit::FLIP_TEXTURE_Y
								)
			)
	)	{
		Status |= FlagBitStatus::UPDATE_TRANSFORM_TEXTURE;
	}
	if(0 != (statusUpdate.Flags & (	Library::Data::Animation::Attribute::Status::FlagBit::FLIP_X
									| Library::Data::Animation::Attribute::Status::FlagBit::FLIP_Y
								)
			)
	)	{
		Status |= FlagBitStatus::UPDATE_COORDINATE;
	}
	DataStatusPrevious.Flags = statusNow.Flags;

	/* スプライトの反転スケールと頂点処理順序の決定 */
	IndexVertexCorrectionTable = 0;
	if(true == statusNow.IsFlipX())	{
		RateScaleMesh.SetX(-1.0f);
		IndexVertexCorrectionTable += 1;
	} else {
		RateScaleMesh.SetX(1.0f);
	}
	if(true == statusNow.IsFlipY())	{
		RateScaleMesh.SetY(-1.0f);
		IndexVertexCorrectionTable += 2;
	} else {
		RateScaleMesh.SetY(1.0f);
	}

	/* マッピングの反転スケールの決定 */
	if(true == statusNow.IsTextureFlipX())	{
		RateScaleTexture.SetX(-1.0f);
	} else {
		RateScaleTexture.SetX(1.0f);
	}
	if(true == statusNow.IsTextureFlipY())	{
		RateScaleTexture.SetY(-1.0f);
	} else {
		RateScaleTexture.SetY(1.0f);
	}
}

/* ********************************************************* */
//! 描画情報の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Draw::Update(	Entity& entity,
					int idParts,
					Parts& controlParts,
					ArgumentContainer& argument,
					const Library::Data::Animation::Parts& dataAnimationParts,
					Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
					Control::Animation::Track& controlTrack
				)
{
	const Library::Data::Project::Animation* dataProjectAnimation = reinterpret_cast<const Library::Data::Project::Animation*>(controlTrack.ArgumentContainer.DataAnimation);

	Uint32 /* Library::Data::Animation::Parts::FlagBitStatus */ statusPartsAnimation = dataAnimationParts.StatusParts;
	bool flagUpdateValueAttribute;

	/* パーツ制御の更新チェック */
	if(	(0 != (statusControlParts & Library::Control::Animation::Parts::FlagBitStatus::UPDATE_SCALELOCAL))
		|| (0 != (entity.Status & Entity::FlagBitStatus::UPDATE_RATE_SCALELOCAL))
	)	{	/* ローカルスケールが更新 */
		Status |= FlagBitStatus::UPDATE_COORDINATE;
	}

	if(	(0 != (statusControlParts & Library::Control::Animation::Parts::FlagBitStatus::UPDATE_RATEOPACITY))
		|| (0 != (entity.Status & Entity::FlagBitStatus::UPDATE_RATE_OPACITY))
	)	{	/* 不透明度（ローカル不透明度・マスク強度）が更新 */
		Status |= FlagBitStatus::UPDATE_PARAMETERBLEND;
	}

	/* 使用しているセルデータからスプライト情報を作成 */
	/* MEMO: 常に「セル」アトリビュートをデコードしないと、動的なセル変更の後の復帰時に誤動作します。 */
	/* MEMO: 「UPDATE_COORDINATE」は描画用の累積ステータスです。                                     */
	/*       ここでは「UPDATE_COORDINATE_NOWFRAME」で判定する必要があります。                        */
	/*       ※「UPDATE_COORDINATE」のみで判定してしまうと、非描画のパーツで更新ミスしてしまいます。 */
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderCell->ValueGet(	&DataCell.Value, &DataCell.FrameKey,
																				dataAnimationParts.Cell,
																				controlTrack.ArgumentContainer
																	);
	if(0 == (statusControlParts & Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_UNREFLECTED))	{
		if(0 == (statusControlParts & Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_ATTRIBUTE))	{
			if(true == flagUpdateValueAttribute)	{	/* 更新 */
				DataCellApply = DataCell.Value;

				Status |= FlagBitStatus::UPDATE_UVTEXTURE;
				Status |= FlagBitStatus::UPDATE_COORDINATE;
				Status |= FlagBitStatus::UPDATE_TRANSFORM_TEXTURE;
				Status |= FlagBitStatus::UPDATE_COORDINATE_NOWFRAME;
			}
		}
	} else {	/* 動的セル変更 */
		if((0 > DataCellApply.IndexCellMap) || (0 > DataCellApply.IndexCell))
		{
			DataCellApply = DataCell.Value;
		}

		Status |= FlagBitStatus::UPDATE_COORDINATE;
		Status |= FlagBitStatus::UPDATE_UVTEXTURE;
		Status |= FlagBitStatus::UPDATE_COORDINATE_NOWFRAME;
	}
	statusControlParts &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_UNREFLECTED;

	int indexCellMap = DataCellApply.IndexCellMap;
	int indexCell = DataCellApply.IndexCell;
	if(0 > indexCellMap)	{
		Status |= FlagBitStatus::NO_DRAW;
	} else {
		Status &= ~FlagBitStatus::NO_DRAW;
	}

	Library::Data::CellMap::Body cellMap;
	Library::Data::CellMap::Cell cell;
	if(nullptr == entity.DataGetCellMap(&cellMap, indexCellMap))	{	/* セルマップがない */
		indexCellMap = -1;
		indexCell = -1;
	} else {	/* セルマップがある */
		if(nullptr == entity.DataGetCell(&cell, cellMap, indexCell))	{	/* セルがない */
			indexCellMap = -1;
			indexCell = -1;
		}
	}
	if(0 > indexCellMap)	{	/* セルがない */
		SizeTexture.Set(SizeTextureDefault);

		/* MEMO: セルがない場合はピボットは中心点になります。 */
		SizeCell = SizeTextureDefault;
		VectorMul(&PivotCell, SizeCell, 0.5f);
		PositionCell.Set(CPU::Vector2::Zero);
	} else {	/* セルがある */
		/* セルマップのピクセルサイズを取得 */
		cellMap.SizeOriginal(&SizeTexture);

		/* セル情報を取得 */
		CPU::Rect rectangleCell;
		cell.Rectangle(&rectangleCell);
		rectangleCell.SizeGet(&SizeCell);
		rectangleCell.PositionGetLU(&PositionCell);
		cell.Pivot(&PivotCell);
	}

	CPU::Vector2 sizeSprite = SizeCell;
	CPU::Vector2 pivotSprite = PivotCell;

	/* アトリビュートの値でスプライト情報の補正 */
	bool flagRecalcSizeSprite = (0 != (Status & FlagBitStatus::UPDATE_COORDINATE_NOWFRAME));	/* ? true : false */
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderOffsetPivot->ValueGet(	&OffsetPivot.Value, &OffsetPivot.FrameKey,
																					dataAnimationParts.OffsetPivot,
																					controlTrack.ArgumentContainer
																				);
	if(true == flagUpdateValueAttribute)	{
		Status |= FlagBitStatus::UPDATE_COORDINATE;
		flagRecalcSizeSprite |= true;
	}

	flagUpdateValueAttribute = dataProjectAnimation->ExpanderSizeForce->ValueGet(	&SizeForce.Value, &SizeForce.FrameKey,
																					dataAnimationParts.SizeForce,
																					controlTrack.ArgumentContainer
																				);
	if(true == flagUpdateValueAttribute)	{
		Status |= FlagBitStatus::UPDATE_COORDINATE;
		flagRecalcSizeSprite |= true;
	}

	/* スプライト情報の再計算 */
	if(true == flagRecalcSizeSprite)	{
		Float32 ratePivot;
		Float32 size;

		/* ピボット */
		pivotSprite.Set(	pivotSprite.GetX() + (sizeSprite.GetX() * OffsetPivot.Value.GetX()),
							pivotSprite.GetY() - (sizeSprite.GetY() * OffsetPivot.Value.GetY())
					);

		/* サイズ */
		size = SizeForce.Value.GetX();
		if(0.0f <= size)	{
			ratePivot = pivotSprite.GetX() / sizeSprite.GetX();
			sizeSprite.SetX(size);
			pivotSprite.SetX(size * ratePivot);
		}
		size = SizeForce.Value.GetY();
		if(0.0f <= size)	{
			ratePivot = pivotSprite.GetY() / sizeSprite.GetY();
			sizeSprite.SetY(size);
			pivotSprite.SetY(size * ratePivot);
		}
	}
	if(true == flagRecalcSizeSprite)	{	/* ピボットとサイズを再設定 */
		SizeSprite.Set(sizeSprite);
		PivotSprite.Set(pivotSprite);
		Status |= FlagBitStatus::UPDATE_COLLIDERRECTANGLE_NOWFRAME;
	}

	/* テクスチャの変形行列を計算 */
	if(0 == (statusPartsAnimation & Library::Data::Animation::Parts::FlagBitStatus::NO_TRANSFORMATION_TEXTURE))	{	/* テクスチャ変形のデータを持っている */
		/* テクスチャ変形のアトリビュート群を取得 */
		bool flagUpdateMatrixTexrure;
		flagUpdateMatrixTexrure = dataProjectAnimation->ExpanderPositionTexture->ValueGet(	&PositionTexture.Value, &PositionTexture.FrameKey,
																							dataAnimationParts.PositionTexture,
																							controlTrack.ArgumentContainer
																						);
		flagUpdateMatrixTexrure |= dataProjectAnimation->ExpanderScalingTexture->ValueGet(	&ScalingTexture.Value, &ScalingTexture.FrameKey,
																							dataAnimationParts.ScalingTexture,
																							controlTrack.ArgumentContainer
																						);
		flagUpdateMatrixTexrure |= dataProjectAnimation->ExpanderRotationTexture->ValueGet(	&RotationTexture.Value, &RotationTexture.FrameKey,
																							dataAnimationParts.RotationTexture,
																							controlTrack.ArgumentContainer
																						);
		if(true == flagUpdateMatrixTexrure)	{
			Status |= FlagBitStatus::UPDATE_TRANSFORM_TEXTURE;
		}
	}

	/* 適用パーツカラーの静動を決定＆パーツカラーを取得 */
	bool flagUseAdditionalColor = false;
	if(0 != (Status & FlagBitStatus::USE_ADDITIONALCOLOR))	{
		Status |= FlagBitStatus::USE_ADDITIONALCOLOR_PREVIOUS;
	} else {
		Status &= ~FlagBitStatus::USE_ADDITIONALCOLOR_PREVIOUS;
	}
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderPartsColor->ValueGet(	&PartsColor.Value, &PartsColor.FrameKey,
																					dataAnimationParts.PartsColor,
																					controlTrack.ArgumentContainer
																				);
	Library::Control::AdditionalColor* additionalColor = entity.AdditionalColor;
	if(nullptr != additionalColor)	{
		if(Library::KindOperationBlend::NON != additionalColor->Operation)	{
			if(0 != (additionalColor->Status & Library::Control::AdditionalColor::FlagBitStatus::CHANGE))	{	/* 更新されている */
				Status |= FlagBitStatus::UPDATE_COLORPARTS;
				Status |= FlagBitStatus::UPDATE_PARAMETERBLEND;
			}

			flagUseAdditionalColor = true;
		}
	}
	if(true == flagUseAdditionalColor)	{	/* 動的パーツカラー（外部から指定）を使用 */
		/* MEMO: flagUseAdditionalColorがtrueの場合は直上でadditionalColor!=nullptrであることが確定しているので、再チェックは省略しています。 */
		OperationPartsColor = additionalColor->Operation;
		TableVertexColorPartsColor = additionalColor->VertexColor;
		TableRateAlphaPartsColor = TableRateAlphaPartsColorDefault;

		Status |= FlagBitStatus::USE_ADDITIONALCOLOR;
	} else {	/* 静的データ（アニメーションデータでの指定）を使用 */
		if(true == flagUpdateValueAttribute)	{
			Status |= FlagBitStatus::UPDATE_COLORPARTS;
			Status |= FlagBitStatus::UPDATE_PARAMETERBLEND;
		} else {
			if(0 != (Status & FlagBitStatus::USE_ADDITIONALCOLOR_PREVIOUS))
			{
				Status |= FlagBitStatus::UPDATE_COLORPARTS;
				Status |= FlagBitStatus::UPDATE_PARAMETERBLEND;
			}
		}
		Status &= ~FlagBitStatus::USE_ADDITIONALCOLOR;

		OperationPartsColor = PartsColor.Value.Operation;
		TableVertexColorPartsColor = PartsColor.Value.VertexColor;
		TableRateAlphaPartsColor = PartsColor.Value.RateAlpha;
	}

	/* 頂点変形 */
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderVertexCorrection->ValueGet(	&VertexCorrection.Value, &VertexCorrection.FrameKey,
																							dataAnimationParts.VertexCorrection,
																							controlTrack.ArgumentContainer
																					);
	if(true == flagUpdateValueAttribute)	{
		Status |=  FlagBitStatus::UPDATE_COORDINATE;
	}

	/* デフォーム */
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderDeform->ValueGet(	&Deform.Value, &Deform.FrameKey,
																				dataAnimationParts.Deform,
																				controlTrack.ArgumentContainer
																			);
	if(true == flagUpdateValueAttribute)	{
		Status |= FlagBitStatus::UPDATE_DEFORM;
	}

	/* シェーダ */
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderShader->ValueGet(	&Shader.Value, &Shader.FrameKey,
																				dataAnimationParts.Shader,
																				controlTrack.ArgumentContainer
																			);

	/* シグナル */
	flagUpdateValueAttribute = dataProjectAnimation->ExpanderSignal->ValueGet(	&Signal.Value, &Signal.FrameKey,
																				dataAnimationParts.Signal,
																				controlTrack.ArgumentContainer
																			);

	/* 更新終了時にクリアするステータス */
	Status &= ~FlagBitStatus::UPDATE_COORDINATE_NOWFRAME;

	return(true);
}

/* ********************************************************* */
//! 矩形パーツの描画情報の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 一見して共通関数化できそうな部分（マスクやブレンドの特定とか）も現在ザラ書きしています。
 * ※純粋に関数呼び出しコストをケチった以外に、描画種別での特殊判定などが入る可能性があるので、
 *   下手に共通処理化すると後のメンテでハマる可能性が高いと思った次第です。
 */
bool Draw::DrawNormal(	Entity& entity,
						int idParts,
						Parts& controlParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						bool flagPreDraw,
						bool flagPlanarization,
						Library::KindMasking::Kind masking,
						Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
						Control::Animation::Track& controlTrack
					)
{
	if(0 != (Status & FlagBitStatus::NO_DRAW))	{	/* 非表示 */
		return(false);
	}

	PrimitiveBatcher::SettingRender settingRender;
//	settingRender.CleanUp();
	bool flagBreakBatching = false;
	bool flagTriangle4 = ((int)Library::KindVertex::TERMINATOR4 == CountVertex);	/* ? true : false */
	Uint32 /* Library::Data::Animation::Parts::FlagBitStatus */ statusPartsAnimation = dataAnimationParts.StatusParts;

	CPU::Vector2 sizeSprite = SizeSprite;
	CPU::Vector2 pivotSprite = PivotSprite;
	CPU::Vector2 sizeMapping = SizeCell;
	CPU::Vector2 positionMapping = PositionCell;

	/* マスキングのチェック */
	if(Library::KindMasking::FOLLOW_DATA == masking)	{	/* データに任せる */
		masking = (0 != (statusPartsAnimation & Library::Data::Animation::Parts::FlagBitStatus::NOT_MASKING)) ? Library::KindMasking::THROUGH : Library::KindMasking::MASK;
	}
	if(Masking != masking)	{	/* 変更 */
		Masking = masking;
		Status |= FlagBitStatus::UPDATE_MASKING;
	}

	/* ブレンド方法を設定 */
	/* MEMO: アニメーションの描画パーツのブレンド種別は、Renderer::SectionBlend::KindTypeと同値であることに */
	/*       依存した処理です（変換せずに直接キャストで済ませています）。注意してください。                 */
	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);
	Renderer::SectionBlend::KindType kindBlend = (Renderer::SectionBlend::KindType)(dataPartsAnimation.OperationBlendTarget());
	settingRender.BlendSet(kindBlend);

	/* 描画フラグを設定 */
	Uint32 /* Renderer::SectionBlend::FlagBitControlPixel */ flagControlPixel = Renderer::SectionBlend::FlagControlPixel[kindBlend];
	FlagsDraw.Set(	((0 != (flagControlPixel & Renderer::SectionBlend::FlagBitControlPixel::FLAG_PIXEL_DISCARD)) ? 1.0f : 0.0f),				/* 透明ピクセルを廃棄するか？ */
					((0 != (flagControlPixel & Renderer::SectionBlend::FlagBitControlPixel::FLAG_PIXEL_PREMULTIPLIED_ALPHA)) ? 1.0f : 0.0f),	/* PMAを行うか？ */
					0.0f,																														/* パーツカラーを頂点色とみなすか？ */
					0.0f																														/* 未使用 */
				);

	/* 使用するシェーダの設定 */
	SpriteStudio6::Shader* shader = &Shader::BuiltInStandard;
	bool flagIsPartsMask = (Library::Data::Parts::Animation::KindFeature::MASK == entity.FeatureGetParts(idParts));	/* ? true : false */

	if(true == flagIsPartsMask)	{	/* マスクパーツ */
		shader = &Shader::BuiltInStandardMask;
		FlagsDraw.SetX(1.0f);	/* discard（アルファテスト）は強制 */
	} else {	/* 描画パーツ */
		/* MEMO: 現在特別処理なし。 */
	}
	settingRender.ShaderSet(shader);

	/* ステンシルテストを設定 */
	/* MEMO: MaskingはDraw～の段階で完全に決定されているので、FOLLOW_DATAでくることはあり得ない。 */
	/* MEMO: SS6Player for Unityではステンシルの扱いはシェーダに含まれていたためマテリアルを切り替えていましたが、 */
	/*       C++版ではステンシルのステートを直接切り替えています。                                                 */
	{
		Renderer::SectionTest::KindTypeDraw kindTypeDraw;
		if(true == flagPreDraw)	{	/* マスク描画前描画 */
			/* MEMO: ここにはマスクパーツしかきません。 */
			kindTypeDraw = Renderer::SectionTest::KindTypeDraw::MASK_PREDRAW;
		} else {	/* 通常描画 */
			/* MEMO: 特殊適用がマスクだけなので、switch-caseではなくifで判定しています。                      */
			/*       ※（他の部分と異なる実装記述なので）本来はあまり推奨できた記述方法ではないのですが……。 */
			/*       ※そもそも非表示パーツ系はここまできません。                                             */
			if(true == flagIsPartsMask)	{	/* マスクパーツ */
				kindTypeDraw = Renderer::SectionTest::KindTypeDraw::MASK;
			} else {	/* 通常描画パーツ */
				kindTypeDraw = Renderer::SectionTest::KindTypeDraw::NORMAL;
			}
		}
		switch(Masking)	{
			case Library::KindMasking::Kind::MASK:
				/* MEMO: ステンシルテストあり。 */
				settingRender.TestSetStencil(true, kindTypeDraw);
				break;

			case Library::KindMasking::Kind::THROUGH:
			default:	/* ClangのWarning除け */
				/* MEMO: ステンシルテストなし。 */
				settingRender.TestSetStencil(false, kindTypeDraw);
				break;
		}
	}

	/* 深度テストを設定 */
	bool depthRead = false;
	bool depthWrite = false;
	settingRender.TestSetDepth(depthRead, depthWrite);

	/* UVを計算 */
	/* MEMO: ここでは四隅のみを計算して、中央は少し後ろで平均値で求めています。 */
	/* MEMO: UVの.zと.wにはカラーブレンドの追加パラメータ（合成種別・補佐パラメータ）が入るので、ここでは設定していません。 */
	CPU::Vector2 uv2C(CPU::Vector2::Zero);
	if(0 != (statusPartsAnimation & Library::Data::Animation::Parts::FlagBitStatus::NO_TRANSFORMATION_TEXTURE))	{	/* テクスチャ変形していない（通常の長方形） */
		CPU::Vector2 uLR(positionMapping.GetX(), positionMapping.GetX() + sizeMapping.GetX());
		Float32 tempFloat = SizeTexture.GetY() - positionMapping.GetY();	/* mapping-Y Inverse */
		CPU::Vector2 vUD(tempFloat, tempFloat - sizeMapping.GetY());
		/* MEMO: FPUに任せずにCPUでやっていますが、（この計算量なので）速度に過度の影響は与えないはずです。 */
		VectorMul(&uLR, uLR, (1.0f / SizeTexture.GetX()));
		VectorMul(&vUD, vUD, (1.0f / SizeTexture.GetY()));

		if(0.0f > RateScaleTexture.GetX())	{	/* Flip X */
			uLR.Set(uLR.GetY(), uLR.GetX());
		}
		if(0.0f > RateScaleTexture.GetY())	{	/* Flip Y */
			vUD.Set(vUD.GetY(), vUD.GetX());
		}

		UVTextureDraw[Library::KindVertex::LU].SetX(uLR.GetX());
		UVTextureDraw[Library::KindVertex::LU].SetY(vUD.GetX());
		uv2C.Set(	uv2C.GetX() + UVTextureDraw[Library::KindVertex::LU].GetX(),
					uv2C.GetY() + UVTextureDraw[Library::KindVertex::LU].GetY()
			);

		UVTextureDraw[Library::KindVertex::RU].SetX(uLR.GetY());
		UVTextureDraw[Library::KindVertex::RU].SetY(vUD.GetX());
		uv2C.Set(	uv2C.GetX() + UVTextureDraw[Library::KindVertex::RU].GetX(),
					uv2C.GetY() + UVTextureDraw[Library::KindVertex::RU].GetY()
			);

		UVTextureDraw[Library::KindVertex::RD].SetX(uLR.GetY());
		UVTextureDraw[Library::KindVertex::RD].SetY(vUD.GetY());
		uv2C.Set(	uv2C.GetX() + UVTextureDraw[Library::KindVertex::RD].GetX(),
					uv2C.GetY() + UVTextureDraw[Library::KindVertex::RD].GetY()
			);

		UVTextureDraw[Library::KindVertex::LD].SetX(uLR.GetX());
		UVTextureDraw[Library::KindVertex::LD].SetY(vUD.GetY());
		uv2C.Set(	uv2C.GetX() + UVTextureDraw[Library::KindVertex::LD].GetX(),
					uv2C.GetY() + UVTextureDraw[Library::KindVertex::LD].GetY()
			);

		if(true == flagTriangle4)	{	/* 中心点がある */
			UVTextureDraw[Library::KindVertex::C].SetX(uv2C.GetX() * 0.25f);
			UVTextureDraw[Library::KindVertex::C].SetY(uv2C.GetY() * 0.25f);
		}
	} else {	/* テクスチャ変形している */
		if(0 != (Status & FlagBitStatus::UPDATE_TRANSFORM_TEXTURE))	{	/* 変形行列を更新する必要あり */
			/* Create Matrix & Transform Texture-UV */
			CPU::Vector2 uLR;
			CPU::Vector2 vUD;

			CPU::Vector2 sizeMappingHalf;
			VectorMul(&sizeMappingHalf, sizeMapping, 0.5f);
			VectorDiv(&sizeMappingHalf, sizeMappingHalf, SizeTexture);

			CPU::Vector2 centerMapping;
			centerMapping.Set(	positionMapping.GetX() / SizeTexture.GetX() + sizeMappingHalf.GetX(),
								(SizeTexture.GetY() - positionMapping.GetY()) / SizeTexture.GetY() - sizeMappingHalf.GetY()
							);

			/* MEMO: FPUに任せずにCPUでやっていますが、（この計算量なので）速度に過度の影響は与えないはずです。 */
			uLR.Set(-sizeMappingHalf.GetX(), sizeMappingHalf.GetX());
			vUD.Set(sizeMappingHalf.GetY(), -sizeMappingHalf.GetY());

			if(0.0f > RateScaleTexture.GetX())	{	/* Flip X */
				uLR.Set(uLR.GetY(), uLR.GetX());
			}
			if(0.0f > RateScaleTexture.GetY())	{	/* Flip Y */
				vUD.Set(vUD.GetY(), vUD.GetX());
			}

			UVTextureDraw[Library::KindVertex::LU].SetX(uLR.GetX());
			UVTextureDraw[Library::KindVertex::LU].SetY(vUD.GetX());

			UVTextureDraw[Library::KindVertex::RU].SetX(uLR.GetY());
			UVTextureDraw[Library::KindVertex::RU].SetY(vUD.GetX());

			UVTextureDraw[Library::KindVertex::RD].SetX(uLR.GetY());
			UVTextureDraw[Library::KindVertex::RD].SetY(vUD.GetY());

			UVTextureDraw[Library::KindVertex::LD].SetX(uLR.GetX());
			UVTextureDraw[Library::KindVertex::LD].SetY(vUD.GetY());

			MatrixCreateTRS(	&MatrixTexture,
								FPU::Vector3(	PositionTexture.Value.GetX(),
												-PositionTexture.Value.GetY(),
												0.0f
											),
								FPU::Vector3(0.0f, 0.0f, -RotationTexture.Value),
								FPU::Vector3(	ScalingTexture.Value.GetX(),
												ScalingTexture.Value.GetY(),
												0.0f
											)
						);

			FPU::Vector4 tempVector4;
			Float32 tempX;
			Float32 tempY;
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
				VectorTransform(&tempVector4, UVTextureDraw[i], MatrixTexture);
				tempX = tempVector4.GetX() + centerMapping.GetX();
				tempY = tempVector4.GetY() + centerMapping.GetY();

				UVTextureDraw[i].SetX(tempX);
				UVTextureDraw[i].SetY(tempY);
				uv2C.Set((uv2C.GetX() + tempX), (uv2C.GetY() + tempY));
			}

			if(true == flagTriangle4)	{	/* 中心点がある */
				UVTextureDraw[Library::KindVertex::C].SetX(uv2C.GetX() * 0.25f);
				UVTextureDraw[Library::KindVertex::C].SetY(uv2C.GetY() * 0.25f);
			}
		}
	}

	/* パーツカラーの計算 */
	/* MEMO: とりあえず今はCPUで計算していますが、後にFPUに変えるかもしれません。 */
	/*       ※平均計算だけなので、速度面でそんなに深刻にはならないはず。         */
	const Float32 operationBlend = OperationPartsColor + 0.01f;	/* 0.01fは丸め誤差防止 */
	const Float32 rateOpacity = entity.RateOpacityForce * controlParts.RateOpacity.Value;
	Float32 fRate = 0.0f;
	Float32 fSrcRatio = 1.0f;
	Float32 fDstRatio = 0.0f;
//	Float32 fDstSrcRatio = (Library::KindOperationBlend::MUL == OperationPartsColor) ? 1.0f : 0.0f;	/* OperationPartsColor == 1 */
	Float32 fDstSrcRatio = (Library::KindOperationBlend::ADD == OperationPartsColor) ? 1.0f : 0.0f;	/* OperationPartsColor == 1 */
	Float32 fReserved = 0.0f;
	if(0 != (Status & (FlagBitStatus::UPDATE_COLORPARTS | FlagBitStatus::UPDATE_PARAMETERBLEND)))	{	/* 更新されている */
		static const int tableIndexVertexDefault[Library::KindVertex::TERMINATOR2] = {
			Library::KindVertex::LU,
			Library::KindVertex::RU,
			Library::KindVertex::RD,
			Library::KindVertex::LD,
		};
		const int* tableIndexVertex = tableIndexVertexDefault;
		int indexVertex;
		Float32 sumR = 0.0f;
		Float32 sumG = 0.0f;
		Float32 sumB = 0.0f;
		Float32 sumA = 0.0f;
		Float32 sumAlpha = 0.0f;
		float rateAlpha;
		for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
			indexVertex = tableIndexVertex[i];

			rateAlpha = TableRateAlphaPartsColor[indexVertex];
			UVTextureDraw[i].SetZ(operationBlend);
			UVTextureDraw[i].SetW(rateOpacity * rateAlpha);

			ColorPartsDraw[i].Set(TableVertexColorPartsColor[indexVertex]);
			sumR += ColorPartsDraw[i].GetR();
			sumG += ColorPartsDraw[i].GetG();
			sumB += ColorPartsDraw[i].GetB();
			sumA += ColorPartsDraw[i].GetA();
			sumAlpha += rateAlpha;

			/* 頂点シェーダ用パラメータの算出 */
			fRate = 1.0f - ColorPartsDraw[i].GetA();
			fSrcRatio = (Library::KindOperationBlend::MUL >= OperationPartsColor) ? (1.0f - fRate) : 1.0f;	/* OperationPartsColor <= 1 */
			fDstRatio = (Library::KindOperationBlend::SUB == OperationPartsColor) ? -fRate : fRate;	/* OperationPartsColor == 3 */
			ArgumentVs00Draw[i].Set(fSrcRatio, fDstRatio, fDstSrcRatio, fReserved);
		}

		if(true == flagTriangle4)	{	/* 中心点がある */
			/* UV・パーツカラー補佐 */
			UVTextureDraw[Library::KindVertex::C].SetZ(operationBlend);
			UVTextureDraw[Library::KindVertex::C].SetW(rateOpacity * (sumAlpha * 0.25f));

			/* パーツカラー */
			ColorPartsDraw[Library::KindVertex::C].Set(sumR * 0.25f, sumG * 0.25f, sumB * 0.25f, sumA * 0.25f);

			/* 頂点シェーダ用パラメータの算出 */
			fRate = 1.0f - ColorPartsDraw[Library::KindVertex::C].GetA();
			fSrcRatio = (Library::KindOperationBlend::MUL >= OperationPartsColor) ? (1.0f - fRate) : 1.0f;	/* OperationPartsColor <= 1 */
			fDstRatio = (Library::KindOperationBlend::SUB == OperationPartsColor) ? -fRate : fRate;	/* OperationPartsColor == 3 */
			ArgumentVs00Draw[Library::KindVertex::C].Set(fSrcRatio, fDstRatio, fDstSrcRatio, fReserved);
		}
	}

	ArgumentFs00Draw.Set(	SizeTexture.GetX(),			/* テクスチャの横ピクセル幅 */
							SizeTexture.GetY(),			/* テクスチャの縦ピクセル幅 */
							1.0f / SizeTexture.GetX(),	/* 1テクセルあたりのU値 */
							1.0f / SizeTexture.GetY()	/* 1テクセルあたりのV値 */
					);
	ArgumentFs02Draw.SetZ(0.0f);	/* プリマルチプライドアルファ */

	/* 頂点座標の初期値を設定 */
	/* MEMO: 中心点の座標は4辺の中点同士の交点です（対角線交点ではありません）。 */
	/* MEMO: 頂点の計算はFPUを使用しているので、ここでベクトルのCPU用からFPU用の変換をしています。 */
	if(0 != (Status & FlagBitStatus::UPDATE_COORDINATE))	{	/* 頂点の再計算 */
		/* Set Coordinates */
		Float32 scaleMeshX = RateScaleMesh.GetX();
		Float32 scaleMeshY = -RateScaleMesh.GetY();	/* * -1.0f ... Y-Axis Inverse */
		Float32 left = (-pivotSprite.GetX()) * scaleMeshX;
		Float32 right = (sizeSprite.GetX() - pivotSprite.GetX()) * scaleMeshX;
		Float32 top = (-pivotSprite.GetY()) * scaleMeshY;
		Float32 bottom = (sizeSprite.GetY() - pivotSprite.GetY()) * scaleMeshY;

		int indexVertex;
		const Library::KindVertex::Kind* tableIndex = TableIndexVertexCorrectionOrder[IndexVertexCorrectionTable];	/* 頂点格納順序 */
		CPU::Vector2* tableCoordinate = VertexCorrection.Value.Coordinate;

		indexVertex = tableIndex[Library::KindVertex::LU];
		CoordinateDraw[Library::KindVertex::LU].Set(	left + tableCoordinate[indexVertex].GetX(),
														top + tableCoordinate[indexVertex].GetY(),
														0.0f,
														1.0f
												);

		indexVertex = tableIndex[Library::KindVertex::RU];
		CoordinateDraw[Library::KindVertex::RU].Set(	right + tableCoordinate[indexVertex].GetX(),
														top + tableCoordinate[indexVertex].GetY(),
														0.0f,
														1.0f
												);

		indexVertex = tableIndex[Library::KindVertex::RD];
		CoordinateDraw[Library::KindVertex::RD].Set(	right + tableCoordinate[indexVertex].GetX(),
														bottom + tableCoordinate[indexVertex].GetY(),
														0.0f,
														1.0f
												);

		indexVertex = tableIndex[Library::KindVertex::LD];
		CoordinateDraw[Library::KindVertex::LD].Set(	left + tableCoordinate[indexVertex].GetX(),
														bottom + tableCoordinate[indexVertex].GetY(),
														0.0f,
														1.0f
												);

		/* 中央座標の算出 */
		if(true == flagTriangle4)	{	/* 中心点がある */
			/* MEMO: FPUで計算するためにVector4を使っていますが、.zと.wは最終的に無視されます。 */
			CPU::Vector2 coordinateLURU;
			CPU::Vector2 coordinateLULD;
			CPU::Vector2 coordinateLDRD;
			CPU::Vector2 coordinateRURD;
			FPU::Vector4 tempVector4;

			VectorAdd(&tempVector4, CoordinateDraw[Library::KindVertex::LU], CoordinateDraw[Library::KindVertex::RU]);	/* (LU + RU) / 2 */
			VectorMul(&tempVector4, tempVector4, 0.5f);
			coordinateLURU.Set(tempVector4.GetX(), tempVector4.GetY());

			VectorAdd(&tempVector4, CoordinateDraw[Library::KindVertex::LU], CoordinateDraw[Library::KindVertex::LD]);	/* (LU + LD) / 2 */
			VectorMul(&tempVector4, tempVector4, 0.5f);
			coordinateLULD.Set(tempVector4.GetX(), tempVector4.GetY());

			VectorAdd(&tempVector4, CoordinateDraw[Library::KindVertex::LD], CoordinateDraw[Library::KindVertex::RD]);	/* (LD + RD) / 2 */
			VectorMul(&tempVector4, tempVector4, 0.5f);
			coordinateLDRD.Set(tempVector4.GetX(), tempVector4.GetY());

			VectorAdd(&tempVector4, CoordinateDraw[Library::KindVertex::RU], CoordinateDraw[Library::KindVertex::RD]);	/* (RU + RD) / 2 */
			VectorMul(&tempVector4, tempVector4, 0.5f);
			coordinateRURD.Set(tempVector4.GetX(), tempVector4.GetY());

			/* MEMO: 入力がCPUで・出力がFPUといういびつな関数である点については、速度最適化の都合です。 */
			/* MEMO: 対角線交点を求める計算をしていますが、入力が対角ではないことに注意してください（4辺の中間点同士の線分交点です）。 */
			CoordinateGetDiagonalIntersection(	&CoordinateDraw[Library::KindVertex::C],
												coordinateLURU,
												coordinateRURD,
												coordinateLULD,
												coordinateLDRD
											);
		}
	}

	/* ローカルスケール係数を作成 */
	/* MEMO: 平面化フラグはローカルスケールのZ扱いで処理しています。                                      */
	/*       もっと複雑な計算に変わる（ローカル回転とかローカル座標シフトなどが追加されたような）場合は、 */
	/*       ローカル行列に混ぜて計算するのが良いのですが、現在は単純なスケール計算なので計算量からベクト */
	/*       ル乗算で行っています。                                                                       */
	/* MEMO: SS6Player for Unityの場合は、平面化は行列のZ軸部分を全部0.0に上書きしてやっています。 */
	FPU::Vector3 scaleVector3(	controlParts.ScaleLocal.Value.GetX() * entity.RateScaleLocalForce.GetX(),
								controlParts.ScaleLocal.Value.GetY() * entity.RateScaleLocalForce.GetY(),
								1.0f
							);
	if(true == flagPlanarization)	{	/* Z成分をつぶす */
		scaleVector3.SetZ(0.0f);
	}

	/* 頂点データを作成 */
	CPU::Vector3 tableCoordinateTransformed[Library::KindVertex::TERMINATOR4];
	CPU::Vector3* coordinateTransformed = tableCoordinateTransformed;
	CPU::Vector3* coordinateOut = coordinateTransformed;
	{	/* 頂点座標 */
		/* MEMO: 頂点座標を毎回計算しなおしているのは、Entityから与えられる変形行列と   */
		/*       ローカルスケール倍率は、外部から毎処理変更される可能性があるためです。 */
		/*       ※CoordinateDrawまではアニメーションデータによって変化する値のために、 */
		/*         データに変化がなければ計算がスキップされます。                       */
		/* MEMO: 頂点の計算処理順序はLibrary::KindVertexの並びに正しくある必要があります。 */
		FPU::Vector4 coordinate;
		FPU::Matrix4x4 matrixTransform;
		const FPU::Vector4 coordinateAdjust(0.0f, 0.0f, 0.0f, 0.0f);

		MatrixCreateTRS(&matrixTransform, FPU::Vector3::Zero, FPU::Vector3::Zero, scaleVector3);
		MatrixMul(&matrixTransform, matrixTransform, argument.MatrixCamera);
		MatrixMul(&matrixTransform, matrixTransform, controlParts.MatrixTransform);

		/* MEMO: CoordinateTransformedDrawは、マスクパーツの場合にのみ存在しています。 */
		/*       ※マスクの場合2回描画されるのでキャッシュしますが、通常パーツの場合は */
		/*         キャッシュする意味がないのでメモリ節約のために確保していません。    */
		if(nullptr != CoordinateTransformedDraw)	{
			coordinateTransformed = CoordinateTransformedDraw;
			coordinateOut = coordinateTransformed;
		}

		VectorTransform(&coordinate, CoordinateDraw[Library::KindVertex::LU], matrixTransform);
		VectorAdd(&coordinate, coordinate, coordinateAdjust);
		coordinateOut->Set(coordinate.GetX(), coordinate.GetY(), coordinate.GetZ());
		coordinateOut++;

		VectorTransform(&coordinate, CoordinateDraw[Library::KindVertex::RU], matrixTransform);
		VectorAdd(&coordinate, coordinate, coordinateAdjust);
		coordinateOut->Set(coordinate.GetX(), coordinate.GetY(), coordinate.GetZ());
		coordinateOut++;

		VectorTransform(&coordinate, CoordinateDraw[Library::KindVertex::RD], matrixTransform);
		VectorAdd(&coordinate, coordinate, coordinateAdjust);
		coordinateOut->Set(coordinate.GetX(), coordinate.GetY(), coordinate.GetZ());
		coordinateOut++;

		VectorTransform(&coordinate, CoordinateDraw[Library::KindVertex::LD], matrixTransform);
		VectorAdd(&coordinate, coordinate, coordinateAdjust);
		coordinateOut->Set(coordinate.GetX(), coordinate.GetY(), coordinate.GetZ());
		coordinateOut++;

		if(true == flagTriangle4)	{	/* 中心点がある */
			VectorTransform(&coordinate, CoordinateDraw[Library::KindVertex::C], matrixTransform);
			VectorAdd(&coordinate, coordinate, coordinateAdjust);
			coordinateOut->Set(coordinate.GetX(), coordinate.GetY(), coordinate.GetZ());
			coordinateOut++;
		}
	}

	/* レンダリングターゲットの設定 */
	int indexCellMap = DataCellApply.IndexCellMap;
	bool flagBindTexture = TextureSetTarget(&settingRender, &flagBreakBatching, entity, idParts, controlParts, argument, indexCellMap);

	/* セルマップテクスチャの設定 */
	TextureSetCellMap(&settingRender, entity, idParts, controlParts, argument, indexCellMap, flagBindTexture);

	/* バッチングメッシュに追加 */
	/* MEMO: CountVertexIndexとIndexVertexDrawが必要なくなるかと思われる。 */
	if(true == flagTriangle4)	{	/* 4分割三角 */
		if(0 >= entity.ModelDraw->PrimitiveAddQuadrilateral4(	settingRender,			/* レンダリング設定 */
																coordinateTransformed,	/* 描画頂点座標 */
																ColorPartsDraw,			/* パーツカラー */
																UVTextureDraw,			/* テクスチャUV */
																FlagsDraw,				/* 描画フラグ群 */
																ArgumentVs00Draw,		/* 頂点シェーダ用パラメータ */
																ArgumentFs00Draw,		/* ピクセルシェーダ用パラメータA0 */
																ArgumentFs01Draw,		/* ピクセルシェーダ用パラメータA1 */
																ArgumentFs02Draw,		/* ピクセルシェーダ用パラメータA2 */
																ArgumentFs03Draw,		/* ピクセルシェーダ用パラメータA3 */
																ParameterFs00Draw,		/* ピクセルシェーダ用パラメータB0 */
																ParameterFs01Draw,		/* ピクセルシェーダ用パラメータB1 */
																ParameterFs02Draw,		/* ピクセルシェーダ用パラメータB2 */
																ParameterFs03Draw,		/* ピクセルシェーダ用パラメータB3 */
																flagBreakBatching		/* バッチング阻害指定 */
															)
		)	{	/* 失敗 */
			return(false);
		}
	} else {	/* 2分割三角 */
		/* MEMO: ここ使われない可能性が高い。 */
		if(0 >= entity.ModelDraw->PrimitiveAddQuadrilateral2(	settingRender,			/* レンダリング設定 */
																coordinateTransformed,	/* 描画頂点座標 */
																ColorPartsDraw,			/* パーツカラー */
																UVTextureDraw,			/* テクスチャUV */
																FlagsDraw,				/* 描画フラグ群 */
																ArgumentVs00Draw,		/* 頂点シェーダ用パラメータ */
																ArgumentFs00Draw,		/* ピクセルシェーダ用パラメータA0 */
																ArgumentFs01Draw,		/* ピクセルシェーダ用パラメータA1 */
																ArgumentFs02Draw,		/* ピクセルシェーダ用パラメータA2 */
																ArgumentFs03Draw,		/* ピクセルシェーダ用パラメータA3 */
																ParameterFs00Draw,		/* ピクセルシェーダ用パラメータB0 */
																ParameterFs01Draw,		/* ピクセルシェーダ用パラメータB1 */
																ParameterFs02Draw,		/* ピクセルシェーダ用パラメータB2 */
																ParameterFs03Draw,		/* ピクセルシェーダ用パラメータB3 */
																flagBreakBatching		/* バッチング阻害指定 */
															)
		)	{	/* 失敗 */
			return(false);
		}
	}

	/* ステータスの更新（フレーム内更新情報のクリア） */
	Status &= ~(	FlagBitStatus::UPDATE_COORDINATE
					| FlagBitStatus::UPDATE_UVTEXTURE
					| FlagBitStatus::UPDATE_PARAMETERBLEND
					| FlagBitStatus::UPDATE_COLORPARTS
					| FlagBitStatus::UPDATE_MASKING
					| FlagBitStatus::UPDATE_DEFORM
					| FlagBitStatus::UPDATE_TRANSFORM_TEXTURE
//					| FlagBitStatus::USE_ADDITIONALCOLOR	/* ここではパーツカラーの更新フラグは落とさない */
			);
	statusControlParts &= ~(	Library::Control::Animation::Parts::FlagBitStatus::UPDATE_SCALELOCAL
								| Library::Control::Animation::Parts::FlagBitStatus::UPDATE_RATEOPACITY
						);

	return(true);
}

/* ********************************************************* */
//! マスクパーツの描画情報の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 一見して共通関数化できそうな部分（マスクやブレンドの特定とか）も現在ザラ書きしています。
 * ※純粋に関数呼び出しコストをケチった以外に、描画種別での特殊判定などが入る可能性があるので、
 *   下手に共通処理化すると後のメンテでハマる可能性が高いと思った次第です。
 */
bool Draw::DrawMask(	Entity& entity,
						int idParts,
						Parts& controlParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						bool flagPreDraw,
						bool flagPlanarization,
						Library::KindMasking::Kind masking,
						Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
						Control::Animation::Track& controlTrack
					)
{
	if(0 != (Status & FlagBitStatus::NO_DRAW))	{	/* 非表示 */
		return(false);
	}

	PrimitiveBatcher::SettingRender settingRender;
//	settingRender.CleanUp();
	bool flagBreakBatching = false;

	/* マスキングのチェック */
	/* MEMO: PreDraw時のDrawNormalでMaskingとUPDATE_MASKINGステータスビットは解決しているので、ここでやることはない。 */

	/* ブレンド方法を設定 */
	/* MEMO: 一応ブレンド方法を取得してはいるが、マスクパーツはMIXで固定です。 */
	/* MEMO: アニメーションの描画パーツのブレンド種別は、Renderer::SectionBlend::KindTypeと同値であることに */
	/*       依存した処理です（変換せずに直接キャストで済ませています）。注意してください。                 */
	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);
	Renderer::SectionBlend::KindType kindBlend = (Renderer::SectionBlend::KindType)(dataPartsAnimation.OperationBlendTarget());
	settingRender.BlendSet(kindBlend);


	/* 描画フラグを設定 */
	/* MEMO: FlagsDrawは、PreDrawのDrawMaskで設定済なので、ここでは行わない。 */

	/* 使用するシェーダの設定 */
	/* MEMO: 本関数が呼ばれるのはマスクパーツの本描画だけなので、シェーダは固定になります。 */
	SpriteStudio6::Shader* shader = &Shader::BuiltInStandardMask;
	settingRender.ShaderSet(shader);

	/* ステンシルテストを設定 */
	Renderer::SectionTest::KindTypeDraw kindTypeDraw;
	kindTypeDraw = Renderer::SectionTest::KindTypeDraw::MASK;
	switch(Masking)	{
		case Library::KindMasking::Kind::MASK:
			/* MEMO: ステンシルテストあり。 */
			settingRender.TestSetStencil(true, kindTypeDraw);
			break;

		case Library::KindMasking::Kind::THROUGH:
		default:	/* ClangのWarning除け */
			/* MEMO: ステンシルテストなし。 */
			settingRender.TestSetStencil(false, kindTypeDraw);
			break;
	}

	/* デプステストを設定 */
	bool depthRead = false;
	bool depthWrite = false;
	settingRender.TestSetDepth(depthRead, depthWrite);

	/* MEMO: 頂点データは全てPreDrawで算出した値をそのまま使用する。 */

	/* レンダリングターゲットの設定 */
	int indexCellMap = DataCellApply.IndexCellMap;
	bool flagBindTexture = TextureSetTarget(&settingRender, &flagBreakBatching, entity, idParts, controlParts, argument, indexCellMap);

	/* セルマップテクスチャの設定 */
	TextureSetCellMap(&settingRender, entity, idParts, controlParts, argument, indexCellMap, flagBindTexture);

	/* バッチングメッシュに追加 */
	/* MEMO: 4三角形分割しか存在しないのが判っている＆頂点データは計算が済んでいる（かつマスクパーツは */
	/*       必ずCoordinateTransformedDrawにキャッシュされている）ので細かなチェックや分岐は行わずに、 */
	/*       そのまま描画しています。                                                                  */
	if(0 >= entity.ModelDraw->PrimitiveAddQuadrilateral4(	settingRender,				/* レンダリング設定 */
															CoordinateTransformedDraw,	/* 描画頂点座標 */
															ColorPartsDraw,				/* パーツカラー */
															UVTextureDraw,				/* テクスチャUV */
															FlagsDraw,					/* 描画フラグ群 */
															ArgumentVs00Draw,			/* 頂点シェーダ用パラメータ */
															ArgumentFs00Draw,			/* ピクセルシェーダ用パラメータA0 */
															ArgumentFs01Draw,			/* ピクセルシェーダ用パラメータA1 */
															ArgumentFs02Draw,			/* ピクセルシェーダ用パラメータA2 */
															ArgumentFs03Draw,			/* ピクセルシェーダ用パラメータA3 */
															ParameterFs00Draw,			/* ピクセルシェーダ用パラメータB0 */
															ParameterFs01Draw,			/* ピクセルシェーダ用パラメータB1 */
															ParameterFs02Draw,			/* ピクセルシェーダ用パラメータB2 */
															ParameterFs03Draw,			/* ピクセルシェーダ用パラメータB3 */
															flagBreakBatching			/* バッチング阻害指定 */
														)
	)	{	/* 失敗 */
		return(false);
	}

	/* ステータスの更新（フレーム内更新情報のクリア） */
	/* MEMO: ステータスはPreDrawでクリアされているので、ここでは何も行わない。 */

	return(true);
}

/* ********************************************************* */
//! メッシュパーツの描画情報の更新
/* --------------------------------------------------------- */
/* MEMO:
 * 一見して共通関数化できそうな部分（マスクやブレンドの特定とか）も現在ザラ書きしています。
 * ※テクスチャ設定（ターゲット・セルマップ）については、処理が大きいこともあるのと、パーツ
 *   種別毎での特殊処理が入りづらいため、共通関数化されています。
 * ※純粋に関数呼び出しコストをケチった以外に、描画種別での特殊判定などが入る可能性があるので、
 *   下手に共通処理化すると後のメンテでハマる可能性が高いと思った次第です。
 */
bool Draw::DrawMesh(	Entity& entity,
						int idParts,
						Parts& controlParts,
						ArgumentContainer& argument,
						const Library::Data::Animation::Parts& dataAnimationParts,
						bool flagPreDraw,
						bool flagPlanarization,
						Library::KindMasking::Kind masking,
						Uint32& /* Library::Control::Animation::Parts::FlagBitStatus& */ statusControlParts,
						Control::Animation::Track& controlTrack
				)
{
	if(0 != (Status & FlagBitStatus::NO_DRAW))	{	/* 非表示 */
		return(false);
	}

	Library::Data::Parts::Animation dataPartsAnimation;
	entity.DataAnimation->TableParts(&dataPartsAnimation, idParts);

	PrimitiveBatcher::SettingRender settingRender;
//	settingRender.CleanUp();
	bool flagBreakBatching = false;
	Uint32 /* Library::Data::Animation::Parts::FlagBitStatus */ statusPartsAnimation = dataAnimationParts.StatusParts;

	/* ブレンド方法を設定 */
	Renderer::SectionBlend::KindType kindBlend = (Renderer::SectionBlend::KindType)(dataPartsAnimation.OperationBlendTarget());
	settingRender.BlendSet(kindBlend);

	/* 描画フラグを設定 */
	Uint32 /* Renderer::SectionBlend::FlagBitControlPixel */ flagControlPixel = Renderer::SectionBlend::FlagControlPixel[kindBlend];
	FlagsDraw.Set(	((0 != (flagControlPixel & Renderer::SectionBlend::FlagBitControlPixel::FLAG_PIXEL_DISCARD)) ? 1.0f : 0.0f),				/* 透明ピクセルを廃棄するか？ */
					((0 != (flagControlPixel & Renderer::SectionBlend::FlagBitControlPixel::FLAG_PIXEL_PREMULTIPLIED_ALPHA)) ? 1.0f : 0.0f),	/* PMAを行うか？ */
					0.0f,																														/* パーツカラーを頂点色とみなすか？ */
					0.0f																														/* 未使用 */
				);

	/* バインドメッシュ情報を取得 */
	Library::Data::Parts::Animation::BindMesh dataBindMesh;
	dataPartsAnimation.Mesh(&dataBindMesh);
	int countTableVertex = dataBindMesh.CountTableVertex();

	/* MEMO: これら（countTableBindMesh・countTableUV・countVertexDeform）の値は、0の場合を除いて同じ値になります。 */
	/*       ※さらに通常であれば、この3値はCountVertexと同値になります。                                           */
	int countTableBindMesh = countTableVertex;
	int countTableUV = dataBindMesh.CountTableRateUV();
	int countVertexDeform = dataBindMesh.CountVertexDeform();
	/* MEMO: SS6Player for Unityでは、下記のいずれかの条件でデフォームを使用しているかどうかを判定 */
	/*       していました。                                                                        */
	/*       - Deform（デフォームのアトリビュートキャッシュ）が有効か？                            */
	/*       - DeformDrawのバッファが存在しているか？                                              */
	/*                                                                                             */
	/*       最終的な判定結果としては同じですが、判断基準を統一してある方が判りやすいので、C++版で */
	/*       は統一してあります。                                                                  */
	bool flagUseDeform = (nullptr != DeformDraw);	/* ? true : false */

	/* マスキングのチェック */
	if(Library::KindMasking::FOLLOW_DATA == masking)	{	/* データに任せる */
		masking = (0 != (statusPartsAnimation & Library::Data::Animation::Parts::FlagBitStatus::NOT_MASKING)) ? Library::KindMasking::THROUGH : Library::KindMasking::MASK;
	}
	if(Masking != masking)	{	/* 変更 */
		Masking = masking;
		Status |= FlagBitStatus::UPDATE_MASKING;
	}

	/* 使用するシェーダの設定 */
	/* MEMO: 現仕様では、メッシュはマスキングには適用できないので、そのままビルトインシェーダを使用しています。 */
	SpriteStudio6::Shader* shader = &Shader::BuiltInStandard;
	settingRender.ShaderSet(shader);

	/* 適用するセルを特定 */
	int indexCellMap = DataCellApply.IndexCellMap;
	int indexCell = DataCellApply.IndexCell;
	Library::Data::CellMap::Body cellMap;
	Library::Data::CellMap::Cell cell;
	Library::Data::CellMap::Cell::DataMesh dataMeshCell;
	if(nullptr == entity.DataGetCellMap(&cellMap, indexCellMap))	{	/* セルマップがない */
		/* MEMO: 描画できない。 */
		return(false);
	} else {	/* セルマップがある */
		if(nullptr == entity.DataGetCell(&cell, cellMap, indexCell))	{	/* セルがない */
			/* MEMO: 描画できない。 */
			return(false);
		}
	}
	if(false == cell.IsMesh())	{	/* メッシュセルではない */
		/* MEMO: 描画できない。                                                    */
		/*       ※メッシュに通常のセルを割り当てることは（SS6上で）できないので、 */
		/*         通常メッシュで描画することもありません。                        */
		return(false);
	}

	/* ステンシルテストを設定 */
	/* MEMO: MaskingはDraw～の段階で完全に決定されているので、FOLLOW_DATAでくることはあり得ない。 */
	/* MEMO: SS6Player for Unityではステンシルの扱いはシェーダに含まれていたためマテリアルを切り替えていましたが、 */
	/*       C++版ではステンシルのステートを直接切り替えています。                                                 */
	/* MEMO: メッシュはマスクパーツになることはできないので、Pre-DrawKindTypeDraw::MASKの判定はしていません。 */
	switch(Masking)	{
		case Library::KindMasking::Kind::MASK:
			/* MEMO: ステンシルテストあり。 */
			settingRender.TestSetStencil(true, Renderer::SectionTest::KindTypeDraw::NORMAL);
			break;

		case Library::KindMasking::Kind::THROUGH:
		default:	/* ClangのWarning除け */
			/* MEMO: ステンシルテストなし。 */
			settingRender.TestSetStencil(false, Renderer::SectionTest::KindTypeDraw::NORMAL);
			break;
	}

	/* 深度テストを設定 */
	bool depthRead = false;
	bool depthWrite = false;
	settingRender.TestSetDepth(depthRead, depthWrite);

	/* UV値の確定 */
	/* MEMO: UVマップはセルデータに結びついているため、UVの再計算はセルの変更・初期設定時のみに発生します。 */
	/*       また、UVマップの変更によってメッシュバインドの頂点情報に変更はありません（メッシュバインド情報 */
	/*       は、セルではなくパーツにバインド情報を持っているため）。                                       */
	if(0 != (Status & FlagBitStatus::UPDATE_UVTEXTURE))	{	/* 再計算が必要 */
		/* UV値の再計算 */
		{
			/* MEMO: 元のメッシュセルのサイズが1の場合、各UV座標は比率になります。 */
			Library::Data::Parts::Animation::BindMesh::RateUV tableRateUV;
			dataBindMesh.TableRateUV(&tableRateUV);
			Float32 sizeTextureX = SizeTexture.GetX();
			Float32 sizeTextureY = SizeTexture.GetY();
			Float32 sizeInverseTextureX = 1.0f / sizeTextureX;
			Float32 sizeInverseTextureY = 1.0f / sizeTextureY;
			Float32 positionCellX = PositionCell.GetX();
			Float32 positionCellY = PositionCell.GetY();
			Float32 sizeCellX = SizeCell.GetX();
			Float32 sizeCellY = SizeCell.GetY();
			CPU::Vector2 rateUV;
			for(int i=0; i<countTableUV; i++)	{
				/* UV座標の取得 */
				tableRateUV.UV(&rateUV, i);

				/* MEMO: 本来セルに格納されているUV座標はピクセル単位系なので、整数に丸めています。 */
				/* MEMO: 計算手順の簡便さからFPUを使っていませんが……恐らくFPUを使うと型変換に時間がかかるのではないかと思われます。 */
				rateUV.SetX(rateUV.GetX() * sizeCellX);
				rateUV.SetX((float)(floor(rateUV.GetX())) + positionCellX);
				UVTextureDraw[i].SetX(rateUV.GetX() * sizeInverseTextureX);

				rateUV.SetY(rateUV.GetY() * sizeCellY);
				rateUV.SetY(sizeTextureY - ((float)(floor(rateUV.GetY())) + positionCellY));
				UVTextureDraw[i].SetY(rateUV.GetY() * sizeInverseTextureY);
			}
		}

		/* ローカル頂点座標を計算 */
		{
			CPU::Vector2 pivot;
			cell.Pivot(&pivot); 
			CPU::Vector2 coordinate;
			cell.Mesh(&dataMeshCell);
			for(int i=0; i<CountVertex; i++)	{
				/* 頂点座標の取得 */
				/* MEMO: CoordinateDrawはFPUなことに注意してください。 */
				dataMeshCell.TableCoordinate(&coordinate, i);
				CoordinateDraw[i].Set(	(coordinate.GetX() - pivot.GetX()),
										-(coordinate.GetY() - pivot.GetY()),
										0.0f,
										1.0f
									);
				if(true == flagUseDeform)	{	/* デフォーム機能を使っている */
					for(int i=0; i<CountVertex; i++)	{
						/* MEMO: 初期値として値を設定しています。 */
						DeformDraw[i].Set(CoordinateDraw[i]);
					}
				}
			}
		}

		/* MEMO: UVを再計算した場合、パーツカラーのリセットが必要 */
		/* MEMO: Need to reset parts color. */
		Status |= (FlagBitStatus::UPDATE_PARAMETERBLEND | FlagBitStatus::UPDATE_COLORPARTS);
	}

	/* パーツカラーの設定 */
	if(0 != (Status & (FlagBitStatus::UPDATE_COLORPARTS | FlagBitStatus::UPDATE_PARAMETERBLEND)))	{
		/* MEMO: メッシュの場合、パーツカラーは頂点毎には設定できません（SS6の仕様でオーバーオールのみ）。 */
		/*       ですので、左上の指定色がそのまま適用されます。                                            */
		const Float32 operationBlend = OperationPartsColor + 0.01f;	/* 0.01fは丸め誤差防止 */
		const Float32 rateOpacity = entity.RateOpacityForce * controlParts.RateOpacity.Value;
		const CPU::Color& colorParts = TableVertexColorPartsColor[Library::KindVertex::LU];
		Float32 fRate = 0.0f;
		Float32 fSrcRatio = 1.0f;
		Float32 fDstRatio = 0.0f;
		Float32 fDstSrcRatio = (Library::KindOperationBlend::MUL == OperationPartsColor) ? 1.0f : 0.0f;	/* OperationPartsColor == 1 */
		Float32 fReserved = 0.0f;

		for(int i=0; i<countTableUV; i++)	{
			UVTextureDraw[i].SetZ(operationBlend);
			UVTextureDraw[i].SetW(rateOpacity);

			ColorPartsDraw[i] = colorParts;

			fRate = 1.0f - ColorPartsDraw[i].GetA();
			fSrcRatio = OperationPartsColor <= 1 ? 1.0f - fRate : 1.0f;
			fDstRatio = OperationPartsColor == 3 ? -fRate : fRate;
			ArgumentVs00Draw[i].Set(fSrcRatio, fDstRatio, fDstSrcRatio, fReserved);
		}
	}

	ArgumentFs00Draw.Set(	SizeTexture.GetX(),			/* テクスチャの横ピクセル幅 */
							SizeTexture.GetY(),			/* テクスチャの縦ピクセル幅 */
							1.0f / SizeTexture.GetX(),	/* 1テクセルあたりのU値 */
							1.0f / SizeTexture.GetY()	/* 1テクセルあたりのV値 */
					);
	ArgumentFs02Draw.SetZ(0.0f);	/* プリマルチプライドアルファ */

	/* ローカルスケール係数を作成 */
	/* MEMO: メッシュパーツには「ローカルスケール」は効果しません（SS6の仕様）。 */
	/*       そのため、本係数ベクトルは（現状）平面化のためだけに存在しています  */
	/* MEMO: SS6Player for Unityの場合は、平面化は行列のZ軸部分を全部0.0に上書きしてやっています。 */
	/*       計算量を減らさなくてはならなくなったら、その手に出るのが良いかもしれません。          */
	FPU::Vector4 scaleVector4(FPU::Vector4::One);
	if(true == flagPlanarization)	{	/* Z成分をつぶす */
		scaleVector4.SetZ(0.0f);
	}

	/* 頂点データを作成 */
	{
		FPU::Vector4 coordinate;
		FPU::Vector4 coordinateScaled;
		FPU::Matrix4x4 matrixTransform;
		MatrixMul(&matrixTransform, argument.MatrixCamera, controlParts.MatrixTransform);

		const FPU::Vector4 coordinateAdjust(0.0f, 0.0f, 0.0f, 0.0f);

		/* MEMO: CPU::Vector3*をFloat32*にキャストしているのは、正直あまり意味はありません。  */
		/*       ※ソースが現在メッシュバッチングする場合としない場合で共用しているための措置 */
		/*         なので、後にメッシュバッチングしない場合を削除した後に記述を変更します。   */
		/* MEMO: メッシュパーツは必ずCoordinateTransformedDrawを持っているので、nullptrチェックしていません。 */
		CPU::Vector3* coordinateTransformed = CoordinateTransformedDraw;
		CPU::Vector3* coordinateOut = coordinateTransformed;

		if(0 >= countTableBindMesh)	{	/* スケルタル（ボーン）アニメーションをしない（メッシュバインドがない） */
			if(true == flagUseDeform)	{	/* デフォーム機能を使っている */
				/* Transform including "Deform" */
				/* MEMO: この場合は「DeformDraw」はデフォームを加味した値です。 */
				if(0 != (Status & FlagBitStatus::UPDATE_DEFORM))	{	/* デフォームが更新されている */
					CPU::Vector2* vertexDeform;
					FPU::Vector4 coordinateDeform;
					for(int i=0; i<CountVertex; i++)	{
						vertexDeform = &Deform.Value.TableCoordinate[i];
						coordinateDeform.Set(vertexDeform->GetX(), vertexDeform->GetY(), 0.0f, 0.0f);
						VectorAdd(&DeformDraw[i], CoordinateDraw[i], coordinateDeform);
					}
				}

				for(int i=0; i<CountVertex; i++)	{
					VectorTransform(&coordinate, DeformDraw[i], matrixTransform);
					VectorMul(&coordinateScaled, coordinate, scaleVector4);	/* ローカルスケールの適用 */
					VectorAdd(&coordinateScaled, coordinateScaled, coordinateAdjust);
					coordinateScaled.SetW(1.0f);	/* 念のため */
					coordinateOut->Set(coordinateScaled.GetX(), coordinateScaled.GetY(), coordinateScaled.GetZ());
					coordinateOut++;
				}
			} else {	/* デフォームを使用していない（何もアニメーションしないメッシュ） */
				/* MEMO: この場合は「DeformDraw」は使用していません。 */
				for(int i=0; i<CountVertex; i++)	{
					VectorTransform(&coordinate, CoordinateDraw[i], matrixTransform);
					VectorMul(&coordinateScaled, coordinate, scaleVector4);	/* ローカルスケールの適用 */
					VectorAdd(&coordinateScaled, coordinateScaled, coordinateAdjust);
					coordinateScaled.SetW(1.0f);	/* 念のため */
					coordinateOut->Set(coordinateScaled.GetX(), coordinateScaled.GetY(), coordinateScaled.GetZ());
					coordinateOut++;
				}
			}
		} else {	/* スケルタル（ボーン）アニメーションする（メッシュバインドがある） */
			/* MEMO: SS6の場合、1頂点に効果するボーン数に制限（レガシーなスキンメッシュのように4本 */
			/*       まで……等）がないので、シェーダではなくCPUで計算しています。                 */
			/* MEMO: アニメーションする場合、CoordinateDrawの内容はボーンからの計算結果で上書きされますので、その点に注意してください。 */
			Library::Data::Parts::Animation::Catalog catalogParts;
			entity.DataAnimation->CatalogParts(&catalogParts);
			const Uint16* tablePartsBone = catalogParts.TableIDPartsBone();

			Library::Data::Parts::Animation::BindMesh::Vertex dataVertex;
			dataBindMesh.TableVertex(&dataVertex);
			Library::Data::Parts::Animation::BindMesh::Vertex::Bone dataBone;

			/* MEMO: バインドされていない頂点があることに注意（なので、countVertexではなくバインドされた頂点分で行うこと）。 */
			int countBone;
			int idPartsBone;
			Float32 weight;
			FPU::Vector4 coordinateOffset;
			FPU::Vector4* coordinateDraw;
			const FPU::Matrix4x4* matrixBone;
			for(int i=0; i<countTableBindMesh; i++)	{
				/* MEMO: coordinateDrawの同次要素（w）は加算時は無視しています。 */
				coordinateDraw = &CoordinateDraw[i];
				coordinateDraw->Set(FPU::Vector4::Zero);	/* クリア（累積計算するため） */

				countBone = dataVertex.CountTableBone(i);
				dataVertex.TableBone(&dataBone, i);
				for(int j=0; j<countBone; j++)	{
					idPartsBone = dataBone.Index(j);
					idPartsBone = (Sint16)(tablePartsBone[idPartsBone]);
					if(0 <= idPartsBone)	{	/* ボーンIDが有効 */
						/* MEMO: 関連するボーンの影響をweightの比率で足しこんでいます。 */
						/* MEMO: coordinateDrawは同次（w）は無視して計算しています。 */
						/* MEMO: この場合でのcoordinateScaledは単なる値を取得するためのテンポラリです。 */
						matrixBone = &entity.TableControlParts[idPartsBone].MatrixTransform;

						dataBone.CoordinateOffset(&coordinateScaled, j);
						VectorTransform(&coordinate, coordinateScaled, *matrixBone);

						weight = dataBone.Weight(j);
						VectorMul(&coordinate, coordinate, weight);
						VectorAdd(coordinateDraw, *coordinateDraw, coordinate);
					}
				}

				coordinateDraw->SetW(1.0f);	/* 合計は（Weightの合計なので）1.0fになるはずなのだけど……念のため */
			}

			/* デフォームの適用 */
			if(true == flagUseDeform)	{	/* デフォーム機能を使っている */
				if(0 != (Status & FlagBitStatus::UPDATE_DEFORM))	{
					/* MEMO: この場合は「DeformDraw」は単なる計算ワーク（デフォームの影響値の計算結果が入っている）です。 */
					for(int i=0; i<countVertexDeform; i++)	{	/* デフォームが更新されている */
						DeformDraw[i].Set(FPU::Vector4::Origin);
					}

					/* MEMO: デフォームされている頂点のみ計算しています。 */
					/* MEMO: 速度のためにFlatBuffersから直接値を取得しています（実装記述的にはあまりよろしくない……）。 */
					int countVertexChange = dataAnimationParts.Deform->TableIndexVertex()->size();
					const Sint32* tableVertexChange = dataAnimationParts.Deform->TableIndexVertex()->data();
					int indexVertex;
					const CPU::Vector2* coordinateCPU;
					for(int i=0; i<countVertexChange; i++)	{
						indexVertex = tableVertexChange[i];

						coordinateCPU = &Deform.Value.TableCoordinate[indexVertex];
						coordinate.Set(coordinateCPU->GetX(), coordinateCPU->GetY(), 0.0f, 0.0f);	/* 平行移動を無視したいので（同時座標ではなく）座標ベクトルとして */
						VectorTransform(&DeformDraw[indexVertex], coordinate, controlParts.MatrixTransform);	/* matrixTransformだとカメラの影響を二重に受ける */
					}
				}

				/* スケルタル（ボーン）アニメーションの結果を足す */
				for(int i=0; i<countTableBindMesh; i++)	{
					VectorAdd(&CoordinateDraw[i], CoordinateDraw[i], DeformDraw[i]);
					CoordinateDraw[i].SetW(1.0f);	/* 念のため */
				}
			}

			/* ローカルスケールの適用 */
			for(int i=0; i<countTableBindMesh; i++)	{
				VectorMul(&coordinateScaled, CoordinateDraw[i], scaleVector4);
				VectorAdd(&coordinateScaled, coordinateScaled, coordinateAdjust);
				/* MEMO: .w（同次）は（Vector3に入れなおすので）1.0fにしなくても大丈夫です。 */
				coordinateOut->Set(coordinateScaled.GetX(), coordinateScaled.GetY(), coordinateScaled.GetZ());
				coordinateOut++;
			}
		}
	}

	/* レンダリングターゲットの設定 */
	bool flagBindTexture = TextureSetTarget(&settingRender, &flagBreakBatching, entity, idParts, controlParts, argument, indexCellMap);

	/* セルマップテクスチャの設定 */
	TextureSetCellMap(&settingRender, entity, idParts, controlParts, argument, indexCellMap, flagBindTexture);

	/* 描画頂点を決定 */
	/* MEMO: セル情報が持っている頂点をそのまま使用します。 */
	cell.Mesh(&dataMeshCell);
	int countPrimitive = dataMeshCell.CountTableIndexVertex();
	if(0 >= countPrimitive)	{	/* 頂点が存在していない */
		/* MEMO: trueを返すべきかどうか……。 */
		return(true);
	}

	/* MEMO: 頂点数は必ず「プリミティブ数×3」存在している。 */
	countPrimitive /= PrimitiveBatcher::Constant::VERTEX_COUNT_TRIANGLE;	/* 3 */
	const Uint16* tableIndexVertex = dataMeshCell.TableIndexVertex();

	/* バッチングメッシュに追加 */
	if(0 >= entity.ModelDraw->PrimitiveAddMesh(	settingRender,				/* レンダリング設定 */
												tableIndexVertex,			/* 頂点インデックステーブル */
												countPrimitive,				/* 描画プリミティブ数 */
												CoordinateTransformedDraw,	/* 描画頂点座標 */
												ColorPartsDraw,				/* パーツカラー */
												UVTextureDraw,				/* テクスチャUV */
												FlagsDraw,					/* 描画フラグ群 */
												ArgumentVs00Draw,			/* 頂点シェーダ用パラメータ */
												ArgumentFs00Draw,			/* ピクセルシェーダ用パラメータA0 */
												ArgumentFs01Draw,			/* ピクセルシェーダ用パラメータA1 */
												ArgumentFs02Draw,			/* ピクセルシェーダ用パラメータA2 */
												ArgumentFs03Draw,			/* ピクセルシェーダ用パラメータA3 */
												ParameterFs00Draw,			/* ピクセルシェーダ用パラメータB0 */
												ParameterFs01Draw,			/* ピクセルシェーダ用パラメータB1 */
												ParameterFs02Draw,			/* ピクセルシェーダ用パラメータB2 */
												ParameterFs03Draw,			/* ピクセルシェーダ用パラメータB3 */
												flagBreakBatching			/* バッチング阻害指定 */
											)
	)	{	/* 失敗 */
		return(false);
	}

	/* ステータスの更新（フレーム内更新情報のクリア） */
	Status &= ~(	FlagBitStatus::UPDATE_COORDINATE
					| FlagBitStatus::UPDATE_UVTEXTURE
					| FlagBitStatus::UPDATE_PARAMETERBLEND
					| FlagBitStatus::UPDATE_COLORPARTS
					| FlagBitStatus::UPDATE_MASKING
					| FlagBitStatus::UPDATE_DEFORM
					| FlagBitStatus::UPDATE_TRANSFORM_TEXTURE
//					| FlagBitStatus::UPDATE_FLIP_COEFFICIENT
//					| FlagBitStatus::USE_ADDITIONALCOLOR	/* ここではパーツカラーの更新フラグは落とさない */
			);
	statusControlParts &= ~(	Library::Control::Animation::Parts::FlagBitStatus::UPDATE_SCALELOCAL
								| Library::Control::Animation::Parts::FlagBitStatus::UPDATE_RATEOPACITY
						);

	return(true);
}

/* ********************************************************* */
//! ターゲットテクスチャをレンダリング設定に設定
/* --------------------------------------------------------- */
/* MEMO:
 * 本関数が返すboolは、セルマップ用テクスチャをバインドするか
 * のフラグです。
 * ※AEだと仕様上テクスチャをバインドしない処理などがあるため
 *   に本関数が同フラグを返す形になっています。
 */
bool Draw::TextureSetTarget(	PrimitiveBatcher::SettingRender* settingRender,
								bool* flagBreakBatching,
								Entity& entity,
								int idParts,
								Parts& controlParts,
								ArgumentContainer& argument,
								int indexCellMap
						)
{
	/* レンダリングターゲットの特定 */
	bool flagBindTexture = (0 <= indexCellMap);	/* ? true : false */
	/* MEMO: 常にフレームバッファに出力 */
	settingRender->TextureSetTargetColor(0, nullptr, false, CPU::Color::Clear);
	settingRender->TextureSetTargetTest(nullptr, false, 0, 0.0f);

	/* MRT数の設定 */
	/* MEMO: 現時点ではターゲットは常に1つ。 */
	settingRender->CountSetTextureTargetColor(1);

	return(flagBindTexture);
}

/* ********************************************************* */
//! セルマップテクスチャをレンダリング設定に設定
/* --------------------------------------------------------- */
/* MEMO:
 * 本関数は、必ずTextureSetTargetの後で呼び出してください。
 * ※直後でなくても構いません。
 */
bool Draw::TextureSetCellMap(	PrimitiveBatcher::SettingRender* settingRender,
								Entity& entity,
								int idParts,
								Parts& controlParts,
								ArgumentContainer& argument,
								int indexCellMap,
								bool flagBindTexture
							)
{
	if(true == flagBindTexture)	{
		int indexCellMap = DataCellApply.IndexCellMap;
		int indexTexture = entity.IndexTextureCellMap[indexCellMap];
		Texture* texture = entity.Texture[indexTexture];

		Library::Data::CellMap::Body dataCellMap;
		entity.DataCellMap->TableCellMap(&dataCellMap, indexCellMap);
		Texture::KindInterpolation interp = dataCellMap.Filter();
		Texture::KindWrap wrap = dataCellMap.Wrap();

		settingRender->TextureSetCellMap(texture, interp, wrap);
	}

	return(true);
}

}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            AdditionalColor */
/* -------------------------------------------------------------------------- */
const CPU::Color AdditionalColor::ColorClear[Library::KindOperationBlend::TERMINATOR_PARTSCOLOR] = {
	/* MIX */	CPU::Color(0.0f, 0.0f, 0.0f, 0.0f),
	/* ADD */	CPU::Color(0.0f, 0.0f, 0.0f, 0.0f),
	/* SUB */	CPU::Color(0.0f, 0.0f, 0.0f, 0.0f),
	/* MUL */	CPU::Color(0.0f, 0.0f, 0.0f, 0.0f)
};
// void AdditionalColor::CleanUp(void)
// bool AdditionalColor::BootUp(void)
// void AdditionalColor::ShutDown(void)

/* ********************************************************* */
//! 全頂点で同じ色を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void AdditionalColor::SetOverall(Library::KindOperationBlend::Kind operation, const CPU::Color& color)
{
	if(Library::KindOperationBlend::NON == operation)	{
		Operation = Library::KindOperationBlend::NON;
		Status |= FlagBitStatus::CHANGE;
		return;
	}

	if((Library::KindOperationBlend::NON >= operation) || (Library::KindOperationBlend::TERMINATOR_PARTSCOLOR <= operation))	{	/* Ignore (Error) */
		return;
	}

	Operation = operation;
	for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)
	{
		VertexColor[i].Set(color);
	}

	Status |= FlagBitStatus::CHANGE;
}

/* ********************************************************* */
//! 四隅の頂点に異なる色を指定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void AdditionalColor::SetVertex(	Library::KindOperationBlend::Kind operation,
									const CPU::Color& colorLU,
									const CPU::Color& colorRU,
									const CPU::Color& colorRD,
									const CPU::Color& colorLD
							)
{
	if(Library::KindOperationBlend::NON == operation)	{
		Operation = Library::KindOperationBlend::NON;
		Status |= FlagBitStatus::CHANGE;
		return;
	}

	if((Library::KindOperationBlend::NON >= operation) || (Library::KindOperationBlend::TERMINATOR_PARTSCOLOR <= operation))	{	/* Ignore (Error) */
		return;
	}

	Operation = operation;
	VertexColor[Library::KindVertex::LU].Set(colorLU);
	VertexColor[Library::KindVertex::RU].Set(colorRU);
	VertexColor[Library::KindVertex::RD].Set(colorRD);
	VertexColor[Library::KindVertex::LD].Set(colorLD);

	Status |= FlagBitStatus::CHANGE;
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                   CallBack */
/* -------------------------------------------------------------------------- */
/* MEMO: テンプレートなので実体側はありません。 */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                       Root */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Root::CleanUp(void)

/* ********************************************************* */
//! 追加ワークエリア長の取得（テクスチャ関連）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// size_t Root::SizeGetWorkAreaTexture(int countTexture)

/* ********************************************************* */
//! 追加ワークエリアの設定（テクスチャ関連）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Root::WorkAreaSetTexture(int countTexture, void* buffer)

/* ********************************************************* */
//! 本クラスの起動（実行用初期化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Root::BootUp(void)

/* ********************************************************* */
//! 本クラスの終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Root::ShutDown(void)

/* ********************************************************* */
//! 本クラスの更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Root::Update(void)

/* ********************************************************* */
//! 最親エンティティを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Entity* Root::EntityGetParentHighest(void)
{
	Entity* parent = EntityParent;
	while(nullptr != parent)	{
		parent = parent->EntityParent;
	}
	return(parent);
}

/* ********************************************************* */
//! セルマップ数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Root::CountGetCellMap(bool flagInUse)
{
	if(true == flagInUse)	{
		/* MEMO: 現在未実装。後に積むこと。 */
	}
	/* MEMO: ここから下は元データから取得するための処理。 */

	return(DataProject->CountGetCellMap());
}

/* ********************************************************* */
//! セルマップのインデックスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Root::IndexGetCellMap(const char* name, bool flagInUse)
{
	if(true == flagInUse)	{
		/* MEMO: 現在未実装。後に積むこと。 */
	}
	/* MEMO: ここから下は元データから取得するための処理。 */

	return(DataProject->IndexGetCellMap(name));
}

/* ********************************************************* */
//! セルマップ番号から名前を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
const char* Root::NameGetCellMap(int indexCellMap, bool flagInUse)
{
	if(true == flagInUse)	{
		/* MEMO: 現在未実装。後に積むこと。 */
	}
	/* MEMO: ここから下は元データから取得するための処理。 */

	if((0 > indexCellMap) || (DataProject->CountGetCellMap() <= indexCellMap))	{
		return(nullptr);
	}

	Library::Data::CellMap::Body dataCellMap;
	DataProject->CellMap.TableCellMap(&dataCellMap, indexCellMap);

	return(dataCellMap.Name());
}

/* ********************************************************* */
//! セルマップ情報の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// const Library::Data::CellMap::Body* Root::DataGetCellMap(Library::Data::CellMap::Body* dataCellMap, int indexCellMap, bool flagInUse)

/* ********************************************************* */
//! セルマップデータを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Root::DataSetCellMap(int indexCellMap, Library::Data::CellMap::Body& dataCellMap)
{
	/* MEMO: 現在非対応 */
	return(false);
}

/* ********************************************************* */
//! セル数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Root::CountGetCell(int indexCellMap, bool flagInUse=true)

/* ********************************************************* */
//! セルのインデックス取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int Root::IndexGetCell(Library::Data::CellMap::Body& dataCellMap, const char* nameCell)
{
	int count = dataCellMap.CountTableCell();
	Library::Data::CellMap::Cell dataCell;
	for(int i=0; i<count; i++)	{
		if(nullptr == dataCellMap.TableCell(&dataCell, i))	{
			continue;
		}
		if(true == Library::Utility::Text::Compare(nameCell, dataCell.Name()))	{
			return(i);
		}
	}

	return(-1);
}

/* ********************************************************* */
//! セル情報の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// const Library::Data::CellMap::Cell* Root::DataGetCell(Library::Data::CellMap::Cell* dataCell, Library::Data::CellMap::Body& dataCellMap, int indexCell)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! PrimitiveBatcherのフラグメント描画前に割り込ませる描画コマンド生成
/* --------------------------------------------------------- */
/* MEMO:
 * C++のスコープ的にはDrawクラスにでも入れるべきかと思われますが、
 * （機能面では多少毛色を異にするので）意図的にファイル内大域
 * 関数にしてあります。
 */
static TypeDrawCommandList CallBackFunctionCommandInsertFragment(TypeDrawCommandList commandList, void* parameterPointer, int parameterInteger)
{
	return(commandList);
}

}	/* Control */
}	/* Library */
}	/* SpriteStudio6 */
