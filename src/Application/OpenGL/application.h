/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* アプリケーションとしての主処理関係をまとめたもので、                       */
/* Application/(Platform)/main.cppに実装されている機種依存したメイン基底処理  */
/* から呼び出される非機種依存の主処理になります。                             */
/* 実際のアプリケーションにおいては、それぞれのタイトル毎の処理が本役割を負い */
/* ので、実質上本ソースはダミー的色合いが強いものです。                       */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include <string>
#include <memory>

/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
namespace SpriteStudio6
{
// forward declarations
namespace Platform {
class FileAbstract;
}

class Application
{
public:
	/* コンパイルオプション: シーケンサ再生 */
	/* MEMO: 定義すると、「シーケンサ（シーケンスデータ）」の再生サンプルになります。                */
	/*       定義しないと、「エンティティ（1アニメーションオブジェクト）」の再生サンプルになります。 */
	// #define _COMPILEOPTION_TEST_SEQUENCER_

	/* コンパイルオプション: 複製描画の追加 */
	/* MEMO: 定義すると、再生しているオブジェクトを画面いっぱいに複製描画します。 */
	// #define _COMPILEOPTION_TEST_REPLICATE_

	/* コンパイルオプション: テクスチャを外部定義 */
	/* MEMO: 少し変わった運用時のための例です。                                                      */
	/*       ※例えば、SS6のデータと他のモデルや描画物がテクスチャを共用しているような場合……など。 */
	// #define _COMPILEOPTION_TEXTURE_EXTERNALENTITY_

	struct Options {
		// NAME_FILE_SSFB2: 読み込むssfb2のファイル名（拡張子付きで記述してください）
		std::string NAME_FILE_SSFB2;

		#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		/* MEMO: シーケンス再生の場合は、ssqeのファイル名と、ssqeの中に定義されているシーケンス名が必要になります。 */

		std::string NAME_SSQE_SEQUENCEPACK;
		std::string NAME_SEQUENCE;
		#else
		/* MEMO: 単独アニメーション再生の場合は、ssaeのファイル名と、ssaeの中に定義されているアニメーション名が必要になります。 */

		// NAME_SSAE_ANIMATIONPACK: 再生するアニメーションデータが入っているアニメーションパック名
		// ※SpriteStudio6上でのssaeファイルのボディ名です。
		// 例：Knight_bomb.ssae -> Knight_bomb
		std::string NAME_SSAE_ANIMATIONPACK;

		// NAME_ANIMATIONが再生するアニメーションの名称です。
		// ※SpriteStudio6上でのssaeファイル内のアニメーションの名称です。
		std::string NAME_ANIMATION;
		#endif	
	};

	/* ********************************************************* */
//! アプリケーションの初期起動処理
/*!
@param	なし

@retval	関数値
	true == 成功
	false == 失敗

アプリケーションの開始時に機種依存メインループから初回の
	ApplicationUpdateの前に1回だけ呼び出されます。
描画スレッドの動作開始前に呼ばれますので、本関数の中で描
	画処理をまたいで行われるような処理を行うことはできま
	せんので注意してください。

基本的にライブラリやSS6Playerの基底初期化は終わった状態で
	呼び出される前提です。
*/
	bool BootUp(const Options& options);

	/* ********************************************************* */
	//! アプリケーションの終了処理
	/*!
	@param	なし

	@retval	なし

	アプリケーションの終了時に機種依存メインループから最後の
		ApplicationUpdateの後に1回だけ呼び出されます。
	*/
	void ShutDown(void);

	/* ********************************************************* */
	//! アプリケーションの定常更新処理
	/*!
	@param	timeElapsed
		前のフレームから経過した時間（1.0f==1秒）

	@retval	関数値
		true == 成功
		false == 失敗

	アプリケーションの定常処理で毎ループ呼び出されます。
	*/
	bool Update(SpriteStudio6::Float32 timeElapsed);

	/* ********************************************************* */
	//! アプリケーションの描画処理
	/*!
	@param	commandList
		描画コマンドリスト

	@retval	関数値
		true == 成功
		false == 失敗

	アプリケーションの描画（描画コマンド設定）時に呼び出されます。
	*/
	bool Draw(SpriteStudio6::TypeDrawCommandList commandList);

public:
	void SetPlatformFile(std::unique_ptr<Platform::FileAbstract>&& abstract) { _file = std::move(abstract); }
	Platform::FileAbstract& GetPlatformFile() { return *_file; }

private:
	std::unique_ptr<Platform::FileAbstract> _file;
};
}

/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */

