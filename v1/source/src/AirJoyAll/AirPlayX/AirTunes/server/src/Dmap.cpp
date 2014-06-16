/*
 * Dmap.cpp
 *
 * coding.tom@gmail.com
 *
 * 2013-2-6
 *
 */

#include "Dmap.hpp"
#include "dmap_parser.h"

using namespace airjoy;

typedef std::map<std::string, std::string> DmapKeyValueType;
static DmapKeyValueType g_dmapKeyValue;
static std::string g_dmapEmptyString;

static void on_dict_start(void *ctx, const char *code, const char *name) 
{
	//printf("on_dict_start: %s:\n", name);
	//indent();
}

static void on_dict_end(void *ctx, const char *code, const char *name) 
{
	//outdent();
}

static void on_int32(void *ctx, const char *code, const char *name, int32_t value) 
{
	//printf("on_int32: %s: %d\n", name, value);
}

static void on_int64(void *ctx, const char *code, const char *name, int64_t value) 
{
	//printf("on_int64: %s: %lld\n", name, value);
}

static void on_uint32(void *ctx, const char *code, const char *name, uint32_t value) 
{
	//printf("on_uint32: %s: %u\n", name, value);
}

static void on_uint64(void *ctx, const char *code, const char *name, uint64_t value) 
{
	//printf("on_uint64: %s: %llu\n", name, value);
}

static void on_date(void *ctx, const char *code, const char *name, uint32_t value) 
{
#if 0
    printf("on_date\n");

	char buf[32];
	time_t timeval = value;
	struct tm* timestruct = gmtime(&timeval);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S +0000", timestruct);
	printf("%s: %s\n", name, buf);
#endif
}

static void on_string(void *ctx, const char *code, const char *name, const char *buf, size_t len) 
{
    g_dmapKeyValue.insert(std::make_pair(name, std::string(buf, len)));

#if 0
	char *str = (char *)malloc(len + 1);
	strncpy(str, buf, len);
	str[len] = '\0';
	printf("on_string: %s: %s\n", name, str);
	free(str);
#endif
}

static void on_data(void *ctx, const char *code, const char *name, const char *buf, size_t len) 
{
#if 0
    const char hexchars[] = "0123456789abcdef";

	char *str = (char *)malloc((len * 3) + 1);
	size_t i;
	char *p = str;
	for (i = 0; i < len; i++) 
    {
		if (i > 0)
			*p++ = ' ';

		*p++ = hexchars[buf[i] >> 4];
		*p++ = hexchars[buf[i] & 0x0f];
	}
	*p = '\0';
	printf("on_data: %s: <%s>\n", name, str);
	free(str);
#endif
}

Dmap::Dmap(const std::string &data)
{
    g_dmapKeyValue.clear();

	dmap_settings settings = 
    {
		on_dict_start,
		on_dict_end,
		on_int32,
		on_int64,
		on_uint32,
		on_uint64,
		on_date,
		on_string,
		on_data,
		NULL
	};

    int result = dmap_parse(&settings, data.c_str(),data.size());
}

Dmap::~Dmap()
{
}

const std::string & Dmap::get(const std::string &name)
{
    DmapKeyValueType::iterator pos = g_dmapKeyValue.find(name);
    if (pos == g_dmapKeyValue.end())
        return g_dmapEmptyString;

    return pos->second;
}
