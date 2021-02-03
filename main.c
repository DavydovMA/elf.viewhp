/* DavydovMA 2013040600 {
 * file:	main.c
 * Block:	variable
 * function:	libc_sdl2_perror
 * Function:	fmain_perror
 * Function:	fmain_draw_pixel
 * Function:	fmain_draw_byte
 * Function:	fmain_draw_line
 * Function:	fmain_draw_win
 * Function:	fmain_neg_vector_byte
 * Function:	fmain_neg_vector_line
 * Function:	fmain_neg_mode_screen
 * Function:	fmain_neg_mode_keyboard
 * Function:	fmain_neg_mode_grid
 * Function:	fmain_size_grid_inc
 * Function:	fmain_size_grid_dec
 * Function:	fmain_size_pixel_inc
 * Function:	fmain_size_pixel_dec
 * Function:	fmain_size_win
 * Function:	fmain_shift_byte_dec
 * Function:	fmain_shift_byte_inc
 * Function:	fmain_buffer_read
 * Function:	fmain_buffer_read_copy
 * Function:	fmain_buffer_read_line
 * Function:	fmain_buffer_read_full
 * Function:	fmain_buffer_read_home
 * Function:	fmain_buffer_read_end
 * Function:	fmain_buffer_read_dec
 * Function:	fmain_buffer_read_inc
 * Function:	fmain_set_conf
 * Function:	main
 */

#define		MAIN_NAME	"viewhp"
#define		MAIN_VERSION	"19.12"
#define		MAIN_COPYRIGHT	"Copyright 1990-2019"
#define		MAIN_AUTHOR	"DavydovMA"
#define		MAIN_URL	"http://domain"
#define		MAIN_EMAIL	"dev-c@domain"
#define		MAIN_CODEPAGE	"UTF-8"
#define		MAIN_REM	"View file hex as pixel."


#ifdef	DEBUG
#define	MAIN_DEBUG
#endif
#include	"main_head.c"
#include	"main_copying.c"
#include	"main_help.c"


/* ------------- ---- ------
 * Block:	variable {
 * rem:		main variable
 * date:	2019-12-18
 */
struct {
	unsigned int	mode_keyboard	: 1;	/* mode keyboard */
	unsigned int	vector_byte	: 1;	/* byte  x/y-vector */
	unsigned int	vector_line	: 1;	/* line  x/y-vector */
	unsigned int	mode_grid	: 1;	/* grid */
	unsigned int	mode_screen	: 1;	/* mode fullscreen */
} flag;
	char		*main_filein		= NULL;
	int		main_filein_fd		= 0;
	int		main_filein_index	= 0;				/* cursor */

	char		*main_win_title		= MAIN_NAME" version "MAIN_VERSION;
	int		main_win_x		= 0;				/* SDL_WINDOWPOS_CENTERED */
	int		main_win_y		= 0;				/* SDL_WINDOWPOS_CENTERED */
#define	MAIN_WINDOW_SIZE_BEGIN_WIDTH		320
#define	MAIN_WINDOW_SIZE_BEGIN_HEIGTH		240
	int		main_win_w		= MAIN_WINDOW_SIZE_BEGIN_WIDTH;
	int		main_win_h		= MAIN_WINDOW_SIZE_BEGIN_HEIGTH;
	Uint32		main_win_mode		= SDL_WINDOW_RESIZABLE;
	SDL_Window	*main_win		= NULL;
	SDL_Renderer	*main_ren		= NULL;
#define	MAIN_PIXEL_SIZE_MIN			1				/* minimal size o_O */
#define	MAIN_PIXEL_SIZE_MAX			32				/* maximal size of pixel */
#define	MAIN_PIXEL_STEP				1				/* step resize of pixel */
	int		main_pixel_size		= MAIN_PIXEL_SIZE_MIN;
	int		main_pixel_in_line_w	= 0;
	int		main_pixel_in_line_h	= 0;
#define	MAIN_LINE_MAX				32				/* line max on display */
#define	MAIN_LINE_SIZE_MIN			CHAR_BIT
#define	MAIN_LINE_SIZE_MAX			CHAR_BIT*2*2*2*2*2
#define	MAIN_LINE_SHIFT_X			0
#define	MAIN_LINE_SHIFT_Y			0
#define	MAIN_GRID_SIZE_MIN			CHAR_BIT
#define	MAIN_GRID_SIZE_MAX			MAIN_LINE_SIZE_MAX
	int		main_grid_size		= MAIN_GRID_SIZE_MIN;
#define	MAIN_BUFFER_SIZE				MAIN_LINE_SIZE_MAX * MAIN_LINE_MAX + MAIN_LINE_SIZE_MAX
#define	MAIN_BUFFER_FILL				0x00
	char		*main_buffer			= NULL;
	char		*main_buffer_read			= NULL;
	int		main_buffer_line_end		= 0;			/* end line */
	int		main_buffer_line_end_size	= 0;			/* size end line */
	int		main_buffer_shift_byte		= 0;
	int		main_buffer_shift_bit		= 0;
	int		main_buffer_shift_line		= 0;
/* } variable */
/* ------------- ---- ------
 * function:	libc_sdl2_perror {
 * rem:		print error
 * date:	2019-12-18
 * in:
 * out:
 */
void	libc_sdl2_perror(
	void
) {

	printf( "ERROR: [sdl2] [%s]\n", SDL_GetError() );

}	/* } libc_sdl2_perror */
/* ------------- ---- ------
 * Function:	fmain_perror {
 * rem:		print error
 * date:	2019-12-18
 * in:
 *	in_errno
 * out:
 *	errno
 */
