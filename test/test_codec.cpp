// test_codec.cpp : �������̨Ӧ�ó������ڵ㡣
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
#include "ut_test_message.h"
#include "ut_test_sub_message.h"

char hex2asc(int hex)
{
    if (hex >= 0 && hex < 10) {
        return hex + 0x30;
    } else if (hex < 16) {
        return 'a' + hex - 10;
    } else {
        return '.';
    }
}

void print_buffer(BYTE * content, size_t filelen)
{
    size_t i;
    size_t line;
    size_t total_len;
    size_t total_line;

    total_len = filelen;
    total_line = total_len / 16;
    fputs("\n", stderr);
    for (line = 0; line < total_line; line++) {
        for (i = 0; i < 16; i++) {
            fputc(hex2asc(content[16 * line + i] / 16), stderr);
            fputc(hex2asc(content[16 * line + i] % 16), stderr);
            fputc(' ', stderr);
        }
        fputs("    ", stderr);
        for (i = 0; i < 16; i++) {
            fputc(isprint(content[16 * line + i]) ? content[16 * line + i] : '.', stderr);
        }
        fputc('\n', stderr);
    }
    for (i = 0; i < total_len % 16; i++) {
        fputc(hex2asc(content[16 * total_line + i] / 16), stderr);
        fputc(hex2asc(content[16 * total_line + i] % 16), stderr);
        fputc(' ', stderr);
    }
    for (i = total_len % 16; i < 16; i++) {
        fputs("   ", stderr);
    }
    fputs("    ", stderr);
    for (i = 0; i < total_len % 16; i++) {
        fputc(isprint(content[16 * total_line + i]) ? content[16 * total_line + i] : '.', stderr);
    }
    fputc('\n', stderr);
    fflush(stderr);
}

void fill_ut_test_sub_message(st_ut_test_sub_message * msg)
{
    constru_message_ut_test_sub_message(msg);
    msg->var_d_uint32.item[msg->var_d_uint32.count++] = 1000;
    msg->var_d_uint32.item[msg->var_d_uint32.count++] = 1001;
}

bool verify_ut_test_sub_message(st_ut_test_sub_message * msg)
{
    if (1000 == msg->var_d_uint32.item[0]) {
        if (1001 == msg->var_d_uint32.item[1]) {
            if (2 == msg->var_d_uint32.count) {
                return true;
            }
        }
    }
    return false;
}

