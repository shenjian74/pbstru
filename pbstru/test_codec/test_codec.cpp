// test_codec.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <asyncconfirmreq.h>
#include "addrequest.h"
#include "compoundrequest.h"
#include "response.h"
#include "statresp.h"
#include "asyncconfirmreq.h"

void print_buffer(BYTE *content, size_t filelen)
{
    size_t i;
    size_t line;
    size_t total_len;
    size_t total_line;

    total_len = filelen;
    total_line = total_len / 16;
    fprintf(stderr, "\n");
    for (line = 0; line < total_line; line++)
    {
        for (i = 0; i < 16; i++)
        {
            fprintf(stderr, "%02x ", content[16 * line + i]);
        }
        fprintf(stderr, "    ");
        for (i = 0; i < 16; i++)
        {
            fprintf(stderr, "%c", isprint(content[16 * line + i]) ? content[16 * line + i] : '.');
        }
        fprintf(stderr, "\n");
    }
    for (i = 0; i < total_len % 16; i++)
    {
        fprintf(stderr, "%02x ", content[16 * total_line + i]);
    }
    for (i = total_len % 16; i < 16; i++)
    {
        fprintf(stderr, "   ");
    }
    fprintf(stderr, "    ");
    for (i = 0; i < total_len % 16; i++)
    {
        fprintf(stderr, "%c", isprint(content[16 * total_line + i])?content[16 * total_line + i] : '.');
    }
    fprintf(stderr, "\n");
    fflush(stderr);
}

void pbstru_free(void *buf){
    if(NULL != buf){
        free(buf);
    }
}

void *pbstru_malloc(size_t size){
    return malloc(size);
}