int	fmain_perror(
	int	in_errno
) {

	if ( in_errno == 0 ) {
		in_errno	= errno;
	}

	error( 0, in_errno, "ERROR: [main] [%d] ", in_errno );

	return( in_errno );

}	/* } fmain_perror */
/* ------------- ---- ------
 * Function:	fmain_draw_pixel {
 * rem:		draw 1 pixel
 * date:	2019-12-14
 * in:
 *	in_x
 *	in_y
 * out:
 */
void	fmain_draw_pixel(
	int	in_x,
	int	in_y
) {

	SDL_Rect	rectfill	= { in_x, in_y, main_pixel_size, main_pixel_size };
	SDL_RenderFillRect( main_ren, &rectfill );
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_draw_pixel]	[%d]:x	[%d]:y	[%d]:main_pixel_size\n",
	in_x,
	in_y,
	main_pixel_size
	);
#endif

}	/* } fmain_draw_pixel */
/* ------------- ---- ------
 * Function:	fmain_draw_byte {
 * rem:		draw 1 byte
 * date:	2019-12-13
 * in:
 *	in_byte
 *	in_x
 *	in_y
 * out:
 */
void	fmain_draw_byte(
	char	in_byte,
	int	in_x,	/* as shift x */
	int	in_y	/* as shift y */
) {

	int	ink;
	int	shift_bit;
	for( int index_bit = CHAR_BIT - 1; index_bit >= 0; index_bit-- ) {	/* index_bit 7.6.5.4.3.2.1.0 { */
		ink	= 0xff * ( ( in_byte >> index_bit ) & 0x01 );
		if ( index_bit == 0 && ink != 0x00 ) {
			SDL_SetRenderDrawColor( main_ren, ink, 0x00, 0x00, ink );	/* set color red*/
		} else {
			SDL_SetRenderDrawColor( main_ren, ink, ink, ink, ink );		/* set color white*/
		}
/* vb vl     */	shift_bit	= (
/* 0  0  7-0 */			(  flag.vector_line * ( !flag.vector_byte * ( CHAR_BIT - 1 - index_bit ) +  flag.vector_byte * ( index_bit ) ) )
/* 0  1  0-7 */			+
/* 1  0  0-7 */			( !flag.vector_line * (  flag.vector_byte * ( CHAR_BIT - 1 - index_bit ) + !flag.vector_byte * ( index_bit ) ) )
/* 1  1  7-0 */			) * main_pixel_size;
		fmain_draw_pixel(\
				in_x + ( flag.vector_line * shift_bit ),
				in_y + ( !flag.vector_line * shift_bit )
				);	/* vector_line: 0 = y; 1 = x */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_draw_byte]	[%d]:index_bit	[%d]:shift_bit	[%d]:x	[%d]:y\n",
	index_bit,
	shift_bit,
	in_x + ( flag.vector_line * shift_bit ),
	in_y + ( !flag.vector_line * shift_bit )
	);
#endif
	}	/* } index_bit */

}	/* } fmain_draw_byte */
/* ------------- ---- ------
 * Function:	fmain_draw_line {
 * rem:		draw 1 line
 * date:	2019-12-17
 * in:
 *	in_line
 * out:
 */
void fmain_draw_line(
	int	in_line	/* as selector */
) {

	if ( ( in_line * CHAR_BIT - CHAR_BIT ) < ( ( flag.vector_line * main_pixel_in_line_w ) + ( !flag.vector_line * main_pixel_in_line_h ) )	/* check on width or heigth */
	&& ( in_line + main_buffer_shift_line < MAIN_LINE_MAX )										/* check limit on shift */
	) {	/* in_line show { */

		int	pixel_in_line	= ( !flag.vector_line * main_pixel_in_line_w )
					+ (  flag.vector_line * main_pixel_in_line_h );	/* current work size width or heigth */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_draw_line]	: [%d]	pixel_in_line\n", pixel_in_line );
printf( "DEBUG	: [fmain_draw_line]	: [%d]	MAIN_LINE_SIZE_MAX\n", MAIN_LINE_SIZE_MAX );
#endif

		if ( MAIN_LINE_SIZE_MAX - main_buffer_shift_byte < pixel_in_line ) {
			pixel_in_line	= MAIN_LINE_SIZE_MAX - main_buffer_shift_byte;
		}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_draw_line]	: [%d]	pixel_in_line\n", pixel_in_line );
