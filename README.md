# Algorithm Analysis & Problem Solving

> Five classic CS problems. Two solutions each. Brute force vs. optimized — benchmarked head-to-head.

This project was developed for the *Algorithms and Problem Solving* course at **University of Modena and Reggio Emilia (UNIMORE)**, under Prof. Matteo Cavaliere. Each problem is solved twice: once with the most intuitive approach, once with the most efficient one — with empirical benchmarks to show exactly why it matters.

---

## What's Inside

### 1. BrainStorming — String Chain Validation
**The problem:** Validate a sequence of N names where each name must start with the last letter of the previous one, and no name can repeat.

| Solution | Approach | Complexity |
|---|---|---|
| A | Linear scan + list comparison | O(N²) |
| B | Two-level OAAT hash map | ~O(N) avg |

**Takeaway:** Hash maps turn an O(N²) duplicate-check into near-linear time. The gap becomes dramatic at N > 100,000.

---

### 2. Battery Usage — Optimal Task Scheduling
**The problem:** A robot has T battery units. Three tasks cost a, b, c units each. Minimize leftover battery, and among equally optimal solutions, maximize the number of tasks completed.

| Solution | Approach | Complexity |
|---|---|---|
| A | Top-down recursion (full tree) | O(3ᵀ) |
| B | Bottom-up Dynamic Programming | O(T) |

**Takeaway:** Exponential vs. linear. The DP solution memoizes subproblems — solution A becomes unusable past T ≈ 30.

---

### 3. Working Breaks — Scheduling with Constraints
**The problem:** N workers each need a mandatory break of kᵢ minutes within a shift of T minutes. At most 2 workers can be on break simultaneously. Find a valid schedule or report none exists.

| Solution | Approach | Complexity |
|---|---|---|
| A | Brute-force decision tree with pruning | O(N²ᵀ) |
| B | Knapsack via DP + feasibility check | O(N·T) |

**Takeaway:** Reframing the problem as a knapsack cuts an exponential search into a polynomial one. Classic example of problem reduction.

---

### 4. TrainStations — Binary Search on Answer
**The problem:** A railway line of length L has N intermediate stops. Remove at most M stations (keeping the first, last, and a mandatory control station) to maximize the minimum distance between any two consecutive stops.

| Solution | Approach | Complexity |
|---|---|---|
| A | Backtracking over all subsets | O(L + 2ᴺ·N) |
| B | Binary search on answer + feasibility check + bitmap | O(L + M·log₂L) |

**Takeaway:** Binary search on the answer is a powerful pattern — instead of searching *which* stations to remove, search *what distance is achievable*, then verify. Bitmap representation keeps memory overhead minimal even at L = 1,000,000.

---

### 5. Dream Team — Optimal Team Selection
**The problem:** Given C candidates with reliability and criticality scores, select P members to maximize team strength = min(reliability) − max(criticality).

| Solution | Approach | Complexity |
|---|---|---|
| A | Monte Carlo randomized sampling (99.99% confidence) | O(C · C choose P) |
| B | Sort by reliability + DP to minimize max criticality | O(C log C + C·P) |

**Takeaway:** Monte Carlo gives probabilistic guarantees but explodes combinatorially. The DP solution sorts candidates to decouple the two objectives and fills a clean Bellman recurrence — exact result, every time.

---

## Benchmarks

Every solution pair is benchmarked empirically with charts comparing execution time vs. input size. The performance gap is consistently dramatic — and intentional. The "worse" solution is included to make the contrast concrete, not as a mistake.

---

## Tech Stack

- **Language:** C
- **Techniques covered:** Dynamic Programming, Hash Maps, Binary Search on Answer, Monte Carlo, Backtracking, Bitmaps, Knapsack

---

## AI Usage Disclosure

AI assistance was used for generating a few specific utility functions (hashing, random team sampling, bitmap primitives, backtracking reconstruction) and for generating test cases. All algorithmic design, analysis, and implementation decisions are original work. See the full declaration in the report.

---

## Author

**Matteo Merolli**
[matteomerollim1@gmail.com](mailto:matteomerollim1@gmail.com)
University of Modena and Reggio Emilia — FIM
