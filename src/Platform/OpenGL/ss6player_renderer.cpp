/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_renderer.h"

#include "ss6player_texture.h"
#include "ss6player_memory.h"
#include "ss6player_buffer.h"
#include "ss6player_shader.h"
#include "ss6player_system.h"

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
const TypeDrawCommandList DrawCommandListInvalid = nullptr;					/* 描画コマンドリストの不正値 */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
/* スワップチェイン情報関係 */
int CountSwapChain = 2;											/* 普通は2 *//* ※定数なのと、色々あることを考えて最初から値を入れてしまっておく（本当はRenderSystemBootUpで入れるのが正しいかと） */
int IndexSwapChain = -1;										/* 現在のスワップチェイン番号 *//* ※コマンドライン作成関数にいちいち引数で渡すとコストがかさむので……。 */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
}	/* SpriteStudio6 */

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
/* レンダリングステートの作成で共通で使用するレンダラ機能のインスタンス */
Renderer Renderer::Instance;

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
/* サンプラ */
/* MEMO: 組み合わせが高々知れているので、使用するバリエーションでサンプラを作成しています。 */
static GLuint SamplerTexture[SpriteStudio6::Texture::KindInterpolation::INTERPOLATION_TERMINATOR][SpriteStudio6::Texture::KindWrap::WRAP_TERMINATOR] = {
	{	/* NEAREST_NEIGHBOR */
		/* CLAMP */		GL_INVALID_INDEX,
		/* REPEAT */	GL_INVALID_INDEX,
		/* MIRROR */	GL_INVALID_INDEX,
	}, {	/* BILINEAR */
		/* CLAMP */		GL_INVALID_INDEX,
		/* REPEAT */	GL_INVALID_INDEX,
		/* MIRROR */	GL_INVALID_INDEX,
	}	
};
GLuint RendererSystemIDGetSampler(SpriteStudio6::Texture::KindInterpolation interpolation, SpriteStudio6::Texture::KindWrap wrap)
{
	return(SamplerTexture[interpolation][wrap]);
}

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
static bool RendererSystemBootUpSampler(void);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                        Renderer::SectionStream::DataEntity */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionStream::DataEntity::DataEntity(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionStream::DataEntity::~DataEntity(void)
{
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionStream::DataEntity::CleanUp(void)
{
	for(int i=0; i<Constant::COUNT_VAO; i++)	{
		IDVertexArrayObject[i] = 0;
	}
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Renderer::SectionStream::DataEntity::BootUp(void)
{
	/* VAOを作成 */
	for(int i=0; i<Constant::COUNT_VAO; i++)	{
		glGenVertexArrays(1, &IDVertexArrayObject[i]);
	}

	return(true);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionStream::DataEntity::ShutDown(void)
{
	/* VAOを解放 */
	for(int i=0; i<Constant::COUNT_VAO; i++)	{
		if(0 != IDVertexArrayObject[i])	{
			glGenVertexArrays(1, &IDVertexArrayObject[i]);
		}
		IDVertexArrayObject[i] = 0;
	}
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                    Renderer::SectionStream */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionStream::SectionStream(void)
{
	Refresh();
}


/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionStream::~SectionStream(void)
{
	ShutDown();
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionStream::BootUp(void)
{
	Refresh();
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionStream::ShutDown(void)
{
	Refresh();
}

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionStream::Refresh(void)

/* ********************************************************* */
//! 描画エンティティ情報設定
/* MEMO:
 * 
 */
// void Renderer::SectionStream::EntitySet(Renderer::DataEntity& dataEntity, bool flagForce)

/* ********************************************************* */
//! 設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionStream::Set(Buffer::GPUBase& buffer, KindDataVertex kind, size_t stride, bool flagForce)

/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::SectionStream::MakeCommand(TypeDrawCommandList commandList)
{
	if(DrawCommandListInvalid == commandList)	{	/* コマンドリストがない */
		return(commandList);
	}

	/* 使用するVAOをバインド */
	DataEntity* dataEntity = Entity;
	if(nullptr == dataEntity)	{	/* 描画エンティティ情報がない */
		return(commandList);
	}
	GLuint idVAO = dataEntity->IDVertexArrayObject[IndexSwapChain];
	if(GL_INVALID_INDEX == idVAO)	{
		return(commandList);
	}
	glBindVertexArray(idVAO);

	/* 頂点ストリームに各データを割当 */
	/* MEMO: ここに来るまでに、Shader.MakeCommandは実行されていて・GPUに定義されているシェーダは確定しています。 */
	/*       ※処理順序は、Renderer::MakeCommandで差配できます。                                                 */
	const SpriteStudio6::Shader* shader = Instance.Shader.InstanceShader;	/* 修飾しないとRenderer.Shaderと衝突する */
	if(nullptr != shader)	{
		TypeAddressGPU addressGPU;
		GLuint* idBuffer;
		Buffer::GPUBase* bufferSource;
		GLuint indexLocation;
		for(int i=0; i<KindDataVertex::DATA_TERMINATOR; i++)	{
//			if(false == FlagDirty[i])	{	/* 変更する必要がない */
//				continue;
//			}
			bufferSource = BufferSource[i];
//			if(nullptr == bufferSource)	{	/* データが指定されていない */
//				continue;
//			}

			indexLocation = shader->LocationAttributeVertex[i];
			if(GL_INVALID_INDEX != indexLocation)	{	/* 頂点が割当っている場合 */
				/* 各バッファを頂点ストリームに接続 */
				bufferSource->AddressGetGPU(&addressGPU);
				idBuffer = reinterpret_cast<GLuint*>(addressGPU);

				glBindBuffer(GL_ARRAY_BUFFER, *idBuffer);
				glEnableVertexAttribArray(indexLocation);	/* 頂点ストリームのアトリビュートを有効化 */
				glVertexAttribPointer(indexLocation, (GLint)(SizeStride[i] / sizeof(Float32)), GL_FLOAT, false, 0, 0);
			}
		}
	}

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                    Renderer::SectionShader */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionShader::SectionShader(void)
{
	Refresh();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionShader::~SectionShader(void)
{
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionShader::BootUp(void)
{
	Refresh();
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionShader::ShutDown(void)
{
	Refresh();
}

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionShader::Refresh(void)

/* ********************************************************* */
//! 設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionShader::Set(const SpriteStudio6::Shader& shader, bool flagForce)

/* ********************************************************* */
//! データ（シェーダ定数）の設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionShader::DataSet(const ShaderData& data, bool flagForce)


/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::SectionShader::MakeCommand(TypeDrawCommandList commandList)
{
	if(DrawCommandListInvalid == commandList)	{	/* コマンドリストがない */
		return(commandList);
	}
	/* MEMO: シェーダが登録されていない場合は、定数もバインドできないので、 */
	/*       処理したフリだけして何もしない。                               */
	if(nullptr == InstanceShader)	{	/* シェーダが登録されていない */
		/* MEMO: nullptrを返すべき？（現在はシェーダ定義をスキップしているだけ） */
		FlagDirty = false;
		FlagDirtyData = false;

		return(commandList);
	}

	/* シェーダの変更 */
	if(true == FlagDirty)	{	/* 変更されている */
		/* シェーダの変更 */
		GLuint idProgram = InstanceShader->IDProgram;
		if(0 < idProgram)	{	/* シェーダが起動している */
			/* シェーダプログラムの設定 */
			glUseProgram(idProgram);

			/* シェーダで使用しているサンプラとテクスチャユニットを接続 */
			GLuint locationSampler;
			for(int i=0; i<Shader::KindSamplerUsage::SAMPLERUSAGE_TERMINATOR; i++)	{
				locationSampler = InstanceShader->LocationSampler[i];
				if(GL_INVALID_INDEX != locationSampler)	{	/* サンプラが有効 */
					glUniform1i(locationSampler, i);
				}
			}

			FlagDirtyData = true;	/* シェーダを変更したら必ず定数も設定する */
		}
	}
	FlagDirty = false;

	/* データ（シェーダ定数）の変更 */
	if(true == FlagDirtyData)	{	/* 変更する必要がある */
		if(nullptr != Data)	{
			TypeDrawCommandList commandListNew = Data->MakeCommandList(commandList, *InstanceShader);	/* 定数を設定 */
			if(nullptr == commandListNew)	{	/* 失敗 */
				commandList = nullptr;
			} else {	/* 成功 */
				commandList = commandListNew;
			}
		}
	}
	FlagDirtyData = false;

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                Renderer::SectionRasterizer */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionRasterizer::SectionRasterizer(void)
{
	Refresh();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionRasterizer::~SectionRasterizer(void)
{
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionRasterizer::BootUp(void)
{
	Refresh();
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionRasterizer::ShutDown(void)
{
	Refresh();
}

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionRasterizer::Refresh(void)

/* ********************************************************* */
//! ラスタライザステートの出力設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionRasterizer::Set(void)

/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::SectionRasterizer::MakeCommand(TypeDrawCommandList commandList)
{
	if(DrawCommandListInvalid == commandList)	{	/* コマンドリストがない */
		return(commandList);
	}
	if(false == FlagDirty)	{	/* 変更する必要がない */
		return(commandList);
	}

	/* MEMO: 現状何もしない。 */

	FlagDirty = false;

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                      Renderer::SectionTest */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionTest::SectionTest(void)
{
	Refresh();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Renderer::SectionTest::~SectionTest(void)
{
}

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTest::BootUp(void)
{
	Refresh();
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTest::ShutDown(void)
{
	Refresh();
}

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionTest::Refresh(void)

/* ********************************************************* */
//! ステンシル設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionTest::StencilSet(KindTypeDraw typeDraw, bool flagMasking, bool flagForce)

/* ********************************************************* */
//! デプス設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionTest::DepthSet(bool flagEnableRead, bool flagEnableWrite, bool flagForce)

/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::SectionTest::MakeCommand(TypeDrawCommandList commandList)
{
	if(DrawCommandListInvalid == commandList)	{	/* コマンドリストがない */
		return(commandList);
	}
	if(false == FlagDirty)	{	/* 変更する必要がない */
		return(commandList);
	}

	/* ステンシルテストを設定 */
	struct	ValuesStencilTest	{
		bool Enable;
		GLenum Function;
		GLenum Operation;
	};
	static const ValuesStencilTest tableStencil[KindTypeDraw::TERMINATOR][2] = {
		{	/* NORMAL */
			/* false */	{	false,	GL_EQUAL,	GL_KEEP			},
			/* true */	{	true,	GL_EQUAL,	GL_KEEP			},
		},
		{	/* MASK_PREDRAW */
			/* false */	{	true,	GL_ALWAYS,	GL_INCR_WRAP	},
			/* true */	{	true,	GL_ALWAYS,	GL_INVERT		},
		},
		{	/* MASK */
			/* false */	{	true,	GL_ALWAYS,	GL_DECR_WRAP	},
			/* true */	{	true,	GL_ALWAYS,	GL_INVERT		},
		}
	};
	int index = (false == FlagMaskingStencil) ? 0 : 1;
	const ValuesStencilTest* dataStencil = &tableStencil[TypeDraw][index];
	if(false == dataStencil->Enable)	{	/* ステンシルテストを行わない */
		glDisable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, (GLint)0xff, (GLuint)0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	} else {	/* ステンシルテストを行う */
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(dataStencil->Function, (GLint)0xff, (GLuint)0xff);
		glStencilOp(dataStencil->Operation, GL_KEEP, GL_KEEP);
	}

	/* 深度テストを設定 */
	if(false == FlagEnableReadDepth)	{	/* テストしない */
		glDisable(GL_DEPTH_TEST);
	} else {	/* テストする */
		glEnable(GL_DEPTH_TEST);
	}
	if(false == FlagEnableWriteDepth)	{	/* 深度を書かない */
		glDepthMask(GL_FALSE);
	} else {	/* 深度を書く */
		glDepthMask(GL_TRUE);
	}

	FlagDirty = false;

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                     Renderer::SectionBlend */
/* -------------------------------------------------------------------------- */
/* 各ブレンド種別で透明ピクセルを廃棄するかのフラグ */
/* MEMO: true==廃棄・false==非廃棄（描画） */
const Uint32 /* Renderer::SectionBlend::FlagBitControlPixel */ Renderer::SectionBlend::FlagControlPixel[Renderer::SectionBlend::KindType::TERMINATOR] = {
	/* MIX: 通常 */						FLAG_PIXEL_DISCARD | FLAG_PIXEL_PREMULTIPLIED_ALPHA,
	/* ADD: 加算 */						FLAG_PIXEL_DISCARD,
	/* SUB: 減算 */						FLAG_PIXEL_DISCARD,
	/* MUL: アルファ乗算 */				FLAG_PIXEL_DISCARD,

	/* MUL_NA: （アルファなし）乗算 */	FLAG_PIXEL_CLEAR,
	/* SCR: スクリーン */				FLAG_PIXEL_CLEAR,
	/* EXC: 除外 */						FLAG_PIXEL_CLEAR,
	/* INV: 反転 */						FLAG_PIXEL_CLEAR,
};

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Renderer::SectionBlend::SectionBlend(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Renderer::SectionBlend::~SectionBlend(void)

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionBlend::BootUp(void)

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionBlend::ShutDown(void)

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionBlend::Refresh(void)

/* ********************************************************* */
//! 設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionBlend::Set(Library::KindOperationBlend::Kind type, bool flagForce)

/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::SectionBlend::MakeCommand(TypeDrawCommandList commandList)
{
	if(DrawCommandListInvalid == commandList)	{	/* コマンドリストがない */
		return(commandList);
	}
	if(false == FlagDirty)	{	/* 変更する必要がない */
		return(commandList);
	}

	/* 合成方法を設定 */
	static const GLenum tableFunction[KindType::TERMINATOR][2] = {	/* [Type][Color/Alpha] */
		/* MIX */		{	GL_FUNC_ADD,				GL_FUNC_ADD					},
		/* ADD */		{	GL_FUNC_ADD,				GL_FUNC_ADD					},
		/* SUB */		{	GL_FUNC_REVERSE_SUBTRACT,	GL_FUNC_REVERSE_SUBTRACT	},
		/* MUL */		{	GL_FUNC_ADD,				GL_FUNC_ADD					},
		/* MUL_NA */	{	GL_FUNC_ADD,				GL_FUNC_ADD					},
		/* SCR */		{	GL_FUNC_ADD,				GL_FUNC_ADD					},
		/* EXC */		{	GL_FUNC_ADD,				GL_FUNC_ADD					},
		/* INV */		{	GL_FUNC_ADD,				GL_FUNC_ADD					},
	};
	static const GLenum tableSource[KindType::TERMINATOR][2] = {	/* [Type][Color/Alpha] */
		/* Color */
		/* MIX */		{	GL_ONE,					GL_ONE					},
		/* ADD */		{	GL_SRC_ALPHA,			GL_SRC_ALPHA			},
		/* SUB */		{	GL_SRC_ALPHA,			GL_SRC_ALPHA			},
		/* MUL */		{	GL_DST_COLOR,			GL_DST_COLOR			},
		/* MUL_NA */	{	GL_ZERO,				GL_ZERO					},
		/* SCR */		{	GL_DST_COLOR,			GL_DST_COLOR			},
		/* EXC */		{	GL_ONE_MINUS_DST_COLOR,	GL_ONE_MINUS_DST_COLOR	},
		/* INV */		{	GL_ONE_MINUS_DST_COLOR,	GL_ONE_MINUS_DST_COLOR	},
	};
	static const GLenum tableDestination[KindType::TERMINATOR][2] = {	/* [Type][Color/Alpha] */
		/* Color */
		/* MIX */		{	GL_ONE_MINUS_SRC_ALPHA,	GL_ONE_MINUS_SRC_ALPHA	},
		/* ADD */		{	GL_ONE,					GL_ONE					},
		/* SUB */		{	GL_ONE,					GL_ONE					},
		/* MUL */		{	GL_ONE_MINUS_SRC_ALPHA,	GL_ONE_MINUS_SRC_ALPHA	},
		/* MUL_NA */	{	GL_SRC_COLOR,			GL_SRC_COLOR			},
		/* SCR */		{	GL_ONE,					GL_ONE					},
		/* EXC */		{	GL_ONE_MINUS_SRC_COLOR,	GL_ONE_MINUS_SRC_COLOR	},
		/* INV */		{	GL_ZERO,				GL_ZERO					},
	};

	glEnable(GL_BLEND);	/* ブレンド有効 */
	glBlendEquationSeparate(	tableFunction[Type][0],	/* ブレンド式: 色 */
								tableFunction[Type][1]	/* ブレンド式: アルファ */
						);
	glBlendFuncSeparate(	tableSource[Type][0],		/* ブレンド係数: SRC色 */
							tableDestination[Type][0],	/* ブレンド係数: DST色 */
							tableSource[Type][1],		/* ブレンド係数: SRCアルファ */
							tableDestination[Type][1]	/* ブレンド係数: DSTアルファ */
					);

	FlagDirty = false;

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                    Renderer::SectionTarget */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTarget::CleanUp(void)
{
	for(int i=0; i<Constant::COUNT_TARGET; i++)	{
		Color[i].CleanUp();
	}
	Test.CleanUp();

	FlagDirty = false;

	CountTargetColor = 0;
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Renderer::SectionTarget::~SectionTarget(void)

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTarget::BootUp(void)
{
	CleanUp();

	/* 色ターゲットの最大バインド数を取得 */
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &CountTargetColor);

	Refresh();
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTarget::ShutDown(void)
{
	CleanUp();
}

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::SectionTarget::Refresh(void)

/* ********************************************************* */
//! 色ターゲットを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTarget::ColorSet(int index, Texture* texture, bool flagClear, const CPU::Color& colorClear, bool flagForce)
{
	if((0 > index) || (Constant::COUNT_TARGET <= index))	{	/* MRT最大数オーバー */
		/* MEMO: 一応何もしないで帰ってしまう。 */
		return;
	}

	BufferColor& buffer = Color[index];
	buffer.Texture = texture;
	buffer.FlagClear = flagClear;
	buffer.ColorClear.Set(colorClear);

	FlagDirty = true;
}

/* ********************************************************* */
//! テストターゲットを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTarget::TestSet(Texture* texture, bool flagClear, int stencilClear, float depthClear, bool flagForce)
{
	Test.Texture = texture;
	Test.FlagClear = flagClear;
	Test.StencilClear = stencilClear;
	Test.DepthClear = depthClear;
	FlagDirty = true;
}

/* ********************************************************* */
//! ターゲットのクリア
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Renderer::SectionTarget::Purge(void)
{
	for(int i=0; i<Constant::COUNT_TARGET; i++)	{
		ColorSet(i, nullptr, false);
	}
	TestSet(nullptr, false);
}

/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::SectionTarget::MakeCommand(TypeDrawCommandList commandList)
{
	GLenum errorGL;

	if(DrawCommandListInvalid == commandList)	{	/* コマンドリストがない */
		return(commandList);
	}
	if(false == FlagDirty)	{	/* 変更する必要がない */
		return(commandList);
	}

	/* 描画用RBOを整理・設定 */
	GLenum buffersDraw[Constant::COUNT_TARGET];	/* 最大で確保しておく */
	int countTargetColor = Constant::COUNT_TARGET;
	if(CountTargetColor < countTargetColor)	{	/* 動作環境の最大でクリップ */
		countTargetColor = CountTargetColor;
	}

	GLenum idAttach;
	Texture* texture;
	Sint32 idTexture;
	Uint32 statusTexture;
	for(int i=0; i<countTargetColor; i++)	{
		/* テクスチャを設定 */
		texture = Color[i].Texture;
		if(nullptr == texture)	{	/* テクスチャが設定されていない */
			buffersDraw[i] = GL_NONE;	/* 描画に使用しない */
			continue;
		}
		statusTexture = texture->Status;
		if(0 == (statusTexture & Texture::FlagBitStatus::USAGE_TARGET))	{	/* ターゲットとして使用できない */
			buffersDraw[i] = GL_NONE;	/* 描画に使用しない */
			continue;
		}
		if(0 != (statusTexture & Texture::FlagBitStatus::USAGE_TARGET_TEST))	{	/* テスト用ターゲット */
			buffersDraw[i] = GL_NONE;	/* 描画に使用しない */
			continue;
		}

		idAttach = GL_COLOR_ATTACHMENT0 + i;
		buffersDraw[i] = idAttach;
		if(0 != (statusTexture & Texture::FlagBitStatus::USAGE_TARGET_MASTER))	{	/* マスタターゲット */
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, idAttach, GL_RENDERBUFFER, 0);
			errorGL = glGetError();
		} else {
			idTexture = texture->IndexGetSlotDiscriptor();
			if(0 >= idTexture)	{	/* テクスチャのIDが不正 */
				buffersDraw[i] = 0;
				continue;
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, idAttach, GL_TEXTURE_2D, idTexture, 0);
			errorGL = glGetError();
		}
	}
	glDrawBuffers(countTargetColor, buffersDraw);	/* 描画バッファを設定 */

	/* テスト用バッファの設定 */
	{
		GLuint idTest = 0;
		bool flagUseTest = true;

		/* テクスチャを設定 */
		texture = Test.Texture;
		if(nullptr == texture)	{	/* テクスチャが設定されていない */
			flagUseTest = false;
		} else {
			statusTexture = texture->Status;
			if(0 == (statusTexture & Texture::FlagBitStatus::USAGE_TARGET))	{	/* ターゲットとして使用できない */
				flagUseTest = false;
			} else {
				if(0 == (statusTexture & Texture::FlagBitStatus::USAGE_TARGET_TEST))	{	/* テスト用ターゲットではない */
					flagUseTest = false;
				}
			}
		}

		if(true == flagUseTest)	{
			if(0 != (statusTexture & Texture::FlagBitStatus::USAGE_TARGET_MASTER))	{	/* マスタターゲット */
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
				errorGL = glGetError();
			} else {
				idTexture = texture->IndexGetSlotDiscriptor();
				if(0 < idTexture)	{	/* テクスチャのIDが正常 */
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, idTexture, 0);
					errorGL = glGetError();
				}
			}
		}
	}

	/* バッファのクリア */
	GLfloat colorClear[4];
	CPU::Color* bufferColor;
	for(int i=0; i<countTargetColor; i++)	{
		if(GL_NONE != buffersDraw[i])	{
			if(true == Color[i].FlagClear)	{
				bufferColor = &Color[i].ColorClear;

				colorClear[0] = bufferColor->GetR();
				colorClear[1] = bufferColor->GetG();
				colorClear[2] = bufferColor->GetB();
				colorClear[3] = bufferColor->GetA();

				glClearBufferfv(GL_COLOR, i, colorClear);
			}
		}
	}
	if(true == Test.FlagClear)	{
		glClearBufferfi(GL_DEPTH_STENCIL, 0, (GLfloat)Test.DepthClear, (GLint)Test.StencilClear);
	}

	FlagDirty = false;

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                   Renderer */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Renderer::Renderer(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Renderer::~Renderer(void)

/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::BootUp(void)

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::ShutDown(void)

/* ********************************************************* */
//! リフレッシュ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Renderer::Refresh(void)

/* ********************************************************* */
//! 描画コマンドを作成
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::MakeCommand(TypeDrawCommandList commandList)
{
	TypeDrawCommandList commandListUpdate = commandList;
	commandListUpdate = Shader.MakeCommand(commandListUpdate);
	commandListUpdate = Target.MakeCommand(commandListUpdate);
	commandListUpdate = Rasterizer.MakeCommand(commandListUpdate);

	commandListUpdate = Blend.MakeCommand(commandListUpdate);
	commandListUpdate = Test.MakeCommand(commandListUpdate);

	commandListUpdate = Stream.MakeCommand(commandListUpdate);

	return(commandListUpdate);
}

/* ********************************************************* */
//! プリミティブ描画（のコマンドを作成）: 頂点インデックス型
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::PrimitiveDraw(TypeDrawCommandList commandList, Buffer::GPUBase& bufferIndex, Uint16 countVertexIndex, Uint16 offsetIndex)
{
	/* MEMO: 未使用につき処理しない */
	return(commandList);
}

/* ********************************************************* */
//! プリミティブ描画（のコマンドを作成）: 頂点直接型
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Renderer::PrimitiveDraw(TypeDrawCommandList commandList, int countVertex, int offsetVertex)
{
	/* 使用するVAOをバインド */
	SectionStream::DataEntity* dataEntity = Instance.Stream.Entity;
	if(nullptr == dataEntity)	{	/* 描画エンティティ情報がない */
		return(commandList);
	}
	GLuint idVAO = dataEntity->IDVertexArrayObject[IndexSwapChain];
	if(GL_INVALID_INDEX == idVAO)	{
		return(commandList);
	}

	glBindVertexArray(idVAO);
	glDrawArrays(GL_TRIANGLES, (GLint)offsetVertex, (GLsizei)countVertex);

	return(commandList);
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            [Global-Static] */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                       [Exceptional-Static] */
/* -------------------------------------------------------------------------- */
/* この関数群は、システム全体の初期化・終了に関わる処理のため、プラットフォー */
/* ムやフレームワーク毎で実装が変わるので、呼出側でexternしてください。       */
/* ※ヘッダに含むのが適切でないプロトタイプを持っています。                   */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! レンダリング管理のシステムを起動
/*!
@param	setting
	システム初期化設定

@retval	関数値
	true == 成功
	false == 失敗

レンダリング（コマンドリスト作成）を管理するための基底処理を
	初期化します。
※主にプラットフォームライブラリなどの初期化のための関数なので、
	別途で起動処理が確定しているような場合には本関数は空（や
	呼ばれなくても）問題はありません。

instanceFrameworkGFX/deviceGFXはtexture処理のローカル内でキャッ
	シュ（保持）されます。
これらのキャッシュはテクスチャやシェーダなどで使用しているため、
	本関数は他のモジュールの起動よりも先に呼び出してください。
*/
bool RendererSystemBootUp(const SettingSystem& setting)
{
	/* フレームワーク・デバイスのキャッシュ */
//	CountSwapChain = 2;
	IndexSwapChain = 0;	/* レンダリング前にも転送用バッファ構築で使用されるので、初期バッファを指しています。 */

	/* 各ステートの作成 */
	if(false == RendererSystemBootUpSampler())	{	/* 失敗 */
		return(false);
	}

	/* インスタンスの起動 */
	Renderer::Instance.BootUp();

	return(true);
}
static bool RendererSystemBootUpSampler(void)
{	/* MEMO: テクスチャサンプリングステートの初期設置 */
	GLenum tableFilter[SpriteStudio6::Texture::KindInterpolation::INTERPOLATION_TERMINATOR] = {	/* フィルタモード */
		/* NEAREST_NEIGHBOR */	GL_NEAREST,
		/* BILINEAR */			GL_LINEAR,
	};
	GLenum tableWrap[SpriteStudio6::Texture::KindWrap::WRAP_TERMINATOR] = {	/* ラップモード */
		/* CLAMP */				GL_CLAMP_TO_EDGE,
		/* REPEAT */			GL_REPEAT,
		/* MIRROR */			GL_MIRRORED_REPEAT,
	};

	GLuint idSampler;
	GLenum filter;
	GLenum wrap;
	for(int i=0; i<SpriteStudio6::Texture::KindInterpolation::INTERPOLATION_TERMINATOR; i++)	{	/* 補間（フィルタ） */
		filter = tableFilter[i];
		for(int j=0; j<SpriteStudio6::Texture::KindWrap::WRAP_TERMINATOR; j++)	{	/* ラップモード */
			glGenSamplers(1, &idSampler);
			if(GL_INVALID_INDEX == idSampler)	{	/* 失敗 */
				return(false);
			}
			wrap = tableWrap[j];

			glSamplerParameteri(idSampler, GL_TEXTURE_MAG_FILTER, filter);
			glSamplerParameteri(idSampler, GL_TEXTURE_MIN_FILTER, filter);
			glSamplerParameteri(idSampler, GL_TEXTURE_WRAP_S, wrap);
			glSamplerParameteri(idSampler, GL_TEXTURE_WRAP_T, wrap);

			SamplerTexture[i][j] = idSampler;
		}
	}

	return(true);
}

/* ********************************************************* */
//! レンダリング管理のシステムを終了
/*!
@param	なし

@retval	なし

RendererSystemBootUp関数で起動した基底処理を終了します。
※主にプラットフォームライブラリなどの終了処理のための関数なので、
	別途で終了処理が確定しているような場合には本関数は空（や呼ば
	れなくても）問題はありません。

内部でテクスチャやシェーダなどで使用しているフレームワークや
	デバイスの参照を消しているので、他のモジュールの終了よりも
	後に呼び出してください。
*/

void RendererSystemShutDown(void)
{
	/* インスタンスの終了 */
	Renderer::Instance.ShutDown();

	/* 各ステートの終了 */
}

/* ********************************************************* */
//! SS6Player内で使用するスワップチェインの更新
/*!
@param	offsetIndex
	現在のスワップチェインから更新する値幅
	省略時: 1

@retval	なし

スワップチェインの更新となっていますが、主にSS6プレーヤ内の
	BufferMultiオブジェクトの「現在更新中（描画時にGPUに転送
	する）」のバッファインデックスを更新します。
※システム全体でのフレームバッファのスワップチェインではなく、
	「SS6Playerが管理しているバッファのスワップチェイン」と
	いう意味です。

offsetIndexは、念のため更新する差分値を指定できるようにして
	ありますが、原則省略記載（1）で扱うことを推奨します。

このスワップチェインのインデックスは、SS6Playerのシステム全域
	で共通で使用している値なので、更新（本関数の呼出）はシス
	テム全体の処理中（処理ループ中）で1回のみ行うようにして
	ください（ループの途中で更新したりするとBufferMultiの管理
	が狂って、更新したバッファが描画時に転送されないなどの障
	害が起こります）。
本関数の呼出タイミングは、特段の意図がない限り「描画が終了し
	た」後で行うようにしてください。
※メイン処理のループが描画コマンド構築スレッドと完全非同期で
	回る（フレームスキップとは関係なく、メインループ内での
	Entity::Update群の処理中に描画コマンド構築処理が実行され
	る～描画コマンド構築スレッドとメインスレッドとのスレッド
	間同期の保証がとられていない～場合に限る）時だけ、本関数
	の内部実装に工夫が必要になるかと思われます（Entity::Update
	中に、GPU転送するためのバッファに各種情報を設定して・描画
	処理スレッドでのEntity::Drawで更新済バッファをGPUに転送
	しているために、メインスレッドで更新最中に本更新を行うと
	管理バッファのインデックスが更新中に想定外の変化してしま
	うため、バッファの内部管理が狂ってしまいます）。
*/
void RendererSwapChainUpdate(int offsetIndex)
{
	IndexSwapChain++;
	IndexSwapChain %= CountSwapChain;
}

}	/* SpriteStudio6 */
