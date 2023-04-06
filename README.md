# Graduation-research-Tsukuba-Univ-

Short project description.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

1.datasetの作成
[教師用学習データ]
Makefile_tutorialを用いてjetinfo.ccをコンパイル
make -f Makefile_tutorial jetinfo

./jetinfo　で実行

実行後にはjetinfo.root, info_jet.txtという二つのファイルが生成される。
jetinfo.rootはこの後の機械学習を用いたジェット識別に利用するデータテーブルが含まれる。
info_jet.txtにはどのような運動量を持ったジェットが生成されたのかの情報を保持している。

[テストデータ]
基本的に実行するコマンドは変わらず、実行後も同様のファイルが出力される。
異なるのはジェット識別に利用するデータテーブルである。
先ほどは識別の学習に用いるためにジェット種によってデータを分けていたが、テストデータは実際に識別を行うため2種類のジェット種を混合させたデータテーブルを生成する。
以下、参考までに実行するコマンドを付記する。

make -f Makefile_tutorial dataset
./dataset


