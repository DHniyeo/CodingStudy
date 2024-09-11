#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int n, m, k;
vector<vector<int>> board;
vector<vector<int>> attack;
vector<vector<bool>> attacked;
vector<pair<int, int>> path;

// 우하좌상
int dy[8] = { 0,1,0,-1,1,1,-1,-1 };
int dx[8] = { 1,0,-1,0,1,-1,1,-1 };


bool Bfs(pair<int,int> src,pair<int,int> dest) {
	vector<vector<pair<int, int>>> parent(n + 1, vector<pair<int, int>>(m + 1));
	vector<vector<bool>> visited(n + 1, vector<bool>(m + 1, false));
	path = vector<pair<int, int>>();

	queue<pair<int, int>> q;
	q.push(src);
	visited[src.first][src.second] = true;
	bool find = false;

	while (q.empty() == false) {
		auto temp = q.front();
		q.pop();
		int nowY = temp.first;
		int nowX = temp.second;
		

		if (nowY == dest.first && nowX == dest.second) {
			find = true;
			break;
		}

		for (int dir = 0; dir < 4; dir++) {
			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY > n)
				nxtY -= n;
			if (nxtY < 1)
				nxtY += n;
			if (nxtX > m)
				nxtX -= m;
			if (nxtX < 1)
				nxtX += m;

			if (board[nxtY][nxtX] == 0)
				continue;

			if (visited[nxtY][nxtX] == true)
				continue;

			parent[nxtY][nxtX] = { nowY,nowX };

			q.push({ nxtY,nxtX });
			visited[nxtY][nxtX] = true;
		}
	}
	if (find == false)
		return false;

	auto pos = dest;
	
	while (pos != src) {
		path.push_back(parent[pos.first][pos.second]);
		pos = parent[pos.first][pos.second];		
	}

	// 시작점은 뺌
	path.pop_back();

	return true;

	//굳이 안해도됨 길찾기가 아니라서
	//std::reverse(path.begin(), path.end());
}

void Bomb(pair<int, int> src, pair<int, int> dest) {
	
	board[dest.first][dest.second] = std::max(0, board[dest.first][dest.second] - board[src.first][src.second]);

	int nowY = dest.first;
	int nowX = dest.second;

	for (int dir = 0; dir < 8; dir++) {
		int nxtY = nowY + dy[dir];
		int nxtX = nowX + dx[dir];
		if (nxtY > n)
			nxtY -= n;
		if (nxtY < 1)
			nxtY += n;
		if (nxtX > m)
			nxtX -= m;
		if (nxtX < 1)
			nxtX += m;

		if (nxtY == src.first && nxtX == src.second)
			continue;

		if (board[nxtY][nxtX] == 0)
			continue;

		board[nxtY][nxtX] = std::max(0, board[nxtY][nxtX] - (board[src.first][src.second]/2));
		attacked[nxtY][nxtX] = true;
		
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> k;

	board = vector<vector<int>>(n+1, vector<int>(m+1, -1));
	attack = vector<vector<int>>(n+1, vector<int>(m+1, 0));
	

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 1; i <= k; i++) {
		attacked = vector<vector<bool>>(n + 1, vector<bool>(m + 1, false));
		// 남은 포탑 한개면 종료
		int cnt = 0;
		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= m; x++) {
				if (board[y][x] != 0)
					cnt++;
			}
		}
		if (cnt < 2)
			break;

		// 공격자 선정 (가장 약한 포탑)
		int minAttack = INT32_MAX;
		int weakY = -1;
		int weakX = -1;

		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= m; x++) {
				if (board[y][x] == 0)
					continue;

				if (board[y][x] < minAttack) {
					minAttack = board[y][x];
					weakY = y;
					weakX = x;
				}
				else if (board[y][x] == minAttack) {
					if (attack[y][x] > attack[weakY][weakX]) {
						minAttack = board[y][x];
						weakY = y;
						weakX = x;
					}
					else if (attack[y][x] == attack[weakY][weakX]) {
						if (y + x > weakY + weakX) {
							minAttack = board[y][x];
							weakY = y;
							weakX = x;
						}
						else if (y + x == weakY + weakX) {
							if (x > weakX) {
								minAttack = board[y][x];
								weakY = y;
								weakX = x;
							}
						}
					}
				}
			}
		}
		board[weakY][weakX] += (n + m);
		attack[weakY][weakX] = i;

		// 피격자 선정 (가장 강한 포탑)
		int maxAttack = INT32_MIN;
		int strY = -1;
		int strX = -1;
		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= m; x++) {
				if (board[y][x] == 0)
					continue;
				if (y == weakY && x == weakX)
					continue;
				if (board[y][x] > maxAttack) {
					maxAttack = board[y][x];
					strY = y;
					strX = x;
				}
				else if (board[y][x] == maxAttack) {
					if (attack[y][x] < attack[strY][strX]) {
						maxAttack = board[y][x];
						strY = y;
						strX = x;
					}
					else if (attack[y][x] == attack[strY][strX]) {
						if (y + x < strY + strX) {
							maxAttack = board[y][x];
							strY = y;
							strX = x;
						}
						else if (y + x == strY + strX) {
							if (x < strX) {
								maxAttack = board[y][x];
								strY = y;
								strX = x;
							}
						}
					}
				}
			}
		}
		// 공격진행

		// 레이저 공격 최단 경로 찾기
		if (Bfs({ weakY,weakX }, { strY,strX }) == false) {
			// 레이저 경로 없으면 포탑 공격
			Bomb({ weakY,weakX }, { strY,strX });
		}
		else {
			// 레이저 공격!
			board[strY][strX] = std::max(0, board[strY][strX] - board[weakY][weakX]);

			for (auto pos : path) {
				board[pos.first][pos.second] = std::max(0, board[pos.first][pos.second] - (board[weakY][weakX] / 2));
				attacked[pos.first][pos.second] = true;
			}
		}

		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= m; x++) {
				if (board[y][x] == 0)
					continue;
				if (y == weakY && x == weakX)
					continue;
				if (y == strY && x == strX)
					continue;
				if (attacked[y][x] == true)
					continue;

				board[y][x]++;
			}
		}
	}
	int ans = -1;
	for (int y = 1; y <= n; y++) {
		for (int x = 1; x <= m; x++) {
			if (board[y][x] > ans)
				ans = board[y][x];
		}
	}
	cout << ans;
	return 0;
}
