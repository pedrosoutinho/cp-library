#pragma once
#include "hashmap.hpp"
#include "geometry-base.hpp"
#include "rng.hpp"

template <typename T> pair<int, int> closest_pair(V<Point<T>> points) {
    int N = sz(points);
    assert(N >= 2);
    HashMap<pi, int> MP;
    V<int> I(N);
    iota(all(I), 0);
    randshf(I);
    points = rearrange(points, I);

    auto calc = [&](int i, int j) -> T {
        return (points[j] - points[i]).dot(points[j] - points[i]);
    };

    T best = calc(0, 1);
    pair<int, int> res = {0, 1};
    T w = sqrtl(best);

    V<int> nxt(N, -1);

    auto insert = [&](int i) -> void {
        pi k = {points[i].x / w, points[i].y / w};
        nxt[i] = (MP.find(k) == MP.end() ? -1 : MP[k]);
        MP[k] = i;
    };

    auto query = [&](int i) -> bool {
        ll a = points[i].x / w;
        ll b = points[i].y / w;
        bool upd = 0;
        rep(dx, -1, 2) rep(dy, -1, 2) {
            pi k = {a + dx, b + dy};
            int j = (MP.find(k) == MP.end() ? -1 : MP[k]);
            while (j != -1) {
                if (ckmin(best, calc(i, j))) {
                    upd = 1, res = {i, j}, w = sqrtl(best);
                }
                j = nxt[j];
            }
        }
        return upd;
    };

    if (best == T(0)) {
        res.fi = I[res.fi], res.se = I[res.se];
        return res;
    }

    insert(0), insert(1);
    rep(i, 2, N) {
        if (query(i)) {
            if (best == T(0)) break;
            MP.clear();
            rep(j, i) insert(j);
        }
        insert(i);
    }
    res.fi = I[res.fi], res.se = I[res.se];
    return res;
}

pair<int, int> closest_pair_dc(V<Point<ll>> point) {
    int N = sz(point);
    assert(N >= 2);

    auto I = sortedIdx(point);
    point = rearrange(point, I);

    ll best = -1;
    pair<int, int> best_pair = {-1, -1};

    auto upd = [&](int i, int j) -> void {
        Point<ll> p = point[i] - point[j];
        ll d = p.dot(p);
        if (best == -1 || best > d) {
            best = d, best_pair = {I[i], I[j]};
        }
    };
    upd(0, 1);

    auto dfs = [&](auto& dfs, int L, int R) -> V<int> {
        // return: [L,R) を y について sort したもの
        if (R == L + 1) return {L};
        int M = (L + R) / 2;
        V<int> I0 = dfs(dfs, L, M);
        V<int> I1 = dfs(dfs, M, R);
        V<int> I;
        V<int> near;
        int a = 0, b = 0;
        rep(R - L) {
            int idx = [&]() -> int {
                if (a == sz(I0)) return I1[b++];
                if (b == sz(I1)) return I0[a++];
                int i = I0[a], j = I1[b];
                if (point[i].y < point[j].y) {
                    ++a;
                    return i;
                }
                ++b;
                return j;
            }();
            I.eb(idx);
            ll dx = point[M].x - point[idx].x;
            if (dx * dx > best) {
                continue;
            }
            per(k, sz(near)) {
                int j = near[k];
                ll dy = point[idx].y - point[j].y;
                if (best == 0 || dy * dy > best) break;
                upd(idx, j);
            }
            near.eb(idx);
        }
        return I;
    };
    dfs(dfs, 0, N);
    return best_pair;
}
