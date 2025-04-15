# translation_3dof_A02
## コンパイル方法
```
make
```
※コンパイル後に ``create_symbolmap_yaml.py`` が実行され ``symbol_map.yaml`` が自動生成される。


## 設定方法（任意）
`` test_case.yaml `` を編集する。

`` time_series_log_input.yaml `` を編集する。


## 実行方法
```
./avsl test_case.yaml
```


## 出力確認方法
`` plot.ipynb `` を用いて `` time_seires_log_output.csv `` をグラフ化する。

