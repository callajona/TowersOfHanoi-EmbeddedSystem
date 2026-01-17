#include <iostream>

// create peg arrays = global variables 
int peg1[5] = {1, 2, 3, 4, 5};
int peg2[5] = {0, 0, 0, 0, 0};
int peg3[5] = {0, 0, 0, 0, 0};

// Funtion proto-types
void display_game();
int first_non_zero_element(int peg);
void move(int pegA, int pegB);
bool gameWin();

void tower_of_hanoi(int num, int source, int dest, int helper);

int main() {
  display_game(); // display arrays
  
  int num = 5;
  
  std::cout<<("The sequence of moves :\n");
  tower_of_hanoi(num,1,2,3);
}


void tower_of_hanoi(int num, int source, int dest, int helper) {
  if (num == 1) {
    std::cout << " Move disk 1 from tower " << source << " to tower " << dest << std::endl;
    move(source,dest);
    display_game();
    return;
  }
  tower_of_hanoi(num - 1, source, helper, dest);
  std::cout << " Move disk " << num << " from tower " << source << " to tower " << dest << std::endl;
  move(source,dest);
  display_game();
  tower_of_hanoi(num - 1, helper, dest, source);
}

int first_non_zero_element(int peg) {
  // function to find first non-zero element
  // this represents the top disc on a peg
  
  int x = 5; // initialise x as 5 - if array is empty, value will stay as 5
  // the reason for this is explained in the move function
  
  bool test = false; 
  int i = 0; // incrementer variable initialised

  // perform same operation on each peg - use switch to chose peg
  // operation - search array, when value not equal to zero, store its POSITION in x (not the value)
  
  // if statement used - if value is non-zero: set x, turn test value to true to exit while loop
  // else  - increment i
  
  switch (peg) {
  case 1:
    while (test == false) {
      if (peg1[i] != 0) {
        x = i; 
        test = true;
      }
      else {
        i++;
      }

      if (i == 5) {
        test = true;
      }
    }
  case 2:
    while (test == false) {
      if (peg2[i] != 0) {
        x = i; 
        test = true;
      }
      else {
        i++;
      }

      if (i == 5) {
        test = true;
      }
    }
  case 3:
    while (test == false) {
      if (peg3[i] != 0) {
        x = i; 
        test = true;
      }
      else {
        i++;
      }

      if (i == 5) {
        test = true;
      }
    }
  }

  return x; // return the position of first non-zero element on peg = top disc
}

void move(int pegA, int pegB) {
  // pegA = peg moving from
  // pegB = peg moving to

  // find position of first non-zero value 
  // for pegA - this is needed as it is the disc to be moved 
  // for pegB - this is needed as it will be used to check if the move is allowed
    // i.e. - the disc on the new peg is larger thatn the one you want to move
  // the position the disc is moving to is one less than the position of the disc currecntly on the peg.
  // NB: this is why an empty peg returns the value of 5. Doing 5 - 1 will index 4, which is the position of the first slot.  
  
  int moveFrom = first_non_zero_element(pegA); 
  int moveTo = first_non_zero_element(pegB);
  int newPosition = moveTo - 1;

  // use nested switches for each possible movement
  // i.e. 1 to 2, 1 to 3, 
  //      2 to 1, 2 to 3,
  //      3 to 1, 3 to 2.

  // The Operation is the same for each of the 6 cases:
  //  check if the moveTo value = 5 - corresponds to an empty peg
  //   if this is the case, don't need to check if movement is valid
  // if peg not empty - check if movement is valid 
  //  if the value of the disc to be moved (pegA[moveFrom]) is larger than the top disc currently on the peg (pegB[moveTo]) - this is an invalid move
  // if the move is not invalid - then it is valid and the disc is moved
  //    set value of new position to value of disc to be moved
  //    set old position value to 0 to remove disc

  // NB: Breaks needed to ensure only the desired cases are run. 
  
  switch (pegA) {
    case 1:
      switch (pegB) {
        case 2:
          std::cout << "1-2\n";
          
          if (moveTo == 5) {
            // = 5 means peg empty
            peg2[newPosition] = peg1[moveFrom];
            peg1[moveFrom] = 0;
          }
          else if (peg1[moveFrom] > peg2[moveTo]) {
            std::cout << "Error \n";
          }
          else {
            peg2[newPosition] = peg1[moveFrom];
            peg1[moveFrom] = 0;
          }
          
          break;

        case 3:
          std::cout << "1-3\n";
            
          if (moveTo == 5) {
            // = 5 means peg empty
            peg3[newPosition] = peg1[moveFrom];
            peg1[moveFrom] = 0;
          }
          else if (peg1[moveFrom] > peg3[moveTo]) {
            std::cout << "Error \n";
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
          std::cout << "2-1\n";
          
          if (moveTo == 5) {
            // = 5 means peg empty
            peg1[newPosition] = peg2[moveFrom];
            peg2[moveFrom] = 0;
          }
          else if (peg2[moveFrom] > peg1[moveTo]) {
            std::cout << "Error \n";
          }
          else {
            peg1[newPosition] = peg2[moveFrom];
            peg2[moveFrom] = 0;
          }
          break; 

        case 3:
          std::cout << "2-3\n";
          
          if (moveTo == 5) {
            // = 5 means peg empty
            peg3[newPosition] = peg2[moveFrom];
            peg2[moveFrom] = 0;
          }
          else if (peg2[moveFrom] > peg3[moveTo]) {
            std::cout << "Error \n";
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
        std::cout << "3-1\n";
        
        if (moveTo == 5) {
          // = 5 means peg empty
          peg1[newPosition] = peg3[moveFrom];
          peg3[moveFrom] = 0;
        }
        else if (peg3[moveFrom] > peg1[moveTo]) {
          std::cout << "Error \n";
        }
        else {
          peg1[newPosition] = peg3[moveFrom];
          peg3[moveFrom] = 0;
        }
        break; 

      case 2:
        
        std::cout << "3-2\n";
          
        if (moveTo == 5) {
          // = 5 means peg empty
          peg2[newPosition] = peg3[moveFrom];
          peg3[moveFrom] = 0;
        }
        else if (peg3[moveFrom] > peg2[moveTo]) {
          std::cout << "Error \n";
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

void display_game() {
  // Display arrays
  //  - increment through each array and display it
  
  for (int i = 0; i < 5; i++) {
    std::cout << peg1[i] << ", ";
  }
  std::cout << "\n";
  for (int i = 0; i < 5; i++) {
    std::cout << peg2[i] << ", ";
  }
  std::cout << "\n";
  for (int i = 0; i < 5; i++) {
    std::cout << peg3[i] << ", ";
  }
  std::cout << "\n";
}

bool gameWin() {
  // game won when tower reconstructed on new peg
  // tower reconstructed means sum of two arrays = 0 and the other array will contain all the elements
  // 5 discs, therefore: triangular number of 5: 1+2+3+4+5 = 15

  // initialise variables
  bool gameStatus = false;
  int sum2 = 0;
  int sum3 = 0;

  // create sum of each array
  for (int i = 0; i < 5; i++) {
    sum2 = sum2 + peg2[i];
    sum3 = sum3 + peg3[i];
  }

  // test if either are 15
  if (sum2 == 15 || sum3 == 15) {
    gameStatus = true;
  }
  
  return gameStatus;

  // NB: sum1 not included as tower must me constructed on new peg
}
