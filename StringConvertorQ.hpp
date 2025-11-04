#ifndef STRINGCONVERTORQ_HPP
#define STRINGCONVERTORQ_HPP

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <list>
#include <deque>
#include <forward_list>

#include <QList>
#include <QVector>
#include <QStringList>
#include <QMetaEnum>
#include <QRegularExpression>
#include "MetaEnum.hpp"

namespace MetaUtility {

    ///数组分隔符
    static const QString spliter("[SP]");

    ///数字量转换为string
    template<typename T,typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    inline QString convertArgToString(const T arg)
    {
        return QString::number(arg);
    }

    ///枚举转换为string
    template<typename T>
    typename std::enable_if<EnumMap<T>::registed,QString>::type
    enumToString(T value)
    {
        static EnumMap<T> emap;
        return emap.toString(value);
    }

    template<typename T>
    typename std::enable_if<!EnumMap<T>::registed,QString>::type
    enumToString(T value)
    {
        return QString::number(value);
    }

    template<typename T,typename std::enable_if<std::is_enum<T>::value,T>::type* = nullptr>
    inline QString convertArgToString(const T arg)
    {
        return enumToString(arg);
    }

    ///string转换为string
    inline QString convertArgToString(const QString& arg)
    {
        return arg;
    }

    ///char*转换为string
    inline QString convertArgToString(const char* arg)
    {
        return QString(arg);
    }

    ///class object转换为string
    template<typename T,typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
    inline QString convertArgToString(const T& obj)
    {
        std::stringstream ss;
        ss << obj;
        return QString::fromStdString(ss.str());
    }

    ///容器转换为字符串
    template<typename T,template<typename...Element> class Array,typename...Args>
    inline QString convertArgToString(const Array<T,Args...>& array)
    {
        QString str;
        for(const T& item : array)
        {
            str.append( convertArgToString(item) );
            str.append(spliter);
        }
        return str;
    }

    ///数组转换为字符串
    template<size_t N,typename T>
    inline QString convertArgToString(const T(&array)[N])
    {
        QString str;
        for(const T& item : array)
        {
            str.append( convertArgToString(item) );
            str.append(spliter);
        }
        return str;
    }

    ///字符串转换为整数
    template <typename T,typename std::enable_if<std::is_integral<T>::value,T>::type* = nullptr>
    inline void convertStringToArg(const QString& str,T& arg)
    {
        arg = static_cast<T>(str.toLongLong());
    }

    ///字符串转枚举
    template<typename T>
    typename std::enable_if<EnumMap<T>::registed,T>::type
    stringToEnum(const QString& str,T& arg)
    {
        static EnumMap<T> emap;
        arg = emap.toEnum(str);
        return arg;
    }

    template<typename T>
    typename std::enable_if<!EnumMap<T>::registed,bool>::type
    stringToEnum(const QString& str,T& arg)
    {
        arg = static_cast<T>(str.toInt());
        return true;
    }

    template<typename T,typename std::enable_if<std::is_enum<T>::value,T>::type* = nullptr >
    inline bool convertStringToArg(const QString& str,  T& arg)
    {
        return stringToEnum(str,arg);
    }

    ///字符串转换为浮点型
    template<typename T,typename std::enable_if<std::is_floating_point<T>::value,T>::type* = nullptr>
    inline void convertStringToArg(const QString& str,T& arg)
    {
        arg = static_cast<T>(str.toDouble());
    }

    inline bool convertStringToArg(const QString& str, QString& arg)
    {
        arg = str;
        return true;
    }

    ///字符串转换为char*
    inline void convertStringToArg(const QString& str,char* arg)
    {
        char* data = new char[str.length()];
        memcpy(data,str.toStdString().data(),str.length());

        arg  = data;
    }

    ///字符串转换为class object pointer
    template<typename T,typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
    inline void convertStringToArg(const QString& str,T& obj)
    {
        std::stringstream in;
        in << str.toStdString();
        in >> obj;
    }

    template<template<typename...> class Array,typename T,typename...Args>
    constexpr static bool IsSequenceContainer =
        std::is_same<Array<T,Args...>, std::list<T,Args...>>::value ||
        std::is_same<Array<T,Args...>,std::vector<T,Args...>>::value ||
        std::is_same<Array<T,Args...>,std::deque<T,Args...>>::value ||
        std::is_same<Array<T>,QList<T>>::value ||
        std::is_same<Array<T>,QVector<T>>::value;

    ///字符串转换为容器
    template<typename T,typename...Args,template<typename...> class Array,
    typename std::enable_if<IsSequenceContainer<Array,T,Args...>,Array<T,Args...>*>::type* = nullptr>
    inline void convertStringToArg(const QString& str, Array<T,Args...>& array)
    {
        QStringList stringList = str.split(spliter,Qt::SkipEmptyParts);

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
    inline void  convertStringToArg(const QString& str, std::array<T,N>& array)
    {
        QStringList stringList = str.split(spliter,Qt::SkipEmptyParts);

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
    inline void convertStringToArg(const QString& str, T(&array)[N])
    {
        QStringList stringList = str.split(spliter,Qt::SkipEmptyParts);
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

#endif // STRINGCONVERTORQ_HPP
