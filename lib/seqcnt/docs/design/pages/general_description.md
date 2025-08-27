\page "General Description"

# Glossary and Abbrebations


## Glossary （用語集）

| # | Name | Description |
|:---:|:---|:---|
| 1 | TBD | あああ |
| 2 | TBD | あああ |
| 3 | TBD | あああ |


## Abbreviations （略語集）

| # | Name | Description |
|:---:|:---|:---|
| 1 | TBD | あああ |
| 2 | TBD | あああ |
| 3 | TBD | あああ |


# Architecture


## Structure （機能構成）
機能構成の概要を以下に示す。

| L/N | Name | Description |
|:---:|:---|:---|
| 1 | seq_ctrl <br> シーケンス制御メイン処理 | シーケンス制御機能の状態管理、シーケンスステップの管理、イベント判定処理呼び出し、イベント実行処理呼び出しを行う。 |
| 2 | event_judge <br> イベント判定処理 | Databaseに登録された参照変数が条件を満たしているか判定する。 |
| 3 | count_check <br> 判定回数チェック処理 | イベント判定が真と判定された回数をカウントし、閾値を上回ったらイベント成立を返す。 |
| 4 | event_exec <br> イベント実行処理 | Databaseに登録された変数に値を代入する。 |


## Block Diagram （ブロック図）
N/A

## State Transition Diagram（状態遷移図）
N/A


## Sequence Data Structure （シーケンスデータ構造）
シーケンスは下図のデータ構造で配列及び構造体に登録されている。

    
\image html seq_ctrl.drawio.png width=50%
\image latex seq_ctrl.drawio.png width=\textwidth


# Function
機能詳細を以下に示す。

| L/N | Name | Description |
|:---:|:---|:---|
| 1 | Under Construction <br> 作成中 | 作成中 |
| 2 | Under Construction <br> 作成中 | 作成中 |

