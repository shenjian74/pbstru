// test_codec.cpp : 定义控制台应用程序的入口点。
//

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <asyncconfirmreq.h>
#include <sys/time.h>
#include "addrequest.h"
#include "compoundrequest.h"
#include "response.h"
#include "statresp.h"
// #include "ut_test_message.h"
// #include "ut_test_sub_message.h"

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

void decode_varint1(BYTE *buf, WORD64 *value, size_t *offset) {
    size_t iloop;
    (*(value)) = 0;
    for(iloop=0;;++iloop) {
        (*(value)) += ((unsigned long long)((buf)[iloop] & 0x7F)) << (7*iloop);
        if(0 == ((buf)[iloop] & 0x80)){
            break;
        }
    }
    (*(offset)) += 1 + iloop;
}

/* void fill_ut_test_sub_message(st_ut_test_sub_message *msg)
{
    clear_message_ut_test_sub_message(msg);
    msg->var_d_uint32 = (st_d_uint32_in_ut_test_sub_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_sub_message_uint32_list));
    msg->var_d_uint32_tail = msg->var_d_uint32;
    msg->var_d_uint32->next = NULL;
    msg->var_d_uint32->value = 1000;
    msg->var_d_uint32->next = (st_d_uint32_in_ut_test_sub_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_sub_message_uint32_list));
    msg->var_d_uint32_tail = msg->var_d_uint32->next;
    msg->var_d_uint32_tail->next = NULL;
    msg->var_d_uint32_tail->value = 1001;
}

bool verify_ut_test_sub_message(st_ut_test_sub_message *msg)
{
    if(1000 == msg->var_d_uint32->value)
    {
        if(1001 == msg->var_d_uint32->next->value)
        {
            if(NULL == msg->var_d_uint32->next->next)
            {
                return true;
            }
        }
    }
    return false;
}
*/

st_addrequest var_AddRequest;
int main(int argc, char* argv[])
{
    BYTE buf[1024];
    size_t buf_len;

    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);

    {
        int value = 0;
        int offset = 0;
        encode_varint(value, buf, &offset);
        assert(1 == offset);
        assert(0 == buf[0]);
    }

    {
        st_tuple var_Tuple;
        clear_message_Tuple(&var_Tuple);
        var_Tuple.var_path.var_path_string.data = (BYTE *)strdup("/20");
        var_Tuple.var_path.var_path_string.length = strlen((char *)var_Tuple.var_path.var_path_string.data);
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
        assert(0 == memcmp(var_Tuple.var_path.var_path_string.data, "/20", var_Tuple.var_path.var_path_string.length));
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
        var_AddRequest.var_tuple.item[0].var_path.var_path_string.data = (BYTE *)strdup("/20");
        var_AddRequest.var_tuple.item[0].var_path.var_path_string.length = strlen((char *)var_AddRequest.var_tuple.item[0].var_path.var_path_string.data);
        var_AddRequest.var_tuple.item[0].var_path.has_path_string = TRUE;

        var_AddRequest.var_tuple.item[0].has_version = TRUE;
        var_AddRequest.var_tuple.item[0].var_version = 2000;
        var_AddRequest.var_tuple.item[0].has_ttl = TRUE;
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
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_path.var_path_string.data, "/20", var_AddRequest.var_tuple.item[0].var_path.var_path_string.length));
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

