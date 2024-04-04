#ifndef VECTORHEADERDEF
#define VECTORHEADERDEF

#include <string>
#include <ostream>
#include <cassert>

template <typename T>
class Vector
{
private:
   T* mData; // data stored in vector
   size_t mSize; // size of vector
public:
	Vector() {};
	// Overridden copy constructor
	// Allocates memory for new vector, and copies
	// entries of other vector into it
	Vector(const Vector& otherVector)
	{
		mSize = otherVector.GetSize();
		mData = new T[mSize];
		for (size_t i = 0; i<mSize; i++)
		{
			mData[i] = otherVector.mData[i];
		}
	}

	// Constructor for vector of a given size
	// Allocates memory, and initialises entries
	// to zero
	Vector(size_t size)
	{
		mSize = size;
		mData = new T[mSize];
		for (size_t i = 0; i<mSize; i++)
		{
			mData[i] = 0.0;
		}
	}

	// Overridden destructor to correctly free memory
	~Vector()
	{
		delete[] mData;
	}

	// Method to get the size of a vector
	size_t GetSize() const
	{
		return mSize;
	}

	//zero-based indexing
	T& operator[](size_t i)
	{
		assert(i < mSize);
		return mData[i];
	}

	// Access the individual elements
	T& operator()(const size_t& i)
	{
		assert(i < mSize);
		return mData[i];
	}
	const T& operator()(const size_t& i) const
	{
		assert(i < mSize);
		return mData[i];
	}

   // assignment
	Vector& operator=(const Vector& otherVector)
	{
		assert(mSize == otherVector.mSize);
		for (size_t i = 0; i<mSize; i++)
		{
			mData[i] = otherVector.mData[i];
		}
		return *this;
	}

	// unary +
	Vector operator+() const
	{
		Vector v(mSize);
		for (size_t i = 0; i<mSize; i++)
		{
			v[i] = mData[i];
		}
		return v;
	}

	//unary -
	Vector operator-() const
	{
		Vector v(mSize);
		for (size_t i = 0; i<mSize; i++)
		{
			v[i] = -mData[i];
		}
		return v;
	}

	//binary +
	Vector operator+(const Vector& v1) const
	{
		assert(mSize == v1.mSize);
		Vector v(mSize);
		for (size_t i = 0; i<mSize; i++)
		{
			v[i] = mData[i] + v1.mData[i];
		}
		return v;
	}

	//binary -
	Vector operator-(const Vector& v1) const
	{
		assert(mSize == v1.mSize);
		Vector v(mSize);
		for (size_t i = 0; i<mSize; i++)
		{
			v[i] = mData[i] - v1.mData[i];
		}
		return v;
	}

	// scalar multiplication
	Vector operator*(T a) const
	{
		Vector v(mSize);
		for (size_t i = 0; i<mSize; i++)
		{
			v[i] = a*mData[i];
		}
		return v;
	}

   // p-norm method
   //double CalculateNorm(int p=2) const;
	/*double norm_val, sum = 0.0;
	for (int i = 0; i<mSize; i++)
	{
		sum += pow(fabs(mData[i]), p);
	}
	norm_val = pow(sum, 1.0 / ((double)(p)));
	return norm_val;*/
   // declare length function as a friend
   friend unsigned length(const Vector& v)
   {
	   return v.mSize;
   }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec)
{
	for (int i = 0; i < vec.GetSize(); i++)
	{
		os << vec(i) << " ";
	}
	os << std::endl;
	return os;
};

#endif
