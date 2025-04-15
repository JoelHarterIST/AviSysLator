\page "General Description"

# Glossary and Abbrebations


## Glossary （用語集）

| # | Name | Description |
|:---:|:---|:---|
| 1 | stdout | 標準出力。 |
| 2 | symbol_map_file | yaml形式で実行ファイルの変数名とアドレスを定義した設定ファイル。 |
| 3 | test_case_file | yaml形式でテスト開始時の変数の初期値、time_series_logやcyclic_log、event_log等を定義した設定ファイル。 |
| 4 | init_input_file | yaml形式でテスト開始時の変数の初期値を定義した設定ファイル。test_case_fileと合わせて使用される。 |
| 5 | std_out_log_input_file | yaml形式でstdoutに出力するcyclic_logやevent_logに関連する情報を定義した設定ファイル。test_case_fileと合わせて使用される。 |
| 6 | time_series_log_input_file | yaml形式でtime_series_logに関連する情報を定義した設定ファイル。test_case_fileと合わせて使用される。 |
| 7 | std_out_log_file | stdoutに出力したcyclic_log、event_logを保存したログファイル。 |
| 8 | time_series_log_file | tsv形式でtest_case_fileで定義した変数の値を周期的に出力したログファイル。 |
| 9 | symbol_map_data | 変数名とアドレスを格納したデータ。 |
| 10 | init_data | 変数名と初期値を格納したデータ。 |
| 11 | cyclic_log_data | stdoutに出力する変数情報を格納したデータ。 |
| 12 | event_log_data | stdoutに出力するイベント情報を格納したデータ。 |
| 13 | time_series_log_data | time_series_logに出力する変数情報を格納したデータ。 |


## Abbreviations （略語集）
N/A


# Architecture


## Structure （機能構成）
機能構成の概要を以下に示す。

| # | Name | Description |
|:---:|:---|:---|
| 1 | userif <br> ユーザーインターフェース | 変数の初期化、ログ出力、イベント監視を行う外部関数を提供する。 |
| 2 | load_file <br> 設定ファイル読み取り | yaml形式の設定ファイルを読み取る。読み取りに失敗した場合は（例えば指定したファイルがない、フォーマットに誤りがある等）、stdoutにエラーを通知し、処理を中断する。読み取りに成功した場合は、読み取った情報をstdoutに出力する。 |
| 3 | create_data <br> 内部データ作成 | 設定ファイルを読み込んだ情報からuserifのログ作成、イベント監視用の内部データを作成する。内部データの個数は制限があり、制限を超えた場合はそれ以上のデータは作成しない。 |
| 4 | write_log <br> ログ出力 | 設定ファイルで指定した変数の値やイベント情報をログに出力する。イベントは条件不成立から条件成立時のみ出力する。 |
| 5 | init_param <br> 初期値設定 | 設定ファイルで指定した変数に初期値を設定する。同じ変数に２回初期値を設定した場合は、値を上書する。 |


## Block Diagram （ブロック図）
ブロック図を以下に示す。

\image latex block.drawio.png width=\textwidth


## State Transition Diagram（状態遷移図）
N/A


# Function
機能詳細を以下に示す。

## userif
userif機能を以下に示す。
| # | Name | Description |
|:---:|:---|:---|
| 1 | userif_init <br> userifの初期化処理 | 設定ファイルの読み込み、ログファイル作成、変数の初期化を行う。 |
| 2 | userif_main_loop_start <br> userifのloopの開始処理 | シミュレーション開始を通知し、ログにヘッダー、シミュレーション開始時の変数の値を出力する。 |
| 3 | userif_main_loop_end <br> userifのloopの終了処理 | シミュレーション終了を通知する。 |
| 4 | userif_main <br> userifの実行処理 | ログ出力、イベント監視を行う。 |
| 5 | userif_finish <br> userifの終了処理 | ログファイルを保存する。 |


## load_file
load_file機能を以下に示す。

| # | Name | Description |
|:---:|:---|:---|
| 1 | load_symbol_map <br> symbol_map_fileの読み込み | symbol_map_fileを読み込む。実行プロセスの開始アドレスは毎回異なるので、開始アドレスをmark_variableという変数名から計算する。読み込み時、フォーマットに異常がある場合はエラー情報をログに出力し、処理を中断する。 |
| 2 | load_test_case <br> test_case_fileの読み込み | test_case_fileを読み込む。読み込み時、フォーマットに異常がある場合はエラー情報をログに出力し、処理を中断する。 |
| 3 | load_init_input <br> init_input_fileの読み込み | init_input_fileを読み込む。読み込み時、フォーマットに異常がある場合はエラー情報をログに出力し、処理を中断する。 |
| 4 | load_time_series_log_input <br> time_series_log_input_fileの読み込み | time_series_log_input_fileを読み込む。読み込み時、フォーマットに異常がある場合はエラー情報をログに出力し、処理を中断する。 |
| 5 | load_std_out_log_input <br> std_out_log_input_fileの読み込み | std_out_log_input_fileを読み込む。読み込み時異常がある場合はエラー情報をログに出力し、処理を中断する。 |


