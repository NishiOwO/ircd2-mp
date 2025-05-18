#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

char* copystr(const char* str){
	char* s = malloc(strlen(str) + 1);
	strcpy(s, str);
	return s;
}

char** argsplit(const char* arg){
	char** r = NULL;
	char* b = copystr(arg);
	int i;
	int old = 0;
	for(i = 0;; i++){
		if(b[i] == ' ' || b[i] == 0){
			char oldc = b[i];
			char* s;
			b[i] = 0;
			s = copystr(b + old);
			arrput(r, s);
			old = i + 1;
			if(oldc == 0) break;
		}
	}
	free(b);
	return r;
}

void argfree(char** args){
	int i;
	for(i = 0; i < arrlen(args); i++){
		free(args[i]);
	}
	arrfree(args);
}

int vasend(int fd, const char* fmt, ...){
	char buf[1024];
	va_list va;
	va_start(va, fmt);
	vsprintf(buf, fmt, va);
	va_end(va);
	return send(fd, buf, strlen(buf), 0);
}

char* ircresp = NULL;
int readresp(int fd){
	char c;
	if(ircresp != NULL) free(ircresp);
	ircresp = malloc(1);
	ircresp[0] = 0;
	while(1){
		int st = recv(fd, &c, 1, 0);
		if(st < 1) return -1;
		if(c == '\n') break;
		if(c != '\r'){
			char* old = ircresp;
			ircresp = malloc(strlen(old) + 2);
			strcpy(ircresp, old);
			ircresp[strlen(old)] = c;
			ircresp[strlen(old) + 1] = 0;
			free(old);
		}
	}
	return 0;
}

ircresp_t ircpresp;
void parseresp(void){
	char* line = ircresp;
	int i;
	char old;

	if(ircpresp.param != NULL){
		arrfree(ircpresp.param);
	}
	ircpresp.param = NULL;
	ircpresp.from = NULL;
	if(ircresp[0] == ':'){
		for(i = 0; line[i] != ' ' && line[i] != 0; i++);
		line[i] = 0;
		ircpresp.from = line + 1;
		line += i + 1;
	}
	for(i = 0; line[i] != ' ' && line[i] != 0; i++);
	old = line[i];
	line[i] = 0;
	ircpresp.cmd = line;
	line += i + 1;
	if(old == 0) return;

	while(1){
		char c = line[0];
		for(i = 0; line[i] != ' ' && line[i] != 0; i++);
		if(c != ':'){
			line[i] = 0;
		}else{
			line++;
		}
		arrput(ircpresp.param, line);
		line += i + 1;
		if(line[0] == 0 || c == ':') break;
	}
}

int isoper(const char* name){
	int i;
	for(i = 0; i < arrlen(users); i++){
		if(strcmp(users[i].name, name) == 0){
			int j;
			for(j = 0; users[i].mode[j] != 0; j++){
				if(users[i].mode[j] == 'o') return 1;
			}
			break;
		}
	}
	return 0;
}

int isauth(const char* name){
	int i;
	for(i = 0; i < arrlen(users); i++){
		if(strcmp(users[i].name, name) == 0) return users[i].auth;
	}
	return 0;
}

void setauth(const char* name, int auth){
	int i;
	for(i = 0; i < arrlen(users); i++){
		if(strcmp(users[i].name, name) == 0){
			users[i].auth = auth;
			break;
		}
	}
}
