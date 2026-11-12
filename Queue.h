#pragma once

#include <cstddef>
#include "List.h"

namespace circular
{
    template<typename T, std::size_t cap>
    class Queue
    {
    private:
        T* _Head;
        std::size_t _Start;
        std::size_t _End;
        std::size_t _Size;

    public:
        Queue()
            : _Head(new T[cap]), _Start(0), _End(0), _Size(0)
        {
        }

        ~Queue()
        {
            delete[] _Head;
        }

        std::size_t capacity() const { return cap; }
        std::size_t size()     const { return _Size; }

        bool full()  const { return _Size == cap; }
        bool empty() const { return _Size == 0; }

        T front() const
        {
            return _Head[_Start];
        }

        T back() const
        {
            return _Head[(cap + _End - 1) % cap];
        }

        void enqueue(const T& val)
        {
            if (full())
                return;

            _Head[_End++] = val;
            _End %= cap;
            _Size++;
        }

        T dequeue()
        {
            if (empty())
                return T();

            T val = _Head[_Start++];
            _Start %= cap;
            _Size--;
            return val;
        }
    };
}

namespace array
{
    template<typename T>
    class Queue
    {
    private:
        T* _Head;
        std::size_t _Size;
        std::size_t _Capacity;
        std::size_t _Deviation;

        void _ReAlloc()
        {
            _Capacity <<= 1;
            T* _NewHead = new T[_Capacity];

            for (std::size_t i = 0; i < _Size; i++)
                _NewHead[i] = _Head[i + _Deviation];

            delete[] _Head;
            _Head = _NewHead;
            _Deviation = 0;
        }

        void _Move()
        {
            for (std::size_t i = _Deviation; i < _Size + _Deviation; i++)
                _Head[i - _Deviation] = _Head[i];
            _Deviation = 0;
        }

    public:
        Queue()
            : _Head(new T[2]), _Size(0), _Capacity(2), _Deviation(0)
        {
        }

        ~Queue()
        {
            delete[] _Head;
        }

        T Front() const { return _Head[_Deviation]; }
        T Back()  const { return _Head[_Deviation + _Size - 1]; }

        std::size_t Size()     const { return _Size; }
        std::size_t Capacity() const { return _Capacity; }

        bool Empty() const { return _Size == 0; }
        bool Full()  const { return (_Size + _Deviation) == _Capacity; }

        T Pop()
        {
            if (Empty())
                return T();

            T val = Front();
            _Deviation++;
            _Size--;
            return val;
        }

        void Push(const T& val)
        {
            if (Full())
                _Deviation > (_Size / 2) ? _Move() : _ReAlloc();

            _Head[_Deviation + _Size++] = val;
        }
    };
}

namespace list
{
    template<typename T>
    class Queue
    {
    private:
        List<T> _List;

    public:
        Queue() : _List()
        {
        }

        std::size_t size() const { return _List.Size(); }
        bool empty() const { return size() == 0; }

        T front() const { return _List.Head(); }
        T back()  const { return _List.Tail(); }

        void enqueue(const T& val) { _List.AddAtEnd(val); }
        T    dequeue() { return _List.DeleteFront(); }
    };
}