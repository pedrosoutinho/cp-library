#pragma once

#include "alg/monoid/min.hpp"
#include "sparse-table.hpp"

// 辞書順 i 番目の suffix が j 文字目始まりであるとき、
// SA[i] = j, ISA[j] = i
// |S|>0 を前提（そうでない場合 dummy 文字を追加して利用せよ）
struct Suffix_Array {
    V<int> SA;
    V<int> ISA;
    V<int> LCP;
    Sparse_Table<Monoid_Min<int>> seg;
    bool build_seg;

    Suffix_Array(string& s) {
        build_seg = 0;
        assert(sz(s) > 0);
        char first = 127, last = 0;
        for (auto&& c : s) {
            ckmin(first, c);
            ckmax(last, c);
        }
        SA = calc_suffix_array(s, first, last);
        calc_LCP(s);
    }

    Suffix_Array(V<int>& s) {
        build_seg = 0;
        assert(sz(s) > 0);
        SA = calc_suffix_array(s);
        calc_LCP(s);
    }

    // lcp(S[i:], S[j:])
    int lcp(int i, int j) {
        if (!build_seg) {
            build_seg = true;
            seg.build(LCP);
        }
        int n = sz(SA);
        if (i == n || j == n) return 0;
        if (i == j) return n - i;
        i = ISA[i], j = ISA[j];
        if (i > j) swap(i, j);
        return seg.prod(i, j);
    }

    // S[i:] との lcp が n 以上であるような半開区間
    pair<int, int> lcp_range(int i, int n) {
        if (!build_seg) {
            build_seg = true;
            seg.build(LCP);
        }
        i = ISA[i];
        int a = seg.min_left([&](auto e) -> bool { return e >= n; }, i);
        int b = seg.max_right([&](auto e) -> bool { return e >= n; }, i);
        return {a, b + 1};
    }

    // -1: S[L1:R1) < S[L2, R2)
    //  0: S[L1:R1) = S[L2, R2)
    // +1: S[L1:R1) > S[L2, R2)
    int compare(int L1, int R1, int L2, int R2) {
        int n1 = R1 - L1, n2 = R2 - L2;
        int n = lcp(L1, L2);
        if (n == n1 && n == n2) return 0;
        if (n == n1) return -1;
        if (n == n2) return 1;
        return (ISA[L1 + n] > ISA[L2 + n] ? 1 : -1);
    }

  private:
    void induced_sort(const V<int>& vect,
                      int val_range,
                      V<int>& SA,
                      const V<bool>& sl,
                      const V<int>& lms_idx) {
        V<int> l(val_range, 0), r(val_range, 0);
        for (int c : vect) {
            if (c + 1 < val_range) ++l[c + 1];
            ++r[c];
        }
        partial_sum(l.begin(), l.end(), l.begin());
        partial_sum(r.begin(), r.end(), r.begin());
        fill(SA.begin(), SA.end(), -1);
        for (int i = (int)lms_idx.size() - 1; i >= 0; --i)
            SA[--r[vect[lms_idx[i]]]] = lms_idx[i];
        for (int i : SA)
            if (i >= 1 && sl[i - 1]) SA[l[vect[i - 1]]++] = i - 1;
        fill(r.begin(), r.end(), 0);
        for (int c : vect) ++r[c];
        partial_sum(r.begin(), r.end(), r.begin());
        for (int k = (int)SA.size() - 1, i = SA[k]; k >= 1; --k, i = SA[k])
            if (i >= 1 && !sl[i - 1]) {
                SA[--r[vect[i - 1]]] = i - 1;
            }
    }

    V<int> SA_IS(const V<int>& vect, int val_range) {
        const int n = vect.size();
        V<int> SA(n), lms_idx;
        V<bool> sl(n);
        sl[n - 1] = false;
        for (int i = n - 2; i >= 0; --i) {
            sl[i] = (vect[i] > vect[i + 1] ||
                     (vect[i] == vect[i + 1] && sl[i + 1]));
            if (sl[i] && !sl[i + 1]) lms_idx.push_back(i + 1);
        }
        reverse(lms_idx.begin(), lms_idx.end());
        induced_sort(vect, val_range, SA, sl, lms_idx);
        V<int> new_lms_idx(lms_idx.size()), lms_vec(lms_idx.size());
        for (int i = 0, k = 0; i < n; ++i)
            if (!sl[SA[i]] && SA[i] >= 1 && sl[SA[i] - 1]) {
                new_lms_idx[k++] = SA[i];
            }
        int cur = 0;
        SA[n - 1] = cur;
        for (size_t k = 1; k < new_lms_idx.size(); ++k) {
            int i = new_lms_idx[k - 1], j = new_lms_idx[k];
            if (vect[i] != vect[j]) {
                SA[j] = ++cur;
                continue;
            }
            bool flag = false;
            for (int a = i + 1, b = j + 1;; ++a, ++b) {
                if (vect[a] != vect[b]) {
                    flag = true;
                    break;
                }
                if ((!sl[a] && sl[a - 1]) || (!sl[b] && sl[b - 1])) {
                    flag = !((!sl[a] && sl[a - 1]) && (!sl[b] && sl[b - 1]));
                    break;
                }
            }
            SA[j] = (flag ? ++cur : cur);
        }
        for (size_t i = 0; i < lms_idx.size(); ++i) lms_vec[i] = SA[lms_idx[i]];
        if (cur + 1 < (int)lms_idx.size()) {
            auto lms_SA = SA_IS(lms_vec, cur + 1);
            for (size_t i = 0; i < lms_idx.size(); ++i) {
                new_lms_idx[i] = lms_idx[lms_SA[i]];
            }
        }
        induced_sort(vect, val_range, SA, sl, new_lms_idx);
        return SA;
    }

    V<int> calc_suffix_array(const string& s,
                              const char first = 'a',
                              const char last = 'z') {
        V<int> vect(s.size() + 1);
        copy(begin(s), end(s), begin(vect));
        for (auto& x : vect) x -= (int)first - 1;
        vect.back() = 0;
        auto ret = SA_IS(vect, (int)last - (int)first + 2);
        ret.erase(ret.begin());
        return ret;
    }

    V<int> calc_suffix_array(const V<int>& s) {
        V<int> ss = s;
        Unique(ss);

        V<int> vect(s.size() + 1);
        copy(all(s), vect.begin());
        for (auto& x : vect) x = lwb(ss, x) + 1;
        vect.back() = 0;
        auto ret = SA_IS(vect, Max(vect) + 2);
        ret.erase(ret.begin());
        return ret;
    }

    template <typename STRING> void calc_LCP(const STRING& s) {
        int n = s.size(), k = 0;
        ISA.resize(n);
        LCP.resize(n);
        for (int i = 0; i < n; i++) ISA[SA[i]] = i;
        for (int i = 0; i < n; i++, k ? k-- : 0) {
            if (ISA[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = SA[ISA[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
            LCP[ISA[i]] = k;
        }
        LCP.resize(n - 1);
    }
};
