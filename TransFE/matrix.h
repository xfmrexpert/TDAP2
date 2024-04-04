#ifndef MATRIXHEADERDEF
#define MATRIXHEADERDEF
#include "vector.h"
#include <ostream>
#include <cassert>

template <typename T>
class Matrix
{
private:
   T** mData; // entries of matrix
   size_t mNumRows, mNumCols; // dimensions
public:
	Matrix() {};
	// Overwritten copy constructor
	// Allocate memory for new matrix, and copy
	// entries into this matrix
	Matrix(const Matrix& otherMatrix)
	{
		mNumRows = otherMatrix.mNumRows;
		mNumCols = otherMatrix.mNumCols;
		mData = new T*[mNumRows];
		for (size_t i = 0; i<mNumRows; i++)
		{
			mData[i] = new T[mNumCols];
		}
		for (size_t i = 0; i<mNumRows; i++)
		{
			for (size_t j = 0; j<mNumCols; j++)
			{
				mData[i][j] = otherMatrix.mData[i][j];
			}
		}
	};

	// Constructor for vector of a given length
	// Allocates memory, and initialises entries
	// to zero
	Matrix(size_t numRows, size_t numCols)
	{
		mNumRows = numRows;
		mNumCols = numCols;
		mData = new T*[mNumRows];
		for (size_t i = 0; i<mNumRows; i++)
		{
			mData[i] = new T[mNumCols];
		}
		for (size_t i = 0; i<mNumRows; i++)
		{
			for (size_t j = 0; j<mNumCols; j++)
			{
				mData[i][j] = 0.0;
			}
		}
	};

	// Overidden destructor to correctly free memory
	~Matrix()
	{
		for (size_t i = 0; i<mNumRows; i++)
		{
			delete[] mData[i];
		}
		delete[] mData;
	};

	size_t GetNumberOfRows() const
	{
		return mNumRows;
	};

	size_t GetNumberOfColumns() const
	{
		return mNumCols;
	};

	// Access the individual elements
	T& operator()(const size_t& row, const size_t& col)
	{
		assert(row < mNumRows);
		assert(col < mNumCols);
		return mData[row][col];
	}
	const T& operator()(const size_t& row, const size_t& col) const
	{
		assert(row < mNumRows);
		assert(col < mNumCols);
		return mData[row][col];
	}

   //overloaded assignment operator
	Matrix& operator=(const Matrix& otherMatrix)
	{
		mNumRows = otherMatrix.mNumRows;
		mNumCols = otherMatrix.mNumCols;
		mData = new T*[mNumRows];
		for (size_t i = 0; i<mNumRows; i++)
		{
			mData[i] = new T[mNumCols];
		}
		for (size_t i = 0; i<mNumRows; i++)
		{
			for (size_t j = 0; j<mNumCols; j++)
			{
				mData[i][j] = otherMatrix.mData[i][j];
			}
		}
		return *this;
	};

   //unary +
   Matrix operator+() const
   {
	   Matrix mat(mNumRows, mNumCols);
	   for (size_t i = 0; i<mNumRows; i++)
	   {
		   for (size_t j = 0; j<mNumCols; j++)
		   {
			   mat[i][j] = mData[i][j];
		   }
	   }
	   return mat;
   };

   //unary -
   Matrix operator-() const
   {
	   Matrix mat(mNumRows, mNumCols);
	   for (size_t i = 0; i<mNumRows; i++)
	   {
		   for (size_t j = 0; j<mNumCols; j++)
		   {
			   mat[i][j] = -mData[i][j];
		   }
	   }
	   return mat;
   };

   // binary +
   Matrix operator+(const Matrix& m1) const
   {
	   assert(mNumRows == m1.mNumRows);
	   assert(mNumCols == m1.mNumCols);
	   Matrix mat(mNumRows, mNumCols);
	   for (size_t i = 0; i<mNumRows; i++)
	   {
		   for (size_t j = 0; j<mNumCols; j++)
		   {
			   mat(i, j) = mData[i][j] + m1.mData[i][j];
		   }
	   }
	   return mat;
   };

   //binary -
   Matrix operator-(const Matrix& m1) const
   {
	   assert(mNumRows == m1.mNumRows);
	   assert(mNumCols == m1.mNumCols);
	   Matrix mat(mNumRows, mNumCols);
	   for (size_t i = 0; i<mNumRows; i++)
	   {
		   for (size_t j = 0; j<mNumCols; j++)
		   {
			   mat(i, j) = mData[i][j] - m1.mData[i][j];
		   }
	   }
	   return mat;
   };

   // scalar multiplication
   Matrix operator*(T a) const
   {
	   Matrix mat(mNumRows, mNumCols);
	   for (size_t i = 0; i<mNumRows; i++)
	   {
		   for (size_t j = 0; j<mNumCols; j++)
		   {
			   mat(i, j) = a*mData[i][j];
		   }
	   }
	   return mat;
   };

