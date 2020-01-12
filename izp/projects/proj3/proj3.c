/** IZP Project 3
 * Ondrej Sedlacek [xsedla1o], December 2019
 *
 * Path through maze
 *  Input:
 *   Maze and entry coordinates, method of pathfinding
 *
 *  Output:
 *   Series of coordinates describing the path through the maze.
 */


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>


#define ERROR 11

// Expected number of arguments
#define HELP 2
#define TEST 3
#define SOLVE 5

// Flags
#define RHAND 40
#define LHAND 41
#define SHORTEST 42

// Representation of triangles
#define TLEFT 1
#define TRIGHT 2
#define THORIZONTAL 4

// Refine representation of squares
#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

// Table of increments
const int ti[4][2] = {
        {-1, 0}, // UP
        {0, 1},  // RIGHT
        {1, 0},  // DOWN
        {0, -1}  // LEFT
};


/**
 * Map: struct
 * rows: int, number of rows
 * cols: int, number of columns
 * cells: unsigned char pointer to dynamically allocated memory
 *  containing all cells of the maze
 */
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;


// Map type functions
Map *map_ctor(int r, int c);
void map_dtor(Map *map);
Map *map_load(char* name);
bool map_validate(Map *m);
void map_print(Map *m);
void map_convert(Map *m);

// Helper functions
bool map_valid_index(Map *m, int r, int c);
bool isborder(Map *map, int r, int c, int border);
bool points_up(int r, int c);
int log_2(int n);

// Pathfinding functions
bool valid_entry(Map *m, int r, int c);
int start_border(Map *m, int *r, int *c, int mode);
void move(int *r, int *c, int dir);
int dir_next(Map *m, int r, int c, int dir, int mode);
int dir_cycle(int dir, int mode);

// Program structure functions
int process_args(int argc, char **argv, int *r, int *c, char **name);
void print_help();
void test_map(Map *m);
void find_path(Map *m, int r, int c, int mode);


/**
 * @brief constructor for map.
 */
Map *map_ctor(int r, int c){
    Map *map = malloc(sizeof(Map));
    if(map == NULL){
        fprintf(stderr, "Error: memory allocation failed.\n");
        return NULL;
    }

    map->rows = r;
    map->cols = c;

    map->cells = (unsigned char*)malloc(sizeof(unsigned char) * r * c);
    if(map->cells == NULL){
        free(map);
        fprintf(stderr, "Error: memory allocation failed.\n");
        return NULL;
    }

    for(int i = 0; i < r*c; i++)
        map->cells[i] = '0';

    return map;
}


/**
 * @brief destructor for map.
 */
void map_dtor(Map *map){
    if(map == NULL)
        return;
    free(map->cells);
    free(map);
}


/**
 * @brief loads map.
 */
Map *map_load(char* name){
    // open file
    FILE *f=fopen(name, "r");
    if(f == NULL){
        fprintf(stderr, "Error: opening file failed.\n");
        return NULL;
    }

    // read rows, cols, init map
    int rows, cols;
    char c, t;
    if(fscanf(f, "%d", &rows) != 1 || rows < 0 || // rows read successfully
       !isspace(c = fgetc(f)) ||                  // followed by one whitespace
       fscanf(f, "%d", &cols) != 1 || cols < 0 || // columns read successfully
       (c = fgetc(f)) != '\n'){                   // followed by newline
        fprintf(stderr, "Error: file format invalid.\n");
        fclose(f);
        return NULL;
    }
    Map *map = map_ctor(rows, cols);

    // load Map values
    for(int r = 0; r < rows; r++){
        for(int col = 0; col < cols; col++){
            if(!(// read valid char
               fscanf(f,"%c", &c) == 1 && c <= '7' && c >= '0'&& // due to ASCII
               // read whitespace ( if last in row, read newline or EOF)
               (col + 1 == cols)? (t = fgetc(f)) == '\n' || t == EOF:
                                   isspace(fgetc(f)))){
                fprintf(stderr, "Error: file format invalid.\n");
                map_dtor(map);
                fclose(f);
                return NULL;
            }
            map->cells[r*cols + col] = c;
        }
    }

    // check if entire file was read
    if(fscanf(f, " %c", &c) == 1){
        fprintf(stderr, "Error: file format invalid.\n");
        map_dtor(map);
        fclose(f);
        return NULL;
    }

    // close file, return
    fclose(f);
    return map;
}


/**
 * @brief checks validity of a map.
 */
