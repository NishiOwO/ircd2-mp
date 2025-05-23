/************************************************************************
 *   IRC - Internet Relay Chat, support/config.h
 *   Copyright (C) 1990 Jarkko Oikarinen
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *  ircdwatch configuration options.
 */

/* how often (in seconds) should we check that ircd runs? */
#define IRCDWATCH_POLLING_INTERVAL 30

/* 
 * should we check for config file changes and HUP the server
 * if a change is detected?
 */
#undef IRCDWATCH_HUP_ON_CONFIG_CHANGE

/* 
 * although you may not want to log ircd-messages to syslog you
 * may want to log when ircdwatch reloads the config or when
 * ircd croaks and ircdwatch has to restart it
 */
#define IRCDWATCH_USE_SYSLOG
#ifdef IRCDWATCH_USE_SYSLOG
# define IRCDWATCH_SYSLOG_IDENT     "ircdwatch"
# define IRCDWATCH_SYSLOG_OPTIONS   (LOG_PID)
# define IRCDWATCH_SYSLOG_FACILITY  LOG_DAEMON
#endif


/*
 *  irc[d] configuration options.
 *
 */

/* CHROOTDIR
 *
 * Define for value added security if you are a rooter.
 * WARNING! Better to use external chroot and then run ircd without
 * this option.
 *
 * CPATH, MPATH, LPATH, PPATH, TPATH, QPATH, OPATH,
 * FNAME_USERLOG, FNAME_OPERLOG, FNAME_CONNLOG, FNAME_AUTHLOG
 * must have RPATH as root directory! Set them in Makefile
 *
 * You may want to define IRC_UID and IRC_GID
 */
#undef CHROOTDIR

#if defined(CHROOTDIR)
  #define ROOT_PATH	"/where/to/change/root/dir"
#endif

/* ENABLE_SUMMON
 *
 * The SUMMON command requires the ircd to be run as group tty in order
 * to work properly in many cases.  If you are on a machine where it
 * won't work, or simply don't want local users to be summoned, undefine
 * this.
 * Consider its security implications before defining.
 */
#undef	ENABLE_SUMMON	/* local summon */

/*
 * If you want to have "USERS" output strictly RFC 1459 (showing 
 * who/finger of your ircd or returning "USERS disabled"), define this.
 * Undefining leads to sending RPL_LOCALUSERS and RPL_GLOBALUSERS.
 * upon "USERS" request.
 */
#undef USERS_RFC1459

#ifdef USERS_RFC1459
/*
 * If defined, outputs who/finger alike output from ircd box.
 * Note that some boxes no longer have /etc/utmp. When undefined, it will
 * return ERR_USERSDISABLED numeric. 
 * Consider its security implications before defining.
 */
#undef	USERS_SHOWS_UTMP
#endif

/* DEFAULT_INVISIBLE
 *
 * When defined, your users will automatically be attributed with user
 * mode "i" (i == invisible). Invisibility means people dont showup in
 * WHO or NAMES unless they are on the same channel as you.
 */
#undef	DEFAULT_INVISIBLE

/* 
 * Define this if you want to have KLINE command for opers and services.
 */
#undef KLINE

/*
 * Define this if you want to have TKLINE and UNTKLINE commands
 * for opers and services.
 */
#define TKLINE

/* If you want tklines to be limited in time, define this (seconds). */
/* #define TKLINE_MAXTIME 86400 */

/* Choose tkill notices to be sent either to &OPER or &NOTICES */
/* #define SCH_TKILL SCH_OPER */
#define SCH_TKILL SCH_NOTICE

/* 
 * Operator rights can be precisely defined in O:line flags. However
 * if you undefine any of the following, it will make given function
 * unaccessible to your operators, disregarding their O:line flags.
 */
#define	OPER_CONNECT
#define	OPER_DIE
#define	OPER_REHASH
#define	OPER_RESTART
#define	OPER_SET
#define	OPER_SQUIT
#define	OPER_SQUIT_REMOTE
#define	OPER_KLINE
#define	OPER_TKLINE
/* In a perfect world these two (or at least second) would be undefined. */
#define	OPER_KILL
#define	OPER_KILL_REMOTE

