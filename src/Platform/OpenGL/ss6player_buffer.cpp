/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_buffer.h"

#include "ss6player_memory.h"
#include "ss6player_system.h"

#include <vector>

/* コンパイルオプション: GPUBase::Chunkを遅延解放 */
/* MEMO: このオプションは、必ず定義しておくようにしてください。 */
/*       ※即時解放（未定義）の時の処理は一応サンプル＆備忘的な */
/*         側面で記載してはありますが……即時解放にするとGPUで  */
/*         使用中のバッファを解放してしまい、正常動作しません。 */
#define _COMPILEOPTION_DELAYDISPOSE_GPU_

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
/* gfxデバイス */
/* MEMO: renderer.cpp内に実体があります。 */
extern int CountSwapChain;
extern int IndexSwapChain;

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */
#define _UNUSED(_var_)	(void)_var_

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
}	/* SpriteStudio6 */

/* ========================================================================== */
/* MEMO: 以下、誤使用防止のため、深い階層に沈めておくものです。               */
/* ========================================================================== */
namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */
#define GFXFRAMEWORK_TYPEPOOL_INVALID	(-1)	/* ((nns::gfx::GraphicsFramework::MemoryPoolType)(-1)) */	/* 定義に存在しない値 */
#define MEMORYOFFSET_POOL_INVALID	(-1)																	/* GPUバッファのプール内ヒープ位置（負数はないはずなので） */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
/* GPUDeleterChunk関係 */
/* MEMO: 完全にプラットフォームに依存しきりの内容になるので、ヘッダに出さないために */
/*       cpp内スコープ内にしています。                                              */
class WorkArea_GPUDeleterChunk	{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	struct FragmentData	{
		/* MEMO: 基本的にGPUBase::Chunkと同じ内容です。     */
		/*       ※念のため現時点では別の構造体としている。 */
		GLenum TypeBuffer;													/* バッファのタイプ */
		GLuint IDBuffer;													/* バッファオブジェクトID */
		size_t SizeBuffer;													/* 実際に取得されているバッファ長 */

		inline void CleanUp(void)
		{
			TypeBuffer = 0;
			IDBuffer = 0;
			SizeBuffer = 0;
		}

		FragmentData(void)
		{
			CleanUp();
		}

