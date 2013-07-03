/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

// forward declarations
template<typename ValueType> class SparseMatrix;

// typedefs
typedef SparseMatrix<unsigned char> UcharSparseMatrix;

/**
 * \ingroup ldpc
 * \brief Sparse matrix representation
 *
 * Sparse matrix representation, in something like "compressed row storage" as
 * 		defined in sparselib++ (http://math.nist.gov/sparselib++/, section 2.3
 * 		of v1.5 documentation, April 1996)
 *
 * In this matrix representation, empty cells are not equivalent to cells that
 * 		contain 0. This allows for specifications of Matrix LDPC codes, like
 * 		the ones specified in 802.11n-2009 standard (section 20.3.11.6.4
 * 		and Annex R)
 *
 * To retrieve values for a specific row, one first gets the 'rowPtr' for that
 * 		row by calling rowPtr(). Then, the values are obtained by calling val(),
 * 		and column indices by calling colInd(). To get all the values in the
 * 		row, one calls val() and colInd() with ptrs:
 * 			rowPtr, rowPtr+1, ..., rowPtr+rowDegree(rowInd)-1
 */

template<typename ValueType>
class SparseMatrix {
public:
	SparseMatrix(int numRows,
				 const unsigned int* rowPtrs,
				 unsigned int numValues,
				 const ValueType* values,
				 const unsigned int* colInd);

	/**
	 * @return the number of rows
	 */
	unsigned int dim() const;

	/**
	 * @return the number of non-empty places in the matrix
	 */
	unsigned int degree() const;

	/**
	 * @return the number of non-empty places in row
	 * @param rowInd: the row to examine
	 */
	unsigned int rowDegree(unsigned int rowInd) const;

	/**
	 * Returns the "rowPtr" used to access values and column indices.
	 * @return pointer to values in row
	 * @param rowInd: the row
	 */
	unsigned int rowPtr(unsigned int rowInd) const;

	/**
	 * @return the column index of the value referenced by 'ptr'
	 */
	unsigned int colInd(unsigned int ptr) const;

	/**
	 * @return the value referenced by 'ptr'
	 */
	ValueType val(unsigned int ptr) const;

private:
	// i'th value is the index of the first value in m_value and m_columnInd
	// that belongs to the i'th row.
	std::vector<unsigned int> m_rowPtr;

	// values
	std::vector<ValueType> m_value;

	// index of the column of the value. The row can be inferred from m_rowPtr
	std::vector<unsigned int> m_columnInd;
};



#include "SparseMatrix.hh"
