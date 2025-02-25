#include "matrix.h"

/**
 * This function initializes and returns a dense matrix 
 * as a dynamic two-dimensional array.
 */
DenseMatrix initDenseMatrix(int columnLength, int rowLength) {
	DenseMatrix denseMatrix = (DenseMatrix)malloc(sizeof(DenseMatrix_t));
    if (denseMatrix == NULL) return NULL;
    denseMatrix->columnLength = columnLength;
    denseMatrix->rowLength = rowLength;
    denseMatrix->matrix = (int**)malloc(rowLength * sizeof(int*));
    if (denseMatrix->matrix == NULL) return NULL;
    
    for (int i = 0; i < rowLength; i++) {
        denseMatrix->matrix[i] = (int*)malloc(columnLength * sizeof(int));
        if (denseMatrix->matrix[i] == NULL) return NULL;
        for (int j = 0; j < columnLength; j++) {
            denseMatrix->matrix[i][j] = 0;  
        }
    }
    return denseMatrix;
}

/**
 * This function deallocates all the rows and 
 * columns of the given dense matrix.
 */
void freeDenseMatrix(DenseMatrix denseMatrix) {
	for (int i = 0; i < denseMatrix->rowLength; i++) {
        free(denseMatrix->matrix[i]);
    }
    free(denseMatrix->matrix);
    free(denseMatrix);
}

/**
 * This function creates a dense matrix and fills 
 * the elements in the given parse matrix as a set.
 */
DenseMatrix parseMatrixToDenseMatrix(Set parseMatrix, int columnLength, int rowLength) {
	DenseMatrix denseMatrix = initDenseMatrix(columnLength, rowLength);
    for (int i = 0; i < parseMatrix->cardinality; i++) {
        Element element = parseMatrix->elements[i];
        int* point = (int*)element->data;
        int row = point[0];
        int col = point[1];
        int value = point[2];
        denseMatrix->matrix[row][col] = value;
    }
    return denseMatrix;
}

/**
 * This function creates and returns a parse matrix 
 * as a set depending on the given dense matrix's elements.
 */
Set denseMatrixToParseMatrix(DenseMatrix denseMatrix) {
	Set sparseMatrix = initSet();
    for (int i = 0; i < denseMatrix->rowLength; i++) {
        for (int j = 0; j < denseMatrix->columnLength; j++) {
            int value = denseMatrix->matrix[i][j];
            if (value != 0) {  
                insertElement(sparseMatrix, createMatrixPointElement(i, j, value));
            }
        }
    }
    return sparseMatrix;
}

/**
 * This matrix creates a new dense matrix, and the 
 * matrix is the addition of the given two matrices.
 */
DenseMatrix addDenseMatrices(DenseMatrix dm1, DenseMatrix dm2) {
	if (dm1->columnLength != dm2->columnLength || dm1->rowLength != dm2->rowLength) {
        return NULL; 
    }

    DenseMatrix result = initDenseMatrix(dm1->columnLength, dm1->rowLength);
    for (int i = 0; i < dm1->rowLength; i++) {
        for (int j = 0; j < dm1->columnLength; j++) {
            result->matrix[i][j] = dm1->matrix[i][j] + dm2->matrix[i][j];
        }
    }
    return result;
}

/**
 * This matrix creates a new sparse matrix as a set, 
 * and the matrix is the addition of the given 
 * two sparse matrices.
 */
Set addSparseMatrices(Set sm1, Set sm2, int columnLength, int rowLength) {
	Set result = initSet();

    for (int i = 0; i < sm1->cardinality; i++) {
        Element element = sm1->elements[i];
        int* point = (int*)element->data;
        if (point[2] != 0) { 
            insertElement(result, createMatrixPointElement(point[0], point[1], point[2]));
        }
    }

    for (int i = 0; i < sm2->cardinality; i++) {
        Element element = sm2->elements[i];
        int* point = (int*)element->data;
        int row = point[0];
        int col = point[1];
        int value = point[2];

        Element existingElement = NULL;
        for (int j = 0; j < result->cardinality; j++) {
            int* existingPoint = (int*)result->elements[j]->data;
            if (existingPoint[0] == row && existingPoint[1] == col) {
                existingElement = result->elements[j];
                break;
            }
        }

        if (existingElement) {
            int newValue = ((int*)existingElement->data)[2] + value;
            if (newValue != 0) { 
                ((int*)existingElement->data)[2] = newValue;
            } else { 
                removeElement(result, existingElement);
            }
        } else if (value != 0) {
            insertElement(result, createMatrixPointElement(row, col, value));
        }
    }
    return result;
}


