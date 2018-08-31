# 简介

	pbstru是Protobuf to Struct的缩写，主要完成PB码流和C语言结构间的相互转换，可依据PB码流定义文件(.proto)自动生成PB消息的编解码代码。

## 版本更新

**2018-3-20 v2.7**

1. 支持生成代码到指定目录

	Usage: pbstru xxx.proto [xxx.proto] target_dir
	
	生成的源代码生成在target_dir/source目录，头文件生成在target_dir/include目录。
	如果target_dir目录下不存在source和include目录，则尝试创建目录。
	
----

**2018-3-16 v2.6**

1. 解决UINT64解码有误的问题

	在32位环境下，对于无符号64位整数的解码有问题。

1. option文件中的max_count支持字符串方式的宏定义

	option文件中可以将max_count定义为字符串方式的宏，这样pbstru所生成的代码可使用上层头文件中定义的宏，实现宏的单次定义多处引用。

	例：以下定义将生成数组tenant_id[MAX_SLOT_NUM]

```
zte.cdb.ccc.CCAResReportRequest.tenant_id max_count:MAX_SLOT_NUM
```

----

**2017-12-8 v2.5**

1. 无重要更新

----

**2017-11-28 v2.4**

1. 支持重复字段的packed属性。

对于整数型的repeated字段，有两种编码方式：

* 普通编码方式：数组中的每一个元素都以tag+len+content的方式出现，相同tag值的信息不需要排列在一起，但顺序需要保证；
* 紧缩编码方式，将数组中的元素全部排列在一起，以tag+len+(content_1+...+content_n)的方式出现，所有相同tag值的信息紧密排列在一起，可减少编码后的字节数；

对于基本的数字类型(varint, 32-bit, 或者64-bit) 可将repeated字段可声明成packed，其他类型的repeated字段不允许。

* 在protobuf版本2中，默认支持普通编码，可设置字段的属性为[packed=true]以支持紧缩编码方式。
* 在protobuf版本3中，默认可以开启紧缩编码方式的都自动开启，可设置字段的属性为[packed=false]以支持普通编码方式。

目前pbstru的特性与protobuf版本2相同，即默认支持普通编码方式。
在和protobuf版本3进行接口联调时，可在proto文件中将字段设置为[packed=true]，或者要求对方将相应字段设置为[packed=false]，两边需要一致即可。

例：以下proto定义，可要求pbstru生成紧缩方式的码流。
```
message ut_test_message
{
    repeated uint32 pf_uint32 = 5 [packed=true];
}
```

# 实现原理

```
    proto文件 -->|                             |--> 编解码部分的C代码 -->|--> 联编到APP
                 |--PBStru工具（pbstru.exe）-->|
```

# 编译方法

## Windows
	Windows下需要预安装Code::Blocks编译器及IDE。
	而后执行/pbstru/build.bat进行编译，可生成pbstru/bin/pbstru.exe。

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
2. 在windows下，运行pbstru\bin\pbstru.exe，入参为proto文件名，生成所有编解码源文件；
	在linux下，运行pbstru/bin/pbstru，入参为proto文件名
3. 在pbstru\bin\codec目录下，可看到自动生成的代码，包含头文件和源文件，可拷贝这些代码到其他目录，并与其他代码进行联编。

# 调用方法

## 消息编码

```C
#include "response.h"
... 
st_Response var_response; 
clear_message_Response(&var_response); 
/* set value in struct var_response */
var_response.xxx = xxx;
... 
BYTE buffer[max_buffer_length];
if(encode_message_Response(&var_response, NULL) <= max_buffer_length)  // First call encode_message_xxx 
{
    size_t bytes_write = encode_message_Response(&var_response, buffer);  // second call encode_message_xxx
    ......
}
```
第一次调用encode_message_xxx时，第二个入参为NULL，只估算包文长度；第二次调用时，第二个入参为有效地址，此时才实际向缓冲区输出数据。
如果对输入有把握，不会造成缓冲区溢出，也可忽略第一次调用以提高运行效率。

## 消息解码

```C
#include "response.h"
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
    2. 不可以增加或删除required字段。
    3. 可以删除可选(optional)或重复(repeated)字段。
    4. 可以添加新的可选或重复字段，但是必须使用新的tag数字，必须是之前的字段所没有用过的。
    5. 新的可选消息不会在旧的消息中显示，所以你需要使用 has_ 严格的检查他们是否存在，或者在proto 文件中提供一个缺省值。如果没有缺省值，就会有一个类型相关的默认缺省值：对于字符串就是空字符串；对于布尔型则是false；对于数字类型默认为0。