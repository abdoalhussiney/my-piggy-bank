//Student Name: [ABDULRHMAN HISHAM ADBULLAH]
#include <stdio.h>
#include <stdint.h>

#define TICKS_GREEN   5U
#define TICKS_YELLOW  2U
#define TICKS_RED     4U
#define QUEUE_BUSY    6U
#define LOG_LEN       20U

typedef enum { LIGHT_GREEN = 0, LIGHT_YELLOW, LIGHT_RED } LightState_t;

#define BIT_NIGHT     0U
#define BIT_BUSY      1U
#define BIT_BLINK_ON  2U

#define SET_BIT(reg, n)     ((reg) |= (uint8_t)(1U << (n)))
#define CLR_BIT(reg, n)     ((reg) &= (uint8_t)~(1U << (n)))
#define TOGGLE_BIT(reg, n)  ((reg) ^= (uint8_t)(1U << (n)))
#define READ_BIT(reg, n)    ((uint8_t)(((reg) >> (n)) & 1U))

static LightState_t light;
static uint8_t status;
static uint8_t ticksLeft;
static uint8_t carsWaiting;
static uint32_t carsPassed;
static char logLine[LOG_LEN];
static uint32_t totalTicks;

static void resetCrossing(void);
static uint8_t ticksFor(LightState_t s);
static LightState_t nextState(LightState_t s);
static void drawLight(void);
static void tick(void);
static void addCars(void);
static void toggleNight(void);
static void pushLog(char c);
static void showLog(void);
static void crossingReport(void);

static void resetCrossing(void) {
    light = LIGHT_RED;
    status = 0;
    carsWaiting = 0;
    carsPassed = 0;
    totalTicks = 0;
    
    for (uint8_t i = 0U; i < LOG_LEN; ++i) {
        logLine[i] = ' ';
    }
    
    ticksLeft = ticksFor(light);
}

static uint8_t ticksFor(LightState_t s) {
    if (s == LIGHT_GREEN) {
        return READ_BIT(status, BIT_BUSY) ? (TICKS_GREEN + 2U) : TICKS_GREEN;
    } else if (s == LIGHT_YELLOW) {
        return TICKS_YELLOW;
    } else if (s == LIGHT_RED) {
        return TICKS_RED;
    }
    return 0;
}

static LightState_t nextState(LightState_t s) {
    switch (s) {
        case LIGHT_GREEN:  return LIGHT_YELLOW;
        case LIGHT_YELLOW: return LIGHT_RED;
        case LIGHT_RED:    return LIGHT_GREEN;
        default:           return LIGHT_GREEN;
    }
}

static void pushLog(char c) {
    for (uint8_t i = 0U; i < LOG_LEN - 1U; ++i) {
        logLine[i] = logLine[i + 1U];
    }
    logLine[LOG_LEN - 1U] = c;
}

static void showLog(void) {
    printf("Log History: [");
    for (uint8_t i = 0U; i < LOG_LEN; ++i) {
        putchar(logLine[i]);
    }
    printf("]\n");
}

static void drawLight(void) {
    printf("\n--- TRAFFIC LIGHT ---\n");
    if (READ_BIT(status, BIT_NIGHT)) {
        printf(" ( ) GREEN\n");
        printf(" (%c) YELLOW (BLINK)\n", READ_BIT(status, BIT_BLINK_ON) ? '*' : ' ');
        printf(" ( ) RED\n");
        printf("Mode: NIGHT | Cars Waiting: %u\n", carsWaiting);
    } else {
        printf(" (%c) GREEN\n", (light == LIGHT_GREEN) ? 'O' : ' ');
        printf(" (%c) YELLOW\n", (light == LIGHT_YELLOW) ? 'O' : ' ');
        printf(" (%c) RED\n", (light == LIGHT_RED) ? 'O' : ' ');
        
        const char* colorStr = (light == LIGHT_GREEN) ? "GREEN" : (light == LIGHT_YELLOW) ? "YELLOW" : "RED";
        printf("Color: %s | Ticks Left: %u | Cars Waiting: %u\n", colorStr, ticksLeft, carsWaiting);
    }
}

static void tick(void) {
    totalTicks++;

    if (READ_BIT(status, BIT_NIGHT)) {
        TOGGLE_BIT(status, BIT_BLINK_ON);
        pushLog('y');
    } else {
        if (light == LIGHT_GREEN) {
            pushLog('G');
            if (carsWaiting > 0) {
                uint8_t passing = (carsWaiting >= 2) ? 2 : carsWaiting;
                carsWaiting -= passing;
                carsPassed += passing;
            }
        } else if (light == LIGHT_YELLOW) {
            pushLog('Y');
        } else if (light == LIGHT_RED) {
            pushLog('R');
        }

        if (carsWaiting > QUEUE_BUSY) {
            SET_BIT(status, BIT_BUSY);
        } else {
            CLR_BIT(status, BIT_BUSY);
        }

        ticksLeft--;
        if (ticksLeft == 0) {
            light = nextState(light);
            ticksLeft = ticksFor(light);
        }
    }
}

static void addCars(void) {
    int num;
    printf("Enter number of cars arrived: ");
    if (scanf("%d", &num) == 1 && num > 0) {
        carsWaiting += (uint8_t)num;
        if (carsWaiting > QUEUE_BUSY) {
            SET_BIT(status, BIT_BUSY);
        }
    } else {
        printf("Invalid input. Ignored.\n");
        while (getchar() != '\n');
    }
}

static void toggleNight(void) {
    TOGGLE_BIT(status, BIT_NIGHT);
    if (READ_BIT(status, BIT_NIGHT)) {
        SET_BIT(status, BIT_BLINK_ON);
    } else {
        CLR_BIT(status, BIT_BLINK_ON);
        light = LIGHT_RED;
        ticksLeft = ticksFor(light);
    }
}

static void crossingReport(void) {
    printf("\n========= CROSSING REPORT =========\n");
    printf("Total Ticks: %u\n", totalTicks);
    printf("Cars Passed: %u\n", carsPassed);
    printf("Cars Waiting: %u\n", carsWaiting);
    printf("Night Mode: %s\n", READ_BIT(status, BIT_NIGHT) ? "YES" : "NO");
    printf("Busy State: %s\n", READ_BIT(status, BIT_BUSY) ? "YES" : "NO");
    
    printf("Status Byte (HEX): 0x%02X\n", status);
    printf("Status Byte (BIN): ");
    for (int i = 7; i >= 0; i--) {
        printf("%d", READ_BIT(status, i));
    }
    printf("\n===================================\n");
}

int main(void) {
    resetCrossing();

    int choice;
    while (1) {
        drawLight();
        showLog();

        printf("\n--- Interactive Menu ---\n");
        printf("1. Pass 1 Tick\n");
        printf("2. Add Arriving Cars\n");
        printf("3. Toggle Night Mode\n");
        printf("4. Show Crossing Report\n");
        printf("5. Exit\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            tick();
        } else if (choice == 2) {
            addCars();
        } else if (choice == 3) {
            toggleNight();
        } else if (choice == 4) {
            crossingReport();
        } else if (choice == 5) {
            printf("Exiting program...\n");
            break;
        }
    }
    return 0;
}