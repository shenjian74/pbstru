// pbstru.cpp : 定义控制台应用程序的入口点。
//

#ifndef _BUILDTIME_
#define _BUILDTIME_ "build: 2018-11-28 11:10:36"
#endif

#if defined (_MSC_VER)
# define _CRT_SECURE_NO_WARNINGS
# pragma warning(disable: 4482)
#endif

#include <string>
#include <algorithm>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include "importer.h"

typedef const char *LPCSTR;
typedef char *LPSTR;
typedef bool BOOL;
#define TRUE 1
#define FALSE 0
#define EOS ('\0')

using namespace google::protobuf;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// 结构定义的前缀，在CDB中可定义为"cdb_"
const string struct_prefix = "st_";
const string struct_postfix = "";
// 结构名称是否需要小写
const bool is_struct_lowercase = true;
///////////////////////////////////////////////////////////////////////////////

const char __THIS_FILE__[] = "pbstru.cpp";
#ifdef _WIN32
const char path_sep = '\\';
#else
const char path_sep = '/';
#endif

typedef enum
{
    PS_SUCCESS,  // 0
    PS_FAIL,
    NO_SUCH_ITEM = 10,
    NO_MAX_COUNT_IN_FILE
} e_error_code;

static string& trim(string& text)
{
    char trim_chars[] = " \n\r\t";
    if(!text.empty())
    {
        text.erase(0, text.find_first_not_of(trim_chars));
        text.erase(text.find_last_not_of(trim_chars) + 1);
    }
    return text;
}

static string &tolower(string& text){
    for(auto it=text.begin(); it!=text.end(); it++){
        *it = tolower(*it);
    }
    return text;
}

static string &toupper(string& text){
    for(auto it=text.begin(); it!=text.end(); it++){
        *it = toupper(*it);
    }
    return text;
}