## create_data
create_data機能を以下に示す。

| # | Name | Description |
|:---:|:---|:---|
| 1 | create_time_series_log_data <br> time_series_log_dataの作成 | time_series_log_dataを作成する。作成時、symbol_map_dataを元に変数のアドレスを計算する。対象ログのデータ数がTIME_SERIES_LOG_DATA_MAXを超えた場合、それ以降のデータを無効とする。 |
| 2 | create_cyclic_log_data <br> cyclic_log_dataの作成 | cyclic_log_dataを作成する。作成時、symbol_map_dataを元に変数のアドレスを計算する。対象ログのデータ数がCYCLIC_LOG_DATA_MAXを超えた場合、それ以降のデータを無効とする。 |
| 3 | create_event_log_data <br> event_log_dataの作成 | event_log_dataを作成する。作成時、symbol_map_dataを元にプロセスの変数のアドレスを計算する。イベントデータ数がEVENT_LOG_DATA_MAXを超えた場合、それ以降のデータを無効とする。また１つのイベントに対するモニタデータ数がEVENT_MONITOR_DATA_MAXを超えた場合、それ以降のデータを無効とする。 |
| 4 | create_variable_data <br> variable_dataの作成 | プロセスのbase_address、変数名、symbol_map_dataから、指定された変数のアドレスを計算し、variable_dataを作成する。指定された変数がsymbol_map_dataにない場合は、variable_dataの変数名は""、アドレスはNULLで返す。 |
| 5 | get_symbol_address <br> symbolのアドレス取得 | 指定した変数名に対応するアドレスを取得する。 |


## write_log
write_log機能を以下に示す。

| # | Name | Description |
|:---:|:---|:---|
| 1 | write_time_series_log_header <br> time_series_logのヘッダ作成 | time_series_log_fileに変数名のヘッダを出力する。変数名がsymbol_map_dataにない場合は処理をスキップする。 |
| 2 | write_time_series_log_frame <br> time_series_logのframe_data出力 | 周期毎にtime_series_log_fileへ変数の値を出力する。変数名がsymbol_map_dataにない場合は処理をスキップする。 |
| 3 | write_cyclic_log_header <br> cyclic_logのヘッダ作成 | stdoutに変数名のヘッダを出力する。変数名がsymbol_map_dataにない場合は処理をスキップする。 |
| 4 | write_cyclic_log_frame <br> cyclic_logのframe_data出力 | 周期毎にstdoutへ変数の値を出力する。変数名がsymbol_map_dataにない場合は処理をスキップする。 |
| 5 | write_event_int <br> イベント出力(型：int) | stdoutへ対象イベントの種類との変数の値を出力する。出力の型はint。 |
| 6 | write_event_double <br> イベント出力(型：double) | stdoutへ対象イベントの種類と変数の値を出力する。出力の型はdouble。 |
| 7 | write_event_monitor <br> イベントに関連する変数の出力 | stdoutへイベントに関連する変数の値を出力する。 |
| 8 | check_event <br> イベント監視 | イベントを監視し、イベント条件が不成立から成立した時にイベントログを出力する。 |
| 9 | check_condition_int <br> イベント条件判定(型：init) | イベント条件が成立したか判定する。型はint。 |
| 10 | check_condition_double <br> イベント条件判定(型：double) | イベント条件が成立したか判定する。型はdouble。 |
| 11 | std_trace_init <br> ログ初期化処理 | std_out_log_fileを作成する。test_case_fileを読む前に初期化を行うため、ファイル名はテンポラリファイル"std_log_out_temp.txt"とする。 |
| 12 | std_out_trace <br> ログ出力 | stdoutとstd_out_log_fileにログを出力する。 |
| 13 | stderr_out_trace <br> エラーログ出力 | stderrとstd_out_log_fileにログを出力する。 |
| 14 | std_trace_term <br> ログ終了処理 | std_out_log_fileを保存する。保存時にテンポラリファイルからtest_case_fileで指定されたファイル名に変更する。 |


## init_param
init_param機能を以下に示す。

| # | Name | Description |
|:---:|:---|:---|
| 1 | init_param <br> 変数の初期化 | 指定した変数を初期化する。同じ変数名を２回初期化した場合、値は上書きされ、警告を表示する。|


## Others
その他の機能を以下に示す。

| # | Name | Description |
|:---:|:---|:---|
| 1 | get_exe_dir <br> 実行ファイルのディレクトリ取得 | 実行しているファイルのディレクトリを取得する。 |

