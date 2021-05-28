/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerでシェーダを扱う場合の基礎機能です。                              */
/* SS6Playerからは本機能を通してプラットフォームのシェーダを扱っています。    */
/* ※各シェーダ毎のカスタムなどについては、本クラスを継承・包含する形で拡張す */
/*   ることになると思います。                                                 */
/*                                                                            */
/* 原則、シェーダは頂点シェーダ・ピクセルシェーダ・定数ブロックなどがひとまと */
/* めになっています（DirectX9のエフェクトファイルの様な形です）。             */
/* また、SpriteStudio用のシェーダについては、1つのシェーダ内にバリアントや複  */
/* 数のシェーダモデルを持たない前提になっています。                           */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_types.h"
#include "ss6player_buffer.h"
#include "ss6player_renderer.h"
#include "ss6player_texture.h"

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* 前方宣言 */
/* MEMO: 下位レイヤから上位レイヤをfriendしているのはできるだけ早く修正すること。                 */
/*       ※ソース整理の過渡的状態なので一時的に混迷しているのですが……さすがにこれは気持ち悪い。 */
class Shader;

namespace Library	{
namespace Control	{
class Root;
}	/* Control */
}	/* Library */
class Entity;
class EntityEffect;

/* 頂点宣言 */
class DeclarationVertex
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
	/* MEMO: privateについてはプラットフォーム毎で内容が変わっても問題ありません。 */
	/*       ※機種非依存部からはアクセスしません。                                */
	void* Work;										/* ワークメモリ */

	/* MEMO: 頂点のlocationが実際にどう反映されたのかについては、GLSLコンパイラがコンパイル時に */
	/*       最適化してしまうため、Shaderの中にlocationインデックスの情報を持っています。       */

protected:
public:
	/* ビルトイン群 */
	/* MEMO: シェーダシステムが起動した後に使用してください。 */
	static DeclarationVertex BuiltInStandard;									/* SS6Playerの標準頂点宣言 */

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
	DeclarationVertex(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	（あまりないと思いますが）本クラスを継承することを考慮して、
		仮想デストラクタになっています。
	*/
	virtual ~DeclarationVertex(void);

	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし
	*/
	void CleanUp(void);

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効（値が設定されていない）

	本クラスが描画用として使用可能かを返します。
	*/
	inline bool IsValid(void)
	{
		/* MEMO: 現在はシェーダデータが割り当たっているかで見ています。 */
		return(nullptr != Work);	/* ? true : false */
	}

	/* ********************************************************* */
	//! 頂点宣言を設定（起動）
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	今はほぼドンガラです（とにかくtrueを返します）。
	※後に拡張がある場合に実装しようかと思います。
	*/
	bool BootUp(void);

	/* ********************************************************* */
	//! 標準の頂点宣言を設定（起動）
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	標準の頂点宣言を作成して起動します（システム起動用関数をfriend
		にしたくないので設置されています）。
	主にシステムの起動用なので、通常に標準頂点形式を取得する場合
		DeclarationVertex::DataStandardSS6Playerを使用するように
		してください。
	*/
	bool BootUpStandard(void);

	/* ********************************************************* */
	//! 頂点宣言を解放（終了）
	/*!
	@param	なし

	@retval	なし

	頂点宣言を解放します。
	※くれぐれも、外部からデフォルトの頂点宣言をShutDownしないで
		ください（何もかもが表示できなくなります）。
	*/
	void ShutDown(void);

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class Shader;
	friend class Renderer::SectionShader;
};

/* シェーダデータ（シェーダ定数）型 */
/* MEMO: いわゆるシェーダ定数やシェーダストレージと呼ばれるものの抽象体です。 */
/*       プラットフォーム毎でシェーダとのやり取りや定数構成（シェーダの実装） */
/*       の都合などが変わるので、直接シェーダ定数群をアクセスせずに、集合的な */
/*       機能クラスとした上でSS6Playerでの使用に前提限定して実装しています。  */
class ShaderData	{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
	/* プラットフォーム用シェーダの内部定数写像構造体群: ビューブロック（プロジェクション・ビュー行列関係） */
	/* MEMO: 現在のシェーダの「include/view_block.glsl」で定義されているブロックのCPU側写像です。 */
	struct ConstantView	{	/* ビュー ブロック */
		/* MEMO: 4x3行列については、各軸の同次成分（.w）を削除して・転置したものですので注意してください（決まった値の部分を削除してあるものです） */
		CPU::Matrix4x4 MatrixProjection;											/* プロジェクション行例 */
		CPU::Matrix4x3 MatrixCamera;												/* カメラ（ビュー）行列 */
		CPU::Matrix4x3 MatrixWorld;													/* ワールド行列 */

