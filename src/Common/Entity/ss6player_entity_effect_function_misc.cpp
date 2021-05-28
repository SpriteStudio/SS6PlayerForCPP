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
//! エンティティが使用可能かの状態を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::StatustGetValidBootUp(void)
{
	return(0 != (Status & FlagBitStatus::VALID));	/* ? true : false */
}

/* ********************************************************* */
//! エンティティが再生可能かの状態を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityEffect::StatusGetValidPlay(void)
{
	return((true == StatustGetValidBootUp()) && (nullptr != DataEffect));	/* ? true : false */
}

/* ********************************************************* */
//! 動的パーツカラーパラメータの生成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Library::Control::AdditionalColor* EntityEffect::AdditionalColorCreate(void)
{
	Library::Control::AdditionalColor* additionalColor = AdditionalColor;
	if(nullptr == additionalColor)	{	/* 定義されていない */
		/* 動的パーツカラーの実体を作成・起動 */
#if 0	/* MEMO: 直接newで書いてしまう場合 */
		additionalColor = new Library::Control::AdditionalColor();
		if(nullptr == additionalColor)	{
			return(nullptr);
		}
		additionalColor->BootUp();

		/* MEMO: エフェクトは子を持たないので自身にだけ設定しています。 */
		AdditionalColor = additionalColor;
		HandleAdditionalColor = nullptr;
#else	/* MEMO: SS6Playerのヒープから取得する場合 */
		void* handleAdditionalColor;
		additionalColor = reinterpret_cast<Library::Control::AdditionalColor*>(Memory::Allocate(&handleAdditionalColor, sizeof(Library::Control::AdditionalColor)));
		if(nullptr == additionalColor)	{
			return(nullptr);
		}
		new(additionalColor) Library::Control::AdditionalColor;
		additionalColor->BootUp();

		/* MEMO: エフェクトは子を持たないので自身にだけ設定しています。 */
		AdditionalColor = additionalColor;
		HandleAdditionalColor = handleAdditionalColor;
#endif
	}

	return(additionalColor);
}

/* ********************************************************* */
//! 動的パーツカラーパラメータの解放
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityEffect::AdditionalColorRelease(void)
{
	Library::Control::AdditionalColor* additionalColor = AdditionalColor;
	if(nullptr != additionalColor)	{
		/* バッファへの参照をクリア */
		AdditionalColor = nullptr;

		/* 実体を解放 */
#if 0	/* MEMO: 直接newしていた場合 */
		delete additionalColor;
#else	/* MEMO: SS6Playerのヒープから取得していた場合 */
		additionalColor->ShutDown();
		additionalColor->~AdditionalColor();
		if(nullptr != HandleAdditionalColor)	{
			Memory::Free(HandleAdditionalColor);
		}
#endif
		HandleAdditionalColor = nullptr;
	}
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