/*
    {
        st_ut_test_message *msg = (st_ut_test_message *)pbstru_malloc(sizeof(st_ut_test_message));
        constru_message_ut_test_message(msg);

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);
            msg->var_d_uint32 = (st_d_uint32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_message_uint32_list));
            msg->var_d_uint32_tail = msg->var_d_uint32;
            msg->var_d_uint32->next = NULL;
            msg->var_d_uint32->value = 1000;
            msg->var_d_uint32->next = (st_d_uint32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_message_uint32_list));
            msg->var_d_uint32_tail = msg->var_d_uint32->next;
            msg->var_d_uint32_tail->next = NULL;
            msg->var_d_uint32_tail->value = 1001;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);
            assert(1000 == msg->var_d_uint32->value);
            assert(1001 == msg->var_d_uint32->next->value);
            assert(NULL == msg->var_d_uint32->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);
            msg->var_r_uint32 = 10;
            msg->has_o_uint32 = TRUE;
            msg->var_o_uint32 = 11;
            msg->var_f_uint32.count = 2;
            msg->var_f_uint32.item[0] = 12;
            msg->var_f_uint32.item[1] = 13;

            msg->var_d_uint32 = (st_d_uint32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_message_uint32_list));
            msg->var_d_uint32_tail = msg->var_d_uint32;
            msg->var_d_uint32->next = NULL;
            msg->var_d_uint32->value = 1000;
            msg->var_d_uint32->next = (st_d_uint32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_message_uint32_list));
            msg->var_d_uint32_tail = msg->var_d_uint32->next;
            msg->var_d_uint32_tail->next = NULL;
            msg->var_d_uint32_tail->value = 1001;

            msg->var_pf_uint32.item[0] = 14;
            msg->var_pf_uint32.item[1] = 15;

            msg->var_pd_uint32 = (st_pd_uint32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_pd_uint32_in_ut_test_message_uint32_list));
            msg->var_pd_uint32_tail = msg->var_pd_uint32;
            msg->var_pd_uint32->next = NULL;
            msg->var_pd_uint32->value = 16;
            msg->var_pd_uint32->next = (st_pd_uint32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_pd_uint32_in_ut_test_message_uint32_list));
            msg->var_pd_uint32_tail = msg->var_pd_uint32->next;
            msg->var_pd_uint32_tail->next = NULL;
            msg->var_pd_uint32_tail->value = 17;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(10 == msg->var_r_uint32);
            assert(TRUE == msg->has_o_uint32);
            assert(11 == msg->var_o_uint32);
            assert(2 == msg->var_f_uint32.count);
            assert(12 == msg->var_f_uint32.item[0]);
            assert(13 == msg->var_f_uint32.item[1]);
            assert(1000 == msg->var_d_uint32->value);
            assert(1001 == msg->var_d_uint32->next->value);
            assert(NULL == msg->var_d_uint32->next->next);
            assert(14 == msg->var_pf_uint32.item[0]);
            assert(15 == msg->var_pf_uint32.item[1]);
            assert(16 == msg->var_pd_uint32->value);
            assert(17 == msg->var_pd_uint32->next->value);
            assert(NULL == msg->var_pd_uint32->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            BYTE buf[64];
            size_t offset = 0;
            WORD64 var_int64;

            encode_varint(20360023315404117, buf, &offset);
            print_buffer(buf, offset);
            offset = 0;
            decode_varint(buf, &var_int64, &offset);
            assert(20360023315404117 == var_int64);

            offset = 0;
            encode_varint(2447866062020153618, buf, &offset);
            print_buffer(buf, offset);
            offset = 0;
            decode_varint(buf, &var_int64, &offset);
            assert(2447866062020153618 == var_int64);

            clear_message_ut_test_message(msg);
            msg->var_r_uint64 = 2447866062020153618;
            msg->has_o_uint64 = TRUE;
            msg->var_o_uint64 = 11;
            msg->var_f_uint64.count = 2;
            msg->var_f_uint64.item[0] = 12;
            msg->var_f_uint64.item[1] = 13;

            msg->var_d_uint64 = (st_d_uint64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_d_uint64_in_ut_test_message_uint64_list));
            msg->var_d_uint64_tail = msg->var_d_uint64;
            msg->var_d_uint64->next = NULL;
            msg->var_d_uint64->value = 1000;
            msg->var_d_uint64->next = (st_d_uint64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_d_uint64_in_ut_test_message_uint64_list));
            msg->var_d_uint64_tail = msg->var_d_uint64->next;
            msg->var_d_uint64_tail->next = NULL;
            msg->var_d_uint64_tail->value = 1001;

            msg->var_pf_uint64.item[0] = 14;
            msg->var_pf_uint64.item[1] = 15;

            msg->var_pd_uint64 = (st_pd_uint64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_pd_uint64_in_ut_test_message_uint64_list));
            msg->var_pd_uint64_tail = msg->var_pd_uint64;
            msg->var_pd_uint64->next = NULL;
            msg->var_pd_uint64->value = 16;
            msg->var_pd_uint64->next = (st_pd_uint64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_pd_uint64_in_ut_test_message_uint64_list));
            msg->var_pd_uint64_tail = msg->var_pd_uint64->next;
            msg->var_pd_uint64_tail->next = NULL;
            msg->var_pd_uint64_tail->value = 17;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(2447866062020153618 == msg->var_r_uint64);
            assert(TRUE == msg->has_o_uint64);
            assert(11 == msg->var_o_uint64);
            assert(2 == msg->var_f_uint64.count);
            assert(12 == msg->var_f_uint64.item[0]);
            assert(13 == msg->var_f_uint64.item[1]);
            assert(1000 == msg->var_d_uint64->value);
            assert(1001 == msg->var_d_uint64->next->value);
            assert(NULL == msg->var_d_uint64->next->next);
            assert(14 == msg->var_pf_uint64.item[0]);
            assert(15 == msg->var_pf_uint64.item[1]);
            assert(16 == msg->var_pd_uint64->value);
            assert(17 == msg->var_pd_uint64->next->value);
            assert(NULL == msg->var_pd_uint64->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);
            msg->var_r_fixed32 = 10;
            msg->has_o_fixed32 = TRUE;
            msg->var_o_fixed32 = 11;
            msg->var_f_fixed32.count = 2;
            msg->var_f_fixed32.item[0] = 12;
            msg->var_f_fixed32.item[1] = 13;

            msg->var_d_fixed32 = (st_d_fixed32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_d_fixed32_in_ut_test_message_uint32_list));
            msg->var_d_fixed32_tail = msg->var_d_fixed32;
            msg->var_d_fixed32->next = NULL;
            msg->var_d_fixed32->value = 1000;
            msg->var_d_fixed32->next = (st_d_fixed32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_d_fixed32_in_ut_test_message_uint32_list));
            msg->var_d_fixed32_tail = msg->var_d_fixed32->next;
            msg->var_d_fixed32_tail->next = NULL;
            msg->var_d_fixed32_tail->value = 1001;

            msg->var_pf_fixed32.item[0] = 14;
            msg->var_pf_fixed32.item[1] = 15;

            msg->var_pd_fixed32 = (st_pd_fixed32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_pd_fixed32_in_ut_test_message_uint32_list));
            msg->var_pd_fixed32_tail = msg->var_pd_fixed32;
            msg->var_pd_fixed32->next = NULL;
            msg->var_pd_fixed32->value = 16;
            msg->var_pd_fixed32->next = (st_pd_fixed32_in_ut_test_message_uint32_list *)pbstru_malloc(sizeof(st_pd_fixed32_in_ut_test_message_uint32_list));
            msg->var_pd_fixed32_tail = msg->var_pd_fixed32->next;
            msg->var_pd_fixed32_tail->next = NULL;
            msg->var_pd_fixed32_tail->value = 17;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(10 == msg->var_r_fixed32);
            assert(TRUE == msg->has_o_fixed32);
            assert(11 == msg->var_o_fixed32);
            assert(2 == msg->var_f_fixed32.count);
            assert(12 == msg->var_f_fixed32.item[0]);
            assert(13 == msg->var_f_fixed32.item[1]);
            assert(1000 == msg->var_d_fixed32->value);
            assert(1001 == msg->var_d_fixed32->next->value);
            assert(NULL == msg->var_d_fixed32->next->next);
            assert(14 == msg->var_pf_fixed32.item[0]);
            assert(15 == msg->var_pf_fixed32.item[1]);
            assert(16 == msg->var_pd_fixed32->value);
            assert(17 == msg->var_pd_fixed32->next->value);
            assert(NULL == msg->var_pd_fixed32->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);
            msg->var_r_fixed64 = 10;
            msg->has_o_fixed64 = TRUE;
            msg->var_o_fixed64 = 11;
            msg->var_f_fixed64.count = 2;
            msg->var_f_fixed64.item[0] = 12;
            msg->var_f_fixed64.item[1] = 13;

            msg->var_d_fixed64 = (st_d_fixed64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_d_fixed64_in_ut_test_message_uint64_list));
            msg->var_d_fixed64_tail = msg->var_d_fixed64;
            msg->var_d_fixed64->next = NULL;
            msg->var_d_fixed64->value = 1000;
            msg->var_d_fixed64->next = (st_d_fixed64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_d_fixed64_in_ut_test_message_uint64_list));
            msg->var_d_fixed64_tail = msg->var_d_fixed64->next;
            msg->var_d_fixed64_tail->next = NULL;
            msg->var_d_fixed64_tail->value = 1001;

            msg->var_pf_fixed64.item[0] = 14;
            msg->var_pf_fixed64.item[1] = 15;

            msg->var_pd_fixed64 = (st_pd_fixed64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_pd_fixed64_in_ut_test_message_uint64_list));
            msg->var_pd_fixed64_tail = msg->var_pd_fixed64;
            msg->var_pd_fixed64->next = NULL;
            msg->var_pd_fixed64->value = 16;
            msg->var_pd_fixed64->next = (st_pd_fixed64_in_ut_test_message_uint64_list *)pbstru_malloc(sizeof(st_pd_fixed64_in_ut_test_message_uint64_list));
            msg->var_pd_fixed64_tail = msg->var_pd_fixed64->next;
            msg->var_pd_fixed64_tail->next = NULL;
            msg->var_pd_fixed64_tail->value = 17;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(10 == msg->var_r_fixed64);
            assert(TRUE == msg->has_o_fixed64);
            assert(11 == msg->var_o_fixed64);
            assert(2 == msg->var_f_fixed64.count);
            assert(12 == msg->var_f_fixed64.item[0]);
            assert(13 == msg->var_f_fixed64.item[1]);
            assert(1000 == msg->var_d_fixed64->value);
            assert(1001 == msg->var_d_fixed64->next->value);
            assert(NULL == msg->var_d_fixed64->next->next);
            assert(14 == msg->var_pf_fixed64.item[0]);
            assert(15 == msg->var_pf_fixed64.item[1]);
            assert(16 == msg->var_pd_fixed64->value);
            assert(17 == msg->var_pd_fixed64->next->value);
            assert(NULL == msg->var_pd_fixed64->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);
            msg->var_r_bool = TRUE;
            msg->has_o_bool = TRUE;
            msg->var_o_bool = FALSE;
            msg->var_f_bool.count = 2;
            msg->var_f_bool.item[0] = TRUE;
            msg->var_f_bool.item[1] = FALSE;

            msg->var_d_bool = (st_d_bool_in_ut_test_message_boolean_list *)pbstru_malloc(sizeof(st_d_bool_in_ut_test_message_boolean_list));
            msg->var_d_bool_tail = msg->var_d_bool;
            msg->var_d_bool->next = NULL;
            msg->var_d_bool->value = TRUE;
            msg->var_d_bool->next = (st_d_bool_in_ut_test_message_boolean_list *)pbstru_malloc(sizeof(st_d_bool_in_ut_test_message_boolean_list));
            msg->var_d_bool_tail = msg->var_d_bool->next;
            msg->var_d_bool_tail->next = NULL;
            msg->var_d_bool_tail->value = FALSE;

            msg->var_pf_bool.item[0] = TRUE;
            msg->var_pf_bool.item[1] = FALSE;

            msg->var_pd_bool = (st_pd_bool_in_ut_test_message_boolean_list *)pbstru_malloc(sizeof(st_pd_bool_in_ut_test_message_boolean_list));
            msg->var_pd_bool_tail = msg->var_pd_bool;
            msg->var_pd_bool->next = NULL;
            msg->var_pd_bool->value = TRUE;
            msg->var_pd_bool->next = (st_pd_bool_in_ut_test_message_boolean_list *)pbstru_malloc(sizeof(st_pd_bool_in_ut_test_message_boolean_list));
            msg->var_pd_bool_tail = msg->var_pd_bool->next;
            msg->var_pd_bool_tail->next = NULL;
            msg->var_pd_bool_tail->value = FALSE;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(TRUE == msg->var_r_bool);
            assert(TRUE == msg->has_o_bool);
            assert(FALSE == msg->var_o_bool);
            assert(2 == msg->var_f_bool.count);
            assert(TRUE == msg->var_f_bool.item[0]);
            assert(FALSE == msg->var_f_bool.item[1]);
            assert(TRUE == msg->var_d_bool->value);
            assert(FALSE == msg->var_d_bool->next->value);
            assert(NULL == msg->var_d_bool->next->next);
            assert(TRUE == msg->var_pf_bool.item[0]);
            assert(FALSE == msg->var_pf_bool.item[1]);
            assert(TRUE == msg->var_pd_bool->value);
            assert(FALSE == msg->var_pd_bool->next->value);
            assert(NULL == msg->var_pd_bool->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            char string1[] = "string1";
            char string2[] = "string2";
            char string3[] = "string3";
            char string4[] = "string4";
            char string5[] = "string5";
            char string6[] = "string6";

            clear_message_ut_test_message(msg);
            msg->var_r_string.data = string1;
            msg->var_r_string.length = strlen(string1);
            msg->has_o_string = TRUE;
            msg->var_o_string.data = string2;
            msg->var_o_string.length = strlen(string2);
            msg->var_f_string.count = 2;
            msg->var_f_string.item[0].data = string3;
            msg->var_f_string.item[0].length = strlen(string3);
            msg->var_f_string.item[1].data = string4;
            msg->var_f_string.item[1].length = strlen(string4);

            msg->var_d_string = (st_d_string_in_ut_test_message_string_list *)pbstru_malloc(sizeof(st_d_string_in_ut_test_message_string_list));
            msg->var_d_string_tail = msg->var_d_string;
            msg->var_d_string->next = NULL;
            msg->var_d_string->value.data = string5;
            msg->var_d_string->value.length = strlen(string5);
            msg->var_d_string->next = (st_d_string_in_ut_test_message_string_list *)pbstru_malloc(sizeof(st_d_string_in_ut_test_message_string_list));
            msg->var_d_string_tail = msg->var_d_string->next;
            msg->var_d_string_tail->next = NULL;
            msg->var_d_string_tail->value.data = string6;
            msg->var_d_string_tail->value.length = strlen(string6);

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(0 == memcmp(msg->var_r_string.data, string1, msg->var_r_string.length));
            assert(TRUE == msg->has_o_string);
            assert(0 == memcmp(msg->var_o_string.data, string2, msg->var_o_string.length));
            assert(2 == msg->var_f_string.count);
            assert(0 == memcmp(msg->var_f_string.item[0].data, string3, msg->var_f_string.item[0].length));
            assert(0 == memcmp(msg->var_f_string.item[1].data, string4, msg->var_f_string.item[1].length));
            assert(0 == memcmp(msg->var_d_string->value.data, string5, msg->var_d_string->value.length));
            assert(0 == memcmp(msg->var_d_string->next->value.data, string6, msg->var_d_string->next->value.length));
            assert(NULL == msg->var_d_string->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            BYTE bytes1[] = "bytes1";
            BYTE bytes2[] = "bytes2";
            BYTE bytes3[] = "bytes3";
            BYTE bytes4[] = "bytes4";
            BYTE bytes5[] = "bytes5";
            BYTE bytes6[] = "bytes6";

            clear_message_ut_test_message(msg);
            msg->var_r_bytes.data = bytes1;
            msg->var_r_bytes.length = strlen((LPCSTR)bytes1);
            msg->has_o_bytes = TRUE;
            msg->var_o_bytes.data = bytes2;
            msg->var_o_bytes.length = strlen((LPCSTR)bytes2);
            msg->var_f_bytes.count = 2;
            msg->var_f_bytes.item[0].data = bytes3;
            msg->var_f_bytes.item[0].length = strlen((LPCSTR)bytes3);
            msg->var_f_bytes.item[1].data = bytes4;
            msg->var_f_bytes.item[1].length = strlen((LPCSTR)bytes4);

            msg->var_d_bytes = (st_d_bytes_in_ut_test_message_buffer_list *)pbstru_malloc(sizeof(st_d_bytes_in_ut_test_message_buffer_list));
            msg->var_d_bytes_tail = msg->var_d_bytes;
            msg->var_d_bytes->next = NULL;
            msg->var_d_bytes->value.data = bytes5;
            msg->var_d_bytes->value.length = strlen((LPCSTR)bytes5);
            msg->var_d_bytes->next = (st_d_bytes_in_ut_test_message_buffer_list *)pbstru_malloc(sizeof(st_d_bytes_in_ut_test_message_buffer_list));
            msg->var_d_bytes_tail = msg->var_d_bytes->next;
            msg->var_d_bytes_tail->next = NULL;
            msg->var_d_bytes_tail->value.data = bytes6;
            msg->var_d_bytes_tail->value.length = strlen((LPCSTR)bytes6);

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(0 == memcmp(msg->var_r_bytes.data, bytes1, msg->var_r_bytes.length));
            assert(TRUE == msg->has_o_bytes);
            assert(0 == memcmp(msg->var_o_bytes.data, bytes2, msg->var_o_bytes.length));
            assert(2 == msg->var_f_bytes.count);
            assert(0 == memcmp(msg->var_f_bytes.item[0].data, bytes3, msg->var_f_bytes.item[0].length));
            assert(0 == memcmp(msg->var_f_bytes.item[1].data, bytes4, msg->var_f_bytes.item[1].length));
            assert(0 == memcmp(msg->var_d_bytes->value.data, bytes5, msg->var_d_bytes->value.length));
            assert(0 == memcmp(msg->var_d_bytes->next->value.data, bytes6, msg->var_d_bytes->next->value.length));
            assert(NULL == msg->var_d_bytes->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);
            msg->var_r_enum = CLIENT_M;
            msg->has_o_enum = TRUE;
            msg->var_o_enum = SERVER_M;
            msg->var_f_enum.count = 2;
            msg->var_f_enum.item[0] = CLIENT_M;
            msg->var_f_enum.item[1] = SERVER_M;

            msg->var_d_enum = (st_d_enum_in_ut_test_message_ut_test_enum_list *)pbstru_malloc(sizeof(st_d_enum_in_ut_test_message_ut_test_enum_list));
            msg->var_d_enum_tail = msg->var_d_enum;
            msg->var_d_enum->next = NULL;
            msg->var_d_enum->value = CLIENT_M;
            msg->var_d_enum->next = (st_d_enum_in_ut_test_message_ut_test_enum_list *)pbstru_malloc(sizeof(st_d_enum_in_ut_test_message_ut_test_enum_list));
            msg->var_d_enum_tail = msg->var_d_enum->next;
            msg->var_d_enum_tail->next = NULL;
            msg->var_d_enum_tail->value = SERVER_M;

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(CLIENT_M == msg->var_r_enum);
            assert(TRUE == msg->has_o_enum);
            assert(SERVER_M == msg->var_o_enum);
            assert(2 == msg->var_f_enum.count);
            assert(CLIENT_M == msg->var_f_enum.item[0]);
            assert(SERVER_M == msg->var_f_enum.item[1]);
            assert(CLIENT_M == msg->var_d_enum->value);
            assert(SERVER_M == msg->var_d_enum->next->value);
            assert(NULL == msg->var_d_enum->next->next);
        }

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_message(msg);

            fill_ut_test_sub_message(&(msg->var_r_message));
            msg->has_o_message = TRUE;
            fill_ut_test_sub_message(&(msg->var_o_message));
            msg->var_f_message.count = 2;
            fill_ut_test_sub_message(&(msg->var_f_message.item[0]));
            fill_ut_test_sub_message(&(msg->var_f_message.item[1]));

            msg->var_d_message = (st_d_message_in_ut_test_message_ut_test_sub_message_list *)pbstru_malloc(sizeof(st_d_message_in_ut_test_message_ut_test_sub_message_list));
            msg->var_d_message_tail = msg->var_d_message;
            msg->var_d_message->next = NULL;
            // 刚申请的message结构需要初始化
            constru_message_ut_test_sub_message(&(msg->var_d_message->value));
            fill_ut_test_sub_message(&(msg->var_d_message->value));
            msg->var_d_message->next = (st_d_message_in_ut_test_message_ut_test_sub_message_list *)pbstru_malloc(sizeof(st_d_message_in_ut_test_message_ut_test_sub_message_list));
            msg->var_d_message_tail = msg->var_d_message->next;
            msg->var_d_message_tail->next = NULL;
            // 刚申请的message结构需要初始化
            constru_message_ut_test_sub_message(&(msg->var_d_message_tail->value));
            fill_ut_test_sub_message(&(msg->var_d_message_tail->value));

            size_t size1 = encode_message_ut_test_message(msg, NULL);
            size_t size2 = encode_message_ut_test_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_message(buf, size2, msg);

            assert(verify_ut_test_sub_message(&(msg->var_r_message)));
            assert(TRUE == msg->has_o_message);
            assert(verify_ut_test_sub_message(&(msg->var_o_message)));
            assert(2 == msg->var_f_message.count);
            assert(verify_ut_test_sub_message(&(msg->var_f_message.item[0])));
            assert(verify_ut_test_sub_message(&(msg->var_f_message.item[1])));
            assert(verify_ut_test_sub_message(&(msg->var_d_message->value)));
            assert(verify_ut_test_sub_message(&(msg->var_d_message->next->value)));
            assert(NULL == msg->var_d_message->next->next);
        }

        destru_message_ut_test_message(msg);
        pbstru_free(msg);
    }

    {
        st_ut_test_sub_message *msg = (st_ut_test_sub_message *)pbstru_malloc(sizeof(st_ut_test_sub_message));
        constru_message_ut_test_sub_message(msg);

        for(int i=0; i<3; ++i)
        {
            clear_message_ut_test_sub_message(msg);
            msg->var_d_uint32 = (st_d_uint32_in_ut_test_sub_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_sub_message_uint32_list));
            msg->var_d_uint32_tail = msg->var_d_uint32;
            msg->var_d_uint32->next = NULL;
            msg->var_d_uint32->value = 1000;
            msg->var_d_uint32->next = (st_d_uint32_in_ut_test_sub_message_uint32_list *)pbstru_malloc(sizeof(st_d_uint32_in_ut_test_sub_message_uint32_list));
            msg->var_d_uint32_tail = msg->var_d_uint32->next;
            msg->var_d_uint32_tail->next = NULL;
            msg->var_d_uint32_tail->value = 1001;

            size_t size1 = encode_message_ut_test_sub_message(msg, NULL);
            size_t size2 = encode_message_ut_test_sub_message(msg, buf);
            assert(size1 == size2);
            decode_message_ut_test_sub_message(buf, size2, msg);
            assert(1000 == msg->var_d_uint32->value);
            assert(1001 == msg->var_d_uint32->next->value);
            assert(NULL == msg->var_d_uint32->next->next);
        }

        destru_message_ut_test_sub_message(msg);
        pbstru_free(msg);
    }
*/
    printf("sizeof(st_addrequest): %lu\n", sizeof(st_addrequest));
    printf("sizeof(st_compountrequest): %lu\n", sizeof(st_compoundrequest));
    printf("sizeof(st_response): %lu\n", sizeof(st_response));
    printf("sizeof(st_tuple): %lu\n", sizeof(st_tuple));

    // printf("<Press any key to continue ...>\n");
    // getchar();

    gettimeofday(&tv_end, NULL);
    double diff = 1000000.0 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    printf("[%s:%d] spend %.0lf(us)\n", __FILE__, __LINE__, diff);

    printf("%s Done.\n", argv[0]);
	return 0;
}




