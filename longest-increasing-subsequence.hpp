// dp[i] := 第 i 項で終わる lis 長の最大値（[1, LIS]）
template <typename T, bool strong = true>
pair<int, V<int>> longest_increasing_subsequence(vector<T> A) {
    const int N = A.size();
    V<T> dp(N, infty<T>);
    int lis = 0;
    V<int> lis_rank(N);
    rep(i, N) {
        int j = (strong ? lwb(dp, A[i]) : upb(dp, A[i]));
        dp[j] = A[i];
        lis_rank[i] = j + 1;
        ckmax(lis, j + 1);
    }
    return {lis, lis_rank};
}
