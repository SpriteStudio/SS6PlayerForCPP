/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_file.h"

#include "ss6player_memory.h"
#include "ss6player_system.h"

#if defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS)
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#else
#endif	/* defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS) */
#include <stdio.h>
#include <string>
#include <algorithm>

namespace SpriteStudio6	{
namespace Platform	{
namespace File	{
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
static std::string PathBase = "";

/* 定型文字関係 */
static const char DelimiterDirectory = '/';
static const char DelimiterDrive = ':';
static const std::string TextDirectoryUpper(u8"../");
static const std::string TextDirectorySame = u8"./";

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
static bool TextStartWith(const std::string& textSource, const std::string& textCompare);
static bool PathMoveUpper(std::string& pathOutput, std::string& path);
static bool PathNormalize(std::string& path, bool flagIsDirectory);
static bool PathGetAbsolute(std::string& pathOutput, const char* path, bool flagIsDirectory);
static bool PathConvertFileSystem(std::string& pathOutput, const std::string& path);

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! データファイルのロード
/* --------------------------------------------------------- */
/* MEMO:
 * 本当はエラー時の返りをgotoで処理してしまえばもっとすっきり
 * するのですが……clangで通らないため（変数の初期化順の問題
 * ではないよう……）。
 */
void* DataLoad(void** handleHeap, const char* path, size_t alignment)
{
	/* 出力をクリア */
	*handleHeap = nullptr;	/* エラー時のためにクリアしておく */

	std::string pathFile;
	FILE* fp = nullptr;
	std::string pathFileSystem;
	long sizeFile = 0;
	void* handleData = nullptr;

	/* 絶対パスのチェック */
	/* MEMO: 相対パスならPathBaseからの相対になります。 */
	if(false == PathGetAbsolute(pathFile, path, false))	{
		return(nullptr);
	}

	/* 出力をクリア */
	*handleHeap = nullptr;	/* エラー時のためにクリアしておく */

	/* ファイルをオープン */
	if(false == PathConvertFileSystem(pathFileSystem, pathFile))	{	/* 失敗 */
		goto DataLoad_ErrorEnd;
	}

	fp = fopen(pathFileSystem.c_str(), "rb");
	if(nullptr == fp)	{	/* ファイルがない */
		goto DataLoad_ErrorEnd;
	}

	/* ファイルサイズの取得 */
	if(0 != fseek(fp, 0, SEEK_END))	{	/* 失敗 */
		goto DataLoad_ErrorEnd;
	}

	sizeFile = ftell(fp);
	if(0 > sizeFile)	{	/* 失敗 */
		goto DataLoad_ErrorEnd;
	}
	if(0 != fseek(fp, 0, SEEK_SET))	{	/* 失敗 */
		fclose(fp);

		return(nullptr);
	}

	/* ファイルを格納するメモリを取得 */
	handleData = Memory::Allocate(handleHeap, sizeFile, alignment);	/* メモリをヒープ */
	if(nullptr == handleData)	{	/* メモリが取得できなかった */
		goto DataLoad_ErrorEnd;
	}

	/* ファイルを読込 */
	fread(handleData, (size_t)sizeFile, 1, fp);

	/* ファイルをクローズ */
	fclose(fp);
	fp = nullptr;

	return(handleData);

DataLoad_ErrorEnd:;
	if(nullptr != fp)	{
		fclose(fp);
	}
	fp = nullptr;

	return(nullptr);
}

/* ********************************************************* */
//! データファイルの解放
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void DataRelease(void* handleHeap)
{
	if(nullptr != handleHeap)	{
		Memory::Free(handleHeap);
	}
}

/* ********************************************************* */
//! テキストファイルのロード（OpenGL実装のみ）
/* --------------------------------------------------------- */
/* MEMO:
 */
void* TextLoad(void** handleHeap, const char* path, size_t alignment)
{
	/* 出力をクリア */
	*handleHeap = nullptr;	/* エラー時のためにクリアしておく */

	std::string pathFile;
	FILE* fp = nullptr;
	std::string pathFileSystem;
	long sizeFile = 0;
	void* handleData = nullptr;
	char* textData = nullptr;

	/* 絶対パスのチェック */
	/* MEMO: 相対パスならPathBaseからの相対になります。 */
	if(false == PathGetAbsolute(pathFile, path, false))	{
		return(nullptr);
	}

	/* 出力をクリア */
	*handleHeap = nullptr;	/* エラー時のためにクリアしておく */

	/* ファイルをオープン */
	if(false == PathConvertFileSystem(pathFileSystem, pathFile))	{	/* 失敗 */
		goto TextLoad_ErrorEnd;
	}

	fp = fopen(pathFileSystem.c_str(), "rb");
	if(nullptr == fp)	{	/* ファイルがない */
		goto TextLoad_ErrorEnd;
	}

	/* ファイルサイズの取得 */
	if(0 != fseek(fp, 0, SEEK_END))	{	/* 失敗 */
		goto TextLoad_ErrorEnd;
	}

	sizeFile = ftell(fp);
	if(0 > sizeFile)	{	/* 失敗 */
		goto TextLoad_ErrorEnd;
	}
	if(0 != fseek(fp, 0, SEEK_SET))	{	/* 失敗 */
		fclose(fp);

		return(nullptr);
	}

	/* ファイルを格納するメモリを取得 */
	/* MEMO: ファイルサイズの+1は終端文字のため */
	handleData = Memory::Allocate(handleHeap, (sizeFile + 1), alignment);	/* メモリをヒープ */
	if(nullptr == handleData)	{	/* メモリが取得できなかった */
		goto TextLoad_ErrorEnd;
	}

	/* 終端文字を書込 */
	textData = reinterpret_cast<char*>(handleData);
	textData[sizeFile] = 0x00;

	/* ファイルを読込 */
	fread(handleData, (size_t)sizeFile, 1, fp);

	/* ファイルをクローズ */
	fclose(fp);
	fp = nullptr;

	return(handleData);

TextLoad_ErrorEnd:;
	if(nullptr != fp)	{
		fclose(fp);
	}
	fp = nullptr;

	return(nullptr);
}

/* -------------------------------------------------------------------------- */
/*                                                         Functions (Static) */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! 文字列先頭の照合
/*!
@param	textSource
	被検索文字列
@param	textCompare
	検索文字列

@retval	関数値
	true == 適合
	false == 被適合

textSourceの先頭が
*/
static bool TextStartWith(const std::string& textSource, const std::string& textCompare)
{
	size_t lengthCompare = textCompare.size();

	for(size_t i=0; i<lengthCompare; i++)	{
		if(textSource[i] != textCompare[i])	{
			return(false);
		}
	}

	return(true);
}

/* ********************************************************* */
//! パスの正規化
/*!
@param	path
	ファイル・ディレクトリのパス
@param	flagIsDirectory
	true == ディレクトリパスとして正規化（末尾に「/」が無ければ付ける）
	false == ファイルパスとして正規化（末尾はいじらない）

@retval	関数値
	true == 成功
	false == 失敗
@retval	path
	正規化されたパス
*/
static bool PathNormalize(std::string& path, bool flagIsDirectory)
{
	if(0 == path.size())	{	/* 空文字列 */
		return(false);
	}

	/* ディレクトリ区切子の正規化 */
	std::replace(path.begin(), path.end(), '\\', DelimiterDirectory);	/* Windowsのディレクトリ区切子を変換 */

	/* ディレクトリの正規化 */
	if(true == flagIsDirectory)	{	/* ディレクトリの正規化 */
		size_t length = path.size();
		if(DelimiterDirectory != path[length - 1])	{	/* 末尾が「/」ではない */
			/* MEMO: 末尾に「/」を付ける */
			path += DelimiterDirectory;
		}
	}

	return(true);
}

/* ********************************************************* */
//! パスを一段階上げる
/*!
@param	pathOutput
	1階層上げた後のパスの出力先
@param	path
	ファイル・ディレクトリのパス

@retval	関数値
	true == 成功
	false == 失敗（不正文字列ないしは上げられない）
@retval	pathOutput
	1階層上げた後のパス
*/
static bool PathMoveUpper(std::string& pathOutput, std::string& path)
{
	/* 末尾が「/」かのチェック */
	size_t positionPath = path.size();
	if(0 == positionPath)	{	/* 空文字列 */
		return(false);
	}

	pathOutput = path;
	bool flagAddDirectory = false;
	positionPath--;
	if(DelimiterDirectory != path[positionPath])	{	/* 末尾が「/」 */
		/* 末尾を消去 */
		pathOutput.erase(positionPath, 1);
		flagAddDirectory = true;	/* 後で「/」を復帰 */
	}

	/* 1階層上に移動（ディレクトリを削除） */
	positionPath = pathOutput.find_last_of(DelimiterDirectory);
	if(std::string::npos == positionPath)	{	/* 現在最上階層 */
		pathOutput.clear();

		return(false);
	}
	pathOutput.erase(positionPath);

	/* 末尾を復帰 */
	if(true == flagAddDirectory)	{	/* 末尾が「/」だった */
		pathOutput += DelimiterDirectory;
	}

	return(true);
}

/* ********************************************************* */
//! 絶対パスを取得
/*!
@param	pathOutput
	絶対パスの出力先
@param	path
	ファイル・ディレクトリのパス（終端が0x00ターミネートされている必要あり）
@param	flagIsDirectory
	true == ディレクトリパスとして正規化（末尾に「/」が無ければ付ける）
	false == ファイルパスとして正規化（末尾はいじらない）

@retval	関数値
	ファイルの絶対パス
@retval	pathOutput
	絶対パス

pathの文字列が下記の場合、絶対パスと判定します。

※C++17に対応していないプラットフォームも考慮して、古めの実装
  にしています。
*/
static bool PathGetAbsolute(std::string& pathOutput, const char* path, bool flagIsDirectory)
{
	pathOutput = u8"";

	/* エラーチェック */
	if(nullptr == path)	{	/* 文字列が不正 */
		return(false);
	}
	if(0x00 == path[0])	{	/* 空文字列 */
		return(false);
	}

	std::string pathRelative(path);

	/* ファイル名の正規化 */
	if(false == PathNormalize(pathRelative, flagIsDirectory))	{
		return(false);
	}

	/* 絶対パスの検査 */
	bool flagAbsolute = false;
	if(DelimiterDirectory == pathRelative[0])	{	/* 最初の文字が「/」 */
		flagAbsolute = true;
	} else if(DelimiterDrive == pathRelative[1])	{	/* 2番目の文字が「:」 */
		flagAbsolute = true;
	}

	if(true == flagAbsolute)	{	/* 絶対パス */
		/* MEMO: 絶対パスの場合は何の加工もせずに正規化だけして返す。 */
		pathOutput = pathRelative;
	} else {	/* 相対パス */
		/* MEMO: 相対パスの場合は相対パスを解決しておく。 */
		/* 階層上昇の解決 */
		std::string pathBase(PathBase);
		std::string pathBaseTemp;
		for( ; ; )	{
			if(false == TextStartWith(pathRelative, TextDirectoryUpper))	{
				break;	/* Inf.-Loop */
			}

			/* 相対側の先頭から「../」を削除・基準パスを1つ上げる */
			pathRelative.erase(0, TextDirectoryUpper.size());
			PathMoveUpper(pathBaseTemp, pathBase);
			pathBase = pathBaseTemp;
		}

		/* 同階層指定を削除 */
		for( ; ; )	{
			if(false == TextStartWith(pathRelative, TextDirectorySame))	{
				break;	/* Inf.-Loop */
			}

			/* 相対側の先頭から「./」を削除 */
			pathRelative.erase(0, TextDirectorySame.size());
		}

		/* パスを合成 */
		pathOutput = pathBase + pathRelative;
	}

	return(true);
}

/* ********************************************************* */
//! パス文字列をOS依存の文字列エンコードに変更
/*!
@param	pathOutput
	変更後の文字列の出力先
@param	path
	ファイル・ディレクトリのパス
@param	flagIsDirectory
	true == ディレクトリパスとして正規化（末尾に「/」が無ければ付ける）
	false == ファイルパスとして正規化（末尾はいじらない）

@retval	関数値
	ファイルの絶対パス
@retval	pathOutput
	変更後の文字列

pathの文字列が下記の場合、絶対パスと判定します。

※C++17に対応していないプラットフォームも考慮して、古めの実装
  にしています。
*/
static bool PathConvertFileSystem(std::string& pathOutput, const std::string& path)
{
#if defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS)
	/* MEMO: Windowsはパスの文字列エンコードはSJIS。 */

	/* UTF-8 → Unicode */
	size_t lengthUTF8 = path.size() + 1;	/* バッファサイズに使用するので+1 */
	size_t lengthUnicode = (size_t)(MultiByteToWideChar(CP_UTF8, 0, path.c_str(), (int)lengthUTF8, NULL, 0));
	wchar_t* bufferUnicode = new wchar_t[lengthUnicode];
	MultiByteToWideChar(CP_UTF8, 0, path.c_str(), (int)lengthUTF8, bufferUnicode, (int)lengthUnicode);

	/* Unicode → SJIS */
	size_t lengthSJIS = (size_t)(WideCharToMultiByte(CP_THREAD_ACP, 0, bufferUnicode, -1, NULL, 0, NULL, NULL));
	char* bufferSJIS = new char[lengthSJIS];
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufferUnicode, (int)(lengthUnicode + 1), bufferSJIS, (int)lengthSJIS, NULL, NULL);

