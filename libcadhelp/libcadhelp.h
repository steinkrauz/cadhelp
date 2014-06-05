#ifndef LIBCADHELP_H
#define LIBCADHELP_H

/*
Low level output
*/
void server_responce();
void header(char *Title);
void footer();
void make_img(char *ImgName);
void make_anchor(char *Name, char *Link);
/*
Error handlers
*/
void do_404();

/*
Content handlers
*/
void process_file(FILE *in);

#endif