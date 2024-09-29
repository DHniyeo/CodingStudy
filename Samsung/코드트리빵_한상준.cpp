#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

int dy[4] = { -1,0,0,1 };
int dx[4] = { 0,-1,1,0 };

struct Player {
	int y = 0;
	int x = 0;
	bool findConvi = false;
	bool start = false;
	pair<int, int> convi;
};

int n, m;
vector<vector<int>> board;
//vector<vector<bool>> visited;
//vector<pair<int,int>> convi;
vector<pair<int,int>> base;
vector<Player> players;

bool FindPath(vector<pair<int,int>>& path, pair<int,int> src,pair<int,int> dest) {
	queue<pair<int, int>> q;
	path = vector<pair<int, int>>();
	vector<vector<bool>> visited(n + 1, vector<bool>(n + 1, false));
	visited[src.first][src.second] = true;
	q.push({ src.first,src.second });
	vector<vector<pair<int, int>>> parent(n+1,vector<pair<int,int>>(n+1));
	parent[src.first][src.second] = src;

	while (q.empty() == false)
	{
		auto temp = q.front();
		q.pop();
		int nowY = temp.first;
		int nowX = temp.second;
		int flag = false;
		for (int dir = 0;dir<4;dir++)
		{
			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY<1 || nxtY > n || nxtX<1 || nxtX> n)
				continue;
			if (board[nxtY][nxtX] == 2 || board[nxtY][nxtX] == 4)
				continue;
			if (visited[nxtY][nxtX] == true)
				continue;

			visited[nxtY][nxtX] = true;
			q.push({ nxtY,nxtX });
			parent[nxtY][nxtX] = { nowY,nowX };
			if (nxtY == dest.first && nxtX == dest.second)
			{
				flag = true;
				break;
			}
				
		}
		if (flag)
			break;
	}
	if (parent[dest.first][dest.second].first == 0 && parent[dest.first][dest.second].second == 0)
	{
		return false;
	}
	path.push_back(dest);
	auto temp = dest;

	while (temp != src)
	{
		temp = parent[temp.first][temp.second];
		path.push_back(temp);
	}

	std::reverse(path.begin(), path.end());
	return true;
}

void Move(Player& player) {
	vector<pair<int, int>> path;
	FindPath(path, { player.y,player.x }, player.convi);
	player.y = path[1].first;
	player.x = path[1].second;
}

void GoBase(Player& player) {
	int minDist = INT32_MAX;
	pair<int, int> minConvi = {0,0};
	vector<pair<int, int>> path;
	for (auto b : base) {
		if (board[b.first][b.second] == 2)
			continue;
		if (false == FindPath(path, { player.convi.first,player.convi.second }, b))
			continue;
		int dist = path.size();
		if (dist <minDist) {
			minDist = dist;
			minConvi = b;
		}
		else if (dist == minDist)
		{
			if (b.first <minConvi.first)
			{
				minDist = dist;
				minConvi = b;
			}
			else if (b.first == minConvi.first)
			{
				if (b.second < minConvi.second)
				{
					minDist = dist;
					minConvi = b;
				}
			}
		}
	}
	// 이제 여기 아무도 못감
	player.y = minConvi.first;
	player.x = minConvi.second;
	board[minConvi.first][minConvi.second] = 2;
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m;

	board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	//visited = vector<vector<bool>>(n + 1, vector<bool>(n + 1, false));
	players = vector<Player>(m + 1);


	for (int i = 1;i<=n;i++){
		for (int j = 1; j <= n; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1){
				base.push_back({ i,j });
			}
		}
	}

	for (int i = 1;i<=m;i++) {
		int temp1, temp2;
		cin >> temp1 >> temp2;
		board[temp1][temp2] = 3;
		players[i].convi = { temp1,temp2 };
	}

	int t = 0;

	while (true) {
		t++;

		//격자에사람들 이동

		for (int i = 1;i<=m;i++) {
			if (players[i].start == false || players[i].findConvi == true)
				continue;
			Move(players[i]);
		}
		//편의점 도착한사람있으면 해당 편의점 이동불가
		for (int i = 1;i<=m;i++) {
			if (players[i].start == false || players[i].findConvi == true)
				continue;
			if (players[i].y == players[i].convi.first && players[i].x == players[i].convi.second) {
				players[i].findConvi = true;
				board[players[i].y][players[i].x] = 4;
			}

		}

		// t<=m 이면 베이스캠프 들어감
		if (t<=m) {
			players[t].start = true;
			GoBase(players[t]);
		}
		int cnt = 0;
		// 모두 편의점 찾으면 종료
		for (int i = 1;i<=m;i++) {
			if (players[i].findConvi == true)
				cnt++;
		}
		if (cnt == m)
			break;

	}


	cout << t;
}
