/*   Final_project
*       - Towers of Hanoi
*    ==========================
*
*    Required Libraries:     Joystick : https://github.com/ELECXJEL2645/Joystick
*                            N5110    : https://github.com/ELECXJEL2645/N5110
*
*    Authored by:            Jonathan Calladine
*    Date:                   2023
*
*    MBED OS Version:        6.12.0
*    Board:	                 NUCLEO L476RG  */

#include "mbed.h"
#include "Joystick.h" 
#include "N5110.h"

// PIN ASSIGNMENTS
//                  y     x
Joystick joystick(PC_3, PC_2);  //attach and create joystick object

DigitalIn Joystick_Button(PC_12); // set joystick button as interupt

//Pin assignment format:  lcd(IO, Ser_TX, Ser_RX, MOSI, SCLK, PWM)  
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);

DigitalOut LED(PC_10); // led pin assignment

// --------------------------------------- Function prototypes ---------------------------------------

// ------------------------------------------- UI functions ------------------------------------------
void main_menu();
void sel_game_tutorial();
void sel_no_discs_screen();
void game_win_screen();
void game_analysis_screen();

// ------------------------------------- Game and tutorial functions ---------------------------------
void game();
void game_loop();
void tutorial();
void tutorial_loop(int num, int source, int dest, int helper);

// ------------------------------------------ Core functions -----------------------------------------
void init_buttons();
void create_disc_array();
int joystick_move();
void draw_pegs();
void draw_discs(int peg_sel);
void draw_arrow(int peg);
int first_non_zero_element(int peg);
void move_discs(int pegA, int pegB);
bool game_win_check();
void error();


// Arrow Sprite
const int arrowSprite [7][10] = {
    { 1,0,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,1 },
    { 0,1,0,0,0,0,0,0,1,0 },
    { 0,0,1,0,0,0,0,1,0,0 },
    { 0,0,0,1,0,0,1,0,0,0 },
    { 0,0,0,0,1,1,0,0,0,0 },
    { 0,0,0,0,1,1,0,0,0,0 },
};

// struct for all fsm in code
struct movement_fsm {
    int output;
    int next_state[3];
};

// fsm for movement of arrow
movement_fsm fsm[3] = {
    {1,{0,1,2}},
    {2,{1,2,0}},
    {3,{2,0,1}},
};

// fsm for sel no. of discs
movement_fsm fsm_noDisc[4] = {
    {3,{0,1,3}},
    {4,{1,2,0}},
    {5,{2,3,1}},
    {6,{3,0,2}},
};

// fsm for sel game for tutorial
movement_fsm fsm_gORt[2] = {
    // outputs = x co-ordinate of rectagle start point
    {4,{0,1,1}}, 
    {35,{1,0,0}},
};

// global peg arrays
int peg1[6];
int peg2[6];
int peg3[6];

bool joystick_trigger = false;

int g_no_discs = 6; // declair global variable for number of discs
int g_no_discs_state = 2; // declair global variable for state in number of dics fsm

// Decalir variables that need to be global for game loop
    // initialse variables for peg selection
int g_game_pegA = 0; // peg that disc is moved from (first selected)
int g_game_pegB = 0; // peg that disc is moved to (second selected)
int g_game_peg_counter = 0; // counter to chose whether peg value stored in pegA or pegB

int g_game_state = 0; // initialse state of fsm
int g_game_detector = 0; // detector detects if button is pressed and allows only one action to be taken
int g_move_counter = 0; // counter for number of moves
int g_error_counter = 0; // counter for number of errors


int main(){
    init_buttons(); // initialse joystick button

    joystick.init();
    lcd.init(LPH7366_1);        //initialise for LPH7366-1 LCD (Options are LPH7366_1 and LPH7366_6)
    lcd.setContrast(0.55);      //set contrast to 55%
    lcd.setBrightness(0.5);     //set brightness to 50% (utilises the PWM)

    main_menu(); // call main menu
}

// --------------------------------------- UI Functions ---------------------------------------
void main_menu() {
    // Display text
    lcd.clear();
    lcd.printString("Towers of",14,2);
    lcd.printString("Hanoi",26,3);
    lcd.refresh();

    ThisThread::sleep_for(1500ms);

    sel_game_tutorial(); // go to select game or tutorial screen
}

