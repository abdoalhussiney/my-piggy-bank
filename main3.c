// Student Name: [ABDULRHMAN HISHAM ADBULLAH]
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CANDY_KINDS 6U
#define BASKET_MAX  8U
#define NAME_LEN    16U

typedef struct {
    char name[NAME_LEN];
    uint16_t price;   
    uint16_t stock;  
    uint16_t sold;    
} Candy_t;

typedef struct {
    uint8_t candyId;
    uint8_t qty;
} Line_t;

static Candy_t shelf[CANDY_KINDS];
static Line_t basket[BASKET_MAX];
static uint8_t basketLines;   
static uint32_t cashDrawer;

static void openShop(void);
static void showShelf(void);
static void addToBasket(void);
static void removeFromBasket(void);
static uint32_t basketTotal(void);
static void showBasket(void);
static void checkout(void);
static void giveChange(uint32_t change);
static uint8_t bestSeller(void);
static void dayReport(void);

// Fill shelf with 6 candies, clear sold and cash drawer
static void openShop(void) {
    cashDrawer = 0;
    basketLines = 0;

    const char* names[CANDY_KINDS] = {"Chocolate", "Gummy Bear", "Lollipop", "Toffee", "Bubblegum", "Jelly Bean"};
    uint16_t prices[CANDY_KINDS] = {500, 250, 150, 200, 100, 300};
    uint16_t stocks[CANDY_KINDS] = {20, 15, 30, 25, 40, 10};

    for (int i = 0; i < CANDY_KINDS; i++) {
        strncpy(shelf[i].name, names[i], NAME_LEN - 1);
        shelf[i].name[NAME_LEN - 1] = '\0';
        shelf[i].price = prices[i];
        shelf[i].stock = stocks[i];
        shelf[i].sold = 0;
    }
}

static void showShelf(void) {
    printf("\n%-4s | %-15s | %-10s | %-10s\n", "ID", "Name", "Price", "Stock");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < CANDY_KINDS; i++) {
        if (shelf[i].stock == 0) {
            printf("%-4d | %-15s | %-7u PT | SOLD OUT\n", i, shelf[i].name, shelf[i].price);
        } else {
            printf("%-4d | %-15s | %-7u PT | %-10u\n", i, shelf[i].name, shelf[i].price, shelf[i].stock);
        }
    }
}

