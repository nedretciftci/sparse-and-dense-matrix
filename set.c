#include "set.h"

/**
 * This function initializes a set without an element and returns it.
 */
Set initSet() {
	Set set = (Set)malloc(sizeof(Set_t));
    if (set == NULL) return NULL;
    set->elements = NULL;
    set->cardinality = 0;
    return set;
}

/**
 * This function creates an integer-typed element and returns it. 
 * It allocates memory for both the element and the integer.
 */
Element createIntegerElement(int data) {
	Element element = (Element)malloc(sizeof(Element_t));
    if (element == NULL) return NULL;
    element->type = INTEGER;
    element->data = malloc(sizeof(int));
    if (element->data == NULL) return NULL;
    *(int*)(element->data) = data;
    return element;
}

/**
 * This function creates an float-typed element and returns it. 
 * It allocates memory for both the element and the float.
 */
Element createFloatElement(float data) {
	Element element = (Element)malloc(sizeof(Element_t));
    if (element == NULL) return NULL;
    element->type = FLOAT;
    element->data = malloc(sizeof(float));
    if (element->data == NULL) return NULL;
    *(float*)(element->data) = data;
    return element;
}

/**
 * This function creates an string-typed element and returns it. 
 * It allocates memory for both the element and the string.
 * This function uses the `strlen` and the `strcpy` functions.
 */
Element createStringElement(char* data) {
	Element element = (Element)malloc(sizeof(Element_t));
    if (element == NULL) return NULL;
    element->type = STRING;
    element->data = malloc(strlen(data) + 1); 
    if (element->data == NULL) return NULL;
    strcpy((char*)element->data, data);
    return element;
}

/**
 * This function creates an matrix-point-typed element and returns it. 
 * It allocates memory for both the element and the matrix point.
 */
Element createMatrixPointElement(int x, int y, int data) {
	Element element = (Element)malloc(sizeof(Element_t));
    if (element == NULL) return NULL;
    element->type = MATRIX_POINT;
    element->data = malloc(3 * sizeof(int));  
    if (element->data == NULL) return NULL;

    ((int*)element->data)[0] = x;    
    ((int*)element->data)[1] = y;     
    ((int*)element->data)[2] = data; 

    return element;
}

/**
 * This function compares two elements to see if they are identical. 
 * First, it checks if the types are identical. If they are the same, 
 * it checks if the data are the same. However, this function does not 
 * compare addresses because the value from two different addresses 
 * can be the same. It returns one if the elements are the same; 
 * otherwise, it returns zero. This function uses the `strcmp` function.
 */
int isSame(Element e1, Element e2) {
	if (e1->type != e2->type) return 0;

    switch (e1->type) {
        case INTEGER:
            return *(int*)e1->data == *(int*)e2->data;
        case FLOAT:
            return *(float*)e1->data == *(float*)e2->data;
        case STRING:
            return strcmp((char*)e1->data, (char*)e2->data) == 0;
        case MATRIX_POINT: {
            int* mp1 = (int*)e1->data;
            int* mp2 = (int*)e2->data;
            return (mp1[0] == mp2[0]) && (mp1[1] == mp2[1]) && (mp1[2] == mp2[2]);
        }
        default:
            return 0;
    }
}

/**
 * This function checks if the given element is in the set or not. 
 * It does not compare the addresses. It returns one if the given 
 * element is in the set; otherwise, it returns zero.
 */
int in(Set set, Element element) {
	for (int i = 0; i < set->cardinality; i++) {
        if (isSame(set->elements[i], element)) return 1;
    }
    return 0;
}

/**
 * This function inserts an element into the given set. 
 * The same element cannot be twice in the same set.
 * It returns one if the inserting element is successful; 
 * otherwise, it returns zero. This function uses the 
 * `realloc` function.
 */
int insertElement(Set set, Element element) {
	if (in(set, element)) return 0;

    set->elements = (Element*)realloc(set->elements, (set->cardinality + 1) * sizeof(Element));
    if (set->elements == NULL) return 0;
    set->elements[set->cardinality++] = element;
    return 1;
}

/**
 * This function removes the given element in the set. 
 * It returns one if removal is successful; otherwise, 
 * it returns zero. This function uses the `realloc` function.
 */
int removeElement(Set set, Element element) {
	for (int i = 0; i < set->cardinality; i++) {
        if (isSame(set->elements[i], element)) {
            freeElement(set->elements[i]);
            for (int j = i; j < set->cardinality - 1; j++) {
                set->elements[j] = set->elements[j + 1];
            }
            set->cardinality--;
            set->elements = (Element*)realloc(set->elements, set->cardinality * sizeof(Element));
            if (set->elements == NULL) return 0;
            return 1;
        }
    }
    return 0;
}

/**
 * This function creates and returns a new set, which is united of the given sets.
 */
Set unite(Set s1, Set s2) {
	Set result = initSet();
    for (int i = 0; i < s1->cardinality; i++) {
        insertElement(result, s1->elements[i]);
    }
    for (int i = 0; i < s2->cardinality; i++) {
        insertElement(result, s2->elements[i]);
    }
    return result;
}

/**
 * This function creates and returns a new set, which is intersected of the given sets.
 */
Set intersect(Set s1, Set s2) {
	Set result = initSet();
    for (int i = 0; i < s1->cardinality; i++) {
        if (in(s2, s1->elements[i])) {
            insertElement(result, s1->elements[i]);
        }
    }
    return result;
}

/**
 * This function creates and returns a new set, which is 
 * subtracted from the first given set by the second one.
 */
Set substract(Set s1, Set s2) {
	Set result = initSet();
    for (int i = 0; i < s1->cardinality; i++) {
        if (!in(s2, s1->elements[i])) {
            insertElement(result, s1->elements[i]);
        }
    }
    return result;
}

/**
 * This function prints the given element depending on its type.
 */
void printElement(Element element) {
	switch (element->type) {
        case INTEGER:
            printf("%d", *(int*)element->data);
            break;
        case FLOAT:
            printf("%f", *(float*)element->data);
            break;
        case STRING:
            printf("%s", (char*)element->data);
            break;
        case MATRIX_POINT: {
            int* mp = (int*)element->data;
            printf("(%d, %d, %d)", mp[0], mp[1], mp[2]);
            break;
        }
    }
}

/**
 * This function prints the given set element by element depending on its type.
 */
void printSet(Set set) {
	printf("{ ");
    for (int i = 0; i < set->cardinality; i++) {
        printElement(set->elements[i]);
        if (i < set->cardinality - 1) {
            printf(", ");
        }
    }
    printf(" }");
}

/**
 * This element deallocates all data in the given set and the set itself.
 */
void freeSet(Set set) {
	for (int i = 0; i < set->cardinality; i++) {
        freeElement(set->elements[i]);
    }
    free(set->elements);
    free(set);
}

void freeElement(Element element) {
    if (element == NULL) return;

    switch (element->type) {
        case INTEGER:
            free(element->data);
            break;
        case FLOAT:
            free(element->data);
            break;
        case STRING:
            free((char*)element->data);
            break;
        case MATRIX_POINT:
            free((int*)element->data); 
            break;
    }

    free(element);
}
