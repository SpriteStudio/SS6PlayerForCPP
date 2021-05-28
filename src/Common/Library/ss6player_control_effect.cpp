/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_control_effect.h"
#include "../Library/ss6player_utility.h"
#include "../Entity/ss6player_entity_effect.h"
#include "../Project/ss6player_project.h"

#include <cmath>

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

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! Floatの直線補間
/*!
@param	_start_
	開始値
@param	_end_
	終了値
@param	_rate_
	区間比率（0.0f:開始点 / 1.0f:終了点）

@retval	関数値
	指定位置値

_rate_の位置の値を直線補間で求めます。
※マクロですが、いつでもinline関数にできるよう関数の命名規則を
	しています。
*/
#define Lerp(_start_,_end_,_rate_)	((((_end_) - (_start_)) * (_rate_)) + (_start_))

/* ********************************************************* */
//! ベクトルの長さ
/*!
@param	value
	元値

@retval	関数値
	ベクトルの長さ

valueの長さを返します。
*/
inline Float32 VectorMagnitude(const CPU::Vector2& value)
{
	const float x = value.GetX();
	const float y = value.GetY();
	return(std::sqrt((x * x) + (y * y)));
}
inline Float32 VectorMagnitude(const CPU::Vector3& value)
{
	const float x = value.GetX();
	const float y = value.GetY();
	const float z = value.GetZ();
	return(std::sqrt((x *x) + (y * y) + (z * z)));
}

/* ********************************************************* */
//! ベクトルの正規化
/*!
@param	result
	出力値の格納先
@param	value
	元値

@retval	関数値
	なし
@param	result
	出力値の格納先

valueを正規化したベクトルを返します。
*/
inline void VectorNormalize(CPU::Vector2* result, const CPU::Vector2& value)
{
	Float32 lengthInverse = 1.0f / VectorMagnitude(value);
	result->Set(	value.GetX() * lengthInverse,
					value.GetY() * lengthInverse
			);
}
inline void VectorNormalize(CPU::Vector3* result, const CPU::Vector3& value)
{
	Float32 lengthInverse = 1.0f / VectorMagnitude(value);
	result->Set(	value.GetX() * lengthInverse,
					value.GetY() * lengthInverse,
					value.GetZ() * lengthInverse
			);
}

/* ********************************************************* */
//! 
/*!
@param	time
	現時点時間
@param	timeFull
	終点時間
@param	valueMax
	最大値
@param	valueMin
	最小値

@retval	関数値
	結果
*/
inline Float32 OutQuad(Float32 time, Float32 timeFull, Float32 valueMax, Float32 valueMin)
{
	if(0.0f >= timeFull)	{
		return(0.0f);
	}
	if(time > timeFull)	{
		time = timeFull;
	}

	valueMax -= valueMin;
	time /= timeFull;
	return(-valueMax * time * (time - 2.0f) + valueMin);
}

