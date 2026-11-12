#pragma once

#include <cstddef>
#include <cstdint>

template<typename T>
class Vector
{
private:
    T* _Head;
    size_t _Size;
    size_t _Capacity;

    void _ReAlloc()
    {
        _Capacity <<= 1;
        T* _NewHead = new T[_Capacity];

        for (size_t i = 0; i < _Size; i++)
            _NewHead[i] = _Head[i];

        delete[] _Head;
        _Head = _NewHead;
    }

    void _MoveLeft(size_t start, size_t end, size_t steps)
    {
        if (steps > start) return;
        for (size_t i = start; i < end; i++)
            _Head[i - steps] = _Head[i];
    }

    void _MoveRight(size_t start, size_t end, size_t steps)
    {
        if (end + steps > _Capacity)
            return;

        for (size_t i = end; i > start; i--)
            _Head[i + steps - 1] = _Head[i - 1];
    }

    size_t _Find(const T& val, size_t start, size_t end) const
    {
        for (size_t i = start; i < end; i++)
        {
            if (val == _Head[i])
                return i;
        }
        return SIZE_MAX;
    }

public:
    Vector() : _Head(new T[2]), _Size(0), _Capacity(2) {}

    ~Vector()
    {
        delete[] _Head;
    }

    T& operator[](size_t index)
    {
        return *(_Head + index);
    }

    size_t Size() const { return _Size; }
    size_t Capacity() const { return _Capacity; }
    bool   IsEmpty() const { return !_Size; }

    T Front() const
    {
        if (IsEmpty()) return T();
        return *_Head;
    }

    T Back() const
    {
        if (IsEmpty()) return T();
        return _Head[_Size - 1];
    }

    void Clear()
    {
        _Size = 0;
    }

    void push(const T& val)
    {
        if (_Size == _Capacity) _ReAlloc();
        _Head[_Size] = val;
        _Size++;
    }

    void pop()
    {
        if (IsEmpty()) return;
        _Size--;
    }

    void Delete(size_t index)
    {
        if (IsEmpty() || index >= _Size) return;
        _MoveLeft(index + 1, _Size, 1);
        _Size--;
    }

    void Remove(const T& val)
    {
        size_t write = Find(val);
        if (write == SIZE_MAX) return;

        for (size_t read = write; read < _Size; read++)
        {
            if (_Head[read] != val)
                _Head[write++] = _Head[read];
        }
        _Size = write;
    }

    size_t Find(const T& val) const
    {
        return _Find(val, static_cast<size_t>(0), _Size);
    }

    void Insert(size_t index, const T& val)
    {
        if (index > _Size) return;
        if (_Size == _Capacity) _ReAlloc();

        _MoveRight(index, _Size, 1);
        _Head[index] = val;
        _Size++;
    }

    void Prepend(const T& val)
    {
        Insert(0, val);
    }
};