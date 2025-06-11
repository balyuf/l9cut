#include <stdlib.h>
#include <stdio.h>

/*
   l9dis.c

   - extracts Level 9 pictures

   Source home: https://github.com/balyuf/l9cut.git
   Archived at: https://ifarchive.org/indexes/if-archive/level9/tools/
   Integrated part of: https://github.com/DavidKinder/Level9.git
 */

int findsubs(unsigned char* testptr, unsigned int testsize, unsigned char** picdata, unsigned int *picsize, int retries)
{
	int retval=0;
	unsigned int i, j, number, length, count;
	unsigned char *picptr, *startptr, *tmpptr, *beginptr=testptr;

	if (testsize < 16) return 0;

	/*
		Try to traverse the graphics subroutines.

		Each subroutine starts with a header: nn | nl | ll
		nnn : the subroutine number ( 0x000 - 0x7ff )
		lll : the subroutine length ( 0x004 - 0x3ff )

		The first subroutine usually has the number 0x000.
		Each subroutine ends with 0xff.

		findsubs() searches for the header of the second subroutine
		(pattern: 0xff | nn | nl | ll) and then tries to find the
		first and next subroutines by evaluating the length fields
		of the subroutine headers.
	*/
	for (i = 0; i < (int)(testsize - 4); i++)
	{
		picptr = testptr + i;
		startptr = picptr;
		count = 0;

		while (1)
		{
			number = ((*(picptr)) << 4) + ((*(picptr + 1) & 0xf0) >> 4);
			length = ((*(picptr + 1) & 0x0f) << 8) + *(picptr + 2);
			if (count == 0 && (number != 0 || length > 0x20))
				break;
			if ((*picptr & 0x80) || length < 4 || length > 0x3ff || picptr + length + 4 > testptr + testsize || (number != 0 && count == 0))
				break;

			picptr += length;
			if (*(picptr - 1) != 0xff)
			{
				picptr -= length;
				break;
			}

			count++;
		}

		if (count > 10)
		{
			printf("Number of pictures = %d\n", count);
			if (*picptr != 0xff)
			{
				if (retries > 0)
				{
					printf("Incomplete picture data at $%04lx ?  Continuing search ...\n", startptr - beginptr);
					testsize -= (picptr - testptr);
					testptr = picptr;
					i = 3;
					retries--;
					retval = 3;
					continue;
				}
				else
				{
					printf("Partial picture data at $%04lx ?\n", startptr - beginptr);
					*picptr = 0xff;
					*(picptr + 1) = 0x1a;
					*(picptr + 2) = 0;
					*(picptr + 3) = 0;
					retval = 2;
				}
			}
			else
			{
				printf("Found picture data at $%04lx !\n", startptr - beginptr);
				retval = 1;
			}

			*picdata = startptr;
			*picsize = picptr - startptr;
			return retval;
		}
	}
	printf("Picture data not found!\n");
	return -retval;
}


int main(int argc, char **argv)
{
	unsigned char *infiledata, *picturedata;
	unsigned int picturesize;
	FILE *fdi, *fdo;
	int retval;

	if ((infiledata = malloc(65540)) != NULL)
	{
		if ((fdi = fopen(argv[1], "rb")) != NULL)
		{
			fread(infiledata, 65536, 1, fdi);
			fclose(fdi);
			printf("Searching for picture data ...\n");
			retval = findsubs(infiledata, 65536, &picturedata, &picturesize, (argv[3] == NULL) ? 65536 : atoi(argv[3]));
			if (retval > 0)
			{
				if ((fdo = fopen(argv[2], "wb")) != NULL)
				{
					printf("Writing pic file ...\n");
					fwrite(picturedata, picturesize + 4, 1, fdo);
					fclose(fdo);
					return 0;
				}
			}
			else
				return -retval;
		}
	}
	return -1;
}
