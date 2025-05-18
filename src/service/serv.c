#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void channel_scan(void){
	int i;
	if(chans != NULL){
		for(i = 0; i < arrlen(chans); i++){
			dbdata_t d;
			int first = !chans[i].joined;
			if(first) vasend(ircfd, ":%s NJOIN %s @OperServ,@ChanServ\r\n", servhost, chans[i].name);
			chans[i].joined = 1;
			if(dbget(db_chan, chans[i].name, &d)){
				dbchan_t* c = (dbchan_t*)&d.value[0];
				char buf[TOPICSIZE + 1];
				buf[TOPICSIZE] = 0;
				memcpy(buf, c->topic, TOPICSIZE);
				if(first) vasend(ircfd, ":ChanServ TOPIC %s %s\r\n", chans[i].name, buf);
			}
		}
	}
}

void servinit(void){
	adduser(ircfd, SERVNAME);
	chanserv_init();
	nickserv_init();
	operserv_init();
	channel_scan();
	vasend(ircfd, ":%s EOB\r\n", servtoken);
}

void servloop(void){
	while(readresp(ircfd) >= 0){
		parseresp();
		common();
		channel_scan();
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