void sel_game_tutorial() {
     bool loop = true; // variable used to loop while function until option is selected
     int x_pos_gORt; // variable for the x position of the rectangle 
     int state = 0; // variable to store state of fsm

    while (loop == true) {

        // Change state of fsm based on movement of joystick
        int direction = joystick_move();
        state = fsm_gORt[state].next_state[direction];

        x_pos_gORt = fsm_gORt[state].output; // use state of fsm to get x_pos of rectangle to be drawn

        int width = 24 + (22 * state); // vary width  of rectagle - due to diff word lengths

        // Draw on lcd display
        lcd.clear();
        
        lcd.printString("Game/Tutorial",4,1);
        lcd.drawRect(x_pos_gORt,17,width,2,FILL_BLACK);
        lcd.printString("Press button",6,4);
        lcd.printString("to start",18,5);

        lcd.refresh();

        // if joystick button pressed, state of fsm corresponds to game or tutorial
        // 0 = game; 1 = tutorial
        // call respective functions
        // loop varaible set to false to stop the while loop

        if (Joystick_Button == 0) {
            loop = false;
            
            if (state == 0) {
                game();
            }
            else if (state == 1) {
                tutorial();
            }
        }
    } 
}

void sel_no_discs_screen() {
    bool loop = true; // variable used to loop while function until button pressed

    while (loop == true) {

        // Set number of discs based on state of fsm
        g_no_discs = fsm_noDisc[g_no_discs_state].output;

        // Draw on lcd display
        lcd.clear();
        
        lcd.printString("Sel no. discs",4,0);
        char buffer[14];
        sprintf(buffer, "<   %d   >", g_no_discs); // display number of discs on screen
        lcd.printString(buffer,15,2);
        lcd.printString("Press button",6,4);
        lcd.printString("to start",18,5);

        lcd.refresh();

        // Change state of fsm based on movement of joystick
        int direction = joystick_move();
        g_no_discs_state = fsm_noDisc[g_no_discs_state].next_state[direction];

        // if button pressed, set loop varaible to false to stop while loop
        // set number of discs as current state of fsm
        if (Joystick_Button == 0) {
            loop = false;
            g_no_discs = fsm_noDisc[g_no_discs_state].output; // declair final number of discs. 
        }
    }  
}

void game_win_screen() {
    // display congratulations message
    // display number of moved
    lcd.clear();

    lcd.printString("Winner!",25,1);
    lcd.printString("No. of Moves:",5,3);
    char buffer[14];
    sprintf(buffer, "%d", g_move_counter);
    lcd.printString(buffer,40,4);

    lcd.refresh();
}

void game_analysis_screen() {
    // analyse game to see how well the user did 
    // min number of moves = (2^no_of_discs) - 1
    // number of mistakes = number of moves - min number of moves - number of errors

    int perfect_moves = pow(2,g_no_discs) - 1;
    int mistakes = g_move_counter - perfect_moves - g_error_counter;

    // display infromation on screen
    lcd.clear();
        
    lcd.printString("Game Analysis",4,0);
    char buffer1[14];
    sprintf(buffer1, "Moves: %d", g_move_counter); // display number of moves on screen
    lcd.printString(buffer1,1,2);

    char buffer2[14];
    sprintf(buffer2, "IdealMoves: %d", perfect_moves); // display number of ideal moves on screen
    lcd.printString(buffer2,1,3);

    char buffer3[14];
    sprintf(buffer3, "Mistakes: %d", mistakes); // display number of mistakes on screen
    lcd.printString(buffer3,1,4);

    char buffer[14];
    sprintf(buffer, "Errors: %d", g_error_counter); // display number of errors on screen
    lcd.printString(buffer,1,5);

    lcd.refresh();
}

// --------------------------------------- Game and tutorial functions ---------------------------------------
void game() {
    // N.B. sleep commands used so one bytton press isn't registered multiple times

    // select nunber of discs   
    ThisThread::sleep_for(500ms);
    sel_no_discs_screen();

    // create arrays
    ThisThread::sleep_for(500ms);
    create_disc_array(); 

    g_move_counter = 0; // reset move counter
    g_error_counter = 0;  // reset error counter

    // game loop - repeat loop until game won
    while (game_win_check() == false) {
        game_loop();
    }
    
    // display win screen
    game_win_screen();
    ThisThread::sleep_for(3000ms);

    // display analysis screen
    game_analysis_screen();
    ThisThread::sleep_for(5000ms);

    main_menu(); // return to main menu
}

