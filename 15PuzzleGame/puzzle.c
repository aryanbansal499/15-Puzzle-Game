
//code Written by Aryan Bansal Student Id 909532.
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>


/**
 * READ THIS DESCRIPTION
 *
 * node data structure: containing state, g, f
 * you can extend it with more information if needed
 */
typedef struct node{
	int state[16];
	int g;
	int f;
} node;

/**
 * Global Variables
 */

// used to track the position of the blank in a state,
// so it doesn't have to be searched every time we check if an operator is applicable
// When we apply an operator, blank_pos is updated
int blank_pos;

//keeps track of the previous move in the game.
int last_move;

// Initial node of the problem
node initial_node;

// Statistics about the number of generated and expendad nodes
unsigned long generated;
unsigned long expanded;


/**
 * The id of the four available actions for moving the blank (empty slot). e.x.
 * Left: moves the blank to the left, etc.
 */

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
//number of tiles in the puzzle
#define PUZZLE_SIZE 16

/*
 * Helper arrays for the applicable function
 * applicability of operators: 0 = left, 1 = right, 2 = up, 3 = down
 */
int ap_opLeft[]  = { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 };
int ap_opRight[]  = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
int ap_opUp[]  = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int ap_opDown[]  = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
int *ap_ops[] = { ap_opLeft, ap_opRight, ap_opUp, ap_opDown };


/* print state */
void print_state( int* s )
{
	int i;

	for( i = 0; i < 16; i++ )
		printf( "%2d%c", s[i], ((i+1) % 4 == 0 ? '\n' : ' ') );
}

void printf_comma (long unsigned int n)
{
    if (n < 0) {
        printf ("-");
        printf_comma (-n);
        return;
    }
    if (n < 1000) {
        printf ("%lu", n);
        return;
    }
    printf_comma (n/1000);
    printf (",%03lu", n%1000);
}
//helper function for manhattan(calculates the sum)
int calculate_manhattan_value(int x,int y,int x1,int y1,int sum){

	return sum + (abs(x - x1)) + (abs(y - y1));
}
/* return the sum of manhattan distances from state to goal */
int manhattan( int* state )
{
	int sum = 0;

  int x , y ,x1, y1;
  int k = 0;


  while(k < PUZZLE_SIZE){
		int state_pos = state[k];
    if (state_pos){
      x = k%4 ;y = k/4;
      x1 = state_pos %4 ;y1 = state_pos/4 ;
      sum = calculate_manhattan_value(x,y,x1,y1,sum);
    }
    k++;
  }
  return( sum );
}


/* return 1 if op is applicable in state, otherwise return 0 */
int applicable( int op )
{
       	return( ap_ops[op][blank_pos] );
}

int reset_blank_pos(int blank,int set_blank){

	return (blank = set_blank);
}

/* apply operator */
void apply( node* n, int op )
{
	int t;

	//find tile that has to be moved given the op and blank_pos
	t = blank_pos + (op == 0 ? -1 : (op == 1 ? 1 : (op == 2 ? -4 : 4)));

	//apply op
	n->state[blank_pos] = n->state[t];
	n->state[t] = 0;

	//update blank pos
	blank_pos = t;
}

//function copies the contents of one node into another node.
//code is adapted and modified from www.geeksforgeeks.com
void copy_data(void *dest, void *src,int n)
{
   // Typecast src and dest addresses to (char *)
   int *csrc = (int *)src;
   int *cdest = (int *)dest;
   int i = 0;

   // Copy contents of src[] to dest[]
   for (i=0; i<n; i++)
       cdest[i] = csrc[i];
}

//helper function to calculate the value of evaluation function
int evaluation_func(node* n,int manhattan_value){
	n -> f = n->g + manhattan_value;
	return n->f;
}