st_AddRequest var_AddRequest;
int main(int argc, char* argv[])
{
    BYTE buf[1024];
    size_t buf_len;

    printf("sizeof(st_AddRequest) == %u\n", sizeof(st_AddRequest));
    printf("sizeof(st_QueryRequest) == %u\n", sizeof(st_QueryRequest));
    {
        printf("  sizeof(st_Identifiers) == %u\n", sizeof(st_Identifiers));
        printf("  sizeof(st_RANGE_IN_QUERYREQUEST_Range_list) == %u\n", sizeof(st_RANGE_IN_QUERYREQUEST_Range_list));
        {
            printf("    sizeof(st_Path) == %u\n", sizeof(st_Path));
            printf("    sizeof(enum_Scope) == %u\n", sizeof(enum_Scope));
            printf("    sizeof(st_FIELD_IN_RANGE_uint32_list) == %u\n", sizeof(st_FIELD_IN_RANGE_uint32_list));
            printf("    sizeof(st_FILTER_IN_RANGE_Filter_list) == %u\n", sizeof(st_FILTER_IN_RANGE_Filter_list));
        }
        printf("  sizeof(st_CONTROL_IN_QUERYREQUEST_Control_list) == %u\n", sizeof(st_CONTROL_IN_QUERYREQUEST_Control_list));
        printf("  sizeof(st_SPINFO_IN_QUERYREQUEST_SPInfo_list) == %u\n", sizeof(st_SPINFO_IN_QUERYREQUEST_SPInfo_list));
        printf("  sizeof(st_BatchQRYPI) == %u\n", sizeof(st_BatchQRYPI));
    }
    printf("sizeof(st_ModifyRequest) == %u\n", sizeof(st_ModifyRequest));
    {
        printf("  sizeof(st_ModData) == %u\n", sizeof(st_ModData));
        printf("  sizeof(st_ModField_list) == %u\n", sizeof(st_FIELD_IN_MODDATA_ModField_list));
    }
    printf("sizeof(st_DeleteRequest) == %u\n", sizeof(st_DeleteRequest));
    printf("sizeof(st_CompoundRequest) == %u\n", sizeof(st_CompoundRequest));
    printf("sizeof(st_Response) == %u\n", sizeof(st_Response));
    printf("sizeof(st_StatResp) == %u\n", sizeof(st_StatResp));

//sizeof(st_AddRequest) == 56424
//sizeof(st_QueryRequest) == 7928
//  sizeof(st_Identifiers) == 1064
//  sizeof(st_Range_list) == 6788
//    sizeof(st_Path) == 148
//    sizeof(enum_Scope) == 4
//    sizeof(st_uint32_list) == 8
//    sizeof(st_Filter_list) == 52
//  sizeof(st_Control_list) == 44
//  sizeof(st_SPInfo_list) == 8
//  sizeof(st_BatchQRYPI) == 20
//sizeof(st_ModifyRequest) == 31584
//  sizeof(st_ModData) == 952
//  sizeof(st_ModField_list) == 772
//sizeof(st_DeleteRequest) == 7904
//sizeof(st_CompoundRequest) == 830984
//sizeof(st_Response) == 451424
//sizeof(st_StatResp) == 65568

    {
        st_PrimaryKey var_PrimaryKey = { 0 };
        var_PrimaryKey.var_name.data = strdup("jflksajdfkljasdlkjflaksdjf");
        var_PrimaryKey.var_name.length = strlen(var_PrimaryKey.var_name.data);
        var_PrimaryKey.var_value.data = (BYTE *)strdup("732098403urouwaoeljr9749712309u4");
        var_PrimaryKey.var_value.length = strlen((char *)var_PrimaryKey.var_value.data);
        buf_len = encode_message_PrimaryKey(&var_PrimaryKey, buf);
        print_buffer(buf, buf_len);
        free(var_PrimaryKey.var_name.data);
        free(var_PrimaryKey.var_value.data);

        decode_message_PrimaryKey(buf, buf_len, &var_PrimaryKey);
        assert(0 == memcmp(var_PrimaryKey.var_name.data, "jflksajdfkljasdlkjflaksdjf", var_PrimaryKey.var_name.length));
        assert(0 == memcmp(var_PrimaryKey.var_value.data, "732098403urouwaoeljr9749712309u4", var_PrimaryKey.var_value.length));
    }

    {
        st_Path var_Path = { 0 };
        var_Path.var_tid = 1000;
        var_Path.var_primary_key.item[0].var_name.data = strdup("jflksajdfkljasdlkjflaksdjf");
        var_Path.var_primary_key.item[0].var_name.length = strlen(var_Path.var_primary_key.item[0].var_name.data);
        var_Path.var_primary_key.item[0].var_value.data = (BYTE *)strdup("732098403urouwaoeljr9749712309u4");
        var_Path.var_primary_key.item[0].var_value.length = strlen((char *)var_Path.var_primary_key.item[0].var_value.data);
        var_Path.var_primary_key.item[1].var_name.data = strdup("3u2iouflkjalskjfklasjflk");
        var_Path.var_primary_key.item[1].var_name.length = strlen(var_Path.var_primary_key.item[1].var_name.data);
        var_Path.var_primary_key.item[1].var_value.data = (BYTE *)strdup("324lkjakslfjlksajdfkljaslkdfj");
        var_Path.var_primary_key.item[1].var_value.length = strlen((char *)var_Path.var_primary_key.item[1].var_value.data);
        var_Path.var_primary_key.count = 2;
        buf_len = encode_message_Path(&var_Path, buf);
        print_buffer(buf, buf_len);

        decode_message_Path(buf, buf_len, &var_Path);
        assert(0 == memcmp(var_Path.var_primary_key.item[0].var_name.data, "jflksajdfkljasdlkjflaksdjf", var_Path.var_primary_key.item[0].var_name.length));
        assert(0 == memcmp(var_Path.var_primary_key.item[0].var_value.data, "732098403urouwaoeljr9749712309u4", var_Path.var_primary_key.item[0].var_value.length));
        assert(0 == memcmp(var_Path.var_primary_key.item[1].var_name.data, "3u2iouflkjalskjfklasjflk", var_Path.var_primary_key.item[1].var_name.length));
        assert(0 == memcmp(var_Path.var_primary_key.item[1].var_value.data, "324lkjakslfjlksajdfkljaslkdfj", var_Path.var_primary_key.item[1].var_value.length));
        assert(2 == var_Path.var_primary_key.count);
        assert(var_Path.var_tid == 1000);
    }

    {
        st_Tuple var_Tuple = { 0 };
        var_Tuple.var_path.var_tid = 1000;
        var_Tuple.var_path.var_primary_key.item[0].var_name.data = strdup("jflksajdfkljasdlkjflaksdjf");
        var_Tuple.var_path.var_primary_key.item[0].var_name.length = strlen(var_Tuple.var_path.var_primary_key.item[0].var_name.data);
        var_Tuple.var_path.var_primary_key.item[0].var_value.data = (BYTE *)strdup("732098403urouwaoeljr9749712309u4");
        var_Tuple.var_path.var_primary_key.item[0].var_value.length = strlen((char *)var_Tuple.var_path.var_primary_key.item[0].var_value.data);
        var_Tuple.var_path.var_primary_key.item[1].var_name.data = strdup("3u2iouflkjalskjfklasjflk");
        var_Tuple.var_path.var_primary_key.item[1].var_name.length = strlen(var_Tuple.var_path.var_primary_key.item[1].var_name.data);
        var_Tuple.var_path.var_primary_key.item[1].var_value.data = (BYTE *)strdup("324lkjakslfjlksajdfkljaslkdfj");
        var_Tuple.var_path.var_primary_key.item[1].var_value.length = strlen((char *)var_Tuple.var_path.var_primary_key.item[1].var_value.data);
        var_Tuple.var_path.var_primary_key.count = 2;
        var_Tuple.has_version = true;
        var_Tuple.var_version = 2000;
        var_Tuple.has_ttl = true;
        var_Tuple.var_ttl = 3000;
        var_Tuple.var_field.count = 2;
        var_Tuple.var_field.item[0].var_fieldid = 1;
        var_Tuple.var_field.item[0].var_value.data = (BYTE *)strdup("fawejlkrj1230940p1243lkjljfksldaj");
        var_Tuple.var_field.item[0].var_value.length = strlen((char *)var_Tuple.var_field.item[0].var_value.data);
        var_Tuple.var_field.item[1].var_fieldid = 2;
        var_Tuple.var_field.item[1].var_value.data = (BYTE *)strdup("jflasjfu32ujfljsljkljkljljoiu");
        var_Tuple.var_field.item[1].var_value.length = strlen((char *)var_Tuple.var_field.item[1].var_value.data);
        buf_len = encode_message_Tuple(&(var_Tuple), buf);
        print_buffer(buf, buf_len);

        decode_message_Tuple(buf, buf_len, &var_Tuple);
        assert(0 == memcmp(var_Tuple.var_path.var_primary_key.item[0].var_name.data, "jflksajdfkljasdlkjflaksdjf", var_Tuple.var_path.var_primary_key.item[0].var_name.length));
        assert(0 == memcmp(var_Tuple.var_path.var_primary_key.item[0].var_value.data, "732098403urouwaoeljr9749712309u4", var_Tuple.var_path.var_primary_key.item[0].var_value.length));
        assert(0 == memcmp(var_Tuple.var_path.var_primary_key.item[1].var_name.data, "3u2iouflkjalskjfklasjflk", var_Tuple.var_path.var_primary_key.item[1].var_name.length));
        assert(0 == memcmp(var_Tuple.var_path.var_primary_key.item[1].var_value.data, "324lkjakslfjlksajdfkljaslkdfj", var_Tuple.var_path.var_primary_key.item[1].var_value.length));
        assert(2 == var_Tuple.var_path.var_primary_key.count);
        assert(var_Tuple.var_path.var_tid == 1000);
        assert(2000 == var_Tuple.var_version);
        assert(TRUE == var_Tuple.has_version);
        assert(3000 == var_Tuple.var_ttl);
        assert(TRUE == var_Tuple.has_ttl);
        assert(var_Tuple.var_field.count == 2);
        assert(var_Tuple.var_field.item[0].var_fieldid == 1);
        assert(0 == memcmp(var_Tuple.var_field.item[0].var_value.data, "fawejlkrj1230940p1243lkjljfksldaj", var_Tuple.var_field.item[0].var_value.length));
        assert(var_Tuple.var_field.item[1].var_fieldid == 2);
        assert(0 == memcmp(var_Tuple.var_field.item[1].var_value.data, "jflasjfu32ujfljsljkljkljljoiu", var_Tuple.var_field.item[1].var_value.length));
    }

    {
        clear_message_AddRequest(&var_AddRequest);

        var_AddRequest.var_identifiers.var_primary.count = 1;
        var_AddRequest.var_identifiers.var_primary.item[0].var_id_type = 1;
        var_AddRequest.var_identifiers.var_primary.item[0].var_value.count = 1;
        var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].data = (BYTE *)strdup("465749674123167465431674613");
        var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].length = strlen((char *)var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].data);

        var_AddRequest.var_identifiers.var_non_primary.count = 1;
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_id_type = 1;
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.count = 1;
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].data = (BYTE *)strdup("465789461313213646461231324654");
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].length = strlen((char *)var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].data);

        var_AddRequest.var_tuple.count = 1;
        clear_message_Tuple(&(var_AddRequest.var_tuple.item[0]));
        var_AddRequest.var_tuple.item[0].var_path.var_tid = 1000;
        var_AddRequest.var_tuple.item[0].var_path.var_primary_key.count = 2;

        st_PrimaryKey *pk = &(var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[0]);
        pk->var_name.data = strdup("jflksajdfkljasdlkjflaksdjf");
        pk->var_name.length = strlen(pk->var_name.data);
        pk->var_value.data = (BYTE *)strdup("732098403urouwaoeljr9749712309u4");
        pk->var_value.length = strlen((char *)pk->var_value.data);

        pk = &(var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[1]);
        pk->var_name.data = strdup("3u2iouflkjalskjfklasjflk");
        pk->var_name.length = strlen(pk->var_name.data);
        pk->var_value.data = (BYTE *)strdup("324lkjakslfjlksajdfkljaslkdfj");
        pk->var_value.length = strlen((char *)pk->var_value.data);

        var_AddRequest.var_tuple.item[0].has_version = true;
        var_AddRequest.var_tuple.item[0].var_version = 2000;
        var_AddRequest.var_tuple.item[0].has_ttl = true;
        var_AddRequest.var_tuple.item[0].var_ttl = 3000;

        var_AddRequest.var_tuple.item[0].var_field.count = 2;
        var_AddRequest.var_tuple.item[0].var_field.item[0].var_fieldid = 1;
        var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.data = (BYTE *)strdup("fawejlkrj1230940p1243lkjljfksldaj");
        var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.length = strlen((char *)var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.data);
        var_AddRequest.var_tuple.item[0].var_field.item[1].var_fieldid = 2;
        var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.data = (BYTE *)strdup("jflasjfu32ujfljsljkljkljljoiu");
        var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.length = strlen((char *)var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.data);

        buf_len = encode_message_AddRequest(&var_AddRequest, buf);
        print_buffer(buf, buf_len);

        decode_message_AddRequest(buf, buf_len, &var_AddRequest);
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[0].var_name.data, "jflksajdfkljasdlkjflaksdjf", var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[0].var_name.length));
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[0].var_value.data, "732098403urouwaoeljr9749712309u4", var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[0].var_value.length));
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[1].var_name.data, "3u2iouflkjalskjfklasjflk", var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[1].var_name.length));
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[1].var_value.data, "324lkjakslfjlksajdfkljaslkdfj", var_AddRequest.var_tuple.item[0].var_path.var_primary_key.item[1].var_value.length));
        assert(1000 == var_AddRequest.var_tuple.item[0].var_path.var_tid);
        assert(2000 == var_AddRequest.var_tuple.item[0].var_version);
        assert(TRUE == var_AddRequest.var_tuple.item[0].has_version);
        assert(3000 == var_AddRequest.var_tuple.item[0].var_ttl);
        assert(TRUE == var_AddRequest.var_tuple.item[0].has_ttl);

        assert(var_AddRequest.var_tuple.item[0].var_field.item[0].var_fieldid == 1);
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.data, "fawejlkrj1230940p1243lkjljfksldaj", var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.length));
        assert(var_AddRequest.var_tuple.item[0].var_field.item[1].var_fieldid == 2);
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.data, "jflasjfu32ujfljsljkljkljljoiu", var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.length));

        assert(var_AddRequest.var_identifiers.var_primary.item[0].var_id_type == 1);
        assert(var_AddRequest.var_identifiers.var_primary.item[0].var_value.count == 1);
        assert(0 == memcmp(var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].data, "465749674123167465431674613", var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].length));

        assert(var_AddRequest.var_identifiers.var_non_primary.item[0].var_id_type == 1);
        assert(var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.count == 1);
        assert(0 == memcmp(var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].data, "465789461313213646461231324654", var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].length));
    }

    /* disable dynamic array function ...
    {
        st_AsyncConfirmReq *msg = (st_AsyncConfirmReq *)pbstru_malloc(sizeof(st_AsyncConfirmReq));
        constru_message_AsyncConfirmReq(msg);

        clear_message_AsyncConfirmReq(msg);
        DWORD kk[1000];
        printf("sizeofkk: %u", sizeof(kk));
        msg->var_slotid_list.max_size = 1000;
        msg->var_slotid_list.item = (DWORD *)pbstru_malloc(sizeof(DWORD) * msg->var_slotid_list.max_size);
        for(int i=0;i<msg->var_slotid_list.max_size;++i){
            msg->var_slotid_list.item[i] = i;
            msg->var_slotid_list.count += 1;
        }
        for(int i=0;i<msg->var_slotid_list.count;++i){
            printf("%u ", msg->var_slotid_list.item[i]);
        }

        destru_message_AsyncConfirmReq(msg);
        assert(0 == msg->var_slotid_list.count);
        assert(NULL == msg->var_slotid_list.item);
        assert(0 == msg->var_slotid_list.max_size);
    }
    */

    printf("%s Done.\n", argv[0]);
}


