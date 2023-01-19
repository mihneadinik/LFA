#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#define EAST 0
#define NORTH 1
#define WEST 2
#define SOUTH 3
#define TRANSITIONS 4

void read_args(int &l, int &c, int &r, int ***cells, std::unordered_map<int, std::pair<int, int> > &robots) {
	std::cin >> l >> c >> r;

	// create and read transition cell matrix
	*cells = new int*[l];
	for (int i = 0; i < l; i++) {
		(*cells)[i] = new int[c];
		for (int j = 0; j < c; j++) {
			std::cin >> (*cells)[i][j];
		}
	}

	// read robots' positions
	for (int i = 0; i < r; i++) {
		int x, y;
		std::cin >> x >> y;

		robots[i] = std::make_pair(x, y);
	}
}

void clear_memory(int **cells, int l) {
	for (int i = 0; i < l; i++) {
		delete[] cells[i];
	}

	delete[] cells;
}

/*
	l = 3, c = 4

	a b c d		0 1 2 3
	e f g h		4 5 6 7
	i j k l		8 9 10 11

	g = 1, 2 => 6
*/

int encode_cells(int posX, int posY, int l, int c) {
	return posX * c + posY;
}

std::vector<int> decode_walls(int cell) {
	std::vector<int> walls;

	if (cell & 1) {
		walls.push_back(1);
	} else {
		walls.push_back(0);
	}

	if (cell & 2) {
		walls.push_back(1);
	} else {
		walls.push_back(0);
	}

	if (cell & 4) {
		walls.push_back(1);
	} else {
		walls.push_back(0);
	}

	if (cell & 8) {
		walls.push_back(1);
	} else {
		walls.push_back(0);
	}

	return walls;
}

void generate_output(int **cells, int l, int c, int r, std::unordered_map<int, std::pair<int, int> > &robots) {
	int n = l * c;
	int m = TRANSITIONS;
	int s = r;

	std::cout << n << " " << m << " " << s << std::endl;

	// generate transitions for each cell
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++) {
			std::vector<int> walls = decode_walls(cells[i][j]);

			if (walls[EAST] || j == c - 1) {
				// wall to the right or last element => no transition
				std::cout << encode_cells(i, j, l, c) << " ";
			} else {
				// no wall to the right => transition to my next neighbour
				std::cout << encode_cells(i, j + 1, l, c) << " ";
			}

			if (walls[NORTH] || i == 0) {
				// wall above or first line => no transition
				std::cout << encode_cells(i, j, l, c) << " ";
			} else {
				// no wall above => transition to my neighbour above
				std::cout << encode_cells(i - 1, j, l, c) << " ";
			}

			if (walls[WEST] || j == 0) {
				// wall to the left or first element on row => no transition
				std::cout << encode_cells(i, j, l, c) << " ";
			} else {
				// no wall to the left => transition to my previous neighbour
				std::cout << encode_cells(i, j - 1, l, c) << " ";
			}

			if (walls[SOUTH] || i == l - 1) {
				// wall below or last line => no transition
				std::cout << encode_cells(i, j, l, c) << std::endl;
			} else {
				// no wall below => transition to my neighbour below
				std::cout << encode_cells(i + 1, j, l, c) << std::endl;
			}
		}
	}

	// robots become the initial states
	for (int i = 0; i < r; i++) {
		std::cout << encode_cells(robots[i].first, robots[i].second, l, c) << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char *argv[]) {
	int l, c, r;
	int **cells;
	std::unordered_map<int, std::pair<int, int> > robots;

	read_args(l, c, r, &cells, robots);
	generate_output(cells, l, c, r, robots);

	clear_memory(cells, l);
	return 0;
}