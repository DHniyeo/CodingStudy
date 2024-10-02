#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

struct Group {
	int id;
	int num;
	int cnt = 0;
};

vector<Group> groups(1);
vector<vector<int>> board;
vector<vector<int>> nboard;
vector<vector<bool>> visited;
vector<vector<int>> adj;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int n;
int score = 0;



void Bfs(pair<int,int> pos,int id) {
	visited[pos.first][pos.second] = true;
	queue<pair<int, int>> q;
	nboard[pos.first][pos.second] = id;
	groups[id].cnt++;
	q.push(pos);

	while (q.empty() == false) {
		auto temp = q.front();
		q.pop();
		int nowY = temp.first;
		int nowX = temp.second;

		for (int dir = 0; dir < 4; dir++) {

			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY<1 || nxtX<1 || nxtY>n || nxtX>n)
				continue;
			if (visited[nxtY][nxtX])
				continue;
			if (board[nowY][nowX] != board[nxtY][nxtX])
				continue;

			q.push({ nxtY,nxtX });
			visited[nxtY][nxtX] = true;
			nboard[nxtY][nxtX] = id;
			groups[id].cnt++;
		}
	}
};

void Bfs2(pair<int, int> pos) {
	visited[pos.first][pos.second] = true;
	queue<pair<int, int>> q;
	q.push(pos);

	while (q.empty() == false) {
		auto temp = q.front();
		q.pop();
		int nowY = temp.first;
		int nowX = temp.second;

		for (int dir = 0; dir < 4; dir++) {

			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY<1 || nxtX<1 || nxtY>n || nxtX>n)
				continue;
			if (board[nowY][nowX] != board[nxtY][nxtX]) {
				adj[nboard[nowY][nowX]][nboard[nxtY][nxtX]]++;
				continue;
			}
			if (visited[nxtY][nxtX])
				continue;

			q.push({ nxtY,nxtX });
			visited[nxtY][nxtX] = true;
		}
	}
}


void CrossRotate() {
	// 십자 돌리기
	vector<vector<int>> temp = board;
	for (int y = 1; y <= n; y++) {
		for (int x = 1; x <= n; x++) {
			if (y == n / 2 + 1 || x == n / 2 + 1) {
				board[y][x] = temp[x][n - y + 1];
			}
		}
	}
}

void QuaterRotate(int y,int x) {
	vector<vector<int>> temp(n / 2, vector<int>(n / 2, 0));

	for (int i = 0; i < n / 2; i++) {
		for (int j = 0; j < n / 2; j++) {
			temp[i][j] = board[y + i][x + j];
		}
	}

	for (int i = 0; i < n / 2; i++) {
		for (int j = 0; j < n / 2; j++) {
			board[y + i][x + j] = temp[n / 2 - 1 - j][i];
		}
	}
}

void Debug_board() {
	for (int y = 1; y <= n; y++) {
		for (int x = 1; x <= n; x++) {
			cout << board[y][x];
		}
		cout << endl;
	}
	cout << endl;
}

void Rotate() {
	
	CrossRotate();
	QuaterRotate(1,1);
	QuaterRotate(n/2+2,1);
	QuaterRotate(n/2+2,n/2+2);
	QuaterRotate(1,n/2+2);
}

int GetScore() {
	int sum = 0;
	for (int i = 1; i <= adj.size() - 1; i++) {
		for (int j = i+1; j <= adj.size() - 1; j++) {
			sum += ((groups[i].cnt + groups[j].cnt) * groups[i].num * groups[j].num * adj[i][j]);
		}
	}
	return sum;
}



int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n;

	board = vector<vector<int>>(n + 1, vector<int>(n + 1));
	for (int y = 1; y <= n; y++) {
		for (int x = 1; x <= n; x++) {
			cin >> board[y][x];
		}
	}


	for (int t = 1; t <= 4; t++) {
		nboard = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
		visited = vector<vector<bool>>(n + 1, vector<bool>(n + 1, false));
		groups = vector<Group>(1);

		int idx = 1;

		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= n; x++) {
				if (visited[y][x] == false) {
					groups.push_back({ idx,board[y][x] });
					Bfs({ y,x }, idx);
					idx++;
				}
			}
		}

		visited = vector<vector<bool>>(n + 1, vector<bool>(n + 1, false));
		adj = vector<vector<int>>(groups.size(), vector<int>(groups.size(), 0));

		for (int y = 1; y <= n; y++) {
			for (int x = 1; x <= n; x++) {
				if (visited[y][x] == false) {
					Bfs2({ y,x });
				}
			}
		}

		
		Rotate();
		//Debug_board();
		score += GetScore();
		int a = 0;
	}
	cout << score;
}