/* ********************************************************* */
//! 反時計回りでの角度
/*!
@param	start
	起点ベクトル
@param	end
	終点

@retval	関数値
	ベクトルの角度
*/
inline Float32 AngleGetCCW(const CPU::Vector2& start, const CPU::Vector2& end)
{
	CPU::Vector2 startNormalized;
	VectorNormalize(&startNormalized, start);
	CPU::Vector2 endNormalized;
	VectorNormalize(&endNormalized, end);

	Float32 dot = (startNormalized.GetX() * endNormalized.GetX()) + (startNormalized.GetY() * endNormalized.GetY());	/* 内積（dot） */
	if(-1.0f > dot)	{	/* Clamp(dot, -1.0f, 1.0f) */
		dot = -1.0f;
	} else {
		if(1.0f < dot)	{
			dot = 1.0f;
		}
	}

	Float32 angle = std::acos(dot);
	Float32 cross = (startNormalized.GetX() * endNormalized.GetY()) - (endNormalized.GetX() * startNormalized.GetY());
	angle = (0.0f > cross) ? ((2.0f * PI) - angle) : angle;

	return(angle);
}

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                 Effect::Particle::Activity */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Effect::Particle::Activity::Activity(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Effect::Particle::Activity::~Activity(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::Particle::Activity::CleanUp(void)

/* ********************************************************* */
//! 更新のメイン処理
/* --------------------------------------------------------- */
/* MEMO:
 * Updateが（格納形態の都合と使用メモリ削減最適化の都合で）静的
 * データからと動的データからの2種類になったため、本処理を別に
 * しました。
 * 呼出回数が多い関数なので、動作速度の足を引っ張る場合、マクロ
 * 等のインライン展開に置き換えるかもしれません（C++の時のみ）。
 */
void Effect::Particle::Activity::UpdateMain(	int frame,
												Library::Control::Effect::Emitter& emitter,
												const Library::Data::Effect::Emitter& dataEmitter,
												int patternOffset,
												int durationTarget,
												int cycleTarget
										)
{
	Status &= ~(FlagBitStatus::BORN | FlagBitStatus::EXIST);

	int duration = emitter.Duration;
	int frameNow = (int)(frame - patternOffset);
	if(0 != cycleTarget)	{
		int countLoop = frameNow / cycleTarget;
		int cycleTop = countLoop * cycleTarget;

		Cycle = countLoop;
		FrameStart = cycleTop + patternOffset;
		FrameEnd = FrameStart +	durationTarget;

		if((frame >= FrameStart) && (frame < FrameEnd))	{
			Status |= (FlagBitStatus::BORN | FlagBitStatus::EXIST);
		}

		if(0 == (dataEmitter.FlagData() & Library::Data::Effect::Emitter::FlagBit::EMIT_INFINITE))	{
			if(FrameStart >= duration)	{
				Status &= ~FlagBitStatus::EXIST;

				FrameStart = ((duration - patternOffset) / cycleTarget) * cycleTarget + patternOffset;
				FrameEnd = FrameStart + durationTarget;
				Status &= ~FlagBitStatus::BORN;
			} else {
				Status |= FlagBitStatus::BORN;
			}
		}

		if(0 > frameNow)	{
			Status &= ~(FlagBitStatus::BORN | FlagBitStatus::EXIST);
		}
	}
}

/* ********************************************************* */
//! 更新（静的データ用）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Effect::Particle::Activity::Update(	int frame,
											Library::Control::Effect::Emitter& emitter,
											const Library::Data::Effect::Emitter& dataEmitter,
											int indexPatternEmit,
											int patternOffset,
											int indexPatternEmitTarget
									)
{
	Library::Data::Effect::Emitter::PatternEmit patternEmitTarget;
	emitter.DataEmitter.TablePatternEmit(&patternEmitTarget, indexPatternEmitTarget);
	UpdateMain(frame, emitter, dataEmitter, patternOffset, patternEmitTarget.Duration, patternEmitTarget.Cycle);;
}

/* ********************************************************* */
//! 更新（動的ワーク用）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Effect::Particle::Activity::Update(	int frame,
											Library::Control::Effect::Emitter& emitter,
											const Library::Data::Effect::Emitter& dataEmitter,
											const Library::Data::Effect::Emitter::PatternEmit& patternEmit,
											int patternOffset,
											const Library::Data::Effect::Emitter::PatternEmit& patternEmitTarget
									)
{
	UpdateMain(frame, emitter, dataEmitter, patternOffset, patternEmitTarget.Duration, patternEmitTarget.Cycle);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Effect::Particle */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
//  Effect::Particle::Particle(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Effect::Particle::~Particle(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::Particle::CleanUp(void)

/* ********************************************************* */
//! 更新
/* --------------------------------------------------------- */
/* MEMO:
 * 本関数が終了した時点で「particleParent」の内容は破壊されますので、
 * 注意してください。
 */
bool Effect::Particle::Exec(	float frame,
								int index,
								EntityEffect& entity,
								Library::Control::Effect& controlEffect,
								Library::Control::Effect::Emitter& emitter,
								Activity& activity,
								int indexEmitterParent,
								Particle& particleParent
						)
{
	Uint32 /* Activity.FlagBitStatus */ flagStatusActivity = activity.Status;
	if(0 == (flagStatusActivity & Activity::FlagBitStatus::BORN))	{
		return(true);
	}

	float frameTarget = frame;
	FrameStart = activity.FrameStart;
	FrameEnd = activity.FrameEnd;
	ID = index + activity.Cycle;
	IDParent = (0 <= indexEmitterParent) ? particleParent.ID : 0;

	if(0 != (flagStatusActivity & Activity::FlagBitStatus::EXIST))	{
		Library::Data::Effect::Emitter dataEmitter;

		if(0 <= indexEmitterParent)	{	/* Has Parent */
			particleParent.PositionX = particleParent.PositionY = 0.0f;

			Library::Control::Effect::Emitter* emitterParent = &controlEffect.TableEmitter[indexEmitterParent];
			emitterParent->DataEffect->TableEmitter(&dataEmitter, indexEmitterParent);

			particleParent.Calculate(	(float)(FrameStart + particleParent.FrameStart),
										entity,
										controlEffect,
										*emitterParent,
										dataEmitter,
										false
									);
			emitter.Position.Set(particleParent.PositionX, particleParent.PositionY);
		}

		emitter.DataEffect->TableEmitter(&dataEmitter, emitter.IndexEmitter);
		if(true == Calculate(	frameTarget,
								entity,
								controlEffect,
								emitter,
								dataEmitter,
								false
							)
			)	{	/* Draw */
			/* MEMO: ここで、実際の描画頂点の定義を行っています。 */
			if(nullptr == entity.ModelDraw)	{	/* 描画モデルが未設定 */
				return(false);
			}
			if(entity.LimitParticleDraw <= entity.CountParticleDraw)	{	/* 描画最大数超過 */
				return(false);
			}
			entity.CountParticleDraw++;

			/* パーティクルの変形行列の算出 */
			FPU::Matrix4x4 matrixTransform;
			CPU::Vector2 scaleLayout;
			entity.DataEffect->ScaleLayout(&scaleLayout);
			MatrixCreateTRS(	&matrixTransform,
								FPU::Vector3((PositionX * scaleLayout.GetX()), (PositionY * scaleLayout.GetY()), 0.0f),
								FPU::Vector3(0.0f, 0.0f, (RotateZ + Direction)),
								Scale
						);
			MatrixMul(&matrixTransform, matrixTransform, controlEffect.MatrixRoot);

			/* 頂点座標の変形 */
			/* MEMO: パーティクルの頂点には中心点はありません（対角線2分割三角形）。 */
			CPU::Vector3 coordinateTransformed[Library::KindVertex::TERMINATOR2];
			FPU::Vector4 coordinate;
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
				VectorTransform(&coordinate, emitter.CoordinateDraw[i], matrixTransform);
				coordinateTransformed[i].Set(coordinate.GetX(), coordinate.GetY(), coordinate.GetZ());	/* FPU::Vector4→CPU::Vector3 */
			}

			/* UV座標に合成設定を埋込 */
			/* MEMO: これはエミッタで統一できるかと思われます。 */
			const Float32 operationBlend = Library::KindOperationBlend::MUL + 0.01f;	/* 0.01fは丸め誤差防止 */
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
				emitter.UVTextureDraw[i].SetZ(operationBlend);	/* カラーブレンド合成方法 */
				emitter.UVTextureDraw[i].SetW(entity.RateOpacityForce);	/* MEMO: パーティクルの不透明度はColorVertex.aと本値の乗算結果です。 */
			}

			/* 頂点を描画定義 */
			/* MEMO: ここで設定されるemitter.FlagDrawは.zが1.0f（パーツカラーを頂点色として扱う）であることに注意してください。 */
			if(0 >= entity.ModelDraw->PrimitiveAddParticle(	emitter.SettingRender,	/* レンダリング設定 */
															coordinateTransformed,	/* 描画頂点座標 */
															ColorVertex,			/* パーツカラー */
															emitter.UVTextureDraw,	/* テクスチャUV */
															emitter.FlagsDraw,		/* 描画フラグ群 */
															CPU::Vector4::Zero,		/* 頂点シェーダ用パラメータ */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータA0 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータA1 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータA2 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータA3 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータB0 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータB1 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータB2 */
															CPU::Vector4::Zero,		/* ピクセルシェーダ用パラメータB3 */
															false					/* バッチング阻害指定 */
														)
			)	{	/* 失敗 */
				return(false);
			}
		}
	}

	return(true);
}

/* ********************************************************* */
//! 計算
/* --------------------------------------------------------- */
/* MEMO:
 * 本関数で行われている乱数の振出回数と振出目的の順序の変更を
 * してはいけません。（要注意）
 * ※乱数の初期種と振出回数とでの振出結果の同一性が保証されて
 *   いることが（SS本体とプレーヤの再現性の）大前提＆担保となっ
 *   ているためです。
 *   （Xor-Shiftにしてもメルセンヌツィスタにしても、特定の種
 *     で初期化した後のn回目の出力は同じ値が出てくることが保
 *     証されています）
 */
