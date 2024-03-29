#include <iostream>
#include <vector>
#include <string>

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

// DFS to find accesible states
void explore_states(int source, int &nr_accessed, std::vector<bool> &accessed, int **transitions, int m) {
	accessed[source] = true;
	nr_accessed--;

	for (int i = 0; i < m && nr_accessed; i++) {
		if (!accessed[transitions[source][i]]) {
			explore_states(transitions[source][i], nr_accessed, accessed, transitions, m);
		}
	}
}

// proceed from sources to find accesible states
void accesible(int n, int m, int s, int **transitions, int *sources) {
	std::vector<bool> accessed(n, false);
	int nr_accessed = n;

	// in case all states are sources
	if (s == n) {
		accessed.assign(n, true);
		print_accesible(accessed);
		return;
	}

	for (int i = 0; i < s && nr_accessed; i++) {
		if (!accessed[sources[i]]) {
			explore_states(sources[i], nr_accessed, accessed, transitions, m);
		}
	}

	print_accesible(accessed);
}

void print_sequence(std::vector<int> &sequence) {
	for (int i = 0; i < sequence.size(); i++) {
		std::cout << sequence[i] << std::endl;
	}
}

// all available source states transition on a partially synchronized sequence
void transition_states(std::vector<int> &states, int state_begin, std::vector<int> &sequence, int **transitions, int m) {
	for (auto &state : states) {
		for (int i = 0; i < sequence.size(); i++) {
			state = transitions[state][sequence[i]];
		}
	}

	// replace the 2 merged states with a single one
	for (int i = 0; i < sequence.size(); i++) {
		state_begin = transitions[state_begin][sequence[i]];
	}
	states.push_back(state_begin);

	sequence.clear();
}

bool contains_visited_state(std::vector<std::pair<int, int> > &visited, int stateA, int stateB) {
	for (auto &it : visited) {
		if (it.first == stateA && it.second == stateB) {
			return true;
		}
	}
	return false;
}

// DFS to find a partially synchronized sequence between 2 states
void find_sync_sequence(std::vector<std::pair<int, int> > &visited, std::vector<int> &sequence, int stateA, int stateB, int **transitions, int m) {
	visited.push_back(std::make_pair(stateA, stateB));

	// found a partially synchronized sequence
	if (stateA == stateB) {
		// mark current sequence as done
		sequence.push_back(-1);
		return;
	}

	for (int i = 0; i < m; i++) {
		int next_state_A = transitions[stateA][i];
		int next_state_B = transitions[stateB][i];

		// avoid cycles
		if (!contains_visited_state(visited, next_state_A, next_state_B)) {
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

// find the synchronized sequence between all source states
void synchronize(int n, int m, int s, int **transitions, int *sources) {
	std::vector<int> sequence;
	std::vector<int> states(sources, sources + s);
	std::vector<std::pair<int, int> > visited;

	while(states.size() > 1) {
		// merge first 2 states
		int stateA = states.back();
		states.pop_back();
		int stateB = states.back();
		states.pop_back();

		find_sync_sequence(visited, sequence, stateA, stateB, transitions, m);
		// remove the mark
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