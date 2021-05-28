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
#include "ss6player_entity_effect.h"
#include "../Library/ss6player_utility.h"
#include "../Project/ss6player_project.h"

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
//! エンティティが使用可能かの状態を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::StatustGetValidBootUp(void)
{
	return(0 != (Status & FlagBitStatus::VALID));	/* ? true : false */
}

/* ********************************************************* */
//! 動的パーツカラーパラメータの生成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Library::Control::AdditionalColor* Entity::AdditionalColorCreate(bool flagInvolveChildren)
{
	Library::Control::AdditionalColor* additionalColor = AdditionalColor;
	if(nullptr == additionalColor)	{	/* 定義されていない */
		/* 動的パーツカラーの実体を作成 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		additionalColor = new Library::Control::AdditionalColor();
		if(nullptr == additionalColor)	{
			return(nullptr);
		}
		HandleAdditionalColor = nullptr;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		void* handleAdditionalColor;
		additionalColor = reinterpret_cast<Library::Control::AdditionalColor*>(Memory::Allocate(&handleAdditionalColor, sizeof(Library::Control::AdditionalColor)));
		if(nullptr == additionalColor)	{
			return(nullptr);
		}
		new(additionalColor) Library::Control::AdditionalColor;
		HandleAdditionalColor = handleAdditionalColor;
#endif
		additionalColor->BootUp();

		if(true == flagInvolveChildren)	{
			/* MEMO: 自身含めて子にも設定 */
			AdditionalColorSet(additionalColor, nullptr);
		} else {
			/* MEMO: 自身だけに設定 */
			AdditionalColor = additionalColor;
		}

		/* MEMO: ワークのハンドルは一旦全部クリアされて帰ってくるので、取得した当事者だけ保存しておく。 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		HandleAdditionalColor = nullptr;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		HandleAdditionalColor = handleAdditionalColor;
#endif
	}

	return(additionalColor);
}
void Entity::AdditionalColorSet(Library::Control::AdditionalColor* additionalColorNew, Library::Control::AdditionalColor* additionalColorCheck)
{
	/* 自身に動的パーツカラーを設定 */
	/* MEMO: additionalColorCheckがnullptrでない時は、同じポインタ値を持っている場合のみ書き換えます。 */
	if(nullptr != additionalColorCheck)	{	/* 同値比較更新時 */
		if(AdditionalColor == additionalColorCheck)	{
			AdditionalColor = additionalColorNew;
		}
	} else {	/* 無条件更新時 */
		AdditionalColor = additionalColorNew;
	}
	HandleAdditionalColor = nullptr;	/* ここでは一旦全部クリアしておく */

	/* 子エンティティ群に動的パーツカラーを設定 */
	if(true == IsValid())	{	/* BootUp済＋PackSet済 */
		int count;
		const Uint16* tableIndexParts = nullptr;
		int indexParts;
		int indexEntity;

		/* パーツカタログを取得 */
		Library::Data::Parts::Animation::Catalog catalog;
		if(nullptr == DataAnimation->CatalogParts(&catalog))	{	/* パーツカタログがない */
			return;
		}

		/* インスタンスパーツを走査 */
		count = catalog.CountTableIDPartsInstance();
		if(0 < count)	{	/* インスタンスパーツを所持 */
			Entity* entityChild = nullptr;
			tableIndexParts = catalog.TableIDPartsInstance();
			for(int i=0; i<count; i++)	{
				/* 子エンティティを取得 */
				indexParts = (int)((unsigned int)tableIndexParts[i]);
				indexEntity = TableControlParts[indexParts].IndexEntityUnderControl;
				if(0 <= indexEntity)	{
					/* MEMO: インスタンスは子の子（孫）以下を持てるので、再帰的に呼び出しています。 */
					entityChild = &ListEntityChildInstance[indexEntity];
					entityChild->AdditionalColorSet(additionalColorNew, additionalColorCheck);
				}
			}
		}

		/* エフェクトパーツを走査 */
		count = catalog.CountTableIDPartsEffect();
		if(0 < count)	{	/* エフェクトパーツを所持 */
			EntityEffect* entityChild = nullptr;
			tableIndexParts = catalog.TableIDPartsEffect();
			for(int i=0; i<count; i++)	{
				/* 子エンティティを取得 */
				indexParts = (int)((unsigned int)tableIndexParts[i]);
				indexEntity = TableControlParts[indexParts].IndexEntityUnderControl;
				if(0 <= indexEntity)	{
					/* MEMO: 現在はエフェクトはAdditionalColorSetを持っていないので、直接入れてしまっています。 */
					/*       エフェクトは子を持てないので、これで実用上は問題ない（アクセサ的に問題があるかとは */
					/*       思うが……）。                                                                     */
					entityChild = &ListEntityChildEffect[indexEntity];
					if(nullptr != additionalColorCheck)	{	/* 同値比較更新時 */
						if(entityChild->AdditionalColor == additionalColorCheck)	{
							entityChild->AdditionalColor = additionalColorNew;
						}
					} else {	/* 無条件更新時 */
						entityChild->AdditionalColor = additionalColorNew;
					}
				}
			}
		}
	}
}