/* Recursive IDA */
node* ida( node* node, int threshold, int* newThreshold )
{


	struct node* n;//this is the new node defined as n' in the psuedocode.
	int i = 0;
	int copy_last_move;//creates a copy of the last_move.
	int first_blank;//it is the initial blank position of state.
	int opp;//it is the opposite action
	int eval_func;//stores the value of evaluation function.
	while(i <= 3)
	{
		//calculates the opposite actions.
		if (i == LEFT)
		opp = RIGHT;
		else if (i == RIGHT)
		opp = LEFT;
		else if (i == UP)
		opp = DOWN;
		else
		opp = UP;

    	//line 1 of the pseudocode (for)
		if ((opp != last_move)&&(applicable(i)))
		{

			++generated;
			//allotting memory to the new_node, performing the line 2 of the psuedocode
			n = malloc(sizeof(struct node));
			//copying the state of node into the new node n.
			copy_data(n->state,node->state,PUZZLE_SIZE);
			first_blank = reset_blank_pos(first_blank,blank_pos);

			apply (n, i);

			//line 3 (calculates the cost);
			n-> g = node-> g + 1;

      		//line 4
			int value_of_manhattan =  manhattan(n->state);
      		//calculating the value of f
			eval_func = evaluation_func(n,value_of_manhattan);
      		// checks if the value of f is greater than threshold performs line 5(if)
			if (eval_func > threshold)
			{
				//checks if value of f is less than newThreshold and resets it accordingly
				//line 6(then) is performed.
				if(eval_func < *newThreshold)
					*newThreshold = n -> f;
			 	free(n);

			} else {
				++expanded;
        		//performing the line 8
			 	if (value_of_manhattan == 0)
			 		return n;//then return the new node n line(9).
			  copy_last_move = last_move;
			 	last_move = i;
				//line 10 is being performed (recursively calling the ida function).
			 	struct node* r = ida(n, threshold, newThreshold);
        		//line 11 and line 12(the resulting node is returned )
			 	if (r != NULL)
			 	{
			 		return r;
			 	}
				last_move = copy_last_move;
			 	free(n);

			 }
			 blank_pos = reset_blank_pos(blank_pos,first_blank);//updating the blank_pos
		}
		i++;
	}
  	//returns Null (line 13)
	return NULL;
}


/* main IDA control loop */
int IDA_control_loop(  )
{
	node* n;
	node* r = NULL;


	int threshold;
	int newThreshold;


	/* initialize statistics */
	generated = 0;
	expanded = 0;
	last_move = -2;


	/* compute initial threshold B */
	initial_node.f = threshold = manhattan( initial_node.state );

	printf( "Initial Estimate = %d\nThreshold = ", threshold );

	do
	{
		n = malloc(sizeof(struct node));
		//copy the stae of initial_node into the new node
		copy_data(n->state, initial_node.state,PUZZLE_SIZE);
		//line 5
		n->g = 0;
		//line 3
		newThreshold = INT_MAX;
		r = ida(n, threshold, &newThreshold);
		free(n);
		//line 7
		if (r == NULL)
		{
			//line 8
			threshold = newThreshold;
			printf(" %d", threshold);
		}
		//line 9
	}while(r==NULL);
		//line 10
	return r->g;
}


static inline float compute_current_time()
{
	struct rusage r_usage;

	getrusage( RUSAGE_SELF, &r_usage );
	float diff_time = (float) r_usage.ru_utime.tv_sec;
	diff_time += (float) r_usage.ru_stime.tv_sec;
	diff_time += (float) r_usage.ru_utime.tv_usec / (float)1000000;
	diff_time += (float) r_usage.ru_stime.tv_usec / (float)1000000;
	return diff_time;
}

int main( int argc, char **argv )
{
	int i, solution_length;

	/* check we have a initial state as parameter */
	if( argc != 2 )
	{
		fprintf( stderr, "usage: %s \"<initial-state-file>\"\n", argv[0] );
		return( -1 );
	}


	/* read initial state */
	FILE* initFile = fopen( argv[1], "r" );
	char buffer[256];

	if( fgets(buffer, sizeof(buffer), initFile) != NULL ){
		char* tile = strtok( buffer, " " );
		for( i = 0; tile != NULL; ++i )
			{
				initial_node.state[i] = atoi( tile );
				blank_pos = (initial_node.state[i] == 0 ? i : blank_pos);
				tile = strtok( NULL, " " );
			}
	}
	else{
		fprintf( stderr, "Filename empty\"\n" );
		return( -2 );

	}

	if( i != 16 )
	{
		fprintf( stderr, "invalid initial state\n" );
		return( -1 );
	}

	/* initialize the initial node */
	initial_node.g=0;
	initial_node.f=0;

	print_state( initial_node.state );


	/* solve */
	float t0 = compute_current_time();

	solution_length = IDA_control_loop();

	float tf = compute_current_time();

	/* report results */
	printf( "\nSolution = %d\n", solution_length);
	printf( "Generated = ");
	printf_comma(generated);
	printf("\nExpanded = ");
	printf_comma(expanded);
	printf( "\nTime (seconds) = %.2f\nExpanded/Second = ", tf-t0 );
	printf_comma((unsigned long int) expanded/(tf+0.00000001-t0));
	printf("\n\n");

	/* aggregate all executions in a file named report.dat, for marking purposes */
	FILE* report = fopen( "report.dat", "a" );

	fprintf( report, "%s", argv[1] );
	fprintf( report, "\n\tSoulution = %d, Generated = %lu, Expanded = %lu", solution_length, generated, expanded);
	fprintf( report, ", Time = %f, Expanded/Second = %f\n\n", tf-t0, (float)expanded/(tf-t0));
	fclose(report);

	return( 0 );
}
