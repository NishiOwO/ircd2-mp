#include "os.h"
#include "mpserv.h"

#ifndef _WIN32
#include <netinet/tcp.h>
#endif

int ircfd;
int incr = 0;

void adduser(int s, const char* name){
	vasend(s, ":%s UNICK %s %s%05d %s %s 0.0.0.0 %s :%s\r\n", servtoken, name, servtoken, incr, name, servhost, "+io", name);
	incr++;
}

/* currently only IPv4... sorry! */
int mpconnect(void){
	struct sockaddr_in addr;
	int val = 1;

	ircpresp.param = NULL;

	incr = 0;

	ircfd = socket(AFINET, SOCK_STREAM, 0);
	if(ircfd == -1) return -1;

	setsockopt(ircfd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(ircport);

	if(connect(ircfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
		CLOSESOCK(ircfd);
		return -1;
	}

	vasend(ircfd, "PASS %s 0211 IRC| P\r\n", servpass);
	vasend(ircfd, "SERVER %s 1 %s :MPService\r\n", servhost, servtoken);
	while(1){
		if(readresp(ircfd) < 0){
			fprintf(stderr, "Connection destroyed, assuming failure\n");
			return -1;
		}
		parseresp();
		common();
		if(strcmp(ircpresp.cmd, "EOB") == 0){
			printf("Got EOB, assuming success\n");
			break;
		}
	}

	return 0;
}
