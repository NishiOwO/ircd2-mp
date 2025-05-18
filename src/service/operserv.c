#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void operserv(void){
	if(strcmp(ircpresp.cmd, "PRIVMSG") == 0 && arrlen(ircpresp.param) > 1 && strcmp(ircpresp.param[0], "OperServ") == 0){
		if(isoper(ircpresp.from)){
			char** args = argsplit(ircpresp.param[1]);
			if(args != NULL){
				if(mpstrcasecmp(args[0], "MODE") == 0){
					if(arrlen(args) >= 4){
						vasend(ircfd, ":OperServ MODE %s %s %s\r\n", args[1], args[2], args[3]);
					}else{
						vasend(ircfd, ":OperServ NOTICE %s :Incorrect arguments.\r\n", ircpresp.from);
					}
				}else if(mpstrcasecmp(args[0], "KICK") == 0){
					if(arrlen(args) >= 3){
						vasend(ircfd, ":%s KICK %s %s\n", servhost, args[1], args[2]);
					}else{
						vasend(ircfd, ":OperServ NOTICE %s :Incorrect arguments.\r\n", ircpresp.from);
					}
				}else{
					vasend(ircfd, ":OperServ NOTICE %s :Invalid subcommand.\r\n", ircpresp.from);
				}
				argfree(args);
			}
		}else{
			vasend(ircfd, ":OperServ NOTICE %s :Access denied.\r\n", ircpresp.from);
		}
	}
}

void operserv_init(void){
	adduser(ircfd, "OperServ");
}
