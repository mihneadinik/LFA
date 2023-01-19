#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <string>
#include <functional>
#include <utility>

void read_args(int &n, int &m, int &s, int ***transitions, int **sources) {
	std::cin >> n >> m >> s;

	// create and read transition matrix
	*transitions = new int*[n];
	for (int i = 0; i < n; i++) {
		(*transitions)[i] = new int[m];
		for (int j = 0; j < m; j++) {
			std::cin >> (*transitions)[i][j];
		}
	}

	// read source states or assign them all
	if (s > 0) {
		*sources = new int[s];
		for (int i = 0; i < s; i++) {
			std::cin >> (*sources)[i];
		}
	} else {
		s = n;
		*sources = new int[n];
		for (int i = 0; i < n; i++) {
			(*sources)[i] = i;
		}
	}
}

void clear_memory(int **transitions, int *sources, int n) {
	delete[] sources;

	for (int i = 0; i < n; i++) {
		delete[] transitions[i];
	}
	delete[] transitions;
}

void print_accesible(std::vector<bool> &accessed) {
	for (int i = 0; i < accessed.size(); i++) {
		if (accessed[i]) {
			std::cout << i << std::endl;
		}
	}
}

void explore_states(int source, int &nr_accessed, std::vector<bool> &accessed, int **transitions, int m) {
	accessed[source] = true;
	nr_accessed++;

	for (int i = 0; i < m; i++) {
		if (!accessed[transitions[source][i]]) {
			explore_states(transitions[source][i], nr_accessed, accessed, transitions, m);
		}
	}
}

void accesible(int n, int m, int s, int **transitions, int *sources) {
	std::vector<bool> accessed(n, false);
	int nr_accessed = 0;

	// in case all states are sources
	if (s == n) {
		accessed.assign(n, true);
		print_accesible(accessed);
		return;
	}

	for (int i = 0; i < s && nr_accessed < n; i++) {
		if (!accessed[sources[i]]) {
			explore_states(sources[i], nr_accessed, accessed, transitions, m);
		}
	}
	print_accesible(accessed);
}

std::string create_merged_state(int stateA, int stateB) {
	return std::to_string(stateA) + "," + std::to_string(stateB);
}

void print_sequence(std::vector<int> &sequence) {
	for (int i = 0; i < sequence.size(); i++) {
		std::cout << sequence[i] << std::endl;
	}
}

void transition_states(std::unordered_set<int> &states, int state_begin, std::vector<int> &sequence, int **transitions, int m) {
	std::unordered_set<int> new_states;
	for (auto state : states) {
		int curr_state = state;
		for (int i = 0; i < sequence.size(); i++) {
			curr_state = transitions[curr_state][sequence[i]];
		}
		new_states.insert(curr_state);
	}

	// replace the 2 merged states with a single one
	for (int i = 0; i < sequence.size(); i++) {
		state_begin = transitions[state_begin][sequence[i]];
	}
	new_states.insert(state_begin);

	states = new_states;
	sequence.clear();
}

bool contains_merged_state(std::unordered_set<std::string> &visited, int stateA, int stateB) {
	return visited.find(create_merged_state(stateA, stateB)) != visited.end() ||
		visited.find(create_merged_state(stateB, stateA)) != visited.end();
}

void find_sync_sequence(std::unordered_set<std::string> &visited, std::vector<int> &sequence, int stateA, int stateB, int **transitions, int m) {
	visited.insert(create_merged_state(stateA, stateB));

	// found a partially synchronized sequence
	if (stateA == stateB) {
		// mark current sequence as done
		sequence.push_back(-1);
		return;
	}

	for (int i = 0; i < m; i++) {
		int next_state_A = transitions[stateA][i];
		int next_state_B = transitions[stateB][i];

		if (!contains_merged_state(visited, next_state_A, next_state_B)) {
			sequence.push_back(i);
			int old_size = sequence.size();
			find_sync_sequence(visited, sequence, next_state_A, next_state_B, transitions, m);

			// this path lead to a partially synchronized sequence
			if (sequence.back() == -1) {
				return;
			}
			// this path lead to a dead end => remove and retry
			sequence.pop_back();
		}
	}
}

void synchronize(int n, int m, int s, int **transitions, int *sources) {
	std::vector<int> sequence;
	std::unordered_set<int> states(sources, sources + s);
	std::unordered_set<std::string> visited;

	while(states.size() > 1) {
		// merge first 2 states
		int stateA = *states.begin();
		states.erase(stateA);
		int stateB = *states.begin();
		states.erase(stateB);

		find_sync_sequence(visited, sequence, stateA, stateB, transitions, m);
		sequence.pop_back();

		print_sequence(sequence);
		transition_states(states, stateA, sequence, transitions, m);
		visited.clear();
	}
}

int main(int argc, char *argv[]) {
	std::string problem = argv[1];
	int n, m, s;
	int **transitions;
	int *sources;

	read_args(n, m, s, &transitions, &sources);

	if (problem == "accessible") {
		accesible(n, m, s, transitions, sources);
	} else if (problem == "synchronize") {
		synchronize(n, m, s, transitions, sources);
	} else {
		std::cout << "Invalid problem" << std::endl;
	}

	clear_memory(transitions, sources, n);
	return 0;
}