void game_loop() {

    lcd.clear();
    draw_pegs();
    draw_discs(g_game_pegA); // draw discs, varible sent to function selects top disc on that peg
    
    // set x coordiante of arrow based on state of fsm
    int arrow_above_peg = fsm[g_game_state].output;
    draw_arrow(arrow_above_peg); // function draws arrow above specific peg

    lcd.refresh();

    // change state of fsm based on movement of joystick
    int direction = joystick_move();
    g_game_state = fsm[g_game_state].next_state[direction];


    // Issue with switch bounce mitigated using game detector varaible
    // Stops the code being run mutiple times due to the multiples inputs caused by switch bounce
    // when first time code is run (on first input), game detector button is set high
    // code can not be run again, instead a delay is ran

    if (Joystick_Button == 0 && g_game_detector == 0) {
        // if button pressed, a peg is selected
        // the first peg selected is the peg to be moved from == pegA
        // the second peg selected is the peg to be moved to == pegB
        // when the first peg is selected, the top disc on that peg can be highlighted
        // when the second peg is selected, the top disc can be moved to the new peg
        // to select if its the first or second peg, the peg_counter variable is used
            // peg counter = 1 --> first peg == pegA
            // peg counter = 2 --> second peg == pegB
        // the curret state of the fsm corresponds to the peg the arrow is above and the peg to be selected
        // this is read and stored in variables pegA or pegB
        
        g_game_detector = 1; // set game_detector to 1 to show that button has been pressed

        if (g_game_peg_counter == 0) {
            g_game_peg_counter++; // increment peg counter
        }

        if (g_game_peg_counter == 1) {
            g_game_pegA = fsm[g_game_state].output; // the peg the user has selected (current state of fsm) is stored in pegA variable

            if (first_non_zero_element(g_game_pegA) == g_no_discs ) {
                // if peg empty, invalid press; therefore, reset
                // if peg empty, first non zero element will = number of discs
                g_game_pegA = 0; // reset pegA varaible
                g_game_peg_counter = 0; // reset peg counter
            }
            else {
                // if peg isn't empty, increment peg counter
                g_game_peg_counter++;
            }
        }
        else if (g_game_peg_counter == 2) {
            g_game_pegB = fsm[g_game_state].output; // the peg the user has selected (current state of fsm) is stored in pegB variable

            move_discs(g_game_pegA, g_game_pegB); // run the move function on the pegs selected

            if (g_game_pegA != g_game_pegB) {
                g_move_counter++;
                // incrment move counter if the two pegs are different
                // ignores when user selects same peg twice
            }

            // reset variables
            g_game_peg_counter = 0; // reset peg counter
            g_game_pegA = 0;
            g_game_pegB = 0;
        }
    }

    if (g_game_detector == 1) {
        // reset game detector after a delay if it has been trigger
        // function used to prevent issues caused by switch bounce
        ThisThread::sleep_for(200ms);
        g_game_detector = 0;
    }
}

void tutorial() {
    // N.B. sleep commands used so one bytton press isn't registered multiple times

    // go to seleect number of disc screens
    ThisThread::sleep_for(500ms);
    sel_no_discs_screen();

    // create disc arrays
    ThisThread::sleep_for(500ms);
    create_disc_array();

    // display game
    lcd.clear();
    draw_pegs();
    draw_discs(0);
    lcd.refresh();

    ThisThread::sleep_for(1000ms);

    // run the iterattive tutorial code
    tutorial_loop(g_no_discs,1,3,2);
    ThisThread::sleep_for(2000ms);

    main_menu(); // return to main menu
}

void tutorial_loop(int num, int source, int dest, int helper) {
    // Iterative tutorial repeated until game sloved

    if (num == 1) {
        ThisThread::sleep_for(500ms);
        move_discs(source,dest);

        // display game
        lcd.clear();
        draw_pegs();
        draw_discs(0);
        lcd.refresh();

        return;
    }

    tutorial_loop(num - 1, source, helper, dest);
    ThisThread::sleep_for(500ms);
    move_discs(source,dest);

    // display game
    lcd.clear();
    draw_pegs();
    draw_discs(0);
    lcd.refresh();

    tutorial_loop(num - 1, helper, dest, source);
}


// --------------------------------------- Core Functions ---------------------------------------
void init_buttons(){
	// Set the joystick push-button to use an internal pull-up resistor
    Joystick_Button.mode(PullUp);
}

