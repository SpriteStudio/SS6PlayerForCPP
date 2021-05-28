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
//! パーツで描画使用中のセルを変更
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::CellChangeParts(int idParts, int indexCellMap, int indexCell)
{
	if((nullptr == DataProject) || (nullptr == DataEffect))	{
		return(false);
	}

	const int countParts = DataEffect->CountGetTableParts();
	if(0 > idParts)	{	/* 全パーツ指定 */
		bool flagSuccess = true;

		for(int i=0; i<countParts; i++)	{
			flagSuccess &= CellChangePartsMain(IndexData, i, indexCellMap, indexCell);
		}

		return(flagSuccess);
	}

	if(countParts <= idParts)	{
		return(false);
	}

	return(CellChangePartsMain(IndexData, idParts, indexCellMap, indexCell));
}
bool EntityEffect::CellChangePartsMain(int indexData, int idParts, int indexCellMap, int indexCell)
{
	/* パーツの特定 */
	Library::Data::Parts::Effect parts;
	if(nullptr == DataProject->Effect[indexData].TableParts(&parts, idParts))	{
		return(false);
	}

	switch(parts.Feature())	{
		case Library::Data::Parts::Effect::KindFeature::ROOT:
			/* MEMO: Rootパーツは何も持っていないので、そのまま帰る */
			return(true);

		case Library::Data::Parts::Effect::KindFeature::EMITTER:
			break;

		case Library::Data::Parts::Effect::KindFeature::PARTICLE:
		default:
			/* MEMO: Particleパーツはあり得ないので、エラーを返す。 */
			/*       ※ただし、現状はここにくることはあり得ない。   */
			return(false);
	}

	/* エミッタの特定 */
	int indexEmitter = parts.IndexEmitter();
	if(0 > indexEmitter)	{
		return(false);
	}

	/* セルマップとセルの特定 */
	if((0 > indexCellMap) || (0 > indexCell))	{	/* 元に戻す指定 */
		Library::Data::Effect::Emitter emitter;
		if(nullptr == DataProject->Effect[indexData].TableEmitter(&emitter, indexEmitter))	{
			return(false);
		}

		indexCellMap = emitter.IndexCellMap();
		indexCell = emitter.IndexCell();
	} else {	/* 変更指定 */
		if(CountGetCellMap() <= indexCellMap)	{
			return(false);
		}
		if(CountGetCell(indexCellMap) <= indexCell)	{
			return(false);
		}
	}

	/* 使用セルマップ＆セルの変更 */
	if(	(ControlEffect.TableEmitter[indexEmitter].DataCellApply.IndexCellMap == indexCellMap)
		&& (ControlEffect.TableEmitter[indexEmitter].DataCellApply.IndexCell == indexCell)
	)	{	/* 現在設定と同じ */
		return(true);
	}

	ControlEffect.TableEmitter[indexEmitter].DataCellApply.IndexCellMap = indexCellMap;
	ControlEffect.TableEmitter[indexEmitter].DataCellApply.IndexCell = indexCell;
	ControlEffect.TableEmitter[indexEmitter].Status |= Library::Control::Effect::Emitter::FlagBitStatus::CHANGE_CELL_UNREFLECTED;

	return(true);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
