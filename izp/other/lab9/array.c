/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#include "array.h"

/** Datove typy pro objekt a pole
typedef struct {
  int id;
  char *name;
} Object;

typedef struct {
  unsigned int size;
  Object *items;
} Array;
*/

/**
 * Konstruktor pole. Vytvoří pole o velikosti size a každý prvek
 * v něm inicializuje na hodnoty 0, NULL.
 */
Array array_ctor(unsigned size)
{
    Array a = {.size = size };
    a.items = malloc(size*sizeof(Object));
    if(a.items == NULL){
        a.items = NULL;
        return a;
    }
    for(int i = 0; i < size; i++){
        Object o = {};
        object_ctor(&o, 0, NULL);
        a.items[i] = o;
    }
    return a;
    /** Napoveda a poznamky
        1]  opet alokujeme pouze slozku, ktera je ukazatel
        2]  v tomto pripade je nutne nejdrive alokovat cele pole a nasledne
            jeho obsah (objekty)
        3]  alokace pole opet pomoci malloc, je nutne zvolit spravny parametr
            u sizeof, velikost pole je v promene size
        4]  objekty tvorime pomoci konstruktoru object_ctor pro kazdy prvek pole
            void object_ctor(Object *o, int id, char *name) 
        5]  nezapomente nastavit velikost a vytvorene pole vratit    
    */
}

/**
 * Uvolní alokované místo pro pole (a každý jeho objekt) z paměti.
 */
void array_dtor(Array *arr)
{
    for(int i = 0; i < arr->size; i++){
        object_dtor(&(arr->items[i]));
    }
    free(arr->items);
    arr->items = NULL;
    /** Napoveda a poznamky
        1]  dealokace opet pomoci funkce free
        2]  je nutne dealokovat v opacnem poradi, nez v jakem jsme alokovali
            (nejdrive jednotlive prvky, pote cele pole)
    */
}

/**
 * Změna velikosti pole. Změní/realokuje zadané pole na novou velikost.
 * V případě zvětšení ještě inicializuje každý prvek na hodnoty 0, NULL
 */
Array array_resize(Array *arr, unsigned size)
{
    arr->items = realloc(arr->items, size*sizeof(Object));
    if (arr->size < size){
        for(int i = arr->size; i < size; i++){
            Object o = {};
            object_ctor(&o, 0, NULL);
            arr->items[i] = o;
        }
    }
    arr->size = size;
    return *arr;
    
    /** Napoveda a poznamky
        1]  realokace pole pomoci funkce realloc
        2]  nove prvky alokujeme pomoci konstruktoru pro objekty    
    */
}

/**
 * Hledání prvku v poli podle identifikátoru prvku. Vrací index prvku v poli
 * nebo -1, pokud prvek pole neobsahuje.
 */
int array_find_id(Array *arr, int id)
{
    for(int i = 0; i < arr->size; i++){
        if(arr->items[i].id == id)
            return i;
    }
    return -1;
}

/**
 * Hledání prvku v poli podle názvu. Vrací index prvku v poli
 * nebo -1, pokud prvek pole neobsahuje.
 */
int array_find_name(Array *arr, char *name)
{
    for(int i = 0; i < arr->size; i++){
        if(arr->items[i].name != NULL && strcmp(arr->items[i].name, name)==0)
            return i;
    }
    return -1;
}

/**
 * Vložení prvku do pole na zadaný index. Vrací index vloženého prvku (idx)
 * nebo -1, pokud se operace nezdarila.
 */
int array_insert_item(Array *arr, Object *item, unsigned idx)
{
    *arr = array_resize(arr, arr->size + 1);
    if(idx > arr->size || arr->items == NULL)
        return -1;
    for(int i = arr->size - 1; i > idx; i--){
        object_swap(&(arr->items[i]), &(arr->items[i-1]));
    }
    arr->items[idx].id = item->id;
    arr->items[idx].name = item->name;
    return idx;
    /** Napoveda a poznamky
        1]  jednoduchy postup, pokud je jiz na idx prvek:
            a]  zvetsit pole o 1, do nove vytvoreneho prvku vlozit novy objekt
            b]  prohodit posledni (nove pridany  prvek) a prvek na indexu
                idx 
        2]  postup, pro ktery vam projdou i testy (opet pokud je na idx prvek):
            a]  zvetsit pole o 1
            b]  posunout vsechny prvky tak, aby byl na indexu idx nove vlozeny
                prazdny prvek
            c]  vlozit novy prvek na index idx  
    */
}

/**
 * Hledání prvku s nejmenším identifikátorem. Vrací index prvku nebo -1,
 * pokud je pole prázdné.
 * Promenna l urcuje index, od ktereho se bude pole prohledavat.
 */
int array_find_min(Array *arr, unsigned l){
    if(arr->items == NULL)
        return -1;
    int min = l;
    for(unsigned i = l; i < arr->size; i++){
        if(arr->items[i].id < arr->items[min].id)
            min = i;
    }
    return min;
}
/**
 * Razeni prvku v poli podle jejich identifikatoru.
 */
void array_sort(Array *arr)
{
    int min;
    for(int i = 0; i < arr->size-1; i++){
        min = array_find_min(arr, i+1);
        object_swap(&(arr->items[i]), &(arr->items[min]));
    }
    /** Napoveda a poznamky
        1]  pouzijte selection sort, lze napsat pouze s pomoci jiz definovanych
            funkci
    */
    
}

/**
 * Vypise prvky pole.
 */
void print_array(Array a)
{
    for (unsigned int i = 0; i < a.size; i++)
    {
        printf("#%d\t", i);
        print_object(&a.items[i]);
    }
    printf("\n");
}

/**
 * Vypise prvky pole dane velikosti.
 */
void print_array_size(Array a, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("#%d\t", i);
        print_object(&a.items[i]);
    }
    printf("\n");
}


