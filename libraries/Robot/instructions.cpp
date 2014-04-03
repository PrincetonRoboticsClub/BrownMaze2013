#include <Robot.h>

#define DONE 0
#define FORWARD 1
#define TURN 2

#ifndef TILE_LENGTH
#define TILE_LENGTH 22.0f
#endif

#ifndef INSTRUCTION_DELAY
#define INSTRUCTION_DELAY 400
#endif

// container for instruction data
typedef struct {
   int type; // turn or go straight, or done
   int value; // how much to go straight or turn
} Instruction;

// complete a set of instructions
/*void Robot::doInstructions(Instruction* instructions) {
   int i = 0;
   Instruction current;
   while ((current = instructions[i++]) != DONE) {
      if (current.type == FORWARD) {
         if (targetAngle = 0)
            changeSetX(TILE_LENGTH * current.value);
         else if (targetAngle = PI/2)
            changeSetY(TILE_LENGTH * current.value);
         else if (targetAngle = PI)
            changeSetX(-TILE_LENGTH * current.value);
         else if (targetAngle = 3*PI/2)
            changeSetY(-TILE_LENGTH * current.value);
      }
      else if (current.type == TURN) {
         changeSetAngle(PI/2 * current.value);
      }
      waitForNext(INSTRUCTION_DELAY);
   }
}*/

// maybe make a function to generate instructions from
// a set of waypoints/tiles
//void generateInstructions() {
//
//}