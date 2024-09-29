#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

struct Player
{
	int x;
	int y;
	int d;
	int s;
	int score = 0;
	int g = 0;
};

int dy[4] = {-1,0,1,0  };
int dx[4] = { 0,1,0,-1 };

int n, m, k;

vector<vector<vector<int>>> guns;
//vector<vector<int>> board;
vector<Player> players;

void Move(int idx)
{
	int nowY = players[idx].y;
	int nowX = players[idx].x;

	int nxtY = nowY + dy[players[idx].d];
	int nxtX = nowX + dx[players[idx].d];
	// 범위 벗어나면 반대편으로 이동
	if (nxtY>n || nxtY < 1 || nxtX>n || nxtX < 1)
	{
		players[idx].d = (players[idx].d + 2) % 4;
		nxtY = nowY + dy[players[idx].d];
		nxtX = nowX + dx[players[idx].d];
	}
	int cIdx = -1;
	
	for (int i =1;i<=m;i++)
	{
		if (i == idx)
			continue;
		if (players[i].y == nxtY && players[i].x == nxtX)
		{
			cIdx = i;
			break;
		}
	}
	// 이동후 플레이어가 있는경우
	if (cIdx != -1)
	{
		int winner;
		int loser;
		// 원래 있던 플레이어가 이김
		if (players[cIdx].s + players[cIdx].g> players[idx].s + players[idx].g)
		{
			winner = cIdx;
			loser = idx;
		}
		// 새로운 플레이어가 이김
		else if (players[cIdx].s + players[cIdx].g == players[idx].s + players[idx].g)
		{
			if (players[cIdx].s> players[idx].s)
			{
				winner = cIdx;
				loser = idx;
			}
			else
			{
				winner = idx;
				loser = cIdx;
			}
		}
		else
		{
			winner = idx;
			loser = cIdx;
		}
		players[winner].score += ((players[winner].s + players[winner].g) - (players[loser].s + players[loser].g));
		//진플레이가 총을 가지고있으면
		if (players[loser].g != 0)
		{
			///////////////
			//guns[players[loser].y][players[loser].x].push_back(players[loser].g);
			guns[nxtY][nxtX].push_back(players[loser].g);
			players[loser].g = 0;
			//std::sort(guns[players[loser].y][players[loser].x].begin(), guns[players[loser].y][players[loser].x].end());
			std::sort(guns[nxtY][nxtX].begin(), guns[nxtY][nxtX].end());

		}
		// 총이 바닥에있으면 이긴플레이가 주움
		if (guns[nxtY][nxtX].size() != 0)
		{
			if (guns[nxtY][nxtX].back() > players[winner].g)
			{
				// 총이 없으면 그냥 집음
				if (players[winner].g == 0)
				{
					players[winner].g = guns[nxtY][nxtX].back();
					guns[nxtY][nxtX].pop_back();
				}
				// 이미 총이 있으면 버리고 주움
				else
				{
					int playerG = players[winner].g;
					players[winner].g = guns[nxtY][nxtX].back();
					guns[nxtY][nxtX].pop_back();
					guns[nxtY][nxtX].push_back(playerG);
					std::sort(guns[nxtY][nxtX].begin(), guns[nxtY][nxtX].end());
				}
			}
		}
		// 이긴플레이어
		players[winner].y = nxtY;
		players[winner].x = nxtX;
		// 진플레이어 이동
		for (int di = 0;di<4;di++)
		{
			int dir = (players[loser].d + di) % 4;
			int lNxtY = nxtY + dy[dir];
			int lNxtX = nxtX + dx[dir];

			if (lNxtY > n || lNxtY<1 || lNxtX > n || lNxtX < 1)
				continue;
			int flag = false;
			for (int j = 1;j<=m;j++)
			{
				if (j == loser || j == winner)
					continue;
				if (lNxtY == players[j].y&&lNxtX == players[j].x)
				{
					flag = true;
					break;
				}
			}
			if (flag)
				continue;
			players[loser].y = lNxtY;
			players[loser].x = lNxtX;
			players[loser].d = dir;

			if (guns[lNxtY][lNxtX].size() != 0)
			{
				players[loser].g = guns[lNxtY][lNxtX].back();
				guns[lNxtY][lNxtX].pop_back();
			}

			return;
		}

	}
	// 이동후 플레이어가 없는 경우
	else
	{
		// 총줍기
		// 총이 바닥에있으면
		if (guns[nxtY][nxtX].size()!=0)
		{
			if (guns[nxtY][nxtX].back()>players[idx].g)
			{
				// 총이 없으면 그냥 집음
				if (players[idx].g == 0)
				{
					players[idx].g = guns[nxtY][nxtX].back();
					guns[nxtY][nxtX].pop_back();
				}
				// 이미 총이 있으면 버리고 주움
				else
				{
					int playerG = players[idx].g;
					players[idx].g = guns[nxtY][nxtX].back();
					guns[nxtY][nxtX].pop_back();
					guns[nxtY][nxtX].push_back(playerG);
					std::sort(guns[nxtY][nxtX].begin(), guns[nxtY][nxtX].end());
				}
			}
		}
		players[idx].y = nxtY;
		players[idx].x = nxtX;
	}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> k;
	guns = vector<vector<vector<int>>>(n + 1, vector<vector<int>>(n + 1));
	//board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	players = vector<Player>(m + 1);

	for (int i = 1;i<=n;i++)
	{
		for (int j =1 ;j<=n;j++)
		{
			int temp;
			cin >> temp;
			if (temp != 0) {
				guns[i][j].push_back(temp);
			}
		}
	}


	for (int i = 1;i<=m;i++)
	{
		cin >> players[i].y >> players[i].x >> players[i].d >> players[i].s;
	}

	for (int r =1;r<=k;r++)
	{
		for (int idx = 1;idx<=m;idx++)
		{
			Move(idx);
		}

	}

	for (int i = 1;i<=m;i++)
	{
		cout << players[i].score << " ";
	}
	
}
