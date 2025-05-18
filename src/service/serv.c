#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void servinit(void){
	adduser(ircfd, SERVNAME);
	chanserv_init();
	nickserv_init();
	operserv_init();
	vasend(ircfd, ":%s EOB\r\n", servtoken);
}

void servloop(void){
	while(readresp(ircfd) >= 0){
		parseresp();
		common();
		if(strcmp(ircpresp.cmd, "PING") == 0){
			vasend(ircfd, ":%s PONG %s %s\r\n", servtoken, servtoken, ircpresp.param[0]);
		}else{
			if(strcmp(ircpresp.cmd, "PRIVMSG") == 0 && arrlen(ircpresp.param) > 1 && ircpresp.param[1][0] == 1 && ircpresp.param[1][strlen(ircpresp.param[1]) - 1] == 1){
				/* ctcp */
				if(strcmp(ircpresp.param[1], "\001VERSION\001") == 0){
					vasend(ircfd, ":%s NOTICE %s :\001VERSION %s version %s\001\r\n", ircpresp.param[0], ircpresp.from, SERVNAME, SERVVER);
				}
				continue;
			}
			chanserv();
			nickserv();
			operserv();
		}
	}
}
