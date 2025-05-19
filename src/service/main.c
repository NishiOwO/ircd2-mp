#include "os.h"
#include "mpserv.h"

#ifdef _WIN32
#include <direct.h>
#endif

int ircport = 6667;
char* servhost = NULL;
char* servpass = NULL;
char servtoken[5];

FILE* db_user;
FILE* db_chan;

/* note: this breaks string */
char* getfield(char* str, int* cursor){
	int i = 0;
	int old = *cursor;
	while(str[(old) + i] != ':' && str[(old) + i] != 0) i++;
	str[(old) + i] = 0;
	*cursor = old + i + 1;
	return &str[old];
}

#ifdef POSIX_SIGNALS
void term(int sig){
	vasend(ircfd, ":ChanServ QUIT :Interrupted!\r\n");
	vasend(ircfd, ":NickServ QUIT :Interrupted!\r\n");
	vasend(ircfd, ":OperServ QUIT :Interrupted!\r\n");
	vasend(ircfd, ":%s SQUIT\r\n", servhost);
}
#endif

int main(int argc, char** argv){
	FILE* f;
	const char* confpath = IRCDCONF_DIR;
	char* buf;
	int sz;
	int i;
	int old = 0;

	printf("ircd2+mp service starting\n");
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			confpath = argv[i + 1];
			if(confpath == NULL){
				fprintf(stderr, "Missing path\n");
				return 1;
			}
		}
	}

	users = NULL;

	if(chdir(confpath) != 0){
		fprintf(stderr, "chdir failed\n");
		return 1;
	}

	if((db_user = dbopen("user.db")) == NULL){
		fprintf(stderr, "dbopen failed\n");
		return 1;
	}

	if((db_chan = dbopen("chan.db")) == NULL){
		fprintf(stderr, "dbopen failed\n");
		return 1;
	}

	f = fopen("ircd.conf", "r");
	if(f == NULL){
		fprintf(stderr, "Could not open %s/ircd.conf\n", confpath);
		return 1;
	}
	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = malloc(sz + 1);
	fread(buf, sz, 1, f);
	buf[sz] = 0;
	fclose(f);

	for(i = 0;; i++){
		if(buf[i] == '\n' || buf[i] == 0){
			char oldc = buf[i];
			char* line = buf + old;
			buf[i] = 0;
			if(strlen(line) > 0 && line[0] != '#'){
				int c = 0;
				char* l = getfield(line, &c);
				if(strcmp(l, "M") == 0){
					char* domain = getfield(line, &c);
					char* extip = getfield(line, &c);
					char* place = getfield(line, &c);
					char* port = getfield(line, &c);
					char* sid = getfield(line, &c);
					int n = 0;
					printf("Host = %s, Port = %d\n", domain, (ircport = atoi(port)));
					while(1){
						sprintf(servtoken, "%04d", n);
						if(strcmp(servtoken, sid) != 0) break;
					}
				}else if(strcmp(l, "S") == 0){
					char* host = getfield(line, &c);
					char* pass = getfield(line, &c);
					char* srvname = getfield(line, &c);
					char* srvtype = getfield(line, &c);
					char* class = getfield(line, &c);
					if(strcmp(srvname, SERVNAME) == 0){
						printf("Service host = %s\n", host);
						if(servhost != NULL) free(servhost);
						if(servpass != NULL) free(servpass);
						servhost = copystr(host);
						servpass = copystr(pass);
					}
				}
			}
			old = i + 1;
			if(oldc == 0) break;
		}
	}
	
	free(buf);

	if(servhost == NULL || servpass == NULL){
		fprintf(stderr, "Could not find S-line\n");
		return 1;
	}

	if(mpconnect() < 0){
		return 1;
	}

	servinit();
#ifdef POSIX_SIGNALS
	signal(SIGINT, term);
	signal(SIGTERM, term);
#endif
	servloop();

	printf("Shutdown\n");

	dbclose(db_user);
	dbclose(db_chan);

	return 0;
}
