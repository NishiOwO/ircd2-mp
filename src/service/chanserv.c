#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void chanserv(void){
	if(strcmp(ircpresp.cmd, "PRIVMSG") == 0 && arrlen(ircpresp.param) > 1 && strcmp(ircpresp.param[0], "ChanServ") == 0){
		char** args = argsplit(ircpresp.param[1]);
		if(args != NULL){
			dbdata_t d;
			if(mpstrcasecmp(args[0], "REGISTER") == 0){
				if(arrlen(args) < 3 || args[1][0] != '#'){
					vasend(ircfd, ":ChanServ NOTICE %s :Incorrect arguments.\r\n", ircpresp.from);
				}else if(dbget(db_chan, args[1], &d)){
					vasend(ircfd, ":ChanServ NOTICE %s :Channel is already registered.\r\n", ircpresp.from);
				}else if(!isauth(ircpresp.from)){
					vasend(ircfd, ":ChanServ NOTICE %s :Identify yourself first.\r\n", ircpresp.from);
				}else{
					int i;
					dbchan_t* c = (dbchan_t*)&d.value[0];
					memset(d.value, 0, sizeof(d.value));

					d.size = sizeof(*c);

					memset(c->pass, 0, PASSSIZE);
					memcpy(c->pass, args[2], strlen(args[2]));

					memset(c->owner, 0, USERSIZE);
					memcpy(c->owner, ircpresp.from, strlen(ircpresp.from));

					memset(c->topic, 0, TOPICSIZE);

					for(i = 0; i < arrlen(chans); i++){
						int j;
						if(strcmp(chans[i].name, args[1]) == 0){
							for(j = 0; j < arrlen(chans[i].users); j++){
								if(strcmp(chans[i].users[j].name, ircpresp.from) == 0){
									if(chans[i].users[j].op){
										vasend(ircfd, ":ChanServ NOTICE %s :Channel got registered successfully.\n", ircpresp.from);

										dbset(db_chan, args[1], &d);
									}else{
										vasend(ircfd, ":ChanServ NOTICE %s :You have to be chanop to register channel.\n", ircpresp.from);
									}
									break;
								}
							}
							break;
						}
					}
				}
			}else if(mpstrcasecmp(args[0], "TOPIC") == 0){
				if(arrlen(args) < 3 || args[1][0] != '#'){
					vasend(ircfd, ":ChanServ NOTICE %s :Incorrect arguments.\r\n", ircpresp.from);
				}else if(!dbget(db_chan, args[1], &d)){
					vasend(ircfd, ":ChanServ NOTICE %s :Channel is not registered.\r\n", ircpresp.from);
				}else if(!isauth(ircpresp.from)){
					vasend(ircfd, ":ChanServ NOTICE %s :Identify yourself first.\r\n", ircpresp.from);
				}else{
					dbchan_t* c = (dbchan_t*)&d.value[0];
					char buf[USERSIZE + 1];
					buf[USERSIZE] = 0;
					memcpy(buf, c->owner, USERSIZE);

					if(strcmp(buf, ircpresp.from) == 0){
						memset(c->topic, 0, TOPICSIZE);
						memcpy(c->topic, args[2], strlen(args[2]));

						vasend(ircfd, ":ChanServ TOPIC %s :%s\r\n", args[1], args[2]);

						dbset(db_chan, args[1], &d);
					}else{
						vasend(ircfd, ":ChanServ NOTICE %s :You don't own this channel.\r\n", ircpresp.from);
					}
				}
			}else if(mpstrcasecmp(args[0], "DROP") == 0){
				dbdata_t d;

				if(arrlen(args) < 1 || args[1][0] != '#'){
					vasend(ircfd, ":ChanServ NOTICE %s :Incorrect arguments.\r\n", ircpresp.from);
				}else if(!dbget(db_chan, args[1], &d)){
					vasend(ircfd, ":ChanServ NOTICE %s :This channel is not registered.\r\n", ircpresp.from);
				}else if(!isauth(ircpresp.from) && 0){
					vasend(ircfd, ":ChanServ NOTICE %s :Identify first.\r\n", ircpresp.from);
				}else{
					dbchan_t* c = (dbchan_t*)&d.value[0];
					char buf[USERSIZE + 1];
					buf[USERSIZE] = 0;
					memcpy(buf, c->owner, USERSIZE);
					if(strcmp(buf, ircpresp.from) == 0){
						vasend(ircfd, ":ChanServ NOTICE %s :Dropped channel.\r\n", ircpresp.from);
						dbdel(db_chan, args[1]);
					}else{
						vasend(ircfd, ":ChanServ NOTICE %s :You don't own this channel.\r\n", ircpresp.from);
					}
				}
			}else{
				vasend(ircfd, ":ChanServ NOTICE %s :Invalid subcommand.\r\n", ircpresp.from);
			}
			argfree(args);
		}
	}
}

void chanserv_init(void){
	adduser(ircfd, "ChanServ");
}
