/// @file   seqcnt.c
/// @brief  sequence control

#include "seqcnt.h"

int seq_step = 0;
int seq_timer = 0;
int step_timer = 0;
int free_timer = 0;

void seqcnt(seqcnt_state_t* seqcnt_state, 
					seqcnt_state_t* seqcnt_state_next, 
					const int seq_id_data_base[SEQ_ID_MAX], 
					event_data_t* seq_step_data_base[SEQ_STEP_MAX]) {
	int i;
	int _base;
	int _step;
	int _idx;
	int _next_step;
	bool _rst;
	int _ret;
	bool _rslt;
	event_data_t* _event_p;

	// check state change and reset
	_rst = false;
	if (seqcnt_state->mode != seqcnt_state_next->mode) {
		free_timer = 0;
		seq_timer = 0;
		step_timer = 0;
		_rst = true;
	}
	else if (seqcnt_state->id != seqcnt_state_next->id) {
		seq_timer = 0;
		step_timer = 0;
		_rst = true;
	}
	else if (seqcnt_state->step != seqcnt_state_next->step) {
		step_timer = 0;
		_rst = true;
	}

	// update state
	*seqcnt_state = *seqcnt_state_next;

	if (SEQCNT_EXEC == seqcnt_state->mode) {
		_base = seq_id_data_base[seqcnt_state->id];
		_step = seqcnt_state->step;
		_idx = _base + _step;
		_event_p = seq_step_data_base[_idx];

		i = 0;
		_ret = 0;
		_rslt = false;
		while(i++ < EVENT_MAX) {
			// call event_judge
			if (0 != _event_p->judge_data_address) {
				_ret = event_judge(_event_p->judge_data_address, 
									_event_p->judge_condition, _rst, &_rslt);
			}
			else {
				_rslt = 1;
			}

			// call event_exec
			if (0 == _ret) {
				if ((_rslt) && (0 != _event_p->exec_data_address)) {
					_ret = event_exec(_event_p->exec_data_address);
				}
			}

			// break event
			if ((0 == _event_p->next_event) ||
				(_rslt) || (0 != _ret)) {
				break;
			}

			// set next event
			_event_p = (event_data_t*)_event_p->next_event;
		}
		if (i > EVENT_MAX) {
			_ret = 1;
		}

		// count timer
		if ((free_timer < TIMER_MAX) &&
			(seq_timer < TIMER_MAX) &&
			(step_timer < TIMER_MAX)) {
			free_timer++;
			seq_timer++;
			step_timer++;
		}
		else {
			_ret = 1;
		}

		// on error stop seq
		if (0 != _ret) {
			seqcnt_state_next->mode = SEQCNT_STOP;
			seqcnt_state_next->error = _ret;
			return ;
		}

		// if true set next state
		if (_rslt) {
			if (0 < _event_p->next_step) {
				seqcnt_state_next->step = _event_p->next_step;
			}
			else if (0 < _event_p->next_seq) {
				seqcnt_state_next->id = _event_p->next_seq;
				seqcnt_state_next->step = 0;
			}
			else {
				seqcnt_state_next->mode = SEQCNT_STOP;
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// function test_func
///////////////////////////////////////////////////////////////////////////////

/// @brief テスト処理
///
/// いずれ消す。
void test_func(int input, int* output) {
	*output = input + 3;
}


///////////////////////////////////////////////////////////////////////////////
// function event_judge
///////////////////////////////////////////////////////////////////////////////

/// @brief イベント判定処理
///
/// Databaseに登録された条件に従いイベント判定処理を行う。
/// また、判定回数チェック処理を呼び出す。
int event_judge(ADDRESS_T judge_data_address, judge_condition_t judge_condition, bool reset_count, bool* judge_result) {

	int i = 0;
	int _ret = 0;
	int _rslt = 0;
	int _ref_i, _thrs_i;
	double _ref_d, _thrs_d, _thrs_lo, _thrs_hi;
	double _eps_d = 1e-8;
	judge_data_t* _judge_p;

	_judge_p = (judge_data_t*)judge_data_address;

	i = 0;
	while(i++ < JUDGE_MAX) {
		if (reset_count) {
			_judge_p->count = 0;
		}
		switch (_judge_p->type) {
			case INT:
				_ref_i = *((int*)_judge_p->ref);
				_thrs_i = (int)_judge_p->thrs;
				switch (_judge_p->ope) {
					case EQ:
						if (_ref_i == _thrs_i) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case NEQ:
						if (_ref_i != _thrs_i) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case LT:
						if (_ref_i < _thrs_i) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case LTE:
						if (_ref_i <= _thrs_i) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case GT:
						if (_ref_i > _thrs_i) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case GTE:
						if (_ref_i >= _thrs_i) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					default:
						_ret = 1;
						break;
				}
				break;
			case DBL:
				_ref_d = *((double*)_judge_p->ref);
				_thrs_d = *((double*)&_judge_p->thrs);
				switch (_judge_p->ope) {
					case EQ:
						if (((_thrs_d - _eps_d) <= _ref_d)
							&& (_ref_d <= (_thrs_d + _eps_d))) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case NEQ:
						if ((_ref_d < (_thrs_d - _eps_d))
							|| ((_thrs_d + _eps_d) < _ref_d)) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case LT:
						if (_ref_d < _thrs_d) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case LTE:
						if (_ref_d <= _thrs_d) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case GT:
						if (_ref_d > _thrs_d) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					case GTE:
						if (_ref_d >= _thrs_d) {
							_rslt = _rslt
								+ check_count(_judge_p->num, &_judge_p->count);
						}
						break;
					default:
						_ret = 1;
						break;
				}
				break;
			default:
				_ret = 1;
				break;
		}
		if ((1 == _ret) || (0 == _judge_p->next)) {
			break;
		}
		_judge_p = (judge_data_t*)_judge_p->next;
	}

	if (JUDGE_MAX <= i) {
		_ret = 1;
	}

	*judge_result = false;
	if (0 == _ret) {
		if (ALL_OR == judge_condition) {
			if (0 < _rslt) {
				*judge_result = true;
			}
		}
		else {
			if (i == _rslt) {
				*judge_result = true;
			}
		}
	}

	return _ret;
}


///////////////////////////////////////////////////////////////////////////////
// function check_count
///////////////////////////////////////////////////////////////////////////////

/// @brief 判定回数チェック処理
///
/// 真と判定された回数をカウントし、イベント成立に必要な回数以上であれば、成立(1)を返す。
int check_count(int num, int* ctr) {
	*ctr = *ctr + 1;
	if(num <= *ctr) {
		*ctr = num;
		return 1;
	}
	else {
		return 0;
	}
}


///////////////////////////////////////////////////////////////////////////////
// function event_exec
///////////////////////////////////////////////////////////////////////////////

/// @brief イベント実行処理
///
/// Databaseに登録されたイベント実行処理を行う。
int event_exec(ADDRESS_T exec_data_address) {

	int i = 0;
	int _ret = 0;
	exec_data_t* _exec_p;

	_exec_p = (exec_data_t*)exec_data_address;

	i = 0;
	while(i++ < EXEC_MAX) {
		switch (_exec_p->type) {
			case INT:
				*((int*)_exec_p->dest) = (int)_exec_p->src;
				break;
			case DBL:
				*((double*)_exec_p->dest) = *((double*)&_exec_p->src);
				break;
			default:
				_ret = 1;
				break;
		}
		if ((1 == _ret) || (0 == _exec_p->next)) {
			break;
		}
		_exec_p = (exec_data_t*)_exec_p->next;
	}

	if (EXEC_MAX <= i) {
		_ret = 1;
	}

	return _ret;
}

