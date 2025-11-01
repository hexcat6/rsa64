#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void savekeys(uint64_t pq, uint64_t e, uint64_t p, uint64_t q) {
    FILE *pub = fopen("pub.rsa", "wb");
    FILE *pvt = fopen("pvt.rsa", "wb");

    if (pub == NULL || pvt == NULL) {
        printf("file open error\n");
        return;
    }

    fwrite(&p, sizeof(uint64_t), 1, pvt);
    fwrite(&q, sizeof(uint64_t), 1, pvt);
    fwrite(&pq, sizeof(uint64_t), 1, pub);
    fwrite(&e, sizeof(uint64_t), 1, pub);

    fclose(pvt);
    fclose(pub);
}

bool loadpvtkeys(uint64_t* p, uint64_t* q) {
    FILE *pvt = fopen("pvt.rsa", "rb");
    if (pvt == NULL) {
        printf("file open error\n");
        return false;
    }
    fread(p, sizeof(uint64_t), 1, pvt);
    fread(q, sizeof(uint64_t), 1, pvt);
    fclose(pvt);
    return true;
}

bool loadpubkeys(uint64_t* pq, uint64_t* e) {
    FILE *pub = fopen("pub.rsa", "rb");
    if (pub == NULL) {
        printf("file open error\n");
        return false;
    }
    fread(pq, sizeof(uint64_t), 1, pub);
    fread(e, sizeof(uint64_t), 1, pub);
    fclose(pub);
    return true;
}

uint64_t randomprime() {
    uint64_t list[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
    short length = sizeof(list)/sizeof(list[0]);
    return list[rand() % length];
}

// greatest common divisor
// euclidean algorithm
uint64_t gcd(uint64_t a, uint64_t b) {
    uint64_t t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

uint64_t finde(uint64_t p, uint64_t q) {
    uint64_t e = 2; // e must be greater than 1 and coprime with (p-1)*(q-1)
    while (gcd(e, (p-1)*(q-1)) != 1) {
        e++;
    }
    return e;
}

// b^e mod m
uint64_t modexp(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        exponent >>= 1;
    }
    return result;
}

// modular inverse ax = 1 mod m
// extended euclidean algorithm
uint64_t modinverse(uint64_t a, uint64_t m) {
    uint64_t b = m;
    int64_t x0 = 1;
    int64_t x1 = 0;
    
    while (b != 0) {
        double q = floor((double)a / b);
        uint64_t temp_a = a;
        a = b;
        b = temp_a % b;
        
        int64_t temp_x0 = x0;
        x0 = x1;
        x1 = temp_x0 - q * x1;
    }
    
    return (uint64_t)(x0 + m) % m;
}

uint64_t *encrypt(const char *message, size_t length, uint64_t pq, uint64_t e) {
    uint64_t *encryptedmessage = malloc((length+1)*sizeof(uint64_t));
    for (size_t i = 0; i < length; i++) {// C = M^e mod pq
        encryptedmessage[i] = modexp((uint64_t)(message[i]), e, pq);
    }
    return encryptedmessage;
}

char *decrypt(const uint64_t message[], size_t length, uint64_t p, uint64_t q, uint64_t e) {
    uint64_t d = modinverse(e, (p-1)*(q-1));// d*e = 1 mod (p-1)(q-1)
    char *decryptedmessage = malloc((length+1)*sizeof(char));
    for (size_t i = 0; i < length; i++) {// M = C^d mod pq
        decryptedmessage[i] = (char)(modexp(message[i], d, p*q));
    }
    decryptedmessage[length] = '\0';
    return decryptedmessage;
}

int main(int argc, const char *argv[]) {
    srand(time(NULL));
    uint64_t p, q, pq, e;

    if (!loadpvtkeys(&p, &q) || !loadpubkeys(&pq, &e)) {
        p = randomprime();
        q = randomprime();
        pq = p*q;
        e = finde(p, q);
        savekeys(pq, e, p, q);
        printf("pub:\n\te:%lld\tpq:%lld\npvt:\n\tp:%lld\tq:%lld\n", e, pq, p, q);
    }

    if (argc > 2) {
        if (argv[1][0] == '-' && argv[1][1] == 'e') {// encrypt
            size_t length = strlen(argv[2]);
            uint64_t *message = encrypt(argv[2], length, pq, e);
            for (size_t i = 0; i < length; i++) {
                printf("%llu ", message[i]);
            }
            free(message);
        }
        if (argv[1][0] == '-' && argv[1][1] == 'd') {// decrypt
            // Parse space-separated numbers for decryption
            size_t length = 0;
            uint64_t *numbers = malloc(1000 * sizeof(uint64_t));  // Adjust size as needed
            char *input = strdup(argv[2]);
            char *token = strtok(input, " ");
            
            while (token != NULL) {
                numbers[length++] = strtoull(token, NULL, 10);
                token = strtok(NULL, " ");
            }
            
            char *decrypted = decrypt(numbers, length, p, q, e);
            printf("%s", decrypted);
            
            free(numbers);
            free(decrypted);
            free(input);
        }
    }

    return 0;
}