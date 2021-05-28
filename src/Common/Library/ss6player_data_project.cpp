/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_data_project.h"
#include "ss6player_utility.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Data	{
namespace Project	{
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

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                    CellMap */
/* -------------------------------------------------------------------------- */
// CellMap::CellMap(void)
// void CellMap::CleanUp(void)
// CellMap::KindVersion CellMap::Version(void)
// bool CellMap::IsValid(void)
// int CellMap::CountGetTableCellMap(void)
// Library::Data::CellMap::Body* CellMap::TableCellMap(Library::Data::CellMap::Body* cellMap, int index)
// int CellMap::CountGetTableIndexTexture(void)
// const Sint16* CellMap::TableIndexTexture(void)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                     Effect */
/* -------------------------------------------------------------------------- */
// Effect::Effect(void)
// void Effect::CleanUp(void)
// Effect::KindVersion Effect::Version(void)
// const char* Effect::Name(void)
// Effect::FlagBit Effect::FlagData(void)
// bool Effect::IsValid(void)

// bool Effect::StatusIsLockSeedRandom(void)
// int Effect::SeedRandom(void)
// int Effect::VersionRenderer(void)
// int Effect::CountMaxParticle(void)
// int Effect::CountFramePerSecond(void)
// CPU::Vector2* Effect::ScaleLayout(CPU::Vector2* scaleLayout)
// int Effect::CountGetTableParts(void)
// Library::Data::Parts::Effect* Effect::TableParts(Library::Data::Parts::Effect* effect, int index)
// int Effect::CountGetTableEmitter(void)
// Library::Data::Effect::Emitter* Effect::TableEmitter(Library::Data::Effect::Emitter* emitter, int index)
// int Effect::CountTableIndexEmitterOrderDraw(void)
// const int* Effect::TableIndexEmitterOrderDraw(void)

int Effect::IDGetParts(const char* name, size_t length)
{
	if(true == Library::Utility::Text::IsNullOrEmpty(name))	{
		return(-1);
	}

	int count = CountGetTableParts();
	if(0 > count)	{
		return(-1);
	}

	Library::Data::Parts::Effect parts;
	for(int i=0; i<count; i++)	{
		if(nullptr == TableParts(&parts, i))	{
			continue;
		}

		if(true == Library::Utility::Text::Compare(name, parts.Name(), length))	{
			return(i);
		}
	}

	return(-1);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                  Animation */
/* -------------------------------------------------------------------------- */
// Animation::Animation(void)
// void Animation::CleanUp(void)
// Animation::KindVersion Animation::Version(void)
// const char* Animation::Name(void)
// int Animation::CountTableParts(void)
// Library::Data::Parts::Animation* Animation::TableParts(Library::Data::Parts::Animation* parts, int idParts)
// const Library::Data::Parts::Animation::Catalog* Animation::CatalogParts(Library::Data::Parts::Animation::Catalog* catalog)
// int Animation::CountTableAnimation(void)
// const Library::Data::Animation::Body* Animation::TableAnimation(Library::Data::Animation::Body* animation, int index)
// bool Animation::IsAnimationSetup(void)
// const Animation::DataSetup* Animation::TableAnimationPartsSetup(DataSetup* dataSetUp)
// bool Animation::IsValid(void)
bool Animation::AttributeExpanderSetUp(void)
{
	/* MEMO: 現状、アトリビュート圧縮は全アニメーションで共通なので、持っている展開クラスはアトリビュート毎に1つだが、 */
	/*       アニメーション毎で使用しているアトリビュートが異なるので、一応全アニメーションを走査する。                */
	/*       ※若干データが冗長になりますが……後にコンバータ側で（プロジェクト内に）圧縮型式テーブルを作るかも。      */
	int countTableAnimation = CountTableAnimation();
	Library::Data::Animation::Body animationBody;
	Library::Data::Animation::Parts animationParts;
	Data::Animation::Pack::KindTypePack::Kind kindType;
	for(int i=0; i<countTableAnimation; i++)	{
		TableAnimation(&animationBody, 0);

		int countTableParts = animationBody.CountTableParts();
		for(int j=0; j<countTableParts; j++)	{
			animationBody.TableParts(&animationParts, j);

			if(nullptr == ExpanderStatus)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Status->TypePack();
				ExpanderStatus = Library::Data::Animation::Pack::AttributeExpanderGetStatus(kindType);
			}
			if(nullptr == ExpanderCell)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Cell->TypePack();
				ExpanderCell = Library::Data::Animation::Pack::AttributeExpanderGetCell(kindType);
			}
			if(nullptr == ExpanderPosition)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Position->TypePack();
				ExpanderPosition = Library::Data::Animation::Pack::AttributeExpanderGetVector3(kindType);
			}
			if(nullptr == ExpanderRotation)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Rotation->TypePack();
				ExpanderRotation = Library::Data::Animation::Pack::AttributeExpanderGetVector3(kindType);
			}
			if(nullptr == ExpanderScaling)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Scaling->TypePack();
				ExpanderScaling = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderScalingLocal)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.ScalingLocal->TypePack();
				ExpanderScalingLocal = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderRateOpacity)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.RateOpacity->TypePack();
				ExpanderRateOpacity = Library::Data::Animation::Pack::AttributeExpanderGetFloat(kindType);
			}
			if(nullptr == ExpanderPriority)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Priority->TypePack();
				ExpanderPriority = Library::Data::Animation::Pack::AttributeExpanderGetInt(kindType);
			}
			if(nullptr == ExpanderPartsColor)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.PartsColor->TypePack();
				ExpanderPartsColor = Library::Data::Animation::Pack::AttributeExpanderGetPartsColor(kindType);
			}
			if(nullptr == ExpanderVertexCorrection)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.VertexCorrection->TypePack();
				ExpanderVertexCorrection = Library::Data::Animation::Pack::AttributeExpanderGetVertexCorrection(kindType);
			}
			if(nullptr == ExpanderOffsetPivot)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.OffsetPivot->TypePack();
				ExpanderOffsetPivot = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderPositionAnchor)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.PositionAnchor->TypePack();
				ExpanderPositionAnchor = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderSizeForce)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.SizeForce->TypePack();
				ExpanderSizeForce = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderPositionTexture)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.PositionTexture->TypePack();
				ExpanderPositionTexture = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderRotationTexture)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.RotationTexture->TypePack();
				ExpanderRotationTexture = Library::Data::Animation::Pack::AttributeExpanderGetFloat(kindType);
			}
			if(nullptr == ExpanderScalingTexture)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.ScalingTexture->TypePack();
				ExpanderScalingTexture = Library::Data::Animation::Pack::AttributeExpanderGetVector2(kindType);
			}
			if(nullptr == ExpanderRadiusCollision)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.RadiusCollision->TypePack();
				ExpanderRadiusCollision = Library::Data::Animation::Pack::AttributeExpanderGetFloat(kindType);
			}
			if(nullptr == ExpanderUserData)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.UserData->TypePack();
				ExpanderUserData = Library::Data::Animation::Pack::AttributeExpanderGetUserData(kindType);
			}
			if(nullptr == ExpanderInstance)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Instance->TypePack();
				ExpanderInstance = Library::Data::Animation::Pack::AttributeExpanderGetInstance(kindType);
			}
			if(nullptr == ExpanderEffect)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Effect->TypePack();
				ExpanderEffect = Library::Data::Animation::Pack::AttributeExpanderGetEffect(kindType);
			}
			if(nullptr == ExpanderDeform)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Deform->TypePack();
				ExpanderDeform = Library::Data::Animation::Pack::AttributeExpanderGetDeform(kindType);
			}
			if(nullptr == ExpanderShader)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Shader->TypePack();
				ExpanderShader = Library::Data::Animation::Pack::AttributeExpanderGetShader(kindType);
			}
			if(nullptr == ExpanderSignal)	{
				kindType = (Data::Animation::Pack::KindTypePack::Kind)animationParts.Signal->TypePack();
				ExpanderSignal = Library::Data::Animation::Pack::AttributeExpanderGetSignal(kindType);
			}
		}
	}

	return(true);
}