   Matrix<T> CalculateTranspose() const
   {
	   Matrix<T> transpose(mNumCols, mNumRows);
	   for (size_t i = 0; i<mNumRows; i++)
	   {
		   for (size_t j = 0; j<mNumCols; j++)
		   {
			   transpose(j, i) = mData[i][j];
		   }
	   }
	   return transpose;
   }

   T CalculateDeterminant() const
   {
	   assert(mNumRows == mNumCols);
	   T determinant = 0.0;

	   if (mNumRows == 1)
	   {
		   determinant = mData[0][0];
	   }
	   else
	   {
		   // More than one entry of matrix
		   for (size_t i_outer = 0; i_outer<mNumRows; i_outer++)
		   {
			   Matrix sub_matrix(mNumRows - 1, mNumRows - 1);
			   for (size_t i = 0; i<mNumRows - 1; i++)
			   {
				   for (size_t j = 0; j<i_outer; j++)
				   {
					   sub_matrix(i, j) = mData[i + 1][j];
				   }
				   for (size_t j = i_outer; j<mNumRows - 1; j++)
				   {
					   sub_matrix(i, j) = mData[i + 1][j + 1];
				   }
			   }
			   T sub_matrix_determinant = sub_matrix.CalculateDeterminant();

			   determinant += pow(-1.0, i_outer) * mData[0][i_outer] * sub_matrix_determinant;
		   }
	   }
	   return determinant;
   };

   Matrix<T> CalculateInverse() const
   {
	   Matrix<T> inverse(mNumCols, mNumRows);
	   size_t order = mNumCols;
	   // get the determinant of a
	   T det = 1.0 / this->CalculateDeterminant();

	   for (size_t j = 0; j<order; j++)
	   {
		   for (size_t i = 0; i<order; i++)
		   {
			   // get the co-factor (matrix) of A(j,i)
			   Matrix<T> minor = this->GetMinor(j, i);
			   inverse(i, j) = det*minor.CalculateDeterminant();
			   if ((i + j) % 2 == 1)
				   inverse(i, j) = -inverse(i, j);
		   }
	   }

	   return inverse;
   }

   // calculate the cofactor of element (row,col)
   Matrix<T> GetMinor(size_t row, size_t col) const
   {
	   // indicate which col and row is being copied to dest
	   size_t colCount = 0, rowCount = 0;
	   size_t order = mNumRows;

	   Matrix sub_matrix(mNumRows - 1, mNumRows - 1);

	   for (size_t i = 0; i < order; i++)
	   {
		   if (i != row)
		   {
			   colCount = 0;
			   for (size_t j = 0; j < order; j++)
			   {
				   // when j is not the element
				   if (j != col)
				   {
					   sub_matrix(rowCount, colCount) = mData[i][j];
					   colCount++;
				   }
			   }
			   rowCount++;
		   }
	   }

	   return sub_matrix;
   }


   friend Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B)
   {
	   assert(A.GetNumberOfColumns() == B.GetNumberOfRows());
	   Matrix<T> product(A.GetNumberOfRows(), B.GetNumberOfColumns());

	   size_t i, j, k;

	   for (i = 0;i<A.GetNumberOfRows();i++) {
		   for (j = 0;j<B.GetNumberOfColumns();j++) {
			   for (k = 0;k<A.GetNumberOfColumns();k++) {
				   product(i, j) += A(i, k) * B(k, j);
			   }
		   }
	   }

	   return product;
   };

   // declare vector multiplication friendship
   friend Vector<T> operator*(const Matrix<T>& m, const Vector<T>& v)
   {
	   size_t original_vector_size = v.GetSize();
	   assert(m.GetNumberOfColumns() == original_vector_size);
	   size_t new_vector_length = m.GetNumberOfRows();
	   Vector<T> new_vector(new_vector_length);

	   for (size_t i = 0; i<new_vector_length; i++)
	   {
		   for (size_t j = 0; j<original_vector_size; j++)
		   {
			   new_vector(i) += m(i, j) * v(j);
		   }
	   }

	   return new_vector;
   };

   friend Vector<T> operator*(const Vector<T>& v, const Matrix<T>& m)
   {
	   size_t original_vector_size = v.GetSize();
	   assert(m.GetNumberOfRows() == original_vector_size);
	   size_t new_vector_length = m.GetNumberOfColumns();
	   Vector<T> new_vector(new_vector_length);

	   for (size_t i = 0; i<new_vector_length; i++)
	   {
		   for (size_t j = 0; j<original_vector_size; j++)
		   {
			   new_vector(i) += v(j)*m(j, i);
		   }
	   }

	   return new_vector;
   };
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& A)
{
	for (size_t i = 0; i < A.GetNumberOfRows(); i++) {
		for (size_t j = 0; j < A.GetNumberOfColumns(); j++) {
			os << A(i, j) << " ";
		}
		os << std::endl;
	}
	return os;
};

#endif
