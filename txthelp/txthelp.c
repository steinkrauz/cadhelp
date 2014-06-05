#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\libcadhelp\libcadhelp.h"

int main(int argc, char * argv[])
{
	char *in_file;
	FILE *in;

	server_responce();
	in_file = getenv("PATH_TRANSLATED");
	if (!in_file) do_404();
	if ((in=fopen(in_file,"rt"))==0) do_404();
	process_file(in);
	
	fclose(in);
	footer();

	return 0;
}
