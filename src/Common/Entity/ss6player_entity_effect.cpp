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
#include "../Project/ss6player_project.h"
#include "../PrimitiveBatcher/ss6player_primitive_batcher.h"

#include "ss6player_entity.h"

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
/* Effect関係 */
Uint32 EntityEffect::RandomKeyMakeID = 123456;

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
/*                                                               EntityEffect */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
EntityEffect::EntityEffect(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
EntityEffect::~EntityEffect(void)
{
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::CleanUp(void)
{
	base::CleanUp();
	ModelDraw = nullptr;

	Status = FlagBitStatus::CLEAR;

	CleanUpWorkAreaProject();
	CleanUpWorkAreaData();

	CallBackPlayEnd.CleanUp();
}

/* ********************************************************* */
//! プロジェクト共通ワークエリア未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::CleanUpWorkAreaProject(void)
{
	DataEffect = nullptr;

	WorkAreaProject = nullptr;
	WorkAreaSetTexture(0, nullptr);

	LimitParticleDraw = Defaults::LIMIT_PARTICLEDRAW;
	CountParticleDraw = 0;
}

/* ********************************************************* */
//! アニメーションパック依存ワークエリア未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::CleanUpWorkAreaData(void)
{
	WorkAreaData = nullptr;

	IndexData = -1;

	StatusPlaying = Library::Control::Animation::Track::FlagBitStatus::CLEAR;
	TimePerFrame = 0.0f;
	TimeElapsed = 0.0f;
	RateTime = 1.0f;

	Frame = 0.0f;
	FrameRange = 0.0f;
	FramePerSecond = 0.0f;

	ControlEffect.CleanUp();
}

/* ********************************************************* */
//! プロジェクト共通ワークエリア起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::BootUpWorkAreaProject(int countTexture)
{
	/* MEMO: このワークエリアには、原則「プロジェクト（SSPJ）を変更しない限り */
	/*       構成が変わらない」ものが配置されています。                       */
	/*       - テクスチャ（描画使用・オフスクリーン）                         */
	const size_t sizeShaderData = sizeof(ShaderData);
	const size_t sizeTexture = base::SizeGetWorkAreaTexture(countTexture);
	size_t sizeWorkArea = sizeTexture;
	if(nullptr == EntityParent)	{
		/* MEMO: 最親の時にはシェーダ定数を持っています。 */
		sizeWorkArea += sizeShaderData;
	}

	/* ヒープ取得 */
	Memory::Allocate(&WorkAreaProject, sizeWorkArea);
	if(nullptr == WorkAreaProject)	{
		return(false);
	}
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(WorkAreaProject);

	/* テクスチャ領域の起動 */
	WorkAreaSetTexture(countTexture, pointWorkAreaNow);
		pointWorkAreaNow += sizeTexture;

	/* シェーダ定数の起動 */
	if(nullptr == EntityParent)	{
		ConstantShader = reinterpret_cast<ShaderData*>(pointWorkAreaNow);
			pointWorkAreaNow += sizeShaderData;

		/* MEMO: シェーダ定数は起動までしておく。 */
		new(ConstantShader) ShaderData;
		ConstantShader->BootUp();
	} else {
		ConstantShader = EntityParent->ConstantShader;
	}

	return(true);
}

/* ********************************************************* */
//! アニメーションパック依存ワークエリア起動
/* --------------------------------------------------------- */
/* MEMO:
 * この関数はPackSetの中からIndexPack・DataAnimationの設定が
 * 完了した段階で呼び出してください。
 */
bool EntityEffect::BootUpWorkAreaData(void)
{
	/* MEMO: このワークエリアには、原則「再生するエフェクトデータ（SSEE）が変更されると */
	/*       構成が変わる」ものが配置されています（主にパーツ構成に影響を受けるもの）。 */

	return(true);
}

/* ********************************************************* */
//! プロジェクト共通ワークエリア終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::ShutDownWorkAreaProject(void)
{
	/* テクスチャ領域の解放 */
	int countTexture = CountTexture;
	if(nullptr != Texture)	{
		for(int i=0; i<countTexture; i++)	{
			Texture[i] = nullptr;
		}
	}
	if(nullptr != IndexTextureCellMap)	{
		for(int i=0; i<countTexture; i++)	{
			IndexTextureCellMap[i] = -1;
		}
	}

	/* シェーダ定数の解放 */
	if(nullptr == EntityParent)	{
		/* MEMO: シェーダ定数は終了まで行う。 */
		if(nullptr != ConstantShader)	{
			ConstantShader->ShutDown();
			ConstantShader->~ShaderData();
		}
	}
	ConstantShader = nullptr;

	/* MEMO: 色々なワーク配列などのクリアは末尾のCleanUpWorkAreaProjectで一括してやっています。 */

	/* ヒープ解放 */
	if(nullptr != WorkAreaProject)	{
		Memory::Free(WorkAreaProject);
	}
}

/* ********************************************************* */
//! アニメーションパック依存ワークエリア終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::ShutDownWorkAreaData(void)
{
}

/* ********************************************************* */
//! ステータス関連
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool EntityEffect::StatusGetIsValid(void)
// bool EntityEffect::StatusGetIsPlaying(void)
// bool EntityEffect::StatusGetIsPlayingInfinity(void)
// void EntityEffect::StatusSetIsPlayingInfinity(bool flagSwitch)
// bool EntityEffect::StatusGetIsUpdateRateScaleLocal(void)
// bool EntityEffect::StatusGetIsUpdateRateOpacity(void)
// bool EntityEffect::StatusGetIsChangeTableMaterial(void)
// bool EntityEffect::StatusGetIsChangeCellMap(void)

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::IsValid(void)
{
	/* MEMO: BootUpとDataSetが行われているかをチェックしています。 */
	return((0 != (Status & FlagBitStatus::VALID)) && (0 <= DataGet()));	/* ? true : false */
}

/* ********************************************************* */
//! 本クラスの起動（初期起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::BootUp(Project& project, Entity* entityParent)
{
	if(false == project.IsValid())	{
		return(false);
	}

	/* 基底を起動 */
	base::BootUp();

	/* 親エンティティを設定 */
	/* MEMO: ワークエリアの初期化時にEntityParentを見ているので、先に設定して */
	/*       おかなくてはならないことに注意してください。                     */
	EntityParent = entityParent;

	/* プロジェクトの持つ情報を定義 */
	DataProject = &project;
	DataCellMap = &project.CellMap;

	/* コールバック群 */
	CallBackPlayEnd.CleanUp();

	/* ワークエリアの確保・設定 */
	/* MEMO: CountTextureは、本関数の中で設定されて帰ってきます。 */
	if(false == BootUpWorkAreaProject(DataProject->CountGetTexture()))	{
		goto BootUp_ErrorEnd;
	}

	/* オフスクリーンテクスチャの設定 */
	/* MEMO: エフェクトが単体でオフスクリーンテクスチャを扱うことはないので何もしない（呼出元のアニメーション任せ）。 */

	/* 使用テクスチャ群・セルマップとテクスチャ変換テーブルの設定 */
	if(false == DataProject->TextureBootUp(Texture, IndexTextureCellMap))	{
		goto BootUp_ErrorEnd;
	}

	/* 初期化終了 */
	Status |= FlagBitStatus::VALID;

	return(true);

BootUp_ErrorEnd:;
	ShutDown();

	return(false);
}

/* ********************************************************* */
//! 本クラスの終了（未使用化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::ShutDown(void)
{
	/* 再生データの解放 */
	DataRelease();

	/* ワークエリアの解放 */
	ShutDownWorkAreaProject();

	/* 未使用化 */
	/* MEMO: CleanUpでまとめてクリアされます。 */
//	CleanUpWorkAreaProject();

	/* 基底を終了 */
	base::ShutDown();

	CleanUp();
}

/* ********************************************************* */
//! 再生状態の定常更新
/* --------------------------------------------------------- */
/* MEMO:
 * エフェクトでは実質本関数は何もしません。
 * ※現在、エフェクトの更新時はDrawのタイミングで行っています。
 */
bool EntityEffect::Update(float timeElapsed, Library::KindMasking::Kind masking)
{
	if(nullptr == EntityParent)	{
		/* MEMO: 自身が最親である場合にのみ実行します。                                                       */
		/*       アニメーション（親）から呼び出されている場合は、親の内部処理から直接UpdateMainが呼ばれます。 */

		/* 描画用情報コンテナの初期化 */
		/* MEMO: 毎ループ内容はクリアして使用します。 */
		ArgumentContainerControl.CleanUp();

		/* アニメーション更新＋描画モデル作成 */
		UpdateMain(	timeElapsed,
					false,
					Library::KindMasking::FOLLOW_DATA,
					ArgumentContainerControl,
					true,
					FlagPlanarization,
					FPU::Matrix4x4::Unit	/* 最親は単位行列 */
				);
	}
	return(false);
}
bool EntityEffect::UpdateMain(	float timeElapsed,
								bool flagHideDefault,
								Library::KindMasking::Kind masking,
								Library::Control::Animation::ArgumentContainer& argumentUpdate,
								bool flagInitializeMatrixCorrection,
								bool flagPlanarization,
								const FPU::Matrix4x4& matrixParent
							)
{
	if(false == IsValid())	{	/* エフェクト再生できる状態でない（未初期化状態） */
		return(false);
	}
	if(false == DataCellMap->IsValid())	{	/* セルマップデータが異常 */
		return(false);
	}
	if(false == DataEffect->IsValid())	{	/* エフェクトデータが異常 */
		return(false);
	}

	bool flagHide = flagHideDefault | FlagHideForce;

	/* 再生状態の更新 */
	if(0 == (StatusPlaying & Library::Control::Animation::Track::FlagBitStatus::PLAYING))	{	/* 再生していない */
		Status &= ~FlagBitStatus::PLAYING;

		return(false);
	}

	Status |= FlagBitStatus::PLAYING;
	TimeElapsed += (	(0 != (StatusPlaying & Library::Control::Animation::Track::FlagBitStatus::PAUSING))
						|| (0 != (StatusPlaying & Library::Control::Animation::Track::FlagBitStatus::PLAYING_START))
					) ? 0.0f : (timeElapsed * RateTime);
	Frame = TimeElapsed * FramePerSecond;
	if(0 != (Status & FlagBitStatus::PLAYING_INFINITY))	{	/* 独立再生 */
//		Frame %= FrameRange;
	} else {	/* 連動再生 */
		/* 有効フレーム数内にクランプ */
		/* MEMO: ラップアラウンドではありません。 */
		if(FrameRange < Frame)	{
			Frame = FrameRange;
		} else {
			if(0.0f > Frame)	{
				Frame = 0.0f;
			}
		}
	}

	/* エミッタ＆パーティクル計算＆描画 */
	CountParticleDraw = 0;	/* 描画済パーティクル数クリア */
	if(false == flagHide)	{
		/* 更新＋描画 */
		ControlEffect.Update(	*this,
								masking,
								argumentUpdate,
								flagPlanarization,
								matrixParent
						);
	}

	/* ステータスの更新 */
	StatusPlaying &= ~Library::Control::Animation::Track::FlagBitStatus::PLAYING_START;
	Status &= ~(	FlagBitStatus::UPDATE_RATE_SCALELOCAL
					| FlagBitStatus::UPDATE_RATE_OPACITY
					| FlagBitStatus::CHANGE_TABLEMATERIAL
					| FlagBitStatus::CHANGE_CELLMAP
				);

	return(true);
}

/* ********************************************************* */
//! 描画関数
/* --------------------------------------------------------- */
/* MEMO:
 * 現在、本関数がエフェクトの更新のメインです。
 * 
 */
TypeDrawCommandList EntityEffect::Draw(	TypeDrawCommandList commandList,
										const CPU::Matrix4x4& matrixProjection,
										const CPU::Matrix4x4& matrixView,
										const CPU::Matrix4x4& matrixWorld
									)
{
//	if(false == DataCellMap->IsValid())	{	/* セルマップデータが異常 */
//		return(commandList);
//	}
	if(false == IsValid())	{	/* エフェクト再生できる状態でない（未初期化状態） */
		return(commandList);
	}

	if(nullptr == EntityParent)	{
		/* MEMO: 自身が最親である場合にのみ実行します。                                                     */
		/*       アニメーション（親）から呼び出されている場合は、親の内部処理から直接DrawMainが呼ばれます。 */

		/* 描画用シェーダ定数を設定 */
		/* MEMO: シェーダ定数バッファは基本的に最親エンティティにのみ存在し、子エ */
		/*       ンティティ群は、最親のものを共用しています。                     */
		/*       ※現時点では全スプライトで共通のシェーダ定数を使用しています。   */
		/*       ※逆に言うと、子アニメーション（インスタンス）であっても、シェー */
		/*         ダ定数はエンティティの中で閉じているので、エンティティである限 */
		/*         りシェーダ定数は個別で持つ必要があります（現状）。             */
		/*       ※パーツ毎のシェーダ定数的なものは、頂点データに入れています。   */
		/*         パーツ毎でシェーダ定数を切り替えるとGPUのキャッシュフラッシュ  */
		/*         が起こってスタック期間が生じるプラットフォームなどがあるので、 */
		/*         それを回避するために頂点データに編入しています。               */
		/* MEMO: 本来はnullptrチェックをすべきですが、IsValidがtrueの時（BootUpが */
		/*       終了している時）は必ず存在しているため、省略しています。         */
//		if(nullptr != ConstantShader)	{
			ConstantShader->MatrixSetProjection(matrixProjection);
			ConstantShader->MatrixSetView(matrixView);
			ConstantShader->MatrixSetWorld(matrixWorld);
			ConstantShader->DataSet();
//		}

		/* バッチングされたプリミティブ群を描画 */
		/* MEMO: 子エンティティ（インスタンス・エフェクトなど）は最親エンティティの描画モデルに集積 */
		/*       されているので、最親エンティティしか描画処理をしないことに注意してください。       */
		TypeDrawCommandList commandListNew = ModelDraw->Draw(commandList, *ConstantShader);
		if(DrawCommandListInvalid == commandListNew)	{	/* 失敗 */
			return(DrawCommandListInvalid);
		}
		commandList = commandListNew;
	}

	return(commandList);
}

/* ********************************************************* */
//! アニメーションパックを設定（パーツモデルの設定）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::DataSet(int index)
{
	if(false == DataProject->IsValid())	{
		goto DataSet_ErrorEnd;
	}
	if(DataProject->Data.CountDataEffect() <= index)	{
		goto DataSet_ErrorEnd;
	}

	if(IndexData == index)	{	/* 今と同じデータ */
		return(true);
	}

	/* 再生データの解放 */
	/* MEMO: プロジェクト依存のワークを解放・破棄したらだめです。 */
	DataRelease();

	/* エフェクトデータ依存処理を起動 */
	if(0 <= index)	{
		/* エフェクト情報を取得 */
		IndexData = index;
		DataEffect = &DataProject->Effect[index];
		if(false == DataEffect->IsValid())	{
			goto DataSet_ErrorEnd;
		}

		/* 追加ワークエリアの確保 */
		size_t sizeWorkArea = 0;
		void* pointerWorkArea = nullptr;
		sizeWorkArea += Library::Control::Effect::SizeGetWorkArea(*this);
		if(0 < sizeWorkArea)	{
			pointerWorkArea = Memory::Allocate(&WorkAreaData, sizeWorkArea);
			if(nullptr == pointerWorkArea)	{
				goto DataSet_ErrorEnd;
			}
		} else {
			WorkAreaData = nullptr;
		}

		/* ワークエリアの確保・設定 */
		if(false == BootUpWorkAreaData())	{
			goto DataSet_ErrorEnd;
		}

		/* エフェクト再生制御の起動 */
		/* MEMO: ここ、ワークを渡す必要あり。 */
		if(false == ControlEffect.BootUp(*this, pointerWorkArea))	{
			goto DataSet_ErrorEnd;
		}

		/* 再生最大フレーム数の設定 */
		FrameRange = (Float32)ControlEffect.DurationFull;
	}

	/* 描画モデルの初期起動 */
	if(false == ModelSetDraw())	{	/* 失敗 */
		goto DataSet_ErrorEnd;
	}

	return(true);

DataSet_ErrorEnd:;
	DataRelease();
	return(false);
}
bool EntityEffect::ModelSetDraw(void)
{
	/* 描画用モデルの初期起動 */
	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		/* 使用最大頂点数を取得 */
		int countMaterialMax;
		int countVertexMax = CountGetVertexDraw(&countMaterialMax);
		if(0 > countVertexMax)	{	/* エラー */
			return(false);
		}

		/* 現在の描画モデルの状態と比較 */
		/* MEMO: 描画モデルが既存の場合、最大フラグメント数と最大頂点数が要求スペックを満たすなら、既得バッファは流用する。 */
		bool flagBootUp = true;
		if(nullptr != ModelDraw)	{	/* 描画モデルが既存 */
			if(true == ModelDraw->IsValid())	{	/* 描画モデルが有効 */
				if((ModelDraw->CountVertexMax < countVertexMax) || (ModelDraw->CountFragmentMax < countMaterialMax))	{	/* 既存のバッファで事足りる */
					flagBootUp = false;
				}
			}
		}

		/* 描画モデルを初期起動 */
		if(true == flagBootUp)	{
			if(false == ModelDraw->BootUp(countMaterialMax, countVertexMax))	{
				return(false);
			}
		}
	} else {
		ModelDraw = EntityParent->ModelDraw;
	}

	return(true);
}

/* ********************************************************* */
//! アニメーションパックを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int EntityEffect::DataGet(void)

/* ********************************************************* */
//! アニメーションパックの解放
/* --------------------------------------------------------- */
/* MEMO:
 * どちらかというと役割は（アニメーションパック毎に構成が決定されている）パーツのワークを解放することです。
 */
void EntityEffect::DataRelease(void)
{
	if(0 <= IndexData)	{	/* データが割当っている */
		/* パーツ制御の解放 */
		ControlEffect.ShutDown();

		/* ワークエリアの解放 */
		ShutDownWorkAreaData();

		/* 追加ワークエリアの解放 */
		if(nullptr != WorkAreaData)	{
			/* MEMO: WorkAreaDataはCleanUpWorkAreaDataでクリアされます。 */
			Memory::Free(WorkAreaData);
		}
	}

	/* 未使用化 */
	CleanUpWorkAreaData();
}

/* ********************************************************* */
//! 現在設定しているエフェクトで使用する最大描画頂点数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int EntityEffect::CountGetVertexDraw(int* countFragmentMax)
{
	/* 最大描画パーティクル数を取得 */
	/* MEMO: 現在は固定値を返していますが、後に事前計算などで最大パーティクルが求められるようになった場合に */
	/*       本関数は任意値を返します。                                                                     */
	int countVertex = ((0 == LimitParticleDraw) ? (int)Defaults::LIMIT_PARTICLEDRAW : LimitParticleDraw);

	/* フラグメント数を設定 */
	/* MEMO: 一瞬エミッタ数で良い気がしますが、エミッタが複数量産されるのでそういう場合に足りなくなるので注意。 */
	/*       ※今は（かなり無駄だが）パーティクルの個数分だけ用意してしまう。                                   */
	*countFragmentMax = countVertex;

	/* MEMO: エフェクトの描画パーティクルは2三角形分割です。 */
	return(countVertex * PrimitiveBatcher::CountGetVertex(Library::KindVertex::TERMINATOR2));
}

// void EntityEffect::TimeSkip(Float32 time, bool flagReverseParent)
// Float32 EntityEffect::TimeGetFramePosition(int frame)
// void EntityEffect::SeedOffsetSet(Uint32 seed)

/* ********************************************************* */
//! 乱数発生器の作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Library::Utility::Random::Generator* EntityEffect::InstanceCreateRandom(void)
{
#if 0	/* MEMO: 直接newで書いてしまう場合 */
	RandomGenerator* instance = new RandomGenerator();
	if(nullptr == instance)	{
		return(nullptr);
	}
	instance->Pointer = nullptr;
	return(instance);
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
	void* handleInstance;
	RandomGenerator* instance = reinterpret_cast<RandomGenerator*>(Memory::Allocate(&handleInstance, sizeof(RandomGenerator)));
	if(nullptr == instance)	{
		return(nullptr);
	}
	new(instance) RandomGenerator;
	instance->Pointer = handleInstance;
	return(instance);
#endif
}

/* ********************************************************* */
//! 乱数発生器の解放
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::InstanceReleaseRandom(Library::Utility::Random::Generator* random)
{
	if(nullptr == random)	{
		return;
	}

#if 0	/* MEMO: 直接newで書いてしまう場合 */
	delete random;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
	void* handleInstance = random->Pointer;
	random->~Generator();
	if(nullptr != handleInstance)	{
		Memory::Free(handleInstance);
	}
#endif
}

/* ********************************************************* */
//! 乱数キーの作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Uint32 EntityEffect::KeyCreateRandom(void)

/* ********************************************************* */
//! ローカルスケール係数を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 直接RateScaleLocalForceを操作しないでください。
 */
void EntityEffect::RateSetScaleLocal(float scaleX, float scaleY)
{
	RateScaleLocalForce.Set(scaleX, scaleY);
	Status |= FlagBitStatus::UPDATE_RATE_SCALELOCAL;
}

/* ********************************************************* */
//! ローカルスケール係数を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 直接RateOpacityForceを操作しないでください。
 */
void EntityEffect::RateSetOpacity(float rate)
{
	RateOpacityForce = rate;
	Status |= FlagBitStatus::UPDATE_RATE_OPACITY;
}


/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
