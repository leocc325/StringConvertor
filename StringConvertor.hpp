#ifndef STRINGCONVERTOR_H
#define STRINGCONVERTOR_H

#include <string>
#include <array>
#include <vector>
#include <list>
#include <deque>
#include <forward_list>
#include <sstream>
#include <type_traits>

namespace MetaUtility {

    inline std::list<std::string> splitArray(const std::string& input)
    {
        std::list<std::string> tokens;
        std::string::const_iterator beg = input.cbegin();
        std::string::const_iterator mid = input.cbegin();
        std::string::const_iterator end = input.cend();
        while (beg < end)
        {
            if(*beg == '(' || *beg == ')' || *beg == ',' || *beg == '@')
                ++beg;
            else//查找字符直到遇到非分隔符或者起始终止符
            {
                mid = beg + 1;//将第二个指针移动到下一个位置,查找下一个分割符或者起始/终止符
                while (mid < end)
                {
                    if(*mid == '(' || *mid == ')' || *mid == ',' || *mid == '@')
                        break;
                    ++mid;
                }

                tokens.push_back(std::string(beg,mid));//查找完成之后将两个指针所代表的字符串拷贝到结果中
                beg = mid;//将第一个指针移动到第二个指针位置
            }
        }
        return tokens;
    }

    ///数字量转换为string
    template<typename T,typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    inline std::string convertArgToString(const T arg)
    {
        return std::to_string(arg);
    }

    ///枚举转换为string
    template<typename T,typename std::enable_if<std::is_enum<T>::value,T>::type* = nullptr>
    inline std::string convertArgToString(const T arg)
    {
        return std::to_string(arg);
    }

    ///string转换为string
    inline std::string convertArgToString(const std::string& arg)
    {
        return arg;
    }

    ///char*转换为string
    inline std::string convertArgToString(const char* arg)
    {
        return std::string(arg);
    }

    ///class object转换为string,需要class支持<<重载
    template<typename T,typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
    inline std::string convertArgToString(const T& obj)
    {
        std::stringstream ss;
        ss << obj;
        return ss.str();
    }

    ///容器转换为字符串
    template<typename T,template<typename...Element> class Array,typename...Args>
    inline std::string convertArgToString(const Array<T,Args...>& array)
    {
        std::string str = "(@";
        for(T item : array)
        {
            str.append( convertArgToString(item) );
            str.append(",");
        }
        str.append(")");
        return str;
    }

    ///数组转换为字符串
    template<size_t N,typename T>
    inline std::string convertArgToString(const T(&array)[N])
    {
        std::string str = "(@";
        for(T item : array)
        {
            str.append( convertArgToString(item) );
            str.append(",");
        }
        str.append(")");
        return str;
    }

    ///字符串转换为整数
    template <typename T,typename std::enable_if<std::is_integral<T>::value,int>::type* = nullptr>
    inline void convertStringToArg(const std::string& str,T& arg)
    {
        arg = static_cast<T>(std::stoll(str));
    }

    template<typename T,typename std::enable_if<std::is_enum<T>::value,T>::type* = nullptr >
    inline bool convertStringToArg(const std::string& str,  T& arg)
    {
        arg = static_cast<T>(std::stoll(str));
        return true;
    }

    ///字符串转换为浮点型
    template<typename T,typename std::enable_if<std::is_floating_point<T>::value,T>::type* = nullptr>
    inline void convertStringToArg(const std::string& str,T& arg)
    {
        arg = static_cast<T>(std::stold(str));
    }

    ///字符串转换为char*
    inline void convertStringToArg(const std::string& str,char* arg)
    {
        char* data = new char[str.length()];
        memcpy(data,str.data(),str.length());

        arg  = data;
    }

    ///字符串转换为class,需要class支持>>重载
    template<typename T,typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
    inline void convertStringToArg(const std::string& str,T& obj)
    {
        std::stringstream in;
        in << str;
        in >> obj;
    }

    template<template<typename...> class Array,typename T,typename...Args>
    constexpr static bool IsSequenceContainer =
        std::is_same<Array<T,Args...>, std::list<T,Args...>>::value ||
        std::is_same<Array<T,Args...>,std::vector<T,Args...>>::value ||
        std::is_same<Array<T,Args...>,std::deque<T,Args...>>::value;

    ///字符串转换为容器
    template<typename T,typename...Args,template<typename...> class Array,
    typename std::enable_if<IsSequenceContainer<Array,T,Args...>,Array<T,Args...>*>::type* = nullptr>
    inline void convertStringToArg(const std::string& str, Array<T,Args...>& array)
    {
        std::list<std::string> stringList = splitArray(str);

        Array<T,Args...> tempArray;
        for(auto& item : stringList)
        {
            T value;
            convertStringToArg(item,value);
            tempArray.push_back(value);
        }
        array = std::move(tempArray);
    }

    ///字符串转换为array
    template<typename T,size_t N>
    inline void  convertStringToArg(const std::string& str, std::array<T,N>& array)
    {
        std::list<std::string> stringList = splitArray(str);

        int index = 0;
        T value;
        for(auto& item : stringList)
        {
            if(index < N)
            {
                convertStringToArg(item,value);
                array[index] = value;
                index++;
            }
        }
    }

    ///字符串转换为数组
    template<typename T,std::size_t N>
    inline void convertStringToArg(const std::string& str, T(&array)[N])
    {
        std::list<std::string> stringList = splitArray(str);
        ///读取到的参数数量和当前容器的大小不匹配的时候不进行参数读取工作
        if ( stringList.size() != N )
            return;

        int index = 0;
        T value;
        for(auto& item : stringList)
        {
            convertStringToArg(item,value);
            array[index] = value;
            index++;
        }
    }
}

#endif // STRINGCONVERTOR_H
