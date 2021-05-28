/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_system.h"

#include "ss6player_renderer.h"
#include "ss6player_buffer.h"

/* ========================================================================== */
/* MEMO: 以下、SpriteStudio6名前空間直下に定義するものです。                  */
/* ========================================================================== */
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
/* Exceptional-Externs */
/* MEMO: プラットフォームやフレームワークに依存するような初期化・終了関数のため、 */
/*       ヘッダに登録せずに直接externする関数群です。                             */
extern bool FileSystemBootUp(const SettingSystem& setting);
extern void FileSystemShutDown(void);

extern bool RendererSystemBootUp(const SettingSystem& setting);
extern void RendererSystemShutDown(void);
extern void RendererSwapChainUpdate(int offsetIndex=1);

extern bool BufferSystemBootUp(const SettingSystem& setting);
extern void BufferSystemShutDown(void);

extern bool TextureSystemBootUp(const SettingSystem& setting);
extern void TextureSystemShutDown(void);

extern bool ShaderSystemBootUp(const SettingSystem& setting);
extern void ShaderSystemShutDown(void);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! プラットフォーム実装基部の起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool SystemBootUp(const SettingSystem& setting)
{
	/* MEMO: メモリシステムの初期化の必要があるなら、行うこと。 */

	/* MEMO: ファイルの基準パスを内部に定義しているだけなので、代替処理がアプリケーション側に */
	/*       存在する場合（特に、ss6player_fileの実装がアプリケーション側の処理を呼んでいる場 */
	/*       合など）は、FileSystemBootUpは必要ありません。                                   */
	if(false == FileSystemBootUp(setting))	{	/* ファイルシステム */
		return(false);
	}

	/* MEMO: 基本的にRendererSystemBootUpを最初に行うようにしてください。 */
	if(false == RendererSystemBootUp(setting))	{	/* レンダラ */
		return(false);
	}
	if(false == BufferSystemBootUp(setting))	{	/* バッファ */
		return(false);
	}
	if(false == TextureSystemBootUp(setting))	{	/* テクスチャ */
		return(false);
	}
	if(false == ShaderSystemBootUp(setting))	{	/* シェーダ（＆頂点宣言） */
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! プラットフォーム実装基部の終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void SystemShutDown(void)
{
	/* MEMO: （ファイルシステム以外では）必ずレンダラを最後に終了させてください。 */
	ShaderSystemShutDown();
	TextureSystemShutDown();
	BufferSystemShutDown();
	RendererSystemShutDown();

	FileSystemShutDown();
}

/* ********************************************************* */
//! メインスレッドの定常処理開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void SystemProcessMainPre(void)
{
	/* バッファ: 遅延解放処理 */
	/* MEMO: 前フレームで描画処理が済んだ分を消去していることに注意。 */
	Buffer::GPUDeleterChunk::Execute();
}

/* ********************************************************* */
//! メインスレッドの定常処理終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void SystemProcessMainPost(void)
{
}

/* ********************************************************* */
//! 描画スレッドの処理開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void SystemProcessRenderPre(void)
{
}

/* ********************************************************* */
//! 描画スレッドの処理終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void SystemProcessRenderPost(void)
{
	/* バッファの遅延管理に実行を要求 */
	Buffer::GPUDeleterChunk::ExecuteRequest();

	/* スワップチェイン管理を更新 */
	RendererSwapChainUpdate();
}

}	/* SpriteStudio6 */
