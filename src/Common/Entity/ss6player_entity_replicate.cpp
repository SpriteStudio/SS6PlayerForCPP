/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_entity_replicate.h"
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
/*                                                            EntityReplicate */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
EntityReplicate::EntityReplicate(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
EntityReplicate::~EntityReplicate(void)
{
//	CleanUp();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityReplicate::CleanUp(void)
{
	Status = FlagBitStatus::CLEAR;

	FlagHideForce = false;
//	ConstantShader
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityReplicate::IsValid(void)
{
	return(0 != (Status & FlagBitStatus::VALID));	/* ? true : false */
}

/* ********************************************************* */
//! 本クラスの起動（初期起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool EntityReplicate::BootUp(void)
{
	if(true == IsValid())	{
		return(false);
	}

	CleanUp();

	/* シェーダ定数の起動 */
	ConstantShader.BootUp();

	/* 初期化終了 */
	Status |= FlagBitStatus::VALID;

	return(true);
}

/* ********************************************************* */
//! 本クラスの終了（未使用化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void EntityReplicate::ShutDown(void)
{
	if(true == IsValid())	{
		/* シェーダ定数の起動 */
		ConstantShader.ShutDown();
	}

	CleanUp();
}

/* ********************************************************* */
//! 描画関数
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList EntityReplicate::Draw(	TypeDrawCommandList commandList,
											const CPU::Matrix4x4& matrixProjection,
											const CPU::Matrix4x4& matrixView,
											const CPU::Matrix4x4& matrixWorld,
											Entity& entityOriginal
										)
{
	if(false == IsValid())	{	/* 起動していない */
		return(commandList);
	}
	if(false == entityOriginal.IsValid())	{	/* 元がアニメーション再生できる状態でない（未初期化状態） */
		return(commandList);
	}
	if(nullptr != entityOriginal.EntityParent)	{	/* 元が最親ではない */
		return(commandList);
	}
	PrimitiveBatcher* modelDrawOriginal = entityOriginal.ModelDraw;
	if(nullptr == modelDrawOriginal)	{	/* 元の描画モデルがない */
		return(commandList);
	}
	if(false == modelDrawOriginal->IsValid())	{	/* 元の描画モデルが未初期化 */
		return(commandList);
	}
	if(true == modelDrawOriginal->IsAddablePrimitive())	{	/* 元の描画モデルが更新中 */
		return(commandList);
	}

	/* 描画 */
	if(false == FlagHideForce)	{	/* 非表示指定でない */
		/* レンダラをリセット */
		Renderer& renderer = Renderer::Instance;
		renderer.Refresh();

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
		ConstantShader.MatrixSetProjection(matrixProjection);
		ConstantShader.MatrixSetView(matrixView);
		ConstantShader.MatrixSetWorld(matrixWorld);
		ConstantShader.DataSet();

		/* バッチングされたプリミティブ群を描画 */
		/* MEMO: 子エンティティ（インスタンス・エフェクトなど）は最親エンティティの描画モデルに集積 */
		/*       されているので、最親エンティティしか描画処理をしないことに注意してください。       */
		TypeDrawCommandList commandListNew = modelDrawOriginal->Draw(commandList, ConstantShader);
		if(DrawCommandListInvalid == commandListNew)	{	/* 失敗 */
			return(DrawCommandListInvalid);
		}
		commandList = commandListNew;
	}

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Texture [static] */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