/*
 * Maximum number of network connections your server will allow.  This must
 * not exceed OS limit of max. number of open file descriptors available upon
 * ircd start.
 * If you have a lot of server connections, it may be worth splitting the load
 * over 2 or more servers.
 * 1 server = 1 connection, 1 user = 1 connection.
 * Due to various sanity checks during startup, minimum is 13.
 */
#define MAXCONNECTIONS	50

/* MAXIMUM LINKS
 *
 * This define is useful for leaf nodes and gateways. It keeps you from
 * connecting to too many places. It works by keeping you from
 * connecting to more than "n" nodes which you have C:blah::blah:6667
 * lines for.
 *
 * Note that any number of nodes can still connect to you. This only
 * limits the number that you actively reach out to connect to.
 *
 * Leaf nodes are nodes which are on the edge of the tree. If you want
 * to have a backup link, then sometimes you end up connected to both
 * your primary and backup, routing traffic between them. To prevent
 * this, #define MAXIMUM_LINKS 1 and set up both primary and
 * secondary with C:blah::blah:6667 lines. THEY SHOULD NOT TRY TO
 * CONNECT TO YOU, YOU SHOULD CONNECT TO THEM.
 *
 * Gateways such as the server which connects Australia to the US can
 * do a similar thing. Put the American nodes you want to connect to
 * in with C:blah::blah:6667 lines, and the Australian nodes with
 * C:blah::blah lines. Have the Americans put you in with C:blah::blah
 * lines. Then you will only connect to one of the Americans.
 *
 * This value is only used if you don't have server classes defined, and
 * a server is in class 0 (the default class if none is set).
 *
 */
#define MAXIMUM_LINKS 1

/*
 * A pure non-routing leaf server can undefine HUB for best performance.
 * If your server is running as a a HUB Server then define this.
 * A HUB Server has many servers connect to it at the same as opposed
 * to a leaf which just has 1 server (typically the uplink).
 */
#undef	HUB

#ifdef HUB
/*
 * MAXSERVERS is the maximum number of servers that will be linked
 * to your server at the same time.  This number is not a limit,
 * it is used to allocate memory when ircd is started.
 */
# define	MAXSERVERS 3
#else
# define	MAXSERVERS 1
#endif

/*
 * If you want to use timed klines, set it to frequence in seconds,
 * how often to check if client is to be removed; usually 60 seconds
 * (smallest granularity you can get in K:line anyway).
 *
 * Advised not to use it ;-) --Beeth
 */
/* #define TIMEDKLINES 300 */

/*
 * NOTE: defining CMDLINE_CONFIG and installing ircd SUID or SGID is a MAJOR
 *       security problem - they can use the "-f" option to read any files
 *       that the 'new' access lets them. Note also that defining this is
 *       a major security hole if your ircd goes down and some other user
 *       starts up the server with a new conf file that has some extra
 *       O-lines. So don't use this unless you're debugging.
 */
/* allow conf-file to be specified on command line */
#define	CMDLINE_CONFIG 1

/*
 * To use m4 as a preprocessor on the ircd.conf file, define M4_PREPROC.
 * The server will then call m4 each time it reads the ircd.conf file,
 * reading m4 output as the server's ircd.conf file.
 * Defining USE_M4_PREFIXES makes GNU m4 use -P flag.
 */
#undef	M4_PREPROC
#undef	USE_M4_PREFIXES

/*
 * Define if you want to use #include "file" in ircd.conf without M4. 
 * If file is not absolute path, ircd etc path is prepended.
 * Note that "#include" must be at the beginning of the line.
 */ 
#undef	CONFIG_DIRECTIVE_INCLUDE

/*
 * If you wish to have the server send 'vital' messages about server
 * through syslog, define USE_SYSLOG. Only system errors and events critical
 * to the server are logged although if this is defined with FNAME_USERLOG,
 * syslog() is used additionally to the above file. It is not recommended that
 * this option is used unless you tell the system administrator beforehand
 * and obtain their permission to send messages to the system log files.
 */
