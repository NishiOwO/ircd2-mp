#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void nickserv_scan(void){
	int i;
	for(i = 0; i < arrlen(users); i++){
		if(users[i].ident == 0){
			vasend(ircfd, ":NickServ NOTICE %s :This nickname is registered and protected - If it is your nickname, type \002/msg NickServ IDENTIFY \037password\017. Otherwise, please choose a different nickname.\r\n", users[i].name);
			users[i].ident = 1;
		}
	}
}

void nickserv(void){
	if(strcmp(ircpresp.cmd, "PRIVMSG") == 0 && arrlen(ircpresp.param) > 1 && strcmp(ircpresp.param[0], "NickServ") == 0){
		dbdata_t d;
		d.size = 0;
		dbset(db_user, ircpresp.from, &d);
	}else if(strcmp(ircpresp.cmd, "UNICK") == 0){
		nickserv_scan();
	}
}

void nickserv_init(void){
	adduser(ircfd, "NickServ");
	nickserv_scan();
}