#endif

		char	byte;
		int	byte_x;
		int	byte_y;
		for ( int index_buffer = 0; index_buffer < pixel_in_line; index_buffer++ ) {	/* index_buffer { */
			byte	= *( main_buffer + ( MAIN_LINE_SIZE_MAX * ( in_line + main_buffer_shift_line ) ) + index_buffer + main_buffer_shift_byte );
			byte_x	= MAIN_LINE_SHIFT_X + ( ( (  flag.vector_line * CHAR_BIT * in_line ) + ( !flag.vector_line * index_buffer ) - (  flag.vector_line * main_buffer_shift_bit ) ) * main_pixel_size );
			byte_y	= MAIN_LINE_SHIFT_Y + ( ( ( !flag.vector_line * CHAR_BIT * in_line ) + (  flag.vector_line * index_buffer ) - ( !flag.vector_line * main_buffer_shift_bit ) ) * main_pixel_size );
			fmain_draw_byte( byte, byte_x, byte_y );
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_draw_line]	: [%02x]:byte	[%d]:x	[%d]:y\n", byte, byte_x, byte_y );
#endif
		}	/* } index_buffer */

	}	/* } in_line show */

}	/* } fmain_draw_line */
/* ------------- ---- ------
 * Function:	fmain_draw_win {
 * rem:		windown refresh
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_draw_win(
	void;
) {

	SDL_SetRenderDrawColor( main_ren, 0x00, 0x00, 0x00, 0xff );	/* clear */
	SDL_RenderClear( main_ren );

	main_pixel_in_line_w	= ( main_win_w - MAIN_LINE_SHIFT_X ) / main_pixel_size;
	main_pixel_in_line_h	= ( main_win_h - MAIN_LINE_SHIFT_Y ) / main_pixel_size;

	for ( int line_index = 0; line_index < MAIN_LINE_MAX; line_index++ ) {	/* line_index : draw buffer { */
		fmain_draw_line( line_index );
	}	/* } line_index : draw buffer */

	if ( flag.mode_grid == 1 ) {	/* draw grid { */
		int index_grid = 1;
		while ( main_grid_size * index_grid < main_win_w || main_grid_size * index_grid < main_win_h ) {	/* draw { */
			SDL_SetRenderDrawColor( main_ren, 0x00, 0x00, 0xff, 0xff );
			SDL_RenderDrawLine( main_ren, 0, main_grid_size * index_grid, main_win_w, main_grid_size * index_grid );
			SDL_RenderDrawLine( main_ren, main_grid_size * index_grid, 0, main_grid_size * index_grid, main_win_h );
			index_grid++;
		}	/* } draw */
	}	/* } draw grid */

	SDL_RenderPresent( main_ren );

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	vector_byte\n",			flag.vector_byte );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	vector_line\n",			flag.vector_line );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	grid\n",			flag.mode_grid );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_GRID_SIZE_MIN\n",		MAIN_GRID_SIZE_MIN );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_grid_size\n",		main_grid_size );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_GRID_SIZE_MAX\n",		MAIN_GRID_SIZE_MAX );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_PIXEL_SIZE_MIN\n",		MAIN_PIXEL_SIZE_MIN );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_pixel_size\n",		main_pixel_size );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_PIXEL_SIZE_MAX\n",		MAIN_PIXEL_SIZE_MAX );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_PIXEL_STEP\n",		MAIN_PIXEL_STEP );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_buffer_shift_byte\n",	main_buffer_shift_byte );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_buffer_shift_bit\n",	main_buffer_shift_bit );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_buffer_shift_line\n",	main_buffer_shift_line );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	CHAR_BIT\n",			CHAR_BIT );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_LINE_SIZE_MIN\n",		MAIN_LINE_SIZE_MIN );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_LINE_SIZE_MAX\n",		MAIN_LINE_SIZE_MAX );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_LINE_MAX\n",		MAIN_LINE_MAX );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_LINE_SHIFT_X\n",		MAIN_LINE_SHIFT_X );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	MAIN_LINE_SHIFT_Y\n",		MAIN_LINE_SHIFT_Y );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_win_w\n",			main_win_w );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_win_h\n",			main_win_h );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_filein_index\n",		main_filein_index );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_buffer_line_end\n",	main_buffer_line_end );
printf( "DEBUG	: [fmain_draw_win]	: [%6d]	main_buffer_line_end_size\n",	main_buffer_line_end_size );
#endif

}	/* } fmain_draw_win */
/* ------------- ---- ------
 * Function:	fmain_neg_vector_byte {
 * rem:		vector byte touch
 * date:	2019-12-13
 * in:
 * out:
 */
void	fmain_neg_vector_byte(
	void;
) {

	flag.vector_byte	= !flag.vector_byte;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_neg_vector_byte]	: [%d]	vector_byte\n", flag.vector_byte );
#endif

	fmain_draw_win();

}	/* } fmain_neg_vector_byte */
/* ------------- ---- ------
 * Function:	fmain_neg_vector_line {
 * rem:		vector line touch
 * date:	2019-12-13
 * in:
 * out:
 */
void	fmain_neg_vector_line(
	void;
) {

	flag.vector_line	= !flag.vector_line;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_neg_vector_line]	: [%d]	vector_line\n", flag.vector_line );
#endif

	fmain_draw_win();

}	/* } fmain_neg_vector_line */
/* ------------- ---- ------
 * Function:	fmain_neg_mode_screen {
 * rem:		mode fullscreen
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_neg_mode_screen(
	void;
) {

//FIXME		flag.mode_screen	= !flag.mode_screen;
//FIXME	
//FIXME		if ( flag.mode_screen == 0 ) {	/* flag.mode_screen { */
//FIXME		} else {
//FIXME		}
//FIXME	
//FIXME	#ifdef	MAIN_DEBUG
//FIXME	printf( "DEBUG	: [fmain_neg_mode_screen]	: [%d]	mode_screen\n", flag.mode_screen );
//FIXME	#endif
//FIXME	
//FIXME		fmain_draw_win();
//FIXME		fmain_draw_win();	/* bug: screen not refresh */

}	/* } fmain_neg_mode_screen */
/* ------------- ---- ------
 * Function:	fmain_neg_mode_keyboard {
 * rem:		mode key touch
 * date:	2019-12-17
 * in:
 * out:
 */
void	fmain_neg_mode_keyboard(
	void;
) {

	flag.mode_keyboard	= !flag.mode_keyboard;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_neg_mode_keyboard]	: [%d]	mode_keyboard\n", flag.mode_keyboard );
