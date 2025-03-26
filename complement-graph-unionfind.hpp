#pragma once

#include "graph-base.hpp"
#include "unionfind.hpp"

template <typename GT> UnionFind complement_graph_unionfind(GT& G) {
    const int N = G.N;
    V<int> que(N), yet(N);
    V<bool> NG(N);
    int p = 0;
    rep(v, N) yet[p++] = v;
    UnionFind uf(N);

    while (p) {
        int ql = 0, qr = 0;
        que[qr++] = yet[--p];
        while (ql < qr) {
            int v = que[ql++];
            for (auto& e : G[v]) NG[e.to] = 1;
            for (int i = p - 1; i >= 0; --i) {
                int to = yet[i];
                if (NG[to]) continue;
                que[qr++] = to;
                swap(yet[i], yet[--p]);
                uf.merge(v, to);
            }
            for (auto& e : G[v]) NG[e.to] = 0;
        }
    }
    return uf;
}
