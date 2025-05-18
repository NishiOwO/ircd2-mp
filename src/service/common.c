#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

ircuser_t* users;
void common(void){
	if(strcmp(ircpresp.cmd, "UNICK") == 0 && arrlen(ircpresp.param) >= 7){
		ircuser_t u;
		dbdata_t d;
		int i;

		strcpy(u.name, ircpresp.param[0]);
		u.mode[0] = 0;
		u.ident = 1; /* avoid sending identify message */
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
				printf("Removed UID = %s\n", users[i].uid);
				arrdel(users, i);
				break;
			}
		}
	}
}