		void SetDefault(void)
		{
			MatrixProjection.Set(CPU::Matrix4x4::Unit);
			MatrixCamera.Set(CPU::Matrix4x3::Unit);
			MatrixWorld.Set(CPU::Matrix4x3::Unit);
		}

		static size_t SizeGetBuffer(void)
		{
			/* MEMO: 何かの都合でサイズをtypeofで取れなくなった時のために、本関数からサイズをとってください。 */
			return(sizeof(ConstantView));
		}
	};

protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
	ConstantView View;

	/* 転送用GPUバッファ */
	Buffer::GPUMulti BufferConstantShaderView;

protected:
public:
	bool FlagBootedUp;

/* ----------------------------------------------- Functions */
private:
	void CleanUp(void);

protected:
public:
	ShaderData(void);
	~ShaderData(void);

	bool BootUp(void);
	void ShutDown(void);

	inline bool IsValid(void)
	{
		return(FlagBootedUp);
	}

	void DataSet(void);
	TypeDrawCommandList MakeCommandList(TypeDrawCommandList commandList, const Shader& shader) const;

	inline void MatrixSetProjection(const FPU::Matrix4x4& matrixProjection)
	{	/* MEMO: 都合でFPUが入力なことに気を付けてください。 */
		View.MatrixProjection.Set(matrixProjection);
	}

	inline void MatrixSetView(const FPU::Matrix4x4& matrixView)
	{	/* MEMO: 都合でFPUが入力なことに気を付けてください。 */
		/* MEMO: Platformの中なので（プラッチフォームに依存するので）、直接FPU::Matrix4x4を覗いてしまっています。 */
		const GLfloat* element = matrixView.Value.Element;
		CPU::Matrix4x3 matrix(	element[CPU::Matrix4x4::KindElement::M00], element[CPU::Matrix4x4::KindElement::M10], element[CPU::Matrix4x4::KindElement::M20], element[CPU::Matrix4x4::KindElement::M30],
								element[CPU::Matrix4x4::KindElement::M01], element[CPU::Matrix4x4::KindElement::M11], element[CPU::Matrix4x4::KindElement::M21], element[CPU::Matrix4x4::KindElement::M31],
								element[CPU::Matrix4x4::KindElement::M02], element[CPU::Matrix4x4::KindElement::M12], element[CPU::Matrix4x4::KindElement::M22], element[CPU::Matrix4x4::KindElement::M32]
							);
		View.MatrixCamera.Set(matrix);
	}

	inline void MatrixSetWorld(const FPU::Matrix4x4& matrixWorld)
	{	/* MEMO: 都合でFPUが入力なことに気を付けてください。 */
		const GLfloat* element = matrixWorld.Value.Element;
		CPU::Matrix4x3 matrix(	element[CPU::Matrix4x4::KindElement::M00], element[CPU::Matrix4x4::KindElement::M10], element[CPU::Matrix4x4::KindElement::M20], element[CPU::Matrix4x4::KindElement::M30],
								element[CPU::Matrix4x4::KindElement::M01], element[CPU::Matrix4x4::KindElement::M11], element[CPU::Matrix4x4::KindElement::M21], element[CPU::Matrix4x4::KindElement::M31],
								element[CPU::Matrix4x4::KindElement::M02], element[CPU::Matrix4x4::KindElement::M12], element[CPU::Matrix4x4::KindElement::M22], element[CPU::Matrix4x4::KindElement::M32]
							);
		View.MatrixWorld.Set(matrix);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class Shader;
	friend class Renderer::SectionShader;

	friend class Library::Control::Root;
	friend class SpriteStudio6::Entity;
	friend class SpriteStudio6::EntityEffect;
};

/* シェーダ管理 */
class Shader
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
	/* シェーダ定数領域種別 */
	/* MEMO: これらのインデックスはシェーダプログラムによらずシェーダモデルで統一されているよう。 */
	/*       ただし、それらの定数群がVertex/Geometory/Pixle/Computeのどれに作用するかは、各シェー */
	/*       ダプログラムで異なるので注意してください。                                           */
	/*       最初は実行効率の点から、（定数のマッピング情報まで）全情報溜め込もうかと思ったのです */
	/*       が、実装が煩雑になるため、マッピングはリアルタイムで見ていくことにして・インデックス */
	/*       で抽象化することにしました。                                                         */
	enum KindUniformBlock	{															/* 各Uniformのブロック領域 */
		UNIFORMBLOCK_VIEW,			/* size: sizeof(ViewBlock) */						/* 「view」: ビュー行列・プロジェクション行列 */

