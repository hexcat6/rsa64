## RSA64

RSA encryption algorithm with 64-bit key size implemented in a single C file without a big integer library. While I made significant progress independently, I needed AI assistance to overcome integer overflow issues with 64-bit limitations. The solution involved breaking encryption and decryption calculations into manageable pieces.

The mathematical elegance of RSA surprised me, making it an interesting one-day project. Here's the core mathematics:

For RSA key generation and encryption:

1. Select two prime numbers $p$ and $q$
2. Calculate $n = pq$
3. Choose $e$ where $gcd(e, (p-1)(q-1)) = 1$
4. Public key is $(n,e)$

For encryption:
- Message $M$ is encrypted character by character using:
    $C \equiv M^e \pmod{n}$

For decryption:
- Cipher $C$ is decrypted using private key $d$ where:
    $M \equiv C^d \pmod{n}$
- $d$ is calculated as the multiplicative inverse of $e$ modulo $(p-1)(q-1)$:
    $ed \equiv 1 \pmod{(p-1)(q-1)}$
