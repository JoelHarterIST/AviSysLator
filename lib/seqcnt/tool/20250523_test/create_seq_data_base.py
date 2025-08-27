# create_seq_data_base.py
# version 2025.05.23
#
# 使い方
# 1. シーケンス制御機能で制御した機能のインタフェース公開用構造体を定義したヘッダファイルを用意する（例：fcu_if.h）。
#    注：状態やオプションの識別子はENUMを使用せず、defineで定義すること。
# 2. spreadsheet:seq_data_base_sampleをコピーしシーケンス情報を入力る。
# 3. シーケンス情報を定義したシートをcsvで出力する。
# 4. 出力したファイルを本pythonスクリプトと同じディレクトリに配置する。
# 5. 本スクリプトを実行する。
# 6. 出力されたseq_data_base.hを適切な場所に配置する。

# import
import os
import numpy as np
import pandas as pd
from datetime import datetime

# setting
I_SEQ_STEP = 'seq_step'
I_STEP_NAME = 'seq_step_name'
I_EVENT_ID = 'event_id'
I_EVENT_NAME = 'event_name'
I_NEXT_STEP = 'event_next_step'
I_NEXT_SEQ = 'event_next_seq'
I_ALL_AND_OR = 'event_all_and / all_or'
I_NEXT_EVENT = 'event_next'
I_JUDGE_ID = 'judge_id'
I_EXEC_ID = 'exec_id'
I_JUDGE_NAME = 'judge_name'
I_JUDGE_TYPE = 'judge_type'
I_JUDGE_REF = 'judge_ref'
I_JUDGE_OPE = 'judge_ope'
I_JUDGE_THRS = 'judge_thrs'
I_JUDGE_NUM = 'judge_num'
I_JUDGE_NEXT = 'judge_next'
I_EXEC_NAME = 'exec_name'
I_EXEC_TYPE = 'exec_type'
I_EXEC_DEST = 'exec_dest'
I_EXEC_SRC = 'exec_src'
I_EXEC_NEXT = 'exec_next'

O_ADDRESS_T = 'ADDRESS_T'
O_SEQ_ID_NUM_MAX = 10
O_STEP_NUM_MAX = 20
O_EVENT_NUM_MAX = 20
O_JUDGE_NUM_MAX = 20
O_EXEC_NUM_MAX = 20
O_SEQ_ID_MAX_DEF = 'SEQ_ID_MAX'
O_SEQ_ID_DATA_BASE = 'seq_id_data_base'
O_SEQ_STEP_MAX_DEF = 'SEQ_STEP_MAX'
O_SEQ_STEP_DATA_BASE = 'seq_step_data_base'
O_EVENT_DATA_T = 'event_data_t'
O_EVENT_DATA_BASE = 'event_data_base'
O_JUDGE_DATA_T = 'judge_data_t'
O_JUDGE_DATA_BASE = 'judge_data_base'
O_EXEC_DATA_T = 'exec_data_t'
O_EXEC_DATA_BASE = 'exec_data_base'

# init
str_def_seq_event_base = ''
str_def_seq_judge_base = ''
str_def_seq_exec_base = ''
str_seq_id_data_base = f'int {O_SEQ_ID_DATA_BASE}[{O_SEQ_ID_MAX_DEF}] = {{\n'
total_step_num = 0
str_seq_step_data_base = f'{O_EVENT_DATA_T}* {O_SEQ_STEP_DATA_BASE}[{O_SEQ_STEP_MAX_DEF}] = {{\n'
total_event_num = 0
str_event = f'{O_EVENT_DATA_T} {O_EVENT_DATA_BASE}[{O_EVENT_NUM_MAX}] = {{\n'
total_judge_num = 0
str_judge = f'{O_JUDGE_DATA_T} {O_JUDGE_DATA_BASE}[{O_JUDGE_NUM_MAX}] = {{\n'
total_exec_num = 0
str_exec = f'{O_EXEC_DATA_T} {O_EXEC_DATA_BASE}[{O_EXEC_NUM_MAX}] = {{\n'

# input files
files = os.listdir()
files = [i for i in files if i.endswith('.csv') == True]
files = sorted(files)
print(files)
lenfiles = len(files)

