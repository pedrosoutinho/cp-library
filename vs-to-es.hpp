#pragma once

#include "hashmap.hpp"

template <typename GT>
V<int> vs_to_es(GT& G, V<int>& vs, bool allow_use_twice = false) {
    assert(!vs.empty());

    HashMap<int, int> MP;
    V<int> nxt(G.M, -1);

    auto get = [&](ll a, ll b) -> u64 {
        if (!GT::is_directed && a > b) swap(a, b);
        return a * G.N + b;
    };

    rep(eid, G.M) {
        u64 k = get(G.edges[eid].frm, G.edges[eid].to);
        int x = (MP.count(k) ? MP[k] : -1);
        nxt[eid] = x, MP[k] = eid;
    }
    int n = sz(vs);
    V<int> es(n - 1);
    rep(i, n - 1) {
        u64 k = get(vs[i], vs[i + 1]);
        int eid = (MP.count(k) ? MP[k] : -1);
        assert(eid != -1);
        es[i] = eid;
        if (!allow_use_twice) {
            MP[k] = nxt[eid];
        }
    }
    return es;
}
