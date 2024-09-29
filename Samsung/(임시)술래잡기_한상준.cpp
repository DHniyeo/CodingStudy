#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };
// 상우하좌

struct Player
{
	int y;
	int x;
	int d; // 1 좌우 2 상하
	int dir = 0;
	bool survive = true;
};

int n, m, h, k;

vector<Player> players;



int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> h >> k;

	players = vector<Player>(m + 1);

	for (int i =1;i<=m;i++)
	{
		cin >> players[i].y >> players[i].x >> players[i].d;
		if (players[i].d==1)
		{
			players[i].dir = 1;
		}
		else
		{
			players[i].dir = 2;
		}
	}

	// 플레이어 이동



	// 술래 이동


	// 플레이어 잡기


}
