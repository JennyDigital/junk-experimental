#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/** Defined states for the state machine.
  *
  * Add your state numbers to the enum here.
  * Make sure they are in the correct order.
  * The first state after ST_Entry is your initial state.
  * The last state before ST_End is your final state.
  * Make sure to update the state function table in main() accordingly.
  * Each state must have a corresponding function defined below.
  * Each function must return the next state to go to.
  * For example:
  * State MyState( void )
  * {
  *  // Your code here
  * return NextState;
  * }
  */ 

typedef enum
{
  ST_Entry,             // This state is called upon SM_Run being called

                        // Start of user defined states
  ST_Hello,
  ST_Eating,
  ST_Goodbye,
                        // End of user defined states

  ST_End,               // This state is called to exit the state machine.
  ST_Error              // If you end up here, your state machine is reporting an error.

} State;

/* Create a special kind of type that defines a function pointer for the SM */
typedef State (*SM_FuncPtr)(void);

State SM_Run( SM_FuncPtr statePtr[], uint16_t last_state );

/** Your function declarations for the SM go here
  *
  * They must all return type State
  * and take no parameters
  */
State SM_Setup  ( void );
State Hello     ( void );
State Goodbye   ( void );
State Eating    ( void );
State SM_Error  ( void );

/* States that are alwsays present 
 */

 /* Starting state
  * Setup goes in here 
  */
State SM_Setup( void )
{
  // Your code goes here, point to your initial state below.
  return ST_Hello;
}

/** Endstate before exit
 * This function is called before exiting the state machine.
 * Use it for any cleanup you need to do.
*/
State SM_End( void )
{
  // Your code goes here for post SM cleanup.
  return ST_End;
}


/* Error state
 * With this state, you can handle errors how you need to
 */
State SM_Error( void )
{
  // Your code goes here, if there's a problem, you end up here.
  return ST_Error;  
}

/* Create our current state nd point it to ST_Entry */
State curr_state = ST_Entry;

/** State functions
  * Add your state functions here
  * Each function must return type State and take no parameters
  * Each function must return the next state to go to.
  * For example:
  * State MyState( void )
  * {
  *   // Your code here
  *   return NextState;
  * }
 */

State Hello( void )
{
  printf("Hello world!\n");
  return ST_Eating;
}


State Eating( void )
{
  static int eggs = 6;

  printf("I have %d eggs left. NOM!\n", eggs-- );
  if( eggs > 0 ) return ST_Eating;
  else return ST_Goodbye;
}

State Goodbye( void )
{
  printf( "Goodbye cruel world, I ate too many eggs!\n" );
  return ST_End;
}
/* End of state functions */


/** State machine runner
 * This function runs the state machine.
 * It takes a pointer to an array of function pointers
 * and the last valid state number.
 * It returns when it reaches the last state.
 * If an invalid state is reached, it calls SM_Error and returns ST_Error.
 * 
 * Parameters:
 *  statePtr[] - array of function pointers for each state
 * last_state - the last valid state number
 * Returns:
 *  ST_End - if the state machine completes successfully
 *  ST_Error - if an error occurs
 */
State SM_Run( SM_FuncPtr statePtr[], uint16_t state_number )
{
  /* Ensure the current state starts at the entry point */
  uint16_t curr_state = ST_Entry;
  uint16_t last_state = state_number - 1;

  while( curr_state != last_state )
  {
    if( curr_state < last_state )
    {
      curr_state = statePtr[ curr_state ]();
    }
    else
    {
      SM_Error();
      return ST_Error;
    }
  }
  return ST_End;
}

/** Application entry point */
int main( int argc, char* argv[] )
{
  /* Set up state function pointer table */
  SM_FuncPtr sm_table[] ={ SM_Setup, Hello, Eating, Goodbye, SM_Error };

  /* Calculate number of states in the table */
  uint16_t state_count = ( sizeof( sm_table ) / sizeof( SM_FuncPtr ) );

  /* Run the state machine */
  SM_Run( sm_table, state_count );

  printf("Terminating\n");
  return EXIT_SUCCESS;
}