#undef	USE_SYSLOG

#ifdef	USE_SYSLOG
/*
 * If you use syslog above, you may want to turn some (none) of the
 * spurious log messages for KILL/SQUIT off.
 */
#undef	SYSLOG_KILL	/* log all operator kills to syslog */
#undef	SYSLOG_SQUIT	/* log all remote squits for all servers to syslog */
#undef	SYSLOG_CONNECT	/* log remote connect messages for other all servs */
#undef	SYSLOG_USERS	/* send userlog stuff to syslog */
#undef	SYSLOG_OPER	/* log all users who successfully become an Op */
#undef	SYSLOG_CONN	/* log all uncomplete/rejected connections */

/*
 * If you want to log to a different facility than DAEMON, change
 * this define.
 */
#define LOG_FACILITY LOG_DAEMON
#endif /* USE_SYSLOG */

/*
** If you want your server channels logged to a file, define this and
** any of LOG_SCH_* you want. Logfile names are taken from channel names
** (like: ircd.ERRORS) and they must exist to be used.
*/
#undef LOG_SERVER_CHANNELS
#ifdef LOG_SERVER_CHANNELS
#undef LOG_SCH_ERROR	/* &ERRORS */
#undef LOG_SCH_NOTICE	/* &NOTICES */
#undef LOG_SCH_KILL	/* &KILLS */
#undef LOG_SCH_CHAN	/* &CHANNEL */
#undef LOG_SCH_NUM	/* &NUMERICS */
#undef LOG_SCH_SERVER	/* &SERVERS */
#undef LOG_SCH_HASH	/* &HASH */
#undef LOG_SCH_LOCAL	/* &LOCAL */
#undef LOG_SCH_SERVICE	/* &SERVICES */
#undef LOG_SCH_DEBUG	/* &DEBUG */
#undef LOG_SCH_AUTH	/* &AUTH */
#undef LOG_SCH_SAVE	/* &SAVE */
#undef LOG_SCH_WALLOP	/* &WALLOPS */
#undef LOG_SCH_CLIENT	/* &CLIENTS (if defined to exist) */
#undef LOG_SCH_OPER	/* &OPER */
#endif /* LOG_SERVER_CHANNELS */

/*
 * LOG_OLDFORMAT
 *
 * Define this if you want old format of logs. (Mind, you will lose
 * additional information, like remote ip, port, etc.)
 */
#undef	LOG_OLDFORMAT

/*
 * Define this if you want to log failed /oper attempts.
 */
#undef FAILED_OPERLOG

/*
 * Define this to make ircd create logfiles if they do not exist.
 */
#undef LOGFILES_ALWAYS_CREATE

/*
 * Define this if you want to use crypted passwords for operators in your
 * ircd.conf file. See contrib/mkpasswd/README for more details on this.
 */
#undef	CRYPT_OPER_PASSWORD

/*
 * If you want to store encrypted passwords in N-lines for server links,
 * define this.  For a C/N pair in your ircd.conf file, the password
 * need not be the same for both, as long as hte opposite end has the
 * right password in the opposite line.  See INSTALL doc for more details.
 */
#undef	CRYPT_LINK_PASSWORD

/*
 * define this if you enable summon and if you want summon to look for the
 * least idle tty a user is logged in on.
 */
#undef	LEAST_IDLE

/*
 * IDLE_FROM_MSG
 *
 * Idle-time nullified only from privmsg, if undefined idle-time
 * is nullified from everything except ping/pong.
 * Added 3.8.1992, kny@cs.hut.fi (nam)
 */
#define	IDLE_FROM_MSG

/*
 * use these to setup a Unix domain socket to connect clients/servers to.
 */
#undef	UNIXPORT

/*
 * IRC_UID
 * IRC_GID
 *
 * If you start the server as root but wish to have it run as another user,
 * define IRC_UID to that user id (and IRC_GID to the desired group id).
 * This should only be defined if you are running as root...
 * and even then perhaps not.
 */
