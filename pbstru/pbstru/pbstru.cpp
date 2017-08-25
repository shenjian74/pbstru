// pbstru.cpp : 定义控制台应用程序的入口点。
//

#if defined (_MSC_VER)
# define _CRT_SECURE_NO_WARNINGS
# pragma warning(disable: 4482)  
#endif
#include <stdio.h>
#include "importer.h"
#include "bstrwrap.h"

typedef const char *LPCSTR;
typedef char *LPSTR;
typedef bool BOOL;
#define TRUE 1
#define FALSE 0

using namespace google::protobuf;

///////////////////////////////////////////////////////////////////////////////
// 结构定义的前缀，在CDB中可定义为"cdb_"
const char struct_prefix[] = "st_";
const char struct_postfix[] = "";
// 结构名称是否需要小写
const bool is_struct_lowercase = false;
///////////////////////////////////////////////////////////////////////////////

const char __THIS_FILE__[] = "pbstru.cpp";
#ifdef _WIN32
char path_prefix[] = "codec\\";
#else
char path_prefix[] = "codec/";
#endif

void gen_comm(void) {
	CBString filename = CBString(path_prefix) + "pbstru_comm.h";
	FILE *fp = fopen((LPCSTR)filename, "wt");
	if (NULL == fp) {
		printf("Cannot open file:%s for write.\n", (LPCSTR)filename);
		return;
	}
	fprintf(fp, "#ifndef __PBSTRU_COMM_H__\n");
	fprintf(fp, "#define __PBSTRU_COMM_H__\n");

	fprintf(fp, "\n#include <stdlib.h>\n");
	fprintf(fp, "#include <memory.h>\n");

	fprintf(fp, "\n#define WIRE_TYPE_VARINT 0\n");
	fprintf(fp, "#define WIRE_TYPE_FIX64 1\n");
	fprintf(fp, "#define WIRE_TYPE_LENGTH_DELIMITED 2\n");
	fprintf(fp, "#define WIRE_TYPE_FIX32 5\n");

	fprintf(fp, "\n#ifdef _WIN32\n");
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
	fprintf(fp, "\n#else\n");
	fprintf(fp, "#include \"tulip.h\"\n");
	fprintf(fp, "\n#endif");
	fprintf(fp, "\ntypedef struct {\n");
	fprintf(fp, "	char *data;\n");
	fprintf(fp, "	size_t length;\n");
	fprintf(fp, "} ps_string;\n");
	fprintf(fp, "\ntypedef struct {\n");
	fprintf(fp, "	unsigned char *data;\n");
	fprintf(fp, "	size_t length;\n");
	fprintf(fp, "} ps_bytes;\n");
	fprintf(fp, "\n");
    fprintf(fp, "void encode_tag_byte(BYTE *buf, WORD tag, BYTE wire_type, size_t *offset);\n");
	fprintf(fp, "void parse_tag_byte(BYTE *buf, WORD *field_num, BYTE *wire_type, size_t *offset);\n");
	fprintf(fp, "void deal_unknown_field(BYTE wire_type, BYTE *buf, size_t *offset);\n");
	fprintf(fp, "\n");
	fprintf(fp, "#define encode_varint(len, buf, offset) { \\\n");
	fprintf(fp, "	unsigned long long remain_len = len; \\\n");
	fprintf(fp, "	size_t iloop; \\\n");
	fprintf(fp, "	for (iloop = 0;; ++iloop) { \\\n");
	fprintf(fp, "		if ((remain_len >> 7) > 0) { \\\n");
	fprintf(fp, "			if (NULL != buf) { \\\n");
	fprintf(fp, "				buf[(*(offset)) + iloop] = ((BYTE)remain_len) | 0x80; \\\n");
	fprintf(fp, "			} \\\n");
	fprintf(fp, "			remain_len = remain_len >> 7; \\\n");
	fprintf(fp, "		} \\\n");
	fprintf(fp, "		else { \\\n");
	fprintf(fp, "			if (NULL != buf) { \\\n");
	fprintf(fp, "				buf[(*(offset)) + iloop] = (BYTE)remain_len; \\\n");
	fprintf(fp, "			} \\\n");
	fprintf(fp, "			break; \\\n");
	fprintf(fp, "		} \\\n");
	fprintf(fp, "	} \\\n");
	fprintf(fp, "	*(offset) += 1 + iloop; \\\n");
	fprintf(fp, "}\n");
	fprintf(fp, "\n");
	fprintf(fp, "#define decode_varint(buf, value, offset) { \\\n");
	fprintf(fp, "    size_t iloop; \\\n");
	fprintf(fp, "    (*(value)) = 0; \\\n");
	fprintf(fp, "    for(iloop=0;;++iloop){ \\\n");
	fprintf(fp, "	     (*(value)) += ((buf)[iloop] & 0x7F) << (7*iloop); \\\n");
	fprintf(fp, "        if(0 == ((buf)[iloop] & 0x80)){ \\\n");
	fprintf(fp, "            break; \\\n");
	fprintf(fp, "        } \\\n");
	fprintf(fp, "    } \\\n");
	fprintf(fp, "    (*(offset)) += 1 + iloop; \\\n");
	fprintf(fp, "}\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n#endif\n\n/* end of file */");
	fclose(fp);

    filename = CBString(path_prefix) + "pbstru_comm.c";
	fp = fopen((LPCSTR)filename, "wt");
	if (NULL == fp) {
		printf("Cannot open file:%s for write.\n", (LPCSTR)filename);
		return;
	}
	fprintf(fp, "#include \"pbstru_comm.h\"\n");
	fprintf(fp, "\n");
	fprintf(fp, "void encode_tag_byte(BYTE *buf, WORD tag, BYTE wire_type, size_t *offset) { \n"); 
	fprintf(fp, "	if (tag < 16) { \n"); 
	fprintf(fp, "		if (NULL != buf) { \n"); 
	fprintf(fp, "			buf[*offset] = (BYTE)(tag << 3) | wire_type; \n"); 
	fprintf(fp, "		} \n"); 
	fprintf(fp, "		*offset += 1; \n"); 
	fprintf(fp, "	} \n"); 
	fprintf(fp, "	else { \n"); 
	fprintf(fp, "		if (NULL != buf) { \n"); 
	fprintf(fp, "			buf[*offset] = ((BYTE)tag) | 0x80; \n"); 
	fprintf(fp, "			buf[(*offset) + 1] = ((BYTE)(tag >> 4) & 0x78) | wire_type; \n"); 
	fprintf(fp, "		} \n"); 
	fprintf(fp, "		*offset += 2; \n"); 
	fprintf(fp, "	} \n"); 
	fprintf(fp, "} \n");
	fprintf(fp, "\n");
	fprintf(fp, "void parse_tag_byte(BYTE *buf, WORD *field_num, BYTE *wire_type, size_t *offset){ \n");
	fprintf(fp, "    if ((buf)[0] & 0x80) { \n");
	fprintf(fp, "	     *field_num = ((buf)[0] & 0x7F) + ((buf)[1] >> 3) * 128; \n");
	fprintf(fp, "	     *wire_type = (buf)[1] & 0x07; \n");
	fprintf(fp, "	     (*(offset)) += 2; \n");
	fprintf(fp, "    } else { \n");
	fprintf(fp, "        *field_num = (buf)[0] >> 3; \n");
	fprintf(fp, "	     *wire_type = (buf)[0] & 0x07; \n");
	fprintf(fp, "	     (*(offset)) += 1; \n");
	fprintf(fp, "    } \n");
	fprintf(fp, "} \n");
	fprintf(fp, "\n");
	fprintf(fp, "void deal_unknown_field(BYTE wire_type, BYTE *buf, size_t *offset) { \n");
	fprintf(fp, "	size_t tmp_field_len; \n");
	fprintf(fp, "	switch(wire_type){ \n");
	fprintf(fp, "	case WIRE_TYPE_VARINT: \n");
	fprintf(fp, "		decode_varint(buf, &tmp_field_len, offset); \n");
	fprintf(fp, "		break; \n");
	fprintf(fp, "	case WIRE_TYPE_FIX64: \n");
	fprintf(fp, "		(*(offset)) += 8; \n");
	fprintf(fp, "		break; \n");
	fprintf(fp, "	case WIRE_TYPE_LENGTH_DELIMITED: \n");
	fprintf(fp, "		decode_varint(buf, &tmp_field_len, offset); \n");
	fprintf(fp, "        (*(offset)) += tmp_field_len; \n");
	fprintf(fp, "		break; \n");
	fprintf(fp, "	case WIRE_TYPE_FIX32: \n");
	fprintf(fp, "        (*(offset)) += 4; \n");
	fprintf(fp, "		break; \n");
	fprintf(fp, "	default: \n");
	fprintf(fp, "		break; \n");
	fprintf(fp, "	} \n");
	fprintf(fp, "}\n");
	fprintf(fp, "/* end of file */\n\n");
	fclose(fp);
}

