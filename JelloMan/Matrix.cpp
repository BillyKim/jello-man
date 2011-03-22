#include "Matrix.h"

Matrix::Matrix(void): m_Matrix()
{
}
Matrix::Matrix( float _11, float _12, float _13, float _14,
                float _21, float _22, float _23, float _24,
                float _31, float _32, float _33, float _34,
                float _41, float _42, float _43, float _44 ):
    m_Matrix(_11,  _12,  _13,  _14,
             _21,  _22,  _23,  _24,
             _31,  _32,  _33,  _34,
             _41,  _42,  _43,  _44)
{
}
Matrix::Matrix(const float* mat): m_Matrix(mat)
{
}
Matrix::Matrix(const D3DXMATRIX& mat): m_Matrix(mat)
{
}
Matrix::Matrix(const Matrix& mat): m_Matrix(mat)
{
}
Matrix& Matrix::operator=(const Matrix& mat)
{
    m_Matrix = mat.m_Matrix;
    return *this;
}

//Static
const Matrix Matrix::Identity = Matrix(1, 0, 0, 0,
                                       0, 1, 0, 0,
                                       0, 0, 1, 0,
                                       0, 0, 0, 1);


Matrix::~Matrix(void)
{
}

// casting operators
Matrix::operator float*()
{
    return (float*)m_Matrix;
}
Matrix::operator const float*() const
{
    return (const float*)m_Matrix;
}
Matrix::operator D3DXMATRIX() const
{
    return m_Matrix;
}
Matrix::operator const D3DXMATRIX() const
{
    return m_Matrix;
}

// assignment operators
Matrix& Matrix::operator*= (const Matrix& mat)
{
    m_Matrix *= mat.m_Matrix;
    return *this;
}
Matrix& Matrix::operator+= (const Matrix& mat)
{
    m_Matrix += mat.m_Matrix;
    return *this;
}
Matrix& Matrix::operator-= (const Matrix& mat)
{
    m_Matrix -= mat.m_Matrix;
    return *this;
}
Matrix& Matrix::operator*= (float f)
{
    m_Matrix *= f;
    return *this;
}
Matrix& Matrix::operator/= (float f)
{
    m_Matrix /= f;
    return *this;
}

// unary operators
Matrix Matrix::operator+() const
{
    return *this;
}
Matrix Matrix::operator-() const
{
    return (*this) * -1.0f;
}

float& Matrix::operator()(UINT row, UINT column)
{
    return m_Matrix(row, column);
}

// binary operators
Matrix Matrix::operator*(const Matrix& mat) const
{
    return Matrix(m_Matrix * mat.m_Matrix);
}
Matrix Matrix::operator+(const Matrix& mat) const
{
    return Matrix(m_Matrix + mat.m_Matrix);
}
Matrix Matrix::operator-(const Matrix& mat) const
{
    return Matrix(m_Matrix - mat.m_Matrix);
}
Matrix Matrix::operator*(float f) const
{
    return Matrix(m_Matrix * f);
}
Matrix Matrix::operator/(float f) const
{
    return Matrix(m_Matrix / f);
}

bool Matrix::operator==(const Matrix& mat) const
{
    return m_Matrix == mat;
}
bool Matrix::operator!=(const Matrix& mat) const
{
    return m_Matrix != mat;
}
