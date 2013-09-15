/*
 * Copyright(c) 2013 Tim Ruehsen
 *
 * This file is part of libmget.
 *
 * Libmget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Libmget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libmget.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Testing Mget
 *
 * Changelog
 * 27.07.2013  Tim Ruehsen  created
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h> // exit()
#include "libtest.h"

#define ccedilla_l15 "\xE7"
#define eurosign_l15 "\xA4"
#define eacute_l1 "\xE9"
#define eacute_u8 "\xC3\xA9"
#define currency_u8 "\xC2\xA4"

int main(void)
{
	mget_test_url_t urls[]={
		{	.name = "/index.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Main Page</title></head><body><p>" \
				"Link to page 1 <a href=\"http://localhost:{{port}}/p1_fran" ccedilla_l15 "ais.html\">La seule page en fran&ccedil;ais</a>." \
				"Link to page 1 <a href=\"http://localhost:{{port}}/p3_" eurosign_l15 eurosign_l15 eurosign_l15 ".html\">My tailor is rich</a>." \
				"</p></body></html>",
			.headers = {
				"Content-type: text/html; charset=ISO-8859-15",
			}
		},
		{	.name = "/robots.txt",
			.code = "200 Dontcare",
			.body = "",
			.headers = {
				"Content-type: text/plain",
			}
		},
		{	.name = "/p1_fran%C3%A7ais.html", // UTF-8 encoded
			.code = "404 Not Found",
			.body =
				"<html><head><title>404</title></head><body><p>What ever</p></body></html>",
			.headers = {
				"Content-type: text/html; charset=UTF-8",
			}
		},
		{	.name = "/p1_fran%E7ais.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>La seule page en français</title>" \
				"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"/></head><body>" \
				"<p>Link to page 2 <a href=\"http://localhost:{{port}}/p2_" eacute_l1 eacute_l1 "n.html\">Die enkele nerderlangstalige pagina</a>." \
				"</p></body></html>",
			.headers = {
				"Content-type: text/html; charset=UTF-8",
			}
		},
		{	.name = "/p2_%C3%A9%C3%A9n.html", // UTF-8 encoded
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Die enkele nederlandstalige pagina</title>" \
				"</head><body><p>&Eacute;&eacute;n is niet veel maar toch meer dan nul.<br/>" \
				"Nerdelands is een mooie taal... dit zin stuckje spreekt vanzelf, of niet :)<br/>" \
				"</p></body></html>",
			.headers = {
				"Content-type: text/html; charset=UTF-8",
			},
		},
		{	.name = "/p2_%E9%E9n.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Die enkele nederlandstalige pagina</title>" \
				"</head><body><p>&Eacute;&eacute;n is niet veel maar toch meer dan nul.<br/>" \
				"Nerdelands is een mooie taal... dit zin stuckje spreekt vanzelf, of niet :)<br/>" \
				"</p></body></html>",
			.headers = {
				"Content-type: text/html; charset=ISO-8859-1",
			},
		},
		{	.name = "/p3_%E2%82%AC%E2%82%AC%E2%82%AC.html", // UTF-8 encoded
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Euro page</title>" \
				"</head><body><p>My tailor isn't rich anymore.</p></body></html>",
			.headers = {
				"Content-type: text/plain",
			},
		},
		{	.name = "/p3_%A4%A4%A4.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Euro page</title>" \
				"</head><body><p>My tailor isn't rich anymore.</p></body></html>",
			.headers = {
				"Content-type: text/plain",
			},
		},
		{	.name = "/p3_%C2%A4%C2%A4%C2%A4.html", // UTF-8 encoded
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Euro page</title>" \
				"</head><body><p>My tailor isn't rich anymore.</p></body></html>",
			.headers = {
				"Content-type: text/plain",
			},
		},
	};

	// functions won't come back if an error occurs
	mget_test_start_http_server(
		MGET_TEST_RESPONSE_URLS, &urls, countof(urls),
		0);

	// test-iri-disabled
	mget_test(
//		MGET_TEST_KEEP_TMPFILES, 1,
		MGET_TEST_OPTIONS, "--iri --trust-server-names --remote-encoding=iso-8859-1 -nH -r",
		MGET_TEST_REQUEST_URL, "index.html",
		MGET_TEST_EXPECTED_ERROR_CODE, 0,
		MGET_TEST_EXPECTED_FILES, &(mget_test_file_t []) {
			{ urls[0].name + 1, urls[0].body },
			{ urls[1].name + 1, urls[1].body },
			{ "p1_fran" ccedilla_l15 "ais.html", urls[3].body },
			{ "p2_" eacute_u8 eacute_u8 "n.html", urls[4].body },
			{ "p3_" currency_u8 currency_u8 currency_u8 ".html", urls[6].body },
			{	NULL } },
		0);

	exit(0);
}