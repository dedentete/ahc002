#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define ALL(v) (v).begin(), (v).end()
using ll = long long;
constexpr int INF = 1e9;
constexpr long long LINF = 1e18;
constexpr long long MOD = 1e9 + 7;

constexpr int N = 50;
int M;
constexpr int dx[] = {-1, 0, 1, 0}, dy[] = {0, 1, 0, -1};
constexpr double TIMELIMIT = 1.9;

int si, sj;
int t[N][N];
int p[N][N];

struct XorShift {
    unsigned int x, y, z, w, t;

    XorShift(int seed) {
        mt19937 rnd(seed);
        x = rnd();
        y = rnd();
        z = rnd();
        w = rnd();
        t = 1;
    }

    int rand() {
        t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w & 0x7fffffff;
    }
} rnd(rand());

struct Timer {
    chrono::system_clock::time_point start, now;

    Timer() {
        start = chrono::system_clock::now();
    }

    double getTime() {
        now = chrono::system_clock::now();
        return chrono::duration<double>(now - start).count();
    }
};

struct State {
    vector<pair<int, int>> xy;
    vector<int> canMove;
    vector<bool> visited;
    int score;
};

Timer tmr;
double startclock = -1, nowclock = -1;

void init(State& bestState) {
    int steps = 0;
    while (true) {
        if (steps % 10000 == 0) {
            nowclock = tmr.getTime();
            if (nowclock - startclock > 0.1) break;
        }
        State state;
        state.visited.resize(M);
        state.visited[t[si][sj]] = true;
        state.score = p[si][sj];
        int x = si, y = sj;
        state.xy.emplace_back(x, y);
        while (true) {
            vector<int> move;
            rep(i, 4) {
                int nx = x + dx[i], ny = y + dy[i];
                if (nx < 0 or N <= nx or ny < 0 or N <= ny) continue;
                if (state.visited[t[nx][ny]]) continue;
                move.emplace_back(i);
            }
            if (move.empty()) {
                state.canMove.emplace_back(0);
                break;
            }
            int i = move[rnd.rand() % move.size()];
            state.canMove.emplace_back(move.size());
            x = x + dx[i], y = y + dy[i];
            state.xy.emplace_back(x, y);
            state.visited[t[x][y]] = true;
            state.score += p[x][y];
        }
        if (state.score > bestState.score) {
            bestState = state;
        }
        steps++;
    }
    cerr << "score : " << bestState.score << endl;
    cerr << "steps : " << steps << endl;
}

int D;

void modify(State& state) {
    int x, y;
    while (not state.xy.empty()) {
        x = state.xy.back().first;
        y = state.xy.back().second;
        if (state.canMove.back() > 1 and rnd.rand() % D == 0) {
            state.canMove.pop_back();
            break;
        }
        state.canMove.pop_back();
        state.visited[t[x][y]] = false;
        state.score -= p[x][y];
        state.xy.pop_back();
    }
    if (state.xy.empty()) {
        return;
    }
    while (true) {
        vector<int> move;
        rep(i, 4) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 or N <= nx or ny < 0 or N <= ny) continue;
            if (state.visited[t[nx][ny]]) continue;
            move.emplace_back(i);
        }
        if (move.empty()) {
            state.canMove.emplace_back(0);
            break;
        }
        int i = move[rnd.rand() % move.size()];
        state.canMove.emplace_back(move.size());
        x = x + dx[i], y = y + dy[i];
        state.xy.emplace_back(x, y);
        state.visited[t[x][y]] = true;
        state.score += p[x][y];
    }
}

void solve(State& state) {
    State bestState = state;
    int steps = 0;
    double starttemp = 1000, endtemp = 0;
    double temp = -1, prob = -1;
    while (true) {
        if (steps % 10000 == 0) {
            nowclock = tmr.getTime();
            if (nowclock - startclock > TIMELIMIT) break;
            temp = starttemp +
                   (endtemp - starttemp) * (nowclock - startclock) / TIMELIMIT;
            state = bestState;
            D = (double)(1.2 - (nowclock - startclock) / TIMELIMIT) * 30.0;
        }
        State newstate = state;
        modify(newstate);
        /*
        if (newstate.score > state.score) {
            state = newstate;
        }
        */
        prob = exp((newstate.score - state.score) / temp);
        if (prob > (rnd.rand() % (int)1e9) / 1e9) {
            state = newstate;
            if (bestState.score < state.score) {
                bestState = state;
            }
        }
        steps++;
    }
    state = bestState;
    cerr << "score : " << state.score << endl;
    cerr << "steps : " << steps << endl;
}

void input() {
    cin >> si >> sj;
    rep(i, N) {
        rep(j, N) {
            cin >> t[i][j];
            M = max(M, t[i][j]);
        }
    }
    rep(i, N) {
        rep(j, N) {
            cin >> p[i][j];
        }
    }
}

void output(State& state) {
    for (int i = 0; i < (int)state.xy.size() - 1; i++) {
        int x = state.xy[i].first, y = state.xy[i].second;
        int nx = state.xy[i + 1].first, ny = state.xy[i + 1].second;
        if (nx == x - 1) {
            cout << "U";
        } else if (nx == x + 1) {
            cout << "D";
        } else if (ny == y - 1) {
            cout << "L";
        } else {
            cout << "R";
        }
    }
}

signed main() {
    startclock = tmr.getTime();
    input();
    State state;
    state.score = 0;
    init(state);
    solve(state);
    output(state);
    return 0;
}