		~FragmentData(void)
		{
		}
	};
	typedef std::vector<FragmentData> ListData;

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum Constant	{
		COUNT_LISTDATA = 2,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	bool FlagRequestExecute;

	/* MEMO: 厳密にはスワップチェイン分設置する必要があるが、実用上「描画終了」「描画定義直後」があれば */
	/*       良いので、2つしか定義リストを確保していません。                                            */
	void* HandleHeap[Constant::COUNT_LISTDATA];			/* リストのヒープハンドル */
	ListData* Data[Constant::COUNT_LISTDATA];			/* 削除情報のリスト */
	size_t CapactyData;									/* 最大配列長 */
	int IndexData;										/* 次のExecuteで削除されるバッファ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:

	/* ----------------------------------------------- Functions (static) */
private:
	inline void CleanUp(void)
	{
		FlagRequestExecute = false;

		for(int i=0; i<Constant::COUNT_LISTDATA; i++)	{
			HandleHeap[i] = nullptr;
			Data[i] = nullptr;
		}
		CapactyData = 0;
		IndexData = -1;
	}
protected:
public:
	WorkArea_GPUDeleterChunk(void)
	{
		CleanUp();
	}

	~WorkArea_GPUDeleterChunk(void)
	{
//		CleanUp();
	}

	/* ********************************************************* */
	//! 起動
	/*!
	@param	countDataMax
		1フレームで処理可能な最大チャンク数

	@retval	関数値
		true == 成功
		false == 失敗

	本クラスを起動します。
	*/
	inline bool BootUp(int countDataMax)
	{
		size_t sizeWorkArea = sizeof(FragmentData) * countDataMax;
		for(int i=0; i<Constant::COUNT_LISTDATA; i++)	{
			/* MEMO: 初期化時に最大配列長を確保しています。                     */
			/*       ※コンストラクタで配列長を与えた場合は要素が作成されるので */
			/*         クリアしなくてはいけないことに注意してください。         */
			void* handleHeap = Memory::Allocate(&HandleHeap[i], sizeWorkArea, AlignmentMemory::SAFETY);
			Data[i] = new(handleHeap) ListData(countDataMax);
			if(nullptr == Data[i])	{	/* 失敗 */
				ShutDown();
				return(false);
			}

			Data[i]->clear();
		}
		CapactyData = countDataMax;

		IndexData = 0;	/* 最初のバッファを定義 */

		FlagRequestExecute = false;

		return(true);
	}

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし
	*/
	inline void ShutDown(void)
	{
		for(int i=0; i<Constant::COUNT_LISTDATA; i++)	{
			Data[i]->clear();
			Data[i]->~vector();	/* 明示的にデストラクト（内部バッファの解放） */

			void* handleHeap = HandleHeap[i];
			if(nullptr != handleHeap)	{
				Memory::Free(handleHeap);
			}
		}

		CleanUp();
	}

	/* ********************************************************* */
	//! 解放されていない予約情報があるかを取得
	/*!
	@param	なし

	@retval	関数値
		true == 残っている
		false == 空（残っていない）
	*/
	inline bool IsBuffered(void)
	{
		/* MEMO: 全バッファが空でないとfalseを返さない。 */
		ListData* listData;
		for(int i=0; i<Constant::COUNT_LISTDATA; i++)	{
			listData = Data[i];
			if(nullptr != listData)	{
				if(0 < listData->size())	{
					return(true);
				}
			}
		}

		return(false);
	}

	/* ********************************************************* */
	//! 現在の実解放対象インデックスを取得
	/*!
	@param	なし

	@retval	関数値
		実解放用リストインデックス

	取得できる値は「次のExecuteで解放されるリストのインデックス」
		です。
	*/
	inline int IndexGetNow(void)
	{
		return(IndexData);
	}

	/* ********************************************************* */
	//! 現在の追加対象インデックスを取得
	/*!
	@param	なし

	@retval	関数値
		追加用リストインデックス

	取得できる値は「次のExecuteで解放されるリストの次のインデックス」
		です。
	*/
	inline int IndexGetNext(void)
	{
		return((IndexData + 1) % Constant::COUNT_LISTDATA);
	}

	/* ********************************************************* */
	//! リストインデックスをスワップ
	/*!
	@param	なし

	@retval	関数値
		true == 成功
		false == 失敗

	解放済リストをクリアして、リストのインデックスをスワップします。
	*/
	inline bool Swap(void)
	{
		/* 遅延解放済みリストのクリア */
		int index = IndexGetNow();
		if((0 > index) || (Constant::COUNT_LISTDATA <= index))	{
			return(false);
		}

		ListData* listData = Data[index];
		if(nullptr != listData)	{
			listData->clear();
		}

		/* バッファを更新 */
		IndexData = IndexGetNext();

		return(true);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};
static WorkArea_GPUDeleterChunk WorkAreaGPUDeleterChunk;				/* 遅延解放処理用ワーク */
static SpriteStudio6::Buffer::GPUDeleterChunk InstanceGPUDeleterChunk;	/* 遅延解放処理本体 */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
static bool BufferBindGPU(GLenum* type, GLuint* idBuffer, Buffer::KindUsageBufferGPU usage, size_t sizeBuffer, void* workArea)
{
	/* 出力初期化 */
	*idBuffer = 0;

	/* バッファ設定 */
	GLenum typeBuffer = 0;
	switch(usage)	{
		case Buffer::KindUsageBufferGPU::USAGE_CODESHADER:
			/* MEMO: 予備につき、現状実装する必要はありません。 */
			break;

		case Buffer::KindUsageBufferGPU::USAGE_CONSTANT:
			typeBuffer = GL_UNIFORM_BUFFER;
			break;

		case Buffer::KindUsageBufferGPU::USAGE_DATAVERTEX:
			typeBuffer = GL_ARRAY_BUFFER;
			break;

		case Buffer::KindUsageBufferGPU::USAGE_DATAINDEX:
			typeBuffer = GL_ELEMENT_ARRAY_BUFFER;
			break;

		case Buffer::KindUsageBufferGPU::USAGE_TEXTURE:
			/* MEMO: 予備につき、現状実装する必要はありません。 */
			break;

		default:
			/* エラー */
			return(false);
	}

	/* バッファを設置 */
	/* MEMO: 初期値は転送しません。 */
	if(0 != typeBuffer)	{
		glGenBuffers(1, idBuffer);
		if(0 == *idBuffer)	{	/* 失敗 */
			return(false);
		}

		glBindBuffer(typeBuffer, *idBuffer);
		glBufferData(typeBuffer, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(typeBuffer, 0);
	}

	/* バッファの種別返値を設定（バッファの再バインドに必要） */
	*type = typeBuffer;

	return(true);
}

static void BufferDischargeGPU(GLuint* idBuffer)
{
	if(0 != *idBuffer)	{
		glDeleteBuffers(1, idBuffer);
		*idBuffer = 0;
	}
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                     Buffer::GPUBase::Chunk */
/* -------------------------------------------------------------------------- */
namespace Buffer	{
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
GPUBase::Chunk::Chunk(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
GPUBase::Chunk::~Chunk(void)
{
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUBase::Chunk::CleanUp(void)
{
	TypeBuffer = 0;
	IDBuffer = 0;
	SizeBuffer = 0;
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUBase::Chunk::IsValid(void) const
{
	return(0 != IDBuffer);	/* ? true : false */
}

/* ********************************************************* */
//! ワークエリアのヒープサイズの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
size_t GPUBase::Chunk::SizeGetWorkArea(void)
{
	/* MEMO: 現状Vバッファオブジェクト自体を管理するための専用クラスなどはない（はずな）ので */
	/*       即時でも遅延でもワークを確保なくて良い。                                        */
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	return((size_t)0);
#else	/* MEMO: 即時解放 */
	return((size_t)0);
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
}

/* ********************************************************* */
//! バッファの起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUBase::Chunk::BootUp(KindUsageBufferGPU usage, size_t size, void* work)
{
	/* 初期化 */
	CleanUp();

	/* GPUバッファ設置 */
	if(false == BufferBindGPU(&TypeBuffer, &IDBuffer, usage, size, work))	{
		CleanUp();

		return(false);
	}

	SizeBuffer = size;

	return(true);
}

/* ********************************************************* */
//! チャンクの終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUBase::Chunk::ShutDown(void)
{
	if(true == IsValid())	{	/* チャンクが有効 */
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
		/* MEMO: それぞれの使用タイミングで遅延処理のタイミングが変わるので、ここでは */
		/*       なにもしない（親側で各チャンクの解放タイミングを決めるため）。       */
#else	/* MEMO: 即時解放 */
		BufferDischargeGPU(&IDBuffer, &Buffer, &HandleBuffer);
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

		CleanUp();
	}
}

/* ********************************************************* */
//! チャンクが管理しているバッファを仮想メモリにマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void* GPUBase::Chunk::Map(void)
{
	if(false == IsValid())	{
		return(nullptr);
	}

	glBindBuffer(TypeBuffer, IDBuffer);
	void* bufferMapped = glMapBuffer(TypeBuffer, GL_WRITE_ONLY);
	glBindBuffer(TypeBuffer, 0);

	return(bufferMapped);
}

/* ********************************************************* */
//! チャンクが管理しているバッファを仮想メモリからアンマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUBase::Chunk::Unmap(void)
{
	glBindBuffer(TypeBuffer, IDBuffer);
	glUnmapBuffer(TypeBuffer);
	glBindBuffer(TypeBuffer, 0);
}

/* ********************************************************* */
//! チャンクが管理しているバッファのGPU上アドレスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUBase::Chunk::AddressGetGPU(TypeAddressGPU* gpuAddress) const
{
	/* MEMO: OpenGL実装はGPUバッファのIDが入ったGLuintのポインタを返しておく。 */
	*gpuAddress = (void*)&IDBuffer;
}

/* ********************************************************* */
//! バッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUBase::Chunk::DataSet(const void* data, size_t size)
{
	if(false == IsValid())	{	/* 有効でない */
		return(false);
	}
	if(nullptr == data)	{	/* NULLアクセス */
		return(false);
	}
	if(SizeBuffer < size)	{	/* サイズオーバー */
		/* MEMO: オーバーランしないようクリップしておく。 */
		size = SizeBuffer;
	}

	/* データを転送 */
#if 1
	void* bufferMapped = Map();
	memcpy(bufferMapped, data, size);
	Unmap();
#else
	glBindBuffer(TypeBuffer, IDBuffer);
	glBufferSubData(TypeBuffer, 0, size, data);
#endif

	return(true);
}
}	/* Buffer */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            Buffer::GPUBase */
/* -------------------------------------------------------------------------- */
namespace Buffer	{
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// GPUBase::GPUBase(void)

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 仮想デストラクタです。
 */
// GPUBase::~GPUBase(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void GPUBase::CleanUp(void)

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// bool GPUBase::IsValid(void) const

/* ********************************************************* */
//! バッファの起動
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// bool GPUBase::BootUp(KindUsageBufferGPU usage, size_t size, void* work)

/* ********************************************************* */
//! バッファの終了
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// void GPUBase::ShutDown(void)

/* ********************************************************* */
//! バッファを仮想メモリにマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// void* GPUBase::Map(void)

/* ********************************************************* */
//! バッファを仮想メモリからアンマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// void GPUBase::Unmap(void)

/* ********************************************************* */
//! バッファのGPU上アドレスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// void GPUBase::AddressGetGPU(TypeAddressGPU* gpuAddress) const

/* ********************************************************* */
//! バッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// bool GPUBase::DataSet(const void* data, size_t size)

/* ********************************************************* */
//! 初期データをバッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 純粋仮想関数です。
 */
// void GPUBase::DataSetInitial(const void* data, size_t size)
}	/* Buffer */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                    Buffer::GPUDeleterChunk */
/* -------------------------------------------------------------------------- */
namespace Buffer	{
/* ********************************************************* */
//! 起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUDeleterChunk::BootUp(int countChunkMax)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	if(false == WorkAreaGPUDeleterChunk.BootUp(countChunkMax))	{	/* 失敗 */
		return(false);
	}
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

	return(true);
}

/* ********************************************************* */
//! 終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUDeleterChunk::ShutDown(void)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
//	if(WorkAreaGPUDeleterChunk.IsBuffered())	{	/* まだ解放できない */
//		return;
//	}

	WorkAreaGPUDeleterChunk.ShutDown();
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
}

/* ********************************************************* */
//! 解放チャンクがバッファリングされているかを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUDeleterChunk::IsBuffered(void)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	return(WorkAreaGPUDeleterChunk.IsBuffered());
#else	/* MEMO: 即時解放 */
	return(false);
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
}

/* ********************************************************* */
//! 解放処理実行
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUDeleterChunk::Execute(void)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	/* MEMO: 削除要請が出ていない（前フレームの描画が終了していない）場合は、 */
	/*       何もしない（バッファのスワップもしない）で帰る。                 */
	if(true == WorkAreaGPUDeleterChunk.FlagRequestExecute)	{	/* 削除要請が出ていない */
		/* 実行リクエストを消去 */
		WorkAreaGPUDeleterChunk.FlagRequestExecute = false;

		/* 全要素を解放 */
		/* MEMO: リストは逐次消さないこと。 */
		int index = WorkAreaGPUDeleterChunk.IndexGetNow();
		WorkArea_GPUDeleterChunk::ListData* listData = WorkAreaGPUDeleterChunk.Data[index];
		WorkArea_GPUDeleterChunk::FragmentData* fragment;
		int count = (int)listData->size();	/* MEMO: よもやintを超えないだろう */
		for(int i=0; i<count; i++)	{
			fragment = &listData->at(i);	/* 念のためコピーを取る */
			if(MEMORYOFFSET_POOL_INVALID != fragment->IDBuffer)	{	/* バッファが取得されている */
				/* バッファの解放 */
				BufferDischargeGPU(&fragment->IDBuffer);
//				fragment->CleanUp();
			}
		}

		/* バッファをスワップ */
		/* MEMO: Swapの中で解放済リストのクリアも行っています。 */
		WorkAreaGPUDeleterChunk.Swap();
	}
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

	return(true);
}

/* ********************************************************* */
//! 解放処理実行要求
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUDeleterChunk::ExecuteRequest(void)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	WorkAreaGPUDeleterChunk.FlagRequestExecute |= true;
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
}

/* ********************************************************* */
//! チャンクを削除定義
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUDeleterChunk::ChunkSet(GPUBase::Chunk& chunk, bool flagSideUpdate)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	if(false == chunk.IsValid())	{	/* チャンクが無効 */
		/* MEMO: 定義しないで帰る */
		return(true);
	}

	/* MEMO: 現在、更新対象のバッファは解放タイミングを1フレーム（厳密には2フレーム） */
	/*       後に解放をずらさなくてはならないことに注意してください。                 */
	/*       ※現在更新中: 設定（→GPU描画設定→次回GPU描画終了）→解放               */
	/*         前回更新: （裏で描画使用中→今回GPU描画終了）→解放                    */
	int index = (true == flagSideUpdate) ? WorkAreaGPUDeleterChunk.IndexGetNext() : WorkAreaGPUDeleterChunk.IndexGetNow();
	WorkArea_GPUDeleterChunk::ListData* listData = WorkAreaGPUDeleterChunk.Data[index];
	if(nullptr == listData)	{
		return(false);
	}

	if(WorkAreaGPUDeleterChunk.CapactyData <= listData->size())	{	/* 配列長制限 */
		return(false);
	}

	/* 値を追加 */
	WorkArea_GPUDeleterChunk::FragmentData fragment;
	fragment.TypeBuffer = chunk.TypeBuffer;
	fragment.IDBuffer = chunk.IDBuffer;
	fragment.SizeBuffer = chunk.SizeBuffer;

	listData->push_back(fragment);
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

	return(true);
}

}	/* Buffer */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          Buffer::GPUSingle */
/* -------------------------------------------------------------------------- */
namespace Buffer	{
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
GPUSingle::GPUSingle(void) :
	GPUBase()
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
GPUSingle::~GPUSingle(void)
{
	/* MEMO: 念のためシャットダウンしておく。 */
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void GPUSingle::CleanUp(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void GPUSingle::CleanUp(void)

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUSingle::IsValid(void) const
{
	/* MEMO: 現在簡易的な判定で済ましています。 */
	return(KindUsageBufferGPU::USAGE_NONE < Usage);	/* ? true : false */
}

/* ********************************************************* */
//! ワークエリアのヒープサイズの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
size_t GPUSingle::SizeGetWorkArea(size_t size)
{
	size_t sizeWorkChunk = Memory::Alignment(Buffer::GPUBase::Chunk::SizeGetWorkArea(), AlignmentMemory::SAFETY);	/* Chunkが必要とするワークサイズ */
#if	defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	/* MEMO: 遅延解放の場合、ワークの中にまとめてバッファを取ると解放時に別々に解放できないため、 */
	/*       ワークとしては計上しない。                                                           */
	return(sizeWorkChunk);
#else	/* MEMO: 即時解放 */
	size_t sizeWorkBuffer = Memory::Alignment(size, AlignmentMemory::SAFETY);	/* 転送用バッファ */
	return(sizeWorkChunk + sizeWorkBuffer);
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
}

/* ********************************************************* */
//! バッファの起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUSingle::BootUp(KindUsageBufferGPU usage, size_t size, void* work)
{
	/* MEMO: 二重起動は認めない。 */
	if(true == IsValid())	{	/* 起動済 */
		return(false);
	}
	CleanUp();

	/* バッファ設定の確定 */
	Usage = usage;
	Size = size;

	/* チャンクの起動 */
#if	0	//defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	/* MEMO: チャンクのメモリ確保は、遅延解放の場合はワークの有無に限らず外部から取得。 */
	if(false == Chunk.BootUp(usage, size, nullptr))	{
		CleanUp();
		return(false);
	}

	/* MEMO: 本クラスそのものは何もワークを取得していない（チャンクのバッファは別計上）。 */
	Work = nullptr;
#else	/* MEMO: 即時解放 */
	/* MEMO: 本クラスそのものが独自で取得したワークのみ設定（チャンクのバッファは別計上）。 */
	if(nullptr == work)	{	/* 自動取得 */
		size_t sizeHeap = SizeGetWorkArea(size);
		if(0 < size)	{
			void* handleWork = Memory::Allocate(&Work, sizeHeap);
			if(nullptr == Work)	{	/* ヒープ取得失敗 */
				CleanUp();

				return(false);
			}

			/* MEMO: 遅延解放の場合、転送用バッファは外部から取得。 */
			if(false == Chunk.BootUp(usage, size, handleWork))	{
				CleanUp();
				return(false);
			}
		}
	} else {	/* 手動設定 */
		/* MEMO: 即時解放の場合、転送用バッファはワーク内に確保。 */
		Work = nullptr;
		if(false == Chunk.BootUp(usage, size, work))	{
			CleanUp();
			return(false);
		}
	}
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

	return(true);
}
/* ********************************************************* */
//! バッファの終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUSingle::ShutDown(void)
{
	if(true == IsValid())	{	/* 起動済 */
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
		/* MEMO: GPUSingleの場合、バッファは初回のみ更新しませんが、常に描画に使用しているので */
		/*      「次」のタイミングで解放します（常に更新している扱い）。                       */
		InstanceGPUDeleterChunk.ChunkSet(Chunk, true);

		/* チャンクを終了 */
		Chunk.ShutDown();
#else	/* MEMO: 即時解放 */
		/* チャンクを終了 */
		Chunk.ShutDown();
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

		/* 自動取得ワークを解放 */
		if(nullptr != Work)	{
			/* MEMO: Workのクリアは直後のCleanUpでやっているので略。 */
			Memory::Free(Work);
		}

		CleanUp();
	}
}

/* ********************************************************* */
//! バッファを仮想メモリにマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * Switchではこの関数を使用する少し前からUnmapまでの間でASSERTを使わないでください。
 * バッファのMap/Unmapと相性が悪いようで、Unmapで例外を吐いてコケます。
 */
void* GPUSingle::Map(int index)
{
	_UNUSED(index);

	if(false == IsValid())	{	/* 起動していない */
		return(nullptr);
	}

	return(Chunk.Map());
}

/* ********************************************************* */
//! バッファを仮想メモリからアンマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUSingle::Unmap(int index)
{
	_UNUSED(index);

	if(true == IsValid())	{	/* 起動済 */
		Chunk.Unmap();
	}
}

/* ********************************************************* */
//! バッファのGPU上アドレスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUSingle::AddressGetGPU(TypeAddressGPU* gpuAddress) const
{
	if(nullptr == gpuAddress)	{	/* 返し先が異常 */
		return;
	}

	Chunk.AddressGetGPU(gpuAddress);
}

/* ********************************************************* */
//! バッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUSingle::DataSet(const void* data, size_t size)
{
	Chunk.DataSet(data, size);

	return(true);
}

/* ********************************************************* */
//! 初期データをバッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUSingle::DataSetInitial(const void* data, size_t size)
{	/* 初期化値設定専用関数 */
	Chunk.DataSet(data, size);
}
}	/* Buffer */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Buffer::GPUMulti */
/* -------------------------------------------------------------------------- */
namespace Buffer	{
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
GPUMulti::GPUMulti(void) :
	GPUBase()
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
GPUMulti::~GPUMulti(void)
{
	/* MEMO: 念のためシャットダウンしておく。 */
	ShutDown();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void GPUMulti::CleanUp(void)

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUMulti::IsValid(void) const
{
	/* MEMO: 現在簡易的な判定で済ましています。 */
	return(KindUsageBufferGPU::USAGE_NONE < Usage);	/* ? true : false */
}

/* ********************************************************* */
//! ワークエリアのヒープサイズの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
size_t GPUMulti::SizeGetWorkArea(size_t size)
{
	/* MEMO: Chunk::SizeGetWorkAreaが0を返しても（特別に判定しなくても）Memory::Alignmentでも0が返るので大丈夫。 */
	/* MEMO: 0クリアに（outでしか使用していないので）意味はないのと、値の受領以外の使用していません。 */
	size_t sizeInstanceChunk;
	size_t sizeWorkChunk;
	size_t sizeWorkChunkUnit;
	return(SizeGetWorkArea(&sizeInstanceChunk, &sizeWorkChunk, &sizeWorkChunkUnit, size));
}

/* ********************************************************* */
//! ワークエリアのヒープサイズの取得（内部処理）
/* --------------------------------------------------------- */
/* MEMO:
 * ヒープ内には下記が含まれています。
 * - チャンク管理クラスの実体（配列）
 * - チャンクワーク
 * 
 * ※チャンクの中にはバッファの実メモリ部は入りません（OpenGL版では
 *   VRAMにバッファが取得された後で、Map関数によってメインメモリに
 *   マッピングされます）。
 */
size_t GPUMulti::SizeGetWorkArea(size_t* sizeInstanceChunk, size_t* sizeWorkChunk, size_t* sizeWorkChunkUnit, size_t sizeBuffer)
{
	const int countBuffer = CountSwapChain;

	*sizeInstanceChunk = Memory::Alignment(sizeof(Buffer::GPUBase::Chunk) * countBuffer, AlignmentMemory::SAFETY);
	*sizeWorkChunkUnit = Memory::Alignment(Buffer::GPUBase::Chunk::SizeGetWorkArea(), AlignmentMemory::SAFETY);	/* Chunkが必要とするワークサイズ */

	*sizeWorkChunk = Memory::Alignment((*sizeWorkChunkUnit) * countBuffer, AlignmentMemory::SAFETY);

	return((*sizeInstanceChunk) + (*sizeWorkChunk));
}

/* ********************************************************* */
//! バッファの起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUMulti::BootUp(KindUsageBufferGPU usage, size_t size, void* work)
{
	/* MEMO: 二重起動は認めない。 */
	if(true == IsValid())	{	/* 起動済 */
		return(false);
	}
	CleanUp();

	/* バッファ設定の確定 */
	Usage = usage;
	Size = size;

	/* チャンクのワークエリアを作成 */
	const int countBuffer = CountSwapChain;
	size_t sizeInstanceChunk;
	size_t sizeWorkChunk;
	size_t sizeWorkChunkUnit;
	size_t sizeWorkArea = SizeGetWorkArea(&sizeInstanceChunk, &sizeWorkChunk, &sizeWorkChunkUnit, size);
	void* pointerWork = nullptr;
	
	if(nullptr == work)	{	/* 自動取得 */
		/* メモリを取得 */
		if(0 < sizeWorkArea)	{
			pointerWork = SpriteStudio6::Memory::Allocate(&Work, sizeWorkArea);
			if(nullptr == Work)	{	/* エラー */
				CleanUp();

				return(false);
			}
		} else {	/* ワークエリアは必要なし（本来はあり得ない。） */
			/* MEMO: チャンクの実体が必ずワーク中に取られるために、0であることはあり得ない。 */
#if 0	/* MEMO: 本来処理 */
			Work = nullptr;
//			handleWork = nullptr;
#else	/* MEMO: 実質的な処理 */
			CleanUp();

			return(false);
#endif
		}
	} else {	/* 手動取得 */
		Work = nullptr;	/* 外部取得（本クラスでは解放しない） */
		pointerWork = work;
	}

	/* チャンクの先頭を設定 */
	uint8_t* pointerWorkTop = reinterpret_cast<uint8_t*>(pointerWork);
	Chunk = reinterpret_cast<GPUBase::Chunk*>(pointerWorkTop);
	pointerWorkTop += sizeInstanceChunk;

	bool flagBootUpChunk = false;
	for(int i=0; i<countBuffer; i++)	{
		new(&Chunk[i]) GPUBase::Chunk;
		if(0 < sizeWorkChunk)	{
			flagBootUpChunk = Chunk[i].BootUp(usage, size, nullptr);
		} else {
			flagBootUpChunk = Chunk[i].BootUp(usage, size, pointerWorkTop);
			pointerWorkTop += sizeWorkChunk;
		}

		if(false == flagBootUpChunk)	{	/* チャンク起動失敗 */
			ShutDown();

			return(false);
		}
	}

	return(true);
}

/* ********************************************************* */
//! バッファの終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUMulti::ShutDown(void)
{
	const int countBuffer = CountSwapChain;
	if(true == IsValid())	{	/* 有効 */
		/* チャンクを終了 */
		if(nullptr != Chunk)	{	/* チャンクの実体が存在 */
			for(int i=0; i<countBuffer; i++)	{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
				/* 遅延解放処理にチャンクを定義 */
				/* MEMO: GPUMultiの場合、下記のタイミングで遅延解放に定義する。 */
				/*       - 今回更新用のバッファは描画に使用しているので「次」   */
				/*       - 前回更新用のバッファは今描画しているので「今回」     */
				InstanceGPUDeleterChunk.ChunkSet(	Chunk[i],
													(IndexSwapChain == i) /* ? true : false */
											);
#else	/* MEMO: 即時解放 */
				/* MEMO: 即時解放の場合、何もしない（即時解放コンパイルの場合、Chunk::ShutDownの中で解放処理される）。 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
				/* チャンクを終了 */
				Chunk[i].ShutDown();
			}
		}

		/* 自動取得ワークを解放 */
		if(nullptr != Work)	{	/* ワークが自動取得されている */
			/* MEMO: Workのクリアは直後のCleanUpでやっているので略。 */
			Memory::Free(Work);
		}

		CleanUp();
	}
}

/* ********************************************************* */
//! バッファを仮想メモリにマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void* GPUMulti::Map(int index)
{	/* MEMO: 返値はGPU上のバッファをCPUメモリ空間にマッピングした仮想メモリです。 */
	if(false == IsValid())	{	/* 起動していない */
		return(nullptr);
	}
	if(CountSwapChain <= index)	{	/* インデックスの範囲が異常 */
		return(nullptr);
	}

	if(0 > index)	{	/* 現在のCPU操作対象 */
		index = IndexSwapChain;
	}

	return(Chunk[index].Map());
}

/* ********************************************************* */
//! バッファを仮想メモリからアンマッピング
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUMulti::Unmap(int index)
{
	if(false == IsValid())	{	/* 起動していない */
		return;
	}
	if(CountSwapChain <= index)	{	/* インデックスの範囲が異常 */
		return;
	}

	if(0 > index)	{	/* 現在のCPU操作対象 */
		index = IndexSwapChain;
	}

	Chunk[index].Unmap();
}

/* ********************************************************* */
//! バッファのGPU上アドレスを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUMulti::AddressGetGPU(TypeAddressGPU* gpuAddress) const
{
	if(nullptr == gpuAddress)	{	/* 返し先が異常 */
		return;
	}
	if(false == IsValid())	{	/* 起動していない */
		*gpuAddress = nullptr;
		return;
	}

	const int index = IndexSwapChain;
	Chunk[index].AddressGetGPU(gpuAddress);
}

/* ********************************************************* */
//! バッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool GPUMulti::DataSet(const void* data, size_t size)
{
	if(false == IsValid())	{	/* 起動していない */
		return(false);
	}

	const int index = IndexSwapChain;
	return(Chunk[index].DataSet(data, size));
}

/* ********************************************************* */
//! 初期データをバッファにデータを転送
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void GPUMulti::DataSetInitial(const void* data, size_t size)
{
	if(false == IsValid())	{	/* 起動していない */
		return;
	}

	/* MEMO: 全バッファに同じ値を転送しています。 */
	for(int i=0; i<CountSwapChain; i++)
	{
		Chunk[i].DataSet(data, size);
	}
}
}	/* Buffer */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                       [Exceptional-Static] */
/* -------------------------------------------------------------------------- */
/* この関数群は、システム全体の初期化・終了に関わる処理のため、プラットフォー */
/* ムやフレームワーク毎で実装が変わるので、呼出側でexternしてください。       */
/* ※ヘッダに含むのが適切でないプロトタイプを持っています。                   */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! バッファ管理のシステムを起動
/*!
@param	setting
	システム初期化設定

@retval	関数値
	true == 成功
	false == 失敗

バッファを管理するための基底処理を初期化します。
基本的に、遅延解放管理の処理を初期化しています。
*/
bool BufferSystemBootUp(const SettingSystem& setting)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	/* 遅延解放処理を初期化 */
	if(false == Buffer::GPUDeleterChunk::BootUp(setting.RendererLimitBufferChunkDelayDelete))	{
		return(false);
	}
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */

	return(true);
}

/* ********************************************************* */
//! バッファ管理のシステムを終了
/*!
@param	なし

@retval	なし

バッファを管理するための基底処理を終了します。
*/
void BufferSystemShutDown(void)
{
#if defined(_COMPILEOPTION_DELAYDISPOSE_GPU_)	/* MEMO: 遅延解放 */
	/* 遅延解放処理を終了 */
	Buffer::GPUDeleterChunk::ShutDown();
#else	/* MEMO: 即時解放 */
#endif	/* defined(_COMPILEOPTION_DELAYDISPOSE_GPU_) */
}

}	/* SpriteStudio6 */