#endif

}	/* } fmain_neg_mode_keyboard */
/* ------------- ---- ------
 * Function:	fmain_neg_mode_grid {
 * rem:		mode_grid touch
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_neg_mode_grid(
	void;
) {

	flag.mode_grid	= !flag.mode_grid;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_neg_mode_grid]	: [%d]	mode_grid\n", flag.mode_grid );
#endif

	fmain_draw_win();

}	/* } fmain_neg_mode_grid */
/* ------------- ---- ------
 * Function:	fmain_size_grid_inc {
 * rem:		grid size increment
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_size_grid_inc(
	void;
) {

	if ( main_grid_size < MAIN_GRID_SIZE_MAX ) {
		main_grid_size	*= 2;
		if ( main_grid_size > MAIN_GRID_SIZE_MAX ) {
			main_grid_size	= MAIN_GRID_SIZE_MAX;
		}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_size_grid_inc]	: [%d]	MAIN_GRID_SIZE_MIN\n", MAIN_GRID_SIZE_MIN );
printf( "DEBUG	: [fmain_size_grid_inc]	: [%d]	main_grid_size\n", main_grid_size );
printf( "DEBUG	: [fmain_size_grid_inc]	: [%d]	MAIN_GRID_SIZE_MAX\n", MAIN_GRID_SIZE_MAX );
#endif
		fmain_draw_win();
	} else {
		main_grid_size	= MAIN_GRID_SIZE_MAX;
	}

}	/* } fmain_size_grid_inc */
/* ------------- ---- ------
 * Function:	fmain_size_grid_dec {
 * rem:		grid size decrement
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_size_grid_dec(
	void;
) {

	if ( main_grid_size > MAIN_GRID_SIZE_MIN ) {
		main_grid_size	/= 2;
		if ( main_grid_size < MAIN_GRID_SIZE_MIN ) {
			main_grid_size	= MAIN_GRID_SIZE_MIN;
		}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_size_grid_dec]	: [%d]	MAIN_GRID_SIZE_MIN\n", MAIN_GRID_SIZE_MIN );
printf( "DEBUG	: [fmain_size_grid_dec]	: [%d]	main_GRID_size\n", main_grid_size );
printf( "DEBUG	: [fmain_size_grid_dec]	: [%d]	MAIN_GRID_SIZE_MAX\n", MAIN_GRID_SIZE_MAX );
#endif
		fmain_draw_win();
	} else {
		main_grid_size	= MAIN_GRID_SIZE_MIN;
	}

}	/* } fmain_size_grid_dec */
/* ------------- ---- ------
 * Function:	fmain_size_pixel_inc {
 * rem:		pixel size increment
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_size_pixel_inc(
	void;
) {

	if ( main_pixel_size < MAIN_PIXEL_SIZE_MAX ) {
		main_pixel_size	+= MAIN_PIXEL_STEP;
		if ( main_pixel_size > MAIN_PIXEL_SIZE_MAX ) {
			main_pixel_size	= MAIN_PIXEL_SIZE_MAX;
		}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_size_pixel_inc]	: [%d]	MAIN_PIXEL_SIZE_MIN\n", MAIN_PIXEL_SIZE_MIN );
printf( "DEBUG	: [fmain_size_pixel_inc]	: [%d]	main_pixel_size\n", main_pixel_size );
printf( "DEBUG	: [fmain_size_pixel_inc]	: [%d]	MAIN_PIXEL_SIZE_MAX\n", MAIN_PIXEL_SIZE_MAX );
#endif
		fmain_draw_win();
	} else {
		main_pixel_size	= MAIN_PIXEL_SIZE_MAX;
	}

}	/* } fmain_size_pixel_inc */
/* ------------- ---- ------
 * Function:	fmain_size_pixel_dec {
 * rem:		pixel size decrement
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_size_pixel_dec(
	void;
) {

	if ( main_pixel_size > MAIN_PIXEL_SIZE_MIN ) {
		main_pixel_size	-= MAIN_PIXEL_STEP;
		if ( main_pixel_size < MAIN_PIXEL_SIZE_MIN ) {
			main_pixel_size	= MAIN_PIXEL_SIZE_MIN;
		}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_size_pixel_dec]	: [%d]	MAIN_PIXEL_SIZE_MIN\n", MAIN_PIXEL_SIZE_MIN );
printf( "DEBUG	: [fmain_size_pixel_dec]	: [%d]	main_pixel_size\n", main_pixel_size );
printf( "DEBUG	: [fmain_size_pixel_dec]	: [%d]	MAIN_PIXEL_SIZE_MAX\n", MAIN_PIXEL_SIZE_MAX );
#endif
		fmain_draw_win();
	} else {
		main_pixel_size	= MAIN_PIXEL_SIZE_MIN;
	}

}	/* } fmain_size_pixel_dec */
/* ------------- ---- ------
 * Function:	fmain_size_win {
 * rem:		window size
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_size_win(
	void
) {

	SDL_GetWindowSize( main_win, &main_win_w, &main_win_h );
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_size_win]	: [%d]:width x [%d]:heigth\n", main_win_w, main_win_h );
#endif

	fmain_draw_win();

}	/* } fmain_size_win */
/* ------------- ---- ------
 * Function:	fmain_shift_byte_dec {
 * rem:		byte shift dec
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_shift_byte_dec(
	void
) {

	if ( main_buffer_shift_byte > 0 ) {
		main_buffer_shift_byte--;
	}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_shift_byte_dec]	: [%d]	main_buffer_shift_byte\n", main_buffer_shift_byte );
#endif

	fmain_draw_win();

}	/* } fmain_shift_byte_dec */
/* ------------- ---- ------
 * Function:	fmain_shift_byte_inc {
 * rem:		byte shift inc
 * date:	2019-12-16
 * in:
 * out:
 */
void	fmain_shift_byte_inc(
	void
) {

	int	pixel_in_line	= ( !flag.vector_line * main_pixel_in_line_w )		/* see&rechek: fmain_draw_line */
				+ (  flag.vector_line * main_pixel_in_line_h );		/* see&rechek: fmain_draw_line */

	if ( main_buffer_shift_byte + pixel_in_line < MAIN_LINE_SIZE_MAX ) {
		main_buffer_shift_byte++;
	}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_shift_byte_inc]	: [%d]	main_buffer_shift_byte\n", main_buffer_shift_byte );
