/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* レンダリング用モデルを構築するための処理群です。                           */
/* ドローコールが最低限になるように、全描画バーツのプリミティブをバッチングし */
/* て、モデルデータを形成します。                                             */
/* ※通常の描画パーツ群では問題にならないのですが、エフェクトの描画量をバッチ */
/*   ングしないで処理すると、Mantle系アーキテクチャではコストが高くなりすぎる */
/*   ため、バッチングを行っています（描画コマンドリストやGPUバッファ群を大量  */
/*   消費しすぎてしまうためです）。                                           */
/*                                                                            */
/* 機種依存していませんが、機能的にLibraryではないので（レイヤ的にはPlatform  */
/* とLibraryの間にあります）、SpriteStudio6の名前空間直下にあります。         */
/*                                                                            */
/* 最終的にGPUに送出される頂点データなどはこのモジュールで形成されます。      */
/* 本クラスが生成する頂点データは、頂点インデックス形式を使用しないで直接頂点 */
/* データでGPU描画を行うためのデータを生成します。                            */
/* ※パーツ毎にブレンド・シェーダ・セルマップの情報が動的に変わるため、頂点イ */
/*   ンデックス長がフレーム毎に不定になることが多いため、頂点インデックス領域 */
/*   を都度再取得するコストがかかりデメリットが勝ってしまうため、ベタで頂点情 */
/*   報を作成しています。                                                     */
/*                                                                            */
/* 補足:                                                                      */
/* 一部Rendererクラスと機能競合する部分がありますが、それはRendererでフォロー */
/* できない機能も踏まえて本クラスが作成された経緯があるためです。             */
/* そのため、本クラス実働後にRendererクラスの機能・処理を簡略化する可能性があ */
/* ります。                                                                   */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>

#include "../Library/ss6player_data.h"