/* #undef	IRC_UID	65534 */
/* #undef	IRC_GID	65534 */

/*
 * CLIENT_FLOOD
 *
 * this controls the number of bytes the server will allow a client to
 * send to the server without processing before disconnecting the client for
 * flooding it.  Values greater than 8000 make no difference to the server.
 */
#define	CLIENT_FLOOD	1000

/* Remote query flood protection. */
#define	CHREPLLEN	8192

/*
 * If you wish to run services, define USE_SERVICES.
 * This can make the server noticeably bigger and slower.
 * services are not fully implemented yet, so don't use it unless you really
 * know what you are doing.
 */
#define  USE_SERVICES 1

/*
 * Define the following to make the delay for nicks random.
 * Some people believe a bot can exactly time the delay and don't like it,
 * I think this is a useless concern. -krys
 */
#undef RANDOM_NDELAY

/*
 * You've read the BOFH saga and you liked it, then define the following.
 *
 * The two following will change the nick delay and channel delay features
 * making them totally user unfriendly but more efficient.
 */
#undef BETTER_NDELAY
#undef BETTER_CDELAY

/*
 * Defining this will enable the use of compressed server-server links.
 * In order to have it work, you must have the zlib version 1.0 or higher.
 * The library and the include files must have been found by configure,
 * if you have installed the zlib after running configure, run it again.
 */
#undef ZIP_LINKS

/*
 * Defining this will add an artificial 2 seconds delay for accepting
 * connections.  This is the OLD behaviour of the server.
 *
 * NOTE: Undefining this leads to a significant increase in CPU usage if
 * you reject client which keeps connecting.
 */
#undef	SLOW_ACCEPT

/*
 * Defining this will make the server check for rapid connections from a single
 * host and reject new connections from this host if the limit is reached.
 *
 * NOTE: Enabling this feature will significantly increase the CPU usage
 * for servers carrying several hundred clients and getting many connections.
 */
#define	CLONE_CHECK

/*
** Servers with many clients and lots of K:lines are having noticable
** lag after rehash. Define this to the number of clients you want to
** be checked each time (all clients will eventually be checked, of course).
** This will lessen the load and eliminate the lag during rehash.
** Undefine (or define to bigger than MAXCONNECTIONS) to check all clients
** in one sweep.
*/
#define MAXDELAYEDKILLS 200

/*
** This defines the message that is sent to clients upon connect.
** Keep it short.
*/
#define HELLO_MSG "Please wait while we process your connection."

/*
 * Connections rejected by check_clones() will be delayed for this many seconds
 * before final error is sent to them and their socket is closed.
 * This effectively reduces bouncing (fast reconnecting clients).
 * CLONE_CHECK must be defined for this to work.
 *
 * Note: it may lead to file descriptors exhaustion. ("All connections in use"
 * error message.)
 */
#define DELAY_CLOSE     15       /* Recommended value: 15 */

/*
** Define this to get oper-only &CLIENTS channel with clients connects,
** quits and nick changes.
*/
#undef CLIENTS_CHANNEL

/*
** Bitmask defining type of information sent to &CLIENTS, combine from
** CCL_CONN (client uid, nick, username, host and IP), CCL_CONNINFO
** (also user info), CCL_QUIT (client uid, nick, username, host and IP),
** CCL_QUITINFO (also quit reason), CCL_NICK (all nick changes).
*/
/* #define CLIENTS_CHANNEL_LEVEL (CCL_CONN|CCL_QUIT) */

/*
** This adds ability to ban users with specified realnames from
** connecting to the server. May be useful for various drones.
*/
#undef XLINE

/*
** Define the reason users get as a rejection message.
** Some may use first form for the sake of transparency, some may believe it
** is not good that users can see the real reason behind X-rejection.
*/
/* #define XLINE_EXIT_REASON "X-denied user" */
#define XLINE_EXIT_REASON "Too many host connections (global)"

/*
** Define this to whatever you want to see after successful OPER
*/
#define TXT_YOUREOPER "You are now an IRC Operator"

