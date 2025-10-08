## StringConvertor && StringConvertorQ
该模板用于将常用的变量和容器转换为字符串,或者将字符串转换为相应变量。 <br />
This template is used to convert common variables and containers into strings, or convert strings into corresponding variables. <br />

StringConvertor用于变量和std::string之间的转换,StringConvertorQ用于变量和QString之间的转换。<br />
StringConvertor is used for conversion between variables and std::string, while StringConvertorQ is used for conversion between variables and QString.<br />

该模板文件详细功能如下: <br />
1.将数值类型变量、枚举、bool、char*、重载了>>和<<运算符的对象以及上述所有类型的容器(链表、向量等)和数组转换为字符串 。<br />
2.将字符串转换为相应的数值类型变量、枚举、bool、char*、重载了>>和<<运算符的对象以及上述所有类型的容器(链表、向量等)和数组。<br />
3.文件中包含一个测试函数Test_StringConvertor用于测试字符串和变量之间的转换是否成功。 <br />
The detailed functionality of this template file is as follows: <br />
1.Convert numeric variables, enums, bool, char*, objects with overloaded >> and << operators, and containers (lists, vectors, etc.) and arrays of all the above types into strings.<br />
2.Convert strings into the corresponding numeric variables, enums, bool, char*, objects with overloaded >> and << operators, and containers (lists, vectors, etc.) and arrays of all the above types.<br />
3.The file contains a test function Test_StringConvertor() to verify whether the conversion between strings and variables is successful. <br />
