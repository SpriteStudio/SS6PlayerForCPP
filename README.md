## SpriteStudio 6 Player for CPP

ドキュメントはこちらです。  
https://github.com/SpriteStudio/SS6PlayerForCpp/wiki

#### ビルド

- とにかく先ずビルドしてみたい場合：
```
scripts/win_vs.batを実行する。

こちらで失敗したい場合、
scripts/win_vs_custom.batを開いて、上の SS6で始まる環境変数を自分の環境の合うように調整し、
win_vs_custom.batを実行する。
```

- Visual Studioと [C++ CMake tools for windows](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-160)をインストールされている場合：
```
a. Visual Studio内部でSS6PlayerForCPPフォルダーを開く。
b. CMakeSettings.jsonの中の設定を選択する。（例：OpenGL_Win64_Debug）
c. CMake Generationの完了を確認する。
(c) の段階で失敗した場合、(Custom_OpenGL)で始まる設定を指定し、 CMakeSettings.jsonのcmakeCommandArgsを自分の環境にあう様に修正。
d. SS6PlayerForCpp_ApplicationMain.exeのビルドを開始。
```

#### 実行

- 上のビルド過程の完了後、
```
bin-Windows-Debug/Debug
bin-Windows-Release/Release
```
- などのパスにて、`SS6PlayerForCpp_AppliationMain.exe`が作成される。

#### ご注意

Clone後にソースコードを修正された方へ

2021/05/28 ビルド環境の修正諸々がなされたコミット(2021/4/20 18:40 JST 67b142e)に、GitHub Actions対応を加えたバージョンで、一度リポジトリを再構築させていただきました。  
このため、既にCloneし、ローカルで修正されている場合は、大変恐れ入りますが、修正分を退避させた後に、新たにリポジトリをCloneいただいた上で差分を適用いただけますようよろしくお願いいたします。  
※修正ファイルはビルド関連のスクリプト、設定ファイルのみでプログラムコードの修正はありません。

お手数をおかけすることとなり大変申し訳ありませんが、ご対応のほどよろしくお願いいたします。
