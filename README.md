# Graduation-research-Tsukuba-Univ-

Short project description.

# Table of Contents

- [インストール](#installation)
- [使用方法](#usage)

# Requirement
・python 3
・hipe4ml
・pythia
# Installation
pythiaとhipe4mlをインストールしてください。

# Usage
## 1.教師用データセットの作成
こんにちは
Makefileを用いてjetinfo.ccをコンパイル
	make -f Makefile_tutorial jetinfo
	./jetinfo
実行後にはjetinfo.root, info_jet.txtという二つのファイルが生成される。
jetinfo.rootはこの後の機械学習を用いたジェット識別に利用するデータテーブルが含まれる。
info_jet.txtにはどのような運動量を持ったジェットが生成されたのかの情報を保持している。

## 2.テスト用データセットの作成
基本的に実行するコマンドは変わらず、実行後も同様のファイルが出力される。
異なるのはジェット識別に利用するデータテーブルである。
先ほどは識別の学習に用いるためにジェット種によってデータを分けていたが、テストデータは実際に識別を行うため2種類のジェット種を混合させたデータテーブルを生成する。
以下、参考までに実行するコマンドを付記する。

	make -f Makefile_tutorial dataset
	./dataset

## 機械学習を用いたジェット識別
生成された"jetinfo.root"と"dataset.root"というrootファイルがあるディレクトリで
	python binary-jet.py
を実行すれば学習の後識別を行い、グラフを出力する。
