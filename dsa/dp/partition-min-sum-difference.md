# Partition a Set into Two Subsets with Minimum Absolute Sum Difference

## Intuition

We have total sum `S`.

If one subset has sum `s1`, the other has sum `S - s1`.

So the difference is:

`|s1 - (S - s1)| = |S - 2*s1|`

Therefore, the real problem is:

> Which subset sums are possible?

This becomes a **Subset Sum DP** problem.

## DP meaning

`dp[i][sum] = true` means using the first `i` elements, we can make `sum`.

Once we know all achievable sums, we only need to check sums up to `S/2`.

For every achievable `s1`:

`difference = S - 2*s1`

and we take the minimum.

## Key connection

Partition Minimum Difference
→ find possible subset sums
→ Subset Sum DP
→ choose the achievable sum closest to `S/2`.
