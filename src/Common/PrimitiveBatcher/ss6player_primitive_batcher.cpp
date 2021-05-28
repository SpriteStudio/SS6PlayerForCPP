/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_primitive_batcher.h"

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
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
/*                                  PrimitiveBatcher::SettingRender::CallBack */
/* -------------------------------------------------------------------------- */
/* MEMO: テンプレートクラスなので、全て.hで宣言しています。 */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                            PrimitiveBatcher::SettingRender */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::CleanUp(void)

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// PrimitiveBatcher::SettingRender::SettingRender(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// PrimitiveBatcher::SettingRender::~SettingRender(void)

/* ********************************************************* */
//! 値の設定（複製）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::Set(const SettingRender& original)

/* ********************************************************* */
//! シェーダを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::ShaderSet(const SpriteStudio6::Shader* shader)

/* ********************************************************* */
//! ブレンド種別を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::BlendSet(Renderer::SectionBlend::KindType blend)

/* ********************************************************* */
//! セルマップテクスチャを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::TextureSetCellMap(Texture* texture, Texture::KindInterpolation interpolation, Texture::KindWrap wrap)

/* ********************************************************* */
//! セルマップテクスチャの補間方法取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Texture::KindInterpolation PrimitiveBatcher::SettingRender::InterpolationGetCellMap(void) const

/* ********************************************************* */
//! セルマップテクスチャのラップアラウンド方法取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Texture::KindWrap PrimitiveBatcher::SettingRender::WrapGetCellMap(void) const

/* ********************************************************* */
//! カラーターゲットテクスチャ数を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::CountSetTextureTargetColor(int count)

/* ********************************************************* */
//! カラーターゲットテクスチャ数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int PrimitiveBatcher::SettingRender::CountGetTextureTargetColor(void) const

/* ********************************************************* */
//! ピクセル色用ターゲットテクスチャを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::TextureSetTargetColor(int index, Texture* texture, bool flagClear, const CPU::Color& colorClear)

/* ********************************************************* */
//! テスト用ターゲットテクスチャを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::TextureSetTargetTest(Texture* texture,  bool flagClear, int valueStencil, float valueDepth)

/* ********************************************************* */
//! ステンシルテストの設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::TestSetStencil(bool flagTest, Renderer::SectionTest::KindTypeDraw kindDraw)

/* ********************************************************* */
//! 深度テストの設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::SettingRender::TestSetDepth(bool flagTest, bool flagWrite)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                 PrimitiveBatcher::Fragment */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// PrimitiveBatcher::Fragment::Fragment(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// PrimitiveBatcher::Fragment::~Fragment(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::Fragment::CleanUp(void)

/* ********************************************************* */
//! 指定描画方法が追加可能かの判定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool PrimitiveBatcher::Fragment::IsAddable(const SettingRender& setting) const

/* ********************************************************* */
//! 指定描画方法が追加可能かの判定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::Fragment::Set(const SettingRender& setting)

