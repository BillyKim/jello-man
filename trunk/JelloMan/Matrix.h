#pragma once
#include "D3DUtil.h"

struct Matrix
{
public:
	Matrix(void);
    Matrix( float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44 );
	explicit Matrix(const float* mat);
	explicit Matrix(const D3DXMATRIX& mat);
	
    //copy & assignment
    Matrix(const Matrix& mat);
    Matrix& operator=(const Matrix& mat);

    //Static
    static const Matrix Identity;

	virtual ~Matrix(void);

    // casting operators
    operator float*();
    operator const float*() const;
    operator D3DXMATRIX() const;
    operator const D3DXMATRIX() const;

    // assignment operators
    Matrix& operator*= (const Matrix& mat);
    Matrix& operator+= (const Matrix& mat);
    Matrix& operator-= (const Matrix& mat);
    Matrix& operator*= (float f);
    Matrix& operator/= (float f);

    // unary operators
    Matrix operator+ () const;
    Matrix operator- () const;

    float& operator()(UINT row, UINT column);

    // binary operators
    Matrix operator* (const Matrix& mat) const;
    Matrix operator+ (const Matrix& mat) const;
    Matrix operator- (const Matrix& mat) const;
    Matrix operator* (float f) const;
    Matrix operator/ (float f) const;

    bool operator== (const Matrix& mat) const;
    bool operator!= (const Matrix& mat) const;

private:
    D3DXMATRIX m_Matrix;
};

