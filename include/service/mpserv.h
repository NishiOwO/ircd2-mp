#ifndef __MPSERV_H__
#define __MPSERV_H__

#define SERVNAME	"MPService"
#define SERVVER		"1.0"

#define KEYSIZE		512
#define VALSIZE		2048
#define MODESIZE	256
#define USERSIZE	256

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

typedef struct ircuser_ {
	int ident;
	char uid[USERSIZE];
	char name[USERSIZE];
	char mode[MODESIZE];
} ircuser_t;

extern int ircport;
extern char* servhost;
extern char* servpass;
extern char servtoken[5];
extern char* ircresp;
extern ircresp_t ircpresp;
extern int ircfd;
extern ircuser_t* users;

extern FILE* db_user;
extern FILE* db_chan;

char* copystr(char*);

int readresp(int fd);
int vasend(int fd, char* fmt, ...);
void parseresp(void);

void adduser(int fd, const char* name);

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

#endif
