#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "..\libcadhelp\libcadhelp.h"
/*----------------------------------------------------------------------------*/
char *get_dir(char *IdxName) {
	char *c = strstr(IdxName,"part.idx");
	if (c) {
		char *dir;
		*c = '\0';
		dir = _strdup(IdxName);
		*c = 'p';
		return dir;
	}
	return NULL;
}
/*----------------------------------------------------------------------------*/
void publish_entry(char *RootMask, char *SubDir, char *FileName) {
	char *c = strstr(RootMask, "*.*");
	char *ind_file;
	FILE *in;

	if (c) *c = '\0';
	ind_file = (char *)malloc(_MAX_PATH);
	sprintf_s(ind_file,_MAX_PATH,"%s%s\\%s", RootMask, SubDir, FileName);
	in = fopen(ind_file,"rt");
	if (!in) {
		free(ind_file);
	} else {
		char head[1024];
		fgets(head,1024,in);
		fclose(in);
		sprintf_s(ind_file,_MAX_PATH,"%s\\%s", SubDir, FileName);
		printf("<li>");
		make_anchor(head, ind_file);
		free(ind_file);
		return;
	}
}
/*----------------------------------------------------------------------------*/
char *str_by_mask(char *Mask, char *Name) {
	int len = strlen(Name) + strlen(Mask) + 1;
	char *str = (char *)malloc(len);
	sprintf_s(str, len, Mask, Name);
	return str;
}
#define GET_TEXTFILE(name) str_by_mask("%s.txt", name);
#define GET_DIRMASK(dir) str_by_mask("%s\\*.*", dir);
/*----------------------------------------------------------------------------*/
void process_dirs(char *Dir) {
	intptr_t fh;
	struct _finddata_t cd;
	char *dir_mask = GET_DIRMASK(Dir);

	puts("<div class=\"dirs\"><ul>");
	if ((fh=_findfirst(dir_mask,&cd))!=-1) do {
		if (cd.name[0]=='.') continue;
		if (cd.attrib & _A_SUBDIR)
			publish_entry(Dir, cd.name, "part.idx");
	}
	while(_findnext(fh, &cd)==0);
	_findclose(fh);
	puts("</ul></div>");
}
/*----------------------------------------------------------------------------*/
void process_txt(char *Dir) {
	intptr_t fh;
	struct _finddata_t cd;
	char *dir_mask = GET_DIRMASK(Dir);

	puts("<ul>");
	if ((fh=_findfirst(dir_mask,&cd))!=-1) do {
		char *txt_file;
		if (cd.name[0]=='.') continue;
		txt_file = GET_TEXTFILE(cd.name);
		if (cd.attrib & _A_SUBDIR)
			publish_entry(Dir, cd.name, txt_file);
		free(txt_file);
	}
	while(_findnext(fh, &cd)==0);
	_findclose(fh);
	puts("</ul>");
}
/*----------------------------------------------------------------------------*/
int main(int argc, char * argv[])
{
	char *in_file;
	char *in_dir;
	FILE *in;

	server_responce();
	in_file = getenv("PATH_TRANSLATED");
	if (!in_file) do_404();
	if ((in=fopen(in_file,"rt"))==0) do_404();
	if ((in_dir=get_dir(in_file))==NULL) {
		fclose(in);
		do_404();
	}
	process_file(in);
	fclose(in);

	puts("<p>");
	process_dirs(in_dir);
	puts("<p>");
	process_txt(in_dir);
	
	free(in_dir);

	footer();

	return 0;
}