/* ********************************************************* */
//! 動的パーツカラーパラメータの解放
/* --------------------------------------------------------- */
/* MEMO:
 * 解放にまつわる仕様がfor Unityと変わっているのは、C++とC#の
 * 言語仕様の違いでの運用性の違いからです。
 */
void Entity::AdditionalColorRelease(void)
{
	Library::Control::AdditionalColor* additionalColor = AdditionalColor;
	if(nullptr != additionalColor)	{
		/* 解放先のワークをキャッシュ */
		/* MEMO: キャッシュしておかないと、AdditionalColorSetで消されてしまうため。 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		void* handleAdditionalColor = nullptr;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		void* handleAdditionalColor = HandleAdditionalColor;
#endif

		/* 自身含めて子エンティティ群で同じポインタを持っているものをクリア */
		AdditionalColorSet(nullptr, additionalColor);

		/* 解放 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		delete additionalColor;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		additionalColor->ShutDown();
		additionalColor->~AdditionalColor();

		if(nullptr != handleAdditionalColor)	{
			Memory::Free(handleAdditionalColor);
		}
#endif
	}
}

/* ********************************************************* */
//! TRSリフレッシュのキャンセル
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::RefreshCancelTransform(int idParts, bool flagPosition, bool flagRotation, bool flagScaling)
{
	if(false == IsValid())	{
		return(false);
	}

	if(0 > idParts)	{	/* 全パーツsh千栄 */
		bool flagSuccess = true;
		for(int i=0; i<CountParts; i++)
		{
			flagSuccess &= RefreshCancelTransformMain(i, flagPosition, flagRotation, flagScaling);
		}

		return(flagSuccess);
	}

	if(CountParts <= idParts)	{
		return(false);
	}

	return(RefreshCancelTransformMain(idParts, flagPosition, flagRotation, flagScaling));
}
bool Entity::RefreshCancelTransformMain(int idParts, bool flagPosition, bool flagRotation, bool flagScaling)
{
	/* MEMO: flag～はtrueの場合にリフレッシュしない（キャンセル）であることに注意。 */
	Uint32 /* Library::Control::Animation::Parts::FlagBitStatus */ statusMask = ~Library::Control::Animation::Parts::FlagBitStatus::CLEAR;	/* All bit on */

	if(true == flagPosition)	{
		statusMask &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_TRANSFORM_POSITION;
	}
	if(true == flagRotation)	{
		statusMask &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_TRANSFORM_ROTATION;
	}
	if(true == flagScaling)	{
		statusMask &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_TRANSFORM_SCALING;
	}

	TableControlParts[idParts].Status &= statusMask;

	return(true);
}

/* ********************************************************* */
//! インスタンスパーツの持つ子エンティティを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Entity* Entity::InstanceGet(int idParts)
{
	if(false == IsValid())	{
		return(nullptr);
	}
	if(Library::Data::Parts::Animation::KindFeature::INSTANCE != FeatureGetParts(idParts))	{
		return(nullptr);
	}

	int indexEntity = TableControlParts[idParts].IndexEntityUnderControl;
	if(0 > indexEntity)	{
		return(nullptr);
	}

	return(&ListEntityChildInstance[indexEntity]);
}

/* ********************************************************* */
//! インスタンスパーツが持つ子エンティティを変更（パック名版）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::InstanceChange(int idParts, Project* project, const char* namePack)
{
	Entity* entityChild = InstanceGet(idParts);
	if(nullptr == entityChild)	{
		return(false);
	}

	/* プロジェクトの特定 */
	/* MEMO: 本関数ではprojectを保全するためにprojectを上書きしていません。                          */
	/*       インデックス版に元の値を渡して・インデックスを検索するためだけに特定しているだけです。  */
	/*       ※実はprojectを上書きしても結果は同じです。                                             */
	Project* projectNew = project;
	if(nullptr == projectNew)	{	/* 今使用中のプロジェクト */
		projectNew = entityChild->DataProject;
	}
	if(nullptr == projectNew)	{
		return(false);
	}

	/* アニメーションの検索 */
	int indexPack = projectNew->IndexGetAnimationPack(namePack);
	if(0 > indexPack)	{
		return(false);
	}

	return(InstanceChange(idParts, project, indexPack));
}

/* ********************************************************* */
//! インスタンスパーツが持つ子エンティティを変更（パック番号版）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::InstanceChange(int idParts, Project* project, int indexPack)
{
	Entity* entityChild = InstanceGet(idParts);
	if(nullptr == entityChild)	{
		return(false);
	}

	/* プロジェクトの特定 */
	bool flagProjectChange = true;
	if(nullptr == project)	{	/* 今使用中のプロジェクト */
		project = entityChild->DataProject;
		flagProjectChange = false;
	} else {	/* プロジェクト指定あり */
		if(entityChild->DataProject == project)	{	/* 今使用中のプロジェクト */
			flagProjectChange = false;
		}
	}
	if(nullptr == project)	{
		return(false);
	}

	/* プロジェクトの設定 */
	if(true == flagProjectChange)	{
		/* MEMO: プロジェクトが変わってしまう場合は一度子をシャットダウンする。 */
		Entity* entityParent = entityChild->EntityParent;

		/* MEMO: 持ち越さなくてはいけない設定関係。 */
		PrimitiveBatcher* modelDraw = entityChild->ModelDraw;
		bool flagHide = entityChild->FlagHideForce;
		bool flagPlanarization = entityChild->FlagPlanarization;
		float rateOpacity = entityChild->RateOpacityForce;
		CPU::Vector2 scaleLocal(entityChild->RateScaleLocalForce);

		entityChild->ShutDown();
		entityChild->BootUp(*project);
		if(nullptr != entityParent)	{
			entityChild->EntityParent = entityParent;

			entityChild->ModelDraw = modelDraw;
			entityChild->FlagHideForce = flagHide;
			entityChild->FlagPlanarization = flagPlanarization;
			entityChild->RateOpacityForce = rateOpacity;
			entityChild->RateScaleLocalForce.Set(scaleLocal);
		}
	}

	/* アニメーションパックの設定 */
	if(false == entityChild->PackSet(indexPack))	{
		return(false);
	}

	/* 最親のバッチングモデルを再構成 */
	Entity* entityHighest = EntityGetParentHighest();
	if(nullptr == entityHighest)	{
		entityHighest = this;
	}
	if(false == entityHighest->ModelSetDraw())	{
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! インスタンスパーツの持つ子エンティティに設定するアニメーションを指定（名前版）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::AnimationChangeInstance(	int idParts,
										const char* nameAnimation,
										Library::KindIgnoreAttribute::Kind ignoreAttribute,
										bool flagStartImmediate,
										int timesPlay,
										float rateTime,
										Library::KindStylePlay::Kind style,
										const char* labelRangeStart,
										int frameRangeOffsetStart,
										const char* labelRangeEnd,
										int frameRangeOffsetEnd
								)
{
	Entity* entityChild = InstanceGet(idParts);
	if(nullptr == entityChild)	{
		return(false);
	}

	int indexAnimation = -1;
	if(false == Library::Utility::Text::IsNullOrEmpty(nameAnimation))	{
		indexAnimation = entityChild->IndexGetAnimation(nameAnimation);
		if(0 > indexAnimation)	{
			return(false);
		}
	}

	return(	AnimationChangeInstanceMain(	&TableControlParts[idParts],
											entityChild,
											indexAnimation,
											ignoreAttribute,
											flagStartImmediate,
											timesPlay,
											rateTime,
											style,
											labelRangeStart,
											frameRangeOffsetStart,
											labelRangeEnd,
											frameRangeOffsetEnd
									)
		);
}
bool Entity::AnimationChangeInstanceMain(	Library::Control::Animation::Parts* controlParts,
											Entity* entityChild,
											int indexAnimation,
											Library::KindIgnoreAttribute::Kind ignoreAttribute,
											bool flagStartImmediate,
											int timesPlay,
											float rateTime,
											Library::KindStylePlay::Kind style,
											const char* labelRangeStart,
											int frameRangeOffsetStart,
											const char* labelRangeEnd,
											int frameRangeOffsetEnd
									)
{
	if(false == entityChild->IsValid())	{
		return(false);
	}
	if(0 > timesPlay)	{
		return(false);
	}
	if(true == std::isnan(rateTime))	{
		return(false);
	}

	/* 再生情報を作成 */
	Library::Data::Animation::Attribute::Instance dataInstance;
	dataInstance.CleanUp();
	dataInstance.Flags = Library::Data::Animation::Attribute::Instance::FlagBit::CLEAR;
	switch(style)	{
		case Library::KindStylePlay::NO_CHANGE:
			return(false);

		case Library::KindStylePlay::NORMAL:
			break;

		case Library::KindStylePlay::PINGPONG:
			dataInstance.Flags |= Library::Data::Animation::Attribute::Instance::FlagBit::PINGPONG;
			break;
	}

	controlParts->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_EXCEPT_NEXTDATA;
	switch(ignoreAttribute)	{
		case Library::KindIgnoreAttribute::NON:
			controlParts->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_ATTRIBUTE;
			controlParts->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_NEWANIMATION;

			dataInstance.Flags &= ~Library::Data::Animation::Attribute::Instance::FlagBit::INDEPENDENT;
			break;

		case Library::KindIgnoreAttribute::NOW_ANIMATION:
			controlParts->Status |= Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_ATTRIBUTE;
			controlParts->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_NEWANIMATION;

			dataInstance.Flags |= Library::Data::Animation::Attribute::Instance::FlagBit::INDEPENDENT;
			break;

		case Library::KindIgnoreAttribute::PERMANENT:
			controlParts->Status |= Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_ATTRIBUTE;
			controlParts->Status |= Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_NEWANIMATION;

			dataInstance.Flags |= Library::Data::Animation::Attribute::Instance::FlagBit::INDEPENDENT;
			break;
	}

	dataInstance.PlayCount = timesPlay;
	dataInstance.RateTime = rateTime;
	dataInstance.LabelStart = labelRangeStart;
	dataInstance.OffsetStart = frameRangeOffsetStart;
	dataInstance.LabelEnd = labelRangeEnd;
	dataInstance.OffsetEnd = frameRangeOffsetEnd;

	/* インスタンス */
	controlParts->IndexAnimationUnderControl = indexAnimation;
	if(false == flagStartImmediate)	{	/* 次のキーを待つ */
		controlParts->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_EXCEPT_NEXTDATA;
		/* MEMO: 即時再生でない場合、常に次のキーをデコードするために、独立再生のステータスを確実に落とす必要があります。 */
		/*       ※そうでないと直前までのデータが独立再生だと、アニメーションのループ後に同じフレームのデータをスキップし */
		/*         てしまうため。                                                                                         */
		controlParts->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_PLAY_INDEPENDENT;
	} else {	/* 即時再生 */
		controlParts->Status |= Library::Control::Animation::Parts::FlagBitStatus::INSTANCE_IGNORE_EXCEPT_NEXTDATA;

		controlParts->DataInstance = dataInstance;

		int indexTrack = controlParts->IndexControlTrack;
		if(0 <= indexTrack)	{	/* トラックに結ばれている */
			/* MEMO: 親アニメーション自体が逆再生されている可能性があるので、トラックの再生時間係数を考慮します。 */
			float rateTimeControlTrack = TableControlTrack[indexTrack].RateTime * ((true == TableControlTrack[indexTrack].StatusGetIsPlayingReverse()) ? -1.0f : 1.0f);
			return(controlParts->InstancePlayStart(*this, rateTimeControlTrack));
		}
	}

	return(true);
}

/* ********************************************************* */
//! インスタンスパーツの持つ子エンティティに設定するアニメーションを指定（番号版）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::AnimationChangeInstance(	int idParts,
										int indexAnimation,
										Library::KindIgnoreAttribute::Kind ignoreAttribute,
										bool flagStartImmediate,
										int timesPlay,
										float rateTime,
										Library::KindStylePlay::Kind style,
										const char* labelRangeStart,
										int frameRangeOffsetStart,
										const char* labelRangeEnd,
										int frameRangeOffsetEnd
								)
{
		Entity* entityChild = InstanceGet(idParts);
		if(nullptr == entityChild)	{
			return(false);
		}

		if((0 > indexAnimation) || (entityChild->CountGetAnimation() <= indexAnimation))	{
			return(false);
		}

		return(AnimationChangeInstanceMain(	&TableControlParts[idParts],
											entityChild,
											indexAnimation,
											ignoreAttribute,
											flagStartImmediate,
											timesPlay,
											rateTime,
											style,
											labelRangeStart,
											frameRangeOffsetStart,
											labelRangeEnd,
											frameRangeOffsetEnd
										)
			);
}

/* ********************************************************* */
//! エフェクトパーツの持つ子エンティティを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
EntityEffect* Entity::EffectGet(int idParts)
{
	if(false == IsValid())	{
		return(nullptr);
	}
	if(Library::Data::Parts::Animation::KindFeature::EFFECT != FeatureGetParts(idParts))	{
		return(nullptr);
	}

	int indexEntity = TableControlParts[idParts].IndexEntityUnderControl;
	if(0 > indexEntity)	{
		return(nullptr);
	}

	return(&ListEntityChildEffect[indexEntity]);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
