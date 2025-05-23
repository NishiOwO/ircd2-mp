/*
 *   IRC - Internet Relay Chat, ircd/version.c
 *   Copyright (C) 1990 Chelsea Ashley Dyerman
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
 * This file is generated by version.c.SH. Any changes made will go away.
 */

#ifndef lint
static const volatile char rcsid[] = "@(#)$Id: version.c.SH.in,v 1.14 2010/08/13 19:58:46 bif Exp $";
#endif

#include "os.h"
#include "s_defines.h"
#define VERSION_C
#include "s_externs.h"
#undef VERSION_C

char *generation = "1";
char *creation = __DATE__;
char *version;				/* Filled by make_version() */
char **isupport;			/* Filled by make_isupport() */
char *pass_version = PATCHLEVEL;

char *infotext[] =
    {
	"IRC --",
	"Based on the original code written by Jarkko Oikarinen",
	"Copyright 1988, 1989, 1990, 1991 University of Oulu, Computing Center",
	"",
	"This program is free software; you can redistribute it and/or",
	"modify it under the terms of the GNU General Public License as",
	"published by the Free Software Foundation; either version 1, or",
	"(at your option) any later version.",
	"",
	"The following persons have made many changes and enhancements to the",
	"code and still know how IRC really works if you have questions about it:",
	"",
	"Beeth        Piotr Kucharski          chopin@42.pl",
	"",
	"Thanks to the following people for their work on 2.11:",
	"",
	"jv           Jakub Vlasek             jv@pilsedu.cz",
	"Q            Kurt Roeckx              kurt@roeckx.be",
	"",
	"Thanks to the following people for their work on 2.10:",
	"",
	"syrk         Christophe Kalt          kalt@stealth.net",
	"Core         Magnus Tjernstrom        d92-mtm@ludd.luth.se",
	"",
	"Those who helped in prior versions:",
	"",
	"Jarkko Oikarinen",
	"Darren Reed         Vesa Ruokonen",
	"Matthew Green       Chuck Kane          Matt Lyle",
	"Markku Savela       Greg Lindahl        Armin Gruner",
	"Stellan Klebom      Dan Goodwin         Mike Bolotski",
	"Ian Frechette       Markku Jarvinen     Kimmo Suominen",
	"Jeff Trim           Vijay Subramaniam   Karl Kleinpaste",
	"Bill Wisner         Tom Davis           Hugo Calendar",
	"Tom Hopkins         Stephen van den Berg",
	"Bo Adler            Michael Sandrof     Jon Solomon",
	"Jan Peterson        Helen Rose          Paul Graham",
	"",
	"Thanks also goes to those persons not mentioned here who have added",
	"their advice, opinions, and code to IRC.",
	"Thanks also to those who provide the kind sys admins who let me and",
	"others continue to develop IRC.",
	"",
	"",
	"",
	0,
    };
