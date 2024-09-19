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
	int same = 0;
};

vector<vector<int>> board;
vector<person> players;

int n, m, k;
int exitY;
int exitX;
int ans = 0;


bool FindRectRotate() {
	int minSize = n+1;
	int minY = 1;
	int minX = 1;
	for (int size = 2; size <= n; size++) {
		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= n; x++) {
				if (y + size - 1 > n || x + size - 1 > n)
					continue;

				if (!((exitY >= y && exitY <= y + size - 1) && (exitX >= x && exitX <= x + size - 1)))
					continue;
				bool check = false;
				for (int idx = 1; idx <= m; idx++) {
					if (players[idx].is_escape)
						continue;
					if ((players[idx].y >= y && players[idx].y <= y + size - 1) && (players[idx].x >= x && players[idx].x <= x + size - 1)) {
						check = true;
						break;
					}		
				}
				if (check == false)
					continue;
				if (size < minSize) {
					minSize = size;
					minY = y;
					minX = x;
				}
				else if (size == minSize) {
					if (y < minY) {
						minSize = size;
						minY = y;
						minX = x;
					}
					else if (y == minY) {
						if (x < minX) {
							minSize = size;
							minY = y;
							minX = x;
						}
					}
				}

			}
		}
	}
	// 사각형 못찾으면
	if (minSize == n + 1)
		return false;

	for (int idx = 1; idx <= m; idx++) {
		players[idx].same = 0;
	}

	// 보드 회전
	vector<vector<int>> temp(minSize, vector<int>(minSize, 0));
	for (int idx = 1; idx <= m; idx++) {
		if (players[idx].is_escape)
			continue;

		if (!(players[idx].x >= minX && players[idx].x <= minX + minSize - 1 && players[idx].y >= minY && players[idx].y <= minY + minSize - 1))
			continue;
		if (board[players[idx].y][players[idx].x] == 0)
			board[players[idx].y][players[idx].x] = -idx;
		else
			players[idx].same = -board[players[idx].y][players[idx].x];
	}
	board[exitY][exitX] = -15;
	for (int y = minY; y <= minY + minSize - 1; y++) {
		for (int x = minX; x <= minX + minSize - 1; x++) {
			temp[y - minY][x-minX] = board[y][x];			
		}
	}



	

	// player 좌표와 출구좌표 회전
	for (int y = 0; y < minSize; y++) {
		for (int x = 0; x < minSize; x++) {
			board[minY+y][minX+x] = temp[minSize-x-1][y];
			if (board[minY + y][minX + x] > 0)
				board[minY + y][minX + x]--;

			if (board[minY + y][minX + x]==-15) {
				exitY = minY + y;
				exitX = minX + x;
				board[minY + y][minX + x] = 0;
				continue;
			}

			if (board[minY + y][minX + x] < 0) {
				players[-board[minY + y][minX + x]].y = minY + y;
				players[-board[minY + y][minX + x]].x = minX + x;
				board[minY + y][minX + x] = 0;
			}
		}
	}

	for (int idx = 1; idx <= m; idx++) {
		if (players[idx].is_escape)
			continue;
		if (players[idx].same == 0)
			continue;
		players[idx].y = players[players[idx].same].y;
		players[idx].x = players[players[idx].same].x;
	}

	return true;
}



int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> k;

	board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	players = vector<person>(m+1);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 1; i <= m; i++) {
		int t1, t2;
		cin >> t1 >> t2;
		players[i] = { t1,t2 };
	}

	cin >> exitY >> exitX;

	for (int t = 1; t <= k; t++) {

		// player 이동
		int cnt = 0;
		for (int idx = 1; idx <= m; idx++) {
			if (players[idx].is_escape)
				continue;

			int minDist = std::abs(players[idx].y - exitY) + std::abs(players[idx].x - exitX);
			int nowX = players[idx].x;
			int nowY = players[idx].y;

			for (int dir = 0; dir < 4; dir++) {
				int nxtY = nowY + dy[dir];
				int nxtX = nowX + dx[dir];

				if (nxtY<1 || nxtY>n || nxtX<1 || nxtX >n)
					continue;
				if (board[nxtY][nxtX] > 0)
					continue;

				int dist = std::abs(nxtY - exitY) + std::abs(nxtX - exitX);

				if (dist < minDist) {
					minDist = dist;
					players[idx].y = nxtY;
					players[idx].x = nxtX;
				}
			}
			if (nowY != players[idx].y || nowX != players[idx].x)
				cnt++;

			if (players[idx].y == exitY && players[idx].x == exitX)
				players[idx].is_escape = true;

		}
		ans += cnt;

		// 정사각형 찾고 회전
		if (FindRectRotate() == false)
			break;

		
	}

	cout << ans<<"\n";
	cout << exitY << " " << exitX;
	



}
