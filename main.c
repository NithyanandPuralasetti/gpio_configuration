
#include "gpio.h"

// Global array for pin references
int arr[Maxpins];

int main()
{
    // Create the array of pins and set them to startup defaults
    GPIO pins[Maxpins];
    initialize(pins);

    int choice = 0;

    do
    {
        // Run the action chosen by the user in the previous turn
        if (choice != 0)
        {
            switch (choice)
            {
                case 1:
                    configureGpioPin(pins); // Set pin direction, state, and pullup
                    break;
                case 2:
                    displayAllPins(pins);   // Print table of all pin states
                    break;
                case 3:
                    search_pin(pins);       // Look up one specific pin
                    break;
                case 4:
                    changeMode(pins);       // Change between INPUT and OUTPUT
                    break;
                case 5:
                    setPinValue(pins);      // Write HIGH or LOW to an output pin
                    break;
                case 6:
                    togglePullup(pins);     // Enable or disable the pullup resistor
                    break;
                case 7:
                    faultDetection(pins);   // Scan for floating input pins
                    break;
                case 8:
                    printf("EXITING.....\n");
                    exit(0);
                    break;
                default:
                    printf("!!! Invalid choice Enter again !!! \n");
                    break;
            }
        }

        // Show the menu options
        menu();
        printf("Enter your choice : ");

        // Read user input safely and clear input buffer to avoid infinite loops
        if (scanf("%d", &choice) != 1) 
        {
            int c;
            // Clear invalid characters like letters from the buffer
            while ((c = getchar()) != '\n' && c != EOF);
            choice = -1; // Set to invalid so it triggers the default case
        } 
        else 
        {
            // Clear leftover newline character from buffer
            while ((getchar()) != '\n'); 
        }

    } while (choice != 0);

    return 0;
}