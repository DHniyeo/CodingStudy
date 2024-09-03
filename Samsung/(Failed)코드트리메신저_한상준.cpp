#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int n, q;

vector<int> parent;
vector<int> auth;
vector<bool> noti;
vector<int> cache;
int ans = 0;

int Search(int idx,int depth)
{
	int sum = 0;
	vector<int> child;

	for (int i = 1; i <= n; i++)
	{
		if (parent[i] == idx)
			child.push_back(i);
	}

	for (int n : child)
	{
		if (noti[n])
			sum += Search(n, depth + 1);
		
	}

	if (depth<=auth[idx])
		return sum + 1;
	else
		return sum;
}


int main()
{
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	cin >> n >> q;

	parent = vector<int>(n + 1);
	auth = vector<int>(n + 1);
	noti = vector<bool>(n + 1, true);

	for (int i = 0; i < q; i++)
	{
		int order;
		cin >> order;

		if (order == 100)
		{
			for (int j = 1; j <= n; j++)
			{
				cin >> parent[j];
			}
			for (int j = 1; j <= n; j++)
			{
				cin >> auth[j];

				if (auth[j] > 20)
					auth[j] = 20;
			}
		}
		else if (order == 200)
		{
			int idx;
			cin >> idx;
			if (noti[idx])
				noti[idx] = false;
			else
				noti[idx] = true;
		}
		else if (order == 300)
		{
			int idx, power;
			cin >> idx >> power;
			auth[idx] = std::min(power,20);
		}
		else if (order == 400)
		{
			int idx1, idx2;
			cin >> idx1 >> idx2;
			std::swap(parent[idx1], parent[idx2]);
		}
		else if (order == 500)
		{
			int c;
			cin >> c;

			ans = 0;

			int result = Search(c,0) -1;
			cout << result << "\n";
		}
	}


	return 0;

}