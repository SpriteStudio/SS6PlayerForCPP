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
//! 強制非表示状態の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::HideSet(bool flagSwitch)
{
	if((nullptr == DataProject) || (nullptr == DataEffect))	{
		return(false);
	}

	FlagHideForce = flagSwitch;

	return(true);

}

/* ********************************************************* */
//! エフェクトデータ中のパーツ数の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int EntityEffect::CountGetParts(void)
{
	if((nullptr == DataProject) || (nullptr == DataEffect))	{
		return(-1);
	}

	return(DataEffect->CountGetTableParts());
}

/* ********************************************************* */
//! パーツ種別の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Library::Data::Parts::Effect::KindFeature EntityEffect::FeatureGetParts(int idParts)
{
	if((nullptr == DataProject) || (nullptr == DataEffect))	{
		return((Library::Data::Parts::Effect::KindFeature)-(1));
	}

	if((0 > idParts) || (DataEffect->CountGetTableParts() <= idParts))	{
		return((Library::Data::Parts::Effect::KindFeature)-(1));
	}

	Library::Data::Parts::Effect dataEffect;
	if(nullptr == DataEffect->TableParts(&dataEffect, idParts))	{
		return((Library::Data::Parts::Effect::KindFeature)-(1));
	}

	return(dataEffect.Feature());
}

/* ********************************************************* */
//! パーツIDの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int EntityEffect::IDGetParts(const char* name, size_t length)
{
	if((nullptr == DataProject) || (nullptr == DataEffect))	{
		return(-1);
	}

	return(DataEffect->IDGetParts(name, length));
}

/* ********************************************************* */
//! パーツ名の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
const char* EntityEffect::NameGetParts(int idParts)
{
	if((nullptr == DataProject) || (nullptr == DataEffect))	{
		return(nullptr);
	}

	if((0 > idParts) || (CountGetParts() <= idParts))	{
		return(nullptr);
	}

	Library::Data::Parts::Effect dataParts;
	if(nullptr == DataEffect->TableParts(&dataParts, idParts))	{
		return(nullptr);
	}

	return(dataParts.Name());
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