// 生成公共文件pbstru_comm.h和pbstru_comm.c
int gen_comm(const string &target_dir)
{
    int retcode = 0;
    string filename = target_dir + "include" + path_sep + "pbstru_comm.h";
    FILE *fp = fopen(filename.c_str(), "wt");
    if (NULL == fp)
    {
        printf("Cannot open file:%s for write.\n", filename.c_str());
        return 1;
    }
    fprintf(fp, "#ifndef __PBSTRU_COMM_H__\n");
    fprintf(fp, "#define __PBSTRU_COMM_H__\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <stdlib.h>\n");
    fprintf(fp, "#include <memory.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#define WIRE_TYPE_VARINT 0\n");
    fprintf(fp, "#define WIRE_TYPE_FIX64 1\n");
    fprintf(fp, "#define WIRE_TYPE_LENGTH_DELIMITED 2\n");
    fprintf(fp, "#define WIRE_TYPE_FIX32 5\n");
    fprintf(fp, "\n");
    fprintf(fp, "#ifdef _TEST_CODEC\n");
    fprintf(fp, "typedef unsigned char BYTE;\n");
    fprintf(fp, "typedef unsigned char *PBYTE;\n");
    fprintf(fp, "typedef char CHAR;\n");
    fprintf(fp, "typedef unsigned short WORD;\n");
    fprintf(fp, "typedef unsigned short WORD16;\n");
    fprintf(fp, "typedef unsigned long DWORD;\n");
    fprintf(fp, "typedef unsigned long WORD32;\n");
    fprintf(fp, "typedef long SWORD32;\n");
    fprintf(fp, "typedef char* LPSTR;\n");
    fprintf(fp, "typedef const char* LPCSTR;\n");
    fprintf(fp, "typedef unsigned long long WORD64;\n");
    fprintf(fp, "\n#ifndef BOOL\n");
    fprintf(fp, "#define BOOL int\n");
    fprintf(fp, "#define TRUE 1\n");
    fprintf(fp, "#define FALSE 0\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "\n");
    fprintf(fp, "#else\n");
    fprintf(fp, "/* 包含数据类型定义 */\n");
    fprintf(fp, "#include \"tulip.h\"\n");
    fprintf(fp, "\n");
    fprintf(fp, "#endif");
    fprintf(fp, "\n");
    fprintf(fp, "typedef struct {\n");
    fprintf(fp, "    char *data;\n");
    fprintf(fp, "    size_t length;\n");
    fprintf(fp, "} ps_string;\n");
    fprintf(fp, "\n");
    fprintf(fp, "typedef struct {\n");
    fprintf(fp, "    unsigned char *data;\n");
    fprintf(fp, "    size_t length;\n");
    fprintf(fp, "} ps_bytes;\n");
    fprintf(fp, "\n");
    fprintf(fp, "void encode_tag_byte(BYTE *buf, WORD tag, BYTE wire_type, size_t *offset);\n");
    fprintf(fp, "void parse_tag_byte(const BYTE* buf, WORD *field_num, BYTE *wire_type, size_t *offset);\n");
    fprintf(fp, "void deal_unknown_field(const BYTE wire_type, const BYTE* buf, size_t* offset);\n");
    fprintf(fp, "\n");
    fprintf(fp, "void *pbstru_malloc(size_t size);\n");
    fprintf(fp, "void pbstru_free(void *buf);\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* 对varint信息进行编码 */\n");
    fprintf(fp, "#define encode_varint(len, buf, offset) do{ \\\n");
    fprintf(fp, "    unsigned long long remain_len = len; \\\n");
    fprintf(fp, "    size_t iloop; \\\n");
    fprintf(fp, "    for (iloop = 0;; ++iloop) { \\\n");
    fprintf(fp, "        if ((remain_len >> 7) > 0) { \\\n");
    fprintf(fp, "            if (NULL != buf) { \\\n");
    fprintf(fp, "                buf[(*(offset)) + iloop] = ((BYTE)remain_len) | 0x80; \\\n");
    fprintf(fp, "            } \\\n");
    fprintf(fp, "            remain_len = remain_len >> 7; \\\n");
    fprintf(fp, "        } \\\n");
    fprintf(fp, "        else { \\\n");
    fprintf(fp, "            if (NULL != buf) { \\\n");
    fprintf(fp, "                buf[(*(offset)) + iloop] = (BYTE)remain_len; \\\n");
    fprintf(fp, "            } \\\n");
    fprintf(fp, "            break; \\\n");
    fprintf(fp, "        } \\\n");
    fprintf(fp, "    } \\\n");
    fprintf(fp, "    *(offset) += 1 + iloop; \\\n");
    fprintf(fp, "} while(0)\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* 对varint信息进行解码 */\n");
    fprintf(fp, "#define decode_varint(buf, value, offset) do{ \\\n");
    fprintf(fp, "    size_t iloop; \\\n");
    fprintf(fp, "    (*(value)) = 0; \\\n");
    fprintf(fp, "    for(iloop=0;;++iloop){ \\\n");
    fprintf(fp, "        (*(value)) += ((unsigned long long)((buf)[iloop] & 0x7F)) << (7*iloop); \\\n");
    fprintf(fp, "        if(0 == ((buf)[iloop] & 0x80)){ \\\n");
    fprintf(fp, "            break; \\\n");
    fprintf(fp, "        } \\\n");
    fprintf(fp, "    } \\\n");
    fprintf(fp, "    (*(offset)) += 1 + iloop; \\\n");
    fprintf(fp, "} while(0)\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n#endif\n\n/* end of file */\n\n");
    fclose(fp);

    filename = target_dir + "source" + path_sep + "pbstru_comm.c";
    fp = fopen(filename.c_str(), "wt");
    if (NULL == fp)
    {
        printf("Cannot open file:%s for write.\n", filename.c_str());
        return 1;
    }
    fprintf(fp, "#include \"pbstru_comm.h\"\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* 对tag信息进行编码 */\n");
    fprintf(fp, "void encode_tag_byte(BYTE *buf, WORD tag, BYTE wire_type, size_t *offset) { \n");
    fprintf(fp, "    if(tag < 16) { \n");
    fprintf(fp, "        if (NULL != buf) { \n");
    fprintf(fp, "            buf[*offset] = (BYTE)(tag << 3) | wire_type; \n");
    fprintf(fp, "        } \n");
    fprintf(fp, "        *offset += 1; \n");
    fprintf(fp, "    } else { \n");
    fprintf(fp, "        if (NULL != buf) { \n");
    fprintf(fp, "            buf[*offset] = ((BYTE)tag) | 0x80; \n");
    fprintf(fp, "            buf[(*offset) + 1] = ((BYTE)(tag >> 4) & 0x78) | wire_type; \n");
    fprintf(fp, "        } \n");
    fprintf(fp, "        *offset += 2; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n");
    fprintf(fp, "/* 对tag信息进行解码 */\n");
    fprintf(fp, "void parse_tag_byte(const BYTE* buf, WORD *field_num, BYTE *wire_type, size_t *offset){ \n");
    fprintf(fp, "    if((buf)[0] & 0x80) { \n");
    fprintf(fp, "        *field_num = ((buf)[0] & 0x7F) + ((buf)[1] >> 3) * 128; \n");
    fprintf(fp, "	 *wire_type = (buf)[1] & 0x07; \n");
    fprintf(fp, "	 (*(offset)) += 2; \n");
    fprintf(fp, "    } else { \n");
    fprintf(fp, "        *field_num = (buf)[0] >> 3; \n");
    fprintf(fp, "        *wire_type = (buf)[0] & 0x07; \n");
    fprintf(fp, "	 (*(offset)) += 1; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n");
    fprintf(fp, "/* 跳过不认识的字段，向前兼容用 */\n");
    fprintf(fp, "void deal_unknown_field(const BYTE wire_type, const BYTE* buf, size_t* offset) { \n");
    fprintf(fp, "    size_t tmp_field_len; \n");
    fprintf(fp, "    switch(wire_type){ \n");
    fprintf(fp, "    case WIRE_TYPE_VARINT: \n");
    fprintf(fp, "        decode_varint(buf, &tmp_field_len, offset); \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    case WIRE_TYPE_FIX64: \n");
    fprintf(fp, "        (*(offset)) += 8; \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    case WIRE_TYPE_LENGTH_DELIMITED: \n");
    fprintf(fp, "        decode_varint(buf, &tmp_field_len, offset); \n");
    fprintf(fp, "        (*(offset)) += tmp_field_len; \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    case WIRE_TYPE_FIX32: \n");
    fprintf(fp, "        (*(offset)) += 4; \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    default: \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "}\n");
    fprintf(fp, "/* end of file */\n\n");
    fclose(fp);

    return retcode;
}

LPSTR proto_filename;
FILE *fp_option = NULL;

// Get the max size of a repeated field.
static bool get_max_count(const string &message_name, const string &field_name, string& max_count)
{
    char str1[128];
    char str2[64];
    static string option_filename;
    string key = message_name + "." + field_name;

    if (NULL == fp_option)
    {
        option_filename = proto_filename;
        size_t pos = option_filename.rfind(".");
        if (pos == string::npos)
        {
            option_filename += ".options";
        }
        else
        {
            option_filename = option_filename.substr(0, pos) + ".options";
        }
        fp_option = fopen(option_filename.c_str(), "rt");
        if (NULL == fp_option)
        {
            printf("Error: [%s:%d] Cannot open file:%s for read.\n", __THIS_FILE__, __LINE__, option_filename.c_str());
            return(10);
        }
    }

    for (rewind(fp_option); ;)
    {
        char buffer[1024];
        if (NULL == fgets(buffer, sizeof(buffer), fp_option))
        {
            break;
        }
        if ('#' == buffer[0])
        {
            continue;
        }
        sscanf(buffer, "%s %s", str1, str2);
        if (0 == strcmp(str1, key.c_str()))
        {
            char *num_str = strstr(str2, "max_count:");
            if (NULL == num_str)
            {
                printf("Error: [%s:%d] Cannot read item:\"%s max_count:?\" from option file:[%s].\n",
                       __THIS_FILE__, __LINE__, key.c_str(), option_filename.c_str());
                return false;
            }
            else
            {
                max_count = string(num_str + strlen("max_count:"));
                trim(max_count);
                return true;
            }
        }
    }

    printf("Info: [%s:%d] Cannot read item:\"%s max_count:?\" from option file:[%s], It will be created dynamically.\n", __THIS_FILE__, __LINE__, key.c_str(), option_filename.c_str());
    // exit(NO_MAX_COUNT_IN_FILE);
    return false;
}

void get_struct_list_name(const FieldDescriptor *field, string &struct_list_name)
{
    string field_name_lower = field->name();
    tolower(field_name_lower);
    string field_containing_type_lower = field->containing_type()->name();
    tolower(field_containing_type_lower);
    string msg_enum_name;

    struct_list_name = field_name_lower + "_in_" + field_containing_type_lower;
    switch(field->type())
    {
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_UINT32:
        struct_list_name += string("_uint32");
        break;
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_UINT64:
        struct_list_name += string("_uint64");
        break;
    case FieldDescriptor::TYPE_BOOL:
        struct_list_name += string("_boolean");
        break;
    case FieldDescriptor::TYPE_STRING:
        struct_list_name += string("_string");
        break;
    case FieldDescriptor::TYPE_BYTES:
        struct_list_name += string("_buffer");
        break;
    case FieldDescriptor::TYPE_MESSAGE:
        msg_enum_name = field->message_type()->name();
        tolower(msg_enum_name);
        struct_list_name += string("_") + msg_enum_name;
        break;
    case FieldDescriptor::TYPE_ENUM:
        msg_enum_name = field->enum_type()->name();
        tolower(msg_enum_name);
        struct_list_name += string("_") + msg_enum_name;
        break;
    default:
        struct_list_name = "";
        break;
    }
}

/* Check the field is unlimit repeated field. */
bool is_dynamic_repeated(const FieldDescriptor *field)
{
    string max_count;
    bool is_dynamic = false;
    static map<string, bool> kv_store;
    map<string, bool>::iterator it;
    string containing_type_name = field->containing_type()->full_name();
    string field_name = field->name();

    it = kv_store.find(containing_type_name + "." + field_name);
    if (it != kv_store.end())
    {
        return it->second;
    }

    if(field->is_repeated())
    {
        // Cannot find max_count in option file
        if(false == get_max_count(containing_type_name, field_name, max_count))
        {
            is_dynamic = true;
        }
        kv_store[containing_type_name + "." + field_name] = is_dynamic;
    }
    return is_dynamic;
}

static void print_field_in_struct(FILE *fp, const FieldDescriptor *field)
{
    string field_name_upper = field->name();
    toupper(field_name_upper);
    string message_type_upper = field->containing_type()->name();
    toupper(message_type_upper);

    // fprintf(fp, "/* %s\n%s\n%s */\n", field->DebugString().c_str(), field->lowercase_name().c_str(), field->camelcase_name().c_str());
    // const OneofDescriptor* containing_oneof() const;

    if(field->is_optional())
    {
        fprintf(fp, "    BOOL has_%s;\n", field->name().c_str());
    }

    const FieldDescriptor::Type type = field->type();
    switch(type)
    {
    case FieldDescriptor::TYPE_FIXED32:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    DWORD var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_FIXED64:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    WORD64 var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_BOOL:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    BOOL var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_UINT32:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    DWORD var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_UINT64:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    WORD64 var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_STRING:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    ps_string var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_BYTES:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    ps_bytes var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
        }
        break;
    case FieldDescriptor::TYPE_MESSAGE:
    {
        string message_type_name = field->message_type()->name();
        if(is_struct_lowercase)
        {
            tolower(message_type_name);
        }
        string struct_name = struct_prefix + message_type_name + struct_postfix;

        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    %s var_%s;  /* tag:%d */\n", struct_name.c_str(), field->name().c_str(), field->number());
        }
    }
    break;
    case FieldDescriptor::TYPE_ENUM:
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    st_%s_list *var_%s;  /* tag:%d 链表头指针 */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
                fprintf(fp, "    st_%s_list *var_%s_tail;  /* 链表尾指针 */\n",
                        struct_list_name.c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n",
                        struct_list_name.c_str(), field->name().c_str(), field->number());
            }
        }
        else
        {
            fprintf(fp, "    enum_%s var_%s;  /* tag:%d */\n", field->enum_type()->name().c_str(), field->name().c_str(), field->number());
        }
        break;
    default:
        fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
        break;
    }
}

