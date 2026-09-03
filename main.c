//Student Name: [ABDULRHMAN HISHAM ADBULLAH]
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COIN_KINDS 5U
#define TOY_COUNT 4U
#define NAME_LEN 16U

/* Coin values in piastres: 25, 50, 100, 200, 500 */
static const uint16_t COIN_VALUE[COIN_KINDS] = {25U, 50U, 100U, 200U, 500U};
static uint16_t coinCount[COIN_KINDS];

typedef struct {
    char name[NAME_LEN];
    uint32_t price;
} Toy_t;

static Toy_t shop[TOY_COUNT];

static void seedBank(void);
static void addCoins(void);
static void takeCoins(void);
static uint32_t bankTotal(void);
static uint32_t sumCoins(const uint16_t *counts, uint8_t n);
static uint8_t biggestPile(void);
static void showBank(void);
static void drawBar(uint16_t value, uint16_t full, uint8_t width);
static void buyToy(void);
static void bankReport(void);

static int readInt(int *value)
{
    int result = scanf("%d", value);
    int ch;

    if (result == 1)
        return 1;

    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    return 0;
}

static void seedBank(void)
{
    uint16_t start[COIN_KINDS] = {8U, 6U, 5U, 3U, 2U};

    memcpy(coinCount, start, sizeof(coinCount));

    strcpy(shop[0].name, "Car");
    shop[0].price = 300U;
    strcpy(shop[1].name, "Ball");
    shop[1].price = 450U;
    strcpy(shop[2].name, "Robot");
    shop[2].price = 750U;
    strcpy(shop[3].name, "Puzzle");
    shop[3].price = 1000U;
}

static void addCoins(void)
{
    int kind, amount;

    printf("Coin (0-4): ");
    if (!readInt(&kind) || kind < 0 || kind >= (int)COIN_KINDS) {
        printf("Invalid coin.\n");
        return;
    }

    printf("How many: ");
    if (!readInt(&amount) || amount < 0 || amount > UINT16_MAX - coinCount[kind]) {
        printf("Invalid amount.\n");
        return;
    }

    coinCount[kind] += (uint16_t)amount;
    printf("Coins added.\n");
}

static void takeCoins(void)
{
    int kind, amount;

    printf("Coin (0-4): ");
    if (!readInt(&kind) || kind < 0 || kind >= (int)COIN_KINDS) {
        printf("Invalid coin.\n");
        return;
    }

    printf("How many: ");
    if (!readInt(&amount) || amount < 0 || (uint32_t)amount > coinCount[kind]) {
        printf("Not enough coins. Nothing taken.\n");
        return;
    }

    coinCount[kind] -= (uint16_t)amount;
    printf("Coins taken.\n");
}

static uint32_t bankTotal(void)
{
    uint32_t total = 0U;
    uint8_t i;

    for (i = 0U; i < COIN_KINDS; ++i)
        total += (uint32_t)coinCount[i] * COIN_VALUE[i];

    return total;
}

static uint32_t sumCoins(const uint16_t *counts, uint8_t n)
{
    if (n == 0U)
        return 0U;

    return (uint32_t)counts[n - 1U] + sumCoins(counts, (uint8_t)(n - 1U));
}

static uint8_t biggestPile(void)
{
    uint8_t best = 0U, i;

    for (i = 1U; i < COIN_KINDS; ++i)
        if (coinCount[i] > coinCount[best])
            best = i;

    return best;
}

static void drawBar(uint16_t value, uint16_t full, uint8_t width)
{
    uint8_t filled, i;

    if (full == 0U)
        filled = 0U;
    else if (value >= full)
        filled = width;
    else
        filled = (uint8_t)(((uint32_t)value * width) / full);

    for (i = 0U; i < filled; ++i)
        putchar('#');
    for (; i < width; ++i)
        putchar('-');
}

static void showBank(void)
{
    uint8_t i, biggest = biggestPile();
    uint16_t full = coinCount[biggest];

    printf("\n--- Piggy Bank ---\n");
    for (i = 0U; i < COIN_KINDS; ++i) {
        printf("%u piastres: %u  ", COIN_VALUE[i], coinCount[i]);
        drawBar(coinCount[i], full, 20U);
        putchar('\n');
    }
}

static void buyToy(void)
{
    int choice;
    uint32_t total, price;

    printf("\nToys:\n");
    for (uint8_t i = 0U; i < TOY_COUNT; ++i)
        printf("%u. %s - %u piastres\n", i + 1U, shop[i].name, shop[i].price);

    printf("Choose a toy (1-4): ");
    if (!readInt(&choice) || choice < 1 || choice > (int)TOY_COUNT) {
        printf("Invalid toy.\n");
        return;
    }

    price = shop[choice - 1].price;
    total = bankTotal();

    if (total >= price)
        printf("You can buy it. %u piastres left over.\n", total - price);
    else
        printf("You need %u more piastres.\n", price - total);
}

static void bankReport(void)
{
    uint8_t best = biggestPile();

    printf("\n=== Bank Report ===\n");
    printf("Total money: %u piastres\n", bankTotal());
    printf("Total coins: %u\n", sumCoins(coinCount, COIN_KINDS));
    printf("Biggest pile: coin %u (%u piastres), %u coins\n",
           best, COIN_VALUE[best], coinCount[best]);
}

int main(void)
{
    int choice;

    seedBank();

    do {
        printf("\n=== My Piggy Bank ===\n");
        printf("1. Add coins\n");
        printf("2. Take coins\n");
        printf("3. Show bank\n");
        printf("4. Buy toy\n");
        printf("5. Bank report\n");
        printf("6. New game\n");
        printf("0. Exit\n");
        printf("Choose: ");

        if (!readInt(&choice)) {
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: addCoins(); break;
            case 2: takeCoins(); break;
            case 3: showBank(); break;
            case 4: buyToy(); break;
            case 5: bankReport(); break;
            case 6: seedBank(); printf("Bank reset.\n"); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n"); break;
        }
    } while (choice != 0);

    return 0;
}