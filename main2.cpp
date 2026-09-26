#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

struct Data {
	int s;
	int cursor, x;
};

struct DataHash {
	size_t operator()(const Data& d) const {
		size_t h = 0;

		h = d.s * 2 + 1;

		h = h * 13 + 1000003 + d.cursor;

		h = h * 13 + 1000003 + d.x;

		return h;
	}
};

bool operator==(const Data& a, const Data& b) {
	return a.s == b.s &&
		a.cursor == b.cursor &&
		a.x == b.x;
}

/*
* Data: {현재 문자열, 현재 큐, 커서 위치(앞에 있는 문자 갯수), 현재까지 처리한 문자 수}
*/

int n;
int arr[32];
unordered_map<Data, int, DataHash> dist;
int ans = 1e9;

int insert(int N, int K) {
	int X = N & ((1 << K) - 1);
	return ((N >> K) << (K + 1) | (1 << K) | X);
}

int find_queue(int N, int X) {
	return ((1 << N) - 1) ^ X;
}

void bfs() {
	deque<Data> q; // 이름은 q인데 사실 dq.
	q.push_front({ 0, 0, 0 });
	dist[{ 0, 0, 0 }] = 0;
	while (!q.empty()) {
		Data d = q.front();
		q.pop_front();
		int s = d.s;
		int c = d.cursor;
		int x = d.x;
		int que = find_queue(x, s);
		if (floor(log2(s)) == n) {
			bool check = true;
			for (int i = 1; i < n; i++) {
				if (s[i - 1] > s[i]) check = false;
			}
			if (!check) continue;
			ans = min(ans, dist[{s, que, c, x}]);
			continue;
		}
		// in x 처리.
		if (x < n && (s.empty() || (c == 0 || s[c - 1] < arr[x]) && (c == s.size() || s[c] > arr[x]))) {
			vector<int> s2 = s;
			s2.insert(s2.begin() + c, arr[x]);
			if (dist.find({ s2, que, c + 1, x + 1 }) == dist.end() ||
				dist[{ s2, que, c + 1, x + 1 }] > dist[{s, que, c, x}]) {
				dist[{s2, que, c + 1, x + 1}] = dist[{s, que, c, x}];
				q.push_front({ s2, que, c + 1, x + 1 });
			}
		}

		// 큐 처리. (push)
		vector<int> que2 = que;
		que2.push_back(arr[x]);
		if (x < n && (dist.find({ s, que2, c, x + 1 }) == dist.end() ||
			dist[{ s, que2, c, x + 1 }] > dist[{s, que, c, x}] + 1)) {
			dist[{ s, que2, c, x + 1 }] = dist[{s, que, c, x}] + 1;
			q.push_back({ s, que2, c, x + 1 });
		}
		// pop 처리.
		if (!que.empty() && (c == 0 || s.empty() || s[c - 1] < que.front()) && (c == s.size() || s[c] > que.front())) {
			vector<int> s2 = s;
			s2.insert(s2.begin() + c, que.front());
			vector<int> que2 = que;
			que2.erase(que2.begin());
			if (dist.find({ s2, que2, c + 1, x }) == dist.end() ||
				dist[{ s2, que2, c + 1, x}] > dist[{s, que, c, x}] + 1) {
				// 참고로 x가 증가하지 않는 이유는 
				// 이 행동에서 pop만 하기 때문에 새로 처리한 문자가 없음.
				dist[{ s2, que2, c + 1, x}] = dist[{s, que, c, x}] + 1;
				q.push_back({ s2, que2, c + 1, x });
			}
		}
		// ptr++ 처리.
		if (c != s.size() && (dist.find({ s, que, c + 1, x }) == dist.end() ||
			dist[{ s, que, c + 1, x }] > dist[{s, que, c, x}] + 1)) {
			dist[{ s, que, c + 1, x }] = dist[{s, que, c, x}] + 1;
			q.push_back({ s, que, c + 1, x });
		}
		// ptr-- 처리. 
		if (c != 0 && (dist.find({ s, que, c - 1, x }) == dist.end() ||
			dist[{ s, que, c - 1, x }] > dist[{s, que, c, x}] + 1)) {
			dist[{ s, que, c - 1, x }] = dist[{s, que, c, x}] + 1;
			q.push_back({ s, que, c - 1, x });
		}
	}
}

int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	bfs();
	cout << ans;
	return 0;
}