/*
** If you have huge number of K-lines and provide your users with
** some other means of looking at them (or you are mean and simply
** do not want to show it at all), define this.
** Note that operators will get list of K anyway.
*/
/* #define TXT_NOSTATSK "Please use http://example.org/klines instead" */

/*
** If you don't want non-oper clients to see tkline list (like when you run
** some service which automatically tklines open proxies), define this.
*/
/* #undef DISABLE_STATSTKLINE */

/*
** If you have problems with two servers autoconnecting to each other
** all the time and thus squitting, define this. It will eat a little
** CPU during AC and it will not allow to try to connect to server that
** is trying to connect to you.
*/
/* #undef DISABLE_DOUBLE_CONNECTS */

/*
** Define delimiter of fields in ircd.conf.
*/
#ifdef INET6
# define IRCDCONF_DELIMITER '%'
#else
# define IRCDCONF_DELIMITER ':'
#endif

/*
 * Max number of channels a user is allowed to join.
 */
#define MAXCHANNELSPERUSER  21	/* Recommended value: 21 */

/*
 * USE_IAUTH makes ircd use the iauth program for authentication.
 * it can always be overriden by using the -s switch
 */
#undef USE_IAUTH

/* Following notice is sent before and after /LIST output.
 * If you do not want such behaviour, undefine.
 */
#define LIST_ALIS_NOTE "Usage of /list for listing all channels is " \
		       "deprecated. Please use \"/squery alis help\" instead."

/* Define this to see when a channel topic was set and who it was set by.
** Warning: eats memory (around 5MB)
*/
#define TOPIC_WHO_TIME

/*
** Define this to show local clients signon time in whois
*/
#define WHOIS_SIGNON_TIME

/*
 * Split detection
 * This defines default thresholds for turning on and off the split-mode,
 * where joins to new channels do not give chanop status.
 * This should always be bigger than the max counts of
 * users/servers in your TLD (or some other close group), so you
 * get in split when you split. It is also a good practice to set
 * around 90% of what your whole network counts minimally (servers
 * inside masks are also counted), so that splits of smaller parts
 * would be noticed on each side.
 *
 * Defining to 0 disables entering split-mode.
 */
#define DEFAULT_SPLIT_USERS	0
#define DEFAULT_SPLIT_SERVERS	0

/*
** Notice sent to connecting users if the server is in the split-mode.
*/
#define SPLIT_CONNECT_NOTICE "Server is currently in split-mode."

/*
 * Undefining NO_OPER_REMOTE removes the restriction that O-lines only become
 * fully effective for people on the 'same network' as the server.
 * Defined, it slightly increases the security of the server by
 * placing restrictions on where people can become operator from.
 * Dubious consideration, IMO.
 */
#undef	NO_OPER_REMOTE

/*
** CLCHNO defines changes in local clients count, which are shown
** as "Local in/de-crease"
** I guess rule of thumb would be 5% of your average user count. --B.
*/
#define CLCHNO 100

/*
** CLCHSEC defines minimum time in seconds between two consecutive
** "New highest local/global clients count" messages (prevents flood).
*/
#define CLCHSEC 300

/*
** Define this to allow MOTD for unregistered clients. Note that this is
** required by some European countries laws.
*/
#undef MOTD_UNREG

/*
** For standard "Gone" away reply for remote clients, undefine this.
** Leave defined for more helpful message.
*/
#define AWAY_MOREINFO

/*
** If you do not wish your clients to use "nick 0" during registration,
** define this.
*/
#undef DISABLE_NICK0_REGISTRATION

/*
** Define this if you want to use Japanese channel names in JIS encoding.
*/
#undef JAPANESE

/*
** If you want to allow your opers to be able to use SIDTRACE command to
** see all clients from all servers that share the same SID prefix as your 
** own, enable this. Useful for server "clusters", like 0PN. Requires also
** proper ACLs in O-line.
*/
#undef ENABLE_SIDTRACE

/*   STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP  */
/* You shouldn't change anything below this line, unless absolutely needed. */

