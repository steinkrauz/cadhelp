#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE	4096
#define HDR_CONTENT "Content-type: text/html; charset=windows-1251"
#define STYLE_LINK  "<link rel=\"stylesheet\" href=\"/cadhelp.css\"/>"

/*----------------------------------------------------------------------------*/
void server_responce() {
	puts(HDR_CONTENT);
	puts("");
}
/*----------------------------------------------------------------------------*/
void chomp(char *Str) {
	char *c;
	while (NULL != Str && NULL != (c = strrchr(Str, '\n'))){
		*c = '\0';
	}
}
/*----------------------------------------------------------------------------*/
void header(char *Title) {
	chomp(Title);
	puts("<html><head>");
	printf("<title>%s</title>\n", Title);
	puts(STYLE_LINK);
	puts("</head><body>");
}
/*----------------------------------------------------------------------------*/
void footer() {
	puts("</body></html>");
}
/*----------------------------------------------------------------------------*/
void do_404() {
	header("Error");
	puts("<h1>File not found or another error!");
	footer();
	exit(0);
}
/*----------------------------------------------------------------------------*/
void make_img(char *ImgName) {
	char file[128];
	strcpy(file,ImgName);
	if (!strstr(file,".")) strcat(file,".jpg");
	printf("<img src=\"%s\" />",file);
}
/*----------------------------------------------------------------------------*/
char *get_link_str(char *str) {
	char *c = str;
	while(*c!='}'&&*c!='\0') c++;
	if (*c=='\0') return NULL;
	*c='\0';
	return ++c;
}
/*----------------------------------------------------------------------------*/
__inline void make_anchor(char *Name, char *Link)  {
	printf("<a href=\"%s\" >%s</a>",Link,Name);
}
/*----------------------------------------------------------------------------*/
void do_link(char **str){
	char *link = *str;
	char *c;
	if (*link=='{') {
		char *name;
		++link;
		c = get_link_str(link);
		if (!c) return;
		if (*c=='}') {
			name = link;
			++c;
		} else {
			name = c;
			c = get_link_str(name);
			if (!c) return;
		}
		make_anchor(name, link);
	} else {
		c = get_link_str(link);
		if (!c) return;
		make_img(link);
	}
	*str = c;
}
/*----------------------------------------------------------------------------*/
void process_file(FILE *in) {
	char buf[STR_SIZE];
	fgets(buf, STR_SIZE, in);
	if (buf[0]=='\n'||buf[0]=='\0') header("CADINFO");
	else header(buf);
	chomp(buf);
	printf("<h2>%s</h2>",buf);
	while (fgets(buf, STR_SIZE, in)){
		char *c = buf;
		if (*c=='\n') { 
			printf("\n<p>");
			continue;
		}
		if (*c=='-') printf("<br>");
		while(*c) {
			if (*c!='{') {
				putchar(*c);
				++c;
			} else {
				++c;
				do_link(&c);
			}
		}
	}
}
/*----------------------------------------------------------------------------*/

