#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <asyncconfirmreq.h>
#include <string>
#include "addrequest.h"
#include "compoundrequest.h"
#include "response.h"
#include "statresp.h"
#include "global_t.h"
#include "ut_test_message.h"
#include "ut_test_sub_message.h"
#include "http2appreq.h"

#define _THIS_FILE "test_codec.cpp"

char hex2asc(int hex)
{
    if (hex >= 0 && hex < 10)
    {
        return hex + 0x30;
    }
    else if (hex < 16)
    {
        return 'a' + hex - 10;
    }
    else
    {
        return '.';
    }
}

void print_buffer(BYTE * content, size_t filelen)
{
    size_t i;
    size_t line;
    size_t total_len;
    size_t total_line;
    char buffer[128];

    total_len = filelen;
    total_line = total_len / 16;
    fputs("\n", stderr);
    sprintf(buffer, "[Total %zu bytes]\n", filelen);
    fputs(buffer, stderr);

    for (line = 0; line < total_line; line++)
    {
        for (i = 0; i < 16; i++)
        {
            fputc(hex2asc(content[16 * line + i] / 16), stderr);
            fputc(hex2asc(content[16 * line + i] % 16), stderr);
            fputc(' ', stderr);
        }
        fputs("    ", stderr);
        for (i = 0; i < 16; i++)
        {
            fputc(isprint(content[16 * line + i]) ? content[16 * line + i] : '.', stderr);
        }
        fputc('\n', stderr);
    }
    for (i = 0; i < total_len % 16; i++)
    {
        fputc(hex2asc(content[16 * total_line + i] / 16), stderr);
        fputc(hex2asc(content[16 * total_line + i] % 16), stderr);
        fputc(' ', stderr);
    }
    for (i = total_len % 16; i < 16; i++)
    {
        fputs("   ", stderr);
    }
    fputs("    ", stderr);
    for (i = 0; i < total_len % 16; i++)
    {
        fputc(isprint(content[16 * total_line + i]) ? content[16 * total_line + i] : '.', stderr);
    }
    fputs("\n--------------------------------------------------------------------\n", stderr);
    fflush(stderr);
}

void write_buffer_file(const char *filename, BYTE * buf, size_t buf_len2)
{
    FILE *fp = fopen(filename, "wb");
    if (NULL == fp)
    {
        return;
    }
    printf("Write %zd bytes to %s\n", buf_len2, filename);
    fwrite(buf, buf_len2, 1, fp);
    fclose(fp);
}

std::string get_pb_string(BYTE * buf,
                          size_t buf_len2,
                          const char *proto_filename,
                          const char *message_type,
                          const char *filename,
                          const int lineno)
{
    char command[256];
    std::string result;
    char pb_filename[] = "pb.bin";
    char line[4*1024];

    print_buffer(buf, buf_len2);
    write_buffer_file(pb_filename, buf, buf_len2);
#ifdef _WIN32
    sprintf(command, "protoc.exe --decode=%s %s < %s", message_type, proto_filename, pb_filename);
#else
    sprintf(command, "./protoc --decode=%s %s < %s", message_type, proto_filename, pb_filename);
#endif // _WIN32
    printf("[%s:%d]> %s\n", filename, lineno, command);

#ifdef _WIN32
    FILE *p = _popen(command, "r");
#else    
    FILE *p = popen(command, "r");
#endif    
    while (fgets(line, sizeof(line), p))
        result += line;
    printf("result:[\n%s\n]\n", result.c_str());
#ifdef _WIN32
    _pclose(p);
#else    
    pclose(p);
#endif    
    return result;
}

void fill_ut_test_sub_message(st_ut_test_sub_message * msg)
{
    memset(msg, 0xCD, sizeof(st_ut_test_sub_message));
    constru_message_ut_test_sub_message(msg);
    msg->var_d_uint32.item[msg->var_d_uint32.count++] = 1000;
    msg->var_d_uint32.item[msg->var_d_uint32.count++] = 1001;
}

