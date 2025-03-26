#pragma once
// https://yukicoder.me/problems/no/2085
// https://codeforces.com/problemset/problem/412/D
template <typename F> V<int> hamiltonian_path_in_tournament(int n, F check) {
    auto dfs = [&](auto& dfs, int L, int R) -> V<int> {
        if (R == L + 1) return {L};
        int M = (L + R) / 2;
        V<int> X = dfs(dfs, L, M);
        V<int> Y = dfs(dfs, M, R);
        V<int> P;
        P.reserve(R - L);
        int i = 0, j = 0;
        while (sz(P) < R - L) {
            if (i == sz(X)) {
                P.eb(Y[j++]);
            }
            else if (j == sz(Y)) { P.eb(X[i++]); }
            else {
                if (check(X[i], Y[j])) {
                    P.eb(X[i++]);
                } else {
                    P.eb(Y[j++]);
                }
            }
        }
        return P;
    };
    return dfs(dfs, 0, n);
}

// https://atcoder.jp/contests/joisp2024/tasks/joisp2024_l
V<string> from_outdegrees(V<int> A) {
    int N = sz(A);
    V<int> F(N);
    for (auto& x : A) F[x]++;

    V<string> ANS(N, string(N, '-'));
    rep(i, N) {
        int lose = N - 1 - i - A[i];
        F[A[i]]--;
        rep(j, i + 1, N) { ANS[i][j] = '1', ANS[j][i] = '0'; }
        int p = N;
        per(x, N) {
            int k = min(lose, F[x]);
            p -= F[x];
            rep(j, p, p + k) { ANS[i][j] = '0', ANS[j][i] = '1'; }
            lose -= k;
        }
        rep(j, i + 1, N) {
            if (ANS[i][j] == '0') {
                F[A[j]]--;
                A[j]--;
                F[A[j]]++;
            }
        }
    }
    return ANS;
}