LPSTR proto_filename;
FILE *fp_option = NULL;

static int get_max_count(LPCSTR message_name, LPCSTR field_name) { 
	char str1[128];
	char str2[64];
	CBString option_filename(proto_filename);

	CBString key(message_name);
	key += ".";
	key += field_name;

	if (NULL == fp_option) {
		int pos = option_filename.reversefind('.', option_filename.length());
		if (BSTR_ERR == pos) {
			option_filename += ".options";
		}
		else {
			option_filename.trunc(pos);
			option_filename += ".options";
		}
		fp_option = fopen((LPCSTR)option_filename, "rt");
		if (NULL == fp_option) {
			printf("[%s:%d] Cannot open file:%s for read.\n", __THIS_FILE__, __LINE__, (LPCSTR)option_filename);
			exit(10);
		}
	}
	
	for (rewind(fp_option); !feof(fp_option);) {
		fscanf(fp_option, "%s %s", str1, str2);
		if (0 == strcmp(str1, (LPCSTR)key)) {
			char *num_str = strstr(str2, "max_count:");
			if (NULL == num_str) {
				printf("[%s:%d] Cannot read item:\"%s max_count:?\" from option file:[%s].\n", __THIS_FILE__, __LINE__, (LPCSTR)key, (LPCSTR)option_filename);
				exit(11);
			}
			else {
				return atoi(num_str + strlen("max_count:"));
			}
		}
	}

	printf("[%s:%d] Cannot read item:\"%s max_count:?\" from option file:[%s].\n", 
		__THIS_FILE__, __LINE__, (LPCSTR)key, (LPCSTR)option_filename);
	exit(12);
	// 文件不关闭
}