void create_disc_array() {
    // fills the first array with numbers 1 to nnumber of discs (descending order)
    // other arrays are filled with zeros
  
    for (int i = 0; i < g_no_discs; i++) {
        peg1[i] = i + 1;
        peg2[i] = 0;
        peg3[i] = 0;
    }
}

int joystick_move() {
    // function returns the direction that joystick has been moved
    // values used to interface with the finite state machines
        // 0 = don't change value   
        // 1 = joystick moved to the right == count up
        // 2 = joystick moved to the left == count down
    // magnitude of the joystick must be above 0.5 for movement to be registers
        // stops accidental knocks or inaccuracies in joystick
    // joystick trigger variable used to only allow one movement per press of the joystick
    // implemented to prevent multiple inputs being registered due to switch bounce
        // variable becomes true if movement allowed
        // varible then turned back to false when magnitude goes below 0.5

    int direction = 0;

    if (joystick_trigger == false) {
        if (joystick.get_direction() == E && joystick.get_mag() > 0.5) {
            direction = 1; // move right (up to next peg)
            joystick_trigger = true;
        }
        else if (joystick.get_direction() == W && joystick.get_mag() > 0.5) {
            direction = 2; // move left (down to next peg)
            joystick_trigger = true;
        }
    }
    else if (joystick_trigger == true) {
        if (joystick.get_mag() < 0.5) {
            joystick_trigger = false;
        }
    }
    else {
        ThisThread::sleep_for(10ms);
    }
    
    return direction;
}

void draw_pegs() {
    // Initialise positions for origin points of base and shafts
    int peg_shaft_array[3] = {17,42,67};
    int peg_base_array[3] = {6,31,56};

    for (int i = 0; i < 3; i++) {
        // draw base
        lcd.drawRect(peg_base_array[i],46,24,2,FILL_BLACK);

        // draw shaft
        lcd.drawRect(peg_shaft_array[i],16,2,30,FILL_BLACK);
    }
}

void draw_arrow(int peg) {
    int peg_sel = peg - 1; // subtract one so peg number matches array index
    int peg_x_array[] = {13,38,63}; // array of x poistions for arrow
    
    // draw arrow sprite in position
    lcd.drawSprite(peg_x_array[peg_sel], 4, 7, 10, (int *)arrowSprite);
}

void draw_discs(int peg_sel) {
    int i = g_no_discs - 1; // i counts down from the last position in the array. It indexes the disc to be drawn
    int j = 0; // j counts up from 0. It is used to space the blocks properly
    bool check = false;

    int height = 4; // set height of all the discs
    int size_factor = 4; // set a scaling factor
 
    int x_pos[3] = {18,43,68}; // set starting x point 
    int y_pos = 42; // set starting y point

    // create a varible to find top disc instead of running on each iteration of the loop
    int peg1_top_disc = first_non_zero_element(1);
    int peg2_top_disc = first_non_zero_element(2);
    int peg3_top_disc = first_non_zero_element(3);

    // loop until all discs drawn or when all elements drawn
    while (check == false) {
        if (peg1[i] == 0 && peg2[i] == 0 && peg3[i] == 0 || i < 0) {
            check = true;
            // stops the loop when the value in the array is 0 or when all elements are drawn
        }
        else {
            // draw discs on top of each other, from bottom to the top
            // only draws disc if the element is not equal to zero

            // centre x_pos defined by array is shifted left by two times the size of the disc. This ensures the disc is centre aligned
            // y_pos initally set ontop of peg base. Decreased to place each disc ontop of the last
            // disc width dependent on array size and scaling factor
            // height is fixed

            // function draws white disc to cover peg lines, then draws transparent rectangle as disc

            // a disc can be highlighted by selecting a peg by sending a number from 1 to 3 to the function
            // The top disc on that peg is highlghted by being filled solid black
            // if the value of 0 is sent, all disc drawn empty

            if (peg1[i] != 0) {
                // white rectangle to remove peg lines
                lcd.drawRect(x_pos[0] - peg1[i] * 2, y_pos - j*height, peg1[i] * size_factor, height, FILL_WHITE);

                if (peg_sel == 1 && peg1_top_disc == i)  {
                    // draw selected disc
                    lcd.drawRect(x_pos[0] - peg1[i] * 2, y_pos - j*height, peg1[i] * size_factor, height, FILL_BLACK);
                }
                else {
                    // draw unselected disc
                    lcd.drawRect(x_pos[0] - peg1[i] * 2, y_pos - j*height, peg1[i] * size_factor, height, FILL_TRANSPARENT);
                }
            }

            if (peg2[i] != 0) {
                // white rectangle to remove peg lines
                lcd.drawRect(x_pos[1] - peg2[i] * 2, y_pos - j*height, peg2[i] * size_factor, height, FILL_WHITE);

                if (peg_sel == 2 && peg2_top_disc == i)  {
                    // draw selected disc
                    lcd.drawRect(x_pos[1] - peg2[i] * 2, y_pos - j*height, peg2[i] * size_factor, height, FILL_BLACK);
                }
                else {
                    // draw unselected disc
                    lcd.drawRect(x_pos[1] - peg2[i] * 2, y_pos - j*height, peg2[i] * size_factor, height, FILL_TRANSPARENT);
                }
            }

            if (peg3[i] != 0) {
                // white rectangle to remove peg lines
                lcd.drawRect(x_pos[2] - peg3[i] * 2, y_pos - j*height, peg3[i] * size_factor, height, FILL_WHITE);

                if (peg_sel == 3 && peg3_top_disc == i)  {
                    // draw selected disc
                    lcd.drawRect(x_pos[2] - peg3[i] * 2, y_pos - j*height, peg3[i] * size_factor, height, FILL_BLACK);
                }
                else {
                    // draw unselected disc
                    lcd.drawRect(x_pos[2] - peg3[i] * 2, y_pos - j*height, peg3[i] * size_factor, height, FILL_TRANSPARENT);
                }
            }
            
            i--; // decrement i
            j++; // increment j
        }
    }
}

