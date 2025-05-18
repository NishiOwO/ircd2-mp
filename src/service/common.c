#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

ircuser_t* users;
ircchan_t* chans = NULL;
void common(void){
	if(strcmp(ircpresp.cmd, "UNICK") == 0 && arrlen(ircpresp.param) >= 7){
		ircuser_t u;
		dbdata_t d;
		int i;

		strcpy(u.name, ircpresp.param[0]);
		u.mode[0] = 0;
		u.auth = 0;
		u.ident = -1; /* send message to remind you to register. :) */
		strcpy(u.uid, ircpresp.param[1]);

		for(i = 0; ircpresp.param[5][i] != 0; i++){
			if(ircpresp.param[5][i] != '+'){
				int l = strlen(u.mode);
				u.mode[l] = ircpresp.param[5][i];
				u.mode[l + 1] = 0;
			}
		}

		if(dbget(db_user, ircpresp.param[0], &d)){
			/* user is registered */
			u.ident = 0;
		}

		printf("Detected User = %s, Mode = %s, UID = %s\n", u.name, u.mode, u.uid);
		arrput(users, u);
	}else if(strcmp(ircpresp.cmd, "QUIT") == 0 && arrlen(ircpresp.param) >= 1){
		int i;
		for(i = 0; i < arrlen(users); i++){
			if(strcmp(users[i].uid, ircpresp.from) == 0){
				int j;
				printf("Removed UID = %s\n", users[i].uid);
				arrdel(users, i);

				for(j = 0; j < arrlen(chans); j++){
					int k;
					for(k = 0; k < arrlen(chans[j].users); k++){
						if(strcmp(chans[j].users[k].name, users[i].name) == 0){
							free(chans[j].users[k].name);
							arrdel(chans[j].users, k);
							break;
						}
					}
				}

				break;
			}
		}
	}else if(strcmp(ircpresp.cmd, "MODE") == 0 && arrlen(ircpresp.param) == 2){
		int i;
		for(i = 0; i < arrlen(users); i++){
			if(strcmp(users[i].name, ircpresp.param[0]) == 0){
				int j;
				int k;
				for(k = 1; ircpresp.param[1][k] != 0; k++){
					int incl = 0;
					for(j = 0; users[i].mode[j] != 0; j++){
						if(users[i].mode[j] == ircpresp.param[1][k]){
							incl = 1;
							break;
						}
					}
					if(!incl && ircpresp.param[1][0] == '+'){
						int s = strlen(users[i].mode);
						users[i].mode[s] = ircpresp.param[1][k];
						users[i].mode[s + 1] = 0;
					}else if(incl && ircpresp.param[1][0] == '-'){
						int l;
						for(l = j; users[i].mode[l] != 0; l++){
							users[i].mode[l] = users[i].mode[l + 1];
						}
					}
				}
				printf("Mode change User = %s, Mode = %s\n", users[i].name, users[i].mode);
				break;
			}
		}
	}else if(strcmp(ircpresp.cmd, "NJOIN") == 0 && arrlen(ircpresp.param) >= 2){
		ircchan_t c;
		int i;
		int k;
		int has = 0;
		int old = 0;
		strcpy(c.name, ircpresp.param[0]);
		c.joined = 0;
		c.users = NULL;
		for(i = 0; i < arrlen(chans); i++){
			if(strcmp(c.name, chans[i].name) == 0){
				has = 1;
				break;
			}
		}
		for(k = 0;; k++){
			char ch = ircpresp.param[1][k];
			if(ch == ',' || ch == 0){
				int m = (ircpresp.param[1][old] == '@' && ircpresp.param[1][old + 1] == '@') ? 2 : ((ircpresp.param[1][old] == '@' || ircpresp.param[1][old] == '+') ? 1 : 0);
				char* s = malloc(k - old + 1 - m);
				char* u;
				int j;
				ircchanuser_t cu;
				memcpy(s, ircpresp.param[1] + old + m, k - old - m);
				s[k - old - m] = 0;

				/* s seems to be UID */
				for(j = 0; j < arrlen(users); j++){
					if(strcmp(users[j].uid, s) == 0){
						u = copystr(users[j].name);
						break;
					}
				}

				cu.name = u;
				cu.checked = ircpresp.param[1][old] == '@' ? 0 : -1;
				if(has){
					int userin = 0;
					for(j = 0; j < arrlen(chans[i].users); j++){
						if(strcmp(chans[i].users[j].name, u) == 0){
							userin = 1;
							break;
						}
					}
					if(!userin){
						arrput(chans[i].users, cu);
					}else{
						free(u);
					}
				}else{
					arrput(c.users, cu);
				}

				free(s);
				old = k + 1;
				if(ch == 0) break;
			}
		}
		if(!has){
			arrput(chans, c);
		}
	}else if(strcmp(ircpresp.cmd, "PART") == 0 && arrlen(ircpresp.param) >= 1){
		int i;
		for(i = 0; i < arrlen(chans); i++){
			if(strcmp(ircpresp.param[0], chans[i].name) == 0){
				int j;
				char* u;
				for(j = 0; j < arrlen(users); j++){
					if(strcmp(users[j].uid, ircpresp.from) == 0){
						u = copystr(users[j].name);
						break;
					}
				}
				for(j = 0; j < arrlen(chans[i].users); j++){
					if(strcmp(chans[i].users[j].name, u) == 0){
						free(chans[i].users[j].name);
						arrdel(chans[i].users, j);
						break;
					}
				}
				free(u);
				break;
			}
		}
	}
}
