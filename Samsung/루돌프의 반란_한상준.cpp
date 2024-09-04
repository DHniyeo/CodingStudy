#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Santa
{
	int id;
	int y;
	int x;
	int dist = -1;
	int score = 0;
	bool dead = false;
	int stun = 0;

	bool operator<(const Santa s) const
	{
		if (dist == s.dist)
		{
			if (y == s.y)
			{
				return x > s.x;
			}
			else
				return y > s.y;
		}
		else
		{
			return dist < s.dist;
		}
	}

	bool operator>(const Santa s) const
	{
		if (dist == s.dist)
		{
			if (y == s.y)
			{
				return x < s.x;
			}
			else
				return y < s.y;
		}
		else
		{
			return dist > s.dist;
		}
	}
};

int p, n, m, c, d;
vector<vector<int>> board;
vector<Santa> s;

int ry, rx;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

bool IsInBoard(int y, int x)
{
	return 1 <= y && y <= n && 1 <= x && x <= n;
}


int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> p >> c >> d;

	s = vector<Santa>(p + 1);
	board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));

	cin >> ry >> rx;

	board[ry][rx] = -1;

	for (int i = 1; i <= p; i++)
	{
		int idx = 0;
		cin >> idx;
		cin >> s[idx].y >> s[idx].x;
		s[idx].id = idx;
		board[s[idx].y][s[idx].x] = idx;
	}



	for (int i = 1; i <= m; i++)
	{
		priority_queue<Santa, vector<Santa>, greater<Santa>> pq;
		for (int idx = 1; idx <= p; idx++)
		{
			if (s[idx].dead)
				continue;

			s[idx].dist = (ry - s[idx].y) * (ry - s[idx].y) + (rx - s[idx].x) * (rx - s[idx].x);
			pq.push(s[idx]);
		}
		// 다죽으면 종료
		if (pq.empty() == false)
		{

			// 루돌프 이동
			auto s1 = pq.top();

			int ddy;
			if (s1.y - ry == 0)
				ddy = 0;
			else
				ddy = (s1.y - ry) / std::abs(s1.y - ry);

			int ddx;
			if (s1.x - rx == 0)
				ddx = 0;
			else
				ddx = (s1.x - rx) / std::abs(s1.x - rx);

			board[ry][rx] = 0;
			ry += ddy;
			rx += ddx;
			board[ry][rx] = -1;

			if (ry == s1.y && rx == s1.x)
			{
				s[s1.id].score += c;
				s[s1.id].y += (c * ddy);
				s[s1.id].x += (c * ddx);

				// 밀리고 보드 안
				if (IsInBoard(s[s1.id].y, s[s1.id].x))
				{
					s[s1.id].stun = i + 1;
					// 밀렸는데 겹침
					if (board[s[s1.id].y][s[s1.id].x] != 0)
					{
						queue<int> q;
						q.push(board[s[s1.id].y][s[s1.id].x]);
						board[s[s1.id].y][s[s1.id].x] = s1.id;

						while (q.empty() == false)
						{
							int i1 = q.front();
							q.pop();

							s[i1].y += ddy;
							s[i1].x += ddx;

							if (IsInBoard(s[i1].y, s[i1].x))
							{

								if (board[s[i1].y][s[i1].x] != 0)
								{
									q.push(board[s[i1].y][s[i1].x]);
									board[s[i1].y][s[i1].x] = s[i1].id;
								}
								else
								{
									board[s[i1].y][s[i1].x] = s[i1].id;
								}
							}
							else
							{
								s[i1].dead = true;
							}
						}
					}
					// 밀렸는데 안겹침
					else
					{
						board[s[s1.id].y][s[s1.id].x] = s1.id;
					}
				}
				// 보드 밖
				else
				{
					s[s1.id].dead = true;
				}
			}
		}
		//산타 움직이기

		for (int idx = 1; idx <= p; idx++)
		{
			if (s[idx].dead || s[idx].stun >= i)
				continue;

			int minDist = (s[idx].y - ry) * (s[idx].y - ry) + (s[idx].x - rx) * (s[idx].x - rx);
			int minDir = -1;

			for (int dir = 0; dir < 4; dir++)
			{
				int ny = s[idx].y + dy[dir];
				int nx = s[idx].x + dx[dir];

				if (IsInBoard(ny, nx) == false || board[ny][nx] > 0)
					continue;

				int dist = (ny - ry) * (ny - ry) + (nx - rx) * (nx - rx);
				if (dist < minDist)
				{
					minDist = dist;
					minDir = dir;
				}
			}

			if (minDir != -1)
			{
				int ny = s[idx].y + dy[minDir];
				int nx = s[idx].x + dx[minDir];


				// 산타이동 루돌프 충돌 밀려남
				if (ny == ry && nx == rx)
				{
					s[idx].stun = i + 1;

					board[s[idx].y][s[idx].x] = 0;
					s[idx].y = ny + (-dy[minDir]) * d;
					s[idx].x = nx + (-dx[minDir]) * d;

					// 밀려서 원래 자기 위치
					if (d == 1)
					{
						s[idx].score += d;
						board[s[idx].y][s[idx].x] = idx;
					}
					else
					{
						s[idx].score += d;
						// 밀려나고 보드안
						if (IsInBoard(s[idx].y, s[idx].x))
						{
							// 밀려나고 다른 산타 충돌
							if (board[s[idx].y][s[idx].x] > 0)
							{
								queue<int> q;
								q.push(board[s[idx].y][s[idx].x]);
								board[s[idx].y][s[idx].x] = idx;

								while (q.empty() == false)
								{
									int i1 = q.front();
									q.pop();

									s[i1].y += (-dy[minDir]);
									s[i1].x += (-dx[minDir]);

									// 밀렸는데 보드안
									if (IsInBoard(s[i1].y, s[i1].x))
									{
										//충돌
										if (board[s[i1].y][s[i1].x] > 0)
										{
											q.push(board[s[i1].y][s[i1].x]);
											board[s[i1].y][s[i1].x] = s[i1].id;
										}
										//충돌 안함
										else
										{
											board[s[i1].y][s[i1].x] = s[i1].id;
										}
									}
									// 밀렸는데 보드 밖
									else
									{
										s[i1].dead = true;
									}
								}
							}
							// 다른산타 충돌 X
							else
							{
								board[s[idx].y][s[idx].x] = idx;
							}
						}
						// 보드 밖
						else
						{
							s[idx].dead = true;
						}
					}
				}
				else
				{
					board[s[idx].y][s[idx].x] = 0;
					s[idx].y = ny;
					s[idx].x = nx;
					board[ny][nx] = idx;
				}
			}
		}
		// 살아있으면 1점
		for (int idx = 1; idx <= p; idx++)
		{
			if (s[idx].dead)
				continue;
			s[idx].score++;
		}

	}

	for (int i = 1; i <= p; i++)
		cout << s[i].score << " ";

	return 0;
}
