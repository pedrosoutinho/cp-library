#pragma once

template <typename Monoid, typename XY, bool SMALL_X = false>
struct FenwickTree_2D {
    using G = Monoid;
    using E = typename G::value_type;
    static_assert(G::commute);
    int N;
    V<XY> keyX;
    XY min_X;
    V<int> indptr;
    V<XY> keyY;
    V<E> dat;

    FenwickTree_2D(V<XY>& X, V<XY>& Y, V<E> wt) { build(X, Y, wt); }
    FenwickTree_2D(V<XY>& X, V<XY>& Y) { build(X, Y); }

    inline int xtoi(XY x) {
        if constexpr (SMALL_X) {
            return clamp<int>(x - min_X, 0, N);
        } else {
            return lwb(keyX, x);
        }
    }
    inline int nxt(int i) { return i + ((i + 1) & -(i + 1)); }
    inline int prev(int i) { return i - ((i + 1) & -(i + 1)); }

    void build(V<XY> X, V<XY> Y, V<E> wt) {
        assert(sz(X) == sz(Y));
        if constexpr (!SMALL_X) {
            keyX = X;
            Unique(keyX);
            N = sz(keyX);
        } else {
            min_X = (sz(X) == 0 ? 0 : MIN(X));
            N = (sz(X) == 0 ? 0 : MAX(X)) - min_X + 1;
            keyX.resize(N);
            rep(i, N) keyX[i] = min_X + i;
        }

        auto I = sortedIdx(Y);
        X = rearrange(X, I), Y = rearrange(Y, I), wt = rearrange(wt, I);

        rep(i, sz(X)) X[i] = xtoi(X[i]);

        V<XY> last_y(N, -infty<XY> - 1);
        indptr.assign(N + 1, 0);
        rep(i, sz(X)) {
            int ix = X[i];
            XY y = Y[i];
            while (ix < N) {
                if (last_y[ix] == y) break;
                last_y[ix] = y, indptr[ix + 1]++, ix = nxt(ix);
            }
        }
        rep(i, N) indptr[i + 1] += indptr[i];
        keyY.resize(indptr.back());
        dat.assign(indptr.back(), G::unit());
        fill(all(last_y), -infty<XY> - 1);
        V<int> prog = indptr;
        rep(i, sz(X)) {
            int ix = X[i];
            XY y = Y[i];
            E w = wt[i];
            while (ix < N) {
                if (last_y[ix] != y) {
                    last_y[ix] = y, keyY[prog[ix]] = y, dat[prog[ix]] = w;
                    prog[ix]++;
                } else {
                    dat[prog[ix] - 1] = G::op(dat[prog[ix] - 1], w);
                }
                ix = nxt(ix);
            }
        }
        rep(i, N) {
            int n = indptr[i + 1] - indptr[i];
            rep(j, n - 1) {
                int k = nxt(j);
                if (k < n)
                    dat[indptr[i] + k] =
                        G::op(dat[indptr[i] + k], dat[indptr[i] + j]);
            }
        }
    }

    void build(V<XY> X, V<XY> Y) {
        assert(sz(X) == sz(Y));
        if constexpr (!SMALL_X) {
            keyX = X;
            Unique(keyX);
            N = sz(keyX);
        } else {
            min_X = (sz(X) == 0 ? 0 : MIN(X));
            N = (sz(X) == 0 ? 0 : MAX(X)) - min_X + 1;
            keyX.resize(N);
            rep(i, N) keyX[i] = min_X + i;
        }

        auto I = sortedIdx(Y);
        X = rearrange(X, I), Y = rearrange(Y, I);

        rep(i, sz(X)) X[i] = xtoi(X[i]);

        V<XY> last_y(N, -infty<XY> - 1);
        indptr.assign(N + 1, 0);
        rep(i, sz(X)) {
            int ix = X[i];
            XY y = Y[i];
            while (ix < N) {
                if (last_y[ix] == y) break;
                last_y[ix] = y, indptr[ix + 1]++, ix = nxt(ix);
            }
        }
        rep(i, N) indptr[i + 1] += indptr[i];
        keyY.resize(indptr.back());
        dat.assign(indptr.back(), G::unit());
        fill(all(last_y), -infty<XY> - 1);
        V<int> prog = indptr;
        rep(i, sz(X)) {
            int ix = X[i];
            XY y = Y[i];
            while (ix < N) {
                if (last_y[ix] == y) break;
                last_y[ix] = y, keyY[prog[ix]++] = y, ix = nxt(ix);
            }
        }
    }

    void add(XY x, XY y, E val) { multiply(x, y, val); }
    void multiply(XY x, XY y, E val) {
        int i = xtoi(x);
        assert(keyX[i] == x);
        while (i < N) {
            multiply_i(i, y, val), i = nxt(i);
        }
    }

    E sum(XY lx, XY rx, XY ly, XY ry) { return prod(lx, rx, ly, ry); }
    E prod(XY lx, XY rx, XY ly, XY ry) {
        E pos = G::unit(), neg = G::unit();
        int L = xtoi(lx) - 1, R = xtoi(rx) - 1;
        while (L < R) {
            pos = G::op(pos, prod_i(R, ly, ry)), R = prev(R);
        }
        while (R < L) {
            neg = G::op(neg, prod_i(L, ly, ry)), L = prev(L);
        }
        return G::op(pos, G::inverse(neg));
    }

    E prefix_sum(XY rx, XY ry) { return prefix_prod(rx, ry); }
    E prefix_prod(XY rx, XY ry) {
        E pos = G::unit();
        int R = xtoi(rx) - 1;
        while (R >= 0) {
            pos = G::op(pos, prefix_prod_i(R, ry)), R = prev(R);
        }
        return pos;
    }

  private:
    void multiply_i(int i, XY y, E val) {
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        auto it = keyY.begin() + LID;
        int j = lower_bound(it, it + n, y) - it;
        while (j < n) {
            dat[LID + j] = G::op(dat[LID + j], val), j = nxt(j);
        }
    }

    E prod_i(int i, XY ly, XY ry) {
        E pos = G::unit(), neg = G::unit();
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        auto it = keyY.begin() + LID;
        int L = lower_bound(it, it + n, ly) - it - 1;
        int R = lower_bound(it, it + n, ry) - it - 1;
        while (L < R) {
            pos = G::op(pos, dat[LID + R]), R = prev(R);
        }
        while (R < L) {
            neg = G::op(neg, dat[LID + L]), L = prev(L);
        }
        return G::op(pos, G::inverse(neg));
    }

    E prefix_prod_i(int i, XY ry) {
        E pos = G::unit();
        int LID = indptr[i], n = indptr[i + 1] - indptr[i];
        auto it = keyY.begin() + LID;
        int R = lower_bound(it, it + n, ry) - it - 1;
        while (R >= 0) {
            pos = G::op(pos, dat[LID + R]), R = prev(R);
        }
        return pos;
    }
};