void move_discs(int pegA, int pegB) {
    // pegA = peg moving from
    // pegB = peg moving to

    // find position of first non-zero value 
    // for pegA - this is needed as it is the disc to be moved 
    // for pegB - this is needed as it will be used to check if the move is allowed
        // i.e. - the disc on the new peg is larger thatn the one you want to move
    // the position the disc is moving to is one less than the position of the disc currecntly on the peg.
    // NB: this is why an empty peg returns the value of number of discs. e.g. doing 5 - 1 will index 4, which is the position of the first slot.  
    
    int moveFrom = first_non_zero_element(pegA); 
    int moveTo = first_non_zero_element(pegB);
    int newPosition = moveTo - 1;

    // use nested switches for each possible movement
    // i.e. 1 to 2, 1 to 3, 
    //      2 to 1, 2 to 3,
    //      3 to 1, 3 to 2.

    // The Operation is the same for each of the 6 cases:
    //  check if the moveTo value = number of discs - corresponds to an empty peg
    //      if this is the case, don't need to check if movement is valid
    // if peg not empty - check if movement is valid 
    //      if the value of the disc to be moved (pegA[moveFrom]) is larger than the top disc currently on the peg (pegB[moveTo]) - this is an invalid move
    //          call the error function
    // if the move is not invalid - then it is valid and the disc is moved
    //      set value of new position to value of disc to be moved
    //      set old position value to 0 to remove disc

    // NB: Breaks needed to ensure only the desired cases are run. 
  
    switch (pegA) {
        case 1:
            switch (pegB) {
                case 2:
                    // peg 1-2

                    if (moveTo == g_no_discs) {
                        // = 5 means peg empty
                        peg2[newPosition] = peg1[moveFrom];
                        peg1[moveFrom] = 0;
                    }
                    else if (peg1[moveFrom] > peg2[moveTo]) {
                        error();
                    }
                    else {
                        peg2[newPosition] = peg1[moveFrom];
                        peg1[moveFrom] = 0;
                    }
                    
                    break;

                case 3:
                    // peg 1-3
                        
                    if (moveTo == g_no_discs) {
                        // = 5 means peg empty
                        peg3[newPosition] = peg1[moveFrom];
                        peg1[moveFrom] = 0;
                    }
                    else if (peg1[moveFrom] > peg3[moveTo]) {
                        error();
                    }
                    else {
                        peg3[newPosition] = peg1[moveFrom];
                        peg1[moveFrom] = 0;
                    }
            break;
            }
        break; 
        
    case 2:
        switch (pegB) {
            case 1:
                // 2-1
                
                if (moveTo == g_no_discs) {
                    // = 5 means peg empty
                    peg1[newPosition] = peg2[moveFrom];
                    peg2[moveFrom] = 0;
                }
                else if (peg2[moveFrom] > peg1[moveTo]) {
                    error();
                }
                else {
                    peg1[newPosition] = peg2[moveFrom];
                    peg2[moveFrom] = 0;
                }
            break; 

            case 3:
                // 2-3
                
                if (moveTo == g_no_discs) {
                    // = 5 means peg empty
                    peg3[newPosition] = peg2[moveFrom];
                    peg2[moveFrom] = 0;
                }
                else if (peg2[moveFrom] > peg3[moveTo]) {
                    error();
                }
                else {
                    peg3[newPosition] = peg2[moveFrom];
                    peg2[moveFrom] = 0;
                }
        break; 
        }
    break; 
        
    case 3:
        switch (pegB) {
            case 1:
                //3-1
                
                if (moveTo == g_no_discs) {
                    // = 5 means peg empty
                    peg1[newPosition] = peg3[moveFrom];
                    peg3[moveFrom] = 0;
                }
                else if (peg3[moveFrom] > peg1[moveTo]) {
                    error();
                }
                else {
                    peg1[newPosition] = peg3[moveFrom];
                    peg3[moveFrom] = 0;
                }
            break; 

            case 2:
                //3-2
                
                if (moveTo == g_no_discs) {
                    // = 5 means peg empty
                    peg2[newPosition] = peg3[moveFrom];
                    peg3[moveFrom] = 0;
                }
                else if (peg3[moveFrom] > peg2[moveTo]) {
                    error();
                }
                else {
                    peg2[newPosition] = peg3[moveFrom];
                    peg3[moveFrom] = 0;
                }
        break; 
        }
    break; 
    }  
}

