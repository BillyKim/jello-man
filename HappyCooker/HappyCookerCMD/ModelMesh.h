#pragma once
#include "stdafx.h"
#include <vector>
#include "vertex.h"

template <typename T>
class ModelMesh
{
public:
    ModelMesh(const string& name) : 
                m_Name(name)
    {
    }
    ~ModelMesh(void)
    {
    }
   
    void SetIndices(vector<unsigned int> indices)
    {
	    m_VecIndices = indices;
    }   
    void SetVertices(vector<T> vertices)
    {
	    m_VecVertices = vertices;
    }  

    const vector<unsigned int>& GetIndices() const { return m_VecIndices; }
    const vector<T>& GetVertices() const { return m_VecVertices; }

    unsigned int GetNumVertices() const
    {
        return m_VecVertices.size();
    }
    unsigned int GetNumTriangles() const
    {
        return m_VecIndices.size() / 3;
    }
    unsigned int GetNumBytesPerVertex() const
    {
        return sizeof(T);
    }
    unsigned int GetNumBytesPerIndex() const
    {
        return sizeof(unsigned int);
    }
    const string& GetName() const
    {
        return m_Name;
    }

private:
    string m_Name;
    vector<unsigned int> m_VecIndices;
    vector<T> m_VecVertices;
};