bool map_validate(Map *m){
    for(int r = 0; r < m->rows; r++){
        for(int c = 0; c < m->rows; c++){
            for(int border = 1; border < 5; border *= 2){
                if(isborder(m, r, c, border)){
                    switch(border){
                        case TLEFT:
                            if(map_valid_index(m, r, c-1))
                                if(!isborder(m,r,c-1, TRIGHT))
                                    return false;
                            break;
                        case TRIGHT:
                            if(map_valid_index(m, r, c+1))
                                if(!isborder(m,r,c+1, TLEFT))
                                    return false;
                            break;
                        case THORIZONTAL:
                            if(!points_up(r,c) && //  triangle pointing down
                               map_valid_index(m, r-1, c)){
                                if(!isborder(m,r-1,c, THORIZONTAL))
                                    return false;
                            }
                            if (points_up(r,c) && //  triangle pointing up
                                map_valid_index(m, r+1, c)){
                                if(!isborder(m,r+1,c, THORIZONTAL))
                                    return false;
                            }
                    }
                }
            }
        }
    }
    return true;
}


/**
 * @brief prints map.
 */
void map_print(Map *m){
    printf("%d %d\n", m->rows, m->cols);
    for(int r = 0; r < m->rows; r++){
        for(int c = 0; c < m->cols; c++){
            printf("%c ", m->cells[r*m->cols + c]);
        }
        printf("\n");
    }
    printf("\n");
}


/**
 * @brief converts triangle cell map to a square cell representation
 */
void map_convert(Map *m){
    char new;
    for(int i = 0; i < m->rows*m->cols; i++){
        new = '0';
        if(m->cells[i] & TLEFT)
            new |= LEFT;
        if(m->cells[i] & TRIGHT)
            new |= RIGHT;
        if(!points_up(i/m->cols, i%m->cols)){ // triangle points down
            new |= DOWN;
            if(m->cells[i] & THORIZONTAL)
                new |= UP;
        }
        else{ // triangle points up;
            new |= UP;
            if(m->cells[i] & THORIZONTAL)
                new |= DOWN;
        }
        m->cells[i] = new;
    }
}


/**
 * @brief checks if coords are in bounds of a map
 */
bool map_valid_index(Map *m, int r, int c){
    return r >= 0 && r < m->rows && c >= 0 && c < m->cols;
}


/**
 * @brief checks if there is a border on a given coord in given direction
 */
bool isborder(Map *map, int r, int c, int dir){
    //printf("%d,%d dir=%d: ", r+1, c+1, dir);
    //printf((map->cells[r*map->cols + c] & dir)? "border\n":"free\n");
    return map->cells[r*map->cols + c] & dir;
}


/**
 * @brief checks if a triangle points up
 *       /\
 *      /__\
 * like that.
 */
bool points_up(int r, int c){
    return (r+c)%2 == 1;
}


/**
 * @brief converts direction to index in table of increments
 */
int log_2(int n){
    int i = 0;
    while((n /= 2) != 0)
        i++;
    return i;
}


/**
 * @brief checks if given coords are a valid entry
 *  This is done by checking if you can legally move out of bounds from them.
 */
bool valid_entry(Map *m, int r, int c){
    for(int i = 1; i < 9; i *= 2){
        if(!isborder(m,r,c,i)){
            if(!map_valid_index(m, r + ti[log_2(i)][0], c + ti[log_2(i)][1]))
                return true;
        }
    }
   return false;
}


/**
 * @brief gives the algorith a starting border to follow, based on entry point
 */
int start_border(Map *m, int *r, int *c, int mode){
    // check if is valid entry
    // bring coords down to real values
    if (!(map_valid_index(m, --*r, --*c)) || !valid_entry(m, *r, *c)){
        fprintf(stderr, "Error, Invalid starting position.\n");
        return ERROR;
    }
    int flags = 0, count = 0;
    if(*r == 0){
        count++;
        flags |= DOWN;
    }
    if(*c == 0){
        count++;
        flags |= RIGHT;
    }
    if(*r+1 == m->rows){
        count++;
        flags |= UP;
    }
    if(*c+1 == m->cols){
        count++;
        flags |= LEFT;
    }

    if(flags > 1){ // entry is a corner
        for(int i = 1; i < 9; i *= 2){
            // find dir out, then adjust border according to mode
            if(!isborder(m,*r,*c,i)){
                if(!map_valid_index(m, *r + ti[log_2(i)][0], *c + ti[log_2(i)][1]))
                    return dir_cycle(i, mode);
            }
        }
    }

    return dir_cycle(flags, (mode == RHAND)? LHAND:RHAND);
}


/**
 * @brief prints current coords and moves
 */
void move(int *r, int *c, int dir){
    //printf("%d,%d dir=%d\n", *r+1, *c+1, dir);
    printf("%d,%d\n", *r+1, *c+1);
    int i = log_2(dir); // converts direction to index in table of increments
    (*r) += ti[i][0];
    (*c) += ti[i][1];
}


/**
 * @brief finds next valid direction
 */
int dir_next(Map *m, int r, int c, int dir, int mode){
    dir = dir_cycle(dir, (mode == RHAND)? LHAND : RHAND); // invert dir_cycle
    if(!isborder(m,r,c,dir))
        return dir;
    while(isborder(m, r, c, dir = dir_cycle(dir, mode))){}
    return dir;
}


