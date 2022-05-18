#pragma once

#include <new>
#include <utility>
#include <iterator>

namespace stl
{


template<typename T>
class vector
{

public:
    using value_type=T;
    using size_type=unsigned long long;
    using iterator=T*;
    using const_iterator=const T*;
    using reverse_iterator=typename std::reverse_iterator<iterator>;
    using const_reverse_iterator=typename std::reverse_iterator<const_iterator>;

    vector();
    vector(size_type, const T&);
    explicit vector(size_type);
    vector(const vector<T>&);
    vector(vector<T>&&);
    vector(std::initializer_list<T>);
    template<typename InputIt>
    vector(InputIt,InputIt);
    ~vector();

    vector<T>& operator=(const vector<T>&);
    vector<T>& operator=(vector<T>&&);

    size_type size() const {return m_sz;}
    size_type capacity() const {return m_capacity;}
    
    T& operator[](size_type i) {return m_elem[i];}
    const T& operator[](size_type i) const {return m_elem[i];}

    T* data() {return m_elem;}
    const T* data() const {return m_elem;}
    
    T& front() {return m_elem[0];}
    const T& front() const {return m_elem[0];}
    
    T& back() {return m_elem[m_sz-1];}
    const T& back() const {return m_elem[m_sz-1];}
    
    T& at(size_type);
    const T& at(size_type) const;

    void clear();

    void push_back(const T&);
    void push_back(T&& value);
    void pop_back() { if (m_sz>0) m_elem[--m_sz].~T(); }

    iterator begin() {return m_elem;}
    iterator end() {return m_elem+m_sz;}
    const_iterator cbegin() const {return m_elem;}
    const_iterator cend() const {return m_elem+m_sz;}

    reverse_iterator rbegin() {return reverse_iterator{m_elem+m_sz};}
    reverse_iterator rend() {return reverse_iterator{m_elem};};
    const_reverse_iterator crbegin() const {return const_reverse_iterator{m_elem+m_sz};};
    const_reverse_iterator crend() const {return const_reverse_iterator{m_elem};};


    iterator erase(const_iterator,const_iterator);
    iterator erase(const_iterator pos) {return erase(pos,pos+1);};    

