/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerでレンダリングする際に描画環境を扱うための基礎機能です。          */
/* 描画コマンドリストの基礎定義関係もここで一緒に扱っています。               */
/* ※描画コマンドの定義量などの最適化の関係から、クラス化しています。         */
/*                                                                            */
/* 本レンダラステート群については、汎用ではありません。                       */
/* 完全汎用的に内部にステートを作成すると、ステートなどで使用するメモリが莫大 */
/* になる可能性が低くないため、SS6Playerの使用範囲内に限定してステートを絞り  */
/* 込んでいます。                                                             */
/*                                                                            */
/* テクスチャサンプラについては、本クラス実装内に隠蔽されている形です（テクス */
/* チャ側に隠蔽する方法もありはしましたが、かえって迂遠な実装になりそうだった */
/* ため、レンダラ側に内包しました）。                                         */
/* また、GPUリソースキャッシュなどで動的に管理するわけではなく、必要とする組  */
/* み合わせが高々「補間モード×ラップモード」個なので静的に作成して、ディスク */
/* リプタに登録しています。                                                   */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_types.h"

/* 前方宣言 */
namespace SpriteStudio6	{
class Texture;
class Shader;
class ShaderData;

namespace Buffer	{
/* MEMO: GPUSingleもGPUMultiも同列で扱うために基底だけ扱っています。 */
class GPUBase;
}	/* Buffer */

}	/* SpriteStudio6 */

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 描画コマンドリスト */
/* MEMO: 機種毎に描画コマンドリストの型が違うので、受渡にはvoid*を経由しています。   */
/*       ※キャストにはreinterpret_castを使用しています（static_castではないです）。 */
typedef void* TypeDrawCommandList;							/* 描画コマンドリストの型 */
extern const TypeDrawCommandList DrawCommandListInvalid;	/* 描画コマンドリストの不正値 *//* ※本来は変数ですが解り易いのでここで定義しています */

