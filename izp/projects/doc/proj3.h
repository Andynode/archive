/**
 * @file    proj3.h
 * @author  xsedla1o
 * @date    2019
 * @brief Header file for work with maze.
 */


#ifndef PROJ3_H
#define PROJ3_H

/**
 * @brief Structure to represent the maze.
 */
typedef struct {
    int rows; /**< Number of rows in the maze. */
    int cols; /**< Number of columns in the maze. */
    unsigned char *cells;/**< Poiner to memory where maze is stored. */
} Map;

/**
 * @brief An enum with bit values of borders.
 */
enum borders { 
    BLEFT=0x1,  /**< Left border.*/
    BRIGHT=0x2, /**< Right border.*/
    BTOP=0x4,   /**< Top border.*/
    BBOTTOM=0x4 /**< Bottom border.*/
};

/**
 * @brief       Frees map.
 * @param[in]   map Pointer to map with maze.
 * @pre         Cells of map must point to allocated memory.
 * @post        Memory will be deallocated, cells set to NULL.
 */
void free_map(Map *map);

/**
 * @brief       Loads map.
 * @param[in]   filename    Name of file to load from.
 * @param[out]  map         Pointer to map to load to.
 * @return      0 if ok.
 * @pre         Filename must be valid.
 * @post        Memory will be allocated for cells and map will be loaded.
 */
int load_map(const char *filename, Map *map);

/**
 * @brief       Checks if there is a wall on the given position.
 * @param[in]   map     Pointer to map with maze.
 * @param[in]   r       Row coordinate.
 * @param[in]   c       Column coordinate.
 * @param[in]   border  Bit value of border to check.
 * @pre         Loaded map, valid coordinates, border from borders enum.
 * @post        No values are changed.
 * @return      True if wall exist at given position, else false.
 */
bool isborder(Map *map, int r, int c, int border);

/**
 * @brief       Check if given coords have bottom border.
 * @param[in]   r   Row coordinate.
 * @param[in]   c   Column coordinate.
 * @pre         No preconditions.
 * @post        No postconditions.
 * @return      True when given coords have bottom border, else false.
 */
bool hasbottom(int r, int c);

/**
 * @brief       Gives starting border.
 * @param[in]   map         Pointer to map with maze.
 * @param[in]   r           Row coordinate.
 * @param[in]   c           Column coordinate.
 * @param[in]   leftright   Indicates which hand rule to use.
 * @pre         Loaded map, valid coordinates, valid leftright value.
 * @post        No values are changed.
 * @return      Starting border.
 */
int start_border(Map *map, int r, int c, int leftright);

/**
 * @brief       Checks if given map is valid.
 * @param[in]   map         Pointer to map with maze.
 * @pre         Loaded map.
 * @post        No values are changed.
 * @return      0 if valid, else coresponding errno.
 */
int check_map(Map *map);

/**
 * @brief       Loads maze from file and checks if is valid.
 * @param[out]  filename    Name of file to load from.
 * @param[in]   map         Pointer of map to load to.
 * @pre         Valid map with cells set to NULL.
 * @post        Cells will point to maze.
 * @return      0 if valid, else coresponding errno.
 */
int load_and_check_map(const char *filename, Map *map);

/**
 * @brief       Checks if given coords are outside maze
 * @param[in]   map         Pointer to map with maze.
 * @param[in]   r           Row coordinate.
 * @param[in]   c           Column coordinate.
 * @pre         Map containing rows and columns.
 * @post        No values are changed.
 * @return      True if coordinates are outside maze.
 */
bool is_out(Map *map, int r, int c);

/**
 * @brief       Prints path through the maze.
 * @param[in]   map         Pointer to map with maze.
 * @param[in]   r           Row coordinate.
 * @param[in]   c           Column coordinate.
 * @param[in]   leftright   Indicates which hand rule to use.
 * @pre         Loaded map, valid starting coordinates, valid leftright value.
 * @post        No values are changed.
 */
void print_path(Map *map, int r, int c, int leftright);

#endif
