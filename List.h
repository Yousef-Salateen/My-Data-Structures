#pragma once

#include <cstddef>
#include <utility>

template <typename T>
class List
{
public:
    struct Node
    {
        T data;
        Node* next;
        Node* prev;

        Node() : data(), next(nullptr), prev(nullptr) {}
        Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
        Node(const T& val, Node* prevNode, Node* nextNode)
            : data(val), next(nextNode), prev(prevNode) {
        }
    };

private:
    Node* _Head;
    Node* _Tail;
    size_t _Size;

    void _InsertBetween(Node* prev, Node* next, const T& val)
    {
        Node* newNode = new Node(val, prev, next);
        prev->next = newNode;
        next->prev = newNode;
        _Size++;
    }

    Node* _Find(Node* node) const
    {
        Node* current = _Head->next;
        while (current != _Tail)
        {
            if (current == node) return current;
            current = current->next;
        }
        return nullptr;
    }

    void _LinkNodes(Node* prev, Node* next)
    {
        prev->next = next;
        next->prev = prev;
    }

    void _Remove(Node* node)
    {
        if (!IsEmpty())
        {
            _LinkNodes(node->prev, node->next);
            delete node;
            _Size--;
        }
    }

    void _Remove(const T& val)
    {
        Node* current = Find(val);
        if (current) _Remove(current);
    }

    Node* _Forward(Node* node, size_t steps)
    {
        while (steps-- && node->next != _Tail)
            node = node->next;
        return node;
    }

    Node* _Backward(Node* node, size_t steps)
    {
        while (steps-- && node->prev != _Head)
            node = node->prev;
        return node;
    }

    List(Node* first, Node* last, size_t size)
        : _Head(new Node()), _Tail(new Node()), _Size(size)
    {
        _LinkNodes(_Head, first);
        _LinkNodes(last, _Tail);
    }

public:
    List() : _Head(new Node()), _Tail(new Node()), _Size(0)
    {
        _LinkNodes(_Head, _Tail);
    }

    ~List()
    {
        Clear();
        delete _Head;
        delete _Tail;
    }

    const T& Head() const { return _Head->next->data; }
    const T& Tail() const { return _Tail->prev->data; }

    size_t Size()    const { return _Size; }
    bool   IsEmpty() const { return !_Size; }

    void Clear()
    {
        Node* current = _Head->next;
        while (current != _Tail)
        {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        _Size = 0;
        _LinkNodes(_Head, _Tail);
    }

    void AddAtFront(const T& val) { _InsertBetween(_Head, _Head->next, val); }
    void AddAtEnd(const T& val) { _InsertBetween(_Tail->prev, _Tail, val); }

    void InsertAfter(Node* node, const T& val)
    {
        if (node) _InsertBetween(node, node->next, val);
    }

    void InsertBefore(Node* node, const T& val)
    {
        if (node) _InsertBetween(node->prev, node, val);
    }

    void InsertAfter(size_t index, const T& val)
    {
        Node* node = at(index);
        if (node) _InsertBetween(node, node->next, val);
    }

    bool NodeExists(Node* node) const
    {
        return node && (node == _Find(node));
    }

    bool NodeExists(const T& val) const
    {
        return Find(val) != nullptr;
    }

    Node* Find(const T& val) const
    {
        Node* current = _Head->next;
        while (current != _Tail)
        {
            if (current->data == val) return current;
            current = current->next;
        }
        return nullptr;
    }

    T DeleteFront()
    {
        T val = _Head->next->data;
        _Remove(_Head->next);
        return val;
    }

    T DeleteEnd()
    {
        T val = _Tail->prev->data;
        _Remove(_Tail->prev);
        return val;
    }

    void Delete(const T& val) { _Remove(val); }
    void Delete(Node* node) { if (node) _Remove(node); }

    void MoveForward(Node*& node, size_t steps) { if (node) node = _Forward(node, steps); }
    void MoveBackward(Node*& node, size_t steps) { if (node) node = _Backward(node, steps); }

    Node* at(size_t index)
    {
        if (index >= _Size) return nullptr;
        if (index < (_Size / 2))
            return _Forward(_Head->next, index);
        else
        {
            index = _Size - index - 1;
            return _Backward(_Tail->prev, index);
        }
    }

    T& operator[](size_t index) { return at(index)->data; }

    void Rotate(int steps)
    {
        const long long size = static_cast<long long>(_Size);
        if (size == 0) return;

        steps %= static_cast<int>(size);
        if (steps == 0) return;

        _LinkNodes(_Tail->prev, _Head->next);

        _Head->next = steps > 0 ? at(size - steps) : at(-steps);

        _Tail->prev = _Head->next->prev;
        _Head->next->prev = _Head;
        _Tail->prev->next = _Tail;
    }

    void Merge(List<T>& other)
    {
        if (&other == this || other.IsEmpty()) return;

        _LinkNodes(_Tail->prev, other._Head->next);
        _LinkNodes(other._Tail->prev, _Tail);
        _Size += other._Size;

        other._Size = 0;
        other._LinkNodes(other._Head, other._Tail);
    }

    List<T> Split(size_t index)
    {
        if (index >= _Size) return List();

        Node* first = at(index);
        Node* last = _Tail->prev;
        size_t newSize = _Size - index;

        _LinkNodes(first->prev, _Tail);
        _Size = index;

        return List(first, last, newSize);
    }

    void Reverse()
    {
        Node* current = _Head;
        while (current)
        {
            std::swap(current->next, current->prev);
            current = current->prev;
        }
        std::swap(_Head, _Tail);
    }

    void SwapNodes(Node* a, Node* b)
    {
        if (!a || !b || a == b) return;
        if (b->next == a) std::swap(a, b);

        Node* aPrev = a->prev;
        Node* aNext = a->next;
        Node* bPrev = b->prev;
        Node* bNext = b->next;

        if (aNext == b)
        {
            _LinkNodes(aPrev, b);
            _LinkNodes(b, a);
            _LinkNodes(a, bNext);
        }
        else
        {
            _LinkNodes(aPrev, b);
            _LinkNodes(b, aNext);
            _LinkNodes(bPrev, a);
            _LinkNodes(a, bNext);
        }
    }

    void RemoveDuplicates()
    {
        Node* current = _Head->next;
        while (current != _Tail)
        {
            Node* check = current->next;
            while (check != _Tail)
            {
                Node* nextNode = check->next;
                if (current->data == check->data)
                    Delete(check);
                check = nextNode;
            }
            current = current->next;
        }
    }
};