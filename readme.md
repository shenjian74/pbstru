# ���

	pbstru��Protobuf to Struct����д����Ҫ���PB������C���Խṹ����໥ת�������Զ�����PB��Ϣ�ı������롣

## �汾����

2017-11-28 v2.4 -- ֧���ظ��ֶε�packed����

# ʵ��ԭ��

```
    proto�ļ� -->|                             |--> ����벿�ֵ�C���� -->|--> ���ൽAPP
                 |--PBStru���ߣ�pbstru.exe��-->|
```

# ���뷽��

## Windows
	Windows����ҪԤ��װCode::Blocks��������IDE��
	����ִ��/pbstru/build.bat���б��룬������pbstru/bin/pbstru.exe��

## linux
	linux�¿�ִ��/pbstru/build.sh���б��룬������pbstru/bin/pbstru��
	ע����ǰ������Ϊprotobuf3�Ѿ�����ͨ��������protobuf3�Ĳ���Ϊ��
```
	$ cd pbstru/protobuf3
	$ autogen.sh
	$ configure
	$ make
```
	
# ִ�з���

1. �޸�proto�ļ��������ӽӿ���Ϣ�������ڽӿ���Ϣ�������ֶΣ��޸�ʱ��ע��ӿڼ����ԣ���������protobuf������׼��
2. ��windows�£�����pbstru\bin\pbstru.exe�����Ϊproto�ļ������������б����Դ�ļ���
	��linux�£�����pbstru/bin/pbstru�����Ϊproto�ļ���
3. ��pbstru\bin\codecĿ¼�£��ɿ����Զ����ɵĴ��룬����ͷ�ļ���Դ�ļ����ɿ�����Щ���뵽����Ŀ¼��������������������ࡣ

# ���÷���

## ��Ϣ����

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
��һ�ε���encode_message_xxxʱ���ڶ������ΪNULL��ֻ������ĳ��ȣ��ڶ��ε���ʱ���ڶ������Ϊ��Ч��ַ����ʱ��ʵ���򻺳���������ݡ�
����������а��գ�������ɻ����������Ҳ�ɺ��Ե�һ�ε������������Ч�ʡ�

## ��Ϣ����

```C
#include "response.h"
...
/* NO need to call clear_message_xxx before this function */ 
decode_message_Response(BYTE *buf, size_t buf_len, &var_response);
```

decode�����ڲ������ȵ���clear_message_XXX��յ�����������ָ��Ľṹ�����Ե����������ֹ�����clear_message_XXX������


# ע������
	
* pbstruʹ���˾�̬����洢PB��repeated�ֶΣ����Ա�������ȷ��Repeated�ֶε������ܳ��ֵĸ����ſ������ɴ��룬��ص�ֵ��Ҫ��pbstru\bin\xxx.options�ļ��ж��塣
���δ������صľ�̬�����С��pbstru���⵽������
���������б�ʾCCA״̬�ϱ���Ϣ���⻧ID��Ϣ�����ֵĴ�������Ҫ���������ĵ���˵���޸�Ϊ��ȷ��ֵ��

```C	
    Cannot read item:"zte.cdb.ccc.CCAResReportRequest.tenant_id max_count:?" from option file.
```

* protobuf������ԭ����Ҫ��

	1. �����Ըı��Ѿ����ڵı�ǩ��tagֵ��
    2. ���������ӻ�ɾ��required�ֶΡ�
    3. ����ɾ����ѡ(optional)���ظ�(repeated)�ֶΡ�
    4. ��������µĿ�ѡ���ظ��ֶΣ����Ǳ���ʹ���µ�tag���֣�������֮ǰ���ֶ���û���ù��ġ�
    5. �µĿ�ѡ��Ϣ�����ھɵ���Ϣ����ʾ����������Ҫʹ�� has_ �ϸ�ļ�������Ƿ���ڣ�������proto �ļ����ṩһ��ȱʡֵ�����û��ȱʡֵ���ͻ���һ��������ص�Ĭ��ȱʡֵ�������ַ������ǿ��ַ��������ڲ���������false��������������Ĭ��Ϊ0��