# input each file
for j in range(lenfiles):
	# define base
	str_seq_event_base = f'SEQ_{j}_EVENT_BASE'
	str_def_seq_event_base = str_def_seq_event_base \
		+ f'#define {str_seq_event_base} ({total_event_num})\n' 
	str_seq_judge_base = f'SEQ_{j}_JUDGE_BASE'
	str_def_seq_judge_base = str_def_seq_judge_base \
		+ f'#define {str_seq_judge_base} ({total_judge_num})\n' 
	str_seq_exec_base = f'SEQ_{j}_EXEC_BASE'
	str_def_seq_exec_base = str_def_seq_exec_base \
		+ f'#define {str_seq_exec_base} ({total_exec_num})\n' 

	# seq_id_to_step
	str_seq_id_data_base = str_seq_id_data_base \
		+ f'\t{total_step_num},\t// SEQ {j}\n'

	# create dataframe
	filename = files[j]
	dfTmp = pd.read_csv(filename)
	new_columns = []
	for i in range(len(dfTmp.columns)):
		column = dfTmp.columns[i]
		title = dfTmp.iloc[0,i]
		if not ('Unnamed' in column):
			category = column
		if type(title) is str:
			new_column = category + '_' + title
			new_columns.append(new_column)
		else:
			new_columns.append(column)
	dfTmp.columns = new_columns
	dfSeq = dfTmp.drop(dfTmp.index[0]).reset_index()

	# seq_step_data_base
	dfSeqTmp = dfSeq.loc[:,[I_SEQ_STEP, I_STEP_NAME, I_EVENT_ID]]
	dfSeqStep = dfSeqTmp.dropna(how='any').reset_index()
	step_num = dfSeqStep.shape[0]
	strtmp = ''
	for i in range(step_num):
		strtmp = strtmp + f'\t&{O_EVENT_DATA_BASE}[{str_seq_event_base}+{dfSeqStep.loc[i,I_EVENT_ID]}],'
		strtmp = strtmp + f'\t// SEQ {j} : {dfSeqStep.loc[i,I_STEP_NAME]}\n'
	total_step_num = total_step_num + step_num
	str_seq_step_data_base = str_seq_step_data_base + strtmp

	# event_data_base
	dfSeqTmp = dfSeq.loc[:,[I_EVENT_ID, I_EVENT_NAME, I_NEXT_STEP, I_NEXT_SEQ, I_NEXT_EVENT, 
							I_ALL_AND_OR, I_JUDGE_ID, I_EXEC_ID]]
	dfSeqEvent = dfSeqTmp.dropna(subset=[I_EVENT_ID]).reset_index()
	event_num = dfSeqEvent.shape[0]
	total_event_num = total_event_num + event_num
	strtmp = ''
	for i in range(event_num):
		if type(dfSeqEvent.loc[i, I_NEXT_STEP]) is str:
			str_next_step = dfSeqEvent.loc[i, I_NEXT_STEP]
		else:
			str_next_step = '-1'
		if type(dfSeqEvent.loc[i, I_NEXT_SEQ]) is str:
			str_next_seq = dfSeqEvent.loc[i, I_NEXT_SEQ]
		else:
			str_next_seq = '-1'
		if type(dfSeqEvent.loc[i, I_NEXT_EVENT]) is str:
			str_next_event = f'({O_ADDRESS_T})&{O_EVENT_DATA_BASE}[{dfSeqEvent.loc[i, I_NEXT_EVENT]}]'
		else:
			str_next_event = '0'
		if type(dfSeqEvent.loc[i, I_JUDGE_ID]) is str:
			str_event_judge = f'({O_ADDRESS_T})&{O_JUDGE_DATA_BASE}[{str_seq_judge_base}+{dfSeqEvent.loc[i, I_JUDGE_ID]}]'
		else:
			str_event_judge = '0'
		if type(dfSeqEvent.loc[i, I_EXEC_ID]) is str:
			str_event_exec = f'({O_ADDRESS_T})&{O_EXEC_DATA_BASE}[{str_seq_exec_base}+{dfSeqEvent.loc[i, I_EXEC_ID]}]'
		else:
			str_event_exec = '0'
		strtmp = strtmp \
				+ f'\t{{{str(dfSeqEvent.loc[i, I_ALL_AND_OR])}' \
				+ ', ' + str_event_judge \
				+ ', ' + str_event_exec \
				+ ', ' + str_next_step \
				+ ', ' + str_next_seq \
				+ ', ' + str_next_event + '},' \
				+ f'\t// SEQ {j} : {dfSeqEvent.loc[i, I_EVENT_NAME]}' \
				+ '\n'
	str_event = str_event + strtmp

	# judge_data_base
	dctOpe = {'==':'EQ', '!=':'NEQ', '<':'LT', '<=':'LTE', '>':'GT', '>=':'GTE'}
	dfSeqTmp = dfSeq.loc[:,I_JUDGE_ID:I_JUDGE_NEXT]
	dfSeqJudge = dfSeqTmp.dropna(subset=[I_JUDGE_ID]).reset_index()
	judge_num = dfSeqJudge.shape[0]
	total_judge_num = total_judge_num + judge_num
	strtmp = ''
	for i in range(judge_num):
		if type(dfSeqJudge.loc[i, I_JUDGE_NEXT]) is str:
			str_next_judge = f'({O_ADDRESS_T})&{O_JUDGE_DATA_BASE}[{dfSeqJudge.loc[i, I_JUDGE_NEXT]}]'
		else:
			str_next_judge = '0'
		strtmp = strtmp \
				+ f'\t{{{dfSeqJudge.loc[i, I_JUDGE_TYPE]}' \
				+ f', {dctOpe[dfSeqJudge.loc[i, I_JUDGE_OPE]]}' \
				+ f', ({O_ADDRESS_T})&{dfSeqJudge.loc[i, I_JUDGE_REF]}' \
				+ f', {dfSeqJudge.loc[i, I_JUDGE_THRS]}' \
				+ f', 0, {dfSeqJudge.loc[i, I_JUDGE_NUM]}' \
				+ f', {str_next_judge}}},' \
				+ f'\t// SEQ {j} : {dfSeqJudge.loc[i, I_JUDGE_NAME]}' \
				+ '\n'
	str_judge = str_judge + strtmp

	# exec_data_base
	dfSeqTmp = dfSeq.loc[:,I_EXEC_ID:I_EXEC_NEXT]
	dfSeqExec = dfSeqTmp.dropna(subset=[I_EXEC_ID]).reset_index()
	exec_num = dfSeqExec.shape[0]
	total_exec_num = total_exec_num + exec_num
	strtmp = ''
	for i in range(exec_num):
		if type(dfSeqExec.loc[i, I_EXEC_NEXT]) is str:
			str_next_exec = f'({O_ADDRESS_T})&{O_EXEC_DATA_BASE}[{dfSeqExec.loc[i, I_EXEC_NEXT]}]'
		else:
			str_next_exec = '0'
		strtmp = strtmp \
				+ f'\t{{{dfSeqExec.loc[i, I_EXEC_TYPE]}' \
				+ f', ({O_ADDRESS_T})&{dfSeqExec.loc[i, I_EXEC_DEST]}' \
				+ f', {dfSeqExec.loc[i, I_EXEC_SRC]}' \
				+ f', {str_next_exec}}},' \
				+ f'\t// SEQ {j} : {dfSeqExec.loc[i, I_EXEC_NAME]}' \
				+ '\n'
	str_exec = str_exec + strtmp

