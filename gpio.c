#include "gpio.h"
extern int arr[Maxpins];

// Simulates an 8-bit hardware register (PORT)
// Each bit represents one pin (Bit 0 for Pin 0, Bit 1 for Pin 1, etc.)
unsigned char PORT_REGISTER = 0x00;

// Shows the main menu on the screen
void menu()
{
    printf("+===============================+\n");
    printf("|\tConfiguration MENU\t|\n");
    printf("+===============================+\n");
    printf("|1.Configure GPIO pin\t\t|\n"
           "|2.Display ALL pins\t\t|\n"
           "|3.Search GPIO pin\t\t|\n"
           "|4.Change Pin mode\t\t|\n"
           "|5.Set Pin value\t\t|\n"
           "|6.Enable/Disable Pullup\t|\n"
           "|7.Fault Detection\t\t|\n"
           "|8.Exit\t\t\t\t|\n");
    printf("+===============================+\n");
}

// Sets up a pin's mode, value, and pullup, then updates the register
void configureGpioPin(GPIO pins[])
{
    int pin, mode, value, pullup;

    printf("Enter the GPIO Pin number\n");
    scanf("%d", &pin);

    // Check if the entered pin number is valid
    if (pin < 0 || pin >= Maxpins)
    {
        printf("Enter Valid GPIO pin (0 to %d)\n", Maxpins - 1);
        return;
    }

    pins[pin].pin_number = pin;

    printf("Enter the GPIO Pin Mode (0 = Input 1 = Output)\n");
    scanf("%d", &mode);

    printf("Enter the GPIO Pin Value (0 = Low 1 = High)\n");
    scanf("%d", &value);

    // Safety rule: An INPUT pin cannot be given a HIGH output state
    if (mode == GPIO_MODE_INPUT && value == 1) {
        printf("\n[FAULT ERROR] Cannot write HIGH state to an INPUT pin!\n");
        printf("[SYSTEM SAFETY] Automatically forcing initial pin state to LOW.\n");
        value = 0; // Reset to LOW for safety
    }

    printf("Enter the Pullup (0 = Disabled 1 = Enabled)\n");
    scanf("%d", &pullup);

    // Save user choices into the pin structure
    pins[pin].pin_mode = mode;
    pins[pin].pin_value = value;
    pins[pin].pullup = pullup;

    // Update the hardware register bit: 1 for HIGH output, 0 for LOW
    if (pins[pin].pin_mode == GPIO_MODE_OUTPUT && pins[pin].pin_value == GPIO_STATE_HIGH) {
        PORT_REGISTER |= (1 << pin);  // Turn bit ON
    } else {
        PORT_REGISTER &= ~(1 << pin); // Turn bit OFF
    }

    // Print the register in Hex and Binary format
    printf("HARDWARE REGISTER (PORT): 0x%02X  [Binary: ", PORT_REGISTER);
    for (int i = 7; i >= 0; i--) {
        printf("%d", (PORT_REGISTER >> i) & 1);
    }
    printf("]\n");

    printf("[SUCCESS] Pin %d configured successfully!\n", pin);
}

// Prints the current state of all pins in a table
void displayAllPins(GPIO pins[])
{
    printf("%-10s %-10s %-10s %-10s\n", "GPIO PIN", "MODE", "VALUE", "PULLUP");
    for (int i = 0; i < Maxpins; i++)
    {
        printf("%-10d %-10d %-10d %-10d\n", i, pins[i].pin_mode, pins[i].pin_value, pins[i].pullup);
    }

    // Print the current register value
    printf("HARDWARE REGISTER (PORT): 0x%02X  [Binary: ", PORT_REGISTER);
    for (int i = 7; i >= 0; i--) {
        printf("%d", (PORT_REGISTER >> i) & 1);
    }
    printf("]\n");
}

// Sets all pins to default safe values at startup
void initialize(GPIO pins[])
{
    for (int i = 0; i < Maxpins; i++)
    {
        pins[i].pin_number = i;
        pins[i].pin_mode = GPIO_MODE_INPUT;
        pins[i].pin_value = GPIO_STATE_LOW;
        pins[i].pullup = PULL_DISABLED;
    }
}

// Finds and shows details of a single pin
void search_pin(GPIO pins[])
{
    int pin;
    char mode[10], value[10], pullup[10];

    printf("Enter the GPIO pin number : ");
    scanf("%d", &pin);

    // Check if the pin exists
    if (pin < 0 || pin >= Maxpins)
    {
        printf("Enter valid pin 0 to %d\n", Maxpins - 1);
        return;
    }

    printf("\n--- PIN %d DETAILS ---\n", pin);
    strcpy(mode, pins[pin].pin_mode ? "OUTPUT" : "INPUT");
    strcpy(value, pins[pin].pin_value ? "HIGH" : "LOW");
    strcpy(pullup, pins[pin].pullup ? "ENABLED" : "DISABLED");

    printf("PIN NUMBER  = %d\nPIN MODE    = %s\nPIN VALUE   = %s\nPULLUP MODE = %s\n", 
           pin, mode, value, pullup);
}

