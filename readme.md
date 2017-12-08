# 简介

	pbstru是Protobuf to Struct的缩写，主要完成PB码流和C语言结构间的相互转换，可自动生成PB消息的编解码代码。

## 版本更新

2017-11-28 v2.4 -- 支持重复字段的packed属性

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