// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_MATRIX_H_INCLUDED__
#define __IRR_MATRIX_H_INCLUDED__

#include "irrTypes.h"
#include "vector3d.h"

namespace irr
{
namespace core
{


	//! 4x4 matrix. Mostly used as transformation matrix for 3d calculations.
	class matrix4
	{
		public:

			//! Constructor
			matrix4();

			//! Simple operator for directly accessing every element of the matrix.
			f32& operator()(s32 row, s32 col) { return M[col * 4 + row]; }

			//! Simple operator for directly accessing every element of the matrix.
			const f32& operator()(s32 row, s32 col) const {  return M[col * 4 + row]; }

			//! Sets this matrix equal to the other matrix.
			matrix4& operator=(const matrix4 &other);

			//! Returns true if other matrix is equal to this matrix.
			bool operator==(const matrix4 &other) const;

			//! Returns true if other matrix is not equal to this matrix.
			bool operator!=(const matrix4 &other) const;

			//! Multiply by another matrix.
			matrix4& operator*=(const matrix4& other);

			//! Multiply by another matrix.
			matrix4 operator*(const matrix4& other) const;

			//! Set matrix to identity. 
			void makeIdentity();												

			//! Set the translation of the current matrix. Will erase any previous values.
			void setTranslation( const vector3df& translation );			

			//! Gets the current translation
			inline vector3df matrix4::getTranslation() const;

			//! Set the inverse translation of the current matrix. Will erase any previous values.
			void setInverseTranslation( const vector3df& translation );	

			//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
			void setRotationRadians( const vector3df& rotation );			

			//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
			void setRotationDegrees( const vector3df& rotation );			

			//! Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
			void setInverseRotationRadians( const vector3df& rotation );	

			//! Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
			void setInverseRotationDegrees( const vector3df& rotation );	

			//! Set Scale
			void setScale( const vector3df& scale );
			
			//! Translate a vector by the inverse of the translation part of this matrix.
			void inverseTranslateVect( vector3df& vect ) const;			

			//! Rotate a vector by the inverse of the rotation part of this matrix.
			void inverseRotateVect( vector3df& vect ) const;				

			//! Transforms the vector by this matrix
			void transformVect( vector3df& vect) const;

			//! Transforms input vector by this matrix and stores result in output vector 
			void transformVect( const vector3df& in, vector3df& out);

			//! Translate a vector by tthe translation part of this matrix.
			void translateVect( vector3df& vect ) const;

			//! Multiplies this matrix by a 1x4 matrix
			void multiplyWith1x4Matrix(f32* matrix) const;

			//! Calculates inverse of matrix. Slow.
			bool makeInverse();

			//! Builds a right-handed perspective projection matrix based on a field of view
			void buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

			//! Builds a left-handed perspective projection matrix based on a field of view
			void buildProjectionMatrixPerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

			//! Builds a right-handed perspective projection matrix.
			void buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a left-handed perspective projection matrix.
			void buildProjectionMatrixPerspectiveLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a left-handed orthogonal projection matrix.
			void buildProjectionMatrixOrthoLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a right-handed orthogonal projection matrix.
			void buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a left-handed look-at matrix.
			void buildCameraLookAtMatrixLH(const vector3df& position, const vector3df& target, const vector3df& upVector);

			//! Builds a right-handed look-at matrix.
			void buildCameraLookAtMatrixRH(const vector3df& position, const vector3df& target, const vector3df& upVector);

		private:

			//! Matrix data, stored in column-major order
			f32 M[16];
	};


	inline matrix4::matrix4()
	{
		makeIdentity();
	}


	//! multiply by another matrix
	inline matrix4& matrix4::operator*=(const matrix4& other)
	{
		f32 newMatrix[16];
		const f32 *m1 = M, *m2 = other.M;

		newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		newMatrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
		newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		newMatrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
		newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		newMatrix[11] = m1[3]*m2[8] + m1[7]*m2[10] + m1[11]*m2[10] + m1[15]*m2[11];
		
		newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		newMatrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		
		for (s32 i=0; i<16; ++i)
			M[i] = newMatrix[i];

		return *this;
	}



	//! multiply by another matrix
	inline matrix4 matrix4::operator*(const matrix4& other) const
	{
		matrix4 newMatrix;
		const f32 *m1 = M, *m2 = other.M;
		f32 *m3 = newMatrix.M;

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[10] + m1[11]*m2[10] + m1[15]*m2[11];
		
		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

		return newMatrix;
	}



	inline vector3df matrix4::getTranslation() const
	{
		return vector3df(M[12], M[13], M[14]);
	}