	/* 結果文字列を格納 */
	std::string pathTemp(bufferSJIS);

	/* バッファを消去 */
	delete[] bufferSJIS;
	delete[] bufferUnicode;

	/* 結果を最終格納 */
	pathOutput = pathTemp;
#else
	/* MEMO: Macはパスの文字列エンコードはUTF-8。 */
	pathOutput = path;
#endif /* defined(SS6PLAYER_COMPILEOPTION_PLATFORM_WINDOWS) */

	return(true);
}
}	/* File */
}	/* Platform */
/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                       [Exceptional-static] */
/* -------------------------------------------------------------------------- */
/* この関数群は、システム全体の初期化・終了に関わる処理のため、プラットフォー */
/* ムやフレームワーク毎で実装が変わるので、呼出側でexternしてください。       */
/* ※ヘッダに含むのが適切でないプロトタイプを持っています。                   */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! ファイルシステムを起動
/*!
@param	setting
	システム初期化設定

@retval	関数値
	true == 成功
	false == 失敗

ファイルシステムの基底処理を初期化します。

ヒープを使用しているため、先にメモリシステムの初期化を行って
	おいてください。
*/
bool FileSystemBootUp(const SettingSystem& setting)
{
	/* 基準パスの設定 */
	Platform::File::PathBase = setting.FilePathBase;

	if(0 == Platform::File::PathBase.size())	{	/* 空文字列 */
		return(false);
	}
	if(false == Platform::File::PathNormalize(Platform::File::PathBase, true))	{	/* 失敗 */
		return(false);
	}

	return(true);
}

/* ********************************************************* */
//! ファイルシステムを終了
/*!
@param	なし

@retval	なし

ファイルシステムを終了します。
*/
void FileSystemShutDown(void)
{
	/* 基準パスのクリア */
	Platform::File::PathBase.clear();
}
}	/* SpriteStudio6 */
