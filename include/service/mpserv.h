#ifndef __MPSERV_H__
#define __MPSERV_H__

#define SERVNAME	"MPService"
#define SERVVER		"1.0"

#define KEYSIZE		512
#define VALSIZE		2048
#define MODESIZE	256
#define USERSIZE	256
#define CHANSIZE	256
#define PASSSIZE	256
#define TOPICSIZE	256

/* 30 days */
#define NICKEXPIRE	(30*24*60*60)

#include <stdio.h>

typedef struct ircresp_ {
	char* from;
	char* cmd;
	char** param;
} ircresp_t;

typedef struct dbdata_ {
	int size;
	unsigned char value[VALSIZE];
} dbdata_t;

#pragma pack(1)
typedef struct dbuser_ {
	char pass[PASSSIZE];
	time_t expire;
} dbuser_t;

typedef struct dbchan_ {
	char owner[USERSIZE];
	char pass[PASSSIZE];
	char topic[TOPICSIZE];
} dbchan_t;
#pragma pack()

typedef struct ircuser_ {
	int ident;
	int auth;
	char uid[USERSIZE + 1];
	char name[USERSIZE + 1];
	char mode[MODESIZE + 1];
} ircuser_t;

typedef struct ircchan_ {
	char name[CHANSIZE + 1];
	int joined;
	char** users;
} ircchan_t;

extern int ircport;
extern char* servhost;
extern char* servpass;
extern char servtoken[5];
extern char* ircresp;
extern ircresp_t ircpresp;
extern int ircfd;
extern ircuser_t* users;
extern ircchan_t* chans;

extern FILE* db_user;
extern FILE* db_chan;

char* copystr(const char* str);
char** argsplit(const char* arg);
void argfree(char** args);

int readresp(int fd);
int vasend(int fd, const char* fmt, ...);
void parseresp(void);

void adduser(int fd, const char* name);

int isauth(const char* name);
void setauth(const char* name, int auth);
int isoper(const char* name);

void servinit(void);
void servloop(void);
void chanserv(void);
void nickserv(void);
void operserv(void);
void chanserv_init(void);
void nickserv_init(void);
void operserv_init(void);
void common(void);

int mpconnect(void);

FILE* dbopen(const char* path);
void dbdel(FILE* f, const char* key);
void dbset(FILE* f, const char* key, dbdata_t* val);
int dbget(FILE* f, const char* key, dbdata_t* val);
void dbclose(FILE* f);

#ifdef _WIN32
#define mpstrcasecmp _stricmp
#else
#define mpstrcasecmp strcasecmp
#endif

#endif
