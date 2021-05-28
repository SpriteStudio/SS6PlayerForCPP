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
//! セル情報の取得
// const Library::Data::CellMap::Cell* DataGetCell(Library::Data::CellMap::Cell* dataCell, Library::Data::CellMap::Body& dataCellMap, int indexCell)

/* ********************************************************* */
//! パーツで描画使用中のセルを変更
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::CellChangeParts(int idParts, int indexCellMap, int indexCell, Library::KindIgnoreAttribute::Kind ignoreAttribute)
{
	if(false == IsValid())	{
		return(false);
	}

	if(0 > idParts)	{	/* 全パーツ指定 */
		int countParts = CountParts;
		bool flagSuccess = true;
		for(int i=0; i<countParts; i++)	{
			flagSuccess &= CellChangePartsMain(i, indexCellMap, indexCell, ignoreAttribute);
		}

		return(flagSuccess);
	}

	if(CountParts <= idParts)	{
		return(false);
	}

	return(CellChangePartsMain(idParts, indexCellMap, indexCell, ignoreAttribute));
}
bool Entity::CellChangePartsMain(int idParts, int indexCellMap, int indexCell, Library::KindIgnoreAttribute::Kind ignoreAttribute)
{
		switch(FeatureGetParts(idParts))	{
			case Library::Data::Parts::Animation::KindFeature::ROOT:
			case Library::Data::Parts::Animation::KindFeature::NULL_PARTS:
				/* MEMO: セルを持たない。 */
				return(true);

			case Library::Data::Parts::Animation::KindFeature::NORMAL:
				break;

			case Library::Data::Parts::Animation::KindFeature::INSTANCE:
			case Library::Data::Parts::Animation::KindFeature::EFFECT:
				/* MEMO: セルを持たない。 */
				return(true);

			case Library::Data::Parts::Animation::KindFeature::MASK:
				break;

			case Library::Data::Parts::Animation::KindFeature::JOINT:
			case Library::Data::Parts::Animation::KindFeature::BONE:
			case Library::Data::Parts::Animation::KindFeature::MOVENODE:
			case Library::Data::Parts::Animation::KindFeature::CONSTRAINT:
			case Library::Data::Parts::Animation::KindFeature::BONEPOINT:
				/* MEMO: セルを持たない。 */
				return(true);

			case Library::Data::Parts::Animation::KindFeature::MESH:
				/* MEMO: 一旦セル変更を認めない。                                                       */
				/*       ※変更は可能だが、メッシュバインドを変更できないため、色々な制限かかかるため。 */
				return(true);

			case Library::Data::Parts::Animation::KindFeature::TRANSFORM_CONSTRAINT:
			case Library::Data::Parts::Animation::KindFeature::CAMERA:
			default:	/* ClangのWarning避け */
				/* MEMO: セルを持たない。 */
				return(true);
		}

		if((0 > indexCellMap) || (0 > indexCell))	{
			indexCellMap = -1;
			indexCell = -1;
		}
		int indexDraw = TableControlParts[idParts].IndexControlDraw;
		if((0 > indexDraw) || (CountPartsDraw <= indexDraw))	{
			/* MEMO: 一旦正常なふりをしておく。 */
			return(true);
		}
		Library::Control::Animation::Draw* controlPartsDraw = &TableControlPartsDraw[indexDraw];
		controlPartsDraw->DataCellApply.IndexCellMap = indexCellMap;
		controlPartsDraw->DataCellApply.IndexCell = indexCell;
		controlPartsDraw->Status |= Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_UNREFLECTED;

		switch(ignoreAttribute)	{
			case Library::KindIgnoreAttribute::NON:
				controlPartsDraw->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_ATTRIBUTE;
				controlPartsDraw->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_NEWANIMATION;
				break;

			case Library::KindIgnoreAttribute::NOW_ANIMATION:
				controlPartsDraw->Status |= Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_ATTRIBUTE;
				controlPartsDraw->Status &= ~Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_NEWANIMATION;
				break;

			case Library::KindIgnoreAttribute::PERMANENT:
				controlPartsDraw->Status |= Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_ATTRIBUTE;
				controlPartsDraw->Status |= Library::Control::Animation::Parts::FlagBitStatus::CHANGE_CELL_IGNORE_NEWANIMATION;
				break;
		}

		return(true);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
