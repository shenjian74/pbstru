# ���
	PBSTRU��Protobuf to Struct����д����Ҫ���PB������C���Խṹ����໥ת�������Զ�����PB��Ϣ�ı������롣

# ʵ��ԭ��

```
    proto�ļ� -->|                             |--> ����벿�ֵ�C����
                 |--PBStru���ߣ�pbstru.exe��-->|
```

# ִ�з���

1. �޸�proto�ļ����ļ�λ��\3rdparty\protobuf\cdb(_ccc).proto����������Ϣ����������Ϣ�������ֶΣ��޸�ʱ��ע��ӿڼ����ԣ��������ļ�����׼��
2. ��windows�£�����\3rdparty\protobuf\protoc.bat�����������ļ�������proto�����ļ�����������������Դ�ļ���
3. ��\cdbtools\pbstru\pbstru\codecĿ¼�£��ɿ����Զ����ɵĴ��룬����ͷ�ļ���Դ�ļ���PB�е�һ����Ϣ���Ӧ1��ͷ�ļ���1��Դ�ļ���

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
if(encode_message_Response(&var_response, NULL) <= max_buffer_length){
    size_t bytes_write = encode_message_Response(&var_response, buffer);
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
	
* pbstruʹ���˾�̬����洢PB��repeated�ֶΣ����Ա�������ȷ��Repeated�ֶε������ܳ��ֵĸ����ſ������ɴ��룬��ص�ֵ��Ҫ��\cdbtools\pbstru\pbstru\xxx.options�ļ��ж��塣���δ������صľ�̬�����С��pbstru���⵽���������������б�ʾCCA״̬�ϱ���Ϣ���⻧ID��Ϣ�����ֵĴ�������Ҫ���������ĵ���˵���޸�Ϊ��ȷ��ֵ��

```C	
    Cannot read item:"zte.cdb.ccc.CCAResReportRequest.tenant_id max_count:?" from option file.
```

* protobuf������ԭ����Ҫ��

	1. �����Ըı��Ѿ����ڵı�ǩ��tagֵ��
    2. ���������ӻ�ɾ��required�ֶΡ�
    3. ����ɾ����ѡ(optional)���ظ�(repeated)�ֶΡ�
    4. ��������µĿ�ѡ���ظ��ֶΣ����Ǳ���ʹ���µ�tag���֣�������֮ǰ���ֶ���û���ù��ġ�
    5. �µĿ�ѡ��Ϣ�����ھɵ���Ϣ����ʾ����������Ҫʹ�� has_ �ϸ�ļ�������Ƿ���ڣ�������proto �ļ����ṩһ��ȱʡֵ�����û��ȱʡֵ���ͻ���һ��������ص�Ĭ��ȱʡֵ�������ַ������ǿ��ַ��������ڲ���������false��������������Ĭ��Ϊ0��