/* GPUアドレス型 */
/* MEMO: 主にVRAMをCPUから覗く場合の仮想配置アドレスとして使用します。 */
typedef void* TypeAddressGPU;

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

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* レンダラ制御クラス */
class Renderer
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* 頂点ストリーム制御 */
	class SectionStream
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		/* 描画エンティティ情報 */
		/* MEMO: 頂点ストリームでの描画に必要な情報ををまとめて保持しておくためのオブジェクトです。 */
		/*       各頂点ストリームに与える以外に必要な情報が保持されています。                       */
		/*       原則として、本クラスは単なるオブジェクトとしてのみ存在しており、（一部の関数を除い */
		/*       て）内容はプラットフォーム依存します（プラットフォーム間で互換性がありません）。   */
		/*       そのため、Common以上のモジュールからは本クラスの内容に対して操作・参照などを行って */
		/*       おらず、単なる抽象的なオブジェクトとして扱われています。                           */
		/*       ※つまり、本クラスの実装のほとんどはプラットフォーム依存しています。               */
		/*       ※本クラスに格納する例としては、頂点宣言情報や（OpenGLで言うところの）頂点配列オブ */
		/*         ジェクトなどを格納することが想定されています（これは例であって、必ずしもそう使わ */
		/*         れているとは限りませんし、プラットフォームによっては内容が空である場合もあり得る */
		/*         点については、あらかじめ注意して下さい）。                                       */
		/*       ※現実としては、本クラスの実体はCommon内PrimitiveBatcherがprivateで包含しており、  */
		/*         初期化時にBootUpを・解放時にShutDownを行っている以外については、単にストリーム制 */
		/*         御に対してRenderer::SectionStream::EntitySet関数でオブジェクトを設定しているだけ */
		/*         の扱いしかしていません。                                                         */
		class DataEntity
		{
			/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
		private:
		protected:
		public:

			/* ----------------------------------------------- Enums & Constants */
		private:
			enum Constant	{
				COUNT_VAO = 2,								/* 所持しているVAOの数 *//* 表裏で2バッファ */
			};

		protected:
		public:

			/* ----------------------------------------------- Variables */
		private:
		protected:
		public:
			/* MEMO: メンバについてはPlatformの各クラス間で利用できるように */
			/*       publicになっているだけで、プラットフォーム間での互換性 */
			/*       はありません。                                         */
			GLuint IDVertexArrayObject[Constant::COUNT_VAO];	/* VAO */

			/* ----------------------------------------------- Functions */
		private:
		protected:
		public:
			/* MEMO: publicの関数群については、下記の5関数のみプラットフォーム間 */
			/*       での関数型の互換が必要です。                                */
			/*       - DataEntity(void);                                         */
			/*       - ~DataEntity(void);                                        */
			/*       - void CleanUp(void);                                       */
			/*       - bool BootUp(void);                                        */
			/*       - void ShutDown(void);                                      */

			/* ********************************************************* */
			//! コンストラクタ
			/*!
			@param	なし

			@retval	なし
			*/
			DataEntity(void);

			/* ********************************************************* */
			//! デストラクタ
			/*!
			@param	なし

			@retval	なし
			*/
			~DataEntity(void);

			/* ********************************************************* */
			//! 未使用化
			/*!
			@param	なし

			@retval	なし

			本クラスの内容を未使用状態にします。
			本関数は、原則PrimitiveBatcherの内部処理以外からは呼び出さないでください。
			*/
			void CleanUp(void);

			/* ********************************************************* */
			//! 起動
			/*!
			@param	なし

			@retval	関数値
				true == 成功
				false == 失敗

			描画情報を初期起動します。
			本関数は、原則PrimitiveBatcher.BootUp以外からは呼び出さないでください。
			*/
			bool BootUp(void);

			/* ********************************************************* */
			//! 終了
			/*!
			@param	なし

			@retval	なし

			描画情報を終了します。
			本関数は、原則PrimitiveBatcher.ShutDown以外からは呼び出さないでください。
			*/
			void ShutDown(void);

			/* ----------------------------------------------- Functions (OpenGL only) */

			/* ----------------------------------------------- Operators */

			/* ----------------------------------------------- Friends */
		};

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		/* 頂点データバインディング */
		/* MEMO: 本値はシェーダの頂点バインドと同じ順序である必要があります（頂点バインドと直結した値のため）。 */
		enum KindDataVertex	{
														/* -------------------- 必須データ */
			DATA_COORDINATE = 0,						/* 頂点座標 */
			DATA_UV,									/* テクスチャUV, パーツカラー合成種別, パーツカラー補佐 */
			DATA_PARTSCOLOR,							/* パーツカラー */
			DATA_FLAGS,									/* 各種フラグ群（boolをfloatにしたもの） */
			DATA_ARGUMENT_VS00,							/* 頂点シェーダ用パラメータ */
			DATA_ARGUMENT_FS00,							/* ピクセルシェーダ用パラメータA 0 */
			DATA_ARGUMENT_FS01,							/* ピクセルシェーダ用パラメータA 1 */
			DATA_ARGUMENT_FS02,							/* ピクセルシェーダ用パラメータA 2 */
			DATA_ARGUMENT_FS03,							/* ピクセルシェーダ用パラメータA 3 */
			DATA_PARAMETER_FS00,						/* ピクセルシェーダ用パラメータB 0 */
			DATA_PARAMETER_FS01,						/* ピクセルシェーダ用パラメータB 1 */
			DATA_PARAMETER_FS02,						/* ピクセルシェーダ用パラメータB 2 */
			DATA_PARAMETER_FS03,						/* ピクセルシェーダ用パラメータB 3 */

			DATA_TERMINATOR_REQUIRED,					/* 必須データの番兵（使用禁止） */
														/* -------------------- 任意データ *//* ※必ずDATA_TERMINATOR_REQUIRED以上の実値にすること */
			DATA_TERMINATOR = DATA_TERMINATOR_REQUIRED,	/* 拡張データの番兵（使用禁止） */
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: 下記は直接設定しないで関数を使用して設定してください。          */
		/*       ※本来はprivateにするべきですが、プラットフォーム共通項目なので */
		/*         その明示のためだけにpublicにしてあります。                    */
		bool FlagDirtyEntity;											/* 変更された（=描画コマンドを再度作成する必要がある）か？ */
		DataEntity* Entity;												/* 描画エンティティ情報 */

		bool FlagDirty[KindDataVertex::DATA_TERMINATOR];				/* 変更された（=描画コマンドを再度作成する必要がある）か？ */
		Buffer::GPUBase* BufferSource[KindDataVertex::DATA_TERMINATOR];	/* ストリームに設定されている頂点バッファ */
		size_t SizeStride[KindDataVertex::DATA_TERMINATOR];				/* データのストライド（単位）サイズ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		SectionStream(void);

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		~SectionStream(void);

		/* ********************************************************* */
		//! 起動
		/*!
		@param	なし

		@retval	なし
		*/
		void BootUp(void);

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし
		*/
		void ShutDown(void);

		/* ********************************************************* */
		//! リフレッシュ
		/*!
		@param	なし

		@retval	なし

		本クラスを起動状態に戻します。
		※レンダリング時に同じインスタンスを使用する際に、描画サイクル
			の変更時や描画オブジェクトの変更時に適時使用してください。
		*/
		inline void Refresh(void)
		{
			FlagDirtyEntity = false;
			Entity = nullptr;

			for(int i=0; i<KindDataVertex::DATA_TERMINATOR; i++)	{
				FlagDirty[i] = false;
				BufferSource[i] = nullptr;
				SizeStride[i] = 0;
			}
		}

		/* ********************************************************* */
		//! 描画エンティティ情報設定
		/*!
		@param	dataEntity
			設定するエンティティ情報
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		描画に使用するエンティティ情報を設定します。
		*/
		inline void EntitySet(DataEntity& entity, bool flagForce=false)
		{
#if 0	/* MEMO: 元処理 */
			if(false == flagForce)	{	/* 強制変更しない */
				if(Entity == &entity)	{	/* 変更の必要なし */
					return;
				}
			}
#else	/* MEMO: PrimitiveBatcherができたので、FlagDirtyがいらないのでは？ */
#endif
			Entity = &entity;
			FlagDirtyEntity = true;
		}

		/* ********************************************************* */
		//! 設定
		/*!
		@param	buffer
			設定するGPUバッファ
		@param	kind
			設定する頂点ストリーム種別（シェーダでバインドされた頂点番号）
		@param	sizeStride
			データのストライド
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		バッファを頂点ストリームとして設定します。
		転送される容量はバッファ（の1バンク）のバイトサイズです（つま
			りバッファ1バンクの全データがそのまま設定されます）。
		以前と設定が異なる場合、内部的にフラグを立てます（このフラグ
			はMakeCommand内で判定されます）。
		*/
		inline void Set(Buffer::GPUBase& buffer, KindDataVertex kind, size_t sizeStride, bool flagForce=false)
		{
			if((0 > kind) || (KindDataVertex::DATA_TERMINATOR <= kind))	{	/* 種別が不正 */
				return;
			}

#if 0	/* MEMO: 元処理 */
			if(false == flagForce)	{	/* 強制変更しない */
				if((BufferSource[kind] == &buffer) && (SizeStride[kind] == sizeStride))	{	/* 変更の必要なし */
					return;
				}
			}
#else	/* MEMO: PrimitiveBatcherができたので、FlagDirtyがいらないのでは？ */
#endif

			BufferSource[kind] = &buffer;
			SizeStride[kind] = sizeStride;
			FlagDirty[kind] = true;
		}

		/* ********************************************************* */
		//! 描画コマンドを作成
		/*!
		@param	commandList
			描画コマンドリスト

		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗

		現在設定を反映するために描画コマンドリストに定義します。
		*/
		TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* シェーダ制御 */
	class SectionShader
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
		/* MEMO: 下記は直接設定しないで関数を使用して設定してください。          */
		/*       ※本来はprivateにするべきですが、プラットフォーム共通項目なので */
		/*         その明示のためだけにpublicにしてあります。                    */
		bool FlagDirty;									/* 変更された（=描画コマンドを再度作成する必要がある）か？ */
		const Shader* InstanceShader;					/* 現在設定してあるシェーダ */

		bool FlagDirtyData;								/* データが変更された（=描画コマンドを再度作成する必要がある）か？ */
		const ShaderData* Data;							/* シェーダ定数: エンティティ用 */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		SectionShader(void);

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		~SectionShader(void);

		/* ********************************************************* */
		//! 起動
		/*!
		@param	なし

		@retval	なし
		*/
		void BootUp(void);

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし
		*/
		void ShutDown(void);

		/* ********************************************************* */
		//! リフレッシュ
		/*!
		@param	なし

		@retval	なし

		本クラスを起動状態に戻します。
		※レンダリング時に同じインスタンスを使用する際に、描画サイクル
			の変更時や描画オブジェクトの変更時に適時使用してください。
		*/
		inline void Refresh(void)
		{
			FlagDirty = false;
			InstanceShader = nullptr;

			FlagDirtyData = false;
			Data = nullptr;
		}

		/* ********************************************************* */
		//! 設定
		/*!
		@param	shader
			設定するシェーダ
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		描画に使用するシェーダを設定します。
		以前と設定が異なる場合、内部的にフラグを立てます（このフラグ
			はMakeCommand内で判定されます）。
		*/
		inline void Set(const Shader& shader, bool flagForce=false)
		{
#if 0	/* MEMO: 元処理 */
			if(false == flagForce)	{	/* 強制変更しない */
				if(InstanceShader == &shader)	{	/* 変更の必要なし */
					return;
				}
			}

			InstanceShader = &shader;
			FlagDirty = true;
#else	/* MEMO: PrimitiveBatcherができたので、FlagDirtyがいらないのでは？ */
			InstanceShader = &shader;
			FlagDirty = true;
#endif
		}

		/* ********************************************************* */
		//! データ（シェーダ定数）の設定
		/*!
		@param	data
			設定するデータ（シェーダ定数）
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		描画に使用するシェーダ定数を設定します。
		以前と設定が異なる場合、内部的にフラグを立てます（このフラグ
			はMakeCommand内で判定されます）。
		*/
		inline void DataSet(const ShaderData& data, bool flagForce=false)
		{
#if 0	/* MEMO: 元処理 */
			if(false == flagForce)	{	/* 強制変更しない */
				if(Data == &data)	{	/* 変更の必要なし */
					return;
				}
			}

			Data = &data;
			FlagDirtyData = true;
#else	/* MEMO: PrimitiveBatcherができたので、FlagDirtyがいらないのでは？ */
			Data = &data;
			FlagDirtyData = true;
#endif
		}

		/* ********************************************************* */
		//! 描画コマンドを作成
		/*!
		@param	commandList
			描画コマンドリスト

		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗

		現在設定を反映するために描画コマンドリストに定義します。
		*/
		TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ラスタライザ制御 */
	class SectionRasterizer
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
		/* MEMO: 下記は直接設定しないで関数を使用して設定してください。          */
		/*       ※本来はprivateにするべきですが、プラットフォーム共通項目なので */
		/*         その明示のためだけにpublicにしてあります。                    */
		bool FlagDirty;									/* 変更された（=描画コマンドを再度作成する必要がある）か？ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		SectionRasterizer(void);

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		~SectionRasterizer(void);

		/* ********************************************************* */
		//! 起動
		/*!
		@param	なし

		@retval	なし
		*/
		void BootUp(void);

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし
		*/
		void ShutDown(void);

		/* ********************************************************* */
		//! リフレッシュ
		/*!
		@param	なし

		@retval	なし

		本クラスを起動状態に戻します。
		※レンダリング時に同じインスタンスを使用する際に、描画サイクル
			の変更時や描画オブジェクトの変更時に適時使用してください。
		*/
		inline void Refresh(void)
		{
			FlagDirty = false;
		}

		/* ********************************************************* */
		//! ラスタライザステートの出力設定
		/*!
		@param	なし

		@retval	なし

		ラスタライザステートを出力するかを設定します。
		*/
		inline void Set(void)
		{
			FlagDirty = true;
		}

		/* ********************************************************* */
		//! 描画コマンドを作成
		/*!
		@param	commandList
			描画コマンドリスト

		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗

		現在設定を反映するために描画コマンドリストに定義します。
		*/
		TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* テスト制御（ステンシル・デプス） */
	class SectionTest
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum KindTypeDraw	{
			NORMAL = 0,									/* 通常描画 */
			MASK_PREDRAW,								/* 描画前描画時マスク描画 */
			MASK,										/* 通常描画時マスク描画 */

			TERMINATOR,									/* 番兵（使用禁止） */
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: 下記は直接設定しないで関数を使用して設定してください。          */
		/*       ※本来はprivateにするべきですが、プラットフォーム共通項目なので */
		/*         その明示のためだけにpublicにしてあります。                    */
		bool FlagDirty;									/* 変更された（=描画コマンドを再度作成する必要がある）か？ */

		KindTypeDraw TypeDraw;							/* 描画種別 */
		bool FlagMaskingStencil;						/* マスキングを行う *//* ※マスキングを行うとステンシルを判定します */
		bool FlagEnableReadDepth;						/* デプステストを行うか？ */
		bool FlagEnableWriteDepth;						/* デプス値を書き込むか？ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		SectionTest(void);

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		~SectionTest(void);

		/* ********************************************************* */
		//! 起動
		/*!
		@param	なし

		@retval	なし
		*/
		void BootUp(void);

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし
		*/
		void ShutDown(void);

		/* ********************************************************* */
		//! リフレッシュ
		/*!
		@param	なし

		@retval	なし

		本クラスを起動状態に戻します。
		※レンダリング時に同じインスタンスを使用する際に、描画サイクル
			の変更時や描画オブジェクトの変更時に適時使用してください。
		*/
		inline void Refresh(void)
		{
			FlagDirty = false;

			TypeDraw = KindTypeDraw::NORMAL;
			FlagMaskingStencil = false;
			FlagEnableReadDepth = false;
			FlagEnableWriteDepth = false;
		}

		/* ********************************************************* */
		//! ステンシル設定
		/*!
		@param	typeDraw
			描画方法
		@param	flagMasking
			マスクを考慮して描画する
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		ステンシルテスト設定を切り替えます。
		以前と設定が異なる場合、内部的にフラグを立てます（このフラグ
			はMakeCommand内で判定されます）。

		flagMaskingは現在効果しているマスキング状態が描画に影響する
			（マスクが適用される）際にtrueにし・マスキングを適用しない
			際にfalseにします。
		*/
		inline void StencilSet(KindTypeDraw typeDraw, bool flagMasking, bool flagForce=false)
		{
#if 0	/* MEMO: 元処理 */
			if(false == flagForce)	{	/* 強制変更しない */
				if(	(typeDraw == TypeDraw)
					&& (flagMasking == FlagMaskingStencil)
				)	{	/* 変更の必要なし */
					return;
				}
			}
#else	/* MEMO: PrimitiveBatcherができたので、FlagDirtyがいらないのでは？ */
#endif

			TypeDraw = typeDraw;
			FlagMaskingStencil = flagMasking;
			FlagDirty = true;
		}

		/* ********************************************************* */
		//! デプス設定
		/*!
		@param	flagEnableRead
			深度読込（テスト）を行う
		@param	flagEnableWrite
			深度書込を行う
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		デプステスト設定を切り替えます。
		以前と設定が異なる場合、内部的にフラグを立てます（このフラグ
			はMakeCommand内で判定されます）。
		*/
		inline void DepthSet(bool flagEnableRead, bool flagEnableWrite, bool flagForce=false)
		{
			if(false == flagForce)	{	/* 強制変更しない */
				if(	(flagEnableRead == FlagEnableReadDepth)
					&& (flagEnableWrite == FlagEnableWriteDepth)
				)	{	/* 変更の必要なし */
					return;
				}
			}

			FlagEnableReadDepth = flagEnableRead;
			FlagEnableWriteDepth = flagEnableWrite;
			FlagDirty = true;
		}

		/* ********************************************************* */
		//! 描画コマンドを作成
		/*!
		@param	commandList
			描画コマンドリスト

		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗

		現在設定を反映するために描画コマンドリストに定義します。
		*/
		TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ブレンド制御 */
	class SectionBlend
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		/* ブレンド種別 */
		/* MEMO: この値はそのままLibrary::KindOperationBlend::Kindで使用していますので、原則変更しないでください。 */
		/* MEMO: MIX～ADDの並びがSpriteStudio6の並びと異なるのですが……SpriteStudio5からの並びを */
		/*       （連番として）受け継いでいる関係から、本順序になっています。                     */
		enum KindType	{
			/* マスク描画用 */
			MASK_PRE = -2,								/* マスク用描画前描画用 */
			MASK = -1,									/* マスク描画用 */

			/* 通常描画用 */
			NONE = -1,									/* エラー用 */

			MIX = 0,									/* 通常 */
			ADD,										/* 加算 */
			SUB,										/* 減算 */
			MUL,										/* アルファ乗算 */

			MUL_NA,										/* （アルファなし）乗算 */
			SCR,										/* スクリーン */
			EXC,										/* 除外 */
			INV,										/* 反転 */

			TERMINATOR,									/* 番兵（使用禁止） */
		};

		/* ブレンド時のピクセル特殊制御フラグの各ビット設定 */
		/* MEMO: この並びはLibrary::KindOperationBlend::Kindの内容と同じである必要があります。 */
		enum FlagBitControlPixel	{
			FLAG_PIXEL_DISCARD = 0x00000001,				/* 各ブレンド種別で透明ピクセルを廃棄するか *//* 0==非廃棄（描画）・1==廃棄 */
			FLAG_PIXEL_PREMULTIPLIED_ALPHA = 0x00000002,	/* 各ブレンド種別でプリマルチプライドアルファ計算を行うかのフラグ *//* 0==ストレート・1==PMA */

			FLAG_PIXEL_CLEAR = 0x00000000,
		};

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: 下記は直接設定しないで関数を使用して設定してください。          */
		/*       ※本来はprivateにするべきですが、プラットフォーム共通項目なので */
		/*         その明示のためだけにpublicにしてあります。                    */
		bool FlagDirty;																			/* 変更された（=描画コマンドを再度作成する必要がある）か？ */
		KindType Type;																			/* ブレンド種別 */

		static const Uint32 /* FlagBitControlPixel */ FlagControlPixel[KindType::TERMINATOR];	/* ブレンド時のピクセル特殊制御フラグ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		inline SectionBlend(void)
		{
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		inline ~SectionBlend(void)
		{
		}

		/* ********************************************************* */
		//! 起動
		/*!
		@param	なし

		@retval	なし
		*/
		inline void BootUp(void)
		{
			Refresh();
		}

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし
		*/
		inline void ShutDown(void)
		{
		}

		/* ********************************************************* */
		//! リフレッシュ
		/*!
		@param	なし

		@retval	なし

		本クラスを起動状態に戻します。
		※レンダリング時に同じインスタンスを使用する際に、描画サイクル
			の変更時や描画オブジェクトの変更時に適時使用してください。
		*/
		inline void Refresh(void)
		{
			FlagDirty = false;
			Type = KindType::MIX;
		}

		/* ********************************************************* */
		//! 設定
		/*!
		@param	type
			ブレンド種別
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		ブレンド種別を切り替えます。
		以前と設定が異なる場合、内部的にフラグを立てます（このフラグ
			はMakeCommand内で判定されます）。
		*/
		inline void Set(KindType type, bool flagForce=false)
		{
			if(false == flagForce)	{	/* 強制変更しない */
				if(Type == type)	{	/* 変更の必要なし */
					return;
				}
			}

			Type = type;
			FlagDirty = true;
		}

		/* ********************************************************* */
		//! 描画コマンドを作成
		/*!
		@param	commandList
			描画コマンドリスト

		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗

		現在設定を反映するために描画コマンドリストに定義します。
		*/
		TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ターゲット制御 */
	class SectionTarget
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		struct BufferColor	{
			Texture* Texture;								/* ターゲット */
			bool FlagClear;									/* ターゲットをクリアするか？ */
			CPU::Color ColorClear;							/* ターゲットをクリアする色 */

			inline void CleanUp(void)
			{
				Texture = nullptr;
				FlagClear = false;
				ColorClear.Set(CPU::Color::Clear);
			}
		};

		struct BufferTest	{
			Texture* Texture;								/* ターゲット */
			bool FlagClear;									/* ターゲットをクリアするか？ */
			int StencilClear;								/* クリア時ステンシル値 *//* 原則下位ビット群が抽出されます */
			Float32	DepthClear;								/* クリア時深度値 */

			inline void CleanUp(void)
			{
				Texture = nullptr;
				FlagClear = false;
				StencilClear = 0;
				DepthClear = 0.0f;
			}
		};

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum	Constant	{
			COUNT_TARGET = 4,							/* MRTで使用可能なターゲット数 *//* ※プラットフォームで異なります */
		};

		/* ----------------------------------------------- Variables */
	private:
		GLint CountTargetColor;							/* 最大のMRT数（動作環境のスペックに依存） */
														/* ※OpenGLにおいては、最大Constant::COUNT_TARGETと本値との低い側が最大MRT数となります。 */

	protected:
	public:
		/* MEMO: 下記は直接設定しないで関数を使用して設定してください。          */
		/*       ※本来はprivateにするべきですが、プラットフォーム共通項目なので */
		/*         その明示のためだけにpublicにしてあります。                    */
		bool FlagDirty;
		BufferColor Color[Constant::COUNT_TARGET];
		BufferTest Test;

		/* ----------------------------------------------- Functions */
	private:
		void CleanUp(void);

	protected:
	public:
		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		SectionTarget(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		~SectionTarget(void)
		{
		}

		/* ********************************************************* */
		//! 起動
		/*!
		@param	なし

		@retval	なし
		*/
		void BootUp(void);

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし
		*/
		void ShutDown(void);

		/* ********************************************************* */
		//! リフレッシュ
		/*!
		@param	なし

		@retval	なし

		本クラスを起動状態に戻します。
		※レンダリング時に同じインスタンスを使用する際に、描画サイクル
			の変更時や描画オブジェクトの変更時に適時使用してください。
		*/
		inline void Refresh(void)
		{
			FlagDirty = false;
		}

		/* ********************************************************* */
		//! 色ターゲットを設定
		/*!
		@param	index
			レンダリングターゲット番号
		@param	texture
			色成分用ターゲットに使用するテクスチャ
			nullptr == 色成分を描画しない
			&Texture::TextureTargetMasterColor == マスタターゲット（色成分）
		@param	flagClear
			true == 指定ターゲットをクリアする
			false == 指定ターゲットの内容は引き継ぐ
		@param	colorClear
			クリア色
			省略時: 完全透明色（ARGB全て0.0f）
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		レンダリングターゲットの色ターゲットを設定します。
		indexは通常は0ですが、MRT（Multiple Render Target）レンダリング
			を行う時は適時値を設定するようにしてください。

		マスタターゲット（フレームバッファ）を指定する場合には、textureに
			Texture::TextureTargetMasterColorを設定してください。
		※マスタターゲットは便宜上テクスチャとして定義されていますが、
			テクスチャとしての要件を満たしていない場合が多いため、テク
			スチャとしての利用は行わないでください。

		colorに設定されるテクスチャはレンダターゲットとして使用可能な
			ように（Texture::BootUpTarget関数で）作成されている必要が
			あります。
		*/
		void ColorSet(int index, Texture* texture, bool flagClear, const CPU::Color& colorClear=CPU::Color::Clear, bool flagForce=false);

		/* ********************************************************* */
		//! テストターゲットを設定
		/*!
		@param	texture
			テストバッファ用ターゲットに使用するテクスチャ
			nullptr == テスト用成分を描画しない
			&TextureTargetMasterTest == マスタターゲット（テスト成分）
		@param	flagClear
			true == 指定ターゲットをクリアする
			false == 指定ターゲットの内容は引き継ぐ
		@param	stencilClear
			クリアステンシル値
			省略時: 0
		@param	depthClear
			クリア深度値
			省略時: 0.0f
		@param	flagForce
			true == 強制的に変更する
			false == 設定値を見て変更を判断

		@retval	なし

		レンダリングターゲットのテストバッファ（深度・ステンシルバッファ）
			を変更します。
		色バッファが複数になり・MRTを行う場合でも、テストバッファは1つ
			である点に気を付けてください。

		テスト用マスタターゲット（フレームバッファと組で設置された深度
			ステンシルバッファ）を指定する場合には、textureに
			Texture::TextureTargetMasterTestを設定してください。
		※マスタターゲットは便宜上テクスチャとして定義されていますが、
			テクスチャとしての要件を満たしていない場合が多いため、テク
			スチャとしての利用は行わないでください。
		*/
		void TestSet(Texture* texture, bool flagClear, int stencilClear=0, float depthClear=0.0f, bool flagForce=false);

		/* ********************************************************* */
		//! ターゲットのクリア
		/*!
		@param	なし

		@retval	なし

		全レンダリングターゲットをクリア（何も割り当てしない）状態に
			します。
		主にレンダラの初期状態時を明確にするために設置してます（に加
			えて、どこでもかしこでもforループでColorSetで初期化するの
			もどうかということで……）。
		*/
		void Purge(void);

		/* ********************************************************* */
		//! 描画コマンドを作成
		/*!
		@param	commandList
			描画コマンドリスト

		@retval	関数値
			描画コマンドリスト（更新値）
			DrawCommandListInvalid == 失敗

		現在設定を反映するために描画コマンドリストに定義します。
		*/
		TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
	/* MEMO: privateについてはプラットフォーム毎で内容が変わっても問題ありません。 */
	/*       ※機種非依存部からはアクセスしません。                                */

protected:
public:
	/* MEMO: publicについてはプラットフォーム問わず同じである必要があります。 */
	/* MEMO: ほぼ実働と関係はないのですが、各セクションの定義順序は大枠GPU内での */
	/*       処理順序にしておいてください。                                      */
	SectionStream Stream;							/* 頂点ストリーム */
	SectionShader Shader;							/* シェーダ */
	SectionRasterizer Rasterizer;					/* ラスタライザ */
	SectionTest Test;								/* テスト（ステンシル・デプス） */
	SectionBlend Blend;								/* ブレンド */
	SectionTarget Target;							/* ターゲット */

	/* レンダリングステートの作成で共通で使用するレンダラ機能のインスタンス */
	/* MEMO: レンダラはシングルトンである必要はありませんが、シングルトンであった方が実行時の効率は良いので */
	/*       一応大域で使用するインスタンスを1つ静的に持っています。                                        */
	static Renderer Instance;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	inline Renderer(void) :
		Stream(),
		Shader(),
		Rasterizer(),
		Test(),
		Blend(),
		Target()
	{
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	inline ~Renderer(void)
	{
	}

	/* ********************************************************* */
	//! 起動
	/*!
	@param	なし

	@retval	なし
	*/
	inline void BootUp(void)
	{
		Rasterizer.BootUp();
		Test.BootUp();
		Blend.BootUp();
		Shader.BootUp();
		Stream.BootUp();
		Target.BootUp();
	}

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし
	*/
	inline void ShutDown(void)
	{
		Stream.ShutDown();
		Shader.ShutDown();
		Blend.ShutDown();
		Test.ShutDown();
		Rasterizer.ShutDown();
		Target.ShutDown();
	}

	/* ********************************************************* */
	//! リフレッシュ
	/*!
	@param	なし

	@retval	なし

	本クラスを起動状態に戻します。
	※レンダリング時に同じインスタンスを使用する際に、描画サイクル
		の変更時や描画オブジェクトの変更時に適時使用してください。
	※持っている全ての設定をリフレッシュ処理を行います。
	*/
	inline void Refresh(void)
	{
		Rasterizer.Refresh();
		Test.Refresh();
		Blend.Refresh();
		Shader.Refresh();
		Stream.Refresh();
		Target.Refresh();
	}

	/* ********************************************************* */
	//! 描画コマンドを作成
	/*!
	@param	commandList
		描画コマンドリスト

	@retval	関数値
		描画コマンドリスト（更新値）
		DrawCommandListInvalid == 失敗

	現在設定を反映するために描画コマンドリストに定義します。
	※持っている全ての設定をコマンド送出処理を行います。
	*/
	TypeDrawCommandList MakeCommand(TypeDrawCommandList commandList);

	/* ********************************************************* */
	//! プリミティブ描画（のコマンドを作成）: 頂点インデックス型
	/*!
	@param	commandList
		描画コマンドリスト
	@param	bufferIndex
		描画する頂点インデックス群
	@param	countVertexIndex
		描画頂点個数
	@param	offsetIndex
		bufferIndexの全頂点インデックスに足す頂点番号オフセット
		省略時: 0

	@retval	関数値
		描画コマンドリスト（更新値）
		DrawCommandListInvalid == 失敗

	※本関数は現在未使用です（同名の別関数を使用）。

	頂点ストリームに定義されている頂点データ（及びここまでで描画
		コマンドリストに定義されている内容）を元に頂点インデックス
		形式でプリミティブ描画を行います。
	※概念的にこの関数が「ここまでのコマンドを実行する」ための処理
		だと解釈してください）。

	描画プリミティブは（現在）Triangle-List形式です。

	本関数で作成される描画コマンドは、各セクションの関数群のように
		（Renderer::MakeCommandの時点まで）キャッシュされず本関数の
		実行時点でコマンドリストに定義されますので注意してください
		（つまり本関数での描画に必要なレンダラ定義は、本関数の前に
		Renderer::MakeCommandで定義されている必要があります）。

	MakeCommandと似た関数ですが、役割が象徴的なため、判り易く別系
		の名前が付いています。
	*/
	TypeDrawCommandList PrimitiveDraw(TypeDrawCommandList commandList, Buffer::GPUBase& bufferIndex, Uint16 countVertexIndex, Uint16 offsetIndex=0);

	/* ********************************************************* */
	//! プリミティブ描画（のコマンドを作成）: 頂点直接型
	/*!
	@param	commandList
		描画コマンドリスト
	@param	countVertexIndex
		描画頂点個数
	@param	offsetIndex
		描画開始する頂点データ番号
		省略時: 0

	@retval	関数値
		描画コマンドリスト（更新値）
		DrawCommandListInvalid == 失敗

	頂点ストリームに定義されている頂点データ（及びここまでで描画
		コマンドリストに定義されている内容）を元にプリミティブ描画
		を行います。
	※概念的にこの関数が「ここまでのコマンドを実行する」ための処理
		だと解釈してください）。

	描画プリミティブは（現在）Triangle-List形式です。

	本関数で作成される描画コマンドは、各セクションの関数群のように
		（Renderer::MakeCommandの時点まで）キャッシュされず本関数の
		実行時点でコマンドリストに定義されますので注意してください
		（つまり本関数での描画に必要なレンダラ定義は、本関数の前に
		Renderer::MakeCommandで定義されている必要があります）。

	MakeCommandと似た関数ですが、役割が象徴的なため、判り易く別系
		の名前が付いています。
	*/
	TypeDrawCommandList PrimitiveDraw(TypeDrawCommandList commandList, int countVertex, int offsetVertex=0);

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