st_addrequest var_AddRequest;
int main(int argc, char *argv[])
{
    BYTE buf[1024];
    size_t buf_len1, buf_len2;

    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);

    {
        int value = 0;
        size_t offset = 0;
        encode_varint(value, buf, &offset);
        assert(1 == offset);
        assert(0 == buf[0]);
        offset = 0;
        decode_varint(buf, &value, &offset);
        assert(1 == offset);
        assert(0 == value);

        value = 1;
        offset = 0;
        encode_varint(value, buf, &offset);
        assert(1 == offset);
        assert(0x01 == buf[0]);
        offset = 0;
        decode_varint(buf, &value, &offset);
        assert(1 == offset);
        assert(1 == value);

        value = 128;
        offset = 0;
        encode_varint(value, buf, &offset);
        assert(2 == offset);
        assert(0x80 == buf[0]);
        assert(0x01 == buf[1]);
        offset = 0;
        decode_varint(buf, &value, &offset);
        assert(2 == offset);
        assert(128 == value);

        value = 65535;
        offset = 0;
        encode_varint(value, buf, &offset);
        print_buffer(buf, offset);
        assert(3 == offset);
        assert(0xFF == buf[0]);
        assert(0xFF == buf[1]);
        assert(0x03 == buf[2]);
        offset = 0;
        decode_varint(buf, &value, &offset);
        assert(3 == offset);
        assert(65535 == value);

        value = 65536;
        offset = 0;
        encode_varint(value, buf, &offset);
        print_buffer(buf, offset);
        assert(3 == offset);
        assert(0x80 == buf[0]);
        assert(0x80 == buf[1]);
        assert(0x04 == buf[2]);
        offset = 0;
        decode_varint(buf, &value, &offset);
        assert(3 == offset);
        assert(65536 == value);

        long long value3 = 0xFFFFFFFF;
        offset = 0;
        encode_varint(value3, buf, &offset);
        print_buffer(buf, offset);
        assert(5 == offset);
        assert(0xFF == buf[0]);
        assert(0x0F == buf[4]);
        offset = 0;
        decode_varint(buf, &value3, &offset);
        assert(5 == offset);
        assert(0xFFFFFFFF == value3);

        fflush(stdout);
    }

    {
        st_tuple var_Tuple;
        constru_message_Tuple(&var_Tuple);
        var_Tuple.var_path.has_path_string = TRUE;
        var_Tuple.var_path.var_path_string.data = (BYTE *) strdup("/20");
        var_Tuple.var_path.var_path_string.length = strlen((char *) var_Tuple.var_path.var_path_string.data);
        var_Tuple.has_version = TRUE;
        var_Tuple.var_version = 2000;
        var_Tuple.has_ttl = TRUE;
        var_Tuple.var_ttl = 3000;
        var_Tuple.var_field.count = 2;
        var_Tuple.var_field.item[0].var_fieldid = 1;
        var_Tuple.var_field.item[0].var_value.data = (BYTE *) strdup("fawejlkrj1230940p1243lkjljfksldaj");
        var_Tuple.var_field.item[0].var_value.length = strlen((char *) var_Tuple.var_field.item[0].var_value.data);
        var_Tuple.var_field.item[1].var_fieldid = 2;
        var_Tuple.var_field.item[1].var_value.data = (BYTE *) strdup("jflasjfu32ujfljsljkljkljljoiu");
        var_Tuple.var_field.item[1].var_value.length = strlen((char *) var_Tuple.var_field.item[1].var_value.data);
        buf_len2 = encode_message_Tuple(&var_Tuple, buf);
        fflush(stdout);
        assert(87 == buf_len2);
        print_buffer(buf, buf_len2);

        decode_message_Tuple(buf, buf_len2, &var_Tuple);
        assert(TRUE == var_Tuple.var_path.has_path_string);
        assert(0 == memcmp(var_Tuple.var_path.var_path_string.data, "/20", var_Tuple.var_path.var_path_string.length));
        assert(TRUE == var_Tuple.has_version);
        assert(2000 == var_Tuple.var_version);
        assert(TRUE == var_Tuple.has_ttl);
        assert(3000 == var_Tuple.var_ttl);
        assert(var_Tuple.var_field.count == 2);
        assert(var_Tuple.var_field.item[0].var_fieldid == 1);
        assert(0 == memcmp(var_Tuple.var_field.item[0].var_value.data, "fawejlkrj1230940p1243lkjljfksldaj", var_Tuple.var_field.item[0].var_value.length));
        assert(var_Tuple.var_field.item[1].var_fieldid == 2);
        assert(0 == memcmp(var_Tuple.var_field.item[1].var_value.data, "jflasjfu32ujfljsljkljkljljoiu", var_Tuple.var_field.item[1].var_value.length));

        fflush(stdout);
    }

    {
        constru_message_AddRequest(&var_AddRequest);
        var_AddRequest.var_identifiers.var_primary.count = 1;
        var_AddRequest.var_identifiers.var_primary.item[0].var_id_type = 1;
        var_AddRequest.var_identifiers.var_primary.item[0].var_value.count = 1;
        var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].data = (BYTE *) strdup("465749674123167465431674613");
        var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].length = strlen((char *) var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].data);

        var_AddRequest.var_identifiers.var_non_primary.count = 1;
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_id_type = 1;
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.count = 1;
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].data = (BYTE *) strdup("465789461313213646461231324654");
        var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].length = strlen((char *) var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].data);

        var_AddRequest.var_tuple.count = 1;
        var_AddRequest.var_tuple.item[0].var_path.has_path_string = TRUE;
        var_AddRequest.var_tuple.item[0].var_path.var_path_string.data = (BYTE *) strdup("/20");
        var_AddRequest.var_tuple.item[0].var_path.var_path_string.length = strlen((char *) var_AddRequest.var_tuple.item[0].var_path.var_path_string.data);
        var_AddRequest.var_tuple.item[0].var_path.has_path_string = TRUE;

        var_AddRequest.var_tuple.item[0].has_version = TRUE;
        var_AddRequest.var_tuple.item[0].var_version = 2000;
        var_AddRequest.var_tuple.item[0].has_ttl = TRUE;
        var_AddRequest.var_tuple.item[0].var_ttl = 3000;

        var_AddRequest.var_tuple.item[0].var_field.count = 2;
        var_AddRequest.var_tuple.item[0].var_field.item[0].var_fieldid = 1;
        var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.data = (BYTE *) strdup("fawejlkrj1230940p1243lkjljfksldaj");
        var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.length = strlen((char *) var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.data);
        var_AddRequest.var_tuple.item[0].var_field.item[1].var_fieldid = 2;
        var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.data = (BYTE *) strdup("jflasjfu32ujfljsljkljkljljoiu");
        var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.length = strlen((char *) var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.data);

        buf_len1 = encode_message_AddRequest(&var_AddRequest, buf);
        assert(160 == buf_len1);
        print_buffer(buf, buf_len1);

        decode_message_AddRequest(buf, buf_len1, &var_AddRequest);
        assert(TRUE == var_AddRequest.var_tuple.item[0].var_path.has_path_string);
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

    {
        st_ut_test_message msg;

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(1000 == msg.var_d_uint32.item[0]);
            assert(1001 == msg.var_d_uint32.item[1]);
            assert(2 == msg.var_d_uint32.count);
        }

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);
            msg.var_r_uint32 = 10;
            msg.has_o_uint32 = TRUE;
            msg.var_o_uint32 = 11;
            msg.var_f_uint32.count = 2;
            msg.var_f_uint32.item[0] = 12;
            msg.var_f_uint32.item[1] = 13;

            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

            msg.var_pf_uint32.item[0] = 14;
            msg.var_pf_uint32.item[1] = 15;

            msg.var_pd_uint32.item[msg.var_pd_uint32.count++] = 16;
            msg.var_pd_uint32.item[msg.var_pd_uint32.count++] = 17;

            size_t size3 = encode_message_ut_test_message(&msg, buf);
            assert(55 == size3);
            decode_message_ut_test_message(buf, size3, &msg);

            assert(10 == msg.var_r_uint32);
            assert(TRUE == msg.has_o_uint32);
            assert(11 == msg.var_o_uint32);
            assert(2 == msg.var_f_uint32.count);
            assert(12 == msg.var_f_uint32.item[0]);
            assert(13 == msg.var_f_uint32.item[1]);
            assert(1000 == msg.var_d_uint32.item[0]);
            assert(1001 == msg.var_d_uint32.item[1]);
            assert(14 == msg.var_pf_uint32.item[0]);
            assert(15 == msg.var_pf_uint32.item[1]);
            assert(16 == msg.var_pd_uint32.item[0]);
            assert(17 == msg.var_pd_uint32.item[1]);
        }

        for (int i = 0; i < 3; ++i) {
            BYTE buf[128];
            size_t offset = 0;
            WORD64 var_int64;

            encode_varint(20360023315404117, buf, &offset);
            print_buffer(buf, offset);
            offset = 0;
            decode_varint(buf, &var_int64, &offset);
            assert(20360023315404117 == var_int64);

            constru_message_ut_test_message(&msg);
            msg.var_r_uint64 = 2447866062020153618;

            msg.has_o_uint64 = TRUE;
            msg.var_o_uint64 = 11;

            msg.var_f_uint64.count = 2;
            msg.var_f_uint64.item[0] = 12;
            msg.var_f_uint64.item[1] = 13;

            msg.var_d_uint64.item[msg.var_d_uint64.count++] = 1000;
            msg.var_d_uint64.item[msg.var_d_uint64.count++] = 1001;

            msg.var_pf_uint64.item[msg.var_pf_uint64.count++] = 14;
            msg.var_pf_uint64.item[msg.var_pf_uint64.count++] = 15;

            msg.var_pd_uint64.item[msg.var_pd_uint64.count++] = 16;
            msg.var_pd_uint64.item[msg.var_pd_uint64.count++] = 17;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(68 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(2447866062020153618 == msg.var_r_uint64);
            assert(TRUE == msg.has_o_uint64);
            assert(11 == msg.var_o_uint64);
            assert(2 == msg.var_f_uint64.count);
            assert(12 == msg.var_f_uint64.item[0]);
            assert(13 == msg.var_f_uint64.item[1]);
            assert(1000 == msg.var_d_uint64.item[0]);
            assert(1001 == msg.var_d_uint64.item[1]);
            assert(14 == msg.var_pf_uint64.item[0]);
            assert(15 == msg.var_pf_uint64.item[1]);
            assert(16 == msg.var_pd_uint64.item[0]);
            assert(17 == msg.var_pd_uint64.item[1]);
        }

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);
            msg.var_r_fixed32 = 10;
            msg.has_o_fixed32 = TRUE;
            msg.var_o_fixed32 = 11;
            msg.var_f_fixed32.count = 2;
            msg.var_f_fixed32.item[0] = 12;
            msg.var_f_fixed32.item[1] = 13;

            msg.var_d_fixed32.item[msg.var_d_fixed32.count++] = 1000;
            msg.var_d_fixed32.item[msg.var_d_fixed32.count++] = 1001;

            msg.var_pf_fixed32.item[0] = 14;
            msg.var_pf_fixed32.item[1] = 15;

            msg.var_pd_fixed32.item[msg.var_pd_fixed32.count++] = 16;
            msg.var_pd_fixed32.item[msg.var_pd_fixed32.count++] = 17;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(108 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(10 == msg.var_r_fixed32);
            assert(TRUE == msg.has_o_fixed32);
            assert(11 == msg.var_o_fixed32);
            assert(2 == msg.var_f_fixed32.count);
            assert(12 == msg.var_f_fixed32.item[0]);
            assert(13 == msg.var_f_fixed32.item[1]);
            assert(1000 == msg.var_d_fixed32.item[0]);
            assert(1001 == msg.var_d_fixed32.item[1]);
            assert(14 == msg.var_pf_fixed32.item[0]);
            assert(15 == msg.var_pf_fixed32.item[1]);
            assert(16 == msg.var_pd_fixed32.item[0]);
            assert(17 == msg.var_pd_fixed32.item[1]);
        }

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);
            msg.var_r_fixed64 = 10;
            msg.has_o_fixed64 = TRUE;
            msg.var_o_fixed64 = 11;
            msg.var_f_fixed64.count = 2;
            msg.var_f_fixed64.item[0] = 12;
            msg.var_f_fixed64.item[1] = 13;

            msg.var_d_fixed64.item[msg.var_d_fixed64.count++] = 1000;
            msg.var_d_fixed64.item[msg.var_d_fixed64.count++] = 1001;

            msg.var_pf_fixed64.item[0] = 14;
            msg.var_pf_fixed64.item[1] = 15;

            msg.var_pd_fixed64.item[msg.var_pd_fixed64.count++] = 16;
            msg.var_pd_fixed64.item[msg.var_pd_fixed64.count++] = 17;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(108 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(10 == msg.var_r_fixed64);
            assert(TRUE == msg.has_o_fixed64);
            assert(11 == msg.var_o_fixed64);
            assert(2 == msg.var_f_fixed64.count);
            assert(12 == msg.var_f_fixed64.item[0]);
            assert(13 == msg.var_f_fixed64.item[1]);
            assert(1000 == msg.var_d_fixed64.item[0]);
            assert(1001 == msg.var_d_fixed64.item[1]);
            assert(14 == msg.var_pf_fixed64.item[0]);
            assert(15 == msg.var_pf_fixed64.item[1]);
            assert(16 == msg.var_pd_fixed64.item[0]);
            assert(17 == msg.var_pd_fixed64.item[1]);
        }

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);
            msg.var_r_bool = TRUE;
            msg.has_o_bool = TRUE;
            msg.var_o_bool = FALSE;
            msg.var_f_bool.count = 2;
            msg.var_f_bool.item[0] = TRUE;
            msg.var_f_bool.item[1] = FALSE;

            msg.var_d_bool.item[msg.var_d_bool.count++] = TRUE;
            msg.var_d_bool.item[msg.var_d_bool.count++] = FALSE;

            msg.var_pf_bool.item[0] = TRUE;
            msg.var_pf_bool.item[1] = FALSE;

            msg.var_pd_bool.item[msg.var_pd_bool.count++] = TRUE;
            msg.var_pd_bool.item[msg.var_pd_bool.count++] = FALSE;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(59 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(TRUE == msg.var_r_bool);
            assert(TRUE == msg.has_o_bool);
            assert(FALSE == msg.var_o_bool);
            assert(2 == msg.var_f_bool.count);
            assert(TRUE == msg.var_f_bool.item[0]);
            assert(FALSE == msg.var_f_bool.item[1]);
            assert(TRUE == msg.var_d_bool.item[0]);
            assert(FALSE == msg.var_d_bool.item[1]);
            assert(TRUE == msg.var_pf_bool.item[0]);
            assert(FALSE == msg.var_pf_bool.item[1]);
            assert(TRUE == msg.var_pd_bool.item[0]);
            assert(FALSE == msg.var_pd_bool.item[1]);
        }

        for (int i = 0; i < 3; ++i) {
            char string1[] = "string1";
            char string2[] = "string2";
            char string3[] = "string3";
            char string4[] = "string4";
            char string5[] = "string5";
            char string6[] = "string6";

            constru_message_ut_test_message(&msg);
            msg.var_r_string.data = string1;
            msg.var_r_string.length = strlen(string1);
            msg.has_o_string = TRUE;
            msg.var_o_string.data = string2;
            msg.var_o_string.length = strlen(string2);
            msg.var_f_string.count = 2;
            msg.var_f_string.item[0].data = string3;
            msg.var_f_string.item[0].length = strlen(string3);
            msg.var_f_string.item[1].data = string4;
            msg.var_f_string.item[1].length = strlen(string4);

            msg.var_d_string.item[msg.var_d_string.count].data = string5;
            msg.var_d_string.item[msg.var_d_string.count].length = strlen(string5);
            msg.var_d_string.count += 1;
            msg.var_d_string.item[msg.var_d_string.count].data = string6;
            msg.var_d_string.item[msg.var_d_string.count].length = strlen(string6);
            msg.var_d_string.count += 1;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(96 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(0 == memcmp(msg.var_r_string.data, string1, msg.var_r_string.length));
            assert(TRUE == msg.has_o_string);
            assert(0 == memcmp(msg.var_o_string.data, string2, msg.var_o_string.length));
            assert(2 == msg.var_f_string.count);
            assert(0 == memcmp(msg.var_f_string.item[0].data, string3, msg.var_f_string.item[0].length));
            assert(0 == memcmp(msg.var_f_string.item[1].data, string4, msg.var_f_string.item[1].length));
            assert(0 == memcmp(msg.var_d_string.item[0].data, string5, msg.var_d_string.item[0].length));
            assert(0 == memcmp(msg.var_d_string.item[1].data, string6, msg.var_d_string.item[1].length));
        }

        for (int i = 0; i < 3; ++i) {
            BYTE bytes1[] = "bytes1";
            BYTE bytes2[] = "bytes2";
            BYTE bytes3[] = "bytes3";
            BYTE bytes4[] = "bytes4";
            BYTE bytes5[] = "bytes5";
            BYTE bytes6[] = "bytes6";

            constru_message_ut_test_message(&msg);
            msg.var_r_bytes.data = bytes1;
            msg.var_r_bytes.length = strlen((LPCSTR) bytes1);
            msg.has_o_bytes = TRUE;
            msg.var_o_bytes.data = bytes2;
            msg.var_o_bytes.length = strlen((LPCSTR) bytes2);
            msg.var_f_bytes.count = 2;
            msg.var_f_bytes.item[0].data = bytes3;
            msg.var_f_bytes.item[0].length = strlen((LPCSTR) bytes3);
            msg.var_f_bytes.item[1].data = bytes4;
            msg.var_f_bytes.item[1].length = strlen((LPCSTR) bytes4);

            msg.var_d_bytes.item[msg.var_d_bytes.count].data = bytes5;
            msg.var_d_bytes.item[msg.var_d_bytes.count].length = strlen((LPCSTR) bytes5);
            msg.var_d_bytes.count += 1;
            msg.var_d_bytes.item[msg.var_d_bytes.count].data = bytes6;
            msg.var_d_bytes.item[msg.var_d_bytes.count].length = strlen((LPCSTR) bytes6);
            msg.var_d_bytes.count += 1;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(90 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(0 == memcmp(msg.var_r_bytes.data, bytes1, msg.var_r_bytes.length));
            assert(TRUE == msg.has_o_bytes);
            assert(0 == memcmp(msg.var_o_bytes.data, bytes2, msg.var_o_bytes.length));
            assert(2 == msg.var_f_bytes.count);
            assert(0 == memcmp(msg.var_f_bytes.item[0].data, bytes3, msg.var_f_bytes.item[0].length));
            assert(0 == memcmp(msg.var_f_bytes.item[1].data, bytes4, msg.var_f_bytes.item[1].length));
            assert(0 == memcmp(msg.var_d_bytes.item[0].data, bytes5, msg.var_d_bytes.item[0].length));
            assert(0 == memcmp(msg.var_d_bytes.item[1].data, bytes6, msg.var_d_bytes.item[1].length));
        }

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);
            msg.var_r_enum = CLIENT_M;
            msg.has_o_enum = TRUE;
            msg.var_o_enum = SERVER_M;
            msg.var_f_enum.count = 2;
            msg.var_f_enum.item[0] = CLIENT_M;
            msg.var_f_enum.item[1] = SERVER_M;

            msg.var_d_enum.item[msg.var_d_enum.count++] = CLIENT_M;
            msg.var_d_enum.item[msg.var_d_enum.count++] = SERVER_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(54 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(CLIENT_M == msg.var_r_enum);
            assert(TRUE == msg.has_o_enum);
            assert(SERVER_M == msg.var_o_enum);
            assert(2 == msg.var_f_enum.count);
            assert(CLIENT_M == msg.var_f_enum.item[0]);
            assert(SERVER_M == msg.var_f_enum.item[1]);
            assert(CLIENT_M == msg.var_d_enum.item[0]);
            assert(SERVER_M == msg.var_d_enum.item[1]);
        }

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_message(&msg);

            fill_ut_test_sub_message(&(msg.var_r_message));
            msg.has_o_message = TRUE;
            fill_ut_test_sub_message(&(msg.var_o_message));
            msg.var_f_message.count = 2;
            fill_ut_test_sub_message(&(msg.var_f_message.item[0]));
            fill_ut_test_sub_message(&(msg.var_f_message.item[1]));

            fill_ut_test_sub_message(&(msg.var_d_message.item[msg.var_d_message.count]));
            msg.var_d_message.count += 1;
            fill_ut_test_sub_message(&(msg.var_d_message.item[msg.var_d_message.count]));
            msg.var_d_message.count += 1;

            size_t size2 = encode_message_ut_test_message(&msg, buf);
            assert(90 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(verify_ut_test_sub_message(&(msg.var_r_message)));
            assert(TRUE == msg.has_o_message);
            assert(verify_ut_test_sub_message(&(msg.var_o_message)));
            assert(2 == msg.var_f_message.count);
            assert(verify_ut_test_sub_message(&(msg.var_f_message.item[0])));
            assert(verify_ut_test_sub_message(&(msg.var_f_message.item[1])));
            assert(verify_ut_test_sub_message(&(msg.var_d_message.item[0])));
            assert(verify_ut_test_sub_message(&(msg.var_d_message.item[1])));
        }
    }

    {
        st_ut_test_sub_message msg;

        for (int i = 0; i < 3; ++i) {
            constru_message_ut_test_sub_message(&msg);
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

            size_t size2 = encode_message_ut_test_sub_message(&msg, buf);
            assert(6 == size2);
            decode_message_ut_test_sub_message(buf, size2, &msg);
            assert(1000 == msg.var_d_uint32.item[0]);
            assert(1001 == msg.var_d_uint32.item[1]);
        }
    }

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
