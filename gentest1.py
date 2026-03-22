import random
import string

parole = [
    "alpha", "beta", "gamma", "delta", "nova", "quantum",
    "matrix", "neural", "vector", "fusion", "orbit",
    "pixel", "logic", "system", "core", "drive","pippo","emilia","amelia","eq","gaus"
]

ALPHABET = string.ascii_lowercase
VARIANTS_PER_PAIR = len(parole) * len(ALPHABET) * len(ALPHABET)
MAX_UNIQUE = len(ALPHABET) * len(ALPHABET) * VARIANTS_PER_PAIR


def de_bruijn(k, n):
    a = [0] * (k * n)
    sequence = []

    def db(t, p):
        if t > n:
            if n % p == 0:
                sequence.extend(a[1 : p + 1])
        else:
            a[t] = a[t - p]
            db(t + 1, p)
            for j in range(a[t - p] + 1, k):
                a[t] = j
                db(t + 1, t)

    db(1, 1)
    return sequence


def edge_cycle_pairs():
    # Consecutive symbols in a de Bruijn sequence of order 2 give all ordered pairs once.
    seq = de_bruijn(len(ALPHABET), 2)
    seq.append(seq[0])
    for i in range(len(seq) - 1):
        yield ALPHABET[seq[i]], ALPHABET[seq[i + 1]]

def test_corretto(n):
    if n > MAX_UNIQUE:
        raise ValueError(
            f"Requested {n} names, but max unique chainable names is {MAX_UNIQUE}."
        )

    nomi = []
    for variant in range(VARIANTS_PER_PAIR):
        base_index = variant // (len(ALPHABET) * len(ALPHABET))
        rem = variant % (len(ALPHABET) * len(ALPHABET))
        mid1 = ALPHABET[rem // len(ALPHABET)]
        mid2 = ALPHABET[rem % len(ALPHABET)]
        base = parole[base_index]
        for start, end in edge_cycle_pairs():
            # Format: start + base + mid1 + mid2 + end; max length is 11.
            nomi.append(f"{start}{base}{mid1}{mid2}{end}")
            if len(nomi) == n:
                return nomi

    return nomi

def test_errore(n):
    nomi = test_corretto(n)
    if n > 2:
        nomi[-1] = nomi[0]  # duplicato
    return nomi

if __name__ == "__main__":
    result = test_corretto(670000)
    with open("/mnt/c/Users/matte/Desktop/Uni/ALG2/exam/coso1.txt", "w") as f:
        f.write(str(len(result)) + "\n")
        f.write("\n".join(result))