/* 前方宣言 */

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* MEMO: 本クラスは、「1つの描画オブジェクトを描画するための全情報」です。        */
/*       3Dモデルなどでは「モデル」「メッシュ」などと呼ばれる単位とほぼ同一です。 */
/*       原則、各Entityなどから描画物を管理する場合、本クラスを基礎単位とします。 */
class PrimitiveBatcher
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* 前方宣言 */
	class Fragment;

	/* レンダリング設定 */
	/* MEMO: 基本は「マテリアル」と同様の内容ですが、レンダリングターゲットなどの情報も追加で入っています。 */
	/* MEMO: Fragmentと重複項目が多いので、共通項目を基底クラスとして抜き出しています。 */
	class SettingRender
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! 描画コマンド割込コマンド作成コールバック
		/*!
		@param	commandList
			描画コマンドリスト
		@param	parameterPointer
			コールバック設定時に指定した任意のポインタ
		@param	parameterInteger
			コールバック設定時に指定した任意の整数
		@param	pointer
			コールバック設定時に指定した任意ポインタ

		@retval	関数値
			更新された描画コマンドリスト
			DrawCommandListInvalid == 失敗

		フラグメントのプリミティブ描画コマンドの発行前に同期処理が必要
			な描画コマンドを挿入するためのコールバック関数です。
		本コマンドは、基本的にプリミティブの描画に必要な描画コマンド群
			の生成前（フラグメントの描画コマンド先頭）の作成タイミング
			で呼ばれますので、フラグメントの描画に必要な各種描画コマン
			ドは描画コマンドリストに定義されていない状態となります。
		*/
		typedef TypeDrawCommandList (*FunctionCommandInsert)(TypeDrawCommandList commandList, void* parameterPointer, int parameterInteger);

		/* コールバック情報クラス */
		/* MEMO: 殆ど構造体のような扱いしかしていません。 */
		template<typename _TypeFunction> class CallBack
		{
			/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
		private:
		protected:
		public:

			/* ----------------------------------------------- Enums & Constants */
		private:
		protected:
		public:

			/* ----------------------------------------------- Variables */
		private:
		protected:
		public:
			_TypeFunction Function;
			void* ParameterPointer;
			int ParameterInteger;

			/* ----------------------------------------------- Functions */
		private:
		protected:
		public:
			/* ********************************************************* */
			//! 未使用化
			/*!
			@param	なし

			@retval	なし

			本クラスを未使用状態にします。
			*/
			inline void CleanUp(void)
			{
				Function = nullptr;
				ParameterPointer = nullptr;
				ParameterInteger = 0;
			}

			/* ********************************************************* */
			//! コンストラクタ
			/*!
			@param	なし

			@retval	なし
			*/
			CallBack(void)
			{
				CleanUp();
			}

			/* ********************************************************* */
			//! デストラクタ
			/*!
			@param	なし

			@retval	なし

			※このクラスは継承したりしないので、仮想化していません。
			*/
			~CallBack(void)
			{
				CleanUp();
			}

			/* ********************************************************* */
			//! コールバックが発行可能か？
			/*!
			@param	なし

			@retval	関数値
				true == 可能（コールバック関数が設定されている）
				false == 不可能（コールバック関数が設定されていない）

			本クラスの内容でコールバックを発行できるかどうかを取得します。
			*/
			inline bool IsValid(void)
			{
				return(nullptr != Function);	/* ? true : false */
			}

			/* ********************************************************* */
			//! コールバックの設定
			/*!
			@param	function
				コールバック受領関数
			@param	parameterPointer
				コールバックポインタ
			@param	parameterInteger
				コールバック整数（符号あり）

			@retval	なし

			本クラスにコールバックを設定します。
			parameterPointer・parameterIntegerはコールバック受領関数に渡さ
				れる任意のポインタ・整数で、本クラス含めたSS6Playerの機能は
				本ポインタに頓着しません。
			*/
			inline void Set(_TypeFunction function, void* parameterPointer, int parameterInteger)
			{
				Function = function;
				ParameterPointer = parameterPointer;
				ParameterInteger = parameterInteger;
			}

			/* ********************************************************* */
			//! コールバックの取得
			/*!
			@param	function
				コールバック受領関数の格納先
			@param	parameterPointer
				コールバックポインタの格納先
			@param	parameterInteger
				コールバック整数の格納先

			@retval	function
				コールバック受領関数
			@retval	parameterPointer
				コールバックポインタ
			@param	parameterInteger
				コールバック整数

			本クラスに設定されているコールバック情報を取得します。
			*/
			inline void Get(_TypeFunction* function, void** parameterPointer, int* parameterInteger)
			{
				*function = Function;
				*parameterPointer = ParameterPointer;
				*parameterInteger = ParameterInteger;
			}

			/* ********************************************************* */
			//! コールバックの複製
			/*!
			@param	original
				複製元のコールバッククラス

			@retval	なし

			originalの内容を本クラスの実体にコピーします。
			*/
			inline void Duplicate(const CallBack& original)
			{
				Function = original.Function;
				ParameterPointer = original.ParameterPointer;
				ParameterInteger = original.ParameterInteger;
			}
			/* ----------------------------------------------- Operators */

			/* ----------------------------------------------- Friends */
		};

		/* ----------------------------------------------- Enums & Constants */
	private:
		enum FlagStatusBit	{
			CLEAR_TARGET_PIXEL = 0x00000fff,			/* ピクセル色用ターゲットをクリアする（念のため12個予約） *//* ※下位ビットから0～(Renderer::SectionTarget::Constant::COUNT_TARGET-1) */
			CLEAR_TARGET_TEST = 0x00001000,				/* テスト用ターゲットをクリアする */
			WRITE_DEPTH = 0x00002000,					/* 深度を書き込む */
			ACTION_STENCIL = 0x0000c000,				/* 描画時ステンシルアクション種類 */
			COUNT_TEXTURE_TARGET_PIXEL = 0x000f0000,	/* ピクセル色用ターゲットテクスチャの枚数 */
			INTERPOLATION_TEXTURE_CELLMAP = 0x00f00000,	/* セルマップテクスチャの補間方法 */
			WRAP_TEXTURE_CELLMAP = 0x0f000000,			/* セルマップテクスチャのラッピング方法 */
			TEST_STENCIL = 0x10000000,					/* ステンシルテストを行う */
			TEST_DEPTH = 0x20000000,					/* 深度テストを行う */
		};
		enum ShiftStatusBit	{
			SHIFT_ACTION_STENCIL = 14,					/* 描画時ステンシルアクション種類 */
			SHIFT_COUNT_TEXTURE_TARGET_PIXEL = 16,		/* ピクセル色用ターゲットテクスチャの枚数 */
			SHIFT_INTERPOLATION_TEXTURE_CELLMAP = 20,	/* セルマップテクスチャの補間方法 */
			SHIFT_WRAP_TEXTURE_CELLMAP = 24,			/* セルマップテクスチャのラッピング方法 */
		};

	protected:
	public:

		/* ----------------------------------------------- Variables */
	private:
		/* MEMO: 外部から直接操作不可にするためにprivateにしています（外部からの操作は必ず関数を使用してください）。 */
		/* MEMO: メンバの並び順はアライメントを考慮しています（大量に設置される可能性があるために、容量削減の意図があります）。 */

		Uint32 /* FlagStatusBit */ Status;															/* ステータス *//* ※外部からの操作禁止 */
		Float32 ValueClearDepth;																	/* ピクセルテスト用レンダリングターゲットテクスチャのクリア深度値 */

		Sint16 ValueClearStencil;																	/* ピクセルテスト用レンダリングターゲットテクスチャのクリアステンシル値 */
		Sint16 /* Renderer::SectionBlend::KindType */ TypeBlend;									/* ブレンド種別 */

		/* MEMO: シェーダ定数は描画時に外部から関数の引数で与える仕様です。      */
		/*       ※増殖描画（疑似的なCPUでのインスタンシング）時に変形行列などの */
		/*         GPUバッファだけ変更すれば良いように……という意図です。       */
		const Shader* Shader;																		/* 使用するシェーダ */

		Texture* TextureCellMap;																	/* セルマップテクスチャ */

		Texture* TextureTargetColor[Renderer::SectionTarget::Constant::COUNT_TARGET];				/* ピクセル色用レンダリングターゲットテクスチャ *//* ※nullptrの場合ピクセル色用フレームバッファを意味します。 */
		CPU::Color ColorClearTargetColor[Renderer::SectionTarget::Constant::COUNT_TARGET];			/* ピクセル色用レンダリングターゲットテクスチャのクリア色 */

		Texture* TextureTargetTest;																	/* ピクセルテスト用レンダリングターゲットテクスチャ *//* ※nullptrの場合テスト用フレームバッファを意味します。 */

	protected:
	public:
		CallBack<FunctionCommandInsert> CallBackInsert;												/* フラグメントの描画コマンド発行前に割り込ませる描画コマンド生成の関数コールバック */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! 未使用化
		/*!
		@param	なし
	
		@retval	なし
		*/
		inline void CleanUp(void)
		{
			Status = 0;
			ValueClearDepth = 0.0f;

			ValueClearStencil = (Sint16)0;
			TypeBlend = Renderer::SectionBlend::KindType::NONE;

			Shader = nullptr;

			TextureCellMap = nullptr;

			for(int i=0; i<Renderer::SectionTarget::Constant::COUNT_TARGET; i++)	{
				TextureTargetColor[i] = nullptr;
				ColorClearTargetColor[i].Set(0.0f, 0.0f, 0.0f, 0.0f);
			}

			TextureTargetTest = nullptr;
			CallBackInsert.CleanUp();
		}

		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし
	
		@retval	なし
		*/
		SettingRender(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし
	
		@retval	なし
		*/
		~SettingRender(void)
		{
//			CleanUp();
		}

		/* ********************************************************* */
		//! 値の設定（複製）
		/*!
		@param	original
			複製元
	
		@retval	なし

		値を複製します。
		*/
		inline void Set(const SettingRender& original)
		{
			Status = original.Status;
			ValueClearStencil = original.ValueClearStencil;

			ValueClearDepth = original.ValueClearDepth;
			TypeBlend = original.TypeBlend;

			Shader = original.Shader;

			TextureCellMap = original.TextureCellMap;

			for(int i=0; i<Renderer::SectionTarget::Constant::COUNT_TARGET; i++)	{
				TextureTargetColor[i] = original.TextureTargetColor[i];
				ColorClearTargetColor[i].Set(original.ColorClearTargetColor[i]);
			}

			TextureTargetTest = original.TextureTargetTest;
			CallBackInsert.Duplicate(original.CallBackInsert);
		}

		/* ********************************************************* */
		//! シェーダを設定
		/*!
		@param	shader
			シェーダ
		@param	dataShader
			シェーダ定数
	
		@retval	なし

		レンダリングに使用するシェーダを設定します。
		※shader/dataShaderは通常なら（入力なので）参照にするところで
			すが、nullptrを許容するためにポインタになっています。
		*/
		inline void ShaderSet(const SpriteStudio6::Shader* shader)
		{
			Shader = shader;
		}

		/* ********************************************************* */
		//! ブレンド種別を設定
		/*!
		@param	blend
			ブレンド種別
	
		@retval	なし

		ブレンド種別を設定します。
		*/
		inline void BlendSet(Renderer::SectionBlend::KindType blend)
		{
			TypeBlend = (Sint16)blend;
		}

		/* ********************************************************* */
		//! セルマップテクスチャを設定
		/*!
		@param	texture
			テクスチャ
	
		@retval	なし

		セルマップ用テクスチャを設定します。
		※textureは通常なら（入力なので）参照にするところですが、
			nullptrを許容するためにポインタになっています。
		*/
		inline void TextureSetCellMap(Texture* texture, Texture::KindInterpolation interpolation, Texture::KindWrap wrap)
		{
			TextureCellMap = texture;
			Status &= ~((Uint32)(FlagStatusBit::INTERPOLATION_TEXTURE_CELLMAP | FlagStatusBit::WRAP_TEXTURE_CELLMAP));
			Status |= (Uint32)interpolation << ShiftStatusBit::SHIFT_INTERPOLATION_TEXTURE_CELLMAP;
			Status |= (Uint32)wrap << ShiftStatusBit::SHIFT_WRAP_TEXTURE_CELLMAP;
		}

		/* ********************************************************* */
		//! セルマップテクスチャの補間方法取得
		/*!
		@param	なし
	
		@retval	関数値
			セルマップテクスチャの補間方法

		セルマップテクスチャの補間方法を取得します。
		*/
		inline Texture::KindInterpolation InterpolationGetCellMap(void) const
		{
			return((Texture::KindInterpolation)((Status & FlagStatusBit::INTERPOLATION_TEXTURE_CELLMAP) >> ShiftStatusBit::SHIFT_INTERPOLATION_TEXTURE_CELLMAP));
		}

		/* ********************************************************* */
		//! セルマップテクスチャのラップアラウンド方法取得
		/*!
		@param	なし
	
		@retval	関数値
			セルマップテクスチャの補間方法

		セルマップテクスチャのラップアラウンド方法を取得します。
		*/
		inline Texture::KindWrap WrapGetCellMap(void) const
		{
			return((Texture::KindWrap)((Status & FlagStatusBit::WRAP_TEXTURE_CELLMAP) >> ShiftStatusBit::SHIFT_WRAP_TEXTURE_CELLMAP));
		}

		/* ********************************************************* */
		//! カラーターゲットテクスチャ数を設定
		/*!
		@param	count
			ターゲットテクスチャ数
	
		@retval	なし

		MRTのターゲット数を設定します。
		*/
		inline void CountSetTextureTargetColor(int count)
		{
			Status &= ~((Uint32)FlagStatusBit::COUNT_TEXTURE_TARGET_PIXEL);
			Status |= (Uint32)count << ShiftStatusBit::SHIFT_COUNT_TEXTURE_TARGET_PIXEL;
		}

		/* ********************************************************* */
		//! カラーターゲットテクスチャ数を取得
		/*!
		@param	なし
	
		@retval	関数値
			ターゲットテクスチャ数

		MRTのターゲット数を取得します。
		*/
		inline int CountGetTextureTargetColor(void) const
		{
			return((int)((Status & FlagStatusBit::COUNT_TEXTURE_TARGET_PIXEL) >> ShiftStatusBit::SHIFT_COUNT_TEXTURE_TARGET_PIXEL));
		}

		/* ********************************************************* */
		//! ピクセル色用ターゲットテクスチャを設定
		/*!
		@param	index
			ターゲット番号
		@param	texture
			テクスチャ
			nullptr == フレームバッファ
		@param	flagClear
			true == ターゲットをクリアする
			false == ターゲットをクリアしない
		@param	colorClear
			ターゲットのクリア色
			※flagClearがtrueの時しか設定されません。
	
		@retval	なし

		ピクセル色用ターゲットテクスチャを設定します。
		※texture/colorClearは通常なら（入力なので）参照にするところ
			ですが、nullptrを許容するためにポインタになっています。
		*/
		inline void TextureSetTargetColor(int index, Texture* texture, bool flagClear, const CPU::Color& colorClear)
		{
			TextureTargetColor[index] = texture;

			if(true == flagClear)	{
				Status |= (Uint32)((0x00000001 << index) & FlagStatusBit::CLEAR_TARGET_PIXEL);

				/* MEMO: trueの時だけ書き換える。 */
				ColorClearTargetColor[index].Set(colorClear);
			} else {
				Status &= ~((Uint32)((0x00000001 << index) & FlagStatusBit::CLEAR_TARGET_PIXEL));
			}
		}

		/* ********************************************************* */
		//! テスト用ターゲットテクスチャを設定
		/*!
		@param	index
			ターゲット番号
		@param	texture
			テクスチャ
			nullptr == フレームバッファ
		@param	flagClear
			true == ターゲットをクリアする
			false == ターゲットをクリアしない
		@param	valueStencil
			ステンシルクリア値
			※flagClearがtrueの時しか設定されません。
		@param	valueDepth
			深度クリア値
			※flagClearがtrueの時しか設定されません。
	
		@retval	なし

		テスト用ターゲットテクスチャを設定します。
		※textureは通常なら（入力なので）参照にするところですが、
			nullptrを許容するためにポインタになっています。
		*/
		inline void TextureSetTargetTest(Texture* texture,  bool flagClear, int valueStencil, float valueDepth)
		{
			TextureTargetTest = texture;

			/* MEMO: trueの時だけ書き換える。 */
			if(true == flagClear)	{
				Status |= (Uint32)FlagStatusBit::CLEAR_TARGET_TEST;

				/* MEMO: trueの時だけ書き換える。 */
				ValueClearStencil = (Sint16)valueStencil;
				ValueClearDepth = (Float32)valueDepth;
			} else {
				Status &= ~((Uint32)FlagStatusBit::CLEAR_TARGET_TEST);
			}
		}

		/* ********************************************************* */
		//! ステンシルテストの設定
		/*!
		@param	flagTest
			true == ステンシルテストを行う
			false == ステンシルテストを行わない
		@param	kindDraw
			描画時のステンシル操作の種類
	
		@retval	なし

		ステンシルテストの動作を設定します。
		*/
		inline void TestSetStencil(bool flagTest, Renderer::SectionTest::KindTypeDraw kindDraw)
		{
			Status &= ~((Uint32)(FlagStatusBit::ACTION_STENCIL | FlagStatusBit::TEST_STENCIL));
			if(true == flagTest)	{
				Status |= FlagStatusBit::TEST_STENCIL;
			}
			Status |= ((Uint32)kindDraw) << ShiftStatusBit::SHIFT_ACTION_STENCIL;
		}

		/* ********************************************************* */
		//! 深度テストの設定
		/*!
		@param	flagTest
			true == 深度テストを行う
			false == 深度テストを行わない
		@param	flagWrite
			true == 深度をデプスバッファに書き込む
			false == 深度をデプスバッファに書き込まない
	
		@retval	なし

		深度テストの動作を設定します。
		*/
		inline void TestSetDepth(bool flagTest, bool flagWrite)
		{
			Status &= ~((Uint32)(FlagStatusBit::TEST_DEPTH | FlagStatusBit::WRITE_DEPTH));
			if(true == flagTest)	{
				Status |= (Uint32)FlagStatusBit::TEST_DEPTH;
			}
			if(true == flagWrite)	{
				Status |= (Uint32)FlagStatusBit::WRITE_DEPTH;
			}
		}

		/* ----------------------------------------------- Functions (static) */
	private:
	protected:
	public:

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
		friend class PrimitiveBatcher;
		friend class PrimitiveBatcher::Fragment;
	};

	/* フラグメント */
	/* MEMO: 本クラスは、PrimitiveBatherクラスの中の「1つの描画単位」です。       */
	/*       3Dモデルなどでは「サブメッシュ」などと呼ばれる単位とほぼ同一です。   */
	/*       ※が、用語がバラバラなため、汎用化の意味で別の言葉を使用しています。 */
	/*       原則、下記の規則を全て満たすものが1つの本クラスとして管理されます。  */
	/*       - 描画順序が連続している。                                           */
	/*       - レンダリング設定の内容が同じ。                                     */
	class Fragment :
		public SettingRender
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		Sint16 VertexTop;
		Sint16 CountVertex;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! 未使用化
		/*!
		@param	なし
	
		@retval	なし
		*/
		inline void CleanUp(void)
		{
			SettingRender::CleanUp();

			VertexTop = -1;
			CountVertex = 0;
		}

		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし
	
		@retval	なし
		*/
		Fragment(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし
	
		@retval	なし
		*/
		~Fragment(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! 指定描画方法が追加可能かの判定
		/*!
		@param	setting
			レンダリング設定
	
		@retval	関数値
			true == 追加可能（描画設定が同じ)
			false == 追加不可能（描画設定が異なる）

		指定された描画設定が（現状の頂点状態で）そのまま追加可能かを
			チェックします。
		*/
		inline bool IsAddable(const SettingRender& setting) const
		{
			/* 基本情報のチェック */
			int countTextureTargetColor = CountGetTextureTargetColor();
			if(	(Status != setting.Status)
				|| (Shader != setting.Shader)
				|| (TypeBlend != setting.TypeBlend)
				|| (TextureCellMap != setting.TextureCellMap)
				|| (countTextureTargetColor != setting.CountGetTextureTargetColor())
				|| (TextureTargetTest != setting.TextureTargetTest)
			)	{
				return(false);
			}

			/* ターゲットテクスチャのチェック */
			for(int i=0; i<countTextureTargetColor; i++)	{
				if(TextureTargetColor[i] != setting.TextureTargetColor[i])	{
					return(false);
				}
			}

			return(true);
		}

		/* ********************************************************* */
		//! 描画（GPUへの描画コマンド設定）
		/*!
		@param	commandList
			描画コマンドリスト
		@param	primitiveBatcher
			自身が所属しているPrimitiveBatcherクラス（親）
		@param	dataShader
			使用するシェーダ定数
	
		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗
	
		フラグメントを描画するための描画コマンドを定義します。
		本関数は、PrimitiveBatcher::Drawの中から呼ばれていますので、
			（原則）外部から単独で呼び出さないでください。
		*/
		TypeDrawCommandList Draw(TypeDrawCommandList commandList, PrimitiveBatcher& primitiveBatcher, const ShaderData& dataShader);

		/* ----------------------------------------------- Functions (static) */
	private:
	protected:
	public:

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum Constant	{
		VERTEX_COUNT_TRIANGLE = 3,					/* 三角形プリミティブの頂点数 */
	};

	/* ----------------------------------------------- Variables */
private:
	/* 描画用情報群 */
	Renderer::SectionStream::DataEntity DataEntity;	/* 描画用のエンティティ毎に持つ情報 */

	/* GPUバッファ群 */
	Buffer::GPUMulti DataCoordinate;				/* 頂点データ: 座標 */
	Buffer::GPUMulti DataPartsColor;				/* 頂点データ: パーツカラー */
	Buffer::GPUMulti DataUV0;						/* 頂点データ: UV0（アルベド）＋パーツカラー補佐パラメータ */
	Buffer::GPUMulti DataFlags;						/* 頂点データ: 各種フラグ群（boolをfloatにしたもの） */
	Buffer::GPUMulti DataArgumentVs00;				/* 頂点データ: 頂点シェーダ用パラメータ */
	Buffer::GPUMulti DataArgumentFs00;				/* 頂点データ: ピクセルシェーダ用パラメータA 0 */
	Buffer::GPUMulti DataArgumentFs01;				/* 頂点データ: ピクセルシェーダ用パラメータA 1 */
	Buffer::GPUMulti DataArgumentFs02;				/* 頂点データ: ピクセルシェーダ用パラメータA 2 */
	Buffer::GPUMulti DataArgumentFs03;				/* 頂点データ: ピクセルシェーダ用パラメータA 3 */
	Buffer::GPUMulti DataParameterFs00;				/* 頂点データ: ピクセルシェーダ用パラメータB 0 */
	Buffer::GPUMulti DataParameterFs01;				/* 頂点データ: ピクセルシェーダ用パラメータB 1 */
	Buffer::GPUMulti DataParameterFs02;				/* 頂点データ: ピクセルシェーダ用パラメータB 2 */
	Buffer::GPUMulti DataParameterFs03;				/* 頂点データ: ピクセルシェーダ用パラメータB 3 */

	/* GPUバッファのCPU空間にマップしたポインタ（キャッシュ） */
	/* MEMO: 本来はキャッシュする必要はない（GPUバッファはフレーム中マップアドレスは固定されているため）のですが、 */
	/*       プリミティブを追加したりする場合に、何度も取得するのも無駄なのでキャッシュしてあります。              */
	CPU::Vector3* BufferCoordinate;
	CPU::Vector4* BufferPartsColor;
	CPU::Vector4* BufferUV0;
	CPU::Vector4* BufferFlags;
	CPU::Vector4* BufferArgumentVs00;
	CPU::Vector4* BufferArgumentFs00;
	CPU::Vector4* BufferArgumentFs01;
	CPU::Vector4* BufferArgumentFs02;
	CPU::Vector4* BufferArgumentFs03;
	CPU::Vector4* BufferParameterFs00;
	CPU::Vector4* BufferParameterFs01;
	CPU::Vector4* BufferParameterFs02;
	CPU::Vector4* BufferParameterFs03;

protected:
public:
	void* WorkArea;

	/* クラスタ情報 */
	int CountFragmentMax;							/* 扱える最大フラグメント数 */
	int CountFragment;								/* 現在使用中のフラグメント数 */
	Fragment* TableFragment;

	/* 頂点データ情報 */
	int CountVertexMax;								/* 扱える最大頂点データ数 */
	int CountVertex;								/* 現在定義されている頂点データ数 */

	/* ----------------------------------------------- Functions */
private:
	/* ********************************************************* */
	//! プリミティブ使用状態を未使用化
	/*!
	@param	なし

	@retval	なし
	*/
	inline void CleanUpPrimitive(void)
	{
		CountFragment = -1;

		CountVertex = 0;
	}

protected:
public:
	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし
	*/
	inline void CleanUp(void)
	{
		DataEntity.CleanUp();

		WorkArea = nullptr;

//		DataCoordinate
//		DataPartsColor
//		DataUV0
//		DataFlags
//		DataArgumentVs00
//		DataArgumentFs00
//		DataArgumentFs01
//		DataArgumentFs02
//		DataArgumentFs03
//		DataParameterFs00
//		DataParameterFs01
//		DataParameterFs02
//		DataParameterFs03

		BufferCoordinate = nullptr;
		BufferPartsColor = nullptr;
		BufferUV0 = nullptr;
		BufferFlags = nullptr;
		BufferArgumentVs00 = nullptr;
		BufferArgumentFs00 = nullptr;
		BufferArgumentFs01 = nullptr;
		BufferArgumentFs02 = nullptr;
		BufferArgumentFs03 = nullptr;
		BufferParameterFs00 = nullptr;
		BufferParameterFs01 = nullptr;
		BufferParameterFs02 = nullptr;
		BufferParameterFs03 = nullptr;

		CountFragmentMax = -1;
//		CountFragment
		TableFragment = nullptr;

		CountVertexMax = -1;
//		CountVertex

		CleanUpPrimitive();
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	PrimitiveBatcher(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	~PrimitiveBatcher(void)
	{
//		CleanUp();
	}

	/* ********************************************************* */
	//! 本クラスが有効か
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効（起動前もしくは終了後）

	本クラスが起動状態かを取得します。
	*/
	inline bool IsValid(void)
	{
		/* MEMO: 最大フラグメント数が正常値であることで判定しています。 */
		return(0 <= CountFragmentMax);	/* ? true : false */
	}

	/* ********************************************************* */
	//! 本クラスにプリミティブを追加可能か
	/*!
	@param	なし

	@retval	関数値
		true == 追加可能（PrimitiveBegin～PrimitiveEnd区間内）
		false == 追加不可能
	*/
	inline bool IsAddablePrimitive(void)
	{
		/* MEMO: 現在簡略化してCoordinateのCPUマップがキャッシュされているかで判定しています。 */
		return(nullptr != BufferCoordinate);	/* ? true : false */
	}

	/* ********************************************************* */
	//! 起動
	/*!
	@param	countFragmentMax
		扱うことができる最大のフラグメント数
	@param	countVertexMax
		扱うことができる最大の頂点データ数

	@retval	関数値
		true == 成功
		false == 失敗（エラー）

	本クラスを初期起動します。
	*/
	bool BootUp(int countFragmentMax, int countVertexMax);

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし

	本クラスを終了します。
	一度終了した本クラスの実体を再利用する場合は、BootUp関数から
		実行しなおしてください。
	*/
	void ShutDown(void);

	/* ********************************************************* */
	//! 描画
	/*!
	@param	commandList
		描画コマンドリスト
		@param	dataShader
			使用するシェーダ定数

	@retval	関数値
		描画コマンドリスト（更新値）
		DrawCommandListInvalid == 失敗
	
	PrimitiveBegin～PrimitiveEndの区間中で構築された本クラスが
		持つバッチングしたプリミティブ群を一斉描画します。
	同じフレーム内で複数回呼び出しても構いません（疑似的なインス
		タンス描画が可能です）。
	*/
	TypeDrawCommandList Draw(TypeDrawCommandList commandList, const ShaderData& dataShader);

	/* ********************************************************* */
	//! プリミティブの構築開始
	/*!
	@param	なし

	@retval	なし

	プリミティブの構築を開始します。
	通常、各フレームでの描画構築処理の最初で一回のみ行います。

	プリミティブを構築する場合には必ず本関数～PrimitiveEndの区間
		内で行う必要があります。
	※本クラス内部にあるGPUバッファ群を本関数でCPUからアクセス可能
		にします。
	*/
	bool PrimitiveBegin(void);

	/* ********************************************************* */
	//! プリミティブの構築終了
	/*!
	@param	なし

	@retval	なし

	プリミティブの構築を終了します。

	プリミティブを構築する場合には必ずPrimitiveBegin～本関数の
		区間内で行う必要があります。
	※本クラス内部にあるGPUバッファ群を本関数でGPU区間に管理を移し
		ます。
	*/
	void PrimitiveEnd(void);

	/* ********************************************************* */
	//! プリミティブの確保
	/*!
	@param	dataCoordinate
		頂点データ:頂点座標の先頭ポインタ格納先
	@param	dataColorParts
		頂点データ:パーツカラーの先頭ポインタ格納先
	@param	dataUVTexture
		頂点データ:UV座標の先頭ポインタ格納先
	@param	dataFlags
		頂点データ:描画フラグの先頭ポインタ格納先
	@param	dataArgumentVs00
		頂点データ:頂点シェーダパラメータの先頭ポインタ格納先
	@param	dataArgumentFs00
		頂点データ:ピクセルシェーダ用引数0の先頭ポインタ格納先
	@param	dataArgumentFs01
		頂点データ:ピクセルシェーダ用引数1の先頭ポインタ格納先
	@param	dataArgumentFs02
		頂点データ:ピクセルシェーダ用引数2の先頭ポインタ格納先
	@param	dataArgumentFs03
		頂点データ:ピクセルシェーダ用引数3の先頭ポインタ格納先
	@param	dataParameterFs00
		頂点データ:ピクセルシェーダ用パラメータ0の先頭ポインタ格納先
	@param	dataParameterFs01
		頂点データ:ピクセルシェーダ用パラメータ1の先頭ポインタ格納先
	@param	dataParameterFs02
		頂点データ:ピクセルシェーダ用パラメータ2の先頭ポインタ格納先
	@param	dataParameterFs03
		頂点データ:ピクセルシェーダ用パラメータ3の先頭ポインタ格納先
	@param	countPrimitive
		確保するプリミティブ（三角形ポリゴン）数
	@param	setting
		レンダリング設定
	@param	flagBreakBatching
		true == 直前のプリミティブとレンダリング設定が同じでもバッチングを切る
		false == 直前のプリミティブとレンダリング設定が同じならバッチングする

	@retval	関数値
		確保された頂点データ数
		0 < 成功（頂点が確保された）
		0 == 警告（定義可能頂点数オーバー or 確保頂点数が存在しない）
		-1 == 失敗（エラー）
	@retval	dataCoordinate
		頂点データ:頂点座標の先頭ポインタ
	@retval	dataColorParts
		頂点データ:パーツカラーの先頭ポインタ
	@retval	dataUVTexture
		頂点データ:UV座標の先頭ポインタ
	@retval	dataFlags
		頂点データ:描画フラグの先頭ポインタ
	@retval	dataArgumentVs00
		頂点データ:頂点シェーダパラメータの先頭ポインタ
	@retval	dataArgumentFs00
		頂点データ:ピクセルシェーダ用引数0の先頭ポインタ
	@retval	dataArgumentFs01
		頂点データ:ピクセルシェーダ用引数1の先頭ポインタ
	@retval	dataArgumentFs02
		頂点データ:ピクセルシェーダ用引数2の先頭ポインタ
	@retval	dataArgumentFs03
		頂点データ:ピクセルシェーダ用引数3の先頭ポインタ
	@retval	dataParameterFs00
		頂点データ:ピクセルシェーダ用パラメータ0の先頭ポインタ
	@retval	dataParameterFs01
		頂点データ:ピクセルシェーダ用パラメータ1の先頭ポインタ
	@retval	dataParameterFs02
		頂点データ:ピクセルシェーダ用パラメータ2の先頭ポインタ
	@retval	dataParameterFs03
		頂点データ:ピクセルシェーダ用パラメータ3の先頭ポインタ

	プリミティブを構成する頂点データを確保して、それぞれのデータを設定
		するためのポインタを返します（頂点の追加は本関数で行います）。
	本関数は必ずPrimitiveBegin～PrimitiveEndの区間内で行う必要があります。

	本関数は、PrimitiveAdd系関数の内部処理関数的位置づけですが、念のため
		公開状態にしてあります（特殊なプリミティブが生じた時の予備です）。
	通常は、各描画に適したPrimitiveAdd系関数を使用してください。

	返値の各ポインタは配列として機能し、配列添字での役目は下記になります。
	[0] : プリミティブ-0 頂点-0
	[1] : プリミティブ-0 頂点-1 
	[2] : プリミティブ-0 頂点-2
	[3] : プリミティブ-1 頂点-0
	[4] : プリミティブ-1 頂点-1 
	[5] : プリミティブ-1 頂点-2
	[6] : プリミティブ-2 頂点-0
	……

	原則、返る各ポインタは恒久的なものではありません（最長でも同一フレー
		ム中でしか保証がありません）ので注意してください。

	また、本関数はプリミティブに対応した頂点データの領域を確保する処理
		しか行いません（返るインタの実体の内容は初期化されていません）。
	*/
	int PrimitiveAllocate(	CPU::Vector3** dataCoordinate,
							CPU::Color** dataColorParts,
							CPU::Vector4** dataUVTexture,
							CPU::Vector4** dataFlags,
							CPU::Vector4** dataArgumentVs00,
							CPU::Vector4** dataArgumentFs00,
							CPU::Vector4** dataArgumentFs01,
							CPU::Vector4** dataArgumentFs02,
							CPU::Vector4** dataArgumentFs03,
							CPU::Vector4** dataParameterFs00,
							CPU::Vector4** dataParameterFs01,
							CPU::Vector4** dataParameterFs02,
							CPU::Vector4** dataParameterFs03,
							int countPrimitive,
							const SettingRender& setting,
							bool flagBreakBatching
						);

	/* ********************************************************* */
	//! 4分割四角プリミティブの追加
	/*!
	@param	setting
		レンダリング設定
	@param	dataCoordinate
		頂点データ:頂点座標（配列）の先頭ポインタ格納先
	@param	dataColorParts
		頂点データ:パーツカラー（配列）の先頭ポインタ格納先
	@param	dataUVTexture
		頂点データ:UV座標（配列）の先頭ポインタ格納先
	@param	dataFlags
		頂点データ:描画フラグ（単独データ）
	@param	dataArgumentVs00
		頂点データ:頂点シェーダパラメータ（配列）の先頭ポインタ格納先
	@param	dataArgumentFs00
		頂点データ:ピクセルシェーダ用引数0（単独データ）
	@param	dataArgumentFs01
		頂点データ:ピクセルシェーダ用引数1（単独データ）
	@param	dataArgumentFs02
		頂点データ:ピクセルシェーダ用引数2（単独データ）
	@param	dataArgumentFs03
		頂点データ:ピクセルシェーダ用引数3（単独データ）
	@param	dataParameterFs00
		頂点データ:ピクセルシェーダ用パラメータ0（単独データ）
	@param	dataParameterFs01
		頂点データ:ピクセルシェーダ用パラメータ1（単独データ）
	@param	dataParameterFs02
		頂点データ:ピクセルシェーダ用パラメータ2（単独データ）
	@param	dataParameterFs03
		頂点データ:ピクセルシェーダ用パラメータ3（単独データ）
	@param	flagBreakBatching
		true == 直前のプリミティブとレンダリング設定が同じでもバッチングを切る
		false == 直前のプリミティブとレンダリング設定が同じならバッチングする

	@retval	関数値
		定義された頂点データ数
		0 < 成功（頂点が確保された）
		0 == 警告（定義可能頂点数オーバー or 確保頂点数が存在しない）
		-1 == 失敗（エラー）

	通常パーツ・マスクパーツなどの4頂点平面を中心点で4三角形分割
		したプリミティブを、頂点インデックス形式で定義します。

	頂点データの「配列」の格納順序はLibrary::KindVertex::Kindの
		並びに準じ・必ずLibrary::KindVertex::TERMINATOR2個必要に
		なります。
	※「単独データ」については、その値が全頂点に適用されます。
	*/
	int PrimitiveAddQuadrilateral4(	const SettingRender& setting,
									const CPU::Vector3* dataCoordinate,
									const CPU::Color* dataColorParts,
									const CPU::Vector4* dataUVTexture,
									const CPU::Vector4& dataFlags,
									const CPU::Vector4* dataArgumentVs00,
									const CPU::Vector4& dataArgumentFs00,
									const CPU::Vector4& dataArgumentFs01,
									const CPU::Vector4& dataArgumentFs02,
									const CPU::Vector4& dataArgumentFs03,
									const CPU::Vector4& dataParameterFs00,
									const CPU::Vector4& dataParameterFs01,
									const CPU::Vector4& dataParameterFs02,
									const CPU::Vector4& dataParameterFs03,
									bool flagBreakBatching
							);

	/* ********************************************************* */
	//! 2分割四角プリミティブの追加
	/*!
	@param	setting
		レンダリング設定
	@param	dataCoordinate
		頂点データ:頂点座標（配列）
	@param	dataColorParts
		頂点データ:パーツカラー（配列）
	@param	dataUVTexture
		頂点データ:UV座標（配列）
	@param	dataFlags
		頂点データ:描画フラグ（単独データ）
	@param	dataArgumentVs00
		頂点データ:頂点シェーダパラメータ（配列）
	@param	dataArgumentFs00
		頂点データ:ピクセルシェーダ用引数0（単独データ）
	@param	dataArgumentFs01
		頂点データ:ピクセルシェーダ用引数1（単独データ）
	@param	dataArgumentFs02
		頂点データ:ピクセルシェーダ用引数2（単独データ）
	@param	dataArgumentFs03
		頂点データ:ピクセルシェーダ用引数3（単独データ）
	@param	dataParameterFs00
		頂点データ:ピクセルシェーダ用パラメータ0（単独データ）
	@param	dataParameterFs01
		頂点データ:ピクセルシェーダ用パラメータ1（単独データ）
	@param	dataParameterFs02
		頂点データ:ピクセルシェーダ用パラメータ2（単独データ）
	@param	dataParameterFs03
		頂点データ:ピクセルシェーダ用パラメータ3（単独データ）
	@param	flagBreakBatching
		true == 直前のプリミティブとレンダリング設定が同じでもバッチングを切る
		false == 直前のプリミティブとレンダリング設定が同じならバッチングする

	@retval	関数値
		定義された頂点データ数
		0 < 成功（頂点が確保された）
		0 == 警告（定義可能頂点数オーバー or 確保頂点数が存在しない）
		-1 == 失敗（エラー）

	4頂点平面をLU-RD対角線で4三角形分割したプリミティブを、頂点イン
		デックス形式で定義します。

	本関数は現在未実装で、動作は実装されていません（予約関数です）。
	※パーティクル用関数はPrimitiveAddParticleとして特殊実装しています。
	*/
	int PrimitiveAddQuadrilateral2(	const SettingRender& setting,
									const CPU::Vector3* dataCoordinate,
									const CPU::Color* dataColorParts,
									const CPU::Vector4* dataUVTexture,
									const CPU::Vector4& dataFlags,
									const CPU::Vector4* dataArgumentVs00,
									const CPU::Vector4& dataArgumentFs00,
									const CPU::Vector4& dataArgumentFs01,
									const CPU::Vector4& dataArgumentFs02,
									const CPU::Vector4& dataArgumentFs03,
									const CPU::Vector4& dataParameterFs00,
									const CPU::Vector4& dataParameterFs01,
									const CPU::Vector4& dataParameterFs02,
									const CPU::Vector4& dataParameterFs03,
									bool flagBreakBatching
							);

	/* ********************************************************* */
	//! 汎用メッシュプリミティブの追加
	/*!
	@param	setting
		レンダリング設定
	@param	tableIndex
		頂点インデックス（配列）
	@param	countPrimitive
		描画プリミティブ数
	@param	dataCoordinate
		頂点データ:頂点座標（配列）
	@param	dataColorParts
		頂点データ:パーツカラー（配列）
	@param	dataUVTexture
		頂点データ:UV座標（配列）
	@param	dataFlags
		頂点データ:描画フラグ（単独データ）
	@param	dataArgumentVs00
		頂点データ:頂点シェーダパラメータ（配列）の先頭ポインタ格納先
	@param	dataArgumentFs00
		頂点データ:ピクセルシェーダ用引数0（単独データ）
	@param	dataArgumentFs01
		頂点データ:ピクセルシェーダ用引数1（単独データ）
	@param	dataArgumentFs02
		頂点データ:ピクセルシェーダ用引数2（単独データ）
	@param	dataArgumentFs03
		頂点データ:ピクセルシェーダ用引数3（単独データ）
	@param	dataParameterFs00
		頂点データ:ピクセルシェーダ用パラメータ0（単独データ）
	@param	dataParameterFs01
		頂点データ:ピクセルシェーダ用パラメータ1（単独データ）
	@param	dataParameterFs02
		頂点データ:ピクセルシェーダ用パラメータ2（単独データ）
	@param	dataParameterFs03
		頂点データ:ピクセルシェーダ用パラメータ3（単独データ）
	@param	flagBreakBatching
		true == 直前のプリミティブとレンダリング設定が同じでもバッチングを切る
		false == 直前のプリミティブとレンダリング設定が同じならバッチングする

	@retval	関数値
		定義された頂点データ数
		0 < 成功（頂点が確保された）
		0 == 警告（定義可能頂点数オーバー or 確保頂点数が存在しない）
		-1 == 失敗（エラー）

	エフェクトのパーティクルなどの4頂点平面をLU-RD対角線で4三角形
		分割したプリミティブを、頂点インデックス形式で定義します。
	配列内の格納順序はtableIndexで使用している値（0～n）である必要
		があり・各頂点データの配列長はn+1個存在している必要があり
		ます。

	countPrimitiveはtableIndexに「いくつのプリミティブが記述されいる
		か」ですので、tableIndexの配列長の1/3の値であり・かつ整数に
		割り切れた値になります（1/3した時に整数で割り切れないような
		テーブルは頂点インデックスとしては異常です）。

	本関数をPrimitiveAddQuadrilateral4・2の代用とすることも可能です
		が、内部処理が最適化されていないため、同関数群に比べると処理
		速度が（少しではありますが）落ちます。
	※ですので、メッシュパーツ以外での使用は、原則避けてください。
	*/
	int PrimitiveAddMesh(	const SettingRender& setting,
							const Uint16* tableIndex,
							int countPrimitive,
							const CPU::Vector3* dataCoordinate,
							const CPU::Color* dataColorParts,
							const CPU::Vector4* dataUVTexture,
							const CPU::Vector4& dataFlags,
							const CPU::Vector4* dataArgumentVs00,
							const CPU::Vector4& dataArgumentFs00,
							const CPU::Vector4& dataArgumentFs01,
							const CPU::Vector4& dataArgumentFs02,
							const CPU::Vector4& dataArgumentFs03,
							const CPU::Vector4& dataParameterFs00,
							const CPU::Vector4& dataParameterFs01,
							const CPU::Vector4& dataParameterFs02,
							const CPU::Vector4& dataParameterFs03,
							bool flagBreakBatching
					);

	/* ********************************************************* */
	//! （エフェクト）パーティクルのプリミティブの追加
	/*!
	@param	setting
		レンダリング設定
	@param	dataCoordinate
		頂点データ:頂点座標（配列）の先頭ポインタ格納先
	@param	dataColorParts
		頂点データ:パーツカラー
	@param	dataUVTexture
		頂点データ:UV座標（配列）の先頭ポインタ格納先
	@param	dataFlags
		頂点データ:描画フラグ（単独データ）
	@param	dataArgumentVs00
		頂点データ:頂点シェーダパラメータ（配列）の先頭ポインタ格納先
	@param	dataArgumentFs00
		頂点データ:ピクセルシェーダ用引数0（単独データ）
	@param	dataArgumentFs01
		頂点データ:ピクセルシェーダ用引数1（単独データ）
	@param	dataArgumentFs02
		頂点データ:ピクセルシェーダ用引数2（単独データ）
	@param	dataArgumentFs03
		頂点データ:ピクセルシェーダ用引数3（単独データ）
	@param	dataParameterFs00
		頂点データ:ピクセルシェーダ用パラメータ0（単独データ）
	@param	dataParameterFs01
		頂点データ:ピクセルシェーダ用パラメータ1（単独データ）
	@param	dataParameterFs02
		頂点データ:ピクセルシェーダ用パラメータ2（単独データ）
	@param	dataParameterFs03
		頂点データ:ピクセルシェーダ用パラメータ3（単独データ）
	@param	flagBreakBatching
		true == 直前のプリミティブとレンダリング設定が同じでもバッチングを切る
		false == 直前のプリミティブとレンダリング設定が同じならバッチングする

	@retval	関数値
		定義された頂点データ数
		0 < 成功（頂点が確保された）
		0 == 警告（定義可能頂点数オーバー or 確保頂点数が存在しない）
		-1 == 失敗（エラー）

	エフェクトのパーティクル用の4頂点平面をLU-RD対角線で4三角形分割
		したプリミティブを、頂点インデックス形式で定義します。
	※本来はPrimitiveAddQuadrilateral2での描画すべきですが、転送量
		などを簡略するために専用実装してあるだけです。

	頂点データの「配列」の格納順序はLibrary::KindVertex::Kindの
		並びに準じ・必ずLibrary::KindVertex::TERMINATOR2個必要に
		なります。
	※「単独データ」については、その値が全頂点に適用されます。
	*/
	int PrimitiveAddParticle(	const SettingRender& setting,
								const CPU::Vector3* dataCoordinate,
								const CPU::Color& dataColorParts,
								const CPU::Vector4* dataUVTexture,
								const CPU::Vector4& dataFlags,
								const CPU::Vector4& dataArgumentVs00,
								const CPU::Vector4& dataArgumentFs00,
								const CPU::Vector4& dataArgumentFs01,
								const CPU::Vector4& dataArgumentFs02,
								const CPU::Vector4& dataArgumentFs03,
								const CPU::Vector4& dataParameterFs00,
								const CPU::Vector4& dataParameterFs01,
								const CPU::Vector4& dataParameterFs02,
								const CPU::Vector4& dataParameterFs03,
								bool flagBreakBatching
						);

	/* ********************************************************* */
	//! プリミティブ種別で使用する頂点数を取得
	/*!
	@param	kind
		プリミティブ（頂点）種別

	@retval	関数値
		頂点数

	プリミティブで使用する頂点数を取得します。
		kindには必ずTERMINATOR2かTEMINATOR4を与えてください。
	※TERMINATOR3（メッシュ）の場合には、メッシュバインドの情報
		から頂点数を取得するようにしてください。
	*/
	static inline int CountGetVertex(Library::KindVertex::Kind kindVertex)
	{
		return(	(Library::KindVertex::TERMINATOR2 == kindVertex)
				? (2 * Constant::VERTEX_COUNT_TRIANGLE)	/* 3頂点 x 2プリミティブ = 6 */
				: (4 * Constant::VERTEX_COUNT_TRIANGLE)	/* 3頂点 x 4プリミティブ = 12 */
			);
	}

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};


/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
