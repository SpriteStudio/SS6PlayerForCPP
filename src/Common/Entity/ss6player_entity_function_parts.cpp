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
//! アニメーションデータ中のパーツ数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Entity::CountGetParts(void)

/* ********************************************************* */
//! パーツ種別の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Library::Data::Parts::Animation::KindFeature Entity::FeatureGetParts(int idParts)

/* ********************************************************* */
//! パーツIDの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Entity::IDGetParts(char* name, size_t length)

/* ********************************************************* */
//! パーツ制御の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Library::Control::Animation::Parts* Entity::ControlGetParts(int idParts)

/* ********************************************************* */
//! パーツの強制非表示状態の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::HideSet(int idParts, bool flagSwitch, bool flagInvolveChildren)
{
	if((nullptr == DataAnimation) || (nullptr == TableControlParts))	{
		return(false);
	}

	if(0 > idParts)	{	/* All parts */
		for(int i=1; i<CountParts; i++)	{
			HideSetMain(i, flagSwitch, false);
		}
		return(true);
	}

	if(0 == idParts)	{	/* "Root"-Parts */
		/* MEMO: Rootパーツへの設定の場合、エンティティのFlagHideForceに値を設定します。 */
		FlagHideForce = flagSwitch;
		return(true);
	}

	if(CountParts <= idParts)	{	/* 不正パーツID */
		return(false);
	}

	HideSetMain(idParts, flagSwitch, flagInvolveChildren);

	return(true);
}
bool Entity::HideSetMain(int idParts, bool flagSwitch, bool flagInvolveChildren)
{
	if(true == flagSwitch)	{
		TableControlParts[idParts].Status |= Library::Control::Animation::Parts::FlagBitStatus::HIDE_FORCE;
	} else {
		TableControlParts[idParts].Status &= ~Library::Control::Animation::Parts::FlagBitStatus::HIDE_FORCE;
	}

	if(true == flagInvolveChildren)	{
		int countPartsChild = 0;
		const Sint32* tableIDPartsChild = DataAnimation->TableGetPartsChild(&countPartsChild, idParts);
		for(int i=0; i<countPartsChild; i++)	{
			/* MEMO: 子パーツに影響がある場合には、その子パーツ群にも影響が及びます。 */
			HideSetMain(tableIDPartsChild[i], flagSwitch, true);
		}
	}

	return(true);
}

/* ********************************************************* */
//! パーツのカラーラベルの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Library::Data::Parts::Animation::ColorLabel::KindForm Entity::FormGetColorLabel(int idParts)
{
	Library::Data::Parts::Animation::ColorLabel colorLabel;
	if(false == DataGetColorLabel(&colorLabel, idParts))	{
		return((Library::Data::Parts::Animation::ColorLabel::KindForm)(-1));
	}

	return(colorLabel.ValueForm);
}
bool Entity::DataGetColorLabel(Library::Data::Parts::Animation::ColorLabel* dataColorLabel, int idParts)
{
	if(nullptr == DataAnimation)	{
		return(false);
	}

	if((0 > idParts) || (DataAnimation->CountTableParts() <= idParts))	{
		return(false);
	}

	Library::Data::Parts::Animation parts;
	DataAnimation->TableParts(&parts, idParts);

	parts.LabelColor(dataColorLabel);

	return(true);
}

/* ********************************************************* */
//! パーツのカラーラベル色の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::ColorGetColorLabel(CPU::Color* color, int idParts)
{
	Library::Data::Parts::Animation::ColorLabel colorLabel;
	if(false == DataGetColorLabel(&colorLabel, idParts))	{
		return(false);
	}
	if(Library::Data::Parts::Animation::ColorLabel::KindForm::NON == colorLabel.ValueForm)	{	/* パーツカラーなし */
		return(false);
	}

	if(nullptr != color)	{
		*color = colorLabel.ValueColor;
	}

	return(true);
}

/* ********************************************************* */
//! パーツの追加反転状態の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::FlipSetParts(int idParts, bool flagFlipX, bool flagFlipY, bool flagFlipImageX, bool flagFlipImageY, bool flagInvolveChildren)
{
	int countParts = CountGetParts();
	if(0 >= countParts)	{	/* パーツがない */
		return(false);
	}
	if((0 > idParts) || (countParts <= idParts))	{	/* 不正パーツID */
		return(false);
	}

	Uint32 /* Library::Control::Animation::Draw::FlagBitStatus */ flags = Library::Control::Animation::Draw::FlagBitStatus::CLEAR;
	if(true == flagFlipX)	{
		flags |= Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_X;
	}
	if(true == flagFlipY)	{
		flags |= Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_Y;
	}
	if(true == flagFlipImageX)	{
		flags |= Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_X;
	}
	if(true == flagFlipImageY)	{
		flags |= Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_Y;
	}

	return(FlipSetPartsMain(idParts, flags, flagInvolveChildren));
}
bool Entity::FlipSetPartsMain(int idParts, Uint32 flags, bool flagInvolveChildren)
{
	bool flagSettable = false;
	switch(FeatureGetParts(idParts))	{
		case Library::Data::Parts::Animation::KindFeature::ROOT:
		case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
//			flagSettable = false;
			break;

		case Library::Data::Parts::Animation::KindFeature::NORMAL:
			flagSettable = true;
			break;

		case Library::Data::Parts::Animation::KindFeature::INSTANCE:
		case Library::Data::Parts::Animation::KindFeature::EFFECT:
//			flagSettable = false;
			break;

		case Library::Data::Parts::Animation::KindFeature::MASK:
			flagSettable = true;
			break;

		case Library::Data::Parts::Animation::KindFeature::JOINT:
		case Library::Data::Parts::Animation::KindFeature::BONE:
		case Library::Data::Parts::Animation::KindFeature::MOVENODE:
		case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
		case Library::Data::Parts::Animation::KindFeature::MESH:
		default:
//			flagSettable = false;
			break;
	}

	if(true == flagSettable)	{
		Library::Control::Animation::Parts* controlParts = ControlGetParts(idParts);
		if(nullptr == controlParts)	{
			return(false);
		}
		int indexDraw = controlParts->IndexControlDraw;
		if(0 > indexDraw)	{
			return(false);
		}

		Library::Control::Animation::Draw* controlDraw = &TableControlPartsDraw[indexDraw];

		controlDraw->Status &=  ~(	Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_X
									| Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_Y
									| Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_X
									| Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_Y
								);
		controlDraw->Status |= flags;
	}

	if(true == flagInvolveChildren)	{
		int countPartsChild = 0;
		const Sint32* tableIDPartsChild = DataAnimation->TableGetPartsChild(&countPartsChild, idParts);
		for(int i=0; i<countPartsChild; i++)	{
			/* MEMO: 子パーツに影響がある場合には、その子パーツ群にも影響が及びます。 */
			/* MEMO: 全てのパーツが設定可能なわけではないので、エラーは見逃しています。 */
			FlipSetPartsMain(tableIDPartsChild[i], flags, true);
		}

		return(true);	/* Always true */
	}

	return(flagSettable);
}

/* ********************************************************* */
//! パーツの追加反転状態の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::FlipGetParts(bool* flagFlipX, bool* flagFlipY, bool* flagFlipImageX, bool* flagFlipImageY, int idParts)
{
	int countParts = CountGetParts();
	if(0 >= countParts)	{	/* パーツがない */
		return(false);
	}
	if((0 > idParts) || (countParts <= idParts))	{	/* 不正パーツID */
		return(false);
	}

	switch(FeatureGetParts(idParts))	{
		case Library::Data::Parts::Animation::KindFeature::ROOT:
		case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
			return(false);

		case Library::Data::Parts::Animation::KindFeature::NORMAL:
			break;

		case Library::Data::Parts::Animation::KindFeature::INSTANCE:
		case Library::Data::Parts::Animation::KindFeature::EFFECT:
			return(false);

		case Library::Data::Parts::Animation::KindFeature::MASK:
			break;

		case Library::Data::Parts::Animation::KindFeature::JOINT:
		case Library::Data::Parts::Animation::KindFeature::BONE:
		case Library::Data::Parts::Animation::KindFeature::MOVENODE:
		case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
		case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
		case Library::Data::Parts::Animation::KindFeature::MESH:
		default:
			return(false);
	}

	Library::Control::Animation::Parts* controlParts = ControlGetParts(idParts);
	if(nullptr == controlParts)	{
		return(false);
	}
	int indexDraw = controlParts->IndexControlDraw;
	if(0 > indexDraw)	{
		return(false);
	}

	Library::Control::Animation::Draw* controlDraw = &TableControlPartsDraw[indexDraw];
	Uint32 flags = controlDraw->Status;

	if(nullptr != flagFlipX)	{
		*flagFlipX = (0 != (flags & Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_X));	/* ? true : false */
	}
	if(nullptr != flagFlipY)	{
		*flagFlipY = (0 != (flags & Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_Y));	/* ? true : false */
	}
	if(nullptr != flagFlipImageX)	{
		*flagFlipImageX = (0 != (flags & Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_X));	/* ? true : false */
	}
	if(nullptr != flagFlipImageY)	{
		*flagFlipImageY = (0 != (flags & Library::Control::Animation::Draw::FlagBitStatus::FLIP_COEFFICIENT_TEXTURE_Y));	/* ? true : false */
	}

	return(true);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