	inline void matrix4::setTranslation( const vector3df& translation )
	{
		M[12] = translation.X;
		M[13] = translation.Y;
		M[14] = translation.Z;
	}

	inline void matrix4::setInverseTranslation( const vector3df& translation )
	{
		M[12] = -translation.X;
		M[13] = -translation.Y;
		M[14] = -translation.Z;
	}

	inline void matrix4::setScale( const vector3df& scale )
	{
		M[0] = scale.X;
		M[5] = scale.Y;
		M[10] = scale.Z;
	}

	inline void matrix4::setRotationDegrees( const vector3df& rotation )
	{
		setRotationRadians( rotation * (f32)3.1415926535897932384626433832795 / 180.0 );
	}

	inline void matrix4::setInverseRotationDegrees( const vector3df& rotation )
	{
		setInverseRotationRadians( rotation * (f32)3.1415926535897932384626433832795 / 180.0 );
	}

	inline void matrix4::setRotationRadians( const vector3df& rotation )
	{
		f64 cr = cos( rotation.X );
		f64 sr = sin( rotation.X );
		f64 cp = cos( rotation.Y );
		f64 sp = sin( rotation.Y );
		f64 cy = cos( rotation.Z );
		f64 sy = sin( rotation.Z );

		M[0] = (f32)( cp*cy );
		M[1] = (f32)( cp*sy );
		M[2] = (f32)( -sp );

		f64 srsp = sr*sp;
		f64 crsp = cr*sp;

		M[4] = (f32)( srsp*cy-cr*sy );
		M[5] = (f32)( srsp*sy+cr*cy );
		M[6] = (f32)( sr*cp );

		M[8] = (f32)( crsp*cy+sr*sy );
		M[9] = (f32)( crsp*sy-sr*cy );
		M[10] = (f32)( cr*cp );
	}

	inline void matrix4::setInverseRotationRadians( const vector3df& rotation )
	{
		f64 cr = cos( rotation.X );
		f64 sr = sin( rotation.X );
		f64 cp = cos( rotation.Y );
		f64 sp = sin( rotation.Y );
		f64 cy = cos( rotation.Z );
		f64 sy = sin( rotation.Z );

		M[0] = (f32)( cp*cy );
		M[4] = (f32)( cp*sy );
		M[8] = (f32)( -sp );

		f64 srsp = sr*sp;
		f64 crsp = cr*sp;

		M[1] = (f32)( srsp*cy-cr*sy );
		M[5] = (f32)( srsp*sy+cr*cy );
		M[9] = (f32)( sr*cp );

		M[2] = (f32)( crsp*cy+sr*sy );
		M[6] = (f32)( crsp*sy-sr*cy );
		M[10] = (f32)( cr*cp );
	}

	inline void matrix4::makeIdentity()
	{
		for (s32 i=0; i<16; ++i)
			M[i] = 0.0f;
		M[0] = M[5] = M[10] = M[15] = 1;
	}

	inline void matrix4::inverseRotateVect( vector3df& vect ) const
	{
		vect.X = vect.X*M[0] + vect.Y*M[1] + vect.Z*M[2];
		vect.Y = vect.X*M[4] + vect.Y*M[5] + vect.Z*M[6];
		vect.Z = vect.X*M[8] + vect.Y*M[9] + vect.Z*M[10];
	}

	inline void matrix4::transformVect( vector3df& vect) const
	{
		f32 vector[3];

		vector[0] = vect.X*M[0] + vect.Y*M[4] + vect.Z*M[8] + M[12];
		vector[1] = vect.X*M[1] + vect.Y*M[5] + vect.Z*M[9] + M[13];
		vector[2] = vect.X*M[2] + vect.Y*M[6] + vect.Z*M[10] + M[14];

		vect.X = vector[0];
		vect.Y = vector[1];
		vect.Z = vector[2];
	}

	inline void matrix4::transformVect( const vector3df& in, vector3df& out)
	{
		out.X = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
		out.Y = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
		out.Z = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
	}

	//! Multiplies this matrix by a 1x4 matrix
	inline void matrix4::multiplyWith1x4Matrix(f32* matrix) const
	{
		/*
		0  1  2  3
		4  5  6  7
		8  9  10 11
		12 13 14 15
		*/

		f32 mat[4];
		mat[0] = matrix[0];
		mat[1] = matrix[1];
		mat[2] = matrix[2];
		mat[3] = matrix[3];

		matrix[0] = M[0]*mat[0] + M[4]*mat[1] + M[8]*mat[2] + M[12]*mat[3];
		matrix[1] = M[1]*mat[0] + M[5]*mat[1] + M[9]*mat[2] + M[13]*mat[3];
		matrix[2] = M[2]*mat[0] + M[6]*mat[1] + M[10]*mat[2] + M[14]*mat[3];
		matrix[3] = M[3]*mat[0] + M[7]*mat[1] + M[11]*mat[2] + M[15]*mat[3];
	}