/* ********************************************************* */
//! 描画（GPUへの描画コマンド設定）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList PrimitiveBatcher::Fragment::Draw(TypeDrawCommandList commandList, PrimitiveBatcher& primitiveBatcher, const ShaderData& dataShader)
{
	/* MEMO: 原則、描画コマンド生成はレンダラを通して行います。                      */
	/*       ※一部クラスは自身に「MakeCommand～」という描画コマンドを作成する関数を */
	/*         所有しているものがありますが……原則、それらはレンダラから呼ぶための  */
	/*         関数です（その内整理して削除します）。                                */
	/* MEMO: 一旦レンダラへは「強制更新」で設定を定義しています。                   */
	/*       基本的にメッシュは最低限描画設定（マテリアル）毎でバッチされているので */
	/*       描画設定そのものをザラで出しても、さしてダメージにはならないはずです。 */
	/*       ※あと、GPUによっては特定の描画コマンドを出した後は、既設定されている  */
	/*         設定がクリアされるものがあるので、その回避のためもあります。         */

	TypeDrawCommandList commandListNew = nullptr;

	/* 描画コマンドに割り込むためのコールバック */
	if(true == CallBackInsert.IsValid())	{
		commandListNew = CallBackInsert.Function(commandList, CallBackInsert.ParameterPointer, CallBackInsert.ParameterInteger);
		if(DrawCommandListInvalid == commandListNew)	{
			return(DrawCommandListInvalid);
		}
		commandList = commandListNew;
	}

	if((nullptr == Shader) || (false == Shader->IsValid()))	{	/* シェーダが異常 */
		/* MEMO: 何も描画しないで帰る。 */
		return(commandList);
	}

	Renderer& renderer = Renderer::Instance;

	/* レンダリングターゲットを設定 */
	renderer.Target.Purge();
	{
		Texture* texture;

		/* カラーターゲットの設定 */
		int countTexture = CountGetTextureTargetColor();
		if(Renderer::SectionTarget::Constant::COUNT_TARGET < countTexture)	{
			countTexture = Renderer::SectionTarget::Constant::COUNT_TARGET;
		}

		Uint32 bitStatusTextureClear = 0x00000001;
		for(int i=0; i<countTexture; i++)	{
			texture = TextureTargetColor[i];
			if(nullptr == texture)	{	/* フレームバッファ指定 */
				texture = &Texture::TargetMasterColor;
			}

			renderer.Target.ColorSet(	i,
										texture,
										(0 != (Status & bitStatusTextureClear)),	/* ? true : false */
										ColorClearTargetColor[i],
										true
									);
		}

		/* テストターゲットの設定 */
		/* MEMO: テストターゲットはMRTであっても1つだけ。 */
		texture = TextureTargetTest;
		if(nullptr == texture)	{	/* フレームバッファ指定 */
			texture = &Texture::TargetMasterTest;
		}
		renderer.Target.TestSet(	texture,
									(0 != (Status & FlagStatusBit::CLEAR_TARGET_TEST)),	/* ? true : false */
									ValueClearStencil,
									ValueClearDepth,
									true
								);
	}

	/* ステンシルテスト方法を設定 */
	renderer.Test.StencilSet(	(Renderer::SectionTest::KindTypeDraw)((Status & FlagStatusBit::ACTION_STENCIL) >> ShiftStatusBit::SHIFT_ACTION_STENCIL),
								(0 != (Status & FlagStatusBit::TEST_STENCIL)),	/* ? true : false */
								true
						);

	/* 深度テスト方法を設定 */
	renderer.Test.DepthSet(	(0 != (Status & FlagStatusBit::TEST_DEPTH)),	/* ? true : false */
							(0 != (Status & FlagStatusBit::WRITE_DEPTH)),	/* ? true : false */
								true
						);

	/* シェーダを設定 */
	/* MEMO: シェーダは強制設定しています。 */
	renderer.Shader.Set(*Shader, true);			/* シェーダ */
	renderer.Shader.DataSet(dataShader, true);	/* シェーダ定数 */

	/* テクスチャを設定（描画用テクスチャを使用するシェーダに設定） */
	if(nullptr != TextureCellMap)	{
		commandListNew = Shader->MakeCommandTexture(	commandList,
														Shader::KindSamplerUsage::SAMPLERUSAGE_ALBEDO0,
														*TextureCellMap,
														InterpolationGetCellMap(),
														WrapGetCellMap()
												);	/* アルベドテクスチャ-0をバインド */
		if(DrawCommandListInvalid == commandListNew)	{
			return(DrawCommandListInvalid);
		}
		commandList = commandListNew;
	}

	/* ブレンド方法を設定 */
	renderer.Blend.Set((Renderer::SectionBlend::KindType)TypeBlend, true);

	/* レンダラに定義した値群で描画コマンドを作成 */
	commandListNew = renderer.MakeCommand(commandList);
	if(DrawCommandListInvalid == commandListNew)	{
		return(DrawCommandListInvalid);
	}
	commandList = commandListNew;

	/* プリミティブを描画（頂点ストリームをキック） */
	/* MEMO: 頂点データのストリームは全フラグメントで共通になるので、呼出元のPrimitiveBatcher::Drawで設定しています。 */
	commandListNew = renderer.PrimitiveDraw(commandList, CountVertex, VertexTop);
	if(DrawCommandListInvalid == commandListNew)	{
		return(DrawCommandListInvalid);
	}
	commandList = commandListNew;

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           PrimitiveBatcher */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 *
 */
// PrimitiveBatcher::PrimitiveBatcher(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// PrimitiveBatcher::~PrimitiveBatcher(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::CleanUp(void)

/* ********************************************************* */
//! プリミティブ使用状態を未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void PrimitiveBatcher::CleanUpPrimitive(void)

/* ********************************************************* */
//! 本クラスが有効か
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool PrimitiveBatcher::IsValid(void)

/* ********************************************************* */
//! 本クラスにプリミティブを追加可能か
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool PrimitiveBatcher::IsAddable(void)

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool PrimitiveBatcher::BootUp(int countFragmentMax, int countVertexMax)
{
	/* MEMO: 一度終了しておく */
	ShutDown();

	if((0 > countFragmentMax) || (0 > countVertexMax))	{	/* そもそもあり得ない値 */
		return(false);
	}

	/* エンティティ毎描画情報の起動 */
	DataEntity.BootUp();

	/* 最大頂点数・最大フラグメント数の設定 */
	CountFragmentMax = countFragmentMax;
	CountVertexMax = countVertexMax;

	/* GPUバッファの起動 */
	/* MEMO: 頂点座標（Coordinate）のみFloat32[3]なので注意してください（その他はFloat32[4]）。 */
	if(0 < countVertexMax)	{
		DataCoordinate.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 3) * countVertexMax);
		DataPartsColor.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataUV0.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataFlags.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataArgumentVs00.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataArgumentFs00.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataArgumentFs01.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataArgumentFs02.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataArgumentFs03.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataParameterFs00.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataParameterFs01.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataParameterFs02.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
		DataParameterFs03.BootUp(Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX, (sizeof(Float32) * 4) * countVertexMax);
	}

	/* ワークエリアの取得 */
	if(0 < countFragmentMax)	{
		size_t sizeTableFragment = Memory::Alignment(sizeof(Fragment) * countFragmentMax, AlignmentMemory::SAFETY);
		TableFragment = reinterpret_cast<Fragment*>(Memory::Allocate(&WorkArea, sizeTableFragment, AlignmentMemory::SAFETY));
		if(nullptr == WorkArea)	{	/* 取得できなかった */
			ShutDown();

			return(false);
		}
		for(int i=0; i<countFragmentMax; i++)	{
			/* MEMO: 明示的に領域初期化しておく。 */
			new(&TableFragment[i]) Fragment;
		}
	}

	return(true);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void PrimitiveBatcher::ShutDown(void)
{
	/* GPUバッファの終了 */
	DataCoordinate.ShutDown();
	DataPartsColor.ShutDown();
	DataUV0.ShutDown();
	DataFlags.ShutDown();
	DataArgumentVs00.ShutDown();
	DataArgumentFs00.ShutDown();
	DataArgumentFs01.ShutDown();
	DataArgumentFs02.ShutDown();
	DataArgumentFs03.ShutDown();
	DataParameterFs00.ShutDown();
	DataParameterFs01.ShutDown();
	DataParameterFs02.ShutDown();
	DataParameterFs03.ShutDown();

	/* ワークエリアの解放 */
	if(nullptr != WorkArea)	{
		Memory::Free(WorkArea);
	}

	/* エンティティ毎描画情報の終了 */
	DataEntity.ShutDown();

	CleanUp();
}

