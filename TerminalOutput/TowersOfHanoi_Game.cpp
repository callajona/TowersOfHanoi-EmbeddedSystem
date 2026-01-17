#include <iostream>

// create peg arrays = global variables 
  int peg1[7];
  int peg2[7];
  int peg3[7];

// Funtion proto-types
void display_game(int no_discs);
int first_non_zero_element(int peg,int no_discs);
void move(int pegA, int pegB, int no_discs);
bool gameWin(int no_discs);

int main() {
  // create arrays with variable number of discs
  int no_discs;
  std::cout << "Enter number of discs: ";
  std::cin >> no_discs;  

  peg1[no_discs];
  peg2[no_discs];
  peg3[no_discs];
  
  for (int i = 0; i < no_discs; i++) {
    peg1[i] = i + 1;
    peg2[i] = 0;
    peg3[i] = 0;
  }
  
  // initialise variables
  bool gameOver = false; 
  int a;
  int b;
  int counter = 0; 

  // loop game until victory
  while (gameOver == false) {
    display_game(no_discs); // display arrays
    
    // input pegs 
      // a = moving from
      // b = moving to
    std::cout << "Enter pegA: ";
    std::cin >> a;
    std::cout << "Enter pegB: ";
    std::cin >> b;
    
    move(a,b,no_discs); // move from a to b - error checking w/in
    counter++; // increment counter

    // check if game has been complted
    if (gameWin(no_discs) == true) {
      gameOver = true;
      display_game(no_discs);
      
      // winning text
      std::cout << "Winner Winner -  vegan chicken dinner!\n";
      std::cout << "You completed the game in: " << counter << " moves.\n";
    }
  }
}

int first_non_zero_element(int peg, int no_discs) {
  // function to find first non-zero element
  // this represents the top disc on a peg
  
  int x = no_discs; // initialise x as number of discs - if array is empty, value will stay as number of discs
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

      if (i == no_discs) {
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

      if (i == no_discs) {
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

      if (i == no_discs) {
        test = true;
      }
    }
  }

  return x; // return the position of first non-zero element on peg = top disc
}

void move(int pegA, int pegB, int no_discs) {
  // pegA = peg moving from
  // pegB = peg moving to

  // find position of first non-zero value 
  // for pegA - this is needed as it is the disc to be moved 
  // for pegB - this is needed as it will be used to check if the move is allowed
    // i.e. - the disc on the new peg is larger thatn the one you want to move
  // the position the disc is moving to is one less than the position of the disc currecntly on the peg.
  // NB: this is why an empty peg returns the value of 5. Doing 5 - 1 will index 4, which is the position of the first slot.  
  
  int moveFrom = first_non_zero_element(pegA,no_discs); 
  int moveTo = first_non_zero_element(pegB,no_discs);
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
          
          if (moveTo == no_discs) {
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
            
          if (moveTo == no_discs) {
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
          
          if (moveTo == no_discs) {
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
          
          if (moveTo == no_discs) {
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
        
        if (moveTo == no_discs) {
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
          
        if (moveTo == no_discs) {
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

void display_game(int no_discs) {
  // Display arrays
  //  - increment through each array and display it
  
  for (int i = 0; i < no_discs; i++) {
    std::cout << peg1[i] << ", ";
  }
  std::cout << "\n";
  for (int i = 0; i < no_discs; i++) {
    std::cout << peg2[i] << ", ";
  }
  std::cout << "\n";
  for (int i = 0; i < no_discs; i++) {
    std::cout << peg3[i] << ", ";
  }
  std::cout << "\n";
}

bool gameWin(int no_discs) {
  // game won when tower reconstructed on new peg
  // tower reconstructed means sum of two arrays = 0 and the other array will contain all the elements
  // 5 discs, therefore: triangular number of 5: 1+2+3+4+5 = 15

  // initialise variables
  bool gameStatus = false;
  int sum2 = 0;
  int sum3 = 0;

  int winning_sum[8] = {0,1,3,6,10,15,21,28};

  // create sum of each array
  for (int i = 0; i < no_discs; i++) {
    sum2 = sum2 + peg2[i];
    sum3 = sum3 + peg3[i];
  }

  // test if either are 15
  if (sum2 == winning_sum[no_discs] || sum3 == winning_sum[no_discs]) {
    gameStatus = true;
  }
  
  return gameStatus;

  // NB: sum1 not included as tower must me constructed on new peg
}
