#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> board = vector<vector<int>>(6, vector<int>(6, 0));
vector<int> num;
int idx = 1;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };


int k, m;

void rotate(vector<vector<int>>& tBoard, int y, int x, int dir) {

	vector<vector<int>> rBoard(3, vector<int>(3, 0));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			rBoard[i][j] = tBoard[y - 1 + i][x - 1 + j];
		}
	}
	// 90도
	if (dir == 0) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tBoard[y - 1 + i][x - 1 + j] = rBoard[2 - j][i];
			}
		}
	}
	// 180도
	else if (dir == 1) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tBoard[y - 1 + i][x - 1 + j] = rBoard[2 - i][2 - j];
			}
		}
	}
	// 270도
	else if (dir == 2) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tBoard[y - 1 + i][x - 1 + j] = rBoard[j][2 - i];
			}
		}
	}
}

int bfs(vector<vector<int>>& tBoard) {
	int result = 0;
	vector<vector<bool>> visited(6, vector<bool>(6, false));
	for (int y = 1; y <= 5; y++) {
		for (int x = 1; x <= 5; x++) {
			if (visited[y][x] == true)
				continue;
			int cnt = 1;
			queue<pair<int, int>> q;
			q.push({ y,x });
			visited[y][x] = true;
			vector<pair<int, int>> z;
			z.push_back({ y,x });

			while (q.empty() == false) {
				auto temp = q.front();
				q.pop();
				int nowY = temp.first;
				int nowX = temp.second;

				for (int dir = 0; dir < 4; dir++) {
					int nxtY = nowY + dy[dir];
					int nxtX = nowX + dx[dir];

					if (nxtY < 1 || nxtY >5 || nxtX < 1 || nxtX>5)
						continue;
					if (visited[nxtY][nxtX] == true)
						continue;
					if (tBoard[nowY][nowX] != tBoard[nxtY][nxtX])
						continue;

					q.push({ nxtY,nxtX });
					z.push_back({ nxtY,nxtX });
					cnt++;
					visited[nxtY][nxtX] = true;
				}
			}
			if (cnt > 2) {
				result += cnt;

				for (auto n : z) {
					tBoard[n.first][n.second] = 0;
				}
			}
		}
	}

	return result;
}


// 방향작은거 x 작은거 그다음 y작은거
int search() {
	int ans = 0;
	int max = 0;
	int minX = 0;
	int minY = 0;
	int minDir = 0;
	vector<vector<int>> fboard;
	for (int y = 2; y <= 4; y++) {
		for (int x = 2; x <= 4; x++) {
			for (int dir = 0; dir < 3; dir++) {
				vector<vector<int>> tboard = board;
				rotate(tboard, y, x, dir);
				int result = bfs(tboard);

				if (result > max) {
					max = result;
					minX = x;
					minY = y;
					minDir = dir;
					fboard = tboard;
				}
				else if (result == max) {
					if (minDir > dir) {
						max = result;
						minX = x;
						minY = y;
						minDir = dir;
						fboard = tboard;
					}
					else if (minDir == dir) {
						if (minX > x) {
							max = result;
							minX = x;
							minY = y;
							minDir = dir;
							fboard = tboard;
						}
						else if (minX == x) {
							if (minY > y) {
								max = result;
								minX = x;
								minY = y;
								minDir = dir;
								fboard = tboard;
							}
						}
					}
				}
			}
		}
	}
	if (max == 0)
		return 0;
	ans += max;
	board = fboard;

	while (true) {
		for (int y = 1; y <= 5; y++) {
			for (int x = 1; x <= 5; x++) {
				if (board[6 - x][y] == 0)
					board[6 - x][y] = num[idx++];
			}
		}

		int result = bfs(board);
		if (result == 0)
			break;

		ans += result;
	}
	return ans;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);


	cin >> k >> m;

	num = vector<int>(m + 1, 0);

	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 5; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 1; i <= m; i++) {
		cin >> num[i];
	}

	for (int i = 1; i <= k; i++) {
		int result = search();
		if (result == 0)
			break;
		cout << result << " ";
	}

	return 0;
}
