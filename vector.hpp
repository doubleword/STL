#pragma once

#include <new>
#include <iostream>

namespace stl
{


template<typename T>
class vector
{

public:
    using value_type=T;
    using size_type=unsigned long long;

    vector();
    vector(size_type, const T&);
    explicit vector(size_type);
    vector(const vector<T>&);
    vector(vector<T>&&);
    vector(std::initializer_list<T>);
    ~vector();

    size_type size() const;
    size_type capacity() const;
    T& operator[](size_type);
    const T& operator[](size_type) const;

private:
    T *m_elem;
    size_type m_sz;
    size_type m_capacity;

};



template<typename T>
vector<T>::vector() : m_elem{nullptr}, m_sz{0}, m_capacity{0}
{}

template<typename T>
vector<T>::vector(size_type n, const T& other)
: m_elem{static_cast<T*>(operator new(n*sizeof(T)))}
, m_sz{n}
, m_capacity{n}
{
    for (size_type i=0;i<n;++i) new(m_elem+i) T{other};
}


template<typename T>
vector<T>::vector(size_type n)
: m_elem{ static_cast<T*>( operator new(n*sizeof(T))) }
, m_sz{n}
, m_capacity{n}
{
    for (size_type i=0;i<n;++i) new(m_elem+i) T{};
}



template<typename T>
vector<T>::vector(const vector<T>& other)
: m_elem{nullptr}
, m_sz{other.m_sz}
, m_capacity{other.m_capacity}
{
    if (m_capacity>0)
        m_elem=static_cast<T*>(operator new(m_capacity*sizeof(T)));
    
    if (m_sz>0)
        for (size_type i=0;i<m_sz;++i) new (m_elem+i) T{other.m_elem[i]};
}


template<typename T>
vector<T>::vector(vector<T>&& other) 
: m_elem{other.m_elem}
, m_sz{other.m_sz}
, m_capacity{other.m_capacity}
{
    other.m_elem=nullptr;
    other.m_sz=other.m_capacity=0;
}

template<typename T>
vector<T>::vector(std::initializer_list<T> vals)
: m_elem{nullptr}
, m_sz{vals.size()}
, m_capacity{vals.size()}
{

    if (m_sz>0)
    {
        m_elem=static_cast<T*>( operator new(m_sz*sizeof(T)) );

        const T *ptr=vals.begin();
        for (size_type i=0;i<m_sz;++i)
            new (m_elem+i) T{ptr[i]};


    }
}

template<typename T>
vector<T>::~vector()
{
    if (m_elem)
    {
        for (size_type i=0;i<m_sz;++i)
            m_elem[i].~T();
        
        operator delete(m_elem);
    }
}


template<typename T>
typename vector<T>::size_type vector<T>::size() const {return m_sz;}

template<typename T>
typename vector<T>::size_type vector<T>::capacity() const {return m_capacity;}

template<typename T>
T& vector<T>::operator[](size_type i) {return m_elem[i];}

template<typename T>
const T& vector<T>::operator[](size_type i) const {return m_elem[i];}



}