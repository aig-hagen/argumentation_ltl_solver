#include "Encodings.h"
#include <algorithm>

using namespace std;

namespace Encodings {

vector<int> add_nonempty(const AF & af, bool reduct) {
	int base = 0;
	if (reduct) {
		base = 2*af.args;
	}
	vector<int> clause(af.args);
	for (int i = 1; i <= af.args; i++) {
		clause[i-1] = (base+i);
	}
	return clause;
}

vector<vector<int>> add_conflict_free(const AF & af) {
	vector<vector<int>> clauses;
	for (int i = 1; i <= af.args; i++) {
		for (int j = 0; j < af.attackers[i].size(); j++) {
			vector<int> clause = { i, -j };
			clauses.push_back(clause);
		}
	}
	return clauses;
}


vector<vector<int>> add_self_defending(const AF & af) {// TODO
	vector<vector<int>> clauses;
	for (int i = 1; i <= af.args; i++) {
		for (int j = 0; j < af.attackers[i].size(); j++) {
			vector<int> clause = { i, -j };
			clauses.push_back(clause);
		}
	}
	return clauses;
}

vector<vector<int>> add_admissible(const AF & af, bool reduct) {
	vector<vector<int>> clauses;
	int base = 0;
	if (reduct) {
		base = 2*af.args;
	}
	for (int i = 1; i <= af.args; i++) {
		vector<int> additional_clause = { -(base+af.args+i), -(base+i) };
		clauses.push_back(additional_clause);
		for (int j = 0; j < af.attackers[i].size(); j++) {
			vector<int> clause = { (base+af.args+i), -(base+af.attackers[i][j]) };
			clauses.push_back(clause);

			// CF clause
			vector<int> cf_clause;
			if (i != af.attackers[i][j]) {
				cf_clause = { -(base+i), -(base+af.attackers[i][j]) };
			} else {
				cf_clause = { -(base+i) };
			}
			clauses.push_back(cf_clause);
		}
		vector<int> clause(af.attackers[i].size() + 1);
		for (int j = 0; j < af.attackers[i].size(); j++) {
			clause[j] = (base+af.attackers[i][j]);
		}
		clause[af.attackers[i].size()] = -(base+af.args+i);
		clauses.push_back(clause);

		// ADM clause
		if (af.self_attack[i]) continue;
		for (int j = 0; j < af.attackers[i].size(); j++) {
			if (af.symmetric_attack.at(make_pair(af.attackers[i][j], i))) continue;
			vector<int> adm_clause = { -(base+i), (base+af.args+af.attackers[i][j]) };
			clauses.push_back(adm_clause);
		}
	}
	return clauses;
}

vector<vector<int>> add_reduct(const AF & af) {
	vector<vector<int>> clauses;
	for (int i = 1; i <= af.args; i++) {
		vector<int> clause;
		clauses.push_back({ -(2*af.args + i), -(i) });
		clauses.push_back({ -(3*af.args + i), -(i) });
		clauses.push_back({ -(2*af.args + i), -(af.args + i) });
		clauses.push_back({ -(3*af.args + i), -(af.args + i) });
	}
	return clauses;
}

}