		UNIFORMBLOCK_TERMINATOR															/* 番兵（使用禁止） */
	};

protected:
public:
	/* テクスチャサンプラの用途種別 */
	/* MEMO: バインドの関係上今は各バインド毎でのアノテーション名を固定してあります。     */
	/*       その上で管理上はアノテーション名ではなく、抽象化したインデックスで扱います。 */
	enum KindSamplerUsage	{
		SAMPLERUSAGE_ALBEDO0 = 0,															/* 通常ピクセルテクスチャ-0 */
		SAMPLERUSAGE_ALBEDO1,																/* 通常ピクセルテクスチャ-1 */
		SAMPLERUSAGE_ALBEDO2,																/* 通常ピクセルテクスチャ-2 */
		SAMPLERUSAGE_NORMAL0,																/* 法線テクスチャ-0 */
		SAMPLERUSAGE_NORMAL1,																/* 法線テクスチャ-1 */
		SAMPLERUSAGE_NORMAL2,																/* 法線テクスチャ-2 */
		SAMPLERUSAGE_SPECULAR0,																/* スペキュラテクスチャ-0 */
		SAMPLERUSAGE_SPECULAR1,																/* スペキュラテクスチャ-1 */
		SAMPLERUSAGE_SPECULAR2,																/* スペキュラテクスチャ-2 */

		SAMPLERUSAGE_TERMINATOR,															/* 番兵（使用禁止） */
	};

	/* ----------------------------------------------- Variables */
private:
	/* MEMO: privateについてはプラットフォーム毎で内容が変わっても問題ありません。 */
	/*       ※機種非依存部からはアクセスしません。                                */
	void* Work;																/* ワーク *//* ※内部にProgram・AttributeVertexの実体などが入っています。 */
//	void* Data;																/* シェーダそのもののデータ（シェーダファイルのメモリイメージ先頭） */

	GLuint IDProgram;														/* リンク済シェーダプログラムのID */

	GLuint LocationUniformBlock[KindUniformBlock::UNIFORMBLOCK_TERMINATOR];	/* シェーダ定数ブロックViewのロケーション */
	GLuint LocationSampler[KindSamplerUsage::SAMPLERUSAGE_TERMINATOR];		/* テクスチャサンプラのロケーション */
	GLuint LocationAttributeVertex[Renderer::SectionStream::KindDataVertex::DATA_TERMINATOR];	/* 頂点データのロケーション */

protected:
public:
	/* MEMO: publicについてはプラットフォーム問わず同じである必要があります。 */
	int IndexArchive;														/* Data内の何番目のシェーダを管理しているか？ */

	DeclarationVertex* DeclarationVertex;									/* 頂点宣言 */