	inline void matrix4::inverseTranslateVect( vector3df& vect ) const
	{
		vect.X = vect.X-M[12];
		vect.Y = vect.Y-M[13];
		vect.Z = vect.Z-M[14];
	}

	inline void matrix4::translateVect( vector3df& vect ) const
	{
		vect.X = vect.X+M[12];
		vect.Y = vect.Y+M[13];
		vect.Z = vect.Z+M[14];
	}

	inline bool matrix4::makeInverse()
	{
		/// Calculates the inverse of this Matrix 
		/// The inverse is calculated using Cramers rule.
		/// If no inverse exists then 'false' is returned.

		const matrix4 &m = *this;

		f32 d = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3))	- (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3))
				+ (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3))	+ (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3))
				- (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3))	+ (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3));
		
		if (d == 0.f)
			return false;
		else
		{
			d = 1.f / d;

			matrix4 temp;

			temp(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) + m(2, 1) * (m(3, 2) * m(1, 3) - m(1, 2) * m(3, 3)) + m(3, 1) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)));
			temp(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3)) + m(2, 2) * (m(3, 0) * m(1, 3) - m(1, 0) * m(3, 3)) + m(3, 2) * (m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3)));
			temp(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) + m(2, 3) * (m(3, 0) * m(1, 1) - m(1, 0) * m(3, 1)) + m(3, 3) * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)));
			temp(3, 0) = d * (m(1, 0) * (m(3, 1) * m(2, 2) - m(2, 1) * m(3, 2)) + m(2, 0) * (m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2)) + m(3, 0) * (m(2, 1) * m(1, 2) - m(1, 1) * m(2, 2)));
			temp(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) + m(3, 1) * (m(2, 2) * m(0, 3) - m(0, 2) * m(2, 3)) + m(0, 1) * (m(3, 2) * m(2, 3) - m(2, 2) * m(3, 3)));
			temp(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(3, 0) * m(0, 3)) + m(3, 2) * (m(2, 0) * m(0, 3) - m(0, 0) * m(2, 3)) + m(0, 2) * (m(3, 0) * m(2, 3) - m(2, 0) * m(3, 3)));
			temp(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) + m(3, 3) * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) + m(0, 3) * (m(3, 0) * m(2, 1) - m(2, 0) * m(3, 1)));
			temp(3, 1) = d * (m(2, 0) * (m(3, 1) * m(0, 2) - m(0, 1) * m(3, 2)) + m(3, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) + m(0, 0) * (m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2)));
			temp(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)) + m(0, 1) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) + m(1, 1) * (m(3, 2) * m(0, 3) - m(0, 2) * m(3, 3)));
			temp(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(1, 0) * m(0, 3)) + m(0, 2) * (m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3)) + m(1, 2) * (m(3, 0) * m(0, 3) - m(0, 0) * m(3, 3)));
			temp(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) + m(0, 3) * (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) + m(1, 3) * (m(3, 0) * m(0, 1) - m(0, 0) * m(3, 1)));
			temp(3, 2) = d * (m(3, 0) * (m(1, 1) * m(0, 2) - m(0, 1) * m(1, 2)) + m(0, 0) * (m(3, 1) * m(1, 2) - m(1, 1) * m(3, 2)) + m(1, 0) * (m(0, 1) * m(3, 2) - m(3, 1) * m(0, 2)));
			temp(0, 3) = d * (m(0, 1) * (m(2, 2) * m(1, 3) - m(1, 2) * m(2, 3)) + m(1, 1) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) + m(2, 1) * (m(1, 2) * m(0, 3) - m(0, 2) * m(1, 3)));
			temp(1, 3) = d * (m(0, 2) * (m(2, 0) * m(1, 3) - m(1, 0) * m(2, 3)) + m(1, 2) * (m(0, 0) * m(2, 3) - m(2, 0) * m(0, 3)) + m(2, 2) * (m(1, 0) * m(0, 3) - m(0, 0) * m(1, 3)));
			temp(2, 3) = d * (m(0, 3) * (m(2, 0) * m(1, 1) - m(1, 0) * m(2, 1)) + m(1, 3) * (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) + m(2, 3) * (m(1, 0) * m(0, 1) - m(0, 0) * m(1, 1)));
			temp(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) + m(1, 0) * (m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2)) + m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2)));
		
			*this = temp;
			return true;
		}
	}



	inline matrix4& matrix4::operator=(const matrix4 &other)
	{
		for (s32 i = 0; i < 16; ++i)
			M[i] = other.M[i];

		return *this;
	}



	inline bool matrix4::operator==(const matrix4 &other) const
	{
		for (s32 i = 0; i < 16; ++i)
			if (M[i] != other.M[i])
				return false;

		return true;
	}



	inline bool matrix4::operator!=(const matrix4 &other) const
	{
		return !(*this == other);
	}



	//! Builds a right-handed perspective projection matrix based on a field of view
	inline void matrix4::buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
	    f32 h = (f32)(cos(fieldOfViewRadians/2) / sin(fieldOfViewRadians/2));
		f32 w = h / aspectRatio;

		(*this)(0,0) = 2*zNear/w;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2*zNear/h;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = zFar/(zFar-zNear);
		(*this)(3,2) = -1;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0;
	}



	//! Builds a left-handed perspective projection matrix based on a field of view
	inline void matrix4::buildProjectionMatrixPerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
		f32 h = (f32)(cos(fieldOfViewRadians/2) / sin(fieldOfViewRadians/2));
		f32 w = h / aspectRatio;

		(*this)(0,0) = 2*zNear/w;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2*zNear/h;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = zFar/(zFar-zNear);
		(*this)(3,2) = 1;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0;
	}



	//! Builds a left-handed orthogonal projection matrix.
	inline void matrix4::buildProjectionMatrixOrthoLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = 1/(zNear-zFar);
		(*this)(3,2) = 0;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear/(zNear-zFar);
		(*this)(3,3) = 1;
	}



	//! Builds a right-handed orthogonal projection matrix.
	inline void matrix4::buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = 1/(zNear-zFar);
		(*this)(3,2) = 0;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear/(zNear-zFar);
		(*this)(3,3) = -1;
	}


	//! Builds a right-handed perspective projection matrix.
	inline void matrix4::buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2*zNear/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2*zNear/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = zFar/(zNear-zFar);
		(*this)(3,2) = -1;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0;
	}


	//! Builds a left-handed perspective projection matrix.
	inline void matrix4::buildProjectionMatrixPerspectiveLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2*zNear/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2*zNear/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = zFar/(zFar-zNear);
		(*this)(3,2) = 1;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0;
	}




	//! Builds a left-handed look-at matrix.
	inline void matrix4::buildCameraLookAtMatrixLH(	const vector3df& position,
													const vector3df& target,
													const vector3df& upVector)
	{
		vector3df zaxis = target - position;
		zaxis.normalize();

		vector3df xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		vector3df yaxis = zaxis.crossProduct(xaxis);

		(*this)(0,0) = xaxis.X;
		(*this)(1,0) = yaxis.X;
		(*this)(2,0) = zaxis.X;
		(*this)(3,0) = 0;

		(*this)(0,1) = xaxis.Y;
		(*this)(1,1) = yaxis.Y;
		(*this)(2,1) = zaxis.Y;
		(*this)(3,1) = 0;

		(*this)(0,2) = xaxis.Z;
		(*this)(1,2) = yaxis.Z;
		(*this)(2,2) = zaxis.Z;
		(*this)(3,2) = 0;

		(*this)(0,3) = -xaxis.dotProduct(position);
		(*this)(1,3) = -yaxis.dotProduct(position);
		(*this)(2,3) = -zaxis.dotProduct(position);
		(*this)(3,3) = 1.0f;
	}



	//! Builds a right-handed look-at matrix.
	inline void matrix4::buildCameraLookAtMatrixRH(	const vector3df& position,
													const vector3df& target,
													const vector3df& upVector)
	{
		vector3df zaxis = position - target;
		zaxis.normalize();

		vector3df xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		vector3df yaxis = zaxis.crossProduct(xaxis);

		(*this)(0,0) = xaxis.X;
		(*this)(1,0) = yaxis.X;
		(*this)(2,0) = zaxis.X;
		(*this)(3,0) = 0;

		(*this)(0,1) = xaxis.Y;
		(*this)(1,1) = yaxis.Y;
		(*this)(2,1) = zaxis.Y;
		(*this)(3,1) = 0;

		(*this)(0,2) = xaxis.Z;
		(*this)(1,2) = yaxis.Z;
		(*this)(2,2) = zaxis.Z;
		(*this)(3,2) = 0;

		(*this)(0,3) = -xaxis.dotProduct(position);
		(*this)(1,3) = -yaxis.dotProduct(position);
		(*this)(2,3) = -zaxis.dotProduct(position);
		(*this)(3,3) = 1.0f;
	}



} // end namespace core
} // end namespace irr

#endif 

