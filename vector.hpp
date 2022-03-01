#pragma once


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
    

private:
    T *m_elem;
    size_type m_sz;
    size_type m_capacity;

};



template<typename T>
vector<T>::vector() : m_elem{nullptr}, m_sz{0}, m_capacity{0}
{}




}