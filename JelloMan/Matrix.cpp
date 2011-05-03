#include "Matrix.h"
#include "Vector3.h"

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
Matrix::Matrix(const NxMat34& mat)
{
    mat.getColumnMajor44(m_Matrix);
}


Matrix Matrix::CreateTranslation(const Vector3& v)
{
    Matrix m;
    D3DXMatrixTranslation(&m.m_Matrix, v.X, v.Y, v.Z);
    return m;
}
Matrix Matrix::CreateRotationX(float f)
{
    Matrix m;
    D3DXMatrixRotationX(&m.m_Matrix, f);
    return m;
}
Matrix Matrix::CreateRotationY(float f)
{
    Matrix m;
    D3DXMatrixRotationY(&m.m_Matrix, f);
    return m;
}
Matrix Matrix::CreateRotationZ(float f)
{
    Matrix m;
    D3DXMatrixRotationZ(&m.m_Matrix, f);
    return m;
}
Matrix Matrix::CreateRotation(const Vector3& axis, float f)
{
    Matrix m;
    D3DXMatrixRotationAxis(&m.m_Matrix, &axis.ToD3DVector3(), f);
    return m;
}
Matrix Matrix::CreateScale(float f)
{
    Matrix m;
    D3DXMatrixScaling(&m.m_Matrix, f, f, f);
    return m;
}
Matrix Matrix::CreateScale(const Vector3& f)
{
    Matrix m;
    D3DXMatrixScaling(&m.m_Matrix, f.X, f.Y, f.Z);
    return m;
}
Matrix Matrix::CreateLookAt(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    Matrix m;
    D3DXMatrixLookAtLH(&m.m_Matrix, &pos.ToD3DVector3(), &target.ToD3DVector3(), &up.ToD3DVector3());
    return m;
}
Matrix Matrix::CreatePerspectiveFov(float fov, float aspect, float zNear, float zFar)
{
    Matrix m;
    D3DXMatrixPerspectiveFovLH(&m.m_Matrix, fov, aspect, zNear, zFar);
    return m;
}


Matrix::Matrix(const Matrix& mat): m_Matrix(mat)
{
}
Matrix& Matrix::operator=(const Matrix& mat)
{
    m_Matrix = mat.m_Matrix;
    return *this;
}
Matrix& Matrix::operator=(const NxMat34& mat)
{
    mat.getColumnMajor44(m_Matrix);
    return *this;
}
Matrix& Matrix::operator=(const D3DXMATRIX& mat)
{
    m_Matrix = mat;
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
Matrix::operator NxMat34() const
{
	NxMat34 mat;
	mat.setColumnMajor44(m_Matrix);
	return mat;
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
