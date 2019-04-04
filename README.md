PBStru
----

![](https://travis-ci.com/shenjian74/pbstru.svg?branch=master)

[中文](README-cn.md)

## Introduction

PBStru is a C source file generator for serialize data using [PROTOBUF](https://github.com/protocolbuffers/protobuf). 

## Principle

```
    .proto definition file --> |              | --> encode/decode source code --> | --> Link with your application
                               | -- PBStru--> |                                   |
```

Characteristics：

1. Using struct in pure C，wrapping the detail in encode/decode process.
1. Read from proto file, generate include file and source file.
1. Zero-copy，high performance.

## How to compile

### Windows
	
* Pre-install the mingw32 compiler.
* Run "build.bat" to build "pbstru/bin/pbstru.exe"

### linux
	
* Run "build.sh" to build "pbstru/bin/pbstru"。

## How to execute

1. Create/Modify your .proto file. you can add message, add fields to message. For Best practices keep the message compatibility. 
2. input parameters is one or multi proto files and target directory name.

```
	Usage: pbstru xxx.proto [xxx.proto] target_dir
```

## How to use

### encoding

```C
#include "response.h"
...
st_Response var_response;
constru_message_Response(&var_response);  // Clear all of message, just like memset, call it once.
...
clear_message_Response(&var_response);  // Clear used fields in struct, call it before encoding.
var_response.xxx = xxx;  // set value in struct var_response
...
BYTE buffer[max_buffer_length];
size_t bytes_write = encode_message_Response_safe(&var_response, buffer, sizeof(buffer));  // encode message
```

### decoding

```C
#include "response.h"
...
st_Response var_response;
...
/* NO need to call clear_message_xxx before this function */
decode_message_Response(BYTE *buf, size_t buf_len, &var_response);
```

## Notice

* PBStru use static array to store repeat fields, so you must define the max_size in options file. PBStru will report error information if you forget to set it. For example:

```
    Cannot read item:"zte.cdb.ccc.CCAResReportRequest.tenant_id max_count:?" from option file.
```

* PROTOBUF compatibility principles（Important）

    1. Do not Change the tag already existed.
    1. Do not add or delete required field.
    1. Optional or repeated field can be deleted.
    1. Use a new tag to add new optional or repeated field。
    1. Use has_xxx macro to check the new added optional field.