bool Effect::Particle::Calculate(	float frame,
									EntityEffect& entity,
									Library::Control::Effect& controlEffect,
									Library::Control::Effect::Emitter& emitter,
									Library::Data::Effect::Emitter& dataEmitter,
									bool flagSimplicity
								)
{
	Library::Utility::Random::Generator* random = emitter.Random;
	Float32 frameRelative = (frame - (Float32)FrameStart);
	Float32 framePower2 = frameRelative * frameRelative;
	Float32 life = (Float32)(FrameEnd - FrameStart);
	if(0.0f >= life)	{	/* (0 == life) */
		return(false);
	}

	Float32 rateLife = frameRelative / life;
	Sint64 seedParticle = emitter.TableSeedParticle[ID % emitter.CountTableSeedParticle];
	random->InitSeed((Uint32)(	(Uint64)seedParticle
								+ (Uint64)emitter.SeedRandom
								+ (Uint64)IDParent
								+ (Uint64)emitter.SeedOffset
						)
				);

	/* Calc Parameters */
	Library::Data::Effect::Emitter::RangeFloat dataRangeFloat;
	Library::Data::Effect::Emitter::RangeVector2 dataRangeVector2;
	Library::Data::Effect::Emitter::RangeColor dataRangeColor;

	Library::Data::Effect::Emitter::FlagBit flagData = dataEmitter.FlagData();

	dataEmitter.Angle(&dataRangeFloat);	/* Angle */
	Float32 radianSub = dataRangeFloat.Sub * CoefficientDegreeToRadian;
	Float32 radian = random->RandomFloat(radianSub);
	radian = radian - (radianSub * 0.5f);
	radian += ((dataRangeFloat.Main + 90.0f) * CoefficientDegreeToRadian);

	dataEmitter.Speed(&dataRangeFloat);	/* Speed */
	Float32 speed = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);

	Float32 radianOffset = 0;
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::TANGENTIALACCELATION))	{
		dataEmitter.RateTangentialAcceleration(&dataRangeFloat);	/* Rate-TangentAccelaration */
		Float32 accel = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);
		Float32 speedTemp = speed;
		speedTemp = (0.0f >= speedTemp) ? 0.1f : speedTemp;
		radianOffset = (accel / (3.14f * (life * speedTemp * 0.2f))) * frameRelative;
	}

	Float32 angleTemp = radian + radianOffset;
	Float32 cos = std::cos(angleTemp);
	Float32 sin = std::sin(angleTemp);
	Float32 speedX = cos * speed;
	Float32 speedY = sin * speed;
	Float32 x = speedX * frameRelative;
	Float32 y = speedY * frameRelative;
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::SPEED_FLUCTUATION))	{
		dataEmitter.SpeedFluctuation(&dataRangeFloat);	/* Speed Fluctuation */
		Float32 speedFluctuation = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);
		Float32 speedOffset = speedFluctuation / life;

		x = (((cos * speedOffset) * frameRelative) + speedX) * ((frameRelative + 1.0f) * 0.5f);
		y = (((sin * speedOffset) * frameRelative) + speedY) * ((frameRelative + 1.0f) * 0.5f);
	}

	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::GRAVITY_DIRECTION))	{
		CPU::Vector2 dataGravityDirectional;
		dataEmitter.GravityDirectional(&dataGravityDirectional);	/* Gravity(Directional) */
		x += (0.5f * dataGravityDirectional.GetX() * framePower2);
		y += (0.5f * dataGravityDirectional.GetY() * framePower2);
	}
	Float32 offsetX = 0.0f;
	Float32 offsetY = 0.0f;
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::POSITION))	{
		dataEmitter.Position(&dataRangeVector2);	/* Position */
		offsetX = dataRangeVector2.Main.GetX() + random->RandomFloat(dataRangeVector2.Sub.GetX());
		offsetY = dataRangeVector2.Main.GetY() + random->RandomFloat(dataRangeVector2.Sub.GetY());
	}

	RotateZ = 0.0f;
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::ROTATION))	{
		dataEmitter.Rotation(&dataRangeFloat);	/* Rotation */
		RotateZ = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);

		dataEmitter.RotationFluctuation(&dataRangeFloat);	/* Rotation-Fluctuation */
		Float32 rotationFluctuation = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);
		if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::ROTATION_FLUCTUATION))	{
			Float32 frameLast = life * dataEmitter.RotationFluctuationRateTime();	/* Rotation Fluctuation Rate-Time */
			Float32 rateRotationFluctuation = 0.0f;
			if(0.0f >= frameLast)	{	/* 0.0以下 */
				RotateZ += (rotationFluctuation * dataEmitter.RotationFluctuationRate()) * frameRelative;
			} else {
				rateRotationFluctuation = ((rotationFluctuation * dataEmitter.RotationFluctuationRate()) - rotationFluctuation) / frameLast;

				Float32 frameModuration = frameRelative - frameLast;
				frameModuration = (0.0f > frameModuration) ? 0.0f : frameModuration;

				Float32 frameRelativeNow = frameRelative;
				frameRelativeNow = (frameRelativeNow > frameLast) ? frameLast : frameRelativeNow;

				Float32 rotateOffsetTemp = rateRotationFluctuation * frameRelativeNow;
				rotateOffsetTemp += rotationFluctuation;
				Float32 rotateOffset = (rotateOffsetTemp + rotationFluctuation) * (frameRelativeNow + 1.0f) * 0.5f;
				rotateOffset -= rotationFluctuation;
				rotateOffset += (frameModuration * rotateOffsetTemp);
				RotateZ += rotateOffset;
			}
		} else {
			RotateZ += (rotationFluctuation * frameRelative);
		}
	}

	/* ColorVertex/AlphaFade */
	ColorVertex = CPU::Color::White;

	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::COLORVERTEX))	{
		dataEmitter.ColorVertex(&dataRangeColor);	/* Color-Vertex */
		ColorVertex.SetA(dataRangeColor.Main.GetA() + random->RandomFloat(dataRangeColor.Sub.GetA()));
		ColorVertex.SetR(dataRangeColor.Main.GetR() + random->RandomFloat(dataRangeColor.Sub.GetR()));
		ColorVertex.SetG(dataRangeColor.Main.GetG() + random->RandomFloat(dataRangeColor.Sub.GetG()));
		ColorVertex.SetB(dataRangeColor.Main.GetB() + random->RandomFloat(dataRangeColor.Sub.GetB()));
	}
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::COLORVERTEX_FLUCTUATION))	{
		dataEmitter.ColorVertexFluctuation(&dataRangeColor);	/* Color-Vertex Fluctuation */
		CPU::Color colorFluctuation;

		colorFluctuation.SetA(dataRangeColor.Main.GetA() + random->RandomFloat(dataRangeColor.Sub.GetA()));
		colorFluctuation.SetR(dataRangeColor.Main.GetR() + random->RandomFloat(dataRangeColor.Sub.GetR()));
		colorFluctuation.SetG(dataRangeColor.Main.GetG() + random->RandomFloat(dataRangeColor.Sub.GetG()));
		colorFluctuation.SetB(dataRangeColor.Main.GetB() + random->RandomFloat(dataRangeColor.Sub.GetB()));

		ColorVertex.SetA(Lerp(ColorVertex.GetA(), colorFluctuation.GetA(), rateLife));
		ColorVertex.SetR(Lerp(ColorVertex.GetR(), colorFluctuation.GetR(), rateLife));
		ColorVertex.SetG(Lerp(ColorVertex.GetG(), colorFluctuation.GetG(), rateLife));
		ColorVertex.SetB(Lerp(ColorVertex.GetB(), colorFluctuation.GetB(), rateLife));
	}

	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::FADEALPHA))	{
		Float32 rateStart = dataEmitter.AlphaFadeStart();	/* Alpha-Fade Start */
		Float32 rateEnd = dataEmitter.AlphaFadeEnd();	/* Alpha-Fade End */
		if(rateLife < rateStart)	{
			ColorVertex.SetA(ColorVertex.GetA() * (1.0f - ((rateStart - rateLife) / rateStart)));
		} else {
			if(rateLife > rateEnd)	{
				if(1.0f <= rateEnd)	{
					ColorVertex.SetA(0.0f);
				} else {
					Float32 Alpha = (rateLife - rateEnd) / (1.0f - rateEnd);
					Alpha = (1.0f <= Alpha) ? 1.0f : Alpha;
					ColorVertex.SetA(1.0f - Alpha);
				}
			}
		}
	}

	/* Scale */
	Scale = FPU::Vector3::One;	/* FPU::Vector2::One */
	Float32 scaleRate = 1.0f;

	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::SCALE_START))	{
		dataEmitter.ScaleStart(&dataRangeVector2);	/* Scale Start */
		Scale.SetX(dataRangeVector2.Main.GetX() + random->RandomFloat(dataRangeVector2.Sub.GetX()));
		Scale.SetY(dataRangeVector2.Main.GetY() + random->RandomFloat(dataRangeVector2.Sub.GetY()));

		dataEmitter.ScaleRateStart(&dataRangeFloat);	/* Scale-Rate Start */
		scaleRate = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);
	}
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::SCALE_END))	{
		FPU::Vector3 scaleEnd;
		float scaleRateEnd;
		dataEmitter.ScaleEnd(&dataRangeVector2);	/* Scale End */
		scaleEnd.Set(	dataRangeVector2.Main.GetX() + random->RandomFloat(dataRangeVector2.Sub.GetX()),
						dataRangeVector2.Main.GetY() + random->RandomFloat(dataRangeVector2.Sub.GetY()),
						1.0f
				);

		dataEmitter.ScaleRateEnd(&dataRangeFloat);	/* Scale-Rate Start */
		scaleRateEnd = dataRangeFloat.Main + random->RandomFloat(dataRangeFloat.Sub);

		Scale.SetX(Lerp(Scale.GetX(), scaleEnd.GetX(), rateLife));
		Scale.SetY(Lerp(Scale.GetY(), scaleEnd.GetY(), rateLife));
		scaleRate = Lerp(scaleRate, scaleRateEnd, rateLife);
	}
	Scale.SetX(Scale.GetX() * scaleRate);
	Scale.SetY(Scale.GetY() * scaleRate);
	Scale.SetZ(1.0f);	/* Overwrite, force */

	/* Position/Gravity */
	PositionX = x + (emitter.Position.GetX() + offsetX);
	PositionY = y + (emitter.Position.GetY() + offsetY);

	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::GRAVITY_POINT))	{
		CPU::Vector2 vectorPosition;
		CPU::Vector2 dataGravityPosition;
		dataEmitter.GravityPointPosition(&dataGravityPosition);	/* Scale End */
		Float32 positionXGravity = dataGravityPosition.GetX();
		Float32 positionYGravity = dataGravityPosition.GetY();
#if 0	/* MEMO: 一時期（5.7開発中）下記だったのだが式が変更になっている＆内容的に（特に最適化時に)間違いやすいので、メモとして残しておく。 */
		vectorPosition.Set((positionXGravity - (offsetX + PositionX)), (positionYGravity - (offsetY + PositionY)));
#else	/* MEMO: 正しい実装式はこちら。 */
		vectorPosition.Set((positionXGravity - (offsetX + emitter.Position.GetX())), (positionYGravity - (offsetY + emitter.Position.GetY())));
#endif

		CPU::Vector2 vectorNormal;
		VectorNormalize(&vectorNormal, vectorPosition);
		Float32 gravityPower = dataEmitter.GravityPointPower();	/* Gravity Power */
		if(0.0f < gravityPower)	{
			Float32 eFrame = (VectorMagnitude(vectorPosition) / gravityPower) * 0.9f;
			Float32 gFrame = (frameRelative >= (int)eFrame) ? (eFrame * 0.9f) : frameRelative;

			Float32 coefficient = gravityPower * gFrame;
			VectorMul(&vectorNormal, vectorNormal, coefficient);
			PositionX += vectorNormal.GetX();
			PositionY += vectorNormal.GetY();

			float Blend = OutQuad(gFrame, eFrame, 0.9f, 0.0f);
			Blend += (frameRelative / life * 0.1f);

			PositionX = PositionX + ((positionXGravity - PositionX) * Blend);	/* CAUTION!: Don't use "Lerp" */
			PositionY = PositionY + ((positionYGravity - PositionY) * Blend);	/* CAUTION!: Don't use "Lerp" */
		} else {
			/* MEMO: In the case negative power, Simply repulsion. Attenuation due to distance is not taken into account. */
			Float32 coefficient = gravityPower * frameRelative;
			VectorMul(&vectorNormal, vectorNormal, coefficient);
			PositionX += vectorNormal.GetX();
			PositionY += vectorNormal.GetY();
		}
	}

	/* Turn-Direction */
	Direction = 0.0f;
	if((0 != (flagData & Library::Data::Effect::Emitter::FlagBit::TURNDIRECTION)) && (false == flagSimplicity))	{
		emitter.ParticleTempolary2.Duplicate(*this);

		Library::Data::Effect::Emitter dataEmitterTempolary;
		emitter.DataEffect->TableEmitter(&dataEmitterTempolary, emitter.IndexEmitter);

		emitter.ParticleTempolary2.Calculate(	(frame + 1.0f),	/* (Frame + 0.1f), */
												entity,
												controlEffect,
												emitter,
												dataEmitterTempolary,
												true
											);
		float radianDirection = AngleGetCCW(	CPU::Vector2(1.0f, 0.0f),
												CPU::Vector2((PositionX - emitter.ParticleTempolary2.PositionX), (PositionY - emitter.ParticleTempolary2.PositionY))
											);

		Direction = (radianDirection * CoefficientRadianToDegree) + 90.0f + dataEmitter.TurnDirectionFluctuation();	/* Turn-Direction Fluctuation */
	}

	return(true);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            Effect::Emitter */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::Emitter::CleanUp(void)

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Effect::Emitter::Emitter(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Effect::Emitter::~Emitter(void)

/* ********************************************************* */
//! エミッタ再生管理が使用するワークエリア長の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
size_t Effect::Emitter::SizeGetWorkArea(EntityEffect& entity, Library::Data::Project::Effect& dataEffect, int indexEmitter)
{
	int countTablePatternOffset;
	int countTablePatternEmit;
	int countTableSeedParticle;
	int countEmitMax;
	int countEmit;
	int countParticleMax;
	size_t sizeTablePattern;
	size_t sizeTableActivity;

	/* エミッタ情報を取得 */
	Library::Data::Effect::Emitter dataEmitter;
	entity.DataEffect->TableEmitter(&dataEmitter, indexEmitter);

	/* 各種テーブル群の情報を取得 */
	countTablePatternOffset = dataEmitter.CountTablePatternOffset();
	countTablePatternEmit = dataEmitter.CountTablePatternEmit();
	countTableSeedParticle = dataEmitter.CountTableSeedParticle();
	countParticleMax = dataEmitter.CountParticleMax();

	/* 生成パターンのテーブル長を計算 */
	sizeTablePattern = 0;
	if((0 >= countTablePatternEmit) || (0 >=countTableSeedParticle))	{	/* ランタイムでテーブルを算出する必要がある（乱数種が固定でない） */
		/* テーブルの格納領域を作成 */
		sizeTablePattern = dataEmitter.SizeGetTablePatternEmit(&countTablePatternEmit, &countTableSeedParticle, &countEmitMax, &countEmit);
	}

	/* パーティクルの死活状況テーブル長を計算 */
	sizeTableActivity = 0;
	if(0 < countParticleMax)	{
		sizeTableActivity = Memory::Alignment(sizeof(Library::Control::Effect::Particle::Activity) * countParticleMax);
	}

	return(	sizeTablePattern
			+ sizeTableActivity
		);
}

/* ********************************************************* */
//! 全再生フレーム数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Effect::Emitter::FrameGetFull(void)

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Effect::Emitter::BootUp(	EntityEffect& entity,
								Library::Data::Project::Effect& dataEffect,
								int idParts,
								int indexDataEmitter,
								int indexParent,
								Library::Control::Effect& controlEffect,
								Uint8*& workArea
						)
{
	Status = FlagBitStatus::CLEAR;

	DataEffect = &dataEffect;
	IDParts = idParts;
	IndexEmitter = indexDataEmitter;
	IndexParent = indexParent;

	DataEffect->TableEmitter(&DataEmitter, IndexEmitter);

	/* Initialize Random */
	if(nullptr == Random)	{
		Random = EntityEffect::InstanceCreateRandom();
	}

	SeedRandom = controlEffect.Seed;
	Library::Data::Effect::Emitter::FlagBit flagData = DataEmitter.FlagData();
	if(0 != (flagData & Library::Data::Effect::Emitter::FlagBit::SEEDRANDOM))	{	/* Overwrite Seed */
		/* MEMO: Overwritten to the Emitter's Seed. */
		SeedRandom = (Uint32)DataEmitter.SeedRandom() + (Uint32)Library::Data::Effect::Emitter::Constant::SEED_MAGIC;
	} else {
		if(0 != (flagData & Library::Data::Project::Effect::FlagBit::SEEDRANDOM_LOCK))	{	/* Seed Locked */
			/* MEMO: Overwritten to the Effect's Seed. */
			SeedRandom = ((Uint32)DataEffect->SeedRandom() + 1) * (Uint32)Library::Data::Effect::Emitter::Constant::SEED_MAGIC;
		}
	}

	/* Get Data-Tables */
	/* MEMO: これらのテーブルはデータがないことがあり得る（乱数種がランダムだったりする場合）ので、注意。 */
	CountTablePatternOffset = DataEmitter.CountTablePatternOffset();
	TablePatternOffset = (0 >= CountTablePatternOffset) ? nullptr : DataEmitter.TablePatternOffset();

	/* MEMO: TablePatternEmitはここではnullptrです（直下で動的に生成した場合のみテーブル先頭が入ります）。 */
	CountTablePatternEmit = DataEmitter.CountTablePatternEmit();
	TablePatternEmit = nullptr;

	CountTableSeedParticle = DataEmitter.CountTableSeedParticle();
	TableSeedParticle = (0 >= CountTableSeedParticle) ? nullptr : DataEmitter.TableSeedParticle();

	/* MEMO: C#ではnullptr判定をしていますが……C++版ではいらないので、省略してあります。                */
	/*       ※Unity上で再インポートなどのタイミングによってはコケる時があったための予備処理なので……。 */
	if((0 >= CountTablePatternEmit) || (0 >=CountTableSeedParticle))	{	/* Calculate on Runtime ... Not Fixed Random-Seed */
		Library::Data::Effect::Emitter dataEffectEmitter;			/* いらなくない？ */
		DataEffect->TableEmitter(&dataEffectEmitter, IndexEmitter);

		/* テーブルの格納領域長・初期化に必要な値を取得 */
		int countEmitMax;
		int countEmit;
		size_t sizeWorkArea = dataEffectEmitter.SizeGetTablePatternEmit(&CountTablePatternEmit, &CountTableSeedParticle, &countEmitMax, &countEmit);

		/* ワークエリアを設定 */
		/* MEMO: 本来は取っておく必要はありませんが、動的にテーブルを生成したフラグ代わりにWorkAreaTableを保存しておきます。 */
		WorkAreaTable = reinterpret_cast<void*>(workArea);

		/* テーブルを作成 */
		Sint64* tableSeedParticle;
		dataEffectEmitter.TableGetPatternEmit(	TablePatternEmit,
												tableSeedParticle,
												*Random,
												SeedRandom,
												workArea,
												CountTablePatternEmit,
												CountTableSeedParticle,
												countEmitMax,
												countEmit
											);
		TableSeedParticle = tableSeedParticle;

		/* ワークの先頭を進める */
		workArea += sizeWorkArea;
	}

	/* Set Particle's UV */
	if(false == CellPresetParticle(entity))	{
		/* MEMO: 本当はgotoでまとめたいが、現状はバラバラで書いておく。 */
		ShutDown();
		return(false);
	}

	/* Create Generating-Particle WorkArea */
	int countParticleMax = DataEmitter.CountParticleMax();
	if(0 < countParticleMax)	{
		/* パーティクル生成用のテーブル領域を設定 */
		TableActivityParticle = reinterpret_cast<Library::Control::Effect::Particle::Activity*>(workArea);
		workArea += Memory::Alignment(sizeof(Library::Control::Effect::Particle::Activity) * countParticleMax);
	}
	for(int i=0; i<countParticleMax; i++)	{
		TableActivityParticle[i].CleanUp();
	}

	/* Set Duration */
	Duration = DataEmitter.DurationEmitter() + DataEmitter.Delay();

	Status |= (FlagBitStatus::VALID | FlagBitStatus::RUNNING);

	return(true);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Effect::Emitter::ShutDown(void)
{
	/* 資源の解放 */
	if(nullptr != Random)	{	/* 乱数発生器 */
		EntityEffect::InstanceReleaseRandom(Random);
	}

	/* ワークの解放 */
	/* MEMO: TablePatternEmitとTableSeedParticleは単にデータテーブルなので、デストラクト処理する必要はありません。    */
	/*       ※データによって、アニメーションデータを直接覗いている場合もあるので、うかつに解放処理しないでください。 */
	if(nullptr != TableActivityParticle)	{
		/* MEMO: 領域を解放するだけなので、何もしていません。 */
		/* MEMO: TableActivityParticleはこの後のCleanUpで消去されます。 */
	}

	if(nullptr != WorkAreaTable)	{	/* パターンテーブルが動的に作成されている */
		/* MEMO: ワークは上位で設置されているので、ここでは何も行わない。 */
	}

	CleanUp();
}

/* ********************************************************* */
//! 更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Effect::Emitter::Update(	float frame,
								EntityEffect& entity,
								Library::Control::Effect& controlEffect,
								int indexEmitterParent
						)
{
	if((0 > IndexEmitter) || (0 > IDParts))	{
		return(false);
	}

	/* パーティクルの動作状態を更新 */
	int countParticle = CountTablePatternEmit;
	int countOffset = CountTablePatternOffset;
	int frameNow = (int)frame;
	Uint32 slide = (0 > indexEmitterParent) ? 0 : (Uint32)ParticleTempolary.ID;
	slide = slide * (Uint32)Library::Data::Effect::Emitter::Constant::SEED_MAGIC;
	Uint32 indexSlide;
	if(nullptr == TablePatternEmit)	{	/* 静的エミットパターン（乱数種固定） */
		for(int i=0; i<countOffset; i++)	{
			indexSlide = ((Uint32)i + slide) % (Uint32)countParticle;
			TableActivityParticle[i].Update(	frameNow,
												*this,
												DataEmitter,
												i,
												TablePatternOffset[i],
												indexSlide
											);
		}
	} else {	/* 動的エミットパターン（乱数種不定） */
		for(int i=0; i<countOffset; i++)	{
			indexSlide = ((Uint32)i + slide) % (Uint32)countParticle;
			TableActivityParticle[i].Update(	frameNow,
												*this,
												DataEmitter,
												TablePatternEmit[i],
												TablePatternOffset[i],
												TablePatternEmit[indexSlide]
											);
		}
	}

	/* パーティクルの描画情報生成 */
	/* MEMO: 描画情報は個別パーティクルでの管理ではなく、発生させたエミッタで共通です。 */
	{
		/* シェーダの設定 */
		SpriteStudio6::Shader* shader = &Shader::BuiltInStandard;
		SettingRender.ShaderSet(shader);

		/* ステンシルテストを設定 */
		/* MEMO: エフェクトはマスクになることはないので、描画タイプは常にNORMALです。 */
		switch(controlEffect.Masking)	{
			case Library::KindMasking::Kind::MASK:
				/* MEMO: ステンシルテストあり。 */
				SettingRender.TestSetStencil(true, Renderer::SectionTest::KindTypeDraw::NORMAL);
				break;

			case Library::KindMasking::Kind::THROUGH:
			default:	/* ClangのWarning除け */
				/* MEMO: ステンシルテストなし。 */
				SettingRender.TestSetStencil(false, Renderer::SectionTest::KindTypeDraw::NORMAL);
				break;
		}

		/* 深度テストを設定 */
		bool depthRead = false;
		bool depthWrite = false;
		SettingRender.TestSetDepth(depthRead, depthWrite);

		/* ブレンド方法を設定 */
		/* MEMO: アニメーションのブレンド方法はLibrary::KindOperationとRenderer::SectionBlend::KindTypeの並びが同じなので、 */
		/*       そのまま（型だけキャストして）設定できるが、Library::KindOperationEffectは並びが違うため変換しています。   */
		static const Renderer::SectionBlend::KindType tableConvertBlend[Library::KindOperationBlendEffect::TERMINATOR] = {
			Renderer::SectionBlend::KindType::MIX,
			Renderer::SectionBlend::KindType::ADD,
		};
		Renderer::SectionBlend::KindType kindBlend = tableConvertBlend[DataEmitter.OperationBlendTarget()];
		SettingRender.BlendSet(kindBlend);

		/* 描画フラグを設定 */
		Uint32 /* Renderer::SectionBlend::FlagBitControlPixel */ flagControlPixel = Renderer::SectionBlend::FlagControlPixel[kindBlend];
		FlagsDraw.Set(	((0 != (flagControlPixel & Renderer::SectionBlend::FlagBitControlPixel::FLAG_PIXEL_DISCARD)) ? 1.0f : 0.0f),				/* 透明ピクセルを廃棄するか？ */
						((0 != (flagControlPixel & Renderer::SectionBlend::FlagBitControlPixel::FLAG_PIXEL_PREMULTIPLIED_ALPHA)) ? 1.0f : 0.0f),	/* PMAを行うか？ */
						1.0f + 0.01f,	/* 0.01fは丸め誤差防止 */																					/* パーツカラーを頂点色とみなすか？ */
						0.0f																														/* 未使用 */
					);

		/* ターゲットテクスチャの設定 */
		/* MEMO: 現在暫定で常にフレームバッファに出力しています。 */
		SettingRender.TextureSetTargetColor(0, nullptr, false, CPU::Color::Clear);
		SettingRender.TextureSetTargetTest(nullptr, false, 0, 0.0f);
		SettingRender.CountSetTextureTargetColor(1);

		/* セルマップテクスチャの設定 */
		int indexCellMap = DataCellApply.IndexCellMap;
		int indexTexture = entity.IndexTextureCellMap[indexCellMap];
		Texture* texture = entity.Texture[indexTexture];

		Library::Data::CellMap::Body dataCellMap;
		entity.DataCellMap->TableCellMap(&dataCellMap, indexCellMap);
		Texture::KindInterpolation interp = dataCellMap.Filter();
		Texture::KindWrap wrap = dataCellMap.Wrap();
		SettingRender.TextureSetCellMap(texture, interp, wrap);
	}

	/* パーティクル群の更新（軌道計算・描画） */
	countParticle = DataEmitter.CountParticleMax();
	for(int i=0; i<countParticle; i++)	{
		if(false == Particle.Exec(	(float)frameNow,
									i,
									entity,
									controlEffect,
									*this,
									TableActivityParticle[i],
									indexEmitterParent,
									ParticleTempolary
								)
		)	{	/* Draw-Limit Over */
			return(false);
		}
	}

	return(true);
}

/* ********************************************************* */
//! 更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Effect::Emitter::UpdateSubEmitters(	float frame,
											EntityEffect& entity,
											Library::Control::Effect& controlEffect,
											int indexEmitterTarget,
											Emitter& emitterTarget
										)
{
	/* Update Particles-Activity */
	/* MEMO: Slide is always 0. */
	int countParticle = CountTablePatternEmit;
	int countOffset = CountTablePatternOffset;
	if(nullptr == TablePatternEmit)	{	/* static Emit-Pattern */
		for(int i=0; i<countOffset; i++)	{
			TableActivityParticle[i].Update(	(int)frame,
												*this,
												DataEmitter,
												i,
												TablePatternOffset[i],
												i
											);
		}
	} else {	/* dynamic Emit-Pattern */
		for(int i=0; i<countOffset; i++)	{
			TableActivityParticle[i].Update(	(int)frame,
												*this,
												DataEmitter,
												TablePatternEmit[i],
												TablePatternOffset[i],
												TablePatternEmit[i]
											);
		}
	}

	/* Update Sub-Emitters */
	int frameTop;
	countParticle = DataEmitter.CountParticleMax();
	for(int i=0; i<countParticle; i++)	{
		if(0 != (TableActivityParticle[i].Status & Library::Control::Effect::Particle::Activity::FlagBitStatus::BORN))	{
			/* MEMO: "ParticleTempolary" is parent's parameter. */
			frameTop = TableActivityParticle[i].FrameStart;
			emitterTarget.ParticleTempolary.FrameStart = frameTop;
			emitterTarget.ParticleTempolary.FrameEnd = TableActivityParticle[i].FrameEnd;
			emitterTarget.ParticleTempolary.ID = i;
			emitterTarget.ParticleTempolary.IDParent = 0;

			/* CAUTION: "ParticleTempolary" will be broken. */
			if(false == emitterTarget.Update(	(frame - (float)frameTop),
												entity,
												controlEffect,
												IndexEmitter
											)
			)	{
				return(false);
			}
		}
	}

	return(true);
}

