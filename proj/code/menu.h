#ifndef _MENU_H
#define _MENU_H



/** @defgroup State Machine State Machine
 * @{
 *
 * Menu State machine declaration
 */

typedef enum {InitialMenu, LocalGameMenu, TwoPCMode, Exit} state_t; /**< @brief Current Menu State */

typedef enum {LocalPressed, TwoPcPressed, ExitPressed, SinglePressed, MultiPressed, BackPressed,
	LocalHilight, TwoPcHilight, ExitHilight,SingleHilight, MultiHilight, BackHilight, Nothing} event_t; /**< @brief Current Event */
state_t current_State;
event_t current_Event;

typedef enum {SinglePlayer,MultiPlayer} Play_Modes_t; /**< @brief Play mode selected */

Play_Modes_t Play_Mode;

/** @} end of state machine declaration */

/**
 * @fn	void updateCoord(unsigned short *packet)
 * @brief Updates mouse coordinates.
 *
 *	Receives the packet array and updates mouse coordinates.
 *	Verifies also if the values overcome the resolution and corrects mouse position.
 * @param packet Pointer to an array of 3 elements sent by the mouse.
 *
 */

void updateCoord(unsigned short *packet);

/**
 * @fn	int mouse_handler(unsigned short *packet)
 * @brief Calls the updateCoord() function and generates events based on the current state of the state machine.
 *
 * @param packet Pointer to an array of 3 elements sent by the mouse.
 */

int mouse_handler(unsigned short *packet);

/**
 * @fn	int machine_Handler()
 * @brief Handles the machine current state and current event.
 *
 * Writes to buffer the necessary BMPs, based on the current state and current event.
 * Calls the functions that will change the screen to the game itself.
 * @return 1 upon failure, 0 upon success.
 */

int machine_Handler();


/**
 * @fn	int start_menu()
 * @brief Initializes State Machine and receives interrupts from mouse and timer.
 * @return 0 upon success, 1 upon failure.
 */

int start_menu();



#endif