LPCSTR get_struct_list_name(const FieldDescriptor *field){
	static CBString struct_list_name;
	switch(field->type()){
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_UINT32:
		struct_list_name = CBString("uint32");
		break;
	case FieldDescriptor::TYPE_FIXED64:
	case FieldDescriptor::TYPE_UINT64:
		struct_list_name = CBString("uint64");
		break;
	case FieldDescriptor::TYPE_BOOL:
		struct_list_name = CBString("boolean");
		break;
	case FieldDescriptor::TYPE_STRING:
		struct_list_name = CBString("string");
		break;
	case FieldDescriptor::TYPE_BYTES:
		struct_list_name = CBString("buffer");
		break;
	case FieldDescriptor::TYPE_MESSAGE:
		struct_list_name = CBString(field->message_type()->name().c_str());
		break;
	case FieldDescriptor::TYPE_ENUM:
		struct_list_name = CBString(field->enum_type()->name().c_str());
		break;
	default:
		struct_list_name = "";
		break;
	}
	return (LPCSTR)struct_list_name;
}

static void print_field_in_struct(FILE *fp, const FieldDescriptor *field){
	CBString field_name_upper(field->name().c_str());
	field_name_upper.toupper();
	CBString message_type_upper(field->containing_type()->name().c_str());
	message_type_upper.toupper();

	// fprintf(fp, "/* %s\n%s\n%s */\n", field->DebugString().c_str(), field->lowercase_name().c_str(), field->camelcase_name().c_str());
	// const OneofDescriptor* containing_oneof() const;

	if(field->is_optional()){
		fprintf(fp, "    BOOL has_%s;\n", field->name().c_str()); 
	}

	const FieldDescriptor::Type type = field->type();
	switch(type){
	case FieldDescriptor::TYPE_FIXED32:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    DWORD var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_FIXED64:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    WORD64 var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_BOOL:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    BOOL var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_UINT32:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    DWORD var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_UINT64:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    WORD64 var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_STRING:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    ps_string var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_BYTES:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    ps_bytes var_%s;  /* tag:%d */\n", field->name().c_str(), field->number());
		}
		break;
	case FieldDescriptor::TYPE_MESSAGE:
		{
			CBString message_type_name(field->message_type()->name().c_str());
			if(is_struct_lowercase){
				message_type_name.tolower();
			}
			CBString struct_name = (CBString)struct_prefix + message_type_name + struct_postfix;

			if(field->is_repeated()){
				fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
			} else {
				fprintf(fp, "    %s var_%s;  /* tag:%d */\n", (LPCSTR)struct_name, field->name().c_str(), field->number());
			}
		}
		break;
	case FieldDescriptor::TYPE_ENUM:
		if(field->is_repeated()){
			fprintf(fp, "    st_%s_list var_%s;  /* tag:%d */\n", get_struct_list_name(field), field->name().c_str(), field->number());
		} else {
			fprintf(fp, "    enum_%s var_%s;  /* tag:%d */\n", field->enum_type()->name().c_str(), field->name().c_str(), field->number());
		}
		break;
	default:
		fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
		break;
	}
}

