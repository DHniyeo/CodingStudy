#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Knight
{
	int r;
	int c;
	int h;
	int w;
	int k;
	int maxK; // 초기 체력
};

// 위 오른 아래 왼
int dx[4] = { 0,1,0,-1 };
int dy[4] = { -1,0,1,0 };


int l, n, q;
int result;

vector<vector<int>> board;
vector<Knight> knights;

void Move(int i,int d)
{
	vector<Knight> nKnights = knights;
	vector<bool> isMoved(n + 1, false); // 한번만 움직여야함
	vector<int> damage(n + 1, 0);
	queue<int> q;
	isMoved[i] = true;
	q.push(i);

	while (q.empty() == false)
	{
		int idx = q.front();
		q.pop();

		nKnights[idx].r += dy[d];
		nKnights[idx].c += dx[d];

		// 이동 후 체스판 범위 체크
		if (nKnights[idx].r<1 || nKnights[idx].c<1 || nKnights[idx].r + nKnights[idx].h - 1>l || nKnights[idx].c + nKnights[idx].w - 1>l)
			return;

		// 이동 후 벽 or 함정 체크
		for (int j = nKnights[idx].r;j<= nKnights[idx].r + nKnights[idx].h - 1;j++)
		{
			for (int m = nKnights[idx].c;m<= nKnights[idx].c + nKnights[idx].w - 1;m++)
			{
				if (board[j][m] == 1)
					damage[idx]++;
				if (board[j][m] == 2)
					return;
			}

		}

		// 이동 후 겹치는 기사 체크 겹치는 기사는 이동(큐에 넣기)
		for (int j = 1;j<=n;j++)
		{
			if (isMoved[j])
				continue;
			if (knights[j].k<=0 )
				continue;
			if (knights[j].r > nKnights[idx].r + knights[idx].h - 1 || nKnights[idx].r > knights[j].r + knights[j].h - 1)
				continue;
			if (knights[j].c > nKnights[idx].c + knights[idx].w - 1 || nKnights[idx].c > knights[j].c + knights[j].w - 1)
				continue;

			
			q.push(j);
			isMoved[j] = true;
		}

	}
	// 명령받은 기사는 데미지 없음
	damage[i] = 0;

	// 받은 데미지 적용
	for (int j = 1;j<=n;j++)
	{
		nKnights[j].k -= damage[j];
		knights = nKnights;
	}
}


int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> l >> n >> q;

	board = vector<vector<int>>(l+1, vector<int>(l+1,-1));
	
	knights = vector<Knight>(n + 1);

	for (int i = 1; i <= l; i++)
		for (int j = 1; j <= l; j++)
			cin >> board[i][j];

	for (int i = 1;i<=n;i++)
	{
		cin >> knights[i].r >> knights[i].c >> knights[i].h >> knights[i].w >> knights[i].k;

		knights[i].maxK = knights[i].k;
	}

	for (int j = 0;j<q;j++)
	{
		int i, d;
		cin >> i >> d;
		if (knights[i].k <= 0)
			continue;
		Move(i, d);
	}

	// 살아있는 기사 만 최대체력 비교해서 더하기
	for (int j = 1;j<=n;j++)
	{
		if (knights[j].k<=0)
			continue;

		result += (knights[j].maxK - knights[j].k);
	}

	cout << result;

	return 0;
}