int gen_header(const Descriptor *desc, string &target_dir, map<string, string> &map_array_size)
{
    int retcode = 0;
    string name_lower = desc->name();
    tolower(name_lower);

    string struct_name;
    if(is_struct_lowercase)
    {
        struct_name = struct_prefix + name_lower + struct_postfix;
    }
    else
    {
        struct_name = struct_prefix + desc->name() + struct_postfix;
    }

    string filename = target_dir + "include" + path_sep + name_lower;
    filename += ".h";
    FILE *fp = fopen(filename.c_str(), "wt");
    if(NULL == fp)
    {
        printf("Cannot open file:%s for write.\n", filename.c_str());
        return 1;
    }

    string name_upper = desc->name();
    toupper(name_upper);
    fprintf(fp, "#ifndef __PBSTRU_%s_H__\n", name_upper.c_str());
    fprintf(fp, "#define __PBSTRU_%s_H__\n", name_upper.c_str());
    fprintf(fp, "\n#ifdef __cplusplus\n");
    fprintf(fp, "extern \"C\"{\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "\n#include \"pbstru_comm.h\"\n");

    set<string> headers;
    headers.clear();
    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(FieldDescriptor::TYPE_MESSAGE == field->type())
        {
            string str = field->message_type()->name();
            tolower(str);
            headers.insert(str);
        }
    }

    // 使用set去掉重复的头文件
    for(set<string>::iterator it=headers.begin(); it!=headers.end(); ++it)
    {
        fprintf(fp, "#include \"%s.h\"\n", it->c_str());
    }
    fprintf(fp, "\n/*\n%s*/\n", desc->DebugString().c_str());

    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(field->is_repeated())
        {
            string max_count;
            if(get_max_count(field->containing_type()->full_name(), field->name(), max_count))
            {
                map_array_size[field->containing_type()->name() + ":" + field->name()] = max_count;
            }
        }
    }

    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(FieldDescriptor::TYPE_ENUM == field->type())
        {
            const EnumDescriptor* enum_desc = field->enum_type();
            string enum_name_upper = enum_desc->name();
            toupper(enum_name_upper);
            fprintf(fp, "\n#ifndef ENUM_%s_DEFINED\n", enum_name_upper.c_str());
            fprintf(fp, "#define ENUM_%s_DEFINED\n", enum_name_upper.c_str());
            fprintf(fp, "typedef enum {\n");
            for(int j=0; j<enum_desc->value_count(); ++j)
            {
                fprintf(fp, "    %s_M = %d", enum_desc->value(j)->name().c_str(), enum_desc->value(j)->number());
                // 处理最后enum值时不加逗号
                if(j!=(enum_desc->value_count()-1))
                {
                    fprintf(fp, ",");
                }
                fprintf(fp, "\n");
            }
            fprintf(fp, "} enum_%s;\n", enum_desc->name().c_str());
            fprintf(fp, "#endif\n");
        }
    }

    // 各个list的结构定义
    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(field->is_repeated())
        {
            string struct_list_name;
			get_struct_list_name(field, struct_list_name);
            fprintf(fp, "\ntypedef struct _st_%s_list {\n", struct_list_name.c_str());

            if(is_dynamic_repeated(field))
            {
                fprintf(fp, "    struct _st_%s_list *next;\n", struct_list_name.c_str());
            }
            else
            {
                fprintf(fp, "    size_t count;\n");
            }

            switch(field->type())
            {
            case FieldDescriptor::TYPE_FIXED32:
            case FieldDescriptor::TYPE_UINT32:
                fprintf(fp, "    DWORD");
                break;
            case FieldDescriptor::TYPE_FIXED64:
            case FieldDescriptor::TYPE_UINT64:
                fprintf(fp, "    WORD64");
                break;
            case FieldDescriptor::TYPE_BOOL:
                fprintf(fp, "    BOOL");
                break;
            case FieldDescriptor::TYPE_STRING:
                fprintf(fp, "    ps_string");
                break;
            case FieldDescriptor::TYPE_BYTES:
                fprintf(fp, "    ps_bytes");
                break;
            case FieldDescriptor::TYPE_MESSAGE:
            {
                string message_type_name = field->message_type()->name();
                if(is_struct_lowercase)
                {
                    tolower(message_type_name);
                }
                string struct_name = struct_prefix + message_type_name + struct_postfix;
                fprintf(fp, "    %s", struct_name.c_str());
            }
            break;
            case FieldDescriptor::TYPE_ENUM:
                fprintf(fp, "    enum_%s", field->enum_type()->name().c_str());
                break;
            default:
                fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author for support.\n",
                        __THIS_FILE__, __LINE__, field->type_name());
                break;
            }
            if(is_dynamic_repeated(field))
            {
                fprintf(fp, " value;  /* tag:%d type:%s */\n", field->number(), field->type_name());
            }
            else
            {
                fprintf(fp, " item[%s];  /* tag:%d type:%s */\n",
                        map_array_size[field->containing_type()->name() + ":" + field->name()].c_str(),
                        field->number(), field->type_name());
            }

            fprintf(fp, "} st_%s_list;\n", struct_list_name.c_str());
        }
    }

    fprintf(fp, "\ntypedef struct _%s {\n", struct_name.c_str());
    for(int i=0; i<desc->field_count(); ++i)
    {
        print_field_in_struct(fp, desc->field(i));
    }
    fprintf(fp, "} %s;\n", struct_name.c_str());

    fprintf(fp, "\n/* construct msg when first use, call clear_message_*() internaly. */\n");
    fprintf(fp, "void constru_message_%s(%s *msg);\n", desc->name().c_str(), struct_name.c_str());
    fprintf(fp, "/* destruct msg */\n");
    fprintf(fp, "void destru_message_%s(%s *msg);\n", desc->name().c_str(), struct_name.c_str());

    fprintf(fp, "\n/* clear and reuse msg */\n");
    fprintf(fp, "void clear_message_%s(%s *msg);\n",
            desc->name().c_str(), struct_name.c_str());
    fprintf(fp, "size_t encode_message_%s(const %s* const msg, BYTE* const buf);\n",
            desc->name().c_str(), struct_name.c_str());
    fprintf(fp, "BOOL decode_message_%s(BYTE* const buf, const size_t buf_len, %s* const msg);\n",
            desc->name().c_str(), struct_name.c_str());

    fprintf(fp, "\n#ifdef __cplusplus\n");
    fprintf(fp, "}\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "\n#endif\n\n/* end of file */\n\n");
    fclose(fp);
    return retcode;
}

