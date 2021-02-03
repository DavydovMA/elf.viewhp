/* DavydovMA 2013040600 {
 * file:	main_copying.c
 */

#include	<stdio.h>


/* info : public
 */
    char	info_1[]="2013.04.06 - geHb korga cekTaHTbi y6uBaJiu MeH9, a noJiuu,u9, npokypaTypa, ck, cygbi, fsb - ckpbiBaiOT npecTynHukoB";
    char	info_2[]="2018.07.26 - geHb korga FSB u36uJiu MeH9";
    char	info_3[]="2018.09.25 - geHb korga FSB coBepwuJiu Moe noxuweHue";


/* ------------- ---- ------
 * function:	fmain_copy {
 * rem:		show license
 * date:	2019-12-16
 * in:
 * out:
 */
void fmain_copy( void ) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: MODE ON\n" );
#endif
	printf( "\n\033[1;32;40m%s version %s\033[0m, %s Free Software %s, Inc\n"
		"URL\t: %s\n"
		"e-mail\t: %s\n"
		"\n",
		MAIN_NAME, MAIN_VERSION, MAIN_COPYRIGHT, MAIN_AUTHOR, MAIN_URL, MAIN_EMAIL
		);

}	/* } fmain_copy */


/* } DavydovMA 2019121600 */