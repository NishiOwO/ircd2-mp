#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void nickserv_scan(void){
	int i;
	for(i = 0; i < arrlen(users); i++){
		if(users[i].ident == 0){
			vasend(ircfd, ":NickServ NOTICE %s :This nickname is registered and protected - If it is your nickname, type \002/msg NickServ IDENTIFY \037password\017. Otherwise, please choose a different nickname.\r\n", users[i].name);
			users[i].ident = 1;
		}else if(users[i].ident == -1){
			vasend(ircfd, ":NickServ NOTICE %s :This nickname isn't registered yet - If you want this nickname, type \002/msg NickServ REGISTER \037password\017 to register.\r\n", users[i].name);
			users[i].ident = 1;
		}
	}
}

void nickserv(void){
	if(strcmp(ircpresp.cmd, "PRIVMSG") == 0 && arrlen(ircpresp.param) > 1 && strcmp(ircpresp.param[0], "NickServ") == 0){
		char** args = argsplit(ircpresp.param[1]);
		nickserv_scan();
		if(args != NULL){
			if(mpstrcasecmp(args[0], "REGISTER") == 0){
				dbdata_t d;
				if(dbget(db_user, ircpresp.from, &d)){
					vasend(ircfd, ":NickServ NOTICE %s :This nickname is already registered.\r\n", ircpresp.from);
				}else if(arrlen(args) >= 2 && strlen(args[1]) <= PASSSIZE){
					dbuser_t* u = (dbuser_t*)&d.value[0];
					int i;

					for(i = 0; i < arrlen(users); i++){
						if(strcmp(users[i].name, ircpresp.from) == 0){
							users[i].auth = 1;
							break;
						}
					}

					d.size = sizeof(*u);

					/* this is bad! think a better solution. */
					memset(u->pass, 0, PASSSIZE);
					memcpy(u->pass, args[1], strlen(args[1]));

					u->expire = time(NULL) + NICKEXPIRE;
					dbset(db_user, ircpresp.from, &d);
					vasend(ircfd, ":NickServ NOTICE %s :Nickname got registered successfully.\r\n", ircpresp.from);
				}else{
					vasend(ircfd, ":NickServ NOTICE %s :Arguments are incorrect, or nickname is too long.\r\n", ircpresp.from);
				}
			}else if(mpstrcasecmp(args[0], "IDENTIFY") == 0){
				dbdata_t d;
				int i;
				int ath = 0;

				for(i = 0; i < arrlen(users); i++){
					if(strcmp(users[i].name, ircpresp.from) == 0){
						ath = users[i].auth;
						break;
					}
				}

				if(!dbget(db_user, ircpresp.from, &d)){
					vasend(ircfd, ":NickServ NOTICE %s :This nickname is not registered.\r\n", ircpresp.from);
				}else if(ath){
					vasend(ircfd, ":NickServ NOTICE %s :You are already identified.\r\n", ircpresp.from);
				}else if(arrlen(args) >= 2){
					char pass[PASSSIZE + 1];
					dbuser_t* u = (dbuser_t*)&d.value[0];
					pass[PASSSIZE] = 0;

					memcpy(pass, u->pass, PASSSIZE);
					if(strcmp(args[1], pass) == 0){
						for(i = 0; i < arrlen(users); i++){
							if(strcmp(users[i].name, ircpresp.from) == 0){
								users[i].auth = 1;
								break;
							}
						}

						u->expire = time(NULL) + NICKEXPIRE;
						dbset(db_user, ircpresp.from, &d);

						vasend(ircfd, ":NickServ NOTICE %s :Welcome back.\r\n", ircpresp.from);
					}else{
						vasend(ircfd, ":NickServ NOTICE %s :Password incorrect.\r\n", ircpresp.from);
					}
				}else{
					vasend(ircfd, ":NickServ NOTICE %s :Incorrect arguments.\r\n", ircpresp.from);
				}
			}
			argfree(args);
		}
	}else if(strcmp(ircpresp.cmd, "UNICK") == 0){
		nickserv_scan();
	}
}

void nickserv_init(void){
	adduser(ircfd, "NickServ");
	nickserv_scan();
}