bool verify_ut_test_sub_message(st_ut_test_sub_message * msg)
{
    if (1000 == msg->var_d_uint32.item[0])
    {
        if (1001 == msg->var_d_uint32.item[1])
        {
            if (2 == msg->var_d_uint32.count)
            {
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

    {
        long value = 0;
        size_t offset = 0;
        encode_varint32(value, buf, &offset);
        assert(1 == offset);
        buf_len1 = offset;
        assert(0 == buf[0]);
        offset = 0;
        decode_varint32(buf, buf_len1, (uint32_t *)&value, &offset);
        assert(1 == offset);
        assert(0 == value);

        value = 1;
        offset = 0;
        encode_varint32(value, buf, &offset);
        assert(1 == offset);
        buf_len1 = offset;
        assert(0x01 == buf[0]);
        offset = 0;
        decode_varint32(buf, buf_len1, (uint32_t *)&value, &offset);
        assert(1 == offset);
        assert(1 == value);

        value = 128;
        offset = 0;
        encode_varint32(value, buf, &offset);
        assert(2 == offset);
        buf_len1 = offset;
        assert(0x80 == buf[0]);
        assert(0x01 == buf[1]);
        offset = 0;
        decode_varint32(buf, buf_len1, (uint32_t *)&value, &offset);
        assert(2 == offset);
        assert(128 == value);

        value = 65535;
        offset = 0;
        encode_varint32(value, buf, &offset);
        // print_buffer(buf, offset);
        assert(3 == offset);
        buf_len1 = offset;
        assert(0xFF == buf[0]);
        assert(0xFF == buf[1]);
        assert(0x03 == buf[2]);
        offset = 0;
        decode_varint32(buf, buf_len1, (uint32_t *)&value, &offset);
        assert(3 == offset);
        assert(65535 == value);

        value = 65536;
        offset = 0;
        encode_varint32(value, buf, &offset);
        // print_buffer(buf, offset);
        assert(3 == offset);
        buf_len1 = offset;
        assert(0x80 == buf[0]);
        assert(0x80 == buf[1]);
        assert(0x04 == buf[2]);
        offset = 0;
        decode_varint32(buf, buf_len1, (uint32_t *)&value, &offset);
        assert(3 == offset);
        assert(65536 == value);

        WORD64 value3 = 0xFFFFFFFF;
        offset = 0;
        encode_varint64(value3, buf, &offset);
        // print_buffer(buf, offset);
        assert(5 == offset);
        buf_len1 = offset;
        assert(0xFF == buf[0]);
        assert(0x0F == buf[4]);
        offset = 0;
        decode_varint64(buf, buf_len1, &value3, &offset);
        assert(5 == offset);
        assert(0xFFFFFFFF == value3);

        long value4 = -1;
        offset = 0;
        encode_varint32(value4, buf, &offset);
        print_buffer(buf, offset);
        buf_len1 = offset;
        offset = 0;
        decode_varint32(buf, buf_len1, (uint32_t *)&value4, &offset);
        assert(-1==value4);

        long long value5 = -1;
        offset = 0;
        encode_varint64(value5, buf, &offset);
        print_buffer(buf, offset);
        buf_len1 = offset;
        offset = 0;
        decode_varint64(buf, buf_len1, (uint64_t *)&value5, &offset);
        assert(-1==value4);
    }

    {
        long value = 1;
        assert(value == decode_zigzag32(encode_zigzag32(value)));
        value = 2;
        assert(value == decode_zigzag32(encode_zigzag32(value)));
        value = -1;
        assert(value == decode_zigzag32(encode_zigzag32(value)));
        value = -10000;
        assert(value == decode_zigzag32(encode_zigzag32(value)));

        long long value1 = 1;
        assert(value1 == decode_zigzag64(encode_zigzag64(value1)));
        value1 = 2;
        assert(value1 == decode_zigzag64(encode_zigzag64(value1)));
        value1 = -1;
        assert(value1 == decode_zigzag64(encode_zigzag64(value1)));
        value1 = -10000;
        assert(value1 == decode_zigzag64(encode_zigzag64(value1)));
    }

    {
        st_global_t var_global;
        constru_message_GLOBAL_T(&var_global);

        var_global.var_mdbSize = 1;  /* tag:1 */
        var_global.var_log_size = 2;  /* tag:2 */
        var_global.var_sync_time = 3;  /* tag:3 */
        var_global.var_auto_scale = 4;  /* tag:4 */
        var_global.var_db_alarm_thre = 5;  /* tag:5 */
        var_global.var_scale_limit = 6;  /* tag:6 */
        var_global.var_sub_ntf_switch = 7;  /* tag:7 */
        var_global.has_overload_cdn_tps_limit = TRUE;
        var_global.var_overload_cdn_tps_limit = 8;  /* tag:8 */
        var_global.has_overload_level1_cpu = TRUE;
        var_global.var_overload_level1_cpu = 9;  /* tag:9 */
        var_global.has_overload_level2_cpu = TRUE;
        var_global.var_overload_level2_cpu = 10;  /* tag:10 */
        var_global.has_overload_level3_cpu = TRUE;
        var_global.var_overload_level3_cpu = 11;  /* tag:11 */
        var_global.has_overload_level4_cpu = TRUE;
        var_global.var_overload_level4_cpu = 12;  /* tag:12 */
        var_global.has_overload_level5_cpu = TRUE;
        var_global.var_overload_level5_cpu = 13;  /* tag:13 */
        var_global.has_overload_level1_high_rate = TRUE;
        var_global.var_overload_level1_high_rate = 14;  /* tag:14 */
        var_global.has_overload_level2_high_rate = TRUE;
        var_global.var_overload_level2_high_rate = 15;  /* tag:15 */
        var_global.has_overload_level3_high_rate = TRUE;
        var_global.var_overload_level3_high_rate = 16;  /* tag:16 */
        var_global.has_overload_level4_high_rate = TRUE;
        var_global.var_overload_level4_high_rate = 17;  /* tag:17 */
        var_global.has_overload_level5_high_rate = TRUE;
        var_global.var_overload_level5_high_rate = 18;  /* tag:18 */
        var_global.has_overload_level1_low_rate = TRUE;
        var_global.var_overload_level1_low_rate = 19;  /* tag:19 */
        var_global.has_overload_level2_low_rate = TRUE;
        var_global.var_overload_level2_low_rate = 20;  /* tag:20 */
        var_global.has_overload_level3_low_rate = TRUE;
        var_global.var_overload_level3_low_rate = 21;  /* tag:21 */
        var_global.has_overload_level4_low_rate = TRUE;
        var_global.var_overload_level4_low_rate = 22;  /* tag:22 */
        var_global.has_overload_level5_low_rate = TRUE;
        var_global.var_overload_level5_low_rate = 23;  /* tag:23 */
        var_global.has_query_trigger_delete = TRUE;
        var_global.var_query_trigger_delete = 24;  /* tag:24 */

        buf_len2 = encode_message_GLOBAL_T(&var_global, buf, sizeof(buf));
        // print_buffer(buf, buf_len2);
        assert(57 == buf_len2);
        std::string pb_string = get_pb_string(buf, buf_len2, "cdb_ccc.proto", "zte.cdb.ccc.GLOBAL_T", _THIS_FILE, __LINE__);
        assert(0 == pb_string.length()); // 解码不正确是正常的
        BOOL bret = decode_message_GLOBAL_T(buf, buf_len2, &var_global);
        assert(TRUE == bret);

        assert(1 == var_global.var_mdbSize);  /* tag:1 */
        assert(2 == var_global.var_log_size);  /* tag:2 */
        assert(3 == var_global.var_sync_time);  /* tag:3 */
        assert(4 == var_global.var_auto_scale);  /* tag:4 */
        assert(5 == var_global.var_db_alarm_thre);  /* tag:5 */
        assert(6 == var_global.var_scale_limit);  /* tag:6 */
        assert(7 == var_global.var_sub_ntf_switch);  /* tag:7 */
        assert(TRUE == var_global.has_overload_cdn_tps_limit);
        assert(8 == var_global.var_overload_cdn_tps_limit);  /* tag:8 */
        assert(TRUE == var_global.has_overload_level1_cpu);
        assert(9 == var_global.var_overload_level1_cpu);  /* tag:9 */
        assert(TRUE == var_global.has_overload_level2_cpu);
        assert(10 == var_global.var_overload_level2_cpu);  /* tag:10 */
        assert(TRUE == var_global.has_overload_level3_cpu);
        assert(11 == var_global.var_overload_level3_cpu);  /* tag:11 */
        assert(TRUE == var_global.has_overload_level4_cpu);
        assert(12 == var_global.var_overload_level4_cpu);  /* tag:12 */
        assert(TRUE == var_global.has_overload_level5_cpu);
        assert(13 == var_global.var_overload_level5_cpu);  /* tag:13 */
        assert(TRUE == var_global.has_overload_level1_high_rate);
        assert(14 == var_global.var_overload_level1_high_rate);  /* tag:14 */
        assert(TRUE == var_global.has_overload_level2_high_rate);
        assert(15 == var_global.var_overload_level2_high_rate);  /* tag:15 */
        assert(TRUE == var_global.has_overload_level3_high_rate);
        assert(16 == var_global.var_overload_level3_high_rate);  /* tag:16 */
        assert(TRUE == var_global.has_overload_level4_high_rate);
        assert(17 == var_global.var_overload_level4_high_rate);  /* tag:17 */
        assert(TRUE == var_global.has_overload_level5_high_rate);
        assert(18 == var_global.var_overload_level5_high_rate);  /* tag:18 */
        assert(TRUE == var_global.has_overload_level1_low_rate);
        assert(19 == var_global.var_overload_level1_low_rate);  /* tag:19 */
        assert(TRUE == var_global.has_overload_level2_low_rate);
        assert(20 == var_global.var_overload_level2_low_rate);  /* tag:20 */
        assert(TRUE == var_global.has_overload_level3_low_rate);
        assert(21 == var_global.var_overload_level3_low_rate);  /* tag:21 */
        assert(TRUE == var_global.has_overload_level4_low_rate);
        assert(22 == var_global.var_overload_level4_low_rate);  /* tag:22 */
        assert(TRUE == var_global.has_overload_level5_low_rate);
        assert(23 == var_global.var_overload_level5_low_rate);  /* tag:23 */
        assert(TRUE == var_global.has_query_trigger_delete);
        assert(24 == var_global.var_query_trigger_delete);  /* tag:24 */
    }

    {
        st_tuple var_Tuple;
        memset(&var_Tuple, 0xCD, sizeof(st_tuple));
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
        buf_len2 = encode_message_Tuple(&var_Tuple, buf, sizeof(buf));
        assert(87 == buf_len2);
        // print_buffer(buf, buf_len2);
        std::string pb_string = get_pb_string(buf, buf_len2, "cdb.proto", "zte.cdb.Tuple", _THIS_FILE, __LINE__);
        assert(pb_string.length()>0);
        BOOL bret = decode_message_Tuple(buf, buf_len2, &var_Tuple);
        assert(TRUE == bret);
        assert(TRUE == var_Tuple.var_path.has_path_string);
        assert(0 == memcmp(var_Tuple.var_path.var_path_string.data, "/20", var_Tuple.var_path.var_path_string.length));
        assert(NULL != strstr(pb_string.c_str(), "path_string: \"/20\""));
        assert(TRUE == var_Tuple.has_version);
        assert(2000 == var_Tuple.var_version);
        assert(NULL != strstr(pb_string.c_str(), "version: 2000"));
        assert(TRUE == var_Tuple.has_ttl);
        assert(3000 == var_Tuple.var_ttl);
        assert(NULL != strstr(pb_string.c_str(), "ttl: 3000"));
        assert(var_Tuple.var_field.count == 2);
        assert(var_Tuple.var_field.item[0].var_fieldid == 1);
        assert(0 == memcmp(var_Tuple.var_field.item[0].var_value.data, "fawejlkrj1230940p1243lkjljfksldaj", var_Tuple.var_field.item[0].var_value.length));
        assert(NULL != strstr(pb_string.c_str(), "value: \"fawejlkrj1230940p1243lkjljfksldaj\""));
        assert(var_Tuple.var_field.item[1].var_fieldid == 2);
        assert(0 == memcmp(var_Tuple.var_field.item[1].var_value.data, "jflasjfu32ujfljsljkljkljljoiu", var_Tuple.var_field.item[1].var_value.length));
        assert(NULL != strstr(pb_string.c_str(), "value: \"jflasjfu32ujfljsljkljkljljoiu\""));
    }

    {
        memset(&var_AddRequest, 0xCD, sizeof(st_addrequest));
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

        buf_len1 = encode_message_AddRequest(&var_AddRequest, buf, sizeof(buf));
        // printf("buf_len:%zu\n", buf_len1);
        assert(160 == buf_len1);
        std::string pb_string = get_pb_string(buf, buf_len1, "cdb.proto", "zte.cdb.AddRequest", _THIS_FILE, __LINE__);
        assert(pb_string.length()>0);
        // print_buffer(buf, buf_len1);

        decode_message_AddRequest(buf, buf_len1, &var_AddRequest);
        assert(TRUE == var_AddRequest.var_tuple.item[0].var_path.has_path_string);
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_path.var_path_string.data, "/20", var_AddRequest.var_tuple.item[0].var_path.var_path_string.length));
	assert(NULL != strstr(pb_string.c_str(), "path_string: \"/20\""));
        assert(TRUE == var_AddRequest.var_tuple.item[0].has_version);
        assert(2000 == var_AddRequest.var_tuple.item[0].var_version);
	assert(NULL != strstr(pb_string.c_str(), "version: 2000"));
        assert(TRUE == var_AddRequest.var_tuple.item[0].has_ttl);
        assert(3000 == var_AddRequest.var_tuple.item[0].var_ttl);
	assert(NULL != strstr(pb_string.c_str(), "ttl: 3000"));

        assert(var_AddRequest.var_tuple.item[0].var_field.item[0].var_fieldid == 1);
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.data, "fawejlkrj1230940p1243lkjljfksldaj", var_AddRequest.var_tuple.item[0].var_field.item[0].var_value.length));
	assert(NULL != strstr(pb_string.c_str(), "value: \"fawejlkrj1230940p1243lkjljfksldaj\""));
        assert(var_AddRequest.var_tuple.item[0].var_field.item[1].var_fieldid == 2);
        assert(0 == memcmp(var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.data, "jflasjfu32ujfljsljkljkljljoiu", var_AddRequest.var_tuple.item[0].var_field.item[1].var_value.length));
	assert(NULL != strstr(pb_string.c_str(), "value: \"jflasjfu32ujfljsljkljkljljoiu\""));

        assert(var_AddRequest.var_identifiers.var_primary.item[0].var_id_type == 1);
        assert(var_AddRequest.var_identifiers.var_primary.item[0].var_value.count == 1);
        assert(0 == memcmp(var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].data, "465749674123167465431674613", var_AddRequest.var_identifiers.var_primary.item[0].var_value.item[0].length));
	assert(NULL != strstr(pb_string.c_str(), "value: \"465749674123167465431674613\""));

        assert(var_AddRequest.var_identifiers.var_non_primary.item[0].var_id_type == 1);
        assert(var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.count == 1);
        assert(0 == memcmp(var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].data, "465789461313213646461231324654", var_AddRequest.var_identifiers.var_non_primary.item[0].var_value.item[0].length));
	assert(NULL != strstr(pb_string.c_str(), "value: \"465789461313213646461231324654\""));
    }

    {
        st_ut_test_message msg;
        memset(&msg, 0xCD, sizeof(st_ut_test_message));

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(1000 == msg.var_d_uint32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_uint32: 1000"));
            assert(1001 == msg.var_d_uint32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_uint32: 1001"));
            assert(2 == msg.var_d_uint32.count);
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.var_r_uint32 = 10;
            msg.has_o_uint32 = TRUE;
            msg.var_o_uint32 = 11;
            msg.var_f_uint32.count = 2;
            msg.var_f_uint32.item[0] = 12;
            msg.var_f_uint32.item[1] = 13;

            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

            msg.var_pf_uint32.item[msg.var_pf_uint32.count++] = 14;
            msg.var_pf_uint32.item[msg.var_pf_uint32.count++] = 15;

            msg.var_pd_uint32.item[msg.var_pd_uint32.count++] = 16;
            msg.var_pd_uint32.item[msg.var_pd_uint32.count++] = 17;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size3 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size3);
            std::string pb_string = get_pb_string(buf, size3, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(95 == size3);
            decode_message_ut_test_message(buf, size3, &msg);

            assert(10 == msg.var_r_uint32);
	    assert(NULL != strstr(pb_string.c_str(), "r_uint32: 10"));
            assert(TRUE == msg.has_o_uint32);
            assert(11 == msg.var_o_uint32);
	    assert(NULL != strstr(pb_string.c_str(), "o_uint32: 11"));
            assert(2 == msg.var_f_uint32.count);
            assert(12 == msg.var_f_uint32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_uint32: 12"));
            assert(13 == msg.var_f_uint32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_uint32: 13"));
            assert(1000 == msg.var_d_uint32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_uint32: 1000"));
            assert(1001 == msg.var_d_uint32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_uint32: 1001"));
            assert(14 == msg.var_pf_uint32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_uint32: 14"));
            assert(15 == msg.var_pf_uint32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_uint32: 15"));
            assert(16 == msg.var_pd_uint32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_uint32: 16"));
            assert(17 == msg.var_pd_uint32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_uint32: 17"));
        }

        for (int i = 0; i < 3; ++i)
        {
            BYTE buf[128];
            size_t offset = 0;
            WORD64 var_int64;

            encode_varint64(20360023315404117, buf, &offset);
            // print_buffer(buf, offset);
            buf_len1 = offset;
            offset = 0;
            decode_varint64(buf, buf_len1, &var_int64, &offset);
            assert(20360023315404117 == var_int64);

            constru_message_ut_test_message(&msg);

            msg.var_r_uint64 = 2447866062020153618;

            msg.has_o_uint64 = TRUE;
            msg.var_o_uint64 = 11;

            msg.var_f_uint64.count = 2;
            msg.var_f_uint64.item[0] = 12;
            msg.var_f_uint64.item[1] = 13;

            msg.var_pf_uint64.item[msg.var_pf_uint64.count++] = 14;
            msg.var_pf_uint64.item[msg.var_pf_uint64.count++] = 15;

            msg.var_d_uint64.item[msg.var_d_uint64.count++] = 1001;
            msg.var_d_uint64.item[msg.var_d_uint64.count++] = 1002;


            msg.var_pd_uint64.item[msg.var_pd_uint64.count++] = 16;
            msg.var_pd_uint64.item[msg.var_pd_uint64.count++] = 17;

            char value12[] = "string";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size2);
            // printf("----------------------------------------");
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(76 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(2447866062020153618 == msg.var_r_uint64);
	    assert(NULL != strstr(pb_string.c_str(), "r_uint64: 2447866062020153618"));
            assert(TRUE == msg.has_o_uint64);
            assert(11 == msg.var_o_uint64);
	    assert(NULL != strstr(pb_string.c_str(), "o_uint64: 11"));
            assert(2 == msg.var_f_uint64.count);
            assert(12 == msg.var_f_uint64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_uint64: 12"));
            assert(13 == msg.var_f_uint64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_uint64: 13"));
            assert(1001 == msg.var_d_uint64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_uint64: 1001"));
            assert(1002 == msg.var_d_uint64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_uint64: 1002"));
            assert(14 == msg.var_pf_uint64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_uint64: 14"));
            assert(15 == msg.var_pf_uint64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_uint64: 15"));
            assert(16 == msg.var_pd_uint64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_uint64: 16"));
            assert(17 == msg.var_pd_uint64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_uint64: 17"));
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.var_r_fixed32 = 10;
            msg.has_o_fixed32 = TRUE;
            msg.var_o_fixed32 = 11;
            msg.var_f_fixed32.count = 2;
            msg.var_f_fixed32.item[0] = 12;
            msg.var_f_fixed32.item[1] = 13;

            msg.var_d_fixed32.item[msg.var_d_fixed32.count++] = 1000;
            msg.var_d_fixed32.item[msg.var_d_fixed32.count++] = 1001;

            msg.var_pf_fixed32.item[msg.var_pf_fixed32.count++] = 14;
            msg.var_pf_fixed32.item[msg.var_pf_fixed32.count++] = 15;

            msg.var_pd_fixed32.item[msg.var_pd_fixed32.count++] = 16;
            msg.var_pd_fixed32.item[msg.var_pd_fixed32.count++] = 17;
            msg.var_pd_fixed32.item[msg.var_pd_fixed32.count++] = -1;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(131 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(10 == msg.var_r_fixed32);
	    assert(NULL != strstr(pb_string.c_str(), "r_fixed32: 10"));
            assert(TRUE == msg.has_o_fixed32);
            assert(11 == msg.var_o_fixed32);
	    assert(NULL != strstr(pb_string.c_str(), "o_fixed32: 11"));
            assert(2 == msg.var_f_fixed32.count);
            assert(12 == msg.var_f_fixed32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_fixed32: 12"));
            assert(13 == msg.var_f_fixed32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_fixed32: 13"));
            assert(1000 == msg.var_d_fixed32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_fixed32: 1000"));
            assert(1001 == msg.var_d_fixed32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_fixed32: 1001"));
            assert(14 == msg.var_pf_fixed32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_fixed32: 14"));
            assert(15 == msg.var_pf_fixed32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_fixed32: 15"));
            assert(16 == msg.var_pd_fixed32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_fixed32: 16"));
            assert(17 == msg.var_pd_fixed32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_fixed32: 17"));
            assert(0xFFFFFFFF == msg.var_pd_fixed32.item[2]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_fixed32: 4294967295"));
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.var_r_fixed64 = 10;
            msg.has_o_fixed64 = TRUE;
            msg.var_o_fixed64 = 11;
            msg.var_f_fixed64.count = 2;
            msg.var_f_fixed64.item[0] = 12;
            msg.var_f_fixed64.item[1] = 13;
            msg.var_f_fixed64.item[2] = -13;

            msg.var_d_fixed64.item[msg.var_d_fixed64.count++] = 1000;
            msg.var_d_fixed64.item[msg.var_d_fixed64.count++] = 1001;

            msg.var_pf_fixed64.item[msg.var_pf_fixed64.count++] = 14;
            msg.var_pf_fixed64.item[msg.var_pf_fixed64.count++] = 15;

            msg.var_pd_fixed64.item[msg.var_pd_fixed64.count++] = 16;
            msg.var_pd_fixed64.item[msg.var_pd_fixed64.count++] = 17;
            msg.var_pd_fixed64.item[msg.var_pd_fixed64.count++] = -1;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // printf("size2:%zu\n", size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(171 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(10 == msg.var_r_fixed64);
	    assert(NULL != strstr(pb_string.c_str(), "r_fixed64: 10"));
            assert(TRUE == msg.has_o_fixed64);
            assert(11 == msg.var_o_fixed64);
	    assert(NULL != strstr(pb_string.c_str(), "o_fixed64: 11"));
            assert(2 == msg.var_f_fixed64.count);
            assert(12 == msg.var_f_fixed64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_fixed64: 12"));
            assert(13 == msg.var_f_fixed64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_fixed64: 13"));
            assert(1000 == msg.var_d_fixed64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_fixed64: 1000"));
            assert(1001 == msg.var_d_fixed64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_fixed64: 1001"));
            assert(14 == msg.var_pf_fixed64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_fixed64: 14"));
            assert(15 == msg.var_pf_fixed64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_fixed64: 15"));
            assert(16 == msg.var_pd_fixed64.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_fixed64: 16"));
            assert(17 == msg.var_pd_fixed64.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_fixed64: 17"));
            assert(0xFFFFFFFFFFFFFFFF == msg.var_pd_fixed64.item[2]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_fixed64: 18446744073709551615"));
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.var_r_bool = TRUE;
            msg.has_o_bool = TRUE;
            msg.var_o_bool = FALSE;
            msg.var_f_bool.count = 2;
            msg.var_f_bool.item[0] = TRUE;
            msg.var_f_bool.item[1] = FALSE;

            msg.var_d_bool.item[msg.var_d_bool.count++] = TRUE;
            msg.var_d_bool.item[msg.var_d_bool.count++] = FALSE;

            msg.var_pf_bool.item[msg.var_pf_bool.count++] = TRUE;
            msg.var_pf_bool.item[msg.var_pf_bool.count++] = FALSE;

            msg.var_pd_bool.item[msg.var_pd_bool.count++] = TRUE;
            msg.var_pd_bool.item[msg.var_pd_bool.count++] = FALSE;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // printf("size2:%zu\n", size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(100 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(TRUE == msg.var_r_bool);
	    assert(NULL != strstr(pb_string.c_str(), "r_bool: true"));
            assert(TRUE == msg.has_o_bool);
            assert(FALSE == msg.var_o_bool);
	    assert(NULL != strstr(pb_string.c_str(), "o_bool: false"));
            assert(2 == msg.var_f_bool.count);
            assert(TRUE == msg.var_f_bool.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_bool: true"));
            assert(FALSE == msg.var_f_bool.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_bool: false"));
            assert(TRUE == msg.var_d_bool.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_bool: true"));
            assert(FALSE == msg.var_d_bool.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_bool: false"));
            assert(TRUE == msg.var_pf_bool.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_bool: true"));
            assert(FALSE == msg.var_pf_bool.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_bool: false"));
            assert(TRUE == msg.var_pd_bool.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_bool: true"));
            assert(FALSE == msg.var_pd_bool.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_bool: false"));
        }

        for (int i = 0; i < 3; ++i)
        {
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

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(112 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(0 == memcmp(msg.var_r_string.data, string1, msg.var_r_string.length));
	    assert(NULL != strstr(pb_string.c_str(), "r_string: \"string1\""));
            assert(TRUE == msg.has_o_string);
            assert(0 == memcmp(msg.var_o_string.data, string2, msg.var_o_string.length));
	    assert(NULL != strstr(pb_string.c_str(), "o_string: \"string2\""));
            assert(2 == msg.var_f_string.count);
            assert(0 == memcmp(msg.var_f_string.item[0].data, string3, msg.var_f_string.item[0].length));
	    assert(NULL != strstr(pb_string.c_str(), "f_string: \"string3\""));
            assert(0 == memcmp(msg.var_f_string.item[1].data, string4, msg.var_f_string.item[1].length));
	    assert(NULL != strstr(pb_string.c_str(), "f_string: \"string4\""));
            assert(0 == memcmp(msg.var_d_string.item[0].data, string5, msg.var_d_string.item[0].length));
	    assert(NULL != strstr(pb_string.c_str(), "d_string: \"string5\""));
            assert(0 == memcmp(msg.var_d_string.item[1].data, string6, msg.var_d_string.item[1].length));
	    assert(NULL != strstr(pb_string.c_str(), "d_string: \"string6\""));
        }

        for (int i = 0; i < 3; ++i)
        {
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

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(106 == size2);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(0 == memcmp(msg.var_r_bytes.data, bytes1, msg.var_r_bytes.length));
	    assert(NULL != strstr(pb_string.c_str(), "r_bytes: \"bytes1\""));
            assert(TRUE == msg.has_o_bytes);
            assert(0 == memcmp(msg.var_o_bytes.data, bytes2, msg.var_o_bytes.length));
	    assert(NULL != strstr(pb_string.c_str(), "o_bytes: \"bytes2\""));
            assert(2 == msg.var_f_bytes.count);
            assert(0 == memcmp(msg.var_f_bytes.item[0].data, bytes3, msg.var_f_bytes.item[0].length));
	    assert(NULL != strstr(pb_string.c_str(), "f_bytes: \"bytes3\""));
            assert(0 == memcmp(msg.var_f_bytes.item[1].data, bytes4, msg.var_f_bytes.item[1].length));
	    assert(NULL != strstr(pb_string.c_str(), "f_bytes: \"bytes4\""));
            assert(0 == memcmp(msg.var_d_bytes.item[0].data, bytes5, msg.var_d_bytes.item[0].length));
	    assert(NULL != strstr(pb_string.c_str(), "d_bytes: \"bytes5\""));
            assert(0 == memcmp(msg.var_d_bytes.item[1].data, bytes6, msg.var_d_bytes.item[1].length));
	    assert(NULL != strstr(pb_string.c_str(), "d_bytes: \"bytes6\""));
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.var_r_enum = CLIENT_M;
            msg.has_o_enum = TRUE;
            msg.var_o_enum = SERVER_M;
            msg.var_f_enum.count = 2;
            msg.var_f_enum.item[0] = CLIENT_M;
            msg.var_f_enum.item[1] = SERVER_M;

            msg.var_d_enum.item[msg.var_d_enum.count++] = CLIENT_M;
            msg.var_d_enum.item[msg.var_d_enum.count++] = SERVER_M;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size2);
            // printf("size2:%zu\n", size2);
            assert(90 == size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            decode_message_ut_test_message(buf, size2, &msg);

            assert(CLIENT_M == msg.var_r_enum);
	    assert(NULL != strstr(pb_string.c_str(), "r_enum: CLIENT"));
            assert(TRUE == msg.has_o_enum);
            assert(SERVER_M == msg.var_o_enum);
	    assert(NULL != strstr(pb_string.c_str(), "o_enum: SERVER"));
            assert(2 == msg.var_f_enum.count);
            assert(CLIENT_M == msg.var_f_enum.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_enum: CLIENT"));
            assert(SERVER_M == msg.var_f_enum.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_enum: SERVER"));
            assert(CLIENT_M == msg.var_d_enum.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_enum: CLIENT"));
            assert(SERVER_M == msg.var_d_enum.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_enum: SERVER"));
        }

        for (int i = 0; i < 3; ++i)
        {
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

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = sizeof(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = sizeof(value12);
            msg.var_r_enum = CLIENT_M;

            size_t size2 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size2);
            // printf("size2:%zu\n", size2);
            assert(128 == size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            BOOL bret;
            for (size_t size3 = 0; size3 < size2 - 1; ++size3)
            {
                bret = decode_message_ut_test_message(buf, size3, &msg);
                // printf("size=%lu\n", size3);
                switch (size3)
                {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 10:
                case 20:
                case 22:
                case 23:
                case 47:
                case 71:
                case 73:
                case 74:
                case 83:
                case 92:
                case 101:
                case 110:
                case 119:
                    assert(TRUE == bret);
                    break;
                default:
                    assert(FALSE == bret);
                    break;
                }
            }
            bret = decode_message_ut_test_message(buf, size2, &msg);
            assert(TRUE == bret);

            assert(verify_ut_test_sub_message(&(msg.var_r_message)));
            assert(TRUE == msg.has_o_message);
            assert(verify_ut_test_sub_message(&(msg.var_o_message)));
            assert(2 == msg.var_f_message.count);
            assert(verify_ut_test_sub_message(&(msg.var_f_message.item[0])));
            assert(verify_ut_test_sub_message(&(msg.var_f_message.item[1])));
            assert(verify_ut_test_sub_message(&(msg.var_d_message.item[0])));
            assert(verify_ut_test_sub_message(&(msg.var_d_message.item[1])));
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.has_o_int32 = TRUE;
            msg.var_o_int32 = 0;
            msg.var_f_int32.count = 2;
            msg.var_f_int32.item[0] = -1;
            msg.var_f_int32.item[1] = 13;
            msg.var_d_int32.item[msg.var_d_int32.count++] = 1000;
            msg.var_d_int32.item[msg.var_d_int32.count++] = -1001;
            msg.var_pf_int32.item[msg.var_pf_int32.count++] = 14;
            msg.var_pf_int32.item[msg.var_pf_int32.count++] = -15;
            msg.var_pd_int32.item[msg.var_pd_int32.count++] = 16;
            msg.var_pd_int32.item[msg.var_pd_int32.count++] = -17;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size3 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size3);
            // printf("size:%lu\n", size3);
            std::string pb_string = get_pb_string(buf, size3, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(117 == size3);
            decode_message_ut_test_message(buf, size3, &msg);

            assert(TRUE == msg.has_o_int32);
            assert(0 == msg.var_o_int32);
	    assert(NULL != strstr(pb_string.c_str(), "o_int32: 0"));
            assert(2 == msg.var_f_int32.count);
            assert(-1 == msg.var_f_int32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "f_int32: -1"));
            assert(13 == msg.var_f_int32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "f_int32: 13"));
            assert(1000 == msg.var_d_int32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "d_int32: 1000"));
            assert(-1001 == msg.var_d_int32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "d_int32: -1001"));
            assert(14 == msg.var_pf_int32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_int32: 14"));
            assert(-15 == msg.var_pf_int32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pf_int32: -15"));
            assert(16 == msg.var_pd_int32.item[0]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int32: 16"));
            assert(-17 == msg.var_pd_int32.item[1]);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int32: -17"));
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.has_o_float = TRUE;
            msg.var_o_float = 0.0;
            msg.var_f_float.count = 2;
            msg.var_f_float.item[0] = -1.2;
            msg.var_f_float.item[1] = 13.3;
            msg.var_d_float.item[msg.var_d_float.count++] = 1000.4;
            msg.var_d_float.item[msg.var_d_float.count++] = -1001.5;
            msg.var_pf_float.item[0] = 14.6;
            msg.var_pf_float.item[1] = -15.7;
            msg.var_pd_float.item[msg.var_pd_float.count++] = 16.8;
            msg.var_pd_float.item[msg.var_pd_float.count++] = -17.9;

            char value12[] = "u4ojlfsjalfjaio;sjfl";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size3 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size3);
            assert(116 == size3);
            std::string pb_string = get_pb_string(buf, size3, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            decode_message_ut_test_message(buf, size3, &msg);

            assert(TRUE == msg.has_o_float);
            assert((msg.var_o_float - 0.0) < 0.001);
            assert((msg.var_o_float - 0.0) > -0.001);
            assert(2 == msg.var_f_float.count);
            assert(fabs(msg.var_f_float.item[0] + 1.2) < 0.001);
            assert(fabs(msg.var_f_float.item[1] - 13.3) < 0.001);
            assert(fabs(msg.var_d_float.item[0] - 1000.4) < 0.001);
            assert(fabs(msg.var_d_float.item[1] + 1001.5) < 0.001);
            assert(fabs(msg.var_pf_float.item[0] - 14.6) < 0.001);
            assert(fabs(msg.var_pf_float.item[1] + 15.7) < 0.001);
            assert(fabs(msg.var_pd_float.item[0] - 16.8) < 0.001);
            assert(fabs(msg.var_pd_float.item[1] + 17.9) < 0.001);
        }

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_message(&msg);
            msg.has_o_int64 = TRUE;
            msg.var_o_int64 = 96;
            msg.var_f_int64.item[msg.var_f_int64.count++] = 97;
            msg.var_f_int64.item[msg.var_f_int64.count++] = -97;
            msg.var_d_int64.item[msg.var_d_int64.count++]  = 4300000000;
            msg.var_d_int64.item[msg.var_d_int64.count++]  = -4300000000;
            msg.var_pf_int64.item[msg.var_pf_int64.count++] = 4300000001;
            msg.var_pf_int64.item[msg.var_pf_int64.count++] = -4300000001;
            msg.var_pd_int64.item[msg.var_pd_int64.count++] = 4300000002;
            msg.var_pd_int64.item[msg.var_pd_int64.count++] = -4300000002;
            msg.var_pd_int64.item[msg.var_pd_int64.count++] = 1;
            msg.var_pd_int64.item[msg.var_pd_int64.count++] = -1;
            msg.var_pd_int64.item[msg.var_pd_int64.count++] = 9223372036854775807;
            msg.var_pd_int64.item[msg.var_pd_int64.count++] = -9223372036854775807;

            msg.has_o_double = TRUE;
            msg.var_o_double = 101.1;
            msg.var_f_double.item[msg.var_f_double.count++]  = 102.2;
            msg.var_f_double.item[msg.var_f_double.count++]  = -102.2;
            msg.var_d_double.item[msg.var_d_double.count++]  = 103.3;
            msg.var_d_double.item[msg.var_d_double.count++]  = -103.3;
            msg.var_pf_double.item[msg.var_pf_double.count++]  = 104.4;
            msg.var_pf_double.item[msg.var_pf_double.count++]  = -104.4;
            msg.var_pd_double.item[msg.var_pd_double.count++]  = 105.5;
            msg.var_pd_double.item[msg.var_pd_double.count++]  = -105.5;

            msg.has_o_sint32 = TRUE;
            msg.var_o_sint32 = 106;
            msg.var_f_sint32.item[msg.var_f_sint32.count++]  = 107;
            msg.var_f_sint32.item[msg.var_f_sint32.count++]  = -107;
            msg.var_d_sint32.item[msg.var_d_sint32.count++]  = 108;
            msg.var_d_sint32.item[msg.var_d_sint32.count++]  = -108;
            msg.var_pf_sint32.item[msg.var_pf_sint32.count++]  = 109;
            msg.var_pf_sint32.item[msg.var_pf_sint32.count++]  = -109;
            msg.var_pd_sint32.item[msg.var_pd_sint32.count++]  = 110;
            msg.var_pd_sint32.item[msg.var_pd_sint32.count++]  = -110;
            msg.var_pd_sint32.item[msg.var_pd_sint32.count++]  = 1;
            msg.var_pd_sint32.item[msg.var_pd_sint32.count++]  = -1;
            msg.var_pd_sint32.item[msg.var_pd_sint32.count++]  = 2147483647;
            msg.var_pd_sint32.item[msg.var_pd_sint32.count++]  = -2147483647;

            msg.has_o_sint64 = TRUE;
            msg.var_o_sint64 = 121;
            msg.var_f_sint64.item[msg.var_f_sint64.count++]  = 122;
            msg.var_f_sint64.item[msg.var_f_sint64.count++]  = -122;
            msg.var_d_sint64.item[msg.var_d_sint64.count++]  = 123;
            msg.var_d_sint64.item[msg.var_d_sint64.count++]  = -123;
            msg.var_pf_sint64.item[msg.var_pf_sint64.count++]  = 124;
            msg.var_pf_sint64.item[msg.var_pf_sint64.count++]  = -124;
            msg.var_pd_sint64.item[msg.var_pd_sint64.count++]  = 125;
            msg.var_pd_sint64.item[msg.var_pd_sint64.count++]  = -125;
            msg.var_pd_sint64.item[msg.var_pd_sint64.count++]  = 1;
            msg.var_pd_sint64.item[msg.var_pd_sint64.count++]  = -1;
            msg.var_pd_sint64.item[msg.var_pd_sint64.count++]  = 9223372036854775807;
            msg.var_pd_sint64.item[msg.var_pd_sint64.count++]  = -9223372036854775807;

            msg.has_o_sfixed32 = TRUE;
            msg.var_o_sfixed32 = 111;
            msg.var_f_sfixed32.item[msg.var_f_sfixed32.count++]  = 112;
            msg.var_f_sfixed32.item[msg.var_f_sfixed32.count++]  = -112;
            msg.var_d_sfixed32.item[msg.var_d_sfixed32.count++]  = 113;
            msg.var_d_sfixed32.item[msg.var_d_sfixed32.count++]  = -113;
            msg.var_pf_sfixed32.item[msg.var_pf_sfixed32.count++]  = 114;
            msg.var_pf_sfixed32.item[msg.var_pf_sfixed32.count++]  = -114;
            msg.var_pd_sfixed32.item[msg.var_pd_sfixed32.count++]  = 115;
            msg.var_pd_sfixed32.item[msg.var_pd_sfixed32.count++]  = -115;
            msg.var_pd_sfixed32.item[msg.var_pd_sfixed32.count++]  = 1;
            msg.var_pd_sfixed32.item[msg.var_pd_sfixed32.count++]  = -1;
            msg.var_pd_sfixed32.item[msg.var_pd_sfixed32.count++]  = 2147483647;
            msg.var_pd_sfixed32.item[msg.var_pd_sfixed32.count++]  = -2147483647;

            msg.has_o_sfixed64 = TRUE;
            msg.var_o_sfixed64 = 116;
            msg.var_f_sfixed64.item[msg.var_f_sfixed64.count++]  = 117;
            msg.var_f_sfixed64.item[msg.var_f_sfixed64.count++]  = -117;
            msg.var_d_sfixed64.item[msg.var_d_sfixed64.count++]  = 118;
            msg.var_d_sfixed64.item[msg.var_d_sfixed64.count++]  = -118;
            msg.var_pf_sfixed64.item[msg.var_pf_sfixed64.count++]  = 119;
            msg.var_pf_sfixed64.item[msg.var_pf_sfixed64.count++]  = -119;
            msg.var_pd_sfixed64.item[msg.var_pd_sfixed64.count++]  = 120;
            msg.var_pd_sfixed64.item[msg.var_pd_sfixed64.count++]  = -120;
            msg.var_pd_sfixed64.item[msg.var_pd_sfixed64.count++]  = 1;
            msg.var_pd_sfixed64.item[msg.var_pd_sfixed64.count++]  = -1;
            msg.var_pd_sfixed64.item[msg.var_pd_sfixed64.count++]  = 9223372036854775807;
            msg.var_pd_sfixed64.item[msg.var_pd_sfixed64.count++]  = -9223372036854775807;

            char value12[] = "string";
            msg.var_r_string.data = value12;
            msg.var_r_string.length = strlen(value12);
            msg.var_r_bytes.data = (BYTE *) value12;
            msg.var_r_bytes.length = strlen(value12);
            msg.var_r_message.var_d_uint32.count = 0;
            msg.var_r_enum = CLIENT_M;

            size_t size3 = encode_message_ut_test_message(&msg, buf, sizeof(buf));
            // print_buffer(buf, size3);
            std::string pb_string = get_pb_string(buf, size3, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(528 == size3);
            decode_message_ut_test_message(buf, size3, &msg);

            assert(msg.has_o_int64 == TRUE);
            assert(msg.var_o_int64 == 96);
	    assert(NULL != strstr(pb_string.c_str(), "o_int64: 96"));
            assert(msg.var_f_int64.count == 2);
            assert(msg.var_f_int64.item[0] == 97);
	    assert(NULL != strstr(pb_string.c_str(), "f_int64: 97"));
            assert(msg.var_f_int64.item[1] == -97);
	    assert(NULL != strstr(pb_string.c_str(), "f_int64: -97"));
            assert(msg.var_d_int64.count == 2);
            assert(msg.var_d_int64.item[0] == 4300000000);
	    assert(NULL != strstr(pb_string.c_str(), "d_int64: 4300000000"));
            assert(msg.var_d_int64.item[1] == -4300000000);
	    assert(NULL != strstr(pb_string.c_str(), "d_int64: -4300000000"));
            assert(msg.var_pf_int64.count == 2);
            assert(msg.var_pf_int64.item[0] == 4300000001);
	    assert(NULL != strstr(pb_string.c_str(), "pf_int64: 4300000001"));
            assert(msg.var_pf_int64.item[1] == -4300000001);
	    assert(NULL != strstr(pb_string.c_str(), "pf_int64: -4300000001"));
            assert(msg.var_pd_int64.count == 6);
            assert(msg.var_pd_int64.item[0] == 4300000002);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int64: 4300000002"));
            assert(msg.var_pd_int64.item[1] == -4300000002);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int64: -4300000002"));
            assert(msg.var_pd_int64.item[2] == 1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int64: 1"));
            assert(msg.var_pd_int64.item[3] == -1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int64: -1"));
            assert(msg.var_pd_int64.item[4] == 9223372036854775807);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int64: 9223372036854775807"));
            assert(msg.var_pd_int64.item[5] == -9223372036854775807);
	    assert(NULL != strstr(pb_string.c_str(), "pd_int64: -9223372036854775807"));

            assert(msg.has_o_double == TRUE);
            assert(msg.var_o_double == 101.1);
	    assert(NULL != strstr(pb_string.c_str(), "o_double: 101.1"));
            assert(msg.var_f_double.count == 2);
            assert(msg.var_f_double.item[0]  == 102.2);
	    assert(NULL != strstr(pb_string.c_str(), "f_double: 102.2"));
            assert(msg.var_f_double.item[1]  == -102.2);
	    assert(NULL != strstr(pb_string.c_str(), "f_double: -102.2"));
            assert(msg.var_d_double.count == 2);
            assert(msg.var_d_double.item[0]  == 103.3);
	    assert(NULL != strstr(pb_string.c_str(), "d_double: 103.3"));
            assert(msg.var_d_double.item[1]  == -103.3);
	    assert(NULL != strstr(pb_string.c_str(), "d_double: -103.3"));
            assert(msg.var_pf_double.count == 2);
            assert(msg.var_pf_double.item[0]  == 104.4);
	    assert(NULL != strstr(pb_string.c_str(), "pf_double: 104.4"));
            assert(msg.var_pf_double.item[1]  == -104.4);
	    assert(NULL != strstr(pb_string.c_str(), "pf_double: -104.4"));
            assert(msg.var_pd_double.count == 2);
            assert(msg.var_pd_double.item[0]  == 105.5);
	    assert(NULL != strstr(pb_string.c_str(), "pd_double: 105.5"));
            assert(msg.var_pd_double.item[1]  == -105.5);
	    assert(NULL != strstr(pb_string.c_str(), "pd_double: -105.5"));

            assert(msg.has_o_sint32 == TRUE);
            assert(msg.var_o_sint32 == 106);
	    assert(NULL != strstr(pb_string.c_str(), "o_sint32: 106"));
            assert(msg.var_f_sint32.count == 2);
            assert(msg.var_f_sint32.item[0]  == 107);
	    assert(NULL != strstr(pb_string.c_str(), "f_sint32: 107"));
            assert(msg.var_f_sint32.item[1]  == -107);
	    assert(NULL != strstr(pb_string.c_str(), "f_sint32: -107"));
            assert(msg.var_d_sint32.count == 2);
            assert(msg.var_d_sint32.item[0]  == 108);
	    assert(NULL != strstr(pb_string.c_str(), "d_sint32: 108"));
            assert(msg.var_d_sint32.item[1]  == -108);
	    assert(NULL != strstr(pb_string.c_str(), "d_sint32: -108"));
            assert(msg.var_pf_sint32.count == 2);
            assert(msg.var_pf_sint32.item[0]  == 109);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sint32: 109"));
            assert(msg.var_pf_sint32.item[1]  == -109);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sint32: -109"));
            assert(msg.var_pd_sint32.count == 6);
            assert(msg.var_pd_sint32.item[0]  == 110);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint32: 110"));
            assert(msg.var_pd_sint32.item[1]  == -110);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint32: -110"));
            assert(msg.var_pd_sint32.item[2]  == 1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint32: 1"));
            assert(msg.var_pd_sint32.item[3]  == -1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint32: -1"));
            assert(msg.var_pd_sint32.item[4]  == 2147483647);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint32: 2147483647"));
            assert(msg.var_pd_sint32.item[5]  == -2147483647);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint32: -2147483647"));

            assert(msg.has_o_sint64 == TRUE);
            assert(msg.var_o_sint64 == 121);
	    assert(NULL != strstr(pb_string.c_str(), "o_sint64: 121"));
            assert(msg.var_f_sint64.count == 2);
            assert(msg.var_f_sint64.item[0]  == 122);
	    assert(NULL != strstr(pb_string.c_str(), "f_sint64: 122"));
            assert(msg.var_f_sint64.item[1]  == -122);
	    assert(NULL != strstr(pb_string.c_str(), "f_sint64: -122"));
            assert(msg.var_d_sint64.count == 2);
            assert(msg.var_d_sint64.item[0]  == 123);
	    assert(NULL != strstr(pb_string.c_str(), "d_sint64: 123"));
            assert(msg.var_d_sint64.item[1]  == -123);
	    assert(NULL != strstr(pb_string.c_str(), "d_sint64: -123"));
            assert(msg.var_pf_sint64.count == 2);
            assert(msg.var_pf_sint64.item[0]  == 124);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sint64: 124"));
            assert(msg.var_pf_sint64.item[1]  == -124);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sint64: -124"));
            assert(msg.var_pd_sint64.count == 6);
            assert(msg.var_pd_sint64.item[0]  == 125);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint64: 125"));
            assert(msg.var_pd_sint64.item[1]  == -125);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint64: -125"));
            assert(msg.var_pd_sint64.item[2]  == 1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint64: 1"));
            assert(msg.var_pd_sint64.item[3]  == -1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint64: -1"));
            assert(msg.var_pd_sint64.item[4]  == 9223372036854775807);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint64: 9223372036854775807"));
            assert(msg.var_pd_sint64.item[5]  == -9223372036854775807);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sint64: -9223372036854775807"));

            assert(msg.has_o_sfixed32 == TRUE);
            assert(msg.var_o_sfixed32 == 111);
	    assert(NULL != strstr(pb_string.c_str(), "o_sfixed32: 111"));
            assert(msg.var_f_sfixed32.count == 2);
            assert(msg.var_f_sfixed32.item[0]  == 112);
	    assert(NULL != strstr(pb_string.c_str(), "f_sfixed32: 112"));
            assert(msg.var_f_sfixed32.item[1]  == -112);
	    assert(NULL != strstr(pb_string.c_str(), "f_sfixed32: -112"));
            assert(msg.var_d_sfixed32.count == 2);
            assert(msg.var_d_sfixed32.item[0]  == 113);
	    assert(NULL != strstr(pb_string.c_str(), "d_sfixed32: 113"));
            assert(msg.var_d_sfixed32.item[1]  == -113);
	    assert(NULL != strstr(pb_string.c_str(), "d_sfixed32: -113"));
            assert(msg.var_pf_sfixed32.count == 2);
            assert(msg.var_pf_sfixed32.item[0]  == 114);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sfixed32: 114"));
            assert(msg.var_pf_sfixed32.item[1]  == -114);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sfixed32: -114"));
            assert(msg.var_pd_sfixed32.count == 6);
            assert(msg.var_pd_sfixed32.item[0]  == 115);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed32: 115"));
            assert(msg.var_pd_sfixed32.item[1]  == -115);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed32: -115"));
            assert(msg.var_pd_sfixed32.item[2]  == 1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed32: 1"));
            assert(msg.var_pd_sfixed32.item[3]  == -1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed32: -1"));
            assert(msg.var_pd_sfixed32.item[4]  == 2147483647);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed32: 2147483647"));
            assert(msg.var_pd_sfixed32.item[5]  == -2147483647);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed32: -2147483647"));

            assert(msg.has_o_sfixed64 == TRUE);
            assert(msg.var_o_sfixed64 == 116);
	    assert(NULL != strstr(pb_string.c_str(), "o_sfixed64: 116"));
            assert(msg.var_f_sfixed64.count == 2);
            assert(msg.var_f_sfixed64.item[0]  == 117);
	    assert(NULL != strstr(pb_string.c_str(), "f_sfixed64: 117"));
            assert(msg.var_f_sfixed64.item[1]  == -117);
	    assert(NULL != strstr(pb_string.c_str(), "f_sfixed64: -117"));
            assert(msg.var_d_sfixed64.count == 2);
            assert(msg.var_d_sfixed64.item[0]  == 118);
	    assert(NULL != strstr(pb_string.c_str(), "d_sfixed64: 118"));
            assert(msg.var_d_sfixed64.item[1]  == -118);
	    assert(NULL != strstr(pb_string.c_str(), "d_sfixed64: -118"));
            assert(msg.var_pf_sfixed64.count == 2);
            assert(msg.var_pf_sfixed64.item[0]  == 119);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sfixed64: 119"));
            assert(msg.var_pf_sfixed64.item[1]  == -119);
	    assert(NULL != strstr(pb_string.c_str(), "pf_sfixed64: -119"));
            assert(msg.var_pd_sfixed64.count == 6);
            assert(msg.var_pd_sfixed64.item[0]  == 120);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed64: 120"));
            assert(msg.var_pd_sfixed64.item[1]  == -120);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed64: -120"));
            assert(msg.var_pd_sfixed64.item[2]  == 1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed64: 1"));
            assert(msg.var_pd_sfixed64.item[3]  == -1);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed64: -1"));
            assert(msg.var_pd_sfixed64.item[4]  == 9223372036854775807);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed64: 9223372036854775807"));
            assert(msg.var_pd_sfixed64.item[5]  == -9223372036854775807);
	    assert(NULL != strstr(pb_string.c_str(), "pd_sfixed64: -9223372036854775807"));
        }
    }

    {
        st_ut_test_sub_message msg;

        for (int i = 0; i < 3; ++i)
        {
            constru_message_ut_test_sub_message(&msg);
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
            msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

            size_t size2 = encode_message_ut_test_sub_message(&msg, buf, sizeof(buf));
            // printf("size2:%zu\n", size2);
            assert(6 == size2);
            std::string pb_string = get_pb_string(buf, size2, "cdb_ccc.proto", "zte.cdb.ccc.ut_test_sub_message", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(TRUE == decode_message_ut_test_sub_message(buf, size2, &msg));
            assert(1000 == msg.var_d_uint32.item[0]);
            assert(1001 == msg.var_d_uint32.item[1]);
        }
    }

    {
        st_http2appreq msg;
        char errinfo[256];
        errinfo[0] = '\0';
        // constru_message_ut_test_sub_message(&msg);
        // msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1000;
        // msg.var_d_uint32.item[msg.var_d_uint32.count++] = 1001;

        // size_t size2 = encode_message_ut_test_sub_message(&msg, buf, sizeof(buf));
        // printf("size2:%zu\n", size2);
        // assert(6 == size2);
        FILE *fp = fopen("http2appreq.bin", "r");
        if(NULL!=fp){
            size_t size2 = fread(buf, 1, sizeof(buf), fp);
            fclose(fp);
            std::string pb_string = get_pb_string(buf, size2, "Http2Proxy.Http2AppReq.proto", "http2proxy.Http2AppReq", _THIS_FILE, __LINE__);
            assert(pb_string.length()>0);
            assert(FALSE == decode_message_Http2AppReq_ex(buf, size2, &msg, errinfo, sizeof(errinfo)));
            printf("errinfo:[%s]\n", errinfo);
            // assert(TRUE == msg.has_url);
            // assert(42 == msg.var_url.length);
            // assert(0==memcmp(msg.var_url.data, "/nudr-dr/v1/application-data/pfds/1234_001", msg.var_url.length));
        }
    }

    printf("sizeof(st_addrequest): %zu\n", sizeof(st_addrequest));
    printf("sizeof(st_compountrequest): %zu\n", sizeof(st_compoundrequest));
    printf("sizeof(st_response): %zu\n", sizeof(st_response));
    printf("sizeof(st_tuple): %zu\n", sizeof(st_tuple));

    printf("%s Done.\n", argv[0]);
    return 0;
}
