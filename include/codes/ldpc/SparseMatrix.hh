/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdexcept>

template<typename ValueType>
inline SparseMatrix<ValueType>::SparseMatrix(int numRows,
											const unsigned int *rowPtrs,
											unsigned int numValues,
											const ValueType *values,
											const unsigned int *colInd)
  : m_rowPtr(rowPtrs, rowPtrs + numRows),
    m_value(values, values + numValues),
    m_columnInd(colInd, colInd + numValues)
{
	m_rowPtr.push_back(numValues);
}


template<typename ValueType>
inline unsigned int SparseMatrix<ValueType>::dim() const {
	return m_rowPtr.size() - 1;
}


template<typename ValueType>
inline unsigned int SparseMatrix<ValueType>::degree() const {
	return m_value.size();
}


template<typename ValueType>
inline unsigned int SparseMatrix<ValueType>::rowDegree(unsigned int rowInd) const
{
	return (m_rowPtr[rowInd + 1] - m_rowPtr[rowInd]);
}


template<typename ValueType>
inline unsigned int SparseMatrix<ValueType>::rowPtr(unsigned int rowInd) const {
	if(rowInd >= dim()) {
		throw(std::runtime_error("row index out of range"));
	}

	return m_rowPtr[rowInd];
}


template<typename ValueType>
inline unsigned int SparseMatrix<ValueType>::colInd(unsigned int ptr) const {
	return m_columnInd[ptr];
}


template<typename ValueType>
inline ValueType SparseMatrix<ValueType>::val(unsigned int ptr) const {
	return m_value[ptr];
}