// Changes pin mode between INPUT and OUTPUT
void changeMode(GPIO pins[]) {
    int pin, mode_input;
    char str[10];

    printf("\nEnter Pin Number: ");
    scanf("%d", &pin);

    if (pin < 0 || pin >= Maxpins) {
        printf("[ERROR] Invalid Pin!\n");
        return;
    }

    printf("Select New Mode (0: INPUT, 1: OUTPUT): ");
    scanf("%d", &mode_input);

    if (mode_input < 0 || mode_input > 1) {
        printf("[ERROR] Invalid Mode selection!\n");
        return;
    }

    // Safety rule: Reset pin to LOW before changing it to INPUT
    if (mode_input == 0 && pins[pin].pin_value == 1)
    {
        printf("[ERROR] Invalid Mode selection! Pin is currently HIGH!\n");
        printf("[SYSTEM SAFETY] Changing pin value to LOW before setting to INPUT.\n");
        pins[pin].pin_value = 0;
        PORT_REGISTER &= ~(1 << pin); // Clear bit in register
    }

    pins[pin].pin_mode = mode_input;
    pins[pin].pin_mode ? strcpy(str, "OUTPUT") : strcpy(str, "INPUT");
    printf("[SUCCESS] Pin %d mode changed to %s.\n", pin, str);
}

// Sets the pin value to HIGH or LOW
void setPinValue(GPIO pins[]) {
    int pin, new_state, prev_state;
    char prev[10], new[10];

    printf("\nEnter Pin Number: ");
    scanf("%d", &pin);

    if (pin < 0 || pin >= Maxpins) {
        printf("[ERROR] Invalid Pin!\n");
        return;
    }

    // Cannot write a value to an INPUT pin
    if (pins[pin].pin_mode == GPIO_MODE_INPUT) {
        printf("\n[FAULT ERROR] Cannot write state to an INPUT pin!\n");
        return;
    }

    printf("Select Value (0: LOW, 1: HIGH): ");
    scanf("%d", &new_state);

    prev_state = pins[pin].pin_value;
    pins[pin].pin_value = new_state ? GPIO_STATE_HIGH : GPIO_STATE_LOW;

    strcpy(prev, prev_state ? "HIGH" : "LOW");
    strcpy(new, new_state ? "HIGH" : "LOW");

    // Check if state changed and trigger an alert
    if (prev_state != new_state) {
        printf("\n>>> [INTERRUPT TRIGGERED] State transition on Pin %d: %s -> %s <<<\n", pin, prev, new);
    }

    // Update the bit in the register
    if (pins[pin].pin_value == GPIO_STATE_HIGH) {
        PORT_REGISTER |= (1 << pin);  // Set bit to 1
    } else {
        PORT_REGISTER &= ~(1 << pin); // Clear bit to 0
    }

    printf("[SUCCESS] Pin %d value updated to %s.\n", pin, new);
    printf("[HARDWARE SYNC] Register PORT updated: 0x%02X\n", PORT_REGISTER);
}

// Turns the pullup resistor ON or OFF
void togglePullup(GPIO pins[]) {
    int pin, status;
    char str[10];

    printf("\nEnter Pin Number: ");
    scanf("%d", &pin);

    if (pin < 0 || pin >= Maxpins) {
        printf("[ERROR] Invalid Pin!\n");
        return;
    }

    printf("Enable Pullup? (1 = Enable, 0 = Disable): ");
    scanf("%d", &status);

    pins[pin].pullup = status;
    status ? strcpy(str, "ENABLE") : strcpy(str, "DISABLE");
    printf("[SUCCESS] Pullup resistor on Pin %d set to %s.\n", pin, str);
}

// Checks for errors like floating input pins
void faultDetection(GPIO pins[]) {
    printf("\n=== FAULT DIAGNOSTIC REPORT ===\n");
    int faults = 0;

    for (int i = 0; i < Maxpins; i++) {
        // Warn if a pin is INPUT but has no pullup resistor (floating pin)
        if (pins[i].pin_mode == GPIO_MODE_INPUT && pins[i].pullup == PULL_DISABLED) {
            printf("[WARNING] Pin %d is FLOATING (Input with Pull-up Disabled).\n", i);
            faults++;
        }
    }

    if (!faults) {
        printf("[SYSTEM OK] All pins operating nominally.\n");
    }
    printf("================================\n");
}