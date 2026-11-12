#pragma once

#include <cstddef>
#include <string>

template<typename T>
class TableObject
{
private:
    T      _Data;
    std::string _Key;

public:
    TableObject() : _Data(), _Key(NullKey()) {}

    TableObject(const T& Data, const std::string& Key)
        : _Data(Data), _Key(Key) {
    }

    void setData(const T& Data) { _Data = Data; }
    T    Data() const { return _Data; }

    void setKey(const std::string& Key) { _Key = Key; }
    std::string Key() const { return _Key; }

    static const std::string& NullKey()
    {
        static const std::string Null = "<NULL>";
        return Null;
    }
};

template<typename T>
class Table
{
private:
    using Record = TableObject<T>;

    Record* _Head;
    size_t  _Size;
    size_t  _Capacity;

    size_t _Hash(const std::string& Key)
    {
        size_t index = 0;
        constexpr size_t PrimeNumber = 31;

        for (unsigned char c : Key)
            index = index * PrimeNumber + c;

        return index % _Capacity;
    }

    size_t _Prob(const std::string& Key, size_t CurrentIndex)
    {
        size_t OriginalIndex = CurrentIndex;

        while (_Head[CurrentIndex].Key() != Record::NullKey())
        {
            if (_Head[CurrentIndex].Key() == Key)
                return CurrentIndex;

            CurrentIndex = (CurrentIndex + 1) % _Capacity;

            if (CurrentIndex == OriginalIndex)
                return OriginalIndex;
        }
        return CurrentIndex;
    }

    void _ReHash(const Record* OldHead, size_t OldCapacity)
    {
        _Size = 0;
        for (size_t i = 0; i < OldCapacity; i++)
            if (OldHead[i].Key() != Record::NullKey())
                _Insert(OldHead[i]);
    }

    void _Insert(const Record& R)
    {
        size_t Index = _Hash(R.Key());

        if (_Head[Index].Key() == Record::NullKey())
        {
            _Head[Index] = R;
            _Size++;
            return;
        }
        if (_Head[Index].Key() == R.Key())
        {
            _Head[Index].setData(R.Data());
            return;
        }

        size_t FoundIndex = _Prob(R.Key(), Index);

        if (_Head[FoundIndex].Key() == R.Key())
        {
            _Head[FoundIndex].setData(R.Data());
            return;
        }
        if (_Head[FoundIndex].Key() == Record::NullKey())
        {
            _Head[FoundIndex] = R;
            _Size++;
        }
    }

    void _Grow()
    {
        Record* OldHead = _Head;
        size_t  OldCapacity = _Capacity;

        _Capacity <<= 1;
        _Head = new Record[_Capacity];

        _ReHash(OldHead, OldCapacity);
        delete[] OldHead;
    }

    void _Shrink()
    {
        Record* OldHead = _Head;
        size_t  OldCapacity = _Capacity;

        _Capacity >>= 1;
        _Head = new Record[_Capacity];

        _ReHash(OldHead, OldCapacity);
        delete[] OldHead;
    }

public:
    Table() : _Head(new Record[2]), _Size(0), _Capacity(2) {}

    ~Table()
    {
        delete[] _Head;
    }

    bool Exists(const std::string& Key)
    {
        size_t Index = _Hash(Key);
        if (_Head[Index].Key() == Key) return true;

        size_t FoundIndex = _Prob(Key, Index);
        if (FoundIndex == Index) return false;
        if (_Head[FoundIndex].Key() == Record::NullKey()) return false;

        return true;
    }

    void Add(const std::string& Key, const T& Data)
    {
        if (_Size == _Capacity) _Grow();
        _Insert(Record(Data, Key));
    }

    T Find(const std::string& Key)
    {
        size_t Index = _Hash(Key);
        if (_Head[Index].Key() == Key) return _Head[Index].Data();

        size_t FoundIndex = _Prob(Key, Index);
        if (FoundIndex == Index) return T();
        if (_Head[FoundIndex].Key() == Record::NullKey()) return T();

        return _Head[FoundIndex].Data();
    }

    bool Delete(const std::string& Key)
    {
        if (Key == Record::NullKey()) return false;

        if (_Size < _Capacity / 4 && _Capacity > 2) _Shrink();

        size_t Found = _Prob(Key, _Hash(Key));
        if (_Head[Found].Key() != Key) return false;

        _Head[Found] = Record();
        _Size--;

        size_t Index = (Found + 1) % _Capacity;
        while (_Head[Index].Key() != Record::NullKey())
        {
            Record Moved = _Head[Index];
            _Head[Index] = Record();
            _Size--;
            _Insert(Moved);
            Index = (Index + 1) % _Capacity;
        }
        return true;
    }
};