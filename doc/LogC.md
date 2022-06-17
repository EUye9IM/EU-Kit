# LogC

## 简介

一个 C 风格日志输出工具

## 使用方式

类名为 `LogC`，但一般使用自带的全局变量 `logc` 足矣，除非想要同时写多个日志文件。

## 成员函数

### 输出重定向

全局变量 `logc` 默认向标准输出。

#### 构造函数

默认构造函数一般不指定输出位置，需要手动指定向文件或向文件指针。

```cpp
LogC();
LogC(const char *file_path);
LogC(FILE *file);
```

#### open/use

当你需要重定向，需要使用这两个函数，分别是向文件和文件指针。

```cpp
int open(const char *file_path);
int use(FILE *file);
```

如果上一次是指定的输出到文件，会关闭文件。如果是指定的文件指针则不会关闭文件指针。

### 关闭文件

一般不太会用得到，但是有这功能。

```cpp
int close();
```

同样的，之前指定的文件指针则不会关闭文件指针。

### 属性

这四个函数可以调整属性

```cpp
unsigned int addAttr(unsigned int attribute);
unsigned int removeAttr(unsigned int attribute);
unsigned int setAttr(unsigned int attribute);
unsigned int getAttr();
```

属性有以下几种

```cpp
static const unsigned int A_TIME = 1 << 0;
static const unsigned int A_UTC = 1 << 1;
static const unsigned int A_DATE = 1 << 2;
static const unsigned int A_DEBUG = 1 << 3;

static const unsigned int DEFAULT_ATTR = A_DATE | A_TIME;
```

- `A_TIME` 输出带时间
- `A_UTC` 输出如果带时间，则为UTC时间，否则是当地时间
- `A_DATE` 输出带日期
- `A_DEBUG` 输出调式信息，即 `LogC.debugf(const char *fmt, ...)` 有效

默认为 `DEFAULT_ATTR`，即以当地时间输出日期和时间，不输出调试信息。

### 输出

```cpp
int printf(const char *fmt, ...);
int debugf(const char *fmt, ...);
int fatalf(const char *fmt, ...);
```

像使用 `printf` 那样使用即可（记得换行）。

`debugf()` 只有在指定了 `A_DEBUG` 属性时才有效。

`fatalf()`  调用结束后会调用 `exit(0);` 结束程序。

## 示例

[main.cpp](../example/logc/main.cpp)