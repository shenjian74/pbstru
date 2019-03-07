# 简介

pbstru是Protobuf to Struct的缩写，主要完成PB码流和C语言结构间的相互转换，可依据PB码流定义文件(.proto)自动生成PB消息的编解码代码。

# 实现原理

```
    proto文件 --> |                               | --> 编解码部分的C代码 --> | --> 联编到APP
                  | --PBStru工具（pbstru.exe）--> |
```

特点：
1. Pure-C，使用C数据结构作为中介，调用者无需关心编解码细节；
1. 读取proto文件定义，自动生成C语言源代码及头文件；
1. 数据零拷贝，数据结构中以指针代替实际存储空间以提高效率；

# 编译方法

## Windows
	Windows下需要预安装Code::Blocks编译器及IDE。
	而后执行/pbstru/build.bat进行编译，可生成pbstru/bin/pbstru.exe，同时生成Code::Blocks下的工程文件(*.cbp)。

## linux
	linux下可执行/pbstru/build.sh进行编译，可生成pbstru/bin/pbstru。
	注意其前提条件为protobuf3已经编译通过，编译protobuf3的步骤为：
```
	$ cd pbstru/protobuf3
	$ autogen.sh
	$ configure
	$ make
```

# 执行方法

1. 修改proto文件。可增加接口消息，或者在接口消息中增加字段，修改时请注意接口兼容性，详见后面的protobuf兼容性准则；
2. 在windows下，运行pbstru\bin\pbstru.exe，入参为多个proto文件名及目标文件夹，执行完成后在目标文件夹的include/source子目录生成所有编解码源文件；
	在linux下，运行pbstru/bin/pbstru，入参为proto文件名及目标文件夹。
	Usage: pbstru xxx.proto [xxx.proto] target_dir

# 调用方法

## 消息编码

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

初始化消息结构的函数constru_message_*()只需要在结构定义后执行一次即可，有点类似memset，但速度比memset要快。
而clear_message_*()函数只清除消息中已经赋值的内容，速度较快，在静态结构重新赋值前需要使用此函数清空结构。

## 消息解码

```C
#include "response.h"
...
st_Response var_response;
...
/* NO need to call clear_message_xxx before this function */
decode_message_Response(BYTE *buf, size_t buf_len, &var_response);
```

decode函数内部会首先调用clear_message_XXX清空第三个参数所指向的结构，所以调用者无需手工调用clear_message_XXX函数。


# 注意事项

* pbstru使用了静态数组存储PB的repeated字段，所以必须事先确定Repeated字段的最多可能出现的个数才可以生成代码，相关的值需要在pbstru\bin\xxx.options文件中定义。
如果未定义相关的静态数组大小，pbstru会检测到并报错。
例如下列行表示CCA状态上报消息中租户ID信息最多出现的次数，需要按照上游文档的说明修改为正确的值。

```C
    Cannot read item:"zte.cdb.ccc.CCAResReportRequest.tenant_id max_count:?" from option file.
```

* protobuf兼容性原则（重要）

    1. 不可以改变已经存在的标签的tag值。
    1. 不可以增加或删除required字段。
    1. 可以删除可选(optional)或重复(repeated)字段。
    1. 可以添加新的可选或重复字段，但是必须使用新的tag数字，必须是之前的字段所没有用过的。
    1. 新的可选消息不会在旧的消息中显示，所以你需要使用 has_ 严格的检查他们是否存在，或者在proto 文件中提供一个缺省值。如果没有缺省值，就会有一个类型相关的默认缺省值：对于字符串就是空字符串；对于布尔型则是false；对于数字类型默认为0。
