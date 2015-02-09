#ifndef __RMSC_H
#define __RMSC_H

struct ast {
	char *name;

	struct ast *l;

	struct ast *r;

	int pointer;
};

struct ast *ast_new_type(const char *name);

struct ast *ast_new_struct(const char *name, struct ast *right);

struct ast *ast_newp_struct(const char *name, struct ast *right);

struct ast *ast_set_field(const char *name, struct ast *left, struct ast *right);

struct ast *ast_setp_field(const char *name, struct ast *left, struct ast *right);

void do_work(struct ast* root);

#endif /* __RMSC_H */
