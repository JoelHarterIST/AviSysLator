# avisyslator_gnc usage

[日本語](README.md) | [English](README.en.md)

## コンパイル方法

```shell
make
```

※コンパイル後に ``create_symbolmap_yaml.py`` が実行され ``symbol_map.yaml`` が自動生成される。

## 設定方法（任意）

`` test_case.yaml `` を編集する。

## 実行方法

```shell
./avsl test_case.yaml
```

## 出力確認方法

`` plot.ipynb `` を用いて `` time_seires_log_output.csv `` をグラフ化する。