void alloc_new_repeated_item(FILE *fp, const Descriptor *desc, const FieldDescriptor *field, LPCSTR spaces)
{
	string struct_list_name;
	get_struct_list_name(field, struct_list_name);
    fprintf(fp, "%s            if(NULL == var_%s->var_%s){\n", spaces, desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                var_%s->var_%s = (st_%s_list *)pbstru_malloc(sizeof(st_%s_list));\n",
            spaces, desc->name().c_str(), field->name().c_str(), struct_list_name.c_str(), struct_list_name.c_str());
    fprintf(fp, "%s                if(NULL == var_%s->var_%s){\n", spaces, desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                    return FALSE;\n", spaces);
    fprintf(fp, "%s                } else {\n", spaces);
    fprintf(fp, "%s                    var_%s->var_%s_tail = var_%s->var_%s;\n",
            spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                    var_%s->var_%s_tail->next = NULL;\n",
            spaces, desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                }\n", spaces);
    fprintf(fp, "%s            } else {\n", spaces);
    fprintf(fp, "%s                st_%s_list *tmptr = (st_%s_list *)pbstru_malloc(sizeof(st_%s_list));\n",
            spaces, struct_list_name.c_str(), struct_list_name.c_str(), struct_list_name.c_str());
    fprintf(fp, "%s                if(NULL == tmptr){\n", spaces);
    fprintf(fp, "%s                    return FALSE;\n", spaces);
    fprintf(fp, "%s                } else {\n", spaces);
    fprintf(fp, "%s                    var_%s->var_%s_tail->next = tmptr;\n", spaces, desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                    var_%s->var_%s_tail = tmptr;\n", spaces, desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                    var_%s->var_%s_tail->next = NULL;\n", spaces, desc->name().c_str(), field->name().c_str());
    fprintf(fp, "%s                }\n", spaces);
    fprintf(fp, "%s            }\n", spaces);
    if(FieldDescriptor::TYPE_MESSAGE == field->type())
    {
        fprintf(fp, "%s            constru_message_%s(&(var_%s->var_%s_tail->value));\n",
                spaces, field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
    }

}

void print_clear_message(FILE *fp, const Descriptor *desc, bool init, const map<string,string> &map_array_size)
{
    int it_count = 0;

    // 必要时打印临时变量
    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(field->is_repeated())
        {
            if(!is_dynamic_repeated(field) && FieldDescriptor::TYPE_MESSAGE == field->type())
            {
                fprintf(fp, "    size_t i = 0;\n");
                break;
            }
        }
    }

    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(field->is_repeated())
        {
			string struct_list_name;
			get_struct_list_name(field, struct_list_name);

            if(is_dynamic_repeated(field))
            {
                // Recursive clearing message
                if(false == init)
                {
                    fprintf(fp, "    st_%s_list *it%d=var_%s->var_%s;\n",
                            struct_list_name.c_str(), ++it_count, desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "    for(; NULL!=it%d; ) {\n", it_count);
                    fprintf(fp, "        st_%s_list *it_tmp = it%d;\n", struct_list_name.c_str(), it_count);
                    fprintf(fp, "        it%d=it%d->next;\n", it_count, it_count);
                    if(FieldDescriptor::TYPE_MESSAGE == field->type())
                    {
                        fprintf(fp, "        clear_message_%s(&(it_tmp->value));\n", field->message_type()->name().c_str());
                    }
                    fprintf(fp, "        pbstru_free((void *)it_tmp);\n");
                    fprintf(fp, "    }\n");
                }
                fprintf(fp, "    var_%s->var_%s = NULL;\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "    var_%s->var_%s_tail = NULL;\n\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                if(init)
                {
                    if(FieldDescriptor::TYPE_MESSAGE == field->type())
                    {
                        fprintf(fp, "    for(i=0; i<%s; ++i){\n",
                                map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                        fprintf(fp, "        constru_message_%s(&(var_%s->var_%s.item[i]));\n",
                                field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                        fprintf(fp, "    }\n");
                    }
                }
                else
                {
                    if(FieldDescriptor::TYPE_MESSAGE == field->type())
                    {
                        fprintf(fp, "    for(i=0; i<var_%s->var_%s.count; ++i){\n", desc->name().c_str(), field->name().c_str());
                        fprintf(fp, "        clear_message_%s(&(var_%s->var_%s.item[i]));\n",
                                field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                        fprintf(fp, "    }\n");
                    }
                }
                fprintf(fp, "    var_%s->var_%s.count = 0;\n", desc->name().c_str(), field->name().c_str());
            }
        }
        else
        {
            if(init)
            {
                if(FieldDescriptor::TYPE_MESSAGE == field->type())
                {
                    fprintf(fp, "    constru_message_%s(&(var_%s->var_%s));\n",
                            field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                char spaces[] = "          ";
                if(field->is_optional())
                {
                    fprintf(fp, "    if(TRUE == var_%s->has_%s){\n", desc->name().c_str(), field->name().c_str());
                    spaces[8] = '\0';
                }
                else
                {
                    spaces[4] = '\0';
                }
                switch(field->type())
                {
                case FieldDescriptor::TYPE_FIXED32:
                case FieldDescriptor::TYPE_FIXED64:
                case FieldDescriptor::TYPE_UINT32:
                case FieldDescriptor::TYPE_UINT64:
                    fprintf(fp, "%svar_%s->var_%s = 0;\n", spaces, desc->name().c_str(), field->name().c_str());
                    break;
                case FieldDescriptor::TYPE_BOOL:
                    fprintf(fp, "%svar_%s->var_%s = FALSE;\n", spaces, desc->name().c_str(), field->name().c_str());
                    break;
                case FieldDescriptor::TYPE_ENUM:
                    break;
                case FieldDescriptor::TYPE_STRING:
                case FieldDescriptor::TYPE_BYTES:
                    fprintf(fp, "%svar_%s->var_%s.length = 0;\n", spaces, desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "%svar_%s->var_%s.data = NULL;\n", spaces, desc->name().c_str(), field->name().c_str());
                    break;
                case FieldDescriptor::TYPE_MESSAGE:
                    fprintf(fp, "%sclear_message_%s(&(var_%s->var_%s));\n", spaces,
                            field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                    break;
                default:
                    fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
                    break;
                }
                if(field->is_optional())
                {
                    fprintf(fp, "    }\n");
                }
            }
            if(field->is_optional())
            {
                fprintf(fp, "    var_%s->has_%s = FALSE;\n", desc->name().c_str(), field->name().c_str());
            }
        }
    }
}

int gen_source(const Descriptor *desc, string &target_dir, const map<string,string> &map_array_size)
{
    int retcode = 0;
    string name_lower = desc->name();
    tolower(name_lower);

    string struct_name;
    if(is_struct_lowercase)
    {
        struct_name = struct_prefix + name_lower + struct_postfix;
    }
    else
    {
        struct_name = struct_prefix + desc->name().c_str() + struct_postfix;
    }

    string filename = target_dir + "source" + path_sep + name_lower;
    filename += ".c";
    FILE *fp = fopen(filename.c_str(), "wt");
    if(NULL == fp)
    {
        printf("Cannot open file:%s for write.\n", filename.c_str());
        return 1;
    }

    fprintf(fp, "#include \"%s.h\"\n", name_lower.c_str());

    fprintf(fp, "\n");
    fprintf(fp, "/* lint -save -e701 -e647 */\n");
    fprintf(fp, "\n");

    ////////////////////////////////////////
    // clear function
    fprintf(fp, "void constru_message_%s(%s *var_%s){\n", desc->name().c_str(), struct_name.c_str(), desc->name().c_str());
    print_clear_message(fp, desc, true, map_array_size);
    fprintf(fp, "}\n");

    ////////////////////////////////////////
    fprintf(fp, "\nvoid destru_message_%s(%s* var_%s){\n",
            desc->name().c_str(), struct_name.c_str(), desc->name().c_str());
    fprintf(fp, "    clear_message_%s(var_%s);\n", desc->name().c_str(), desc->name().c_str());
    fprintf(fp, "}\n");

    // clear function
    fprintf(fp, "\nvoid clear_message_%s(%s* var_%s){\n",
            desc->name().c_str(), struct_name.c_str(), desc->name().c_str());
    print_clear_message(fp, desc, false, map_array_size);
    fprintf(fp, "}\n");

    ////////////////////////////////////////
    fprintf(fp, "\nsize_t encode_message_%s(const %s* var_%s, BYTE* buf){\n",
            desc->name().c_str(), struct_name.c_str(), desc->name().c_str());
    // 有嵌套message的时候会用到编码长度
    for(int i=0; i<desc->field_count(); ++i)
    {
        if(FieldDescriptor::TYPE_MESSAGE == desc->field(i)->type())
        {
            fprintf(fp, "    size_t encode_buf_len;\n");
            break;
        }
    }
    // 有repeat字段的时候会用到循环变量
    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        if(field->is_repeated())
        {
            if(!is_dynamic_repeated(field))
            {
                fprintf(fp, "    size_t i;\n");
                break;
            }
        }
    }
    fprintf(fp, "    size_t offset = 0;\n");

    int it_count = 0;
    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
		string struct_list_name;
		get_struct_list_name(field, struct_list_name);
        if(is_dynamic_repeated(field))
        {
            fprintf(fp, "    st_%s_list *it%d=var_%s->var_%s;\n",
                    struct_list_name.c_str(), ++it_count, desc->name().c_str(), field->name().c_str());
        }
    }

    // 逐个字段encode
    it_count = 0;
    for(int i=0; i<desc->field_count(); ++i)
    {
        string prefix_spaces = "    ";
        const FieldDescriptor *field = desc->field(i);

        fprintf(fp, "\n");
        if(field->is_repeated())
        {
            if(is_dynamic_repeated(field))
            {
                it_count += 1;
                if(field->is_packed())
                {
                    switch(field->type())
                    {
                    case FieldDescriptor::TYPE_FIXED32:
                        fprintf(fp, "    encode_tag_byte(buf, %d, WIRE_TYPE_FIX32, &offset);\n", field->number());
                        break;
                    case FieldDescriptor::TYPE_FIXED64:
                        fprintf(fp, "    encode_tag_byte(buf, %d, WIRE_TYPE_FIX64, &offset);\n", field->number());
                        break;
                    case FieldDescriptor::TYPE_BOOL:
                    case FieldDescriptor::TYPE_UINT32:
                    case FieldDescriptor::TYPE_UINT64:
                    case FieldDescriptor::TYPE_ENUM:
                        fprintf(fp, "    encode_tag_byte(buf, %d, WIRE_TYPE_VARINT, &offset);\n", field->number());
                        break;
                    default:
                        break;
                    }
                    fprintf(fp, "    i = 0;  /* i复用为元素个数使用 */\n");
                    fprintf(fp, "    for(it%d=var_%s->var_%s; NULL!=it%d; it%d=it%d->next) {\n",
                            it_count, desc->name().c_str(), field->name().c_str(), it_count, it_count, it_count);
                    fprintf(fp, "        i += 1;\n");
                    fprintf(fp, "    }\n");
                    fprintf(fp, "    encode_varint(i, buf, &offset);\n");
                }
                fprintf(fp, "    for(it%d=var_%s->var_%s; NULL!=it%d; it%d=it%d->next) {\n",
                        it_count, desc->name().c_str(), field->name().c_str(), it_count, it_count, it_count);
            }
            else
            {
                if(field->is_packed())
                {
                    switch(field->type())
                    {
                    case FieldDescriptor::TYPE_FIXED32:
                        fprintf(fp, "    encode_tag_byte(buf, %d, WIRE_TYPE_FIX32, &offset);\n", field->number());
                        break;
                    case FieldDescriptor::TYPE_FIXED64:
                        fprintf(fp, "    encode_tag_byte(buf, %d, WIRE_TYPE_FIX64, &offset);\n", field->number());
                        break;
                    case FieldDescriptor::TYPE_BOOL:
                    case FieldDescriptor::TYPE_UINT32:
                    case FieldDescriptor::TYPE_UINT64:
                    case FieldDescriptor::TYPE_ENUM:
                        fprintf(fp, "    encode_tag_byte(buf, %d, WIRE_TYPE_VARINT, &offset);\n", field->number());
                        break;
                    default:
                        break;
                    }
                    fprintf(fp, "    encode_varint(var_%s->var_%s.count, buf, &offset);\n",
                            desc->name().c_str(), field->name().c_str());

                }
                fprintf(fp, "    for(i = 0; i < var_%s->var_%s.count; ++i){\n", desc->name().c_str(), field->name().c_str());
            }
            prefix_spaces = "        ";
        }
        else if(desc->field(i)->is_optional())
        {
            fprintf(fp, "    if(var_%s->has_%s){\n", desc->name().c_str(), field->name().c_str());
            prefix_spaces = "        ";
        }
        else
        {
            prefix_spaces = "    ";
        }

        fprintf(fp, "%s/* tag:%d type:%s */\n", prefix_spaces.c_str(), field->number(), field->type_name());
        switch(field->type())
        {
        case FieldDescriptor::TYPE_FIXED32:
            if(!field->is_packed())
            {
                fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_FIX32, &offset);\n", prefix_spaces.c_str(), field->number());
            }
            fprintf(fp, "%sif(NULL != buf){\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%s    *((DWORD *)(buf + offset)) = it%d->value;\n",
                            prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%s    *((DWORD *)(buf + offset)) = var_%s->var_%s.item[i];\n",
                            prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%s    *((DWORD *)(buf + offset)) = var_%s->var_%s;\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%s}\n", prefix_spaces.c_str());
            fprintf(fp, "%soffset += sizeof(DWORD);\n", prefix_spaces.c_str());
            break;

        case FieldDescriptor::TYPE_FIXED64:
            if(!field->is_packed())
            {
                fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_FIX64, &offset);\n", prefix_spaces.c_str(), field->number());
            }
            fprintf(fp, "%sif(NULL != buf){\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%s    *((WORD64 *)(buf + offset)) = it%d->value;\n",
                            prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%s    *((WORD64 *)(buf + offset)) = var_%s->var_%s.item[i];\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%s    *((WORD64 *)(buf + offset)) = var_%s->var_%s;\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%s}\n", prefix_spaces.c_str());
            fprintf(fp, "%soffset += sizeof(WORD64);\n", prefix_spaces.c_str());
            break;

        case FieldDescriptor::TYPE_BOOL:
        case FieldDescriptor::TYPE_UINT32:
        case FieldDescriptor::TYPE_UINT64:
        case FieldDescriptor::TYPE_ENUM:
            if(!field->is_packed())
            {
                fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_VARINT, &offset);\n", prefix_spaces.c_str(), field->number());
            }
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%sencode_varint(it%d->value, buf, &offset);\n",
                            prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%sencode_varint(var_%s->var_%s.item[i], buf, &offset);\n",
                            prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%sencode_varint(var_%s->var_%s, buf, &offset);\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            break;

        case FieldDescriptor::TYPE_STRING:
            fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_LENGTH_DELIMITED, &offset);\n", prefix_spaces.c_str(), field->number());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%sencode_varint(it%d->value.length, buf, &offset);\n",
                            prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%sencode_varint(var_%s->var_%s.item[i].length, buf, &offset);\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%sencode_varint(var_%s->var_%s.length, buf, &offset);\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%sif (NULL != buf) {\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%s    memcpy(buf + offset, (unsigned char *)it%d->value.data, it%d->value.length);\n",
                            prefix_spaces.c_str(), it_count, it_count);
                }
                else
                {
                    fprintf(fp, "%s    memcpy(buf + offset, (unsigned char *)var_%s->var_%s.item[i].data, var_%s->var_%s.item[i].length);\n",
                            prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%s    memcpy(buf + offset, (unsigned char *)var_%s->var_%s.data, var_%s->var_%s.length);\n",
                        prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%s}\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%soffset += it%d->value.length;\n",
                            prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%soffset += var_%s->var_%s.item[i].length;\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%soffset += var_%s->var_%s.length;\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            break;

        case FieldDescriptor::TYPE_BYTES:
            fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_LENGTH_DELIMITED, &offset);\n", prefix_spaces.c_str(), field->number());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%sencode_varint(it%d->value.length, buf, &offset);\n",
                            prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%sencode_varint(var_%s->var_%s.item[i].length, buf, &offset);\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%sencode_varint(var_%s->var_%s.length, buf, &offset);\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%sif (NULL != buf) {\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%s    memcpy(buf + offset, it%d->value.data, it%d->value.length);\n",
                            prefix_spaces.c_str(), it_count, it_count);
                }
                else
                {
                    fprintf(fp, "%s    memcpy(buf + offset, var_%s->var_%s.item[i].data, var_%s->var_%s.item[i].length);\n",
                            prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%s    memcpy(buf + offset, var_%s->var_%s.data, var_%s->var_%s.length);\n",
                        prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%s}\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%soffset += it%d->value.length;\n", prefix_spaces.c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%soffset += var_%s->var_%s.item[i].length;\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%soffset += var_%s->var_%s.length;\n", prefix_spaces.c_str(), desc->name().c_str(), field->name().c_str());
            }
            break;

        case FieldDescriptor::TYPE_MESSAGE:
            fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_LENGTH_DELIMITED, &offset);\n", prefix_spaces.c_str(), field->number());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%sencode_buf_len = encode_message_%s(&(it%d->value), NULL);\n",
                            prefix_spaces.c_str(), field->message_type()->name().c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%sencode_buf_len = encode_message_%s(&(var_%s->var_%s.item[i]), NULL);\n",
                            prefix_spaces.c_str(), field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%sencode_buf_len = encode_message_%s(&(var_%s->var_%s), NULL);\n", prefix_spaces.c_str(), field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%sencode_varint(encode_buf_len, buf, &offset);\n", prefix_spaces.c_str());
            fprintf(fp, "%sif(NULL != buf){\n", prefix_spaces.c_str());
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    fprintf(fp, "%s    encode_message_%s(&(it%d->value), buf + offset);\n",
                            prefix_spaces.c_str(), field->message_type()->name().c_str(), it_count);
                }
                else
                {
                    fprintf(fp, "%s    encode_message_%s(&(var_%s->var_%s.item[i]), buf + offset);\n", prefix_spaces.c_str(), field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                }
            }
            else
            {
                fprintf(fp, "%s    encode_message_%s(&(var_%s->var_%s), buf + offset);\n", prefix_spaces.c_str(), field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "%s}\n", prefix_spaces.c_str());
            fprintf(fp, "%soffset += encode_buf_len;\n", prefix_spaces.c_str());
            break;

        default:
            fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
            break;
        }
        if(desc->field(i)->is_repeated() || desc->field(i)->is_optional())
        {
            fprintf(fp, "    }\n");
        }
    }
    fprintf(fp, "    return offset;\n");
    fprintf(fp, "}\n");

