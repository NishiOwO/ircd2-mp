#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void operserv(void){
	if(strcmp(ircpresp.cmd, "PRIVMSG") == 0 && arrlen(ircpresp.param) > 1 && strcmp(ircpresp.param[0], "OperServ") == 0){
		int i;
		int oper = 0;
		for(i = 0; i < arrlen(users); i++){
			if(strcmp(users[i].name, ircpresp.from) == 0){
				int j;
				for(j = 0; users[i].mode[j] != 0; j++){
					if(users[i].mode[j] == 'o'){
						/* this user is server op, allow using operserv */
						oper = 1;
						break;
					}
				}
				break;
			}
		}
		if(oper){
		}else{
			vasend(ircfd, ":OperServ NOTICE %s :Access denied\r\n", ircpresp.from);
		}
	}
}

void operserv_init(void){
	adduser(ircfd, "OperServ");
}
