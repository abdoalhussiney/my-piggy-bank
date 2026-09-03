//Student Name: [ABDULRHMAN HISHAM ADBULLAH]
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define STAT_MAX     10U
#define LOW_WARNING  3U
#define NAME_LEN     12U

/* Mood bits */
#define BIT_ASLEEP   0U
#define BIT_HUNGRY   1U
#define BIT_SAD      2U
#define BIT_SICK     3U

/* Bit Manipulation Macros */
#define SET_BIT(reg, n)   ((reg) |= (uint8_t)(1U << (n)))
#define CLR_BIT(reg, n)   ((reg) &= (uint8_t)~(1U << (n)))
#define READ_BIT(reg, n)  ((uint8_t)(((reg) >> (n)) & 1U))

typedef struct {
    char name[NAME_LEN];
    uint8_t food;     
    uint8_t fun;      
    uint8_t energy;     
    uint8_t mood;     /* the bits above */
    uint16_t hours;   /* how long Fluffy has been alive */
} Pet_t;

static Pet_t fluffy;

static void hatchPet(void);
static void drawFace(void);
static void drawStat(const char *label, uint8_t value);
static void feed(void);
static void play(void);
static void sleepPet(void);
static void hourPasses(void);
static void updateMood(void);
static uint8_t isHappy(void);
static void petReport(void);

static void hatchPet(void) {
    strncpy(fluffy.name, "Fluffy", NAME_LEN - 1);
    fluffy.name[NAME_LEN - 1] = '\0';
    fluffy.food = STAT_MAX;
    fluffy.fun = STAT_MAX;
    fluffy.energy = STAT_MAX;
    fluffy.mood = 0;
    fluffy.hours = 0;
}

//Draw Fluffy's face according to highest priority mood
static void drawFace(void) {
    printf("+-------+\n");
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("|  zZz  |\n|  -_-  |\n");
    } else if (READ_BIT(fluffy.mood, BIT_SICK)) {
        printf("|  x_x  |\n|   ~   |\n");
    } else if (READ_BIT(fluffy.mood, BIT_SAD) || READ_BIT(fluffy.mood, BIT_HUNGRY)) {
        printf("|  o_o  |\n|   (   |\n");
    } else {
        printf("|  ^_^  |\n|   v   |\n");
    }
    printf("+-------+\n");
}

static void drawStat(const char *label, uint8_t value) {
    printf("%-7s [", label);
    for (uint8_t i = 0; i < STAT_MAX; i++) {
        if (i < value) {
            printf("#");
        } else {
            printf(".");
        }
    }
    printf("] %u/%u\n", value, STAT_MAX);
}

// Feed Fluffy
static void feed(void) {
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("%s is sleeping! Wake it up first.\n", fluffy.name);
        return;
    }

    if (fluffy.food >= STAT_MAX) {
        SET_BIT(fluffy.mood, BIT_SICK);
        printf("%s ate too much and got sick!\n", fluffy.name);
    } else {
        fluffy.food += 3U;
        if (fluffy.food > STAT_MAX) {
            fluffy.food = STAT_MAX;
        }
        printf("%s enjoyed the food!\n", fluffy.name);
    }
}

// Play with Fluffy
static void play(void) {
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("%s is sleeping and cannot play.\n", fluffy.name);
        return;
    }

    fluffy.fun += 3U;
    if (fluffy.fun > STAT_MAX) {
        fluffy.fun = STAT_MAX;
    }

    if (fluffy.energy >= 2U) {
        fluffy.energy -= 2U;
    } else {
        fluffy.energy = 0;
    }

    printf("Played with %s!\n", fluffy.name);
}

static void sleepPet(void) {
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        CLR_BIT(fluffy.mood, BIT_ASLEEP);
        CLR_BIT(fluffy.mood, BIT_SICK); // Sickness goes away after sleep
        printf("%s woke up!\n", fluffy.name);
    } else {
        SET_BIT(fluffy.mood, BIT_ASLEEP);
        printf("%s went to sleep. zZz...\n", fluffy.name);
    }
}

static void updateMood(void) {
    // Hungry check
    if (fluffy.food <= LOW_WARNING) {
        SET_BIT(fluffy.mood, BIT_HUNGRY);
    } else {
        CLR_BIT(fluffy.mood, BIT_HUNGRY);
    }

    // Sad check
    if (fluffy.fun <= LOW_WARNING) {
        SET_BIT(fluffy.mood, BIT_SAD);
    } else {
        CLR_BIT(fluffy.mood, BIT_SAD);
    }

    // Energy / Auto-sleep check
    if (fluffy.energy == 0) {
        SET_BIT(fluffy.mood, BIT_ASLEEP);
    }
}

// Advance time by 1 hour
static void hourPasses(void) {
    fluffy.hours++;

    // Food decay
    if (fluffy.food > 0) {
        fluffy.food--;
    }

    // Fun decay
    if (fluffy.fun > 0) {
        fluffy.fun--;
    }

    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        fluffy.energy += 2U;
        if (fluffy.energy > STAT_MAX) {
            fluffy.energy = STAT_MAX;
        }
    } else {
        if (fluffy.energy > 0) {
            fluffy.energy--;
        }
    }

    updateMood();
}

// Check if happy
static uint8_t isHappy(void) {
    if (fluffy.food > LOW_WARNING && 
        fluffy.fun > LOW_WARNING && 
        fluffy.energy > LOW_WARNING && 
        fluffy.mood == 0) {
        return 1U;
    }
    return 0U;
}

static void petReport(void) {
    printf("\n============ CARE CARD ============\n");
    printf("Name: %s | Age: %u hours\n", fluffy.name, fluffy.hours);
    drawFace();
    
    drawStat("Food", fluffy.food);
    drawStat("Fun", fluffy.fun);
    drawStat("Energy", fluffy.energy);

    printf("Mood Byte (Binary): ");
    for (int8_t i = 7; i >= 0; i--) {
        printf("%u", READ_BIT(fluffy.mood, (uint8_t)i));
    }
    printf("\n");

    printf("Needs: ");
    if (READ_BIT(fluffy.mood, BIT_SICK)) {
        printf("%s is sick and needs sleep!\n", fluffy.name);
    } else if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("%s is sleeping peacefully.\n", fluffy.name);
    } else if (READ_BIT(fluffy.mood, BIT_HUNGRY)) {
        printf("%s needs food!\n", fluffy.name);
    } else if (READ_BIT(fluffy.mood, BIT_SAD)) {
        printf("%s wants to play!\n", fluffy.name);
    } else {
        printf("%s is doing great!\n", fluffy.name);
    }
    printf("===================================\n");
}

int main(void) {
    hatchPet();

    int choice;
    while (1) {
        petReport();
        printf("\n1. Feed %s\n", fluffy.name);
        printf("2. Play with %s\n", fluffy.name);
        printf("3. Sleep / Wake Up\n");
        printf("4. Pass 1 Hour\n");
        printf("5. Exit\n");
        printf("Select action: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            feed();
        } else if (choice == 2) {
            play();
        } else if (choice == 3) {
            sleepPet();
        } else if (choice == 4) {
            hourPasses();
        } else if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
    }

    return 0;
}