// int Animation::IDGetPartsParent(int idParts)
// const char* Animation::NameGetParts(int idParts)
// const Sint32* Animation::TableGetPartsChild(int* count, int idParts)
// Library::Data::Parts::Animation::KindFeature Animation::FeatureGetParts(int idParts)
// Library::KindOperationBlend::Kind Animation::OperationGetPartsBlendTarget(int idParts)
// int Animation::IndexGetPartsAnimationPackUnderControl(int idParts)
// const char* Animation::NameGetPartsAnimationUnderControl(int idParts)
int Animation::IndexGetAnimation(const char* name)
{
	if(true == Library::Utility::Text::IsNullOrEmpty(name))	{
		return(-1);
	}

	Library::Data::Animation::Body dataAnimationBody;
	int count = CountTableAnimation();
	for(int i=0; i<count; i++)	{
		TableAnimation(&dataAnimationBody, i);
		if(true == Library::Utility::Text::Compare(name, dataAnimationBody.Name()))	{
			return(i);
		}
	}

	return(-1);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                       Animation::DataSetup */
/* -------------------------------------------------------------------------- */
// bool Animation::DataSetup::IsUserData(void) const
// Library::Data::Animation::Attribute::UserData* Animation::DataSetup::UserData(Library::Data::Animation::Attribute::UserData* userData) const
// bool Animation::DataSetup::IsSignal(void) const
// Library::Data::Animation::Attribute::Signal* Animation::DataSetup::Signal(Library::Data::Animation::Attribute::Signal* signal) const

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                   Sequence */
/* -------------------------------------------------------------------------- */
// Sequence::Sequence(void)
// bool Sequence::IsValid(void)
// Sequence::KindVersion Sequence::Version(void)
// const char* Sequence::Name(void)
// int Sequence::CountGetSequence(void)
int Sequence::IndexGetSequence(const char* name)
{
	if(true == Library::Utility::Text::IsNullOrEmpty(name))	{
		return(-1);
	}

	Library::Data::Sequence::Body dataSequence;
	int count = CountGetSequence();
	for(int i=0; i<count; i++)	{
		SequenceGet(&dataSequence, i);
		if(true == Library::Utility::Text::Compare(name, dataSequence.Name()))	{
			return(i);
		}
	}

	return(-1);
}
// Library::Data::Sequence::Body* Sequence::SequenceGet(Library::Data::Sequence::Body* sequence, int index)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                    Project */
/* -------------------------------------------------------------------------- */
// bool Project::BootUp(const void* data)
// void Project::ShutDown(void)
// bool Project::IsValid(void)
// const char* Project::Name(void)
// CellMap* Project::DataCellMap(CellMap* cellMap)
// int Project::CountDataAnimation(void)
int Project::IndexGetDataAnimation(const char* name)
{
	if(true == Library::Utility::Text::IsNullOrEmpty(name))	{
		return(-1);
	}

	Animation dataAnimation;
	int count = CountDataAnimation();
	for(int i=0; i<count; i++)	{
		DataAnimation(&dataAnimation, i);
		if(true == Library::Utility::Text::Compare(name, dataAnimation.Name()))	{
			return(i);
		}
	}

	return(-1);
}
// Animation* Project::DataAnimation(Animation* animation)
// int Project::CountDataEffect(void)
int Project::IndexGetDataEffect(const char* name)
{
	if(true == Library::Utility::Text::IsNullOrEmpty(name))	{
		return(-1);
	}

	Effect dataEffect;
	int count = CountDataEffect();
	for(int i=0; i<count; i++)	{
		DataEffect(&dataEffect, i);
		if(true == Library::Utility::Text::Compare(name, dataEffect.Name()))	{
			return(i);
		}
	}

	return(-1);
}
// Effect* Project::DataEffect(Effect* effect)

// int Project::CountDataSequence(void)
int Project::IndexGetDataSequence(const char* name)
{
	if(true == Library::Utility::Text::IsNullOrEmpty(name))	{
		return(-1);
	}

	Sequence dataSequence;
	int count = CountDataSequence();
	for(int i=0; i<count; i++)	{
		DataSequence(&dataSequence, i);
		if(true == Library::Utility::Text::Compare(name, dataSequence.Name()))	{
			return(i);
		}
	}

	return(-1);
}
// Sequence* Project::DataSequence(Sequence* sequence, int index)

// int Project::CountNameTexture(void)
// const char* Project::NameGetTexture(int index)


}	/* Project */
}	/* Data */
}	/* Library */
}	/* SpriteStudio6 */
