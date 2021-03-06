﻿===================
GameEngine KVARCO
===================

KVARCOとは
----------

KVARCO(クヴァルコ)はスクリプト言語Xtalを組み込んだ汎用ゲームエンジンです。
現在2D専用のWindows向けゲームエンジンとなっていますが、
将来的に3D対応、クロスプラットフォーム対応を目指していきます。

必要なもの
----------

KVARCO開発に必要なライブラリは以下の通り:

- Xtal
- DXライブラリ
- Boost
- OggVorbis
- OpenAL

現在実装済みの関数などをまとめたドキュメントがないため、使用したい関数は自分で見つけてください。すいません。

ビルドしたKVARCOの起動には次のものが必要です:

- "setting.ini"	
- メインのXtalスクリプトファイル
- 読み込むべきスクリプトファイルを列挙したXtalスクリプトファイル (以下、「スクリプトファイルリスト」)


setting.iniについて
^^^^^^^^^^^^^^^^^^^
"setting.ini"にはKVARCOの環境設定を書いておきます。

**サンプル**
::
    [WindowSetting]
    Title			=GameName	;ウィンドウのタイトルになります
    UseIcon			=1			;使うアイコンの番号。resouce.rcでのアイコン画像の番号です
    WndWidth		=640		;ウィンドウの幅(px単位)
    WndHeight		=480		;ウィンドウの高さ(px単位)

    ;フルスクリーンモード	で起動する場合は1
    ;ウィンドウモード		で起動する場合は0
    ;を設定してください
    FullScreen		=0

    [SystemSetting]
    AlwaysRun		=1			;ウィンドウが非アクティブの時も実行し続けるか

    ;可変フレームレートで起動する場合は1
    ;固定フレームレートで起動する場合は0
    ;を設定してください
    RockFPS			=0

    ;1を設定すると30fps以下になったとき、プログラムの実行優先度を上げます
    ;ただしこれは悪魔の契約です。PC全体が不安定になる可能性があります
    ;基本的に使用しないでください
    UpPG_Priority	=0

    ;スクリプトファイルリストの実行ファイルがあるフォルダからの相対パス
    [ScriptSetting]
    LoadFileList	=Script\LoadFileList.xtal

スクリプトファイルリストについて
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

スクリプトファイルリストの内容は単純に、
読み込むべきスクリプトファイルの相対パスを要素に持つ配列を返すだけです。
メインのXtalスクリプトファイルもここで読み込ませます。

**サンプル**
::
    return [
        "Script\\main.xtal"					,
        "Script\\stage.xtal"				,
        "Script\\own_char.xtal"				,
        "Script\\boss.xtal"					,
    ];

メインスクリプトファイルについて
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

メインのXtalスクリプトファイルには、

- Init()
- Run()
- Draw()

という関数(メソッド)を持った一つのsingletonクラスが書かれている必要があります。
これらの関数を含むsingletonクラスはlib::GameFrameworkに代入してください。

**サンプル**
::
    singleton Game
    {
        Init()	{}
        Run()	{}
        Draw()	{}
    }

    lib::GameFramework: Game;

=====================
CryptedZip Project
=====================
CryptedZip Projectは、暗号化済みアーカイバ制作の為のプロジェクトです。
(お知らせ : CryptedZipは再実装され、圧縮とArcfourによる圧縮と暗号化ができるようになりました)

使用用途にゲームのリソース(アセット)のアーカイブを想定していますが、現在の実装では十分日常的用途にも耐えられると思われます。
圧縮はzlibによるものとgzipによるものが選択できます。暗号化はArcfourによるものです。

このアーカイバはディレクトリをサポートしています。
ディレクトリごとアーカイブする場合は、親ディレクトリを指定してアーカイブしてください。
階層化していても、再帰的にファイルが探索され、全てアーカイブされます。

なお、現在このアーカイバはファイル名の暗号化をサポートしていません。
このアーカイバを攻撃者が入手した場合は、容易にファイル名とディレクトリがバレてしまいます。注意してください。
もしくは自分で実装するか、誰かが実装するのを待ってください。
自分で実装した場合は、公開してください。お願いします。

CryptedZipプロジェクトの開発に必要なものは以下のとおり:

- Boost
- 暗号化技術についての知識
- 論理力

特に論理力は必要です。暗号化/復号処理は結果から推理できるようなバグは滅多にありません。
使い方はtest.cppを参考にしてください。これはもともとデバッグ用のものなのでいろいろごちゃごちゃしています。すいません。

ライセンスについて
------------------
どちらのプロジェクトも、LGPLライセンスに従います。

それと、差し支えなければKVARCOを使用してゲームを作った場合、
その旨を作者である七条に連絡していただけると嬉しいです。強制はしません。

=============
何かあったら
=============

twitter
  七条彰紀 http:twitter.com/shitijyou
e-mail
  shitijyou.a1072 (at) gmail.com
辺りまでご連絡ください

Copyright (C) 2011 七条 彰紀(shichijou akinori)
