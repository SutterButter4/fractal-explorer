#pragma once
#include <array>
#include <string>
#include <stdlib.h>
#include <limits.h>
#include <ostream>

#include "globals.h"

template<ulong LENGTH>
class PercisionUInt
{
public:
    PercisionUInt(const std::string& str);
    PercisionUInt(const unsigned long& i);

    static PercisionUInt<LENGTH> add(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b);
    static PercisionUInt<LENGTH> multiply(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b);
    static PercisionUInt<LENGTH> pow(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b);
    static PercisionUInt<LENGTH> intDivision(const PercisionUInt<LENGTH>& a, const ulong& m);
    static PercisionUInt<LENGTH> mod(const PercisionUInt<LENGTH>& a, const ulong& m);
    static PercisionUInt<LENGTH> bitshiftLeft(const PercisionUInt<LENGTH>& a, const ulong& b);
    static PercisionUInt<LENGTH> bitshiftRight(const PercisionUInt<LENGTH>& a, const ulong& b);
    static PercisionUInt<LENGTH> bitAnd(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b);
    static PercisionUInt<LENGTH> bitOr(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b);
    static PercisionUInt<LENGTH> bitXOr(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b);

    std::string toHex();

    bool operator<(const PercisionUInt<LENGTH>& a);
    bool operator>(const PercisionUInt<LENGTH>& a);
    bool operator==(const PercisionUInt<LENGTH>& a);
private:
    // we divide by 4 because it would be the number of digits base 8, which is an upper limit ceiling for 10 aswell
    static constexpr unsigned long MAX_10_EXPONENT = LENGTH * sizeof(unsigned long) / 3;

    PercisionUInt(const std::array<unsigned long, LENGTH>& data);
    std::array<unsigned long, LENGTH> _data = {};
};

template<ulong LENGTH>
PercisionUInt<LENGTH>::PercisionUInt(const std::string& str)
{
    PercisionUInt<LENGTH> sum(0);
    //compute the sum of 10^n*d_n for all d in the string
    for(unsigned long i = 0; i < str.length(); i++)
    {
        std::cout<< atoi(str.substr(str.length() - (i + 1), 1).c_str()) << std::endl;
        
        sum = PercisionUInt<LENGTH>::add(sum,
            PercisionUInt<LENGTH>::multiply(PercisionUInt<LENGTH>(atoi(str.substr(str.length() - (i + 1), 1).c_str())),
                pow(PercisionUInt<LENGTH>(10), PercisionUInt<LENGTH>(i))));
    }

    this->_data = sum._data;
}

template<ulong LENGTH>
PercisionUInt<LENGTH>::PercisionUInt(const unsigned long& i)
{
    _data[LENGTH - 1] = i;
}

template<ulong LENGTH>
PercisionUInt<LENGTH>::PercisionUInt(const std::array<unsigned long, LENGTH>& data)
    :_data(data)
{}


template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::add(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b)
{
    std::array<unsigned long, LENGTH> newData {};

    for(unsigned long i = 0; i <LENGTH ; i++ )
    {
        newData[i] += a._data[i] + b._data[i];
        if((newData[i] < a._data[i] || newData[i] < b._data[i]) && (i != 0))
            newData[i - 1] += 1;
    }

    return PercisionUInt<LENGTH>(newData);
}

__uint128_t i = 0;