/* ********************************************************* */
//! 描画
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList PrimitiveBatcher::Draw(TypeDrawCommandList commandList, const ShaderData& dataShader)
{
	if(true == IsAddablePrimitive())	{	/* 頂点更新区間内 */
		return(DrawCommandListInvalid);
	}

	if(nullptr == TableFragment)	{	/* 扱っているフラグメントがない */
		return(commandList);
	}

	/* 頂点データの設定 */
	/* MEMO: 各フラグメントはまとまって定義されている頂点データの一部区間を描画するだけなので、ストリームは共通で設定しています。 */
	Renderer& renderer = Renderer::Instance;
	renderer.Refresh();	/* 内部情報をリフレッシュ */

	renderer.Stream.EntitySet(DataEntity);	/* 描画エンティティ情報 */

	renderer.Stream.Set(DataCoordinate, Renderer::SectionStream::KindDataVertex::DATA_COORDINATE, (sizeof(Float32) * 3));	/* 頂点座標 */
	renderer.Stream.Set(DataUV0, Renderer::SectionStream::KindDataVertex::DATA_UV, (sizeof(Float32) * 4));	/* 頂点UV・パーツカラー補佐 */
	renderer.Stream.Set(DataPartsColor, Renderer::SectionStream::KindDataVertex::DATA_PARTSCOLOR, (sizeof(Float32) * 4));	/* パーツカラー */
	renderer.Stream.Set(DataArgumentVs00, Renderer::SectionStream::KindDataVertex::DATA_ARGUMENT_VS00, (sizeof(Float32) * 4));	/* 頂点シェーダ用パラメータ */

	renderer.Stream.Set(DataFlags, Renderer::SectionStream::KindDataVertex::DATA_FLAGS, (sizeof(Float32) * 4));	/* 描画補佐フラグ */

	renderer.Stream.Set(DataArgumentFs00, Renderer::SectionStream::KindDataVertex::DATA_ARGUMENT_FS00, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータA-0 */
	renderer.Stream.Set(DataArgumentFs01, Renderer::SectionStream::KindDataVertex::DATA_ARGUMENT_FS01, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータA-1 */
	renderer.Stream.Set(DataArgumentFs02, Renderer::SectionStream::KindDataVertex::DATA_ARGUMENT_FS02, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータA-2 */
	renderer.Stream.Set(DataArgumentFs03, Renderer::SectionStream::KindDataVertex::DATA_ARGUMENT_FS03, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータA-3 */

	renderer.Stream.Set(DataParameterFs00, Renderer::SectionStream::KindDataVertex::DATA_PARAMETER_FS00, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータB-0 */
	renderer.Stream.Set(DataParameterFs01, Renderer::SectionStream::KindDataVertex::DATA_PARAMETER_FS01, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータB-1 */
	renderer.Stream.Set(DataParameterFs02, Renderer::SectionStream::KindDataVertex::DATA_PARAMETER_FS02, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータB-2 */
	renderer.Stream.Set(DataParameterFs03, Renderer::SectionStream::KindDataVertex::DATA_PARAMETER_FS03, (sizeof(Float32) * 4));	/* ピクセルシェーダ用パラメータB-3 */

	/* 各フラグメントの描画 */
	/* MEMO: CountFragmentは「現在使用している末尾のフラグメントのインデックス」なので、forループで */
	/*       使用する場合「以下」で見なくてはならないことに注意してください。                       */
	for(int i=0; i<=CountFragment; i++)	{
		commandList = TableFragment[i].Draw(commandList, *this, dataShader);
		if(DrawCommandListInvalid == commandList)	{	/* 失敗 */
			return(DrawCommandListInvalid);
		}
	}

	return(commandList);
}

/* ********************************************************* */
//! プリミティブの構築開始
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool PrimitiveBatcher::PrimitiveBegin(void)
{
	/* プリミティブ情報のクリア */
	CleanUpPrimitive();

	if(nullptr == TableFragment)	{	/* 扱っているフラグメントがない */
		/* MEMO: 現在は（頂点追加できないだけで）成功にしています。 */
		return(true);
	}

	/* GPUバッファのマップ */
	BufferCoordinate = reinterpret_cast<CPU::Vector3*>(DataCoordinate.Map());
	BufferPartsColor = reinterpret_cast<CPU::Vector4*>(DataPartsColor.Map());
	BufferUV0 = reinterpret_cast<CPU::Vector4*>(DataUV0.Map());
	BufferFlags = reinterpret_cast<CPU::Vector4*>(DataFlags.Map());
	BufferArgumentVs00 = reinterpret_cast<CPU::Vector4*>(DataArgumentVs00.Map());
	BufferArgumentFs00 = reinterpret_cast<CPU::Vector4*>(DataArgumentFs00.Map());
	BufferArgumentFs01 = reinterpret_cast<CPU::Vector4*>(DataArgumentFs01.Map());
	BufferArgumentFs02 = reinterpret_cast<CPU::Vector4*>(DataArgumentFs02.Map());
	BufferArgumentFs03 = reinterpret_cast<CPU::Vector4*>(DataArgumentFs03.Map());
	BufferParameterFs00 = reinterpret_cast<CPU::Vector4*>(DataParameterFs00.Map());
	BufferParameterFs01 = reinterpret_cast<CPU::Vector4*>(DataParameterFs01.Map());
	BufferParameterFs02 = reinterpret_cast<CPU::Vector4*>(DataParameterFs02.Map());
	BufferParameterFs03 = reinterpret_cast<CPU::Vector4*>(DataParameterFs03.Map());

	return(true);
}

/* ********************************************************* */
//! プリミティブの構築終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void PrimitiveBatcher::PrimitiveEnd(void)
{
	if(nullptr == TableFragment)	{	/* 扱っているフラグメントがない */
		/* MEMO: 現在は（頂点追加できないだけで）成功にしています。 */
		return;
	}

	/* GPUバッファのアンマップ */
	DataCoordinate.Unmap();
	DataPartsColor.Unmap();
	DataUV0.Unmap();
	DataFlags.Unmap();
	DataArgumentVs00.Unmap();
	DataArgumentFs00.Unmap();
	DataArgumentFs01.Unmap();
	DataArgumentFs02.Unmap();
	DataArgumentFs03.Unmap();
	DataParameterFs00.Unmap();
	DataParameterFs01.Unmap();
	DataParameterFs02.Unmap();
	DataParameterFs03.Unmap();

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
}

/* ********************************************************* */
//! プリミティブの確保
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int PrimitiveBatcher::PrimitiveAllocate(	CPU::Vector3** dataCoordinate,
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
										)
{
	if(nullptr == TableFragment)	{	/* 扱っているフラグメントがない */
		return(-1);
	}

	if(false == IsAddablePrimitive())	{	/* 追加禁止区間 */
		return(-1);
	}

	const int countVertexNew = countPrimitive * Constant::VERTEX_COUNT_TRIANGLE;
	if(0 >= countVertexNew)	{	/* 定義頂点がない */
		return(0);	/* countVertexNew */
	}
	if((CountVertex + countVertexNew) > CountVertexMax)	{	/* 定義可能頂点数オーバー */
		return(-1);
	}

	/* 追加可能なフラグメントを特定 */
	Fragment* fragment = nullptr;
	bool flagFragmentNew = false;
	if(0 > CountFragment)	{	/* 何も定義されていない */
		flagFragmentNew = true;
		CountFragment = 0;	/* ++ */
	} else {
		fragment = &TableFragment[CountFragment];
		if(	(true == flagBreakBatching)						/* バッチングを意図的に阻害 */
			|| (false == fragment->IsAddable(setting))		/* 追加できない（設定が異なる） */
		)	{	/* 新しいフラグメント */
			if((CountFragmentMax - 1) <= CountFragment)	{	/* 最終フラグメントを使っている */
				return(0);	/* countVertexNew */
			}

			flagFragmentNew = true;
			CountFragment++;
		}
	}
	if(true == flagFragmentNew)	{	/* 新規フラグメント */
		/* 新規フラグメントの初期設定 */
		/* MEMO: Fragment::Set関数はSettingRender::Set関数なので、レンダリング設定しか複製設定されないことに注意。 */
		fragment = &TableFragment[CountFragment];
		fragment->CleanUp();
		fragment->Set(setting);
		fragment->VertexTop = CountVertex;
	}

	/* フラグメントに情報を追加（フラグメントの描画頂点数を加算） */
	fragment->CountVertex += countVertexNew;

	/* 各ポインタを設定 */
	int indexVertex = CountVertex;
	*dataCoordinate = reinterpret_cast<CPU::Vector3*>(&BufferCoordinate[indexVertex]);
	*dataColorParts = reinterpret_cast<CPU::Color*>(&BufferPartsColor[indexVertex]);
	*dataUVTexture = reinterpret_cast<CPU::Vector4*>(&BufferUV0[indexVertex]);
	*dataFlags = reinterpret_cast<CPU::Vector4*>(&BufferFlags[indexVertex]);
	*dataArgumentVs00 = reinterpret_cast<CPU::Vector4*>(&BufferArgumentVs00[indexVertex]);
	*dataArgumentFs00 = reinterpret_cast<CPU::Vector4*>(&BufferArgumentFs00[indexVertex]);
	*dataArgumentFs01 = reinterpret_cast<CPU::Vector4*>(&BufferArgumentFs01[indexVertex]);
	*dataArgumentFs02 = reinterpret_cast<CPU::Vector4*>(&BufferArgumentFs02[indexVertex]);
	*dataArgumentFs03 = reinterpret_cast<CPU::Vector4*>(&BufferArgumentFs03[indexVertex]);
	*dataParameterFs00 = reinterpret_cast<CPU::Vector4*>(&BufferParameterFs00[indexVertex]);
	*dataParameterFs01 = reinterpret_cast<CPU::Vector4*>(&BufferParameterFs01[indexVertex]);
	*dataParameterFs02 = reinterpret_cast<CPU::Vector4*>(&BufferParameterFs02[indexVertex]);
	*dataParameterFs03 = reinterpret_cast<CPU::Vector4*>(&BufferParameterFs03[indexVertex]);

	/* 頂点数を進める（PrimitiveBatcherの管理頂点数を加算） */
	CountVertex += countVertexNew;

	return(countVertexNew);
}

/* ********************************************************* */
//! 4分割四角プリミティブの追加
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int PrimitiveBatcher::PrimitiveAddQuadrilateral4(	const SettingRender& setting,
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
												)
{
	/* 頂点データのバッファ先頭を取得 */
	CPU::Vector3* bufferCoordinate;
	CPU::Color* bufferColorParts;
	CPU::Vector4* bufferUVTexture;
	CPU::Vector4* bufferFlags;
	CPU::Vector4* bufferArgumentVs00;
	CPU::Vector4* bufferArgumentFs00;
	CPU::Vector4* bufferArgumentFs01;
	CPU::Vector4* bufferArgumentFs02;
	CPU::Vector4* bufferArgumentFs03;
	CPU::Vector4* bufferParameterFs00;
	CPU::Vector4* bufferParameterFs01;
	CPU::Vector4* bufferParameterFs02;
	CPU::Vector4* bufferParameterFs03;
	static const int countVertex = CountGetVertex(Library::KindVertex::TERMINATOR4);
	int countVertexAllocated = 0;
	countVertexAllocated = PrimitiveAllocate(	&bufferCoordinate,
												&bufferColorParts,
												&bufferUVTexture,
												&bufferFlags,
												&bufferArgumentVs00,
												&bufferArgumentFs00,
												&bufferArgumentFs01,
												&bufferArgumentFs02,
												&bufferArgumentFs03,
												&bufferParameterFs00,
												&bufferParameterFs01,
												&bufferParameterFs02,
												&bufferParameterFs03,
												4,	/* Library::KindVertex::TERMINATOR4のプリミティブ数 */
												setting,
												flagBreakBatching
											);
	if(countVertex != countVertexAllocated)	{	/* 要求頂点数と異なる */
		return(countVertexAllocated);
	}

	/* 頂点データを格納 */
	/* MEMO: 実際の頂点データ格納状態は下記になり、処理を最適化してあります。 */
	/*       三角形0: LU( 0) - C( 1) - RU( 2)                                 */
	/*       三角形1: RU( 3) - C( 4) - RD( 5)                                 */
	/*       三角形2: RD( 6) - C( 7) - LD( 8)                                 */
	/*       三角形3: LD( 9) - C(10) - LU(11)                                 */
	CPU::Vector3 dataVector3;
	CPU::Vector4 dataVector4;
	CPU::Color dataColor;

	{	/* MEMO: 左上 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::LU]);
			bufferCoordinate[0].Set(dataVector3);
			bufferCoordinate[11].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::LU]);
			bufferColorParts[0].Set(dataColor);
			bufferColorParts[11].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::LU]);
			bufferUVTexture[0].Set(dataVector4);
			bufferUVTexture[11].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::LU]);
			bufferArgumentVs00[0].Set(dataVector4);
			bufferArgumentVs00[11].Set(dataVector4);
	}
	{	/* MEMO: 右上 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::RU]);
			bufferCoordinate[2].Set(dataVector3);
			bufferCoordinate[3].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::RU]);
			bufferColorParts[2].Set(dataColor);
			bufferColorParts[3].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::RU]);
			bufferUVTexture[2].Set(dataVector4);
			bufferUVTexture[3].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::RU]);
			bufferArgumentVs00[2].Set(dataVector4);
			bufferArgumentVs00[3].Set(dataVector4);
	}
	{	/* MEMO: 右下 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::RD]);
			bufferCoordinate[5].Set(dataVector3);
			bufferCoordinate[6].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::RD]);
			bufferColorParts[5].Set(dataColor);
			bufferColorParts[6].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::RD]);
			bufferUVTexture[5].Set(dataVector4);
			bufferUVTexture[6].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::RD]);
			bufferArgumentVs00[5].Set(dataVector4);
			bufferArgumentVs00[6].Set(dataVector4);
	}
	{	/* MEMO: 左下 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::LD]);
			bufferCoordinate[8].Set(dataVector3);
			bufferCoordinate[9].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::LD]);
			bufferColorParts[8].Set(dataColor);
			bufferColorParts[9].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::LD]);
			bufferUVTexture[8].Set(dataVector4);
			bufferUVTexture[9].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::LD]);
			bufferArgumentVs00[8].Set(dataVector4);
			bufferArgumentVs00[9].Set(dataVector4);
	}
	{	/* MEMO: 中央 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::C]);
			bufferCoordinate[1].Set(dataVector3);
			bufferCoordinate[4].Set(dataVector3);
			bufferCoordinate[7].Set(dataVector3);
			bufferCoordinate[10].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::C]);
			bufferColorParts[1].Set(dataColor);
			bufferColorParts[4].Set(dataColor);
			bufferColorParts[7].Set(dataColor);
			bufferColorParts[10].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::C]);
			bufferUVTexture[1].Set(dataVector4);
			bufferUVTexture[4].Set(dataVector4);
			bufferUVTexture[7].Set(dataVector4);
			bufferUVTexture[10].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::C]);
			bufferArgumentVs00[1].Set(dataVector4);
			bufferArgumentVs00[4].Set(dataVector4);
			bufferArgumentVs00[7].Set(dataVector4);
			bufferArgumentVs00[10].Set(dataVector4);
	}
	/* MEMO: 全頂点共通データ */
	dataVector4.Set(dataFlags);
	const CPU::Vector4 dataVector4a(dataArgumentFs00);
	const CPU::Vector4 dataVector4b(dataArgumentFs01);
	const CPU::Vector4 dataVector4c(dataArgumentFs02);
	const CPU::Vector4 dataVector4d(dataArgumentFs03);
	const CPU::Vector4 dataVector4e(dataParameterFs00);
	const CPU::Vector4 dataVector4f(dataParameterFs01);
	const CPU::Vector4 dataVector4g(dataParameterFs02);
	const CPU::Vector4 dataVector4h(dataParameterFs03);
	for(int i=0; i<countVertex; i++)	{
		bufferFlags[i].Set(dataVector4);
		bufferArgumentFs00[i].Set(dataVector4a);
		bufferArgumentFs01[i].Set(dataVector4b);
		bufferArgumentFs02[i].Set(dataVector4c);
		bufferArgumentFs03[i].Set(dataVector4d);
		bufferParameterFs00[i].Set(dataVector4e);
		bufferParameterFs01[i].Set(dataVector4f);
		bufferParameterFs02[i].Set(dataVector4g);
		bufferParameterFs03[i].Set(dataVector4h);
	}

	return(countVertex);
}

/* ********************************************************* */
//! 2分割四角プリミティブの追加
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int PrimitiveBatcher::PrimitiveAddQuadrilateral2(	const SettingRender& setting,
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
												)
{
#if 0	/* MEMO: 誤呼出に誤動作しないよう、一旦伏せておきます。 */
	/* 頂点データのバッファ先頭を取得 */
	CPU::Vector3* bufferCoordinate;
	CPU::Color* bufferColorParts;
	CPU::Vector4* bufferUVTexture;
	CPU::Vector4* bufferFlags;
	CPU::Vector4* bufferArgumentVs00;
	CPU::Vector4* bufferArgumentFs00;
	CPU::Vector4* bufferArgumentFs01;
	CPU::Vector4* bufferArgumentFs02;
	CPU::Vector4* bufferArgumentFs03;
	CPU::Vector4* bufferParameterFs00;
	CPU::Vector4* bufferParameterFs01;
	CPU::Vector4* bufferParameterFs02;
	CPU::Vector4* bufferParameterFs03;
	static const int countVertex = CountGetVertex(Library::KindVertex::TERMINATOR2);
	int countVertexAllocated = 0;
	countVertexAllocated = PrimitiveAllocate(	&bufferCoordinate,
												&bufferColorParts,
												&bufferUVTexture,
												&bufferFlags,
												&bufferArgumentVs00,
												&bufferArgumentFs00,
												&bufferArgumentFs01,
												&bufferArgumentFs02,
												&bufferArgumentFs03,
												&bufferParameterFs00,
												&bufferParameterFs01,
												&bufferParameterFs02,
												&bufferParameterFs03,
												2,	/* Library::KindVertex::TERMINATOR2のプリミティブ数 */
												setting,
												flagBreakBatching
											);
	if(countVertex != countVertexAllocated)	{	/* 要求頂点数と異なる */
		return(countVertexAllocated);
	}

	/* 頂点データを格納 */
	/* MEMO: 実際の頂点データ格納状態は下記になり、処理を最適化してあります。 */
	/*       三角形0: LU( 0) - RD( 1) - RU( 2)                                */
	/*       三角形1: LU( 3) - LD( 4) - RD( 5)                                */
	CPU::Vector3 dataVector3;
	CPU::Vector4 dataVector4;
	CPU::Color dataColor;

	{	/* MEMO: 左上 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::LU]);
			bufferCoordinate[0].Set(dataVector3);
			bufferCoordinate[3].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::LU]);
			bufferColorParts[0].Set(dataColor);
			bufferColorParts[3].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::LU]);
			bufferUVTexture[0].Set(dataVector4);
			bufferUVTexture[3].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::LU]);
			bufferArgumentVs00[0].Set(dataVector4);
			bufferArgumentVs00[3].Set(dataVector4);
	}
	{	/* MEMO: 右上 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::RU]);
			bufferCoordinate[2].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::RU]);
			bufferColorParts[2].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::RU]);
			bufferUVTexture[2].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::RU]);
			bufferArgumentVs00[2].Set(dataVector4);
	}
	{	/* MEMO: 右下 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::RD]);
			bufferCoordinate[1].Set(dataVector3);
			bufferCoordinate[5].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::RD]);
			bufferColorParts[1].Set(dataColor);
			bufferColorParts[5].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::RD]);
			bufferUVTexture[1].Set(dataVector4);
			bufferUVTexture[5].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::RD]);
			bufferArgumentVs00[1].Set(dataVector4);
			bufferArgumentVs00[5].Set(dataVector4);
	}
	{	/* MEMO: 左下 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::LD]);
			bufferCoordinate[4].Set(dataVector3);

		dataColor.Set(dataColorParts[Library::KindVertex::LD]);
			bufferColorParts[4].Set(dataColor);

		dataVector4.Set(dataUVTexture[Library::KindVertex::LD]);
			bufferUVTexture[4].Set(dataVector4);

		dataVector4.Set(dataArgumentVs00[Library::KindVertex::LD]);
			bufferArgumentVs00[4].Set(dataVector4);
	}
	/* MEMO: 全頂点共通データ */
	dataVector4.Set(dataFlags);
	const CPU::Vector4 dataVector4a(dataArgumentFs00);
	const CPU::Vector4 dataVector4b(dataArgumentFs01);
	const CPU::Vector4 dataVector4c(dataArgumentFs02);
	const CPU::Vector4 dataVector4d(dataArgumentFs03);
	const CPU::Vector4 dataVector4e(dataParameterFs00);
	const CPU::Vector4 dataVector4f(dataParameterFs01);
	const CPU::Vector4 dataVector4g(dataParameterFs02);
	const CPU::Vector4 dataVector4h(dataParameterFs03);
	for(int i=0; i<countVertex; i++)	{
		bufferFlags[i].Set(dataVector4);
		bufferArgumentFs00[i].Set(dataVector4a);
		bufferArgumentFs01[i].Set(dataVector4b);
		bufferArgumentFs02[i].Set(dataVector4c);
		bufferArgumentFs03[i].Set(dataVector4d);
		bufferParameterFs00[i].Set(dataVector4e);
		bufferParameterFs01[i].Set(dataVector4f);
		bufferParameterFs02[i].Set(dataVector4g);
		bufferParameterFs03[i].Set(dataVector4h);
	}

	return(countVertex);
#else	/* MEMO: 現在は描画数は0としておきます。 */
	return(0);
#endif
}

/* ********************************************************* */
//! 汎用メッシュプリミティブの追加
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int PrimitiveBatcher::PrimitiveAddMesh(	const SettingRender& setting,
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
								)
{
	if(0 >= countPrimitive)	{	/* プリミティブ数が異常 */
		return(-1);
	}

	/* 頂点データのバッファ先頭を取得 */
	CPU::Vector3* bufferCoordinate;
	CPU::Color* bufferColorParts;
	CPU::Vector4* bufferUVTexture;
	CPU::Vector4* bufferFlags;
	CPU::Vector4* bufferArgumentVs00;
	CPU::Vector4* bufferArgumentFs00;
	CPU::Vector4* bufferArgumentFs01;
	CPU::Vector4* bufferArgumentFs02;
	CPU::Vector4* bufferArgumentFs03;
	CPU::Vector4* bufferParameterFs00;
	CPU::Vector4* bufferParameterFs01;
	CPU::Vector4* bufferParameterFs02;
	CPU::Vector4* bufferParameterFs03;
	const int countVertex = countPrimitive * Constant::VERTEX_COUNT_TRIANGLE;
	int countVertexAllocated = 0;
	countVertexAllocated = PrimitiveAllocate(	&bufferCoordinate,
												&bufferColorParts,
												&bufferUVTexture,
												&bufferFlags,
												&bufferArgumentVs00,
												&bufferArgumentFs00,
												&bufferArgumentFs01,
												&bufferArgumentFs02,
												&bufferArgumentFs03,
												&bufferParameterFs00,
												&bufferParameterFs01,
												&bufferParameterFs02,
												&bufferParameterFs03,
												countPrimitive,
												setting,
												flagBreakBatching
											);
	if(countVertex != countVertexAllocated)	{	/* 要求頂点数と異なる */
		return(countVertexAllocated);
	}

	/* 頂点データを格納 */
	/* MEMO: どの順序でインデックスが羅列されているかわからないので、純然と転記しています。                                 */
	/*       ※後に転記用の最適化キャッシュを作っても良いのだけど、恐らくあまり効果が望めない（扇展開とかならまだしも……） */
	Uint16 indexVertex;
	for(int i=0; i<countVertex; i++)	{
		indexVertex = tableIndex[i];
		bufferCoordinate[i].Set(dataCoordinate[indexVertex]);
		bufferColorParts[i].Set(dataColorParts[indexVertex]);
		bufferUVTexture[i].Set(dataUVTexture[indexVertex]);
		bufferFlags[i].Set(dataFlags);
		bufferArgumentVs00[i].Set(dataArgumentVs00[indexVertex]);
		bufferArgumentFs00[i].Set(dataArgumentFs00);
		bufferArgumentFs01[i].Set(dataArgumentFs01);
		bufferArgumentFs02[i].Set(dataArgumentFs02);
		bufferArgumentFs03[i].Set(dataArgumentFs03);
		bufferParameterFs00[i].Set(dataParameterFs00);
		bufferParameterFs01[i].Set(dataParameterFs01);
		bufferParameterFs02[i].Set(dataParameterFs02);
		bufferParameterFs03[i].Set(dataParameterFs03);
	}

	return(countVertex);
}

/* ********************************************************* */
//! （エフェクト）パーティクルのプリミティブの追加
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
int PrimitiveBatcher::PrimitiveAddParticle(	const SettingRender& setting,
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
									)
{
	/* 頂点データのバッファ先頭を取得 */
	CPU::Vector3* bufferCoordinate;
	CPU::Color* bufferColorParts;
	CPU::Vector4* bufferUVTexture;
	CPU::Vector4* bufferFlags;
	CPU::Vector4* bufferArgumentVs00;
	CPU::Vector4* bufferArgumentFs00;
	CPU::Vector4* bufferArgumentFs01;
	CPU::Vector4* bufferArgumentFs02;
	CPU::Vector4* bufferArgumentFs03;
	CPU::Vector4* bufferParameterFs00;
	CPU::Vector4* bufferParameterFs01;
	CPU::Vector4* bufferParameterFs02;
	CPU::Vector4* bufferParameterFs03;
	static const int countVertex = CountGetVertex(Library::KindVertex::TERMINATOR2);
	int countVertexAllocated = 0;
	countVertexAllocated = PrimitiveAllocate(	&bufferCoordinate,
												&bufferColorParts,
												&bufferUVTexture,
												&bufferFlags,
												&bufferArgumentVs00,
												&bufferArgumentFs00,
												&bufferArgumentFs01,
												&bufferArgumentFs02,
												&bufferArgumentFs03,
												&bufferParameterFs00,
												&bufferParameterFs01,
												&bufferParameterFs02,
												&bufferParameterFs03,
												2,	/* Library::KindVertex::TERMINATOR2のプリミティブ数 */
												setting,
												flagBreakBatching
											);
	if(countVertex != countVertexAllocated)	{	/* 要求頂点数と異なる */
		return(countVertexAllocated);
	}

	/* 頂点データを格納 */
	/* MEMO: 実際の頂点データ格納状態は下記になり、処理を最適化してあります。 */
	/*       三角形0: LU( 0) - RD( 1) - RU( 2)                                */
	/*       三角形1: LU( 3) - LD( 4) - RD( 5)                                */
	CPU::Vector3 dataVector3;
	CPU::Vector4 dataVector4;

	{	/* MEMO: 左上 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::LU]);
			bufferCoordinate[0].Set(dataVector3);
			bufferCoordinate[3].Set(dataVector3);

		dataVector4.Set(dataUVTexture[Library::KindVertex::LU]);
			bufferUVTexture[0].Set(dataVector4);
			bufferUVTexture[3].Set(dataVector4);
	}
	{	/* MEMO: 右上 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::RU]);
			bufferCoordinate[2].Set(dataVector3);

		dataVector4.Set(dataUVTexture[Library::KindVertex::RU]);
			bufferUVTexture[2].Set(dataVector4);
	}
	{	/* MEMO: 右下 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::RD]);
			bufferCoordinate[1].Set(dataVector3);
			bufferCoordinate[5].Set(dataVector3);

		dataVector4.Set(dataUVTexture[Library::KindVertex::RD]);
			bufferUVTexture[1].Set(dataVector4);
			bufferUVTexture[5].Set(dataVector4);
	}
	{	/* MEMO: 左下 */
		dataVector3.Set(dataCoordinate[Library::KindVertex::LD]);
			bufferCoordinate[4].Set(dataVector3);

		dataVector4.Set(dataUVTexture[Library::KindVertex::LD]);
			bufferUVTexture[4].Set(dataVector4);
	}
	/* MEMO: 全頂点共通データ */
	for(int i=0; i<countVertex; i++)	{
		bufferColorParts[i].Set(dataColorParts);
		bufferArgumentVs00[i].Set(dataArgumentVs00);
		bufferFlags[i].Set(dataFlags);
		bufferArgumentFs00[i].Set(dataArgumentFs00);
		bufferArgumentFs01[i].Set(dataArgumentFs01);
		bufferArgumentFs02[i].Set(dataArgumentFs02);
		bufferArgumentFs03[i].Set(dataArgumentFs03);
		bufferParameterFs00[i].Set(dataParameterFs00);
		bufferParameterFs01[i].Set(dataParameterFs01);
		bufferParameterFs02[i].Set(dataParameterFs02);
		bufferParameterFs03[i].Set(dataParameterFs03);
	}

	return(countVertex);
}

/* ********************************************************* */
//! プリミティブ種別で使用する頂点数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int PrimitiveBatcher::CountGetVertex(Library::KindVertex::Kind kindVertex)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
