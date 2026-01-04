#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/** Defined states for the state machine.
  *
  * Add your state numbers to the enum here.
  *
  */ 

typedef enum
{
  ST_Entry,             // This state is called upon SM_Run being called

                        // Start of user defined states
  ST_Hello,
  ST_Eating,
  ST_Goodbbye,
                        // End of user defined states
  ST_End,               // This state is called to exit the state machine.
  ST_Error              // If you end up here, your state machine is reporting an error.

} State;

/* Create a special kind of type that defines a function pointer for the SM */
typedef State (*SM_FuncPtr)(void);

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

/* create our current state nd point it to ST_Entry */
State curr_state = ST_Entry;

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
  else return ST_Goodbbye;
}

State Goodbye( void )
{
  printf( "Goodbye cruel world, I ate too many eggs!\n" );
  return ST_End;
}


State SM_Setup( void )
{
  // Your code goes here, point to your initial state below.
  return ST_Hello;
}

State SM_End( void )
{
  // Your code goes here for post SM cleanup.
  return ST_Error;
}

State SM_Error( void )
{
  // Your code goes here, if there's a problem, you end up here.
  return ST_Error;  
}


State SM_Run( SM_FuncPtr statePtr[], uint16_t last_state )
{
  while( curr_state != last_state )
  {
    if( curr_state <= last_state )
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


int main( int argc, char* argv[] )
{
  SM_FuncPtr sm_table[] ={ SM_Setup, Hello, Eating, Goodbye, SM_Error };

  uint16_t state_count = ( sizeof( sm_table ) / sizeof( SM_FuncPtr ) );

  SM_Run( sm_table, state_count );

  printf("Terminating\n");
  return EXIT_SUCCESS;
}
