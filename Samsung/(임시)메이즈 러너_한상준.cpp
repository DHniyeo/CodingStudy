#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

struct person {
	int y = 0;
	int x = 0;

	bool is_escape = false;
};

vector<vector<int>> board;
//vector<person> players;

int n, m, k;
bool exitCnt = false;

int exitY;
int exitX;
int ans = 0;

void UpdateExitPos() {
	for (int y = 1; y <= n; y++) {
		for (int x = 1; x <= n; x++) {
			if (board[y][x] == -1) {
				exitY = y;
				exitX = x;
			}
		}
	}
}

// 좌상단
void FindRect(int& resultY,int& resultX,int& resultSize) {
	// 사각형 최소 크기는 2 최대는 n
	int minX = 1;
	int minY = 1;
	int minSize = n+1;

	for (int size = 2; size <= n; size++) {
		for (int y = 1; y <= n - size + 1; y++) {
			for (int x = 1; x <= n - size + 1; x++) {
				bool checkExit = false;
				bool checkPlayer = false;

				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						if (board[y+i][x+j] < -1)
							checkPlayer = true;
												
						if (y + i == exitY && x + j == exitX)
							checkExit = true;
					}
				}

				if (checkPlayer == true && checkExit == true) {
					if (size < minSize) {
						minX = x;
						minY = y;
						minSize = size;
					}
					else if (size == minSize) {
						if (y < minY) {
							minX = x;
							minY = y;
							minSize = size;
						}
						else if (y == minY) {
							if (x < minX) {
								minX = x;
								minY = y;
								minSize = size;
							}
						}
					}
				}
			}
		}
	}
	resultY = minY;
	resultX = minX;
	resultSize = minSize;

	if (resultSize == n + 1)
		exitCnt = true;
}

void Rotate(int y, int x, int size) {
	vector<vector<int>> temp(size,vector<int>(size));

	for (int i = y; i <= y + size - 1; i++) {
		for (int j = x; j <= x + size - 1; j++) {
			temp[i - y][j - x] = board[i][j];
		}
	}

	for (int i = y; i <= y + size - 1; i++) {
		for (int j = x; j <= x + size - 1; j++) {
			board[i][j] = temp[size - 1 + (x - j)][i-y];

			if (board[i][j] > 0)
				board[i][j]--;
		}
	}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> k;

	board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	//players = vector<person>(m+1);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 1; i <= m; i++) {
		int t1, t2;

		cin >> t1 >> t2;

		//players[i] = { t1,t2 };
		board[t1][t2] = -1 - i;
	}

	cin >> exitY >> exitX;
	board[exitY][exitX] = -1;

	for (int t = 1; t <= k; t++) {
		int cnt = 0;
		vector<bool> visited(m,false);
		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= n; x++) {
				if (board[y][x] >= -1)
					continue;
				if (visited[std::abs(board[y][x])-2])
					continue;
				visited[std::abs(board[y][x]) - 2] = true;

				int nowY = y;
				int nowX = x;

				int dist = std::abs(exitY - nowY) + std::abs(exitX - nowX);

				int newY = y;
				int newX = x;

				for (int dir = 0; dir < 4; dir++) {
					int nxtY = nowY + dy[dir];
					int nxtX = nowX + dx[dir];

					if (nxtY > n || nxtY < 1 || nxtX >n || nxtX < 1)
						continue;
					if (board[nxtY][nxtX] >0)
						continue;

					int newDist = std::abs(exitY - nxtY) + std::abs(exitX - nxtX);

					if (newDist < dist) {
						//if (board[nxtY][nxtX] != -1)
						//	board[nxtY][nxtX] = board[nowY][nowX];

						//board[nowY][nowX] = 0;
						//cnt++;
						dist = newDist;
						newY = nxtY;
						newX = nxtX;

					}
				}

				if (newY != y && newX != x) {
					cnt++;
					if (board[newY][newX] != -1)
						board[newY][newX] = board[y][x];
					board[y][x] = 0;
				}
			}
		}
		ans += cnt;
		int rectY = 0;
		int rectX = 0;
		int rectSize = 0;
		FindRect(rectY, rectX, rectSize);
		if (exitCnt)
			break;
		Rotate(rectY, rectX, rectSize);
		UpdateExitPos();

	}

	cout << ans<<"\n";
	cout << exitY << " " << exitX;
	



}
