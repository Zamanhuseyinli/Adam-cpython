from adamlibrary import adam
from adamlibrary import datasetname
from adamlibrary import stringer
from adamlibrary import memreplication_cutline
from adamlibrary import math
import ctypes

# calloc test
count = 5
size = ctypes.sizeof(ctypes.c_int)
ptr = adam.calloc(count, size)
print(f"Memory allocated using calloc at {ptr}")

# realloc test
new_size = size * 10
new_ptr = adam.realloc(ptr, new_size)
print(f"Memory reallocated to {new_ptr}")

# bsearch test
base_list = [1, 3, 5, 7, 9]
key = 5
result = adam.bsearch(key, base_list, len(base_list), ctypes.sizeof(ctypes.c_int))
print(f"Result of bsearch for key {key}: {result}")

# qsort test
unsorted_list = [9, 1, 3, 7, 5]
sorted_list = adam.qsort(unsorted_list, len(unsorted_list), ctypes.sizeof(ctypes.c_int))
print(f"Sorted list: {sorted_list}")

# memcpy test
src = b"Hello"
dest = ctypes.create_string_buffer(6)
adam.memcpy(dest, src, len(src))
print(f"Copied string: {dest.value.decode()}")

# strtok test
string = "Hello,world,example"
delimiters = ","
token = adam.strtok(string, delimiters)
print(f"First token: {token}")

# asctime test
import time
tm = time.localtime()  # Current time structure
formatted_time = adam.asctime(tm.tm_sec, tm.tm_min, tm.tm_hour, tm.tm_mday, tm.tm_mon, tm.tm_year)
print(f"Formatted time using asctime: {formatted_time}")

# localtime test
rawtime = int(time.time())
local_time = adam.localtime(rawtime)
print(f"Local time structure: {local_time}")

# memset test
mem_block = ctypes.create_string_buffer(10)
adam.memset(mem_block, 65, 10)  # ASCII value of 'A' is 65
print(f"Memory block after memset: {mem_block.raw}")

# memmove test
src = ctypes.create_string_buffer(b"abcdefg")
dest = ctypes.create_string_buffer(7)
adam.memmove(dest, src, 7)
print(f"Moved data: {dest.raw.decode()}")

# strncpy test
dest = ctypes.create_string_buffer(10)
src = "HelloWorld"
adam.strncpy(dest, src, 5)
print(f"String after strncpy: {dest.value.decode()}")

# memcmp test
ptr1 = b"abc"
ptr2 = b"abc"
result = adam.memcmp(ptr1, ptr2, len(ptr1))
print(f"Memory comparison result: {result}")

# sprintf test
formatted_str = datasetname.sprintf("This is a test %d", 123)
print(f"Formatted string: {formatted_str}")

# strchr test
char_str = "Hello, world!"
ch = ord('o')
found_str = datasetname.strchr(char_str, ch)
print(f"Result of strchr: {found_str}")

# strrchr test
found_last = datasetname.strrchr(char_str, ch)
print(f"Result of strrchr: {found_last}")
import datasetname
import ctypes

# Test for memset
def test_memset():
    buffer = ctypes.create_string_buffer(10)  # Creating a buffer of 10 bytes
    datasetname.memset(buffer, 65, 10)  # Set the buffer with ASCII value 65 (A)
    print(f"Memset result: {buffer.raw}")  # Should print 10 'A' characters

# Test for memcmp
def test_memcmp():
    buffer1 = "Hello"
    buffer2 = "Hello"
    result = datasetname.memcmp(buffer1, buffer2, len(buffer1))
    print(f"Memcmp result: {result}")  # Should print 0, since they are identical

    buffer2 = "World"
    result = datasetname.memcmp(buffer1, buffer2, len(buffer1))
    print(f"Memcmp result: {result}")  # Should print a non-zero value

# Test for strncpy
def test_strncpy():
    src = "Hello, World!"
    dest = ctypes.create_string_buffer(20)
    datasetname.strncpy(dest, src, 5)  # Copy first 5 characters
    print(f"Strncpy result: {dest.value.decode()}")  # Should print 'Hello'

# Test for memmove
def test_memmove():
    source = ctypes.create_string_buffer(b"1234567890")
    dest = ctypes.create_string_buffer(10)
    datasetname.memmove(dest, source, 5)  # Move first 5 characters
    print(f"Memmove result: {dest.raw}")  # Should print b'12345'

# Test for itoa
def test_itoa():
    value = 123
    result = datasetname.itoa(value)
    print(f"Itoa result: {result}")  # Should print '123'

# Test for sprintf
def test_sprintf():
    format_str = "Number: %d"
    result = datasetname.sprintf(format_str, 123)
    print(f"Sprintf result: {result}")  # Should print 'Number: 123'

# Test for strchr
def test_strchr():
    string = "Hello, World!"
    result = datasetname.strchr(string, ord('W'))
    print(f"Strchr result: {result}")  # Should print the substring starting from 'W'

# Test for strrchr
def test_strrchr():
    string = "Hello, World!"
    result = datasetname.strrchr(string, ord('o'))
    print(f"Strrchr result: {result}")  # Should print the substring starting from the last 'o'

# Test for atoi
def test_atoi():
    result = datasetname.atoi("123")
    print(f"Atoi result: {result}")  # Should print 123

# Test for abs
def test_abs():
    result = datasetname.abs(-50)
    print(f"Abs result: {result}")  # Should print 50

# Test for gcd
def test_gcd():
    result = datasetname.gcd(56, 98)
    print(f"GCD result: {result}")  # Should print 14
    
# Test stringer module functions
print("Testing stringer module:")
print(stringer.strdup("Hello, world!"))
print(stringer.strncat("Hello, ", "world!", 6))
print(stringer.strncmp("Hello", "Hello", 5))
print(stringer.strstr("Hello, world!", "world"))
print(stringer.strtok("apple,orange,banana", ","))
print(stringer.strlen("Hello, world!"))

# Test memreplication module functions
print("\nTesting memreplication module:")
memory = "Hello, world!"
print(memreplication.memchr(memory, 119, len(memory)))  # Find byte 'w'
print(memreplication.memset_s(memory, len(memory), 120))  # Set memory to 'x'
print(memreplication.malloc_usable_size(memory))
print(memreplication.aligned_alloc(16, 1024))  # Aligned memory allocation
print(memreplication.valloc(1024))  # Page-aligned memory allocation
print(memreplication.posix_memalign(16, 1024))  # POSIX memory alignment

# Test math module functions
print("\nTesting math module:")
print(math.pow(2, 3))  # 2 raised to the power of 3
print(math.sqrt(16))  # Square root of 16
print(math.exp(1))  # e^1
print(math.log(10))  # Natural log of 10
print(math.sin(3.14159 / 2))  # Sine of pi/2
print(math.cos(3.14159))  # Cosine of pi
print(math.tan(3.14159 / 4))  # Tangent of pi/4
print(math.ceil(4.3))  # Round up
print(math.floor(4.7))  # Round down
print(math.mod(10, 3))  # 10 mod 3
   

if __name__ == "__main__":
    test_memset()
    test_memcmp()
    test_strncpy()
    test_memmove()
    test_itoa()
    test_sprintf()
    test_strchr()
    test_strrchr()
    test_atoi()
    test_abs()
    test_gcd()