#endif

	fmain_draw_win();

}	/* } fmain_shift_byte_inc */
/* ------------- ---- ------
 * Function:	fmain_shift_bit_dec {
 * rem:		bit shift dec
 * date:	2019-12-17
 * in:
 * out:
 */
void	fmain_shift_bit_dec(
	void
) {

	if ( main_buffer_shift_bit > 0 ) {
		main_buffer_shift_bit--;
	} else if ( main_buffer_shift_line > 0 ) {
		main_buffer_shift_line--;
		main_buffer_shift_bit	= CHAR_BIT;
	} else {
		main_buffer_shift_bit	== 0;
		main_buffer_shift_line	== 0;
	}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_shift_bit_dec]	: [%d]	main_buffer_shift_bit\n", main_buffer_shift_bit );
printf( "DEBUG	: [fmain_shift_bit_dec]	: [%d]	main_buffer_shift_line\n", main_buffer_shift_line );
#endif

	fmain_draw_win();

}	/* } fmain_shift_bit_dec */
/* ------------- ---- ------
 * Function:	fmain_shift_bit_inc {
 * rem:		bit shift inc
 * date:	2019-12-17
 * in:
 * out:
 */
void	fmain_shift_bit_inc(
	void
) {

	int	pixel_in_line	= (  flag.vector_line * main_pixel_in_line_w )		/* !modify! see&rechek: fmain_draw_line */
				+ ( !flag.vector_line * main_pixel_in_line_h );		/* !modify! see&rechek: fmain_draw_line */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_shift_bit_inc]	: [%d]	main_pixel_in_line_w\n", main_pixel_in_line_w );
printf( "DEBUG	: [fmain_shift_bit_inc]	: [%d]	main_pixel_in_line_h\n", main_pixel_in_line_h );
printf( "DEBUG	: [fmain_shift_bit_inc]	: [%d]	pixel_in_line\n", pixel_in_line );
#endif

	if ( main_buffer_shift_bit < CHAR_BIT ) {
		main_buffer_shift_bit++;
		
	} else if ( main_buffer_shift_bit == CHAR_BIT
		&& main_buffer_shift_line < MAIN_LINE_MAX
		&& ( pixel_in_line + CHAR_BIT < ( MAIN_LINE_MAX - main_buffer_shift_line ) * CHAR_BIT  )
		) {
		main_buffer_shift_line++;
		main_buffer_shift_bit	= 0;
	} else if ( main_buffer_shift_bit == CHAR_BIT
		&& main_buffer_shift_line == MAIN_LINE_MAX - 1
		) {
		;
//	} else {
//		main_buffer_shift_bit	= CHAR_BIT;
//		main_buffer_shift_line	= MAIN_LINE_MAX - 1;
	}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_shift_bit_inc]	: [%d]	main_buffer_shift_bit\n", main_buffer_shift_bit );
printf( "DEBUG	: [fmain_shift_bit_inc]	: [%d]	main_buffer_shift_line\n", main_buffer_shift_line );
#endif

	fmain_draw_win();

}	/* } fmain_shift_bit_inc */
/* ------------- ---- ------
 * Function:	fmain_buffer_read {
 * rem:		read 1 line to main_buffer_read
 * date:	2019-12-18
 * in:
 *	in_filein_index
 * out:
 *	main_buffer_read_size	: -1 error
 */
ssize_t	fmain_buffer_read(
	int	in_filein_index
) {

	ssize_t	main_buffer_read_size	= pread( main_filein_fd, main_buffer_read, MAIN_LINE_SIZE_MAX, in_filein_index * MAIN_LINE_SIZE_MAX );

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read]	: [%d]	in_filein_index\n", in_filein_index );
printf( "DEBUG	: [fmain_buffer_read]	: [%d]	main_buffer_read_size\n", main_buffer_read_size );
#endif

	return( main_buffer_read_size );

}	/* } fmain_buffer_read */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_copy {
 * rem:		copy 1 line from main_buffer_read to main_buffer with clear
 * date:	2019-12-18
 * in:
 *	in_buffer_index
 *	in_buffer_size
 *
 * out:
 */
void	fmain_buffer_read_copy(
	int	in_buffer_index,
	ssize_t	in_buffer_size
) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_copy]	: [%d]	in_buffer_index\n", in_buffer_index );
printf( "DEBUG	: [fmain_buffer_read_copy]	: [%d]	in_buffer_size\n", in_buffer_size );
#endif

	if ( in_buffer_size != - 1 ) {	/* in_buffer_size { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_copy]	: mode 1. [clear&copy]\n" );
#endif
		memset( main_buffer + ( MAIN_LINE_SIZE_MAX * in_buffer_index ),	/* destination */
			MAIN_BUFFER_FILL,					/* fill */
			MAIN_LINE_SIZE_MAX );					/* size */
		memcpy( main_buffer + ( MAIN_LINE_SIZE_MAX * in_buffer_index ),	/* destination */
			main_buffer_read,					/* source */
			in_buffer_size * sizeof( char ) );			/* size */
	} else {	/* : in_buffer_size */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_copy]	: mode 9. [error:-1]\n" );