    template<typename ...Args>
    T& emplace_back(Args&&...);


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
template<typename InputIt>
vector<T>::vector(InputIt first,InputIt last)
: m_elem{nullptr}
, m_sz{0}
, m_capacity{0}
{
    size_type sz=0;

    InputIt it=first;
    while (it++!=last) ++sz;

    if (sz)
    {
        m_sz=m_capacity=sz;
        m_elem=static_cast<T*>( operator new(m_sz*sizeof(T)) );
        
        size_type i=0;
        while (first!=last)
        {
            new (m_elem+i) T{ static_cast<T>(*first) };
            ++first;
            ++i;
        }

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


template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& other)
{

    if (m_capacity>=other.m_sz)
    {
        
        int k=std::max(m_sz,other.m_sz);
        
        for (size_type i=0;i<k;++i)
        {
            if (i<m_sz)
                m_elem[i].~T();

            if (i<other.m_sz)
                new(m_elem+i) T{other.m_elem[i]};
        }
        
        m_sz=other.m_sz;
    }
    else
    {
        vector<T> tmp{other};
        *this=std::move(tmp);
    
    }
    return *this;
}


template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other)
{

    if (m_elem)
    {
        for (size_type i=0;i<m_sz;++i)
            m_elem[i].~T();
        
        operator delete(m_elem);

    }

    m_elem=other.m_elem;
    m_sz=other.m_sz;
    m_capacity=other.m_capacity;

    other.m_elem=nullptr;
    other.m_sz=other.m_capacity=0;

    return *this;
}



template <typename T>
T& vector<T>::at(size_type pos)
{
    if (pos>=0 && pos<m_sz)
        return m_elem[pos];
    else
        throw std::out_of_range{"Out of range access attempt"};
}

template <typename T>
const T& vector<T>::at(size_type pos) const 
{
    if (pos>=0 && pos<m_sz)
        return m_elem[pos];
    else
        throw std::out_of_range{"Out of range access attempt"};    
}


template <typename T>
void vector<T>::clear()
{
    if (m_sz>0)
    {
        for (size_type i=0;i<m_sz;++i)
            m_elem[i].~T();
        
        m_sz=0;
    }

}


template <typename T>
void vector<T>::push_back(const T& value)
{
    
    if (m_sz<m_capacity)
    {
        new (m_elem+m_sz) T{value};
        ++m_sz;
    }
    else if (m_elem==nullptr)
    {
        m_elem=static_cast<T*>( operator new(sizeof(T)) );
        new (m_elem) T{value};
        m_sz=m_capacity=1;
    }
    else
    {
        T *ptr=static_cast<T*>( operator new(2*m_sz*sizeof(T)) );
        for (size_type i=0; i<m_sz;++i)
        {
            new (ptr+i) T{ std::move(m_elem[i]) };
            m_elem[i].~T();
        }

        new(ptr+m_sz) T{value};
        m_capacity=2*m_sz;
        ++m_sz;

        operator delete(m_elem);
        m_elem=ptr;
    }


}


template <typename T>
void vector<T>::push_back(T&& value)
{
    
    if (m_sz<m_capacity)
    {
        new (m_elem+m_sz) T{std::move(value)};
        ++m_sz;
    }
    else if (m_elem==nullptr)
    {
        m_elem=static_cast<T*>( operator new(sizeof(T)) );
        new (m_elem) T{std::move(value)};
        m_sz=m_capacity=1;
    }
    else
    {
        T *ptr=static_cast<T*>( operator new(2*m_sz*sizeof(T)) );
        for (size_type i=0; i<m_sz;++i)
        {
            new (ptr+i) T{ std::move(m_elem[i]) };
            m_elem[i].~T();
        }

        new(ptr+m_sz) T{std::move(value)};
        m_capacity=2*m_sz;
        ++m_sz;

        operator delete(m_elem);
        m_elem=ptr;
    }


}


template<typename T>
bool operator==(const vector<T>& lhs,const vector<T>& rhs)
{

    if (lhs.size()!=rhs.size()) return false;

    for (typename vector<T>::size_type i=0; i<lhs.size();++i)
        if (lhs[i]!=rhs[i]) return false;
    
    return true;
}


template<typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
{

    iterator a=const_cast<iterator>(first);
    iterator b=const_cast<iterator>(last);
    
    const_iterator e=cend();

    size_type i=0;
    for (;a+i!=b;++i)
        a[i].~T();

    for (;b!=e;++b)
    {
        new (b-i) T{std::move(*b)};
        b->~T();
    }


    m_sz-=last-first;

    return a;

}


template<typename T>
template<typename... Args>
T& vector<T>::emplace_back(Args&&... args)
{
    
    if (m_sz<m_capacity)
    {
        new (m_elem+m_sz) T{std::forward<Args>(args)...};
        ++m_sz;
    }
    else if (m_elem==nullptr)
    {
        m_elem=static_cast<T*>( operator new(sizeof(T)) );
        new (m_elem) T{std::forward<Args>(args)...};
        m_sz=m_capacity=1;
    }
    else
    {
        T *ptr=static_cast<T*>( operator new(2*m_sz*sizeof(T)) );
        for (size_type i=0; i<m_sz;++i)
        {
            new (ptr+i) T{ std::move(m_elem[i]) };
            m_elem[i].~T();
        }

        new(ptr+m_sz) T{std::forward<Args>(args)...};
        m_capacity=2*m_sz;
        ++m_sz;

        operator delete(m_elem);
        m_elem=ptr;
    }

    return *(m_elem+m_sz-1);
}



}