/*
** Accept only hostnames conforming to RFC1034/1035/1123.
** Conformance of the internal resolver to RFC1123.
*/
#define	RESTRICT_HOSTNAMES

/*
** Allow only "proper" chars in username.
** Disallow '^', '~', '+', '=', '-' as a first char.
** Require at least one alphanum and no more than two nonalphanum.
*/
#define	RESTRICT_USERNAMES

/* Maximum length the queue of pending connections to one port may grow to.
 * Note that your system limits this, too. FreeBSD, for instance, has sysctl
 * kern.ipc.somaxconn for that. Such must be bigger than this define prior to
 * ircd start.
 */
#define LISTENQUEUE 128

/* define DEBUGMODE to enable debugging mode.*/
#undef	DEBUGMODE
#ifdef DEBUG
#define DEBUGMODE
#endif

/*
 * Time interval to wait and if no messages have been received, then check for
 * PINGFREQUENCY and CONNECTFREQUENCY
 */
#define TIMESEC  60		/* Recommended value: 60 */

/*
 * If daemon doesn't receive anything from any of its links within
 * PINGFREQUENCY seconds, then the server will attempt to check for
 * an active link with a PING message. If no reply is received within
 * (PINGFREQUENCY * 2) seconds, then the connection will be closed.
 */
#define PINGFREQUENCY    120	/* Recommended value: 120 */

/*
 * If the connection to to uphost is down, then attempt to reconnect every
 * CONNECTFREQUENCY  seconds.
 */
#define CONNECTFREQUENCY 600	/* Recommended value: 600 */

/*
 * Often net breaks for a short time and it's useful to try to
 * establishing the same connection again faster than CONNECTFREQUENCY
 * would allow. But, to keep trying on bad connection, we require
 * that connection has been open for certain minimum time
 * (HANGONGOODLINK) and we give the net few seconds to steady
 * (HANGONRETRYDELAY). This latter has to be long enough that the
 * other end of the connection has time to notice it broke too.
 */
#define HANGONRETRYDELAY 30	/* Recommended value: 30 seconds */
#define HANGONGOODLINK 900	/* Recommended value: 15 minutes */

/*
 * Number of seconds to wait for write to complete if stuck.
 */
#define WRITEWAITDELAY     15	/* Recommended value: 15 */

/*
 * Number of seconds to wait for DNS/authentication to complete.
 * Note that iauth's default timeout per module is 30 seconds, so this value
 * should be at least 30 * number of modules.  Extra time should really be
 * given to be safe.
 */
#define	ACCEPTTIMEOUT      90	/* Recommended value: 90 */

/*
 * Max time from the nickname change that still causes KILL
 * automaticly to switch for the current nick of that user. (seconds)
 */
#define KILLCHASETIMELIMIT 90   /* Recommended value: 90 */

#ifdef ZIP_LINKS
/*
 * the compression level used. (Suggested values: 3, 4, 5)
 * Above 5 will only give a *very* marginal increase in compression for a
 * *very* large increase in CPU usage.
 */
# define	ZIP_LEVEL	5
#endif

#ifdef	CLONE_CHECK
/*
 * If CLONE_CHECK has been defined, these control how the checks are performed,
 * and how the alarm is triggered.
 * This triggers alarm when more than 10 clients in 2 seconds from the same host
 * try to connect to ircd.
 */
# define	CLONE_MAX	10
# define	CLONE_PERIOD	2
#endif

/*
 * define NO_IDENT if you don't want to support ident (RFC1413).
 * it is a VERY bad idea to do so, since this will make it impossible to
 * efficientely track abusers.
 * NO_PREFIX should always be undefined.
 */
/* #undef	NO_IDENT */
/* #undef	NO_PREFIX */

/*
** LISTENER_DELAY, if defined, adds an artificial delay between checking
** listeners for new connections. If undefined, it accepts clients faster
** but the CPU load increases.
** LISTENER_MAXACCEPT defines how many clients will be accepted during one
** listener check.
*/
#ifdef SLOW_ACCEPT
#define LISTENER_DELAY 2
#define LISTENER_MAXACCEPT 1
#else
#define LISTENER_DELAY 1
#define LISTENER_MAXACCEPT 10
#endif