std::array<ulong, 2> multLong(unsigned long a, unsigned long b)
{
    //split a into a1a0 where a1 is the left 32 bits and a0 is the right 32, same with b.
    ulong a1 = a & ((1UL << (sizeof(ulong) * BITS_PER_BYTE / 2)) - 1);        //00000000001111111111 = (1L >> (sizeof(ulong)/4 - 1))
    ulong a0 = a >> (sizeof(ulong) * BITS_PER_BYTE / 2);                      //11111111110000000000 = ULONG_MAX - (1L >> (sizeof(ulong)/4 - 1))
    ulong b1 = b & ((1UL << (sizeof(ulong) * BITS_PER_BYTE / 2)) - 1);        //00000000001111111111
    ulong b0 = b >> (sizeof(ulong) * BITS_PER_BYTE / 2);                      //11111111110000000000

    //now compute the products
    ulong a0b0 = a0*b0; // 64 bit offset
    ulong a0b1 = a0*b1; // 32 bit offset
    ulong a1b0 = a1*b0; // 32 bit offset
    ulong a1b1 = a1*b1; // no offset

    ulong _64offset = a0b0;
    ulong _32offset = a0b1 + a1b0; // we have to check for overflow on this addition...
    ulong _00offset = a1b1;

    if(_32offset < a1b0 || _32offset < a1b0)
        _64offset += (1 << sizeof(ulong) * BITS_PER_BYTE / 2);

    _64offset += _32offset >> (sizeof(ulong) * BITS_PER_BYTE / 2);
    _00offset += _32offset << (sizeof(ulong) * BITS_PER_BYTE / 2); //we also have to check for overflow on this addition

    if(_00offset - (_32offset >> (sizeof(ulong) * BITS_PER_BYTE / 2)) > _00offset)
        _64offset += 1;

    return std::array<ulong, 2> {_64offset, _00offset};
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::multiply(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b)
{
    //TODO: IMPLEMENT UNIT TESTING!!!!!!!!!!!!!!!!!!!!
    PercisionUInt<LENGTH> sum(0);

    for(ulong i = LENGTH - 1; i <= LENGTH; i--)
    {
        for(ulong j =  0; j <= i; j++)
        {
            std::array<ulong, LENGTH> newData = {};
            auto arr = multLong(a._data[i + j], b._data[LENGTH - (j + 1)]);
            if(i != 0) newData[i - 1] = arr[0];
            newData[i] = arr[1];
            sum = PercisionUInt<LENGTH>::add(sum, PercisionUInt<LENGTH>(newData));
        }
    }

    return sum;
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::pow(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b)
{
    PercisionUInt<LENGTH> product(1);
    for(PercisionUInt<LENGTH> i(0); i < b; i = PercisionUInt<LENGTH>::add(i, 1))
    {
        product = PercisionUInt<LENGTH>::multiply(product, a);
    }

    return product;
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::intDivision(const PercisionUInt<LENGTH>& a, const ulong& m)
{
    return PercisionUInt<LENGTH>(0);
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::mod(const PercisionUInt<LENGTH>& a, const ulong& m)
{
    PercisionUInt<LENGTH> sum = 0;
    ulong wordMod = ((ULONG_MAX % m) + 1) % m; //ULONG_MAX + 1 = s^sizeof(ulong)
    ulong currentWordMod = 1;
    
    for(ulong i = 0; i < LENGTH; i++)
    {
        sum += PercisionUInt<LENGTH>(currentWordMod) * PercisionUInt<LENGTH>(a._data[i] % m);
        std::array<ulong, 2> newWordMod = multLong(wordMod, currentWordMod);

    }
    return PercisionUInt<LENGTH>(0);
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::bitshiftLeft(const PercisionUInt<LENGTH>& a, const ulong& b)
{
    if(b==0)
        return a;
    else if(b > (sizeof(ulong) * BITS_PER_BYTE)) // we perform a full ulong length shift
    {
        ulong numShifts = b / (sizeof(ulong) * BITS_PER_BYTE);

        PercisionUInt<LENGTH> restOfShifts = bitshiftLeft(a, b - numShifts * (sizeof(ulong) * BITS_PER_BYTE));
        std::array<ulong, LENGTH> newData {};
        for(ulong i = 0; i < LENGTH - numShifts; i++)
        {
            newData[i + numShifts] = restOfShifts._data[i];
        }

        return PercisionUInt<LENGTH>(newData);
    }
    else //shift in range of [0,sizeof(ulong)) bits.
    {
        std::array<ulong, LENGTH> newData {};
        
        newData[i] = (a._data[0] << b);
        for(ulong i = 1; i < LENGTH; i++)
        {
            newData[i] = (a._data[i] << b) + (a._data[i - 1] >> ((sizeof(ulong) * BITS_PER_BYTE) - b));
        }

        return PercisionUInt<LENGTH>(newData);
    }
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::bitshiftRight(const PercisionUInt<LENGTH>& a, const ulong& b)
{
    if(b==0)
        return a;
    else if(b > (sizeof(ulong) * BITS_PER_BYTE)) // we perform a full ulong length shift
    {
        ulong numShifts = b / (sizeof(ulong) * BITS_PER_BYTE);

        PercisionUInt<LENGTH> restOfShifts = bitshiftLeft(a, b - numShifts * (sizeof(ulong) * BITS_PER_BYTE));
        std::array<ulong, LENGTH> newData {};
        for(ulong i = 0; i < LENGTH - numShifts; i++)
        {
            newData[i] = restOfShifts._data[i + numShifts];
        }

        return PercisionUInt<LENGTH>(newData);
    }
    else //shift in range of [0,sizeof(ulong)) bits.
    {
        std::array<ulong, LENGTH> newData {};
        
        newData[i] = (a._data[0] << b);
        for(ulong i = 1; i < LENGTH; i++)
        {
            newData[i] = (a._data[i] >> b) + (a._data[i + 1] << ((sizeof(ulong) * BITS_PER_BYTE) - b));
        }

        return PercisionUInt<LENGTH>(newData);
    }
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::bitAnd(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b)
{
    PercisionUInt<LENGTH> result(0);
    for(ulong i = 0; i < LENGTH; i++)
    {
        result._data[i] = a._data[i] & b._data[i];
    }
    return result;
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::bitOr(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b)
{
    PercisionUInt<LENGTH> result(0);
    for(ulong i = 0; i < LENGTH; i++)
    {
        result._data[i] = a._data[i] | b._data[i];
    }
    return result;
}

template<ulong LENGTH>
PercisionUInt<LENGTH> PercisionUInt<LENGTH>::bitXOr(const PercisionUInt<LENGTH>& a, const PercisionUInt<LENGTH>& b)
{
    PercisionUInt<LENGTH> result(0);
    for(ulong i = 0; i < LENGTH; i++)
    {
        result._data[i] = a._data[i] ^ b._data[i];
    }
    return result;
}

template<ulong LENGTH>
std::string PercisionUInt<LENGTH>::toHex()
{
    std::ostream ostream;
    ostream << "0x";
    ostream.width(sizeof(ulong) / 2);
    ostream.fill('0');
    for(auto d : _data)
        ostream << std::hex << d;
    return ostream.str();
}

template<ulong LENGTH>
bool PercisionUInt<LENGTH>::operator<(const PercisionUInt<LENGTH>& a)
{
    for(ulong i = 0; i < LENGTH; i++)
        if(_data[i] < a._data[i]) return true;
    return false;
}

template<ulong LENGTH>
bool PercisionUInt<LENGTH>::operator>(const PercisionUInt<LENGTH>& a)
{
    for(ulong i = 0; i < LENGTH; i++)
        if(_data[i] > a._data[i]) return true;
    return false;
}

template<ulong LENGTH>
bool PercisionUInt<LENGTH>::operator==(const PercisionUInt<LENGTH>& a)
{
    for(ulong i = 0; i < LENGTH; i++)
        if(_data[i] != a._data[i]) return false;
    return true;
}