# close array
strtmp = ''
for i in range(j+1, O_SEQ_ID_NUM_MAX):
	strtmp = strtmp + f'\t0,\n'
str_seq_id_data_base = str_seq_id_data_base + strtmp[0:-2] + '};\n'
strtmp = ''
for i in range(total_step_num, O_STEP_NUM_MAX):
	strtmp = strtmp + f'\t0,\n'
str_seq_step_data_base = str_seq_step_data_base + strtmp[0:-2] + '};\n'
strtmp = ''
for i in range(total_event_num, O_EVENT_NUM_MAX):
	strtmp = strtmp + '\t{0, 0, 0, -1, -1, 0},\n'
str_event = str_event + strtmp[0:-2] + '\n};\n'
strtmp = ''
for i in range(total_judge_num, O_JUDGE_NUM_MAX):
	strtmp = strtmp + '\t{0, 0, 0, 0, 0, 0, 0},\n'
str_judge = str_judge + strtmp[0:-2] + '\n};\n'
strtmp = ''
for i in range(total_exec_num, O_EXEC_NUM_MAX):
	strtmp = strtmp + '\t{0, 0, 0, 0},\n'
str_exec = str_exec + strtmp[0:-2] + '\n};\n'

# output
with open('seq_data_base.h', mode='w') as f:
	f.write('// autogenarated sequence definition header file\n')
	f.write(f'// generated {datetime.now()}\n')
	f.write('\n')
	f.write('// define\n')
	f.write(str_def_seq_event_base)
	f.write(str_def_seq_judge_base)
	f.write(str_def_seq_exec_base)
	f.write('\n')
	f.write('// extern\n')
	f.write(f'extern int {O_SEQ_ID_DATA_BASE}[{O_SEQ_ID_MAX_DEF}];\n')
	f.write(f'extern {O_EVENT_DATA_T}* {O_SEQ_STEP_DATA_BASE}[{O_SEQ_STEP_MAX_DEF}];\n')
	f.write(f'extern {O_EVENT_DATA_T} {O_EVENT_DATA_BASE}[{O_EVENT_NUM_MAX}];\n')
	f.write(f'extern {O_JUDGE_DATA_T} {O_JUDGE_DATA_BASE}[{O_JUDGE_NUM_MAX}];\n')
	f.write(f'extern {O_EXEC_DATA_T} {O_EXEC_DATA_BASE}[{O_EXEC_NUM_MAX}];\n')
	f.write('\n')
	f.write('// seq_id definition\n')
	f.write(str_seq_id_data_base)
	f.write('\n')
	f.write('// seq_step definition\n')
	f.write(str_seq_step_data_base)
	f.write('\n')
	f.write('// event definition\n')
	f.write(str_event)
	f.write('\n')
	f.write('// judge definition\n')
	f.write(str_judge)
	f.write('\n')
	f.write('// exec definition\n')
	f.write(str_exec)