int first_non_zero_element(int peg) {
    // function to find first non-zero element
    // this represents the top disc on a peg
    
    int pos_first_non_zero = g_no_discs; // initialise variable as number of discs 
    // if array is empty, value will stay as number of discs
    // position for disc to be moved to is first non-zero element minus one
    // therefore initialising this varible as the number of discs will automatially place a disc for an empty peg 

    bool test = false; 
    int i = 0; // incrementer variable initialised

    // perform same operation on each peg - use switch to chose peg
    // operation - search array, when value not equal to zero, store its POSITION in pos_first_non_zero (not the value)
    
    // if statement used - if value is non-zero: set pos_first_non_zero, turn test value to true to exit while loop
    // else  - increment i
    // if enterily peg (array) searched (i = number of discs), stop search
    
    switch (peg) {
    case 1:
        while (test == false) {
            if (peg1[i] != 0) {
                pos_first_non_zero = i; 
                test = true;
            }
            else {
                i++;
            }

            if (i == g_no_discs) {
                test = true;
            }
        }
    case 2:
        while (test == false) {
            if (peg2[i] != 0) {
                pos_first_non_zero = i; 
                test = true;
            }
            else {
                i++;
            }

            if (i == g_no_discs) {
                test = true;
            }
        }
    case 3:
        while (test == false) {
            if (peg3[i] != 0) {
                pos_first_non_zero = i; 
                test = true;
            }
            else {
                i++;
            }

            if (i == g_no_discs) {
                test = true;
            }
        }
    }

  return pos_first_non_zero; // return the position of first non-zero element on peg = top disc
}

bool game_win_check() {
    // game won when tower reconstructed on new peg
    // tower reconstructed means sum of two arrays = 0 and the other array will contain all the elements
    // e.g. 5 discs, therefore, full tower sum = triangular number of 5: 1+2+3+4+5 = 15

    // initialise variables
    bool gameStatus = false;
    int sum2 = 0;
    int sum3 = 0;

    int winning_sum[7] = {0,1,3,6,10,15,21}; // winning sum for each number of discs
    int winning_no = winning_sum[g_no_discs]; // select specific winning number

    // create sum of each array
    for (int i = 0; i < g_no_discs; i++) {
        sum2 = sum2 + peg2[i];
        sum3 = sum3 + peg3[i];
    }

    // test if either are equal to winning number
    if (sum2 == winning_no || sum3 == winning_no) {
        gameStatus = true;
    }
    
    return gameStatus;

    // NB: sum1 not included as tower must me constructed on new peg
}

void error() {
    g_error_counter++; // increment error counter

    // trigger LED  
    // flash LED 5 times - lasts for a second
    for (int i = 0; i < 5; i++) {
        LED = 1;
        ThisThread::sleep_for(100ms);
        LED = 0;
        ThisThread::sleep_for(100ms);
    }
}
