# args

## 简介

一个设定命令行参数工具。

arg 为 argument 的缩写；s表示复数，也有 simple/set 的意思。

即：这是一个简单的用于设定多的参数的工具。

## 缺陷

1. 短参数名不能连用(比如 ps -aux)
2. 不考虑线程安全
3. 便宜英文错误描述（有能力改的话可以去改include/args/text.h）
4. 没有测很多次，大概有 bug 吧，发现了务必告诉我，我什么都不会做的

## 功能

1. 支持短参数名或长参数名
2. 支持多种参数类型(标志，整数，小数，字符串)
3. 参数类型支持一个参数名后接零至多个参数(多参数值短期不实现，因为没有类型)
4. 支持类型检查

## 使用方式

ArgParser 建立解析器

使用解析器的 add 方法添加参数

参数使用 newArg 方法建立，后跟 -> 算符添加更多设定，具体有：

1. type() 指定类型
2. check<...>() 指定参数检查函数
3. info() 设定提示信息
4. defaultValue<...>() 设定默认值，不指定则是必选参数

> 可能还会注意到有 share_from_this 和 weak_from_this, 是为了实现智能指针的链式调用的副产物，我不知道怎么去除，知道的话务必告诉我。

目前支持 Flag、Int、Dec、Str 四种类型
（bool,int,double,std::string）

ArgParser.parse() 分析参数，成功 0 失败 -1

ArgParser.val<...>() 取值

ArgParser.errMsg()和.helpInfo() 分别是错误信息和帮助信息

## 示例

总之有什么问题就看示例吧

[main.cpp](../example/args/main.cpp)