	/* ビルトイン群 */
	/* MEMO: シェーダシステムが起動した後に使用してください。 */
	static Shader BuiltInStandard;											/* SS6Playerの標準シェーダ */
	static Shader BuiltInStandardMask;										/* SS6Playerの標準シェーダ（マスク描画用） */

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
	Shader(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	（あまりないと思いますが）本クラスを継承することを考慮して、
		仮想デストラクタになっています。
	*/
	virtual ~Shader(void);

	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし
	*/
	void CleanUp(void);

	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効（値が設定されていない）

	本クラスが描画用として使用可能かを返します。
	*/
	inline bool IsValid(void) const
	{
		return(0 < IDProgram);	/* ? true : false */
	}

#if defined(SS6PLAYER_COMPILEOPTION_SHADER_SEPALATE)	/* 分離型シェーダの場合 */
	/* ********************************************************* */
	//! シェーダを設定（起動）
	/*!
	@param	dataVertex
		頂点シェーダを格納したメモリ
	@param	dataPixel
		ピクセル（フラグメント）シェーダを格納したメモリ
	@param	declarationVertex
		適用する頂点宣言
		nullptr == シェーダで設定されている頂点宣言を使用
	@param	indexArchive
		シェーダアーカイブ内のシェーダ番号
		-1 == 内包しているシェーダの個数を取得
	@param	flagInitializeArchive
		シェーダアーカイブを初期化するか？
		true == 初期化する
		false == 初期化しない（初期化済）

	@retval	関数値
		実際に初期化した内包シェーダ番号（通常indexArchiveと同値）
		-1 == 失敗（エラー）

	指定されたメモリに格納されているデータをシェーダとみなして
		本クラスを起動します。
	シェーダファイルの内容は機種依存します。
	dataはSpriteStudio::AlignmentMemory::FILE_BUFFERでアライメント
		されている必要があります（シェーダはVRAM資産なので、モデル
		データなどと同様のアライメントを求められます）。

	declarationVertexはシェーダに適用する頂点宣言（頂点インタリーブ）
		を指定します。
		nullptrを指定するとシェーダに書き込まれた宣言を適用します
		（が、現在対応していない予備仕様ですので、必ず与えてくだ
		さい）。

	indexArchiveとflagInitializeArchiveは、シェーダデータ中に複数
		の実体シェーダを持っている場合（例えばSwitchのシェーダファ
		イルは内部に複数のシェーダモデルを保持することができます）に、
		そのいくつ目のシェーダを管理するか……という際に必要になる
		引数群です（シェーダ内に単一のシェーダしかない場合においては
		省略記載してください）。

	flagInitializeArchiveはシェーダ内に複数のシェーダが入っている時
		に、最初の1回のみtrueにしてください（2回目以降をtrueで実行
		するとハングアップしたりする場合があります）。
		※最初にindexArchive=-1で実行して個数だけを取得した場合は、
			その個数取得時にtrueにして・後はfalseにしてください。

	※dataがconst void*でないのは、テクスチャのCacheStore関数と同じ
		理由で、ファイルデータの一部がそのまま管理のためのワークに
		なっているためです。
	*/
	int BootUp(void* dataVertex, void* dataPixel, SpriteStudio6::DeclarationVertex& declarationVertex, int indexArchive=0, bool flagInitializeArchive=true);
#else	/* 統合型シェーダの場合 */
	/* ********************************************************* */
	//! シェーダを設定（起動）
	/*!
	@param	data
		シェーダを格納したメモリ
	@param	declarationVertex
		適用する頂点宣言
		nullptr == シェーダで設定されている頂点宣言を使用
	@param	indexArchive
		シェーダアーカイブ内のシェーダ番号
		-1 == 内包しているシェーダの個数を取得
	@param	flagInitializeArchive
		シェーダアーカイブを初期化するか？
		true == 初期化する
		false == 初期化しない（初期化済）

	@retval	関数値
		実際に初期化した内包シェーダ番号（通常indexArchiveと同値）
		-1 == 失敗（エラー）

	指定されたメモリに格納されているデータをシェーダとみなして
		本クラスを起動します。
	シェーダファイルの内容は機種依存します。
	dataはSpriteStudio::AlignmentMemory::FILE_BUFFERでアライメント
		されている必要があります（シェーダはVRAM資産なので、モデル
		データなどと同様のアライメントを求められます）。

	declarationVertexはシェーダに適用する頂点宣言（頂点インタリーブ）
		を指定します。
		nullptrを指定するとシェーダに書き込まれた宣言を適用します
		（が、現在対応していない予備仕様ですので、必ず与えてくだ
		さい）。

	indexArchiveとflagInitializeArchiveは、シェーダデータ中に複数
		の実体シェーダを持っている場合（例えばSwitchのシェーダファ
		イルは内部に複数のシェーダモデルを保持することができます）に、
		そのいくつ目のシェーダを管理するか……という際に必要になる
		引数群です（シェーダ内に単一のシェーダしかない場合においては
		省略記載してください）。

	flagInitializeArchiveはシェーダ内に複数のシェーダが入っている時
		に、最初の1回のみtrueにしてください（2回目以降をtrueで実行
		するとハングアップしたりする場合があります）。
		※最初にindexArchive=-1で実行して個数だけを取得した場合は、
			その個数取得時にtrueにして・後はfalseにしてください。

	※dataがconst void*でないのは、テクスチャのCacheStore関数と同じ
		理由で、ファイルデータの一部がそのまま管理のためのワークに
		なっているためです。
	*/
	int BootUp(void* data, SpriteStudio6::DeclarationVertex& declarationVertex, int indexArchive=0, bool flagInitializeArchive=true);
#endif

	/* ********************************************************* */
	//! シェーダを解放（終了）
	/*!
	@param	flagFinalizeArchive
		true == シェーダアーカイブも終了する
		false == シェーダアーカイブは終了しない

	@retval	なし

	本クラスと実際のシェーダとの連動を切ります（本クラスを未使用
		状態にします）。
	*/
	void ShutDown(bool flagFinalizeArchive=true);

	/* ********************************************************* */
	//! シェーダのサンプラにテクスチャをバインド
	/*!
	@param	commandList
		描画コマンドリスト
	@param	kindSampler
		設定するサンプラの種別
	@param	texture
		設定するテクスチャ
	@param	interpolation
		設定するテクスチャの補間モード
	@param	wrap
		設定するテクスチャのラップアラウンドモード

	@retval	関数値
		更新されたコマンドリスト
		DrawCommandListInvalid == 失敗

	指定したシェーダの持つサンプラにテクスチャをバインドして、描画
		リストに定義します。
	*/
	TypeDrawCommandList MakeCommandTexture(	TypeDrawCommandList commandList,
											KindSamplerUsage kindSampler,
											const Texture& texture,
											Texture::KindInterpolation interpolation,
											Texture::KindWrap wrap
										) const;

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
	friend class ShaderData;
	friend class Renderer::SectionShader;
	friend class Renderer::SectionStream;
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