void gen_header(const Descriptor *desc){
	CBString name_lower(desc->name().c_str());
	name_lower.tolower();
	
	CBString struct_name;
	if(is_struct_lowercase){
		struct_name = (CBString)struct_prefix + name_lower + struct_postfix;
	} else {
		struct_name = (CBString)struct_prefix + desc->name().c_str() + struct_postfix;
	}

	CBString filename = CBString(path_prefix) + name_lower;
	filename += ".h";
	FILE *fp = fopen((LPCSTR)filename, "wt");
	if(NULL == fp){
		printf("Cannot open file:%s for write.\n", (LPCSTR)filename);
		return;
	}

	CBString name_upper(desc->name().c_str());
	name_upper.toupper();
	fprintf(fp, "#ifndef __PBSTRU_%s_H__\n", (LPCSTR)name_upper);
	fprintf(fp, "#define __PBSTRU_%s_H__\n", (LPCSTR)name_upper);
	fprintf(fp, "\n#ifdef __cplusplus\n");
	fprintf(fp, "extern \"C\"{\n");
	fprintf(fp, "#endif\n");
	fprintf(fp, "\n#include \"pbstru_comm.h\"\n");
	for(int i=0;i<desc->field_count();++i){
		const FieldDescriptor *field = desc->field(i);
		if(FieldDescriptor::TYPE_MESSAGE == field->type()){
			CBString str(field->message_type()->name().c_str());
			str.tolower();
			fprintf(fp, "#include \"%s.h\"\n", (LPCSTR)str);
		}
	}
	fprintf(fp, "\n/*\n%s*/\n", desc->DebugString().c_str());

	for(int i=0;i<desc->field_count();++i){
		const FieldDescriptor *field = desc->field(i);
		CBString field_name_upper(field->name().c_str());
		field_name_upper.toupper();
		CBString field_containing_type_upper(field->containing_type()->name().c_str());
		field_containing_type_upper.toupper();
		if(field->is_repeated()){
			fprintf(fp, "#define MAX_%s_IN_%s %d\n", 
				(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper,
				get_max_count(field->containing_type()->full_name().c_str(), 
					field->name().c_str()));
		}
	}

	for(int i=0;i<desc->field_count();++i){
		const FieldDescriptor *field = desc->field(i);
		if(FieldDescriptor::TYPE_ENUM == field->type()){
			const EnumDescriptor* enum_desc = field->enum_type();
			CBString enum_name_upper = CBString(enum_desc->name().c_str());
			enum_name_upper.toupper();
			fprintf(fp, "\n#ifndef ENUM_%s_DEFINED\n", (LPCSTR)enum_name_upper);
			fprintf(fp, "#define ENUM_%s_DEFINED\n", (LPCSTR)enum_name_upper);
			fprintf(fp, "typedef enum {\n");
			for(int j=0;j<enum_desc->value_count();++j){
				fprintf(fp, "    %s_M = %d", enum_desc->value(j)->name().c_str(), enum_desc->value(j)->number());
				// 处理最后enum值时不加逗号
				if(j!=(enum_desc->value_count()-1)){
					fprintf(fp, ",");
				}
				fprintf(fp, "\n");
			}
			fprintf(fp, "} enum_%s;\n", enum_desc->name().c_str());
			fprintf(fp, "#endif\n");
		}
	}

	// 各个list的结构定义
	for(int i=0;i<desc->field_count();++i){
		const FieldDescriptor *field = desc->field(i);
		if(field->is_repeated()){
			
			CBString field_name_upper(field->name().c_str());
			field_name_upper.toupper();
			CBString field_containing_type_upper(field->containing_type()->name().c_str());
			field_containing_type_upper.toupper();
			
			CBString struct_list_name = get_struct_list_name(field);
			struct_list_name.toupper();
			fprintf(fp, "\n#ifndef STRUCT_%s_LIST_DEFINED\n", (LPCSTR)struct_list_name);
			fprintf(fp, "#define STRUCT_%s_LIST_DEFINED\n", (LPCSTR)struct_list_name);
			fprintf(fp, "typedef struct {\n");
			fprintf(fp, "    size_t count;\n"); 
			switch(field->type()){
				case FieldDescriptor::TYPE_FIXED32:
				case FieldDescriptor::TYPE_UINT32:
					fprintf(fp, "    DWORD item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", 
						(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					break;
				case FieldDescriptor::TYPE_FIXED64:
				case FieldDescriptor::TYPE_UINT64:
					fprintf(fp, "    WORD64 item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", 
						(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					break;
				case FieldDescriptor::TYPE_BOOL:
					fprintf(fp, "    BOOL item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", 
						(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					break;
				case FieldDescriptor::TYPE_STRING:
					fprintf(fp, "    ps_string item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", 
						(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					break;
				case FieldDescriptor::TYPE_BYTES:
					fprintf(fp, "    ps_bytes item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", 
						(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					break;
				case FieldDescriptor::TYPE_MESSAGE:
					{
						CBString message_type_name(field->message_type()->name().c_str());
						if(is_struct_lowercase){
							message_type_name.tolower();
						}
						CBString struct_name = (CBString)struct_prefix + message_type_name + struct_postfix;

						fprintf(fp, "    %s item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", (LPCSTR)struct_name, 
								(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					}
					break;
				case FieldDescriptor::TYPE_ENUM:
						fprintf(fp, "    enum_%s item[MAX_%s_IN_%s];  /* tag:%d type:%s */\n", field->enum_type()->name().c_str(),  
							(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper, field->number(), field->type_name());
					break;
				default:
					fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
					break;
				}
			fprintf(fp, "} st_%s_list;\n", get_struct_list_name(field));
			fprintf(fp, "#endif\n");
		}
	}

	fprintf(fp, "\ntypedef struct {\n");
	for(int i=0;i<desc->field_count();++i){
		print_field_in_struct(fp, desc->field(i));
	}
	fprintf(fp, "} %s;\n", (LPCSTR)struct_name);

	fprintf(fp, "\nvoid clear_message_%s(%s *);\n", desc->name().c_str(), (LPCSTR)struct_name);
	fprintf(fp, "size_t encode_message_%s(%s *, BYTE *);\n", desc->name().c_str(), (LPCSTR)struct_name);
	fprintf(fp, "BOOL decode_message_%s(BYTE *, size_t, %s *);\n", desc->name().c_str(), (LPCSTR)struct_name);

	fprintf(fp, "\n#ifdef __cplusplus\n");
	fprintf(fp, "}\n");
	fprintf(fp, "#endif\n");
	fprintf(fp, "\n#endif\n\n/* end of file */\n");

	fclose(fp);
}

void gen_source(const Descriptor *desc){
	CBString name_lower(desc->name().c_str());
	name_lower.tolower();

	CBString struct_name;
	if(is_struct_lowercase){
		struct_name = (CBString)struct_prefix + name_lower + struct_postfix;
	} else {
		struct_name = (CBString)struct_prefix + desc->name().c_str() + struct_postfix;
	}

	CBString filename = CBString(path_prefix) + name_lower;
	filename += ".c";
	FILE *fp = fopen((LPCSTR)filename, "wt");
	if(NULL == fp){
		printf("Cannot open file:%s for write.\n", (LPCSTR)filename);
		return;
	}

	fprintf(fp, "#include \"%s.h\"\n", (LPCSTR)name_lower);

	fprintf(fp, "\n");
	fprintf(fp, "/* lint -save -e701 -e647 */\n");
	fprintf(fp, "\n");
	
	// clear function
	fprintf(fp, "void clear_message_%s(%s *var_%s){\n", desc->name().c_str(), (LPCSTR)struct_name, desc->name().c_str());
	for(int i=0;i<desc->field_count();++i){
		const FieldDescriptor *field = desc->field(i);
		if(field->is_repeated()){
			fprintf(fp, "    var_%s->var_%s.count = 0;\n", desc->name().c_str(), field->name().c_str());
		} else if(field->is_optional()){
			fprintf(fp, "    var_%s->has_%s = FALSE;\n", desc->name().c_str(), field->name().c_str());
		} else {
			switch(field->type()){
			case FieldDescriptor::TYPE_FIXED32:
			case FieldDescriptor::TYPE_FIXED64:
			case FieldDescriptor::TYPE_UINT32:
			case FieldDescriptor::TYPE_UINT64:
				fprintf(fp, "    var_%s->var_%s = 0;\n", desc->name().c_str(), field->name().c_str());
				break;
			case FieldDescriptor::TYPE_BOOL:
				fprintf(fp, "    var_%s->var_%s = FALSE;\n", desc->name().c_str(), field->name().c_str());
				break;
			case FieldDescriptor::TYPE_ENUM:
				break;
			case FieldDescriptor::TYPE_STRING:
			case FieldDescriptor::TYPE_BYTES:
				fprintf(fp, "    var_%s->var_%s.length = 0;\n", desc->name().c_str(), field->name().c_str());
				break;
			case FieldDescriptor::TYPE_MESSAGE:
				fprintf(fp, "    clear_message_%s(&(var_%s->var_%s));\n", 
					field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
				break;
			default:
				fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
				break;
			}
		}
	}
	fprintf(fp, "}\n");

	fprintf(fp, "\nsize_t encode_message_%s(%s *var_%s, BYTE *buf){\n", desc->name().c_str(), (LPCSTR)struct_name, desc->name().c_str());
	// 有嵌套message的时候会用到编码长度
	for(int i=0;i<desc->field_count();++i){
		if(FieldDescriptor::TYPE_MESSAGE == desc->field(i)->type()){
			fprintf(fp, "    size_t encode_buf_len;\n");
			break;
		}
	}
	// 有repeat字段的时候会用到循环变量
	for(int i=0;i<desc->field_count();++i){
		if(desc->field(i)->is_repeated()){
			fprintf(fp, "    size_t i;\n");
			break;
		}
	}
	fprintf(fp, "    size_t offset = 0;\n");

	// 逐个字段encode
	for(int i=0;i<desc->field_count();++i){
		CBString prefix_spaces("    ");
		const FieldDescriptor *field = desc->field(i);

		fprintf(fp, "\n");
		if(desc->field(i)->is_repeated()){
			fprintf(fp, "    for(i = 0; i < var_%s->var_%s.count; ++i){\n", desc->name().c_str(), field->name().c_str());
			prefix_spaces = "        ";
		} else if(desc->field(i)->is_optional()){
			fprintf(fp, "    if(var_%s->has_%s){\n", desc->name().c_str(), field->name().c_str()); 
			prefix_spaces = "        ";
		} else {
			prefix_spaces = "    ";
		}
		fprintf(fp, "%s/* type:%s */\n", (LPCSTR)prefix_spaces, field->type_name());

		switch(field->type()){
		case FieldDescriptor::TYPE_FIXED32:
			fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_FIX32, &offset);\n", (LPCSTR)prefix_spaces, field->number());
                        fprintf(fp, "%sif(NULL != buf){\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%s    *((DWORD *)(buf + offset)) = var_%s->var_%s.item[i];\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%s    *((DWORD *)(buf + offset)) = var_%s->var_%s;\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
                        fprintf(fp, "%s}\n", (LPCSTR)prefix_spaces);
			fprintf(fp, "%soffset += sizeof(DWORD);\n", (LPCSTR)prefix_spaces);
			break;

		case FieldDescriptor::TYPE_FIXED64:
			fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_FIX64, &offset);\n", (LPCSTR)prefix_spaces, field->number());
                        fprintf(fp, "%sif(NULL != buf){\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%s    *((WORD64 *)(buf + offset)) = var_%s->var_%s.item[i];\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%s    *((WORD64 *)(buf + offset)) = var_%s->var_%s;\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
                        fprintf(fp, "%s}\n", (LPCSTR)prefix_spaces);
			fprintf(fp, "%soffset += sizeof(WORD64);\n", (LPCSTR)prefix_spaces);
			break;

		case FieldDescriptor::TYPE_BOOL:
		case FieldDescriptor::TYPE_UINT32:
		case FieldDescriptor::TYPE_UINT64:
		case FieldDescriptor::TYPE_ENUM:
			fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_VARINT, &offset);\n", (LPCSTR)prefix_spaces, field->number());
			if(field->is_repeated()){
				fprintf(fp, "%sencode_varint(var_%s->var_%s.item[i], buf, &offset);\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%sencode_varint(var_%s->var_%s, buf, &offset);\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
			break;

		case FieldDescriptor::TYPE_STRING:
			fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_LENGTH_DELIMITED, &offset);\n", (LPCSTR)prefix_spaces, field->number());
			if(field->is_repeated()){
				fprintf(fp, "%sencode_varint(var_%s->var_%s.item[i].length, buf, &offset);\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%sencode_varint(var_%s->var_%s.length, buf, &offset);\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "%sif (NULL != buf) {\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%s    memcpy(buf + offset, (unsigned char *)var_%s->var_%s.item[i].data, var_%s->var_%s.item[i].length);\n", 
					(LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%s    memcpy(buf + offset, (unsigned char *)var_%s->var_%s.data, var_%s->var_%s.length);\n", 
					(LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "%s}\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%soffset += var_%s->var_%s.item[i].length;\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%soffset += var_%s->var_%s.length;\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
			break;

		case FieldDescriptor::TYPE_BYTES:
			fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_LENGTH_DELIMITED, &offset);\n", (LPCSTR)prefix_spaces, field->number());
			if(field->is_repeated()){
				fprintf(fp, "%sencode_varint(var_%s->var_%s.item[i].length, buf, &offset);\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%sencode_varint(var_%s->var_%s.length, buf, &offset);\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "%sif (NULL != buf) {\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%s    memcpy(buf + offset, var_%s->var_%s.item[i].data, var_%s->var_%s.item[i].length);\n", 
					(LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%s    memcpy(buf + offset, var_%s->var_%s.data, var_%s->var_%s.length);\n", 
					(LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "%s}\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%soffset += var_%s->var_%s.item[i].length;\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%soffset += var_%s->var_%s.length;\n", (LPCSTR)prefix_spaces, desc->name().c_str(), field->name().c_str());
			}
			break;

		case FieldDescriptor::TYPE_MESSAGE:
			fprintf(fp, "%sencode_tag_byte(buf, %d, WIRE_TYPE_LENGTH_DELIMITED, &offset);\n", (LPCSTR)prefix_spaces, field->number());
			if(field->is_repeated()){
				fprintf(fp, "%sencode_buf_len = encode_message_%s(&(var_%s->var_%s.item[i]), NULL);\n", (LPCSTR)prefix_spaces, field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%sencode_buf_len = encode_message_%s(&(var_%s->var_%s), NULL);\n", (LPCSTR)prefix_spaces, field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "%sencode_varint(encode_buf_len, buf, &offset);\n", (LPCSTR)prefix_spaces);
			fprintf(fp, "%sif(NULL != buf){\n", (LPCSTR)prefix_spaces);
			if(field->is_repeated()){
				fprintf(fp, "%s    encode_message_%s(&(var_%s->var_%s.item[i]), buf + offset);\n", (LPCSTR)prefix_spaces, field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "%s    encode_message_%s(&(var_%s->var_%s), buf + offset);\n", (LPCSTR)prefix_spaces, field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "%s}\n", (LPCSTR)prefix_spaces);
			fprintf(fp, "%soffset += encode_buf_len;\n", (LPCSTR)prefix_spaces);
			break;

		default:
			fprintf(fp, "[%s:%d] Unknown field type:%s, Please contact the author.\n", __THIS_FILE__, __LINE__, field->type_name());
			break;
		}
		if(desc->field(i)->is_repeated() || desc->field(i)->is_optional()){
			fprintf(fp, "    }\n");
		}
	}
	fprintf(fp, "    return offset;\n");
	fprintf(fp, "}\n");

	///////////////////////////////////////////////////////////////////////////
	// Decode function
	fprintf(fp, "\nBOOL decode_message_%s(BYTE *buf, size_t buf_len, %s* var_%s){\n", desc->name().c_str(), (LPCSTR)struct_name, desc->name().c_str());
	fprintf(fp, "	size_t offset = 0;\n");
	// 包含message字段时，才需要使用此变量
	for(int i=0;i<desc->field_count();++i){
		if(FieldDescriptor::TYPE_MESSAGE == desc->field(i)->type()){
			fprintf(fp, "	size_t tmp_field_len;\n");
			break;
		}
	}
	fprintf(fp, "    WORD field_num;\n");
	fprintf(fp, "    BYTE wire_type;\n");
	fprintf(fp, "\n    clear_message_%s(var_%s);\n\n", desc->name().c_str(), desc->name().c_str());

	fprintf(fp, "    for(;offset < buf_len;){\n");
	fprintf(fp, "        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);\n");
	fprintf(fp, "        switch(field_num){\n");
	for(int i=0;i<desc->field_count();++i){
		const FieldDescriptor *field = desc->field(i);
		CBString field_name_upper(field->name().c_str());
		field_name_upper.toupper();
		CBString field_containing_type_upper(field->containing_type()->name().c_str());
		field_containing_type_upper.toupper();

		fprintf(fp, "        /* type:%s */\n", field->type_name());
		fprintf(fp, "        case %d:\n", field->number());
		switch(field->type()){
		case FieldDescriptor::TYPE_FIXED32:
			if(field->is_repeated()){
				fprintf(fp, "            if(var_%s->var_%s.count >= MAX_%s_IN_%s) {\n", desc->name().c_str(), field->name().c_str(), 
					(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper);
				fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
				fprintf(fp, "            }\n");
				fprintf(fp, "            var_%s->var_%s.item[var_%s->var%s.count] = *((DWORD *)(buf + offset));\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += sizeof(DWORD);\n");
				fprintf(fp, "            var_%s->var%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
			} else if(field->is_optional()) {
				fprintf(fp, "            var_%s->var_%s = *((DWORD *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += sizeof(DWORD);\n");
				fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "            var_%s->var_%s = *((DWORD *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += sizeof(DWORD);\n");
			}
			fprintf(fp, "            break;\n");
			break;
		case FieldDescriptor::TYPE_FIXED64:
			if(field->is_repeated()){
				fprintf(fp, "            if(var_%s->var_%s.count >= MAX_%s_IN_%s) {\n", desc->name().c_str(), field->name().c_str(), 
					(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper);
				fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
				fprintf(fp, "            }\n");
				fprintf(fp, "            var_%s->var_%s.item[var_%s->var_%s.count] = *((WORD64 *)(buf + offset));\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += sizeof(WORD64);\n");
				fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
			} else if(field->is_optional()) {
				fprintf(fp, "            var_%s->var_%s = *((WORD64 *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += sizeof(WORD64);\n");
				fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "            var_%s->var_%s = *((WORD64 *)(buf + offset));\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += sizeof(WORD64);\n");
			}
			fprintf(fp, "            break;\n");
			break;
		case FieldDescriptor::TYPE_BOOL:
		case FieldDescriptor::TYPE_UINT32:
		case FieldDescriptor::TYPE_UINT64:
		case FieldDescriptor::TYPE_ENUM:
			if(field->is_repeated()){
				fprintf(fp, "            if(var_%s->var_%s.count >= MAX_%s_IN_%s) {\n", desc->name().c_str(), field->name().c_str(), 
					(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper);
				fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
				fprintf(fp, "            }\n");
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.item[var_%s->var_%s.count]), &offset);\n", desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
			} else if(field->is_optional()) {
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s), &offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s), &offset);\n", desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "            break;\n");
			break;
		case FieldDescriptor::TYPE_STRING:
			if(field->is_repeated()){
				fprintf(fp, "            if(var_%s->var_%s.count >= MAX_%s_IN_%s) {\n", desc->name().c_str(), field->name().c_str(), 
					(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper);
				fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
				fprintf(fp, "            }\n");
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.item[var_%s->var_%s.count].length), &offset);\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.item[var_%s->var_%s.count].data = (char *)(buf + offset);\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += var_%s->var_%s.item[var_%s->var_%s.count].length;\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
			} else if(field->is_optional()) {
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.data = (char *)(buf + offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.data = (char *)(buf + offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "            break;\n");
			break;
		case FieldDescriptor::TYPE_BYTES:
			if(field->is_repeated()){
				fprintf(fp, "            if(var_%s->var_%s.count >= MAX_%s_IN_%s) {\n", desc->name().c_str(), field->name().c_str(), 
					(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper);
				fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
				fprintf(fp, "            }\n");
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.item[var_%s->var_%s.count].length), &offset);\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.item[var_%s->var_%s.count].data = buf + offset;\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += var_%s->var_%s.item[var_%s->var_%s.count].length;\n", 
					desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
			} else if(field->is_optional()) {
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.data = buf + offset;\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
			} else {
				fprintf(fp, "            decode_varint(buf + offset, &(var_%s->var_%s.length), &offset);\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            var_%s->var_%s.data = buf + offset;\n", desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += var_%s->var_%s.length;\n", desc->name().c_str(), field->name().c_str());
			}
			fprintf(fp, "            break;\n");
			break;
		case FieldDescriptor::TYPE_MESSAGE:
			if(field->is_repeated()){
				fprintf(fp, "            if(var_%s->var_%s.count >= MAX_%s_IN_%s) {\n", desc->name().c_str(), field->name().c_str(), 
					(LPCSTR)field_name_upper, (LPCSTR)field_containing_type_upper);
				fprintf(fp, "                return FALSE;  /* 数组超限 */\n");
				fprintf(fp, "            }\n");
				fprintf(fp, "            decode_varint(buf + offset, &tmp_field_len, &offset);\n");
				fprintf(fp, "            decode_message_%s(buf + offset, tmp_field_len, &(var_%s->var_%s.item[var_%s->var_%s.count]));\n", field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += tmp_field_len;\n");
				fprintf(fp, "            var_%s->var_%s.count += 1;\n", desc->name().c_str(), field->name().c_str());
			} else if(field->is_optional()) {
				fprintf(fp, "            decode_varint(buf + offset, &tmp_field_len, &offset);\n");
				fprintf(fp, "            decode_message_%s(buf + offset, tmp_field_len, &(var_%s->var_%s));\n", field->message_type()->name().c_str(), desc->name().c_str(), field->name().c_str());
				fprintf(fp, "            offset += tmp_field_len;\n");
				fprintf(fp, "            var_%s->has_%s = TRUE;\n", desc->name().c_str(), field->name().c_str());
			} else {
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
	fprintf(fp, "/* end of file */\n");

	fclose(fp);
}

void gen_all_from_file(const FileDescriptor *f){
	for(int i=0; i<f->message_type_count(); ++i){
		const Descriptor* desc = f->message_type(i);
		gen_header(desc);
		gen_source(desc);

                /* close option file */
		if(NULL != fp_option){
			fclose(fp_option);
			fp_option = NULL;
		}
	}
}

class ImporterError : public compiler::MultiFileErrorCollector {
private:
	void AddError(
		const string & filename,
		int line,
		int column,
		const string & message) {
		printf("%s %d:%d -- %s", filename.c_str(), line, column, message.c_str());
	}
};

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s xxx.proto xxx.proto\n", argv[0]);
		return 1;
	}

	std::string str;
	const FileDescriptor *f;
	ImporterError errorCollector;
	compiler::DiskSourceTree sourceTree;
	compiler::Importer importer(&sourceTree, &errorCollector);
	sourceTree.MapPath("", ".");

	for(int i=1;i<argc;++i){
		proto_filename = argv[i];
		f = importer.Import(proto_filename);
		if (NULL == f) {
			printf("Cannot import file:%s", proto_filename);
			return 2;
		}
		gen_all_from_file(f);
	}
	// common header file
	gen_comm();

	return 0;
}

