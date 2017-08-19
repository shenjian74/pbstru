// test_codec.cpp : 定义控制台应用程序的入口点。
//

#pragma warning(disable:4996)  

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "addrequest.h"
#include "response.h"

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

st_AddRequest var_AddRequest;
int main(int argc, char* argv[])
{
	BYTE buf[1024];
	size_t buf_len;

	printf("sizeof(st_AddRequest) == %lu\n", sizeof(st_AddRequest));

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

	printf("%s Done.\n", argv[0]);
	getchar();
}