/*
** If you don't feel your users should use too long nicknames, you can
** restrict length of their dnicks here.
*/
#define LOCALNICKLEN 15

/*
** If you don't want your users to have too short nicknames, define
** minimum nicklen allowed.
*/
/* #define MINLOCALNICKLEN 2 */

/*
** You probably don't want to disable it, but if you run into any problems,
** here's the knob.
*/
#define ENABLE_CIDR_LIMITS

/*
** Define this to significantly speed up rehash with large amounts of I-lines.
** Wastes some memory each rehash (luckily not indefinitely).
*/
#define FASTER_ILINE_REHASH

/*
** Restores old behaviour of Y-lines maxlinks limit, which was counted
** separately per each I-line using it.
** Note: if you use it with FASTER_ILINE_REHASH, clients will be able
** to exceed maxlinks limits after each rehash. Sorry.
*/
#undef YLINE_LIMITS_OLD_BEHAVIOUR

/*
** Y-line [user@]host limits are now using ip hash instead of hostname hash,
** which means counts are against IPs, not hostnames.
** If you don't like that behaviour, you can undefine it.
*/
#define YLINE_LIMITS_IPHASH

/*
** find_userhost (used in PRIVMSG user%host@server syntax) uses hostname
** hash. Noone really used that syntax anyway, so noone should notice.
** If you are rebellious or just don't want to waste memory -- define it.
** Note that if there's a hostname hash needed (and thus compiled in) 
** for other reasons, it will be used in find_userhost anyway.
** Also note that defining this breaks compatibility with RFC 2812, as
** clients no longer will be able to PRIVMSG user%host@server (though they 
** will be able to use user%ip@server), but it would still be compatible 
** with RFC 1459, which defined only "user@server" syntax.
*/
#undef FIND_USERHOST_IPHASH

/*
** Default behaviour of the server after the first netjoin is done.
** Possible values: 0 (SET CACCEPT OFF), 1 (SET CACCEPT ON)
** and 2 (SET CACCEPT SPLIT).
*/
#define CACCEPT_DEFAULT 2

/* Minimum values admin can set. */
#define SPLIT_USERS	50000
#define SPLIT_SERVERS	50

/*   STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP  */
/*   STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP  */
/*   STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP STOP  */
/* ------------------------- END CONFIGURATION SECTION -------------------- */

#ifndef ENABLE_SUMMON
#  undef LEAST_IDLE
#endif

#define SEQ_NOFILE    128	/* For Dynix (sequent OS) users :
				 * set to your current kernel impl,
				 * max number of socket connections;
				 * ignored on other OS.	
				 */
/*
 * safety margin so we can always have one spare fd, for motd/authd or
 * whatever else.  -5 allows "safety" margin of 1 and space reserved.
 */
#define	MAXCLIENTS	(MAXCONNECTIONS-5)

/* In fact, if using server channels logs, it would be even more. */
#if (MAXCONNECTIONS < 13)
# error Sorry, MAXCONNECTIONS minimum 13 for ircd to start.
#endif

#if defined(CLIENT_FLOOD)
# if	(CLIENT_FLOOD > 8000) || (CLIENT_FLOOD < 512)
#error CLIENT_FLOOD needs redefining.
# endif
#else
#error CLIENT_FLOOD undefined
#endif

/* sanity check */
#if defined(DELAY_CLOSE)
#  if defined(SLOW_ACCEPT) || !defined(CLONE_CHECK)
#    error define CLONE_CHECK and undefine SLOW_ACCEPT to use DELAY_CLOSE.
#  endif
#  if (DELAY_CLOSE < 1)
#    error DELAY_CLOSE needs redefining.
#  endif
#endif

#if defined(ZIP_LINKS)
# if	(ZIP_MINIMUM > ZIP_MAXIMUM)
#error ZIP_MINIMUM needs redefining.
# endif
#endif