#endif
	}	/* } in_buffer_size */


}	/* } fmain_buffer_read_copy */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_line {
 * rem:		read 1 line to buffer
 * date:	2019-12-18
 * in:
 *	in_filein_index
 *	in_buffer_index
 *
 * out:
 *	main_buffer_read_size	: -1 error
 */
ssize_t	fmain_buffer_read_line(
	int	in_filein_index,
	int	in_buffer_index
) {

	ssize_t	main_buffer_read_size	= fmain_buffer_read( in_filein_index );

	if ( main_buffer_read_size != - 1 ) {	/* main_buffer_read_size { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_line]	: mode 2. [read]\n" );
#endif
		fmain_buffer_read_copy( in_buffer_index, main_buffer_read_size );
		if ( main_buffer_read_size != MAIN_LINE_SIZE_MAX ) {	/* unsize is end (tail) */
			main_buffer_line_end		= in_buffer_index;
			main_buffer_line_end_size	= main_buffer_read_size;
		}

	} else {	/* : main_buffer_read_line_size */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_line]	: mode 9. [error:-1]\n" );
#endif


	}	/* } main_buffer_read_size */

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_line]	: [%d]	in_filein_index\n", in_filein_index );
printf( "DEBUG	: [fmain_buffer_read_line]	: [%d]	in_buffer_index\n", in_buffer_index );
printf( "DEBUG	: [fmain_buffer_read_line]	: [%d]	main_buffer_read_size\n", main_buffer_read_size );
#endif
	return( main_buffer_read_size );

}	/* } fmain_buffer_read_line */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_full {
 * rem:		read buffer full, main_filein_index is selector
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_buffer_read_full(
	void
) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
#endif
	ssize_t	buffer_read_size	= MAIN_LINE_SIZE_MAX;
	for ( int main_buffer_index = 0; main_buffer_index < MAIN_LINE_MAX && buffer_read_size == MAIN_LINE_SIZE_MAX; main_buffer_index++ ) {	/* main_buffer_index { */
		buffer_read_size	= fmain_buffer_read_line( main_buffer_index + main_filein_index,
								  main_buffer_index );
		if ( buffer_read_size != -1 ) {
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_full]	: mode 1. [end&end_size]\n" );
#endif
			main_buffer_line_end		= main_buffer_index;
			main_buffer_line_end_size	= buffer_read_size;
		} else {
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_full]	: mode 9. [error:-1]\n" );
#endif
		}
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_buffer_index\n", main_buffer_index );
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	buffer_read_size\n", buffer_read_size );
#endif
	}	/* } main_buffer_index */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
printf( "DEBUG	: [fmain_buffer_read_full]	: [%d]	MAIN_LINE_SIZE_MAX\n", MAIN_LINE_SIZE_MAX );
#endif

}	/* } fmain_buffer_read_full */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_home {
 * rem:		read buffer at home
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_buffer_read_home(
	void
) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
#endif
	main_filein_index	= 0;

	fmain_buffer_read_full();
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
printf( "DEBUG	: [fmain_buffer_read_home]	: [%d]	MAIN_LINE_SIZE_MAX\n", MAIN_LINE_SIZE_MAX );
#endif

	fmain_draw_win();

}	/* } fmain_buffer_read_home */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_end {
 * rem:		read buffer at end
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_buffer_read_end(
	void
) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
#endif

	int	line_size	= MAIN_LINE_SIZE_MAX;	/* fix bug of type */
	struct stat	filein_stat;
	fstat( main_filein_fd, &filein_stat );
	if ( ( filein_stat.st_size / line_size < MAIN_LINE_MAX ) ||
	     ( filein_stat.st_size / line_size == MAIN_LINE_MAX && filein_stat.st_size % line_size == 0 )		/* last line & tail not found */
	) {
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_end]	: mode 1. [last/0]\n" );
#endif
		main_filein_index	= 0;
	} else if ( filein_stat.st_size / line_size == MAIN_LINE_MAX && filein_stat.st_size % line_size != 0 ) {	/* last line & tail found */
		main_filein_index	= 1;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_end]	: mode 2. [last/tail]\n" );
#endif
	} else if ( filein_stat.st_size / line_size > MAIN_LINE_MAX ) {
		main_filein_index	= filein_stat.st_size / line_size - MAIN_LINE_MAX;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_end]	: mode 3. [upper]\n" );