/* ********************************************************* */
//! パーティクルの使用セルを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Effect::Emitter::CellPresetParticle(EntityEffect& entity)
{
	int indexCellMap = DataCellApply.IndexCellMap;
	int indexCell = DataCellApply.IndexCell;
	if((0 > indexCellMap) || (0 > indexCell))	{
		indexCellMap = DataEmitter.IndexCellMap();
		indexCell = DataEmitter.IndexCell();
	}

	Status &= ~FlagBitStatus::CHANGE_CELL_UNREFLECTED;

	Library::Data::CellMap::Body dataCellMap;
	if(nullptr == entity.DataGetCellMap(&dataCellMap, indexCellMap))	{
		goto CellPresetParticle_ErrorEnd;
	} else {
		if((0 > indexCell) || (dataCellMap.CountTableCell() <= indexCell))	{
			goto CellPresetParticle_ErrorEnd;
		} else {
			Library::Data::CellMap::Cell dataCell;
			dataCellMap.TableCell(&dataCell, indexCell);

			DataCellApply.IndexCellMap = indexCellMap;
			DataCellApply.IndexCell = indexCell;
			CPU::Vector2 pivotCell;
			dataCell.Pivot(&pivotCell);
			CPU::Rect rectangleCell;
			dataCell.Rectangle(&rectangleCell);
			CPU::Vector2 sizeCell;
			rectangleCell.SizeGet(&sizeCell);

			/* 頂点座標 */
			float pivotXCell = pivotCell.GetX();
			float pivotYCell = pivotCell.GetY();
			float coordinateLUx = -pivotXCell;
			float coordinateLUy = pivotYCell;
			float coordinateRDx = sizeCell.GetX() - pivotXCell;
			float coordinateRDy = -(sizeCell.GetY() - pivotYCell);
			CoordinateDraw[Library::KindVertex::LU].Set(coordinateLUx, coordinateLUy, 0.0f, 1.0f);
			CoordinateDraw[Library::KindVertex::LD].Set(coordinateLUx, coordinateRDy, 0.0f, 1.0f);
			CoordinateDraw[Library::KindVertex::RU].Set(coordinateRDx, coordinateLUy, 0.0f, 1.0f);
			CoordinateDraw[Library::KindVertex::RD].Set(coordinateRDx, coordinateRDy, 0.0f, 1.0f);

			/* UV座標 */
			CPU::Vector2 sizeOriginalCellMap;
			dataCellMap.SizeOriginal(&sizeOriginalCellMap);
			float sizeXTexture = sizeOriginalCellMap.GetX();
			float sizeYTexture = sizeOriginalCellMap.GetY();
			coordinateLUx = rectangleCell.xMin / sizeXTexture;	/* L */
			coordinateRDx = rectangleCell.xMax / sizeXTexture;	/* R */
			coordinateLUy = (sizeYTexture - rectangleCell.yMin) / sizeYTexture;	/* U */
			coordinateRDy = (sizeYTexture - rectangleCell.yMax) / sizeYTexture;	/* D */
			UVTextureDraw[Library::KindVertex::LU].Set(coordinateLUx, coordinateLUy, 0.0f, 0.0f);
			UVTextureDraw[Library::KindVertex::LD].Set(coordinateLUx, coordinateRDy, 0.0f, 0.0f);
			UVTextureDraw[Library::KindVertex::RU].Set(coordinateRDx, coordinateLUy, 0.0f, 0.0f);
			UVTextureDraw[Library::KindVertex::RD].Set(coordinateRDx, coordinateRDy, 0.0f, 0.0f);
		}
	}

	return(true);

CellPresetParticle_ErrorEnd:;
	DataCellApply.CleanUp();
	return(false);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                     Effect */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::CleanUp(void)

/* ********************************************************* */
//! 乱数種の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::SeedSet(Uint32 seed)

/* ********************************************************* */
//! 乱数種オフセットの設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::SeedOffsetSet(Uint32 offsetSeed)

/* ********************************************************* */
//! エフェクトのループ再生の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Effect::StatusSetLoop(bool flagSwitch)

/* ********************************************************* */
//! エフェクト再生管理が使用するワークエリア長の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
size_t Effect::SizeGetWorkArea(EntityEffect& entity)
{
	if((nullptr == entity.DataCellMap) || (nullptr == entity.DataEffect))	{
		return(false);
	}
	Library::Data::Project::Effect& dataEffect = *entity.DataEffect;

	/* パーツ・エミッタの総合ワークを取得 */
	/* MEMO: エフェクトのパーツは、原則データを直接参照しているだけなので（パーツ毎で制御が */
	/*       必要なわけではないので）、ワークは必要ありません。                             */
//	int countParts = dataEffect.CountGetTableParts();
	int countEmitter = dataEffect.CountGetTableEmitter();
	size_t sizeParts = 0;
	size_t sizeEmitter = Memory::Alignment(sizeof(Emitter) * countEmitter);

	/* エミッタ毎に必要なワーク長を取得 */
	/* MEMO: エミッタは制御以外に生成するパーティクルの生成・生存状況の事前計算領域が必要に */
	/*       なるため、個々のエミッタで必要なワークのサイズが異なります。                   */
	size_t sizeEmitterEach = 0;
	for(int i=0; i<countEmitter; i++)	{
		sizeEmitterEach += Emitter::SizeGetWorkArea(entity, dataEffect, i);
	}

	return(	sizeParts
			+ sizeEmitter
			+ sizeEmitterEach
		);
}

/* ********************************************************* */
//! エフェクト再生管理が使用するワークエリア長の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 関数値はエミッタ毎のワークの先頭（与えられた領域から自身が
 * 必要とするワーク分を消費した後の先頭ポインタ）です。
 */
void* Effect::BootUpWorkArea(EntityEffect& entity, void* workArea)
{
	Library::Data::Project::Effect& dataEffect = *entity.DataEffect;
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(workArea);

	/* パーツ・エミッタの総合ワークを取得 */
	/* MEMO: エフェクトのパーツは、原則データを直接参照しているだけなので（パーツ毎で制御が */
	/*       必要なわけではないので）、ワークは必要ありません。                             */
//	int countParts = dataEffect.CountGetTableParts();
	int countEmitter = dataEffect.CountGetTableEmitter();
//	size_t sizeParts = 0;
	size_t sizeEmitter = Memory::Alignment(sizeof(Emitter) * countEmitter);

	/* エミッタテーブルの初期化 */
	if((nullptr == TableEmitter) || (countEmitter != CountTableEmitter))	{
		TableEmitter = reinterpret_cast<Emitter*>(pointWorkAreaNow);
			pointWorkAreaNow += sizeEmitter;
		CountTableEmitter = countEmitter;
		for(int i=0; i<countEmitter; i++)	{
			/* MEMO: 一応全てコンストラクタを走らせておきます。 */
			new(&TableEmitter[i]) Emitter;
		}
	}

	return(pointWorkAreaNow);
}

/* ********************************************************* */
//! エフェクト再生管理が使用するワークエリア長の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Effect::ShutDownWorkArea(void)
{
	/* MEMO: ここではデストラクトまでで、ワーク領域の解放は上位が行います。 */
	/* MEMO: 各エミッタ毎のワークのデストラクトはEffect::Emitter::ShutDownでやっています。 */
	/* エミッタテーブルの初期化 */
	if(nullptr != TableEmitter)	{
		for(int i=0; i<CountTableEmitter; i++)	{
			/* MEMO: ShutDownはすでに終わっているので、デストラクタだけ走らせています（BootUpの配置newのカウンタ）。 */
			TableEmitter[i].~Emitter();
		}
	}
	TableEmitter = nullptr;
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Effect::BootUp(EntityEffect& entity, void* workArea)
{
	if((nullptr == entity.DataCellMap) || (nullptr == entity.DataEffect))	{
		return(false);
	}
	Library::Data::Project::Effect& dataEffect = *entity.DataEffect;
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(workArea);

	/* ワークの確保・初期起動 */
	/* MEMO: pointWorkAreaNowには、各エミッタ毎での追加ワークに使用するメモリの先頭が返ってきます。 */
	pointWorkAreaNow = reinterpret_cast<Uint8*>(BootUpWorkArea(entity, pointWorkAreaNow));
	if(nullptr == pointWorkAreaNow)	{
		return(false);
	}

	/* マスキングの設定 */
	/* MEMO: とりあえず最初はスルー（マスキングなし）にしておく */
	Masking = Library::KindMasking::THROUGH;

	/* エミッタの起動 */
	Status = FlagBitStatus::CLEAR;
	SeedSet(entity.KeyCreateRandom());

	int indexEmitter;
	int indexEmitterParent;
	Library::Data::Parts::Effect dataPartsEffect;
	Library::Data::Parts::Effect dataPartsEffectParent;
	Library::Data::Effect::Emitter dataEmitterEffect;
	bool flagInfinite = false;

	int countParts = dataEffect.CountGetTableParts();
	int countEmitter = dataEffect.CountGetTableEmitter();

	/* MEMO: 最親パーツ（Root）はエミッタを持たないことに注意してください。 */
	for(int i=1; i<countParts; i++)	{
		dataEffect.TableParts(&dataPartsEffect, i);

		/* 親パーツの特定 */
		indexEmitter = dataPartsEffect.IDParent();	/* テンポラリ */
		if(0 >= indexEmitter)	{	/* 最親パーツ */
			indexEmitterParent = -1;
		} else {	/* 子パーツ */
			dataEffect.TableParts(&dataPartsEffectParent, indexEmitter);
			indexEmitterParent = dataPartsEffectParent.IndexEmitter();
		}

		/* エミッタ管理の起動 */
		/* MEMO: 次のエミッタ用のワークエリアの先頭はpointWorkAreaNowに上書きされてきます。 */
		indexEmitter = dataPartsEffect.IndexEmitter();
		TableEmitter[indexEmitter].CleanUp();
		if(false == TableEmitter[indexEmitter].BootUp(	entity,
														dataEffect,
														i,
														indexEmitter,
														indexEmitterParent,
														*this,
														pointWorkAreaNow
													)
		)	{
			goto BootUp_ErrorEnd;
		}

		/* 再生状態取得 */
		dataEffect.TableEmitter(&dataEmitterEffect, indexEmitter);
		flagInfinite |= (0 != (dataEmitterEffect.FlagData() & Library::Data::Effect::Emitter::FlagBit::EMIT_INFINITE)) ? true : false;
	}

	entity.StatusSetIsPlayingInfinity(flagInfinite);
	Status |= (true == flagInfinite) ? FlagBitStatus::INFINITE_EMIT : FlagBitStatus::CLEAR;
	Status |= (true == dataEffect.StatusIsLockSeedRandom()) ? FlagBitStatus::LOCKSEED : FlagBitStatus::CLEAR;

	/* エフェクト再生時間の取得 */
	int frameFull;
	DurationFull = 0;
	for (int i=0; i<countEmitter; i++)	{
		indexEmitterParent = TableEmitter[i].IndexParent;
		frameFull = TableEmitter[i].FrameGetFull();
		if(0 <= indexEmitterParent)	{	/* Sub-Emitters */
			frameFull += TableEmitter[indexEmitterParent].FrameGetFull();
		}
		TableEmitter[i].FrameGlobal = frameFull;

		DurationFull = (frameFull > DurationFull) ? frameFull : DurationFull;
	}

	/* 描画トランスフォームの初期化 */
	MatrixRoot.Set(FPU::Matrix4x4::Unit);

	/* 動作状態ステータスの設定 */
	Status |= FlagBitStatus::RUNNING;

	return(true);

BootUp_ErrorEnd:;
	CleanUp();

	return(false);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Effect::ShutDown(void)
{
	if(0 != (Status & FlagBitStatus::RUNNING))	{	/* 動作中 */
		/* エミッタ制御の解放 */
		/* MEMO: 解放する時はパーツを経由しないで、直接エミッタ制御を解放しています。 */
		/*       ※解放時は親子関係での処理順序が関係しません。                       */
		if(nullptr != TableEmitter)	{
			for(int i=0; i<CountTableEmitter; i++)	{
				TableEmitter[i].ShutDown();
			}
		}

		/* ワークエリアの終了処理 */
		ShutDownWorkArea();
	}

	CleanUp();
}

/* ********************************************************* */
//! 更新
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Effect::Update(	EntityEffect& entity,
						Library::KindMasking::Kind masking,
						const Library::Control::Animation::ArgumentContainer& argumentUpdate,
						bool flagPlanarization,
						const FPU::Matrix4x4& matrixParent
				)
{
	if(nullptr == TableEmitter)	{
		/* MEMO: 初期化前に飛んできている場合は、何もしないで帰る。 */
		return;
	}

	/* エミッタの変形行列を作成 */
	Float32 scaleZ = (true == flagPlanarization) ? 0.0f : 1.0f;
	FPU::Matrix4x4 matrix;
	MatrixCreateTRS(&matrix, FPU::Vector3::Zero, FPU::Vector3::Zero, FPU::Vector3(entity.RateScaleLocalForce.GetX(), entity.RateScaleLocalForce.GetY(), scaleZ));
	MatrixMul(&MatrixRoot, matrix, matrixParent);

	/* エミッタの乱数種の再設定 */
	int frame = (int)entity.Frame;
	int frameTarget = frame;
	int countLoop = 0;
	if(0 == (Status & FlagBitStatus::INFINITE_EMIT))	{
		if(0 != (Status & FlagBitStatus::LOOP))	{
			if(frame > DurationFull)	{
				frameTarget = frame % DurationFull;
				countLoop = frame / DurationFull;
				SeedOffsetSet((Uint32)countLoop);
			}
		}
	}

	/* マスキングとセル情報の更新 */
	bool flagUpdateCell = entity.StatusGetIsChangeCellMap();
	if(Library::KindMasking::FOLLOW_DATA == masking)	{
		masking = Library::KindMasking::THROUGH;
	}
	if(Masking != masking)	{
		Masking = masking;
	}

	/* エミッタ毎の更新 */
	const int* tableIndexEmitter = entity.DataEffect->TableIndexEmitterOrderDraw();
	int countEmitter = entity.DataEffect->CountTableIndexEmitterOrderDraw();
	int indexEmitter;
	int indexEmitterParent;
	bool flagDrawAll;
	for(int i=0; i<countEmitter; i++)	{
		indexEmitter = tableIndexEmitter[i];

		/* セルの更新 */
		if((true == flagUpdateCell) || (0 != (TableEmitter[indexEmitter].Status & Emitter::FlagBitStatus::CHANGE_CELL_UNREFLECTED)))	{
			TableEmitter[indexEmitter].CellPresetParticle(entity);
		}

		/* 乱数種オフセットの更新 */
		TableEmitter[indexEmitter].SeedOffset = SeedOffset;

		/* エミッタの更新 */
		indexEmitterParent = TableEmitter[indexEmitter].IndexParent;
		if(0 <= indexEmitterParent)	{   /* 親エミッタがいる */
			flagDrawAll = TableEmitter[indexEmitterParent].UpdateSubEmitters(	(float)frameTarget,
																				entity,
																				*this,
																				indexEmitter,	// indexEmitterParent,
																				TableEmitter[indexEmitter]
																			);
		} else {	/* 最親エミッタ */
			flagDrawAll = TableEmitter[indexEmitter].Update(	(float)frameTarget,
																entity,
																*this,
																-1
															);
		}

		if(false == flagDrawAll)	{	/* Draw-Limit Over */
			/* MEMO: 描画量オーバーの場合は（どうせ）そこから後ろのパーティクルも描画できないので、更新をキャンセルして帰る。 */
			/*       ※単に無駄処理を飛ばしているだけです。                                                                   */
			return;
		}
	}
	return;
}


/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* Control */
}	/* Library */
}	/* SpriteStudio6 */
