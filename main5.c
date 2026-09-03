//Student Name: [ABDULRHMAN HISHAM ADBULLAH]
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#define MSG_MAX    128U
#define ALPHABET   26U

static char message[MSG_MAX];
static char scratch[MSG_MAX];
static uint8_t shiftKey = 3; 

static void readMessage(void);
static uint16_t textLength(const char *s);
static char shiftChar(char c, uint8_t key);
static void encode(void);
static void decode(void);
static void reverseText(char *s, uint16_t n);
static uint16_t countVowels(const char *s);
static uint8_t isPalindrome(const char *s);
static void letterHistogram(const char *s);
static void showMessage(void);

static void readMessage(void) {
    printf("Enter message: ");
    uint16_t idx = 0;
    int c;

    // Clear leftover newline if any
    while ((c = getchar()) == '\n' || c == '\r');

    while (c != '\n' && c != '\r' && c != EOF) {
        if (idx < (MSG_MAX - 1U)) {
            message[idx++] = (char)c;
        }
        c = getchar();
    }
    message[idx] = '\0';
}

// Custom length function (no strlen used)
static uint16_t textLength(const char *s) {
    uint16_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

static char shiftChar(char c, uint8_t key) {
    key = key % ALPHABET;

    if (c >= 'a' && c <= 'z') {
        return (char)('a' + (c - 'a' + key) % ALPHABET);
    } else if (c >= 'A' && c <= 'Z') {
        return (char)('A' + (c - 'A' + key) % ALPHABET);
    }
    return c; // Punctuation, spaces, numbers unchanged
}

static void encode(void) {
    uint16_t len = textLength(message);
    for (uint16_t i = 0; i < len; i++) {
        scratch[i] = shiftChar(message[i], shiftKey);
    }
    scratch[len] = '\0';
    printf("Encoded Message: %s\n", scratch);
}

static void decode(void) {
    uint8_t decodeKey = (ALPHABET - (shiftKey % ALPHABET)) % ALPHABET;
    uint16_t len = textLength(message);
    for (uint16_t i = 0; i < len; i++) {
        scratch[i] = shiftChar(message[i], decodeKey);
    }
    scratch[len] = '\0';
    printf("Decoded Message: %s\n", scratch);
}

static void reverseText(char *s, uint16_t n) {
    if (n == 0) return;
    uint16_t start = 0;
    uint16_t end = n - 1U;
    while (start < end) {
        char temp = s[start];
        s[start] = s[end];
        s[end] = temp;
        start++;
        end--;
    }
}

static uint16_t countVowels(const char *s) {
    uint16_t count = 0;
    uint16_t len = textLength(s);
    for (uint16_t i = 0; i < len; i++) {
        char lowerC = (char)tolower((unsigned char)s[i]);
        if (lowerC == 'a' || lowerC == 'e' || lowerC == 'i' || lowerC == 'o' || lowerC == 'u') {
            count++;
        }
    }
    return count;
}

static uint8_t isPalindrome(const char *s) {
    uint16_t len = textLength(s);
    if (len == 0) return 1U;

    int16_t left = 0;
    int16_t right = (int16_t)len - 1;

    while (left < right) {
        while (left < right && !isalpha((unsigned char)s[left])) {
            left++;
        }
        while (left < right && !isalpha((unsigned char)s[right])) {
            right--;
        }

        if (tolower((unsigned char)s[left]) != tolower((unsigned char)s[right])) {
            return 0U;
        }
        left++;
        right--;
    }
    return 1U;
}

static void letterHistogram(const char *s) {
    uint16_t counts[ALPHABET] = {0};
    uint16_t len = textLength(s);

    for (uint16_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)s[i])) {
            char lowerC = (char)tolower((unsigned char)s[i]);
            counts[lowerC - 'a']++;
        }
    }

    printf("\n--- Letter Histogram ---\n");
    for (uint8_t i = 0; i < ALPHABET; i++) {
        if (counts[i] > 0) {
            printf("%c: ", 'a' + i);
            for (uint16_t j = 0; j < counts[i]; j++) {
                printf("#");
            }
            printf(" (%u)\n", counts[i]);
        }
    }
}

// Print framed status card
static void showMessage(void) {
    uint16_t len = textLength(message);
    uint16_t vowels = countVowels(message);

    printf("\n+--------------------------------------------------+\n");
    printf("| Message: %-39s |\n", message);
    printf("| Length: %-5u | Shift Key: %-2u | Vowels: %-5u |\n", len, shiftKey, vowels);
    printf("| Is Palindrome: %-33s |\n", isPalindrome(message) ? "YES" : "NO");
    printf("+--------------------------------------------------+\n");
}

int main(void) {
    message[0] = '\0';

    int choice;
    while (1) {
        printf("\n=== SECRET MESSAGE MACHINE ===\n");
        printf("1. Enter / Change Message\n");
        printf("2. Set Shift Key (Current: %u)\n", shiftKey);
        printf("3. Show Framed Message Card\n");
        printf("4. Encode Message\n");
        printf("5. Decode Message\n");
        printf("6. Reverse Message In-Place\n");
        printf("7. Show Letter Histogram\n");
        printf("8. Exit\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            readMessage();
        } else if (choice == 2) {
            int keyInput;
            printf("Enter shift key (0-25): ");
            if (scanf("%d", &keyInput) == 1 && keyInput >= 0) {
                shiftKey = (uint8_t)(keyInput % ALPHABET);
            }
        } else if (choice == 3) {
            showMessage();
        } else if (choice == 4) {
            encode();
        } else if (choice == 5) {
            decode();
        } else if (choice == 6) {
            uint16_t len = textLength(message);
            reverseText(message, len);
            printf("Reversed Message: %s\n", message);
        } else if (choice == 7) {
            letterHistogram(message);
        } else if (choice == 8) {
            printf("Goodbye!\n");
            break;
        }
    }

    return 0;
}