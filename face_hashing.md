## Face hashing algorithm

$\mathcal O(d)$ face hashing algorithm

Hashing parameters:
- $\ell$ — hypercube side
- $\varepsilon$ — bucket width multiplier
- $a$ — rolling hash polynomial
- $m$ — rolling hash modulo

For hashing point $p$:
- $p_i$ — $i$-th coordinate of $p$
- $\alpha_i := p_i \bmod \ell$ — coordinate distance to lowest hypercube point
- $\delta_i := \min(\alpha_i, \ell - \alpha_i)$ — distance to nearest hypercube face

### Hashing

1. For given point $p$, find how many coordinates $p_i$ holds $(j-1)\varepsilon \leq \delta_i < j\cdot\varepsilon$ for each $j$.

2. Now find biggest $x$ for which there are $x$ coordinates with $\delta_i < x \cdot \varepsilon$. Then $d - x$ is the dimension of the corresponding face bucket.

3. Let $q = b(p)$ be a point representing bucket $p$ belongs to:

$$q_i = \begin{cases}
    p_i - \alpha_i & \alpha_i < x\varepsilon\\
    p_i - \alpha_i + \ell & \alpha_i > \ell - x\varepsilon\\
    p_i - \alpha_i + \frac{\ell}{2} & \text{otherwise} \\
\end{cases}$$

4. Use polynomial rolling hash to convert $q$ to `ull`:
$$h(q) := \left(\sum_{i=1}^{d} \frac{2q_i}{\ell} a^{d-i} \right)\bmod m$$

```c++
typedef ull unsigned long long;

int d; // dimension
int l; // hypercube side
float eps; // epsilon - hashing parameter

ull hash_poly, hash_mod;  // rolling hash parameters

ull FaceHash(vector<float> point) {
    // distance calculation
    vector<int> epsilon_multiply(d+1, 0);
    for (int i=0; i<d; i++) {
        float delta = point[i] % l;
        delta = min(delta, l - delta);

        epsilon_multiply[min(int(delta/eps), d)]++;
    }

    // find face dimension
    int mul = -1;
    int points_within = 0;
    for (int x=0; x<d; d++) {
        points_within += epsilon_multiply[x];
        if (points_within >= x)
            mul = x;
    }

    // normalize point
    for (int i=0; i<d; i++) {
        float alpha = point[i] % l;

        if (alpha < mul*eps)
            point[i] -= alpha;
        else if (alpha > l - mul*eps)
            point[i] += l - alpha;
        else
            point[i] += l/2.0 - alpha;
    }

    // compute hash
    ull hash = 0;
    for (int i=0; i<d; i++) {
        hash *= hash_poly;
        hash += 2*point[i] / l;
        hash %= hash_mod;
    }
    return hash;
}
```

### Finding buckets within distance

Given point $p$ we want to find nearby buckets within distance at most $\frac{\varepsilon}{2}$.
First we notice that a ball of diameter $\varepsilon$ can intersect at most one bucket
corresponding to dimension $d'$.

Therefore we iterate over each $d'$ and try to construct a point $p'$ in the closest bucket.
Afterwards, we check if $|p - p'| \leq \frac{\varepsilon}{2}$, and if so we add the bucket to result.

For constructing point $p'$:
1. We sort all coordinates by $\delta_i$ ascending.
2. For the first $d - d'$ coordinates, if $\delta_i \geq (d-d') \cdot \varepsilon$ we decrease them until inequality flips.
3. For the rest, we increase $\delta_i$ to $(d-d') \cdot \varepsilon$ if they are less than that.
