#include "os.h"
#include "mpserv.h"

#include "stb_ds.h"

void channel_scan(void){
	int i;
	if(chans != NULL){
		for(i = 0; i < arrlen(chans); i++){
			dbdata_t d;
			int first = !chans[i].joined;

			if(arrlen(chans[i].users) == 0 && !first){
				vasend(ircfd, ":OperServ PART %s\r\n", chans[i].name);
				vasend(ircfd, ":ChanServ PART %s\r\n", chans[i].name);
				arrfree(chans[i].users);
				arrdel(chans, i);
				i = -1;
				continue;
			}

			if(first) vasend(ircfd, ":%s NJOIN %s @OperServ,@ChanServ\r\n", servhost, chans[i].name);
			chans[i].joined = 1;
			if(dbget(db_chan, chans[i].name, &d)){
				dbchan_t* c = (dbchan_t*)&d.value[0];
				int j;
				char buf[TOPICSIZE + 1];
				char buf2[USERSIZE + 1];
				buf[TOPICSIZE] = 0;
				buf2[USERSIZE] = 0;
				memcpy(buf, c->topic, TOPICSIZE);
				memcpy(buf2, c->owner, USERSIZE);
				if(first) vasend(ircfd, ":ChanServ TOPIC %s %s\r\n", chans[i].name, buf);
				for(j = 0; j < arrlen(chans[i].users); j++){
					if(strcmp(chans[i].users[j].name, buf2) == 0 && isauth(chans[i].users[j].name)){
						if(chans[i].users[j].checked == -1) chans[i].users[j].checked = 0;
						if(!chans[i].users[j].checked){
							vasend(ircfd, ":ChanServ MODE %s +o %s\r\n", chans[i].name, chans[i].users[j].name);
						}
						chans[i].users[j].checked = 1;
					}else{
						if(chans[i].users[j].checked == 1) chans[i].users[j].checked = 0;
						if(!chans[i].users[j].checked){
							vasend(ircfd, ":ChanServ MODE %s -o %s\r\n", chans[i].name, chans[i].users[j].name);
						}
						chans[i].users[j].checked = -1;
					}
				}
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
		channel_scan();
	}
}
