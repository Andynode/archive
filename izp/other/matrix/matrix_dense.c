#include<stdio.h>
#include<stdlib.h>

/// DATOVE TYPY ///
typedef struct matrix {
    unsigned rows;
    unsigned cols;
    float *data;
} matrix_t;


/// POMOCNE FUNKCE - IMPLEMENTUJTE JAKO PRVNI
/**
	@param m matice
	@param r index radku matice
	@param c index sloupce matice
	@param value hodnota, ktera se vklada na [r,c] matice
	@brief Funkce vklada hodnotu na urcenou polohu v matici
*/ 
void matrix_set_item(matrix_t *m, unsigned r, unsigned c, float value){
    if (m->data == NULL || r >= m->rows || c >= m->cols)
        return;
    m->data[m->cols*r + c] = value;
}

/**
	@param matrix matice
	@brief Funkce inicializuje matici hodnotami od 1 vzestupne.
*/
void matrix_init(matrix_t *m){
    for(int i = 0; i < (int)(m->rows*m->cols); i++){
        m->data[i] = i+1;
    }
}

/**
	@param rows pocet radku matice
	@param cols pocet sloupcu matice
	@return inicializovana matice naplnena daty funkci matrix_init
	
	Funkce inicializuje matici s poctem radku a sloupcu pomoci malloc. Pokud se alokace
	podarila, inicializuje v ni i data. 

	Na konci matici vraci.
*/
matrix_t *matrix_ctor(unsigned rows, unsigned cols){
    //matrix_t m = { .rows = rows, .cols = cols, .data = NULL};
    matrix_t *m = malloc(sizeof(matrix_t));
    if(m != NULL){
        m->rows = rows;
        m->cols = cols;
    }
    m->data = malloc(sizeof(float) * rows * cols);
    if (m->data != NULL){
        matrix_init(m);
    }
    return m;
}

/**
	@param matrix matice
	@brief Funkce uvolnuje pamet alokovanou pro matici.
*/ 
void matrix_dtor(matrix_t *m){
    free(m->data);
    free(m);
}

/**
	@param m matice
	@param r radek
	@param c sloupec
	@return hodnotu ze souradnice [r,c] matice
	@brief Funkce vraci prvek na souradnici [r,c] v matici.
*/
float matrix_get_item(matrix_t *m, unsigned r, unsigned c){
    if (m->data == NULL || r >= m->rows || c >= m->cols)
        return 0.0;
    return m->data[r*m->cols + c];
}

/**
	@param m matice
	@brief Funkce vypisuje matici po radcich. Jednotlive prvky budou oddeleny tabulatorem (\t), radky znakem novy radek (\n).
*/
void matrix_print(matrix_t *m){
    if (m->data == NULL)
        return;
    for(unsigned r = 0; r < m->rows; r++){
        for (unsigned c = 0; c < m->cols; c++){
            printf("%f\t", m->data[r*m->cols + c]);
        }
        printf("\n");
    }
    printf("\n");
}


/// FUNKCE PRO PRACI S MATICEMI  
/**
	@param dst matice obsahujici vysledek operace nasobeni konstantou
	@param k konstanta, kterou se matice nasobi
	@brief Funkce vynasobi matici konstantou. dst = dst*k
*/
void matrix_mult_const(matrix_t *dst, int k){
    if (dst->data == NULL)
        return;
    for(unsigned i = 0; i < dst->rows * dst->cols; i++){
            dst->data[i] = dst->data[i] * k;
    }
}
/**
	@param dst matice, ktera obsahuje soucet
	@param src zdrojova matice
	@brief dst = src + dst
*/
void matrix_add (matrix_t *dst, matrix_t *src){
    if(dst->cols != src->cols || dst->rows != src->rows)
        return;
    for(unsigned i = 0; i < dst->cols * dst->rows; i++)
        dst->data[i] = src->data[i] + dst->data[i];
}

/**
	@param c matice, ktera obsahuje vysledek nasobeni matic
	@param a matice
	@param b matice
	@brief c = a*b
*/
void matrix_mult (matrix_t *c, matrix_t *a, matrix_t *b){
    if (a->cols != b->rows)
        return;
    int n = a->cols;
    for(unsigned r = 0; r < c->rows; r++){
        for(unsigned col = 0; col < c->cols; col++){
            float sum = 0;
            for(int i = 0; i < n; i++){
                sum += matrix_get_item(a, r, i) * matrix_get_item(b, i, col);
            }
            matrix_set_item(c, r, col, sum);
        }
    }
}

/**
	@param m matice
	@brief Funkce vypisuje prvky pod hlavni diagonalou.
*/
void matrix_under_diagonal(matrix_t* m);
// mrdat.



int main() {
    matrix_t *m = matrix_ctor(4, 2);
    matrix_t *n = matrix_ctor(2, 3);
    matrix_t *d = matrix_ctor(4, 3);

    matrix_print(m);
    matrix_print(n);

    matrix_mult(d, m, n);
    matrix_print(d);

    matrix_dtor(m);
    matrix_dtor(n);

    matrix_mult_const(m, 0);

    return 0;
}