///////////////////////////////////////////////////////////////////////////
// Decode function
    fprintf(fp, "\nBOOL decode_message_%s(BYTE* buf, const size_t buf_len, %s* var_%s){\n", desc->name().c_str(), struct_name.c_str(), desc->name().c_str());
    fprintf(fp, "	size_t offset = 0;\n");
// 包含message字段时，才需要使用此变量
    for(int i=0; i<desc->field_count(); ++i)
    {
        if(FieldDescriptor::TYPE_MESSAGE == desc->field(i)->type())
        {
            fprintf(fp, "	size_t tmp_field_len;\n");
            break;
        }
    }
    fprintf(fp, "    WORD field_num;\n");
    fprintf(fp, "    BYTE wire_type;\n");
    fprintf(fp, "\n    clear_message_%s(var_%s);\n\n",
            desc->name().c_str(), desc->name().c_str());

    fprintf(fp, "    for(;offset < buf_len;){\n");
    fprintf(fp, "        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);\n");
    fprintf(fp, "        switch(field_num){\n");
    for(int i=0; i<desc->field_count(); ++i)
    {
        const FieldDescriptor *field = desc->field(i);
        fprintf(fp, "        /* type:%s */\n", field->type_name());
        fprintf(fp, "        case %d:\n", field->number());
        switch(field->type())
        {
        case FieldDescriptor::TYPE_FIXED32:
            if(field->is_repeated())
            {
                char spaces[100];
                spaces[0] = '\0';
                if(is_dynamic_repeated(field))
                {
                    if(field->is_packed())
                    {
                        fprintf(fp, "            {\n");
                        fprintf(fp, "                size_t i = 0;\n");
                        fprintf(fp, "                size_t array_size = 0;  /* packed repeated field */\n");
                        fprintf(fp, "                decode_varint(buf + offset, &(array_size), &offset);\n");
                        fprintf(fp, "                for(i=0; i<array_size; ++i){\n");
                        strcpy(spaces, "        ");
                    }
                    alloc_new_repeated_item(fp, desc, field, spaces);
                    fprintf(fp, "%s            var_%s->var_%s_tail->value = *((DWORD *)(buf + offset));\n",
                            spaces, desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "%s            offset += sizeof(DWORD);\n", spaces);
                    if(field->is_packed())
                    {
                        fprintf(fp, "                }\n");
                        fprintf(fp, "            }\n");
                    }
                }
                else
                {
                    if(field->is_packed())
                    {
                        fprintf(fp, "            {\n");
                        fprintf(fp, "                size_t i = 0;\n");
                        fprintf(fp, "                size_t array_size = 0;  /* packed repeated field */\n");
                        fprintf(fp, "                decode_varint(buf + offset, &(array_size), &offset);\n");
                        fprintf(fp, "                for(i=0; i<array_size; ++i){\n");
                        strcpy(spaces, "        ");
                    }
                    fprintf(fp, "%s            if(var_%s->var_%s.count >= %s) {\n",
                            spaces, desc->name().c_str(), field->name().c_str(),
                            map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                    fprintf(fp, "%s                return FALSE;  /* 数组超限 */\n", spaces);
                    fprintf(fp, "%s            }\n", spaces);
                    fprintf(fp, "%s            var_%s->var_%s.item[var_%s->var_%s.count] = *((DWORD *)(buf + offset));\n",
                            spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "%s            offset += sizeof(DWORD);\n", spaces);
                    fprintf(fp, "%s            var_%s->var_%s.count += 1;\n",
                            spaces, desc->name().c_str(), field->name().c_str());
                    if(field->is_packed())
                    {
                        fprintf(fp, "                }\n");
                        fprintf(fp, "            }\n");
                    }
                }
            }
            else if(field->is_optional())
            {
                fprintf(fp, "            var_%s->var_%s = *((DWORD *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += sizeof(DWORD);\n");
                fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "            var_%s->var_%s = *((DWORD *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += sizeof(DWORD);\n");
            }
            fprintf(fp, "            break;\n");
            break;
        case FieldDescriptor::TYPE_FIXED64:
            if(field->is_repeated())
            {
                char spaces[100];
                spaces[0] = '\0';
                if(is_dynamic_repeated(field))
                {
                    if(field->is_packed())
                    {
                        fprintf(fp, "            {\n");
                        fprintf(fp, "                size_t i = 0;\n");
                        fprintf(fp, "                size_t array_size = 0;  /* packed repeated field */\n");
                        fprintf(fp, "                decode_varint(buf + offset, &(array_size), &offset);\n");
                        fprintf(fp, "                for(i=0; i<array_size; ++i){\n");
                        strcpy(spaces, "        ");
                    }
                    alloc_new_repeated_item(fp, desc, field, spaces);
                    fprintf(fp, "%s            var_%s->var_%s_tail->value = *((WORD64 *)(buf + offset));\n",
                            spaces, desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "%s            offset += sizeof(WORD64);\n", spaces);
                    if(field->is_packed())
                    {
                        fprintf(fp, "                }\n");
                        fprintf(fp, "            }\n");
                    }
                }
                else
                {
                    if(field->is_packed())
                    {
                        fprintf(fp, "            {\n");
                        fprintf(fp, "                size_t i = 0;\n");
                        fprintf(fp, "                size_t array_size = 0;  /* packed repeated field */\n");
                        fprintf(fp, "                decode_varint(buf + offset, &(array_size), &offset);\n");
                        fprintf(fp, "                for(i=0; i<array_size; ++i){\n");
                        strcpy(spaces, "        ");
                    }
                    fprintf(fp, "%s            if(var_%s->var_%s.count >= %s) {\n",
                            spaces, desc->name().c_str(), field->name().c_str(),
                            map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                    fprintf(fp, "%s                return FALSE;  /* 数组超限 */\n", spaces);
                    fprintf(fp, "%s            }\n", spaces);
                    fprintf(fp, "%s            var_%s->var_%s.item[var_%s->var_%s.count] = *((WORD64 *)(buf + offset));\n",
                            spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "%s            offset += sizeof(WORD64);\n", spaces);
                    fprintf(fp, "%s            var_%s->var_%s.count += 1;\n",
                            spaces, desc->name().c_str(), field->name().c_str());
                    if(field->is_packed())
                    {
                        fprintf(fp, "                }\n");
                        fprintf(fp, "            }\n");
                    }
                }
            }
            else if(field->is_optional())
            {
                fprintf(fp, "            var_%s->var_%s = *((WORD64 *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += sizeof(WORD64);\n");
                fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "            var_%s->var_%s = *((WORD64 *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += sizeof(WORD64);\n");
            }
            fprintf(fp, "            break;\n");
            break;
        case FieldDescriptor::TYPE_BOOL:
        case FieldDescriptor::TYPE_UINT32:
        case FieldDescriptor::TYPE_UINT64:
        case FieldDescriptor::TYPE_ENUM:
            if(field->is_repeated())
            {
                char spaces[100];
                spaces[0] = '\0';
                if(is_dynamic_repeated(field))
                {
                    if(field->is_packed())
                    {
                        fprintf(fp, "            {\n");
                        fprintf(fp, "                size_t i = 0;\n");
                        fprintf(fp, "                size_t array_size = 0;  /* packed repeated field */\n");
                        fprintf(fp, "                decode_varint(buf + offset, &(array_size), &offset);\n");
                        fprintf(fp, "                for(i=0; i<array_size; ++i){\n");
                        strcpy(spaces, "        ");
                    }
                    alloc_new_repeated_item(fp, desc, field, spaces);
                    fprintf(fp, "%s            decode_varint(buf + offset, &(var_%s->var_%s_tail->value), &offset);\n",
                            spaces, desc->name().c_str(), field->name().c_str());
                    if(field->is_packed())
                    {
                        fprintf(fp, "                }\n");
                        fprintf(fp, "            }\n");
                    }
                }
                else
                {
                    if(field->is_packed())
                    {
                        fprintf(fp, "            {\n");
                        fprintf(fp, "                size_t i = 0;\n");
                        fprintf(fp, "                size_t array_size = 0;  /* packed repeated field */\n");
                        fprintf(fp, "                decode_varint(buf + offset, &(array_size), &offset);\n");
                        fprintf(fp, "                for(i=0; i<array_size; ++i){\n");
                        strcpy(spaces, "        ");
                    }
                    fprintf(fp, "%s            if(var_%s->var_%s.count >= %s) {\n",
                            spaces, desc->name().c_str(), field->name().c_str(),
                            map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                    fprintf(fp, "%s                return FALSE;  /* 数组超限 */\n", spaces);
                    fprintf(fp, "%s            }\n", spaces);
                    fprintf(fp, "%s            decode_varint(buf + offset, &(var_%s->var_%s.item[var_%s->var_%s.count]), &offset);\n",
                            spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "%s            var_%s->var_%s.count += 1;\n",
                            spaces, desc->name().c_str(), field->name().c_str());
                    if(field->is_packed())
                    {
                        fprintf(fp, "                }\n");
                        fprintf(fp, "            }\n");
                    }
                }
            }
            else if(field->is_optional())
            {
                fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s), &offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s), &offset);\n", desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "            break;\n");
            break;
        case FieldDescriptor::TYPE_STRING:
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    alloc_new_repeated_item(fp, desc, field, "");
                    fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s_tail->value.length), &offset);\n",
                            desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            var_%s->var_%s_tail->value.data = (char *)(buf + offset);\n",
                            desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            offset += var_%s->var_%s_tail->value.length;\n",
                            desc->name().c_str(), field->name().c_str());
                }
                else
                {
                    fprintf(fp, "            if(var_%s->var_%s.count >= %s) {\n", desc->name().c_str(), field->name().c_str(),
                            map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                    fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
                    fprintf(fp, "            }\n");
                    fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.item[var_%s->var_%s.count].length), &offset);\n",
                            desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            var_%s->var_%s.item[var_%s->var_%s.count].data = (char *)(buf + offset);\n",
                            desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            offset += var_%s->var_%s.item[var_%s->var_%s.count].length;\n",
                            desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
                }
            }
            else if(field->is_optional())
            {
                fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->var_%s.data = (char *)(buf + offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->var_%s.data = (char *)(buf + offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "            break;\n");
            break;
        case FieldDescriptor::TYPE_BYTES:
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    alloc_new_repeated_item(fp, desc, field, "");
                    fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s_tail->value.length), &offset);\n",
                            desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            var_%s->var_%s_tail->value.data = buf + offset;\n",
                            desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            offset += var_%s->var_%s_tail->value.length;\n",
                            desc->name().c_str(), field->name().c_str());
                }
                else
                {
                    fprintf(fp, "            if(var_%s->var_%s.count >= %s) {\n", desc->name().c_str(), field->name().c_str(),
                            map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                    fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
                    fprintf(fp, "            }\n");
                    fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.item[var_%s->var_%s.count].length), &offset);\n",
                            desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            var_%s->var_%s.item[var_%s->var_%s.count].data = buf + offset;\n",
                            desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            offset += var_%s->var_%s.item[var_%s->var_%s.count].length;\n",
                            desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
                }
            }
            else if(field->is_optional())
            {
                fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->var_%s.data = buf + offset;\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            var_%s->var_%s.data = buf + offset;\n", desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
            }
            fprintf(fp, "            break;\n");
            break;
        case FieldDescriptor::TYPE_MESSAGE:
            if(field->is_repeated())
            {
                if(is_dynamic_repeated(field))
                {
                    alloc_new_repeated_item(fp, desc, field, "");
                    fprintf(fp, "            decode_varint(buf + offset, &tmp_field_len, &offset);\n");
                    fprintf(fp, "            decode_message_%s(buf + offset, tmp_field_len, &(var_%s->var_%s_tail->value));\n",
                            field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            offset += tmp_field_len;\n");
                }
                else
                {
                    fprintf(fp, "            if(var_%s->var_%s.count >= %s) {\n", desc->name().c_str(), field->name().c_str(),
                            map_array_size.at(field->containing_type()->name() + ":" + field->name()).c_str());
                    fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
                    fprintf(fp, "            }\n");
                    fprintf(fp, "            decode_varint(buf + offset, &tmp_field_len, &offset);\n");
                    fprintf(fp, "            decode_message_%s(buf + offset, tmp_field_len, &(var_%s->var_%s.item[var_%s->var_%s.count]));\n", field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
                    fprintf(fp, "            offset += tmp_field_len;\n");
                    fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
                }
            }
            else if(field->is_optional())
            {
                fprintf(fp, "            decode_varint(buf + offset, &tmp_field_len, &offset);\n");
                fprintf(fp, "            decode_message_%s(buf + offset, tmp_field_len, &(var_%s->var_%s));\n", field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += tmp_field_len;\n");
                fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
            }
            else
            {
                fprintf(fp, "            decode_varint(buf + offset, &tmp_field_len, &offset);\n");
                fprintf(fp, "            decode_message_%s(buf + offset, tmp_field_len, &(var_%s->var_%s));\n", field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
                fprintf(fp, "            offset += tmp_field_len;\n");
            }
            fprintf(fp, "            break;\n");
            break;
        default:
            fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
            break;
        }
    }
    fprintf(fp, "        default:\n");
    fprintf(fp, "        	deal_unknown_field(wire_type, buf+offset, &offset);\n");
    fprintf(fp, "         	break;\n");
    fprintf(fp, "        }\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    return TRUE;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* lint -restore */\n");
    fprintf(fp, "/* end of file */\n\n");
    fclose(fp);
    return retcode;
}

int gen_all_from_file(const FileDescriptor *f, string &target_dir)
{
    int retcode = 0;
    for(int i=0; i<f->message_type_count(); ++i)
    {
        const Descriptor* desc = f->message_type(i);
        map<string, string> map_array_size;

        retcode = gen_header(desc, target_dir, map_array_size);
        if(0 == retcode){
            retcode = gen_source(desc, target_dir, map_array_size);
        }
        map_array_size.clear();

        /* close option file */
        if(NULL != fp_option)
        {
            fclose(fp_option);
            fp_option = NULL;
        }
    }
    return retcode;
}

class ImporterError : public compiler::MultiFileErrorCollector
{
private:
    void AddError(
        const string & filename,
        int line,
        int column,
        const string & message)
    {
        printf("%s %d:%d -- %s", filename.c_str(), line, column, message.c_str());
    }
};

/* RAII */
static inline void freep(char **p)
{
    if(*p)
    {
        free(*p);
    }
    *p = NULL;
}
#define _cleanup_free_ __attribute__((cleanup(freep)))

int create_path(string &path)
{
    _cleanup_free_ char* dir_name = (char *)malloc(path.length() + 10);
    if(path[path.length()-1] != path_sep)
    {
        path += path_sep;
    }
    strcpy(dir_name, path.c_str());

    size_t len = path.length();
    for(size_t i=1; i<len; i++)
    {
        if(dir_name[i] == path_sep)
        {
            dir_name[i] = EOS;
            if(access(dir_name, 0))
            {
#ifdef _WIN32
                if(mkdir(dir_name))
#else
                if(mkdir(dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#endif
                {
                    printf("Cannot create directory: %s\n", dir_name);
                    return -1;
                }
            }
            dir_name[i] = path_sep;
        }
    }
    return 0;
}

int get_syntax(LPCSTR proto_filename)
{
    int syntax = 0;
    char str_syntax[128];
    char buf[8 * 1024];

    FILE *fp = fopen(proto_filename, "rt");
    if(NULL != fp)
    {
        for(;;)
        {
            if(NULL == fgets(buf, sizeof(buf), fp))
            {
                break;
            }
            else
            {
                int i = 0;
                int start = 0;
                int len = 0;

                for(;; ++i)
                {
                    if(' ' != buf[i] && '\t' != buf[i])
                    {
                        break;
                    }
                }
                // printf("%s\n", buf+i);

                // 发现"syntax"关键字
                if(0 == memcmp(buf+i, "syntax", 6))
                {
                    for(;; ++i)
                    {
                        if(isdigit(buf[i]))
                        {
                            start = i;
                            break;
                        }
                    }
                    for(;; ++i)
                    {
                        if(!isdigit(buf[i]))
                        {
                            len = i - start;
                            break;
                        }
                    }
                    memcpy(str_syntax, buf+start, len);
                    str_syntax[len] = EOS;
                    syntax = atoi(str_syntax);
                    break;
                }
            }
        }
        fclose(fp);
    }
    return syntax;
}

void convert_pbv3(LPCSTR pbv3_filename, LPCSTR pbv2_filename)
{
    char buf[8 * 1024];
    char key_type[40];
    char key_value[40];
    char field_name[80];
    char field_no[40];
    char append_buf[8*1024];
    set<string> map_entrys;

    append_buf[0] = EOS;
    FILE *fp = fopen(pbv3_filename, "rt");
    if(NULL != fp)
    {
        FILE *fpout = fopen(pbv2_filename, "wt");
        if(NULL!= fpout)
        {
            for(;;)
            {
                if(NULL == fgets(buf, sizeof(buf), fp))
                {
                    break;
                }
                else
                {
                    int i = 0;
                    int start = 0;
                    int len = 0;

                    for(;; ++i)
                    {
                        if(' ' != buf[i] && '\t' != buf[i])
                        {
                            break;
                        }
                    }

                    if(0 == memcmp(buf+i, "map", 3))   // 发现"map"关键字
                    {
                        printf("%s", buf+i);
                        for(i=i+3;; ++i)
                        {
                            if('<'!=buf[i])
                            {
                                break;
                            }
                        }
                        start = i;
                        for(;; ++i)
                        {
                            if(','==buf[i] || ' '==buf[i] || '\t'==buf[i])
                            {
                                break;
                            }
                        }
                        len = i - start;
                        // 定位到key_type信息
                        memcpy(key_type, buf + start, len);
                        key_type[len] = EOS;
                        printf("key_type:%s\n", key_type);

                        for(;; ++i)
                        {
                            if(' '!=buf[i] && '\t'!=buf[i] && ','!=buf[i])
                            {
                                break;
                            }
                        }
                        start = i;
                        for(;; ++i)
                        {
                            if('>'==buf[i] || ' '==buf[i] || '\t'==buf[i])
                            {
                                break;
                            }
                        }
                        len = i - start;
                        // 定位到key_value信息
                        memcpy(key_value, buf + start, len);
                        key_value[len] = EOS;
                        printf("key_value:%s\n", key_value);

                        for(;; ++i)
                        {
                            if('>'!=buf[i] && ' '!=buf[i] && '\t'!=buf[i])
                            {
                                break;
                            }
                        }
                        start = i;
                        for(;; ++i)
                        {
                            if('='==buf[i] || ' '==buf[i] || '\t'==buf[i])
                            {
                                break;
                            }
                        }
                        len = i - start;
                        memcpy(field_name, buf + start, len);
                        field_name[len] = EOS;
                        printf("field_name:%s\n", field_name);

                        for(;; ++i)
                        {
                            if('='!=buf[i] && ' '!=buf[i] && '\t'!=buf[i])
                            {
                                break;
                            }
                        }
                        start = i;
                        for(;; ++i)
                        {
                            if(';'==buf[i] || ' '==buf[i] || '\t'==buf[i])
                            {
                                break;
                            }
                        }
                        len = i - start;
                        memcpy(field_no, buf + start, len);
                        field_no[len] = EOS;
                        printf("field_no:%s\n", field_no);

                        // 使用set避免同名mapentrys重复插入
                        string map_entry_name = string("Map") + field_name + "Entry";
                        if(map_entrys.find(map_entry_name) == map_entrys.end())
                        {
                            sprintf(buf, "message Map%sEntry {\n  %s key = 1;\n  %s value = 2;\n}\n\n",
                                    field_name, key_type, key_value);
                            strcat(append_buf, buf);
                            map_entrys.insert(map_entry_name);
                        }
                        sprintf(buf, "repeated Map%sEntry %s = %s;\n", field_name, field_name, field_no);
                    }
                    // 没有"map"关键字的情况，则直接拷贝
                    fputs(buf, fpout);
                }
            }
            fputs(append_buf, fpout);
            fclose(fpout);
        }
        fclose(fp);
    }
}

int main(int argc, char *argv[])
{
    char no_map_filename[256];
    string str;
    const FileDescriptor *f;
    ImporterError errorCollector;
    compiler::DiskSourceTree sourceTree;
    int retcode = 0;
    sourceTree.MapPath("", ".");

    if (argc < 3)
    {
        printf("Usage: %s xxx.proto [xxx.proto] target_dir\n", argv[0]);
        return 1;
    }

    string target_dir = string(argv[argc-1]);
    if(create_path(target_dir))
    {
        return 2;
    }
    string include_path = target_dir + "include" + path_sep;
    if(create_path(include_path))
    {
        return 2;
    }
    string source_path = target_dir + "source" + path_sep;
    if(create_path(source_path))
    {
        return 2;
    }

    for(int i=1; i<argc-1; ++i)
    {
        compiler::Importer *importer = new compiler::Importer(&sourceTree, &errorCollector);
        proto_filename = argv[i];
        int syntax = get_syntax(proto_filename);

        // 如果是v3的语法，则转换map关键字
        ///////////////////////////////////////////////////
        // message MapFieldEntry {
        //   key_type key = 1;
        //   value_type value = 2;
        // }
        // repeated MapFieldEntry Field = N;
        ///////////////////////////////////////////////////
        // map<key_type, value_type> Field = N;
        ///////////////////////////////////////////////////
        if(3 == syntax)
        {
            int fd = -1;
            sprintf(no_map_filename, "%s.tmp_XXXXXX", proto_filename);
            fd = mkstemp(no_map_filename);
            if(-1 == fd){
                printf("Cannot create tempfile.\n");
                delete importer;
                return 4;
            } else {
                printf("tempfile %s created.\n", no_map_filename);
                close(fd);
            }
            // sprintf(no_map_filename, "%s.tmp", proto_filename);
            convert_pbv3(proto_filename, no_map_filename);
        }
        else
        {
            strcpy(no_map_filename, proto_filename);
        }

        f = importer->Import(no_map_filename);
        if (NULL == f)
        {
            printf("Cannot import file:%s", no_map_filename);
            retcode = 3;
        } else {
            retcode = gen_all_from_file(f, target_dir);
        }

        if(3 == syntax)
        {
            /* char command[256];
            sprintf(command, "rm -f %s", no_map_filename);
            system(command); */
            remove(no_map_filename);
            printf("tempfile %s removed.\n", no_map_filename);
        }
        delete importer;

        if(0!=retcode){
            return retcode;
        }
    }

    // common header file
    retcode = gen_comm(target_dir);
    return retcode;
}

