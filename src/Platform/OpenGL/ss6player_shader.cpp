/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_shader.h"

#include "ss6player_memory.h"
#include "ss6player_file.h"
#include "ss6player_system.h"

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */
/* シェーダ定数のバインドポイント */
#define SHADER_UBO_BINDPOINT	0

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */
/* DeclarationVertex関係 */
/* MEMO: 標準マスク用の頂点宣言は標準と同じなので存在していません。 */
DeclarationVertex DeclarationVertex::BuiltInStandard;

/* Shader関係 */
Shader Shader::BuiltInStandard;
Shader Shader::BuiltInStandardMask;

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
static void ShaderReplaceCRLF(char* source);
static bool ShaderCheckCompile(GLuint idShader);
static bool ShaderCheckLink(GLuint idProgram);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          DeclarationVertex */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
DeclarationVertex::DeclarationVertex(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
DeclarationVertex::~DeclarationVertex(void)
{
	/* MEMO: 念のため。 */
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void DeclarationVertex::CleanUp(void)
{
	Work = nullptr;
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool DeclarationVertex::IsValid(void)

/* ********************************************************* */
//! 頂点宣言を設定（起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool DeclarationVertex::BootUp(void)
{
	return(true);
}

/* ********************************************************* */
//! 標準の頂点宣言を設定（起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool DeclarationVertex::BootUpStandard(void)
{
	CleanUp();

	/* MEMO: 現在何もしない。 */

	return(true);
}

/* ********************************************************* */
//! 頂点宣言を解放（終了）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void DeclarationVertex::ShutDown(void)
{
	if(nullptr != Work)	{	/* メモリを使用 */
		SpriteStudio6::Memory::Free(Work);
	}

	CleanUp();
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                 ShaderData */
/* -------------------------------------------------------------------------- */
void ShaderData::CleanUp(void)
{
	FlagBootedUp = false;

//	View

//	BufferConstantShaderView
}

ShaderData::ShaderData(void)
{
	CleanUp();
}

ShaderData::~ShaderData(void)
{
	/* MEMO: 念のため */
	ShutDown();
}

bool ShaderData::BootUp(void)
{
	if(true == IsValid())	{	/* 多重起動 */
		return(false);
	}

	CleanUp();

	View.SetDefault();

	BufferConstantShaderView.BootUp(Buffer::KindUsageBufferGPU::USAGE_CONSTANT, ConstantView::SizeGetBuffer());
	BufferConstantShaderView.DataSetInitial(&View, ConstantView::SizeGetBuffer());

	FlagBootedUp = true;

	return(true);
}

void ShaderData::ShutDown(void)
{
	if(false == IsValid())	{	/* 未使用 */
		return;
	}

	BufferConstantShaderView.ShutDown();

	CleanUp();
}

// bool ShaderData::IsValid(void)
void ShaderData::DataSet(void)
{
	BufferConstantShaderView.DataSet(&View, ConstantView::SizeGetBuffer());
}
TypeDrawCommandList ShaderData::MakeCommandList(TypeDrawCommandList commandList, const Shader& shader) const
{
	TypeAddressGPU addressGPU;

	/* ビューブロック */
	GLuint idProgramShader = shader.IDProgram;
	if(0 < idProgramShader)	{	/* シェーダが存在 */
		/* ViewブロックのUBOのIDを取得 */
		BufferConstantShaderView.AddressGetGPU(&addressGPU);
		if(nullptr != addressGPU)	{
			GLuint* idBufferStorage = reinterpret_cast<GLuint*>(addressGPU);
			GLuint idBuffer = *idBufferStorage;
			if(0 < idBuffer)	{	/* バッファが存在 */
				/* シェーダのViewBlockのインデックスを取得 */
				GLuint indexBlockShader = shader.LocationUniformBlock[Shader::KindUniformBlock::UNIFORMBLOCK_VIEW];
				if(GL_INVALID_INDEX != indexBlockShader)	{
					/* シェーダ定数をバインディング */
					glBindBufferBase(GL_UNIFORM_BUFFER, SHADER_UBO_BINDPOINT, idBuffer);
				}
			}
		}
	}

	return(commandList);
}
// void ShaderData::MatrixSetProjection(const FPU::Matrix4x4& matrixProjection)
// void ShaderData::MatrixSetView(const FPU::Matrix4x4& matrixView)
// void ShaderData::MatrixSetWorld(const FPU::Matrix4x4& matrixWorld)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                     Shader */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Shader::Shader(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Shader::~Shader(void)
{
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Shader::CleanUp(void)
{
	Work = nullptr;
	IDProgram = 0;

	for(int i=0; i<KindUniformBlock::UNIFORMBLOCK_TERMINATOR; i++)	{
		LocationUniformBlock[i] = GL_INVALID_INDEX;
	}
	for(int i=0; i<KindSamplerUsage::SAMPLERUSAGE_TERMINATOR; i++)	{
		LocationSampler[i] = GL_INVALID_INDEX;
	}
	for(int i=0; i<Renderer::SectionStream::KindDataVertex::DATA_TERMINATOR; i++)	{
		LocationAttributeVertex[i] = GL_INVALID_INDEX;
	}

	IndexArchive = -1;

	DeclarationVertex = nullptr;
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// bool Shader::IsValid(void) const

/* ********************************************************* */
//! シェーダを設定（起動）
/* --------------------------------------------------------- */
/* MEMO:
 * OpenGLはシェーダは分離型のみです。
 */
int Shader::BootUp(void* dataVertex, void* dataPixel, SpriteStudio6::DeclarationVertex& declarationVertex, int indexArchive, bool flagInitializeArchive)
{
	static const char* NameShaderVertexData[] = {
		/* DATA_COORDINATE */		"i_Position",
		/* DATA_UV */				"i_UV0",
		/* DATA_PARTSCOLOR */		"i_ColorParts",
		/* DATA_FLAGS */			"i_Flags",
		/* DATA_ARGUMENT_VS00 */	"i_ArgumentVs00",
		/* DATA_ARGUMENT_FS00 */	"i_ArgumentFs00",
		/* DATA_ARGUMENT_FS01 */	"i_ArgumentFs01",
		/* DATA_ARGUMENT_FS02 */	"i_ArgumentFs02",
		/* DATA_ARGUMENT_FS03 */	"i_ArgumentFs03",
		/* DATA_PARAMETER_FS00 */	"i_ParameterFs00",
		/* DATA_PARAMETER_FS01 */	"i_ParameterFs01",
		/* DATA_PARAMETER_FS02 */	"i_ParameterFs02",
		/* DATA_PARAMETER_FS03 */	"i_ParameterFs03",
	};

	/* MEMO: ブロック名とサンプラ名は、未使用の部分はnullptrにしておけば検索しません。 */
	static const char* NameUniformBlock[KindUniformBlock::UNIFORMBLOCK_TERMINATOR] = {
		/* UNIFORMBLOCK_VIEW */		"View",
	};
	static const char* NameSampler[KindSamplerUsage::SAMPLERUSAGE_TERMINATOR] = {
		/* SAMPLERUSAGE_ALBEDO0 */		"TexureAlbedo0",
		/* SAMPLERUSAGE_ALBEDO1 */		nullptr,
		/* SAMPLERUSAGE_ALBEDO2 */		nullptr,
		/* SAMPLERUSAGE_NORMAL0 */		nullptr,
		/* SAMPLERUSAGE_NORMAL1 */		nullptr,
		/* SAMPLERUSAGE_NORMAL2 */		nullptr,
		/* SAMPLERUSAGE_SPECULAR0 */	nullptr,
		/* SAMPLERUSAGE_SPECULAR1 */	nullptr,
		/* SAMPLERUSAGE_SPECULAR2 */	nullptr,
	};

	GLchar* dataSource[1] = { nullptr };
	GLuint idVertex = 0;
	GLuint idFragment = 0;

	/* シェーダを作成 */
	idVertex = glCreateShader(GL_VERTEX_SHADER);
	idFragment = glCreateShader(GL_FRAGMENT_SHADER);
	if((0 == idVertex) && (0 == idFragment))	{
		goto BootUp_ErrorEnd;
	}

	/* 頂点シェーダのコンパイル */
	ShaderReplaceCRLF(reinterpret_cast<char*>(dataVertex));	/* CR+LFを置き換え */
	dataSource[0] = reinterpret_cast<GLchar*>(dataVertex);
	glShaderSource(idVertex, 1, dataSource, nullptr);
	glCompileShader(idVertex);
	if(false == ShaderCheckCompile(idVertex))	{	/* 失敗 */
		goto BootUp_ErrorEnd;
	}

	/* フラグメントシェーダのコンパイル */
	ShaderReplaceCRLF(reinterpret_cast<char*>(dataPixel));	/* CR+LFを置き換え */
	dataSource[0] = reinterpret_cast<GLchar*>(dataPixel);
	glShaderSource(idFragment, 1, dataSource, nullptr);
	glCompileShader(idFragment);
	if(false == ShaderCheckCompile(idFragment))	{	/* 失敗 */
		goto BootUp_ErrorEnd;
	}

	/* シェーダプログラムに各シェーダを設定 */
	IDProgram = glCreateProgram();
	if(0 == IDProgram)	{
		goto BootUp_ErrorEnd;
	}
	glAttachShader(IDProgram, idVertex);
	glAttachShader(IDProgram, idFragment);
	glDeleteShader(idFragment);
	glDeleteShader(idVertex);
	idFragment = 0;
	idVertex = 0;

	/* シェーダプログラムのリンク */
	glLinkProgram(IDProgram);
	if(false == ShaderCheckLink(IDProgram))	{	/* 失敗 */
		goto BootUp_ErrorEnd;
	}

	/* 頂点シェーダの頂点情報のロケーション取得 */
	/* MEMO: 基本的にはシェーダ側でlocationを縛っているのでiと同じ値になるはずだが、 */
	/*       シェーダがコンパイル＆リンクの結果で、最適化でハジかれるものがあるので  */
	/*       取得しておく。                                                          */
	for(int i=0; i<Renderer::SectionStream::KindDataVertex::DATA_TERMINATOR; i++)	{
		LocationAttributeVertex[i] = glGetAttribLocation(IDProgram, NameShaderVertexData[i]);
	}

	/* 頂点シェーダのシェーダ定数のロケーション取得 */
	for(int i=0; i<KindUniformBlock::UNIFORMBLOCK_TERMINATOR; i++)	{
		if(nullptr != NameUniformBlock[i])	{
			LocationUniformBlock[i] = glGetUniformBlockIndex(IDProgram, NameUniformBlock[i]);
		} else {
			LocationUniformBlock[i] = GL_INVALID_INDEX;
		}
	}
	if(GL_INVALID_INDEX != LocationUniformBlock[KindUniformBlock::UNIFORMBLOCK_VIEW])	{
		/* シェーダへのUBOのバインドポイントを設定 */
		glUniformBlockBinding(IDProgram, LocationUniformBlock[KindUniformBlock::UNIFORMBLOCK_VIEW], SHADER_UBO_BINDPOINT);
	}

	/* ピクセルシェーダのサンプラのロケーション取得 */
	for(int i=0; i<KindSamplerUsage::SAMPLERUSAGE_TERMINATOR; i++)	{
		if(nullptr != NameSampler[i])	{
			LocationSampler[i] = glGetUniformLocation(IDProgram, NameSampler[i]);
		} else {
			LocationSampler[i] = GL_INVALID_INDEX;
		}
	}
	
	/* 各種値の設定 */
	IndexArchive = indexArchive;
	DeclarationVertex = nullptr;	/* 頂点宣言もいらないかも。 */

	return(indexArchive);

BootUp_ErrorEnd:;
	if(0 != idFragment)	{
		glDeleteShader(idFragment);
	}
	if(0 != idVertex)	{
		glDeleteShader(idVertex);
	}

	CleanUp();
	return(-1);
}
static void ShaderReplaceCRLF(char* source)
{
	/* MEMO: OpenGLのランタイムのシェーダコンパイラがCR+LFをエラーと見做してエラーを出す環境の場合、 */
	/*       ここにコンパイル前の事前処理を挟むことができます。                                      */
}
static bool ShaderCheckCompile(GLuint idShader)
{
	/* コンパイル結果の取得 */
	GLint flagCompileShader;
	glGetShaderiv(idShader, GL_COMPILE_STATUS, &flagCompileShader);
	bool flagRv = (GL_FALSE == flagCompileShader) ? false : true;

	/* コンパイルログの取得 */
	GLsizei sizeBufferLog;
	glGetShaderiv(idShader, GL_INFO_LOG_LENGTH, &sizeBufferLog);
	if(1 < sizeBufferLog)	{	/* ログの中身が存在 */
		void* handleText = nullptr;
		void* handleMemory = nullptr;
		handleText = Memory::Allocate(&handleMemory, sizeBufferLog);
		if(nullptr == handleText)	{	/* 確保失敗 */
			/* MEMO: ログの確保失敗なのでそのまま帰ってしまう。 */
			return(flagRv);
		}

		GLchar* textLog = reinterpret_cast<GLchar*>(handleText);
		GLsizei lengthLog;
		glGetShaderInfoLog(idShader, sizeBufferLog, &lengthLog, textLog);

		Memory::Free(handleMemory);
	}

	return(flagRv);
}
static bool ShaderCheckLink(GLuint idProgram)
{
	/* リンク結果の取得 */
	GLint flagLinkShader;
	glGetProgramiv(idProgram, GL_LINK_STATUS, &flagLinkShader);
	bool flagRv = (GL_FALSE == flagLinkShader) ? false : true;

	/* リンクログの取得 */
	GLsizei sizeBufferLog;
	glGetProgramiv(idProgram, GL_INFO_LOG_LENGTH, &sizeBufferLog);
	if(1 < sizeBufferLog)	{	/* ログの中身が存在 */
		void* handleText = nullptr;
		void* handleMemory = nullptr;
		handleText = Memory::Allocate(&handleMemory, sizeBufferLog);
		if(nullptr == handleText)	{	/* 確保失敗 */
			/* MEMO: ログの確保失敗なのでそのまま帰ってしまう。 */
			return(flagRv);
		}

		GLchar* textLog = reinterpret_cast<GLchar*>(handleText);
		GLsizei lengthLog;
		glGetProgramInfoLog(idProgram, sizeBufferLog, &lengthLog, textLog);

		Memory::Free(handleMemory);
	}

	return(flagRv);
}


/* ********************************************************* */
//! シェーダを解放（終了）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Shader::ShutDown(bool flagFinalizeArchive)
{
	/* シェーダプログラムを解放 */
	if(0 != IDProgram)	{
		glDeleteProgram(IDProgram);
	}

	/* 自身の未使用化 */
	CleanUp();
}

/* ********************************************************* */
//! シェーダのサンプラにテクスチャをバインド
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
/* MEMO: 隠蔽したい関数（設置方法が暫定）なので、ここでexternしています。 */
extern GLuint RendererSystemIDGetSampler(SpriteStudio6::Texture::KindInterpolation interpolation, SpriteStudio6::Texture::KindWrap wrap);
TypeDrawCommandList Shader::MakeCommandTexture(	TypeDrawCommandList commandList,
												KindSamplerUsage kindSampler,
												const Texture& texture,
												Texture::KindInterpolation interpolation,
												Texture::KindWrap wrap
											) const
{
	/* サンプラとテクスチャのIDを取得 */
	GLuint idSampler = RendererSystemIDGetSampler(interpolation, wrap);
	int idTexture = texture.IndexGetSlotDiscriptor();
	if((GL_INVALID_INDEX == idSampler) || (0 > idTexture))	{	/* エラー */
		return(nullptr);
	}

	/* テクスチャユニットにサンプラを設定 */
	/* MEMO: シェーダのサンプラロケーションはRenderer::SectionShaderでGK_TEXTURE～に結びつけてあります。 */
	static const GLenum unitTexture[] = {
		/* SAMPLERUSAGE_ALBEDO0 */		GL_TEXTURE0,
		/* SAMPLERUSAGE_ALBEDO1 */		GL_TEXTURE1,
		/* SAMPLERUSAGE_ALBEDO2 */		GL_TEXTURE2,
		/* SAMPLERUSAGE_NORMAL0 */		GL_TEXTURE3,
		/* SAMPLERUSAGE_NORMAL1 */		GL_TEXTURE4,
		/* SAMPLERUSAGE_NORMAL2 */		GL_TEXTURE5,
		/* SAMPLERUSAGE_SPECULAR0 */	GL_TEXTURE6,
		/* SAMPLERUSAGE_SPECULAR1 */	GL_TEXTURE7,
		/* SAMPLERUSAGE_SPECULAR2 */	GL_TEXTURE8,
	};
	glActiveTexture(unitTexture[(int)kindSampler]);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glBindSampler((GLuint)kindSampler, idSampler);

	return(commandList);
}


/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                       [Exceptional-static] */
/* -------------------------------------------------------------------------- */
/* この関数群は、システム全体の初期化・終了に関わる処理のため、プラットフォー */
/* ムやフレームワーク毎で実装が変わるので、呼出側でexternしてください。       */
/* ※ヘッダに含むのが適切でないプロトタイプを持っています。                   */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! シェーダ管理のシステムを起動
/*!
@param	setting
	システム初期化設定

@retval	関数値
	true == 成功
	false == 失敗

シェーダ（コマンドリスト作成）を管理するための基底処理を
	初期化します。
※とは言え、設置しているのは頂点宣言関係だったりします
	（最近は頂点宣言関係はシェーダのバインドやアノテーション
	で宣言できるので、何かあった時にシェーダ内にあった方が
	良いだろうと思ったため、ここに設置してあります）。
*/
bool ShaderSystemBootUp(const SettingSystem& setting)
{
	/* 標準頂点宣言を作成 */
	if(false == DeclarationVertex::BuiltInStandard.BootUpStandard())	{	/* 失敗 */
		return(false);
	}

	/* 標準シェーダを読込 */
	void* handleDataVertex = nullptr;
	void* handleDataFragment = nullptr;
	void* handleHeapVertex = nullptr;
	void* handleHeapFragment = nullptr;
	handleDataVertex = SpriteStudio6::Platform::File::TextLoad(&handleHeapVertex, u8"shader/ss6player_standard.vs", SpriteStudio6::AlignmentMemory::FILE_BUFFER);
	handleDataFragment = SpriteStudio6::Platform::File::TextLoad(&handleHeapFragment, u8"shader/ss6player_standard.fs", SpriteStudio6::AlignmentMemory::FILE_BUFFER);
	if((nullptr == handleDataVertex) || (nullptr == handleDataFragment))	{
		return(false);
	}
	Shader::BuiltInStandard.BootUp(handleDataVertex, handleDataFragment, SpriteStudio6::DeclarationVertex::BuiltInStandard, 0, true);
	SpriteStudio6::Platform::File::DataRelease(handleHeapFragment);
//	SpriteStudio6::Platform::File::DataRelease(handleHeapVertex);

//	handleDataVertex = SpriteStudio6::Platform::File::TextLoad(&handleHeapVertex, u8"shader/ss6player_standard.vs", SpriteStudio6::AlignmentMemory::FILE_BUFFER);
	handleDataFragment = SpriteStudio6::Platform::File::TextLoad(&handleHeapFragment, u8"shader/ss6player_standard_mask.fs", SpriteStudio6::AlignmentMemory::FILE_BUFFER);
	if((nullptr == handleDataVertex) || (nullptr == handleDataFragment))	{
		return(false);
	}
	Shader::BuiltInStandardMask.BootUp(handleDataVertex, handleDataFragment, SpriteStudio6::DeclarationVertex::BuiltInStandard, 1, false);
	SpriteStudio6::Platform::File::DataRelease(handleHeapFragment);
	SpriteStudio6::Platform::File::DataRelease(handleHeapVertex);

	return(true);
}

/* ********************************************************* */
//! シェーダ管理のシステムを終了
/*!
@param	なし

@retval	なし

ShaderSystemBootUp関数で起動した基底処理を終了します。
*/

void ShaderSystemShutDown(void)
{
	/* 標準シェーダを解放 */
	Shader::BuiltInStandard.ShutDown(true);
	Shader::BuiltInStandardMask.ShutDown(true);

	/* 標準頂点宣言を解放 */
	DeclarationVertex::BuiltInStandard.ShutDown();
}

}	/* SpriteStudio6 */
