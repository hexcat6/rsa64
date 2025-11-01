## RSA64

RSA encryption algorithm with 64-bit key size implemented in a single C file without a big integer library. While I made significant progress independently, I needed AI assistance to overcome an integer overflow issues due to the 64-bit limitations. This was due to the fact the encryption and decryption forumla require calculating large powers. The solution involved breaking encryption and decryption calculations into manageable smaller pieces.

### The Maths

The mathematical simplicity of RSA surprised me, making it an interesting one-day project. Here's the core mathematics for RSA key generation and encryption/decryption:

For generation:
1. Select two random prime numbers $p$ and $q$
2. Calculate the product of the two prime numbers $pq$
3. Choose $e$ where $gcd(e, (p-1)(q-1)) = 1$
4. Public key is $(pq,e)$
5. Private key is $(p,q)$ 

For encryption:
- Message $M$ is encrypted character by character using:
    $C \equiv M^e \pmod{pq}$

For decryption:
- Cipher $C$ is decrypted using private key $d$ where:
    $M \equiv C^d \pmod{pq}$
- $d$ is calculated as the multiplicative inverse of $e$ modulo $(p-1)(q-1)$:
    $ed \equiv 1 \pmod{(p-1)(q-1)}$

### How to use

To compile the program, execute one of the  following command:

```sh
gcc RSA.c -o RSA
```
```sh
make
```
To use the program, execute it once with no parameter to generate the keys.
```sh
./RSA
file open error
pub:
        e:3     pq:348913
pvt:
        p:911   q:383
```
To encrypt a message, send the `pub.rsa` file to a friend and then make them run this command:
```sh
./RSA -e "friend's message"
14469 85892 110886 332475 284261 302174 59319 125223 32768 248290 332475 125223 125223 214847 45988 332475
```
To decrypt their message, use:
```sh
./RSA -d "14469 85892 110886 332475 284261 302174 59319 125223 32768 248290 332475 125223 125223 214847 45988 332475"
friend's message
```