/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#include "struct.h"

/** Definice datoveho typu pro objekt
typedef struct {
  int id;
  char *name;
} Object;
*/

/**
 * Inizializace objektu. Funkce objekt nealokuje. Název/jméno objektu kopíruje. 
 * Objekt bude mít název roven NULL, pokud se inicializace nezdařila.
 */
void object_ctor(Object *o, int id, char *name)
{
    o->id = id;
    if (name == NULL){
        o->name = NULL;
        return;
    }
    o->name = malloc(strlen(name) + 1);
    if(o->name != NULL){
        strcpy(o->name, name);
        return;
    }
    o->name=NULL;
    /** Napoveda a poznamky
        1]  alokujeme slozku objektu, ne cely objekt. Alokovat lze pouze ukazatele.
        2]  k alokaci pameti pouzijeme funkci malloc. Jako parametr ji predame
            pocet bytu, ktere chceme alokovat. Po alokaci je nutne zkontrolovat,
            zda se alokace podarila.
        3]  funkce strlen vraci delku retezce (bez ukoncujici \0 !!!). 
            Retezec ktery funkci predavame, nesmi byt NULL.
        4]  operator sizeof vraci velikost typu, ktery se mu zada v parametru
        5]  pristup ke slozkam struktury pomoci operatoru . nebo -> 
        6]  pro zkopirovani retezce do struktury pouzijte funkci strcpy(dst,src)
    */
}

/**
 * Záměna dat dvou objektù.
 */
void object_swap(Object *i1, Object *i2)
{
    Object t = {};
    t.id = i2->id;
    t.name = i2->name;

    i2->id = i1->id;
    i2->name = i1->name;

    i1->id = t.id;
    i1->name = t.name;
    /** Napoveda a poznamky
        1]  nejjednodusi je vymenit cely objekt (presne dle schematu pro funkci
            swap)
        2]  Schema: 
            a]  vytvorime pomocny objekt, do nej zkopirujeme hodnotu jednoho
                z objektu, ktere chceme prohodit
            b]  provedeme vymenu hodnot objektu
            c]  do druheho objektu vlozime hodnotu pomocneho objektu    
    */
}

/**
 * Hluboká kopie objektu src na dst. Vrací dst, pokud se operace povedla,
 * jinak NULL.
 */
Object *object_cpy(Object *dst, Object *src)
{
    object_ctor(dst, src->id, src->name);
    if(dst->name != NULL){
        return dst;
    }
    return NULL;
    /** Napoveda a poznamky
        1]  hlubokou kopii je myslena nova inicializace objektu dst pomoci dat
            ze src    
    */
}

/**
 * Uvolní objekt (resp. jeho jméno) z paměti. 
 * Nastavi jmeno objektu na NULL.
 */
void object_dtor(Object *o)
{
    free(o->name);
    o->name = NULL;
    /** Napoveda a poznamky 
        1]  k uvolneni pameti se pouziva funkce free, ktera je inverzni k funkci
            malloc (pamet, ktera byla alokovana pomoci malloc se musi uvolnit
            pomoci free)
        2]  po uvolneni pameti nezapomente nastavit jmeno objektu na NULL 
    */
}

/**
 * Vytiske hodnoty daneho objektu.
 */
void print_object(Object *o)
{
    printf("ID: %d, NAME: %s\n", o->id, o->name);
}