#endif
		if ( filein_stat.st_size % line_size != 0 ) {	/* tail found */
			main_filein_index++;
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_end]	: mode 4. [tail]\n" );
#endif
		}
	}

	fmain_buffer_read_full();
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	MAIN_LINE_SIZE_MAX\n", MAIN_LINE_SIZE_MAX );
#endif

	fmain_draw_win();

}	/* } fmain_buffer_read_end */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_dec {
 * rem:		line in buffer dec
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_buffer_read_dec(
	void
) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_dec]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_dec]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
#endif

	if ( main_filein_index != 0 ) {	/* main_filein_index { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 0. [not first]\n" );
#endif

		if ( main_buffer_line_end == 0 ) {	/* main_buffer_line_end { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 1. [fixed]\n" );
#endif
			memcpy( main_buffer + MAIN_LINE_SIZE_MAX,	/* destination */
				main_buffer,				/* source */
				MAIN_LINE_SIZE_MAX * sizeof( char ) );	/* size */
		} else {	/* : main_buffer_line_end */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 2. [move]\n" );
#endif

			for ( int buffer_index = main_buffer_line_end; buffer_index > 0; buffer_index-- ) {	/* buffer_index { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	buffer_index\n", buffer_index );
#endif
				memcpy( main_buffer + ( buffer_index * MAIN_LINE_SIZE_MAX ),				/* destination */
					main_buffer + ( buffer_index * MAIN_LINE_SIZE_MAX ) - MAIN_LINE_SIZE_MAX,	/* source */
					MAIN_LINE_SIZE_MAX * sizeof( char ) );						/* size */
			}	/* } buffer_index */

		}	/* } main_buffer_line_end */

		if ( main_buffer_line_end == MAIN_LINE_MAX - 1 ) {	/* main_buffer_line_end { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 3. [end last]\n" );
#endif
			main_buffer_line_end_size	= MAIN_LINE_SIZE_MAX;
		} else {	/* : ( main_buffer_line_end == 0 ) || ( main_buffer_line_end < MAIN_LINE_MAX - 1 ) */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 4. [end move]\n" );
#endif
			main_buffer_line_end++;
		}	/* } main_buffer_line_end */

		main_filein_index--;

		ssize_t	main_buffer_read_size	= fmain_buffer_read_line( main_filein_index, 0 );
		if ( main_buffer_read_size != -1 ) {
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 5. [read 0]\n" );
#endif
		} else {
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 9. [error:-1]\n" );
#endif
		}

	} else {	/* : main_filein_index */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: mode 8. [first]\n" );
#endif
	}	/* } main_filein_index */

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_dec]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_dec]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_dec]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	MAIN_LINE_SIZE_MAX\n", MAIN_LINE_SIZE_MAX );
#endif

	fmain_draw_win();

}	/* } fmain_buffer_read_dec */
/* ------------- ---- ------
 * Function:	fmain_buffer_read_inc {
 * rem:		line in buffer inc
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_buffer_read_inc(
	void
) {

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
#endif
	if ( main_buffer_line_end_size == MAIN_LINE_SIZE_MAX && main_buffer_line_end == MAIN_LINE_MAX - 1 ) {	/* last & full { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: mode 0. \n" );
#endif

		ssize_t	main_buffer_read_size	= MAIN_LINE_SIZE_MAX;
		main_buffer_read_size	= fmain_buffer_read( main_filein_index + MAIN_LINE_MAX );
		if ( main_buffer_read_size != -1 ) {	/* main_buffer_read_size { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: mode 1. [read]\n" );
#endif

			memcpy(	main_buffer,							/* destination */
				main_buffer + MAIN_LINE_SIZE_MAX,				/* source */
				( MAIN_LINE_MAX - 1 ) * MAIN_LINE_SIZE_MAX * sizeof( char ) );	/* size */
			fmain_buffer_read_copy( MAIN_LINE_MAX - 1, main_buffer_read_size );
			main_buffer_line_end		= MAIN_LINE_MAX - 1;
			main_buffer_line_end_size	= main_buffer_read_size;
			main_filein_index++;

		} else {	/* : main_buffer_read_size */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: mode 9. [error:-1]\n" );
#endif
		}	/* } main_buffer_read_size */

	} else {	/* : last & full */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: mode 8. [last]\n" );
#endif
	}	/* } last & full */

#ifdef	MAIN_DEBUG
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	main_filein_index\n", main_filein_index );
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	main_buffer_line_end\n", main_buffer_line_end );
printf( "DEBUG	: [fmain_buffer_read_inc]	: [%d]	main_buffer_line_end_size\n", main_buffer_line_end_size );
printf( "DEBUG	: [fmain_buffer_read_end]	: [%d]	MAIN_LINE_SIZE_MAX\n", MAIN_LINE_SIZE_MAX );
#endif

	fmain_draw_win();

}	/* } fmain_buffer_read_inc */
/* ------------- ---- ------
 * Function:	fmain_set_conf {
 * rem:		set configuration optimal
 * date:	2019-12-18
 * in:
 * out:
 */
void	fmain_set_conf(
	void
) {

	if ( flag.mode_grid == 0 ) {
		flag.mode_grid		= 1;
		main_grid_size		= MAIN_GRID_SIZE_MIN;
		main_pixel_size		= CHAR_BIT;
		flag.mode_keyboard	= 1;
		SDL_MaximizeWindow( main_win );
	} else {
		flag.mode_grid		= 0;
		main_grid_size		= MAIN_GRID_SIZE_MIN;
		main_pixel_size		= MAIN_PIXEL_SIZE_MIN;
		flag.mode_keyboard	= 0;
		SDL_RestoreWindow( main_win );
	}

	fmain_draw_win();

}	/* } fmain_set_conf */


/* ------------- ---- ------
 * Function:	main {
 * rem:		MAIN
 * date:	2019-12-18
 * in:
 *	argc
 *	**argv
 *	**envp
 * out:
 *	ret_status
 */
