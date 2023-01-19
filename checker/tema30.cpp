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

void synchronize(int n, int m, int s, int **transitions, int *sources) {
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
