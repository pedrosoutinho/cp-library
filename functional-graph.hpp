#pragma once

#include "tree.hpp"
#include "unionfind.hpp"

// N が根となる木を新たに作る

template <typename T = int> struct FunctionalGraph {
    int N, M;
    V<int> TO;
    V<T> wt;
    V<int> root;
    Graph<T, 1> G;

    FunctionalGraph() {}
    FunctionalGraph(int N) : N(N), M(0), TO(N, -1), wt(N), root(N, -1) {}

    void add(int a, int b, T c = 1) {
        assert(0 <= a && a < N);
        assert(TO[a] == -1);
        ++M;
        TO[a] = b;
        wt[a] = c;
    }

    pair<Graph<T, 1>, Tree<Graph<T, 1>>> build() {
        assert(N == M);
        UnionFind uf(N);
        rep(v, N) if (!uf.merge(v, TO[v])) { root[v] = v; }
        rep(v, N) if (root[v] == v) root[uf[v]] = v;
        rep(v, N) root[v] = root[uf[v]];

        G.build(N + 1);
        rep(v, N) {
            if (root[v] == v)
                G.add(N, v, wt[v]);
            else
                G.add(TO[v], v, wt[v]);
        }
        G.build();
        Tree<Graph<T, 1>> tree(G, N);
        return {G, tree};
    }

    // a -> b にかかる回数. 不可能なら infty<int>. O(1).

    template <typename TREE> int dist(TREE& tree, int a, int b) {
        if (tree.in_subtree(a, b)) return tree.depth[a] - tree.depth[b];
        int r = root[a];
        int btm = TO[r];
        // a -> r -> btm -> b

        if (tree.in_subtree(btm, b)) {
            int x = tree.depth[a] - tree.depth[r];
            x += 1;
            x += tree.depth[btm] - tree.depth[b];
            return x;
        }
        return infty<int>;
    }

    // functional graph に向かって進む

    template <typename TREE> int jump(TREE& tree, int v, ll step) {
        int d = tree.depth[v];
        if (step <= d - 1) return tree.jump(v, N, step);
        v = root[v];
        step -= d - 1;
        int bottom = TO[v];
        int c = tree.depth[bottom];
        step %= c;
        if (step == 0) return v;
        return tree.jump(bottom, N, step - 1);
    }

    // functional graph に step 回進む

    template <typename TREE> V<int> jump_all(TREE& tree, ll step) {
        V<int> res(N, -1);
        // v の k 個先を res[w] に入れる

        V<vpi> query(N);
        rep(v, N) {
            int d = tree.depth[v];
            int r = root[v];
            if (d - 1 > step) {
                query[v].eb(v, step);
            }
            if (d - 1 <= step) {
                ll k = step - (d - 1);
                int bottom = TO[r];
                int c = tree.depth[bottom];
                k %= c;
                if (k == 0) {
                    res[v] = r;
                    continue;
                }
                query[bottom].eb(v, k - 1);
            }
        }

        V<int> path;
        auto dfs = [&](auto& dfs, int v) -> void {
            path.eb(v);
            for (auto&& [w, k] : query[v]) {
                res[w] = path[sz(path) - 1 - k];
            }
            for (auto&& e : G[v]) dfs(dfs, e.to);
            path.pop_back();
        };
        for (auto&& e : G[N]) {
            dfs(dfs, e.to);
        }
        return res;
    }

    template <typename TREE> bool in_cycle(TREE& tree, int v) {
        int r = root[v];
        int bottom = TO[r];
        return tree.in_subtree(bottom, v);
    }

    V<int> collect_cycle(int r) {
        assert(r == root[r]);
        V<int> cyc = {TO[r]};
        while (cyc.back() != r) cyc.eb(TO[cyc.back()]);
        return cyc;
    }
};
