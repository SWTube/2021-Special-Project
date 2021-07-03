# `cave::String`
* defined in header `"String/String.h"`

The class `cave::String` provides support for strings in the engine. `cave::String` is used to store characters dynamically as `std::string`.

|Type |Definition  |
|-----|------------|
|`cave::String`|`cave::String`|

# Member functions

|Name|Description|
|---|---|
|[(constructor)](#user-content-constructor)|constructs a `cave::String`<br>(public member function)|
|(destructor)|destroys the string, deallocating internal storage if used<br>(public member function)|
|[`operator=`](#user-content-operator)|assigns values to the string<br>(public member function)|

* Element access

|Name|Description|
|---|---|
|[`GetCString`](#user-content-getcstring)|returns a non-modifiable standard C character array version of the string<br>(public member function)|

* Capacity

|Name|Description|
|---|---|
|[`GetLength`](#user-content-getlength)|returns the number of characters<br>(public member function)|

* Operations

|Name|Description|
|---|---|
|[`RemoveAt`](#user-content-removeat)|removes characters<br>(public member function)|
|[`Append`](#user-content-append)|appends characters to the end<br>(public member function)|
|[`operator+=`](#user-content-operator)|appends characters to the end<br>(public member function)|
|[`operator+`](#user-content-operator)|concatenates two strings<br>(public member function)|

* Search

|Name|Description|
|---|---|
|[`IndexOf`](#user-content-indexof)|find characters in the string<br>(public member function)|
|[`LastIndexOf`](#user-content-lastindexof)|find the last occurrence of a substring<br>(public member function)|

* Constants

|Name|Description|
|---|---|
|[`NPOS`](#user-content-NPOS)[static]|special value. The exact meaning depends on the context<br>(public static member constant)|

* Non-member functions
	* Input / output

|Name|Description|
|---|---|
|[`operator<<`](#user-content-operator)|performs stream output on strings<br>(function)|

### Constructor
```cpp
String();						// (1)
String(const char* s);			// (2)
String(const String& other);	// (3)
```

Constructs new string from a variety of data sources
1. Default constructor. Constructs empty string (zero size and capacity). If no allocator is supplied, allocator is obtained from the Core Memory Pool.
2. Constructs the string with the contents initialized with a copy of the null-terminated character string pointer to by `s`. The length of the string is determined by the first null character.
3. Copy constructor. Constructs the string with a copy of the contents of other.

#### Paramters
|Parameters|Description|
|---|---|
|`s`|pointer to an array of characters to use as source to initialize the string with|
|`other`|another string to use as source to initialize the string with|

#### Complexity
1. Constant
2. Linear in length of `s`
3. Linear in size of `other`

#### Example
```cpp
#include "String/String.h"

int main()
{
	{
		cave::String string;
		assert(string.GetLength() == 0ul);
	}

	{
		cave::String string("C-style\0string");
		std::cout << string << '\n';	// "C-style"
	}

	{
		cave::String const other("Hello");
		cave::String string(other);
		std::cout << string << '\n';	// "Hello"
	}

	return 0;
}
```

Output:
```
C-style
Hello
```

### `cave::String::operator=`
```cpp
String& operator=(const String& str);	// (1)
String& operator=(const char* s);		// (2)
```

Replaces the contents of the string
1. Replaces the contents with a copy of `str`
2. Replaces the contents with those of null-terminated character string pointed to by `s`

#### Parameters

|Parameters|Description|
|---|---|
|`str`|string to be used as source to initialize the string with|
|`s`|pointer to a null-terminated character string to use as source to initialize the string with|

#### Return value
```cpp
*this
```

#### Complexity
1. Linear in size of `str`
2. Linear in size of `s`

#### Example
```cpp
#include "String/String.h"

int main()
{
	cave::String str1;
	cave::String str2("alpha");

	// (1) operator=(const cave::String&);
	str1 = str2;
	std::cout << str1 << ' '	// "alpha"
			  << str2 << '\n';	// "alpha"

	// (2) operator=(const char*);
	str1 = "beta";
	std::cout << str1 << '\n';	// "beta"

	return 0;
}
```

Possible output:
```
alpha alpha
beta
```

## Element Access
### `cave::String::GetCString`
```cpp
const char* GetCString() const;
```

Returns a pointer to a null-terminated character array with data equivalend to those stored in the string.
Writing to the character array access through `GetCString()` is undefined behavior.

#### Parameters
(none)

#### Return values
Pointer to the underlying character storage.

#### Complexity
Constant

#### Notes
The pointer obtained from `GetCString()` may only be treated as a pointer to a null-terminated character string if the string object does not contain other null characters.

#### Example
```cpp
#include <cstring>

#include "String/String.h"

int main()
{
	cave::String const s("Hello");
	assert(s.GetLength() == std::strlen(s.GetCString()));
	assert('\0' == *(s.GetCString() + s.GetLength()));

	return 0;
}
```

## Iterators


## Capacity
### `cave::String::GetLength()`
```cpp
size_t GetLength() const;
```

* Returns the number of characters in the string

#### Parameters
(none)

#### Return value
The number of characters in the string

#### Complexity
Constant

#### Notes
The elements are bytes (objets of type `char`), which are not the same as characters if a multibye encoding such as UTF-8 is used.

#### Example
* Samples/Main.cpp
```cpp
#include "Debug/Log.h"

int main()
{
    cave::String string("Hello, World!");
	assert(string.GetLength() == 13);

    return 0;
}
```

## Operations
### `cave::String::RemoveAt`
```cpp
bool RemoveAt(size_t index);
```

Removes specified characters from the string.
Removes the character at `index`

#### Parameters
|Parameters|Description|
|---|---|
|`index`|index to the character to remove|

#### Return value
`true` if removal was successful, `false` if otherwise.

#### Example
```cpp
#include "String/String.h"

int main()
{
	cave::String s = "This is an example";
	std::cout << s << '\n';

	s.RemoveAt(0);	// Remove 'T'
	std::cout << s << '\n';

	return 0;
}
```

Output:
```
This is an example
his is an example
```

### `cave::String::Append`
```cpp
void Append(const char* s);
```

Appends additional characters to the string.
Appends the null-terminated character string pointed to by `s`. The length of the string is determined by the first null character using `strlen(s)`.

#### Parameters
|Parameters|Description|
|---|---|
|`s`|pointer to the character string to append|

#### Return value
```cpp
*this
```

#### Complexity
Linear in size of `s`

#### Example
```cpp
#include "String/String.h

int main()
{
	cave::String str = "string";
	const char* cPtr = "C-string";
	const char cArr[] = "Two and one";

	cave::String output;

	// Append a whole C-string
	output.append(cPtr);
	std::cout << output << '\n';

	return 0;
}
```

Output:
```
C-string
```

### `cave::String::operator+=`
```cpp
String& operator+=(const String& str);	// (1)
String& operator+=(const char* s);		// (2)
```

Appends additional characters to the string.
1. Appends string `str`
2. Appends the null-terminated character string pointed to by `s`

#### Parameters
|Parameters|Description|
|---|---|
|`str`|string to append|
|`s`|pointer a null-terminated character string to append|

#### Return value
```cpp
*this
```

#### Complexity
1. Linear in size of `str`
2. Linear in size of `s`

#### Example
```cpp
#include "String/String.h"

int main()
{
	cave::String str;
	std::cout << str << '\n';	// empty string

	str += "This";
   std::cout << str << '\n';
 
   str += cave::String(" is a");
   std::cout << str << '\n';
 
   str += {' ','s','t','r','i','n','g','.'};
   std::cout << str << '\n';
}
```

Output:
```

This
This is a
This is a string.
```

### `cave::String::operator+`
```cpp
String operator+(const String& rhs) const;
```

Returns a string containing characters from itself followed by the characters from `rhs`.

#### Parameters
|Parameters|Description|
|---|---|
|`rhs`|string|

#### Return value
A string containing the characters from itself followed by the characters from `rhs`.

#### Example
```cpp
#include <iostream>

#include "String/String.h"
 
int main()
{
    cave::String s1 = "Hello";
    cave::String s2 = "world";
    std::cout << s1 + ' ' + s2 + "!\n";
}
```

Output:
```
Hello world!
```

### `cave::String::operator==`
```cpp
bool operator==(const String& rhs) const;
```

Compares the contents the string with another string.
* Two strings are equal if both the size of itself and `rhs` are equal and each character of itself has equivalent character in `rhs` at the same position.

Compares itself with another `String` objects

#### Parameters
|Parameters|Description|
|---|---|
|`rhs`|string whose contents to compare|

#### Return value
`true` if the corresponding comparison holds, `false` otherwise.

#### Complexity
Linear in the size of the strings.

## Search
### `cave::String::IndexOf`
```cpp
int32_t IndexOf(const char* s);
```

Finds the first substring equal to the given character sequence.

Finds the first substring equal to the character string pointed to by `s`. The length of the string is determined by the first null character using `strlen(s)`

#### Parameters
|Parameters|Description|
|---|---|
|`s`|pointer to a character string to search for|

#### Return value
Position of the first character of the found substring or [`NPOS`](#user-content-NPOS) if no such substring is found.

#### Example
```cpp
#include <iostream>

#include "String/String.h"

void Print(size_t n, cave::String const &s)
{
    if (n == -1)
	{
        std::cout << "not found\n";
    }
	else
	{
        std::cout << "found" << &s.GetCString()[n] << '\n';
    }
}
 
int main()
{
    size_t n;
    cave::String const s = "This is a string";
 
    // search from beginning of string
    n = s.IndexOf("is");
    Print(n, s);
}
```

Output:
```
found: is is a string
```

### `cave::String::LastIndexOf`
```cpp
int32_t LastIndexOf(const char* s);
```

Finds the last substring equal to the given character sequence.

Finds the last substring equal to the character string pointed to by `s`. The length of the string is determined by the first null character using `strlen(s)`.

#### Parameters
|Parameters|Description|
|---|---|
|`s`|pointer to a character string to search for|

#### Return value
Position of the first character of the found substring or [`NPOS`](#user-content-NPOS) if no such substring is found. Note that this is an offset from the string of the string, not the end.

If searching for an empty string(`str.GetLength()` is zero), [`NPOS`](#user-content-NPOS) is always returned.

#### Example
```cpp
#include <iostream>

#include "String/String.h"

void Print(size_t n, cave::String const &s)
{
    if (n == -1)
	{
        std::cout << "not found\n";
    }
	else
	{
        std::cout << "found: \"" << &s.GetCString()[n] << "\" at " << n << '\n';
    }
}
 
int main()
{
    cave::size_t n;
    cave::String const s = "This is a string";
 
    // search backwards from end of string
    n = s.LastIndexOf("is");
    Print(n, s);
}
```

Output:
```
found: "is a string" at 5
```

## Constants
### `cave::String::NPOS`
```cpp
static constexpr size_t NPOS = -1ul;
```

This is a special value equal to the maximum value representable by the type `size_t`. The exact meaning depends on context, but it is generally used either as end of string indicator by functions that expects a string index or as the error indicator by the functions that return a string index.

#### Note
Although the definition uses `-1`, `size_t` is an unsigned integer type, and the value of `NPOS` is the largest positive value it can hold, due to [signed-to-unsigned implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion#Integral_conversions). This is a portable way to specify the largest value of any unsigned type.

#### Example
```cpp
#include <iostream>

#include "String/String.h"
 
int main()
{
    // string search functions return npos if nothing is found
    cave::String s = "test";
    if(s.IndexOf("a") == cave::String::NPOS)
	{
        std::cout << "no \"a\" in \"test\"\n";
	}
}
```

Output:
```
no "a" in "test"
```

# Non-member functions
## Input / Output
### `operator<<`
```cpp
std::ostream& operator<<(std::ostream& os, const cave::String& str);
```

Behaves as a [FormattedOutputFunction](https://en.cppreference.com/w/cpp/named_req/FormattedOutputFunction). After constructing and checking the sentry object, determines the output format padding as follows:
1. If `str.GetLength()` is not less than `os.width()`, uses the range `[0, str.GetLength())` as-is
2. Otherwise, if (`os.flags()` & `ios_base::adjustfield`) == `ios_base::left`, places `os.width() - str.GetLength()` copies of the `os.fill()` character after the character sequence.
3. Otherwise, places `os.width() - str.size()` copies of the `os.fill()` character before the character sequence.

Then stored each character from the resulting sequence (the contents of `str` plus padding) to the output stream `os` as if by calling `os.rdbuf()->sputn(seq, n)`, where `n=std::max(os.width(), str.GetLength())`

Finally, calls `os.width(0)` to cancel the effects of `std::setw`, if any.

#### Parameters
|Parameters|Description|
|---|---|
|`os`|a character output stream|
|`is`|a character input stream|
|`str`|the string to be inserted|

#### Return value
```cpp
os
```

#### Example
```cpp
#include <iostream>
#include <sstream>

#include "String/String.h"
 
int main()
{
    cave::String greeting = "Hello, whirled!";

    std::cout << greeting << '\n';
}
```

Output:
```
Hello, whirled!
```