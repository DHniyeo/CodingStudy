#include<iostream>
#include<vector>
#include<queue>
using namespace std;

int p, n, m,c,d;
vector<vector<int>> board;
vector<int> stuns; // 스턴 풀 목록

int dy[4] = {-1,0,1,0};
int dx[4] = {0,1,0,-1};

struct Santa
{
	int id;
	int y;
	int x;
	int dist = -1;
	int score = 0;
	bool dead = false;
	bool stun = false;

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

vector<Santa> s;

int ry, rx;

int main()
{
	cin >> n >> m >> p >> c >> d;

	s = vector<Santa>(p + 1);
	stuns = vector<int>(p+1,0);
	
	cin >> ry>>rx;

	for (int i = 1; i <= p; i++)
	{
		int idx = 0;
		cin >> idx;
		cin >> s[idx].y >> s[idx].x;
		s[idx].id = idx;
	}

	for (int i = 0; i < m; i++)
	{
		for (int idx = 1; idx <= p; idx++)
		{
			if (stuns[idx] > 0)
			{
				stuns[idx]--;
				if (stuns[idx] == 0)
					s[idx].stun = false;
			}
		}

		
		// 루돌프 이동 
		priority_queue<Santa, vector<Santa>, greater<Santa>> pq;
		for (int idx = 1; idx <= p; idx++)
		{
			s[idx].dist = (ry - s[idx].y) * (ry - s[idx].y) + (rx - s[idx].x) * (rx - s[idx].x);
			pq.push(s[idx]);
		}

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

		ry += ddy;
		rx += ddx;

		// 루돌프 이동후 산타 충돌 체크
		for (int idx = 1; idx <= p; idx++)
		{
			if (s[idx].dead)
				continue;
			if (s[idx].y == ry && s[idx].x == rx)
			{
				s[idx].score += c;
				s[idx].y += (c * ddy);
				s[idx].x += (c * ddx);

				// 밀려난후 범위체크
				if (s[idx].y<1 || s[idx].y>n || s[idx].x<1 || s[idx].x>n)
				{
					s[idx].dead = true;
				}
				else
				{
					// 스턴 카운트 증가 
					s[idx].stun = true;
					stuns[idx]+=2;

					for (int j = 1; j <= p; j++)
					{
						if (s[idx].dead)
							continue;
						if (j == idx)
							continue;

						if (s[idx].y == s[j].y && s[idx].x == s[j].x)
						{
							queue<int> q;
							q.push(j);
							while (q.empty() == false)
							{
								int idx2 = q.front();
								q.pop();
								s[idx2].y += ddy;
								s[idx2].x += ddx;
								if (s[idx2].y<1 || s[idx2].y>n || s[idx2].x<1 || s[idx2].x>n)
								{
									s[idx2].dead = true;
								}
								else
								{
									for (int k = 1; k <= p; k++)
									{
										if (s[k].dead)
											continue;
										if (k == idx2)
											continue;

										if (s[idx2].y == s[k].y && s[idx2].x == s[k].x)
										{
											q.push(k);
										}

									}
								}
							}
						}
					}
				}
			}
		}

		// 산타 이동
		for (int idx = 1; idx <= p; idx++)
		{
			// 죽거나 기절이면 스킵
			if (s[idx].dead || s[idx].stun)
				continue;

			// 루돌프 방향으로 이동
			int minIdx = 0;
			int minDist = 0;
			for (int j = 0; j < 4; j++)
			{
				int dis = 0;
				int ny = s[idx].y + dy[j];
				int nx = s[idx].x + dx[j];
				dis = (ry - ny) * (ry - ny) + (rx - nx) * (rx - nx);

				if (j == 0)
				{
					minDist = dis;
				}
				else
				{
					if (minDist > dis)
					{
						minDist = dis;
						minIdx = j;
					}
				}
			}
			s[idx].y += dy[minIdx];
			s[idx].x += dx[minIdx];

			// 루돌프 충돌
			if (s[idx].y == ry || s[idx].x == rx)
			{
				s[idx].score += d;
				int nidx = (minIdx + 2) % 4;
				s[idx].y += d * dy[nidx];
				s[idx].x += d * dx[nidx];
				
				// 다른 산타 충돌 체크
				for (int l = 1; l <= p; l++)
				{
					if (s[idx].dead)
						continue;
					if (l == idx)
						continue;

					if (s[idx].y == s[l].y && s[idx].x == s[l].x)
					{
						queue<int> q;
						q.push(l);
						while (q.empty() == false)
						{
							int idx2 = q.front();
							q.pop();
							s[idx2].y += dy[nidx];
							s[idx2].x += dx[nidx];
							if (s[idx2].y<1 || s[idx2].y>n || s[idx2].x<1 || s[idx2].x>n)
							{
								s[idx2].dead = true;
							}
							else
							{
								for (int k = 1; k <= p; k++)
								{
									if (s[k].dead)
										continue;
									if (k == idx2)
										continue;

									if (s[idx2].y == s[k].y && s[idx2].x == s[k].x)
									{
										q.push(k);
									}

								}
							}
						}
					}
				}

			}
			else
			{
				//산타 충돌 체크
				for (int l = 1; l <= p; l++)
				{
					if (s[idx].dead)
						continue;
					if (l == idx)
						continue;

					if (s[idx].y == s[l].y && s[idx].x == s[l].x)
					{
						queue<int> q;
						q.push(l);
						while (q.empty() == false)
						{
							int idx2 = q.front();
							q.pop();
							s[idx2].y += dy[minIdx];
							s[idx2].x += dx[minIdx];
							if (s[idx2].y<1 || s[idx2].y>n || s[idx2].x<1 || s[idx2].x>n)
							{
								s[idx2].dead = true;
							}
							else
							{
								for (int k = 1; k <= p; k++)
								{
									if (s[k].dead)
										continue;
									if (k == idx2)
										continue;

									if (s[idx2].y == s[k].y && s[idx2].x == s[k].x)
									{
										q.push(k);
									}

								}
							}
						}
					}
				}
			}
			
			
		}

		int cnt = 0;

		for (int idx = 1; idx <= p; idx++)
		{
			if (s[idx].dead)
				cnt++;
		}

		if (cnt == p)
			break;

		for (auto ss : s)
		{
			if (ss.dead)
				continue;
			ss.score++;
		}
	}

	for (int idx3 = 1;idx3<=p;idx3++)
		cout << s[idx3].score << " ";
	
	
	return 0;
}