static void addToBasket(void) {
    int id, qty;
    printf("Enter Candy ID (0-%u): ", CANDY_KINDS - 1);
    if (scanf("%d", &id) != 1 || id < 0 || id >= CANDY_KINDS) {
        printf("Invalid Candy ID.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Quantity: ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf("Invalid quantity.\n");
        while (getchar() != '\n');
        return;
    }

    uint16_t inBasketQty = 0;
    int existingLine = -1;
    for (int i = 0; i < basketLines; i++) {
        if (basket[i].candyId == (uint8_t)id) {
            inBasketQty = basket[i].qty;
            existingLine = i;
            break;
        }
    }

    if (shelf[id].stock < (inBasketQty + qty)) {
        printf("Not enough stock on shelf!\n");
        return;
    }

    if (existingLine != -1) {
        basket[existingLine].qty += (uint8_t)qty;
        printf("Updated quantity in basket.\n");
    } else {
        if (basketLines >= BASKET_MAX) {
            printf("Basket is full!\n");
            return;
        }
        basket[basketLines].candyId = (uint8_t)id;
        basket[basketLines].qty = (uint8_t)qty;
        basketLines++;
        printf("Added to basket.\n");
    }
}

static void removeFromBasket(void) {
    if (basketLines == 0) {
        printf("Basket is empty.\n");
        return;
    }

    showBasket();
    int line;
    printf("Enter Basket Line Number to remove (0-%u): ", basketLines - 1);
    if (scanf("%d", &line) != 1 || line < 0 || line >= basketLines) {
        printf("Invalid line.\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = line; i < basketLines - 1; i++) {
        basket[i] = basket[i + 1];
    }
    basketLines--;
    printf("Line removed successfully.\n");
}

static uint32_t basketTotal(void) {
    uint32_t total = 0;
    for (int i = 0; i < basketLines; i++) {
        uint8_t id = basket[i].candyId;
        total += (uint32_t)shelf[id].price * basket[i].qty;
    }
    return total;
}

static void showBasket(void) {
    printf("\n--- CURRENT BASKET ---\n");
    if (basketLines == 0) {
        printf("Basket is empty.\n");
        return;
    }

    printf("%-5s | %-15s | %-5s | %-8s | %-8s\n", "Line", "Name", "Qty", "Price", "Cost");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < basketLines; i++) {
        uint8_t id = basket[i].candyId;
        uint32_t cost = (uint32_t)shelf[id].price * basket[i].qty;
        printf("%-5d | %-15s | %-5u | %-6u PT | %-6u PT\n", i, shelf[id].name, basket[i].qty, shelf[id].price, cost);
    }
    printf("----------------------------------------------------\n");
    printf("Total Cost: %u PT\n", basketTotal());
}

// Change calculation using standard coin denominations (500, 200, 100, 50, 25)
static void giveChange(uint32_t change) {
    if (change == 0) {
        printf("No change, thank you.\n");
        return;
    }

    printf("Change Breakdown (%u PT):\n", change);
    uint32_t coins[] = {500, 200, 100, 50, 25};
    
    for (int i = 0; i < 5; i++) {
        uint32_t count = change / coins[i];
        if (count > 0) {
            printf("- %u x %u PT coin(s)\n", count, coins[i]);
            change %= coins[i];
        }
    }

    if (change > 0) {
        printf("Remaining %u PT cannot be given with available coin denominations.\n", change);
    }
}

// Process checkout transaction
static void checkout(void) {
    uint32_t total = basketTotal();
    if (total == 0) {
        printf("Basket is empty! Nothing to checkout.\n");
        return;
    }

    printf("Total due: %u PT\n", total);
    uint32_t paid;
    printf("Enter money handed over (in PT): ");
    if (scanf("%u", &paid) != 1 || paid < total) {
        printf("Payment insufficient! Basket untouched.\n");
        while (getchar() != '\n');
        return;
    }

    // Deduct stock and increment sold
    for (int i = 0; i < basketLines; i++) {
        uint8_t id = basket[i].candyId;
        shelf[id].stock -= basket[i].qty;
        shelf[id].sold += basket[i].qty;
    }

    cashDrawer += total;
    giveChange(paid - total);

    basketLines = 0; // Empty basket
    printf("Checkout completed successfully!\n");
}

// Find position of best selling candy
static uint8_t bestSeller(void) {
    uint8_t bestIdx = 0;
    uint16_t maxSold = 0;
    for (int i = 0; i < CANDY_KINDS; i++) {
        if (shelf[i].sold > maxSold) {
            maxSold = shelf[i].sold;
            bestIdx = i;
        }
    }
    return bestIdx;
}

// Print daily report
static void dayReport(void) {
    printf("\n========= END OF DAY REPORT =========\n");
    printf("Money in Cash Drawer: %u PT\n", cashDrawer);

    uint32_t totalCandiesSold = 0;
    for (int i = 0; i < CANDY_KINDS; i++) {
        totalCandiesSold += shelf[i].sold;
    }
    printf("Total Candies Sold: %u\n", totalCandiesSold);

    uint8_t best = bestSeller();
    if (shelf[best].sold > 0) {
        printf("Best Seller: %s (%u sold)\n", shelf[best].name, shelf[best].sold);
    } else {
        printf("Best Seller: None (0 sold)\n");
    }

    printf("\nSold Out Items:\n");
    int soldOutCount = 0;
    for (int i = 0; i < CANDY_KINDS; i++) {
        if (shelf[i].stock == 0) {
            printf("- %s\n", shelf[i].name);
            soldOutCount++;
        }
    }
    if (soldOutCount == 0) {
        printf("None\n");
    }
    printf("======================================\n");
}

int main(void) {
    openShop();

    int choice;
    while (1) {
        printf("\n--- CANDY SHOP CASHIER ---\n");
        printf("1. Show Shelf\n");
        printf("2. Add Candy to Basket\n");
        printf("3. Remove Line from Basket\n");
        printf("4. Show Basket\n");
        printf("5. Checkout\n");
        printf("6. End of Day Report\n");
        printf("7. Exit\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            showShelf();
        } else if (choice == 2) {
            addToBasket();
        } else if (choice == 3) {
            removeFromBasket();
        } else if (choice == 4) {
            showBasket();
        } else if (choice == 5) {
            checkout();
        } else if (choice == 6) {
            dayReport();
        } else if (choice == 7) {
            printf("Closing shop...\n");
            break;
        }
    }

    return 0;
}