/**
 * @brief cycles givnen direction based on mode
 */
int dir_cycle(int dir, int mode){
    if(mode == RHAND){ // cycle counter-clockwise
        if (dir == UP)
            return LEFT;
        return dir/2;
    }
    if(mode == LHAND){ // cycle clockwise
        if (dir == LEFT)
            return UP;
        return 2*dir;
    }
    return ERROR;
}


/**
 * @brief checks and processes arguments
 */
int process_args(int argc, char **argv, int *r, int *c, char **name){
    // basic argc check
    if(argc == 1){
        fprintf(stderr, "Error: no args found, try --help for more info.\n");
        return ERROR;
    }

    // load first argument
    int expected_argc, flag;
    if(strcmp(argv[1], "--help") == 0)
        expected_argc = HELP;
    else if(strcmp(argv[1], "--test") == 0)
        expected_argc = TEST;
    else if(strcmp(argv[1], "--rpath") == 0){
        expected_argc = SOLVE;
        flag = RHAND;
    }
    else if(strcmp(argv[1], "--lpath") == 0){
        expected_argc = SOLVE;
        flag = LHAND;
    }
    else if(strcmp(argv[1], "--shortest") == 0){
        expected_argc = SOLVE;
        flag = SHORTEST;
    }
    else{
        fprintf(stderr, "ERROR, wrong args, try --help for more info.\n");
        return ERROR;
    }

    // check number of args
    if(expected_argc != argc){
        fprintf(stderr, "wrong number of args, try --help for more info.\n");
        return ERROR;
    }

    // load rest of arguments
    switch(expected_argc){
        case HELP:
            print_help();
            return expected_argc;
        case TEST:
            *name = argv[2];
            return expected_argc;
        case SOLVE:
            if(sscanf(argv[2], "%d", r) != 1 || 
               sscanf(argv[3], "%d", c) != 1) {
                fprintf(stderr, "wrong format of args, try --help for more info.\n");
                return ERROR;
            }
            *name = argv[4];
            return flag;
    }

    (void)name; // avoiding unused variable warning

    fprintf(stderr, "Error: unexpected ERROR has occurred.\n");
    return ERROR;
}


/**
 * @brief prints help to stdin.
 */
void print_help(){
    printf("Possible combinations of arguments:\n");
    printf("\t--help\n\t Prints this text.\n\n");
    printf("\t--test file.txt\n\t \
Checks if given file contains a valid maze map.\n\n");
    printf("\t--rpath R C file.txt\n\t \
Finds a path through the maze entering on row R and column C, \
using the right hand rule.\n\n");
    printf("\t--lpath R C file.txt\n\t \
Same as --rpath, but uses left hand rule.\n\n");
    printf("\t--shortest R C file.txt [under construction]\n\t \
Finds the shortest path from the maze, which happens to be \
the one you came on.\n\t (given your entrypoint is valid)\n");
}


/**
 * @brief checks if map is valid
 */
void test_map(Map *m){
    if(map_validate(m))
        printf("Valid\n");
    else
        printf("Invalid\n");
}


/**
 * @brief finds path for RIGHT/LEFT hand rule
 * @param mode RHAND or LHAND
 */
void find_path(Map *m, int r, int c, int mode){
    int dir; //direction
    if((dir = start_border(m, &r, &c, mode)) == ERROR){
        fprintf(stderr, "Error, was unable to establish starting conditions.\n");
        return;
    }
    // First move
    while(isborder(m, r, c, dir))
            dir = dir_cycle(dir, mode);
    move(&r, &c, dir);

    // Loop until you leave maze
    while(map_valid_index(m, r, c)){
        dir = dir_next(m, r, c, dir, mode);// Get direction
        move(&r, &c, dir);// Move
    }
}


/**
 * @brief Main program.
 */
int main(int argc, char **argv){
    int r, c;
    char* name = NULL;
    int flag = process_args(argc, argv, &r, &c, &name);
    Map *m;

    switch(flag){
        case HELP:
            return 0;
        case ERROR:
            return 1;
        case TEST:
            m = map_load(name);
            if(m == NULL)
            {
                printf("Invalid\n");
                return 1;
            }
            test_map(m);
            map_dtor(m);
            return 0;
    }

    // LOAD DATA
    m = map_load(name);
    if(m == NULL)
        return 1;
    if(!map_validate(m)){
        fprintf(stderr, "Error, given map is invalid.\n");
        return 1;
    }
    map_convert(m);

    switch(flag){
        case RHAND:
        case LHAND:
            find_path(m, r, c, flag);
            break;
        case SHORTEST:
            printf("%d,%d\n", r, c);
            break;
    }

    map_dtor(m);
    return 0;
}