int main(
	int	argc,
	char	**argv,
	char	**envp
) {

	int	ret_status	= 0;	/* Return Code To OS */
	int	vi_sh_path;		/* PATH - number of shell parametr */
	int	vi_count;
	char	*vc_path;

/* show : license */
	fmain_copy();
	sleep(1);
	main_file_elf = argv[0];

/* find : --help | -h */
	for ( int vi_count = 0; vi_count < argc; vi_count++ ) {
		if ( ( ! strcmp( "-h", argv[ vi_count ] ) ) || ( ! strcmp( "--help", argv[ vi_count ] ) ) ) {
			fmain_help();
			exit( 0 );
			break;
		}
	}

/* find : --file | -f */
	for ( int vi_count = 0; vi_count < argc; vi_count++ ) {
		if ( ( ! strcmp( "-f", argv[ vi_count ] ) ) || ( ! strcmp( "--file", argv[ vi_count ] ) ) ) {
			main_filein	= argv[ vi_count + 1 ];
			break;
		}
	}

	if ( main_filein != NULL ) {	/* filein { ------------- ---- ------ */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [main filein]	file	: [%s]\n", main_filein );
#endif
		main_filein_fd = open( main_filein, O_RDONLY );
		if ( main_filein_fd != -1 ) {	/* main_filein_fd { */
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [main filein]	open	: [%s]\n", main_filein );
#endif
	/* | main_filein_fd { */

	main_buffer	= ( char * ) malloc( MAIN_BUFFER_SIZE );	/* main_buffer { ------------- ---- ------ */
	if ( main_buffer != NULL ) {	/* malloc main_buffer */
		main_buffer_read	= main_buffer + MAIN_BUFFER_SIZE - MAIN_LINE_SIZE_MAX;
	/* | main_buffer { */

	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {	/* SDL_Init { ------------- ---- ------ */
		ret_status	= 101;	/* ERROR: [101] SDL_Init */
		libc_sdl2_perror();
	} else {	/* : SDL_Init */
		main_win	= SDL_CreateWindow(	main_win_title,
							main_win_x,
							main_win_y,
							main_win_w,
							main_win_h,
							main_win_mode
							);
		if ( main_win == NULL ){	/* SDL_CreateWindow: main_win { */
			ret_status	= 102;	/* ERROR: [102] SDL_CreateWindow */
			libc_sdl2_perror();
		} else {	/* : SDL_CreateWindow: main_win_main */
			main_ren	= SDL_CreateRenderer( main_win, -1, SDL_RENDERER_ACCELERATED );
			if ( main_ren == NULL ) {	/* SDL_Render: main_ren { */
				libc_sdl2_perror();
				ret_status	= 103;	/* ERROR: [103] SDL_CreateRenderer */
			} else {	/* : SDL_Render: main_ren */
	/* | SDL2 main_win { */

	fmain_buffer_read_home();
	fmain_size_win();
//FIXME +1	fmain_draw_win();

	SDL_Event	main_event;	/* SDL2 Event { ------------- ---- ------ */
	const Uint8	*state		= SDL_GetKeyboardState( NULL );
	int		event_flag	= 0;

	while ( event_flag == 0 ) {	/* while event_flag { */
		SDL_WaitEvent( &main_event );

		if ( main_event.type == SDL_QUIT ) {	/* event.type { */
	event_flag	= 1;
		}	/* } event.type */

		if ( main_event.window.event == SDL_WINDOWEVENT_RESIZED ) {	/* event.window { */
	fmain_size_win();
		}	/* } event.window */

		if ( ( flag.mode_keyboard == 0 && main_event.key.repeat != 1 && main_event.key.state == SDL_PRESSED )	/* wait mode */
		|| ( flag.mode_keyboard == 1 && main_event.key.state == SDL_PRESSED )					/* not wait */
		) {	/* event.key { */
			if( state[ SDL_SCANCODE_GRAVE ] ) {
	fmain_neg_mode_keyboard();
			} else if( state[ SDL_SCANCODE_UP ] ) {
	if ( flag.vector_line == 1 ) {
		fmain_shift_byte_dec();
	} else {
		fmain_shift_bit_dec();
	}
			} else if( state[ SDL_SCANCODE_DOWN ] ) {
	if ( flag.vector_line == 1 ) {
		fmain_shift_byte_inc();
	} else {
		fmain_shift_bit_inc();
	}
			} else if( state[ SDL_SCANCODE_LEFT ] ) {
	if ( flag.vector_line == 0 ) {
		fmain_shift_byte_dec();
	} else {
		fmain_shift_bit_dec();
	}
			} else if( state[ SDL_SCANCODE_RIGHT ] ) {
	if ( flag.vector_line == 0 ) {
		fmain_shift_byte_inc();
	} else {
		fmain_shift_bit_inc();
	}
			} else if( state[ SDL_SCANCODE_HOME ] ) {
	fmain_buffer_read_home();
			} else if( state[ SDL_SCANCODE_END ] ) {
	fmain_buffer_read_end();
			} else if( state[ SDL_SCANCODE_PAGEUP ] ) {
	fmain_buffer_read_dec();
			} else if( state[ SDL_SCANCODE_PAGEDOWN ] ) {
	fmain_buffer_read_inc();
			} else if( state[ SDL_SCANCODE_LEFTBRACKET ] ) {
	fmain_neg_vector_byte();
			} else if( state[ SDL_SCANCODE_RIGHTBRACKET ] ) {
	fmain_neg_vector_line();
			} else if( state[ SDL_SCANCODE_SLASH ] ) {
	fmain_neg_mode_grid();
			} else if( state[ SDL_SCANCODE_COMMA ] ) {
	fmain_size_grid_dec();
			} else if( state[ SDL_SCANCODE_PERIOD ] ) {
	fmain_size_grid_inc();
			} else if( state[ SDL_SCANCODE_MINUS ] ) {
	fmain_size_pixel_dec();
			} else if( state[ SDL_SCANCODE_EQUALS ] ) {
	fmain_size_pixel_inc();
			} else if( state[ SDL_SCANCODE_TAB ] ) {
	fmain_set_conf();
			} else if( state[ SDL_SCANCODE_F11 ] ) {
	fmain_neg_mode_screen();
			}
		}

	}	/* } while event flag */
	/* } SDL_Event */

	/* | } SDL2 main_win */
			}	/* : SDL_Render: main_ren */
			SDL_DestroyRenderer( main_ren );
			main_ren	= NULL;
		}	/* } SDL_CreateWindow: main_win */
		SDL_DestroyWindow( main_win );
		main_win	= NULL;
	}	/* } SDL_Init */
	SDL_Quit();	/* SDL_Quit */

	/* | } main_buffer */
		free( main_buffer );
	} else {	/* : malloc nain_buffer */
		fmain_perror( 0 );	/* malloc main_buffer */
	}	/* } malloc main_buffer } main_buffer */

	/* | } main_filein_fd */
			close( main_filein_fd );
#ifdef	MAIN_DEBUG
printf( "DEBUG	: [main filein]	close	: [%s]\n", main_filein );
#endif
		} else {	/* : main_filein_fd */
		fmain_help();
		fmain_perror( ENOENT );
		}	/* } main_filein_fd */
	} else {	/* : filein */
		fmain_help();
		fmain_perror( EINVAL );
	}	/* } filein */

	return( ret_status );

}	/* } main */


/* } DavydovMA 2019121800 */