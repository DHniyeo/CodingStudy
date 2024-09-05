#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int r, c, k;
int ans;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

vector<vector<int>> board;
// 출구 좌표 
vector<pair<int,int>> exitPos;

bool isInBoard(int y,int x) {
	return (y >= 0 && y <= r + 2 && x >= 1 && x <= c);
}

bool canGo(int y, int x) {
	if (isInBoard(y, x) == false)
		return false;

	for (int dir = 0; dir < 4; dir++) {
		int ny = y + dy[dir];
		int nx = x + dx[dir];
		if (isInBoard(ny, nx) == false)
			return false;
		if (board[ny][nx] != 0)
			return false;
	}
	return true;
}

bool moveDown(int y, int x) {
	return canGo(y+1, x);
}

bool moveRight(int y, int x) {
	return canGo(y, x+1);
}

bool moveLeft(int y, int x) {
	return canGo(y, x-1);
}

bool moveRightDown(int y,int x) {
	if (moveRight(y, x) == false) {
		return false;
	}
	return moveDown(y,x+1);
}

bool moveLeftDown(int y, int x) {
	if (moveLeft(y, x) == false) {
		return false;
	}
	return moveDown(y, x -1);
}

void Bfs(int y, int x) {
	int maxY = 0;

	queue<pair<int, int>> q;
	vector<vector<bool>> visited = vector<vector<bool>>(r + 3, vector<bool>(c + 1, false));

	visited[y][x] = true;
	q.push({ y,x });

	while (q.empty() == false) {
		auto temp = q.front();
		q.pop();
		int nowY = temp.first;
		int nowX = temp.second;
		visited[nowY][nowX] = true;

		if (nowY > maxY)
			maxY = nowY;

		for (int dir = 0; dir < 4; dir++) {
			int nextY = nowY + dy[dir];
			int nextX = nowX + dx[dir];

			if (isInBoard(nextY, nextX) == false)
				continue;
			if (nextY < 2)
				continue;
			if (board[nextY][nextX] == 0)
				continue;
			if (visited[nextY][nextX] == true)
				continue;
			if (board[nextY][nextX] != board[nowY][nowX]) {
				if (nowY == exitPos[board[nowY][nowX]].first && nowX == exitPos[board[nowY][nowX]].second)
					q.push({ nextY,nextX });
			}
			else {
				q.push({ nextY,nextX });
			}
		}
	}

	ans += maxY;
	ans -= 2;
}

void applyInBoard(int y, int x, int i,int d) {
	if (canGo(y, x) == false)
		return;
	if (y < 4) {
		// 보드 위로 나오면 초기화
		board = vector<vector<int>>(r + 3, vector<int>(c + 1, 0));
		return;
	}

	board[y][x] = i;

	for (int dir = 0; dir < 4; dir++) {
		int ny = y + dy[dir];
		int nx = x + dx[dir];

		board[ny][nx] = i;
	}

	exitPos[i] = { y + dy[d], x + dx[d] };
	Bfs(y, x);
}



int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> r >> c >> k;

	board = vector<vector<int>>(r + 3, vector<int>(c + 1, 0));
	exitPos = vector<pair<int,int>>(k + 1);

	for (int i = 1; i <= k; i++) {
		int s, d;
		cin >> s >> d;

		int y = 1;
		int x = s;

		while (true) {
			// 아래
			if (moveDown(y,x)) {
				y++;
			}
			// 왼쪽 아래
			else if (moveLeftDown(y,x)) {
				x--;
				y++;
				d = (d + 3) % 4;
			}
			// 오른쪽 아래
			else if (moveRightDown(y,x)) {
				x++;
				y++;
				d = (d + 1) % 4;
			}
			else {
				break;
			}
		}
		// 보드에 그리기
		applyInBoard(y, x, i, d);

	}
	cout << ans;

	return 0;
}
