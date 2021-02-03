/* DavydovMA 2013040600 {
 * file:	main_help.c
 */

#include	<stdio.h>


/* ------------- ---- ------
 * function:	fmain_help {
 * rem:		show help
 * date:	2019-12-18
 * in:
 * out:
 */
void fmain_help( void ) {

	printf( "\nUsage:\t%s <options>\n"
		"\nCommon options:\n"
		"\t-h|--help\t\t: this help\n"
		"\t-f|--file <path>\t: path to file\n"
		"\nDescription:\n"
		"\t%s\n"
		"\nControl keys:\n"
		"\t[ or {\t\t: byte x/y-vector\n"
		"\t] or }\t\t: line x/y-vector\n"
		"\t/ or ?\t\t: grid on/off\n"
		"\t< or ,\t\t: grid decrement\n"
		"\t> or .\t\t: grid increment\n"
		"\t- or _\t\t: pixel decrement\n"
		"\t+ or =\t\t: pixel increment\n"
		"\t` or ~\t\t: keyboard mode\n"
		"\tUp\t\t: move pixel up\n"
		"\tDown\t\t: move pixel down\n"
		"\tLeft\t\t: move pixel left\n"
		"\tRight\t\t: move pixel right\n"
		"\tPageUp\t\t: move line back\n"
		"\tPageDown\t: move line forward\n"
		"\tHome\t\t: move line first\n"
		"\tEnd\t\t: move line last\n"
		"\tTab\t\t: switch preset config\n"

		"\n",
		main_file_elf, MAIN_REM
		);

}	/* } fmain_help */


/* } DavydovMA 2019121800 */