/* I think we may allow undefining both SLOW_ACCEPT and CLONE_CHECK --B. */
#if 0
#if !defined(SLOW_ACCEPT) && !defined(CLONE_CHECK)
# define	CLONE_CHECK
# define	CLONE_MAX       2
# define	CLONE_PERIOD    10
#endif
#endif

/*
** you wouldn't want to compress messages one by one.. would you?
** (it's not implemented anyways)
*/
#ifdef	ZIP_LINKS
# define	SENDQ_ALWAYS
#endif

#if ! USE_POLL
# if (MAXCONNECTIONS > FD_SETSIZE)
#error FD_SETSIZE must be bigger than MAXCONNECTIONS
# endif
#endif

/*
** buffer used for iauth-ircd communication
** very big servers might consider increasing it
*/
#ifdef USE_IAUTH
# define       IAUTH_BUFFER    65535
#endif

/*
** Make sure that if zlib isn't found, or you don't want to use it, that you
** don't define ZIP_LINKS
*/
#ifndef	USE_ZLIB
#undef	ZIP_LINKS
#endif

#if defined(TIMEDKLINES)
# if (TIMEDKLINES + 0 < 60)
# error TIMEDKLINES must be bigger than 60 seconds
# endif
#endif

#ifndef SPLIT_USERS
#error SPLIT_USERS must be defined
#endif

#ifndef SPLIT_SERVERS
#error SPLIT_SERVERS must be defined
#endif

#if defined(CONFIG_DIRECTIVE_INCLUDE) && defined(M4_PREPROC)
#error CONFIG_DIRECTIVE_INCLUDE and M4_PREPROC are mutually exclusive
#endif

#ifndef TKLINE
#undef OPER_TKLINE
#endif

#if defined(LOGFILES_ALWAYS_CREATE) && !defined(HAVE_SYS_STAT_H)
#error LOGFILES_ALWAYS_CREATE requires sys/stat.h
#endif

#ifndef IRCDCONF_DELIMITER
#error IRCDCONF_DELIMITER must be defined
#endif

#if !defined(HELLO_MSG)
#error HELLO_MSG must be defined
#endif

#if defined(TKLINE_MAXTIME) && (TKLINE_MAXTIME + 0) == 0
#undef TKLINE_MAXTIME
#endif

/* Hard limit tkline time (more than 3 years) */
#if !defined(TKLINE_MAXTIME) || (TKLINE_MAXTIME > 99999999)
#define TKLINE_MAXTIME 99999999
#endif

#if !defined(SCH_TKILL)
#define SCH_TKILL SCH_NOTICE
#endif

#if defined(CLIENTS_CHANNEL) && !defined(CLIENTS_CHANNEL_LEVEL)
#error define CLIENTS_CHANNEL_LEVEL
#endif

#if defined(MINLOCALNICKLEN) && ((MINLOCALNICKLEN + 0) < 2)
#undef MINLOCALNICKLEN
#endif

#if defined(MINLOCALNICKLEN) && (MINLOCALNICKLEN > LOCALNICKLEN)
#error MINLOCALNICKLEN bigger than (max)LOCALNICKLEN
#endif

/* we should probably not bother with that :) */
#if !defined(NBLOCK_POSIX) && !defined(NBLOCK_BSD) && !defined(NBLOCK_SYSV)
#undef LISTENER_MAXACCEPT
#define LISTENER_MAXACCEPT 1
#endif

/* well, we save some memory if we don't actually need either of the hashes */
#if defined(YLINE_LIMITS_IPHASH) || defined(FIND_USERHOST_IPHASH)
#define USE_IPHASH
#endif

#if !defined(YLINE_LIMITS_IPHASH) || !defined(FIND_USERHOST_IPHASH)
#define USE_HOSTHASH
#endif

#if defined(CACCEPT_DEFAULT)
#if ((CACCEPT_DEFAULT) > 2) || ((CACCEPT_DEFAULT) < 0)
#error CACCEPT_DEFAULT can be 0, 1 or 2.
#endif
#endif
