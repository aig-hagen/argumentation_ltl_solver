#include <iostream>
#include "AF.h"

using namespace std;

AF::AF() : args(0) {}

void AF::set_num_arguments(int num_args)
{
	args = num_args;
	attackers.resize(args+1);
	attacked.resize(args+1);
	unattacked.resize(args+1, true);
	self_attack.resize(args+1);
}

void AF::add_attack(pair<int,int> att)
{
	if (att.first > args || att.second > args) {
		return;
	}
	attackers[att.second].push_back(att.first);
	attacked[att.first].push_back(att.second);
	unattacked[att.second] = false;
	if (att.first == att.second) {
		self_attack[att.first] = true;
	}
	att_exists[att] = true;
	if (att_exists.count(make_pair(att.second, att.first)) > 0) {
		symmetric_attack[att] = true;
		symmetric_attack[make_pair(att.second, att.first)] = true;
	} else {
		symmetric_attack[att] = false;
	}
}

void AF::set_solver_path(string path) {
	solver_path = path;
}

int AF::accepted_var(int arg) {
	return arg;
}

int AF::rejected_var(int arg) {
	return args+arg;
}

int AF::accepted_var_r(int arg) {
	return 2*args + arg;
}

int AF::rejected_var_r(int arg) {
	return 3*args + arg;
}