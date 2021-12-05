/*
 * =====================================================================================
 *
 *       Filename:  program.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/21 12:54:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/program.h"

static void read_line(prgm_t *program);
static void free_program(prgm_t *program);
static void print_prompt(prgm_t *program);
static void exit_program(prgm_t *program);
static void evaluate(prgm_t *program);
static void builtins(prgm_t *program);
static void assign_var(prgm_t *program);
static void echo(prgm_t *program);
static void cd(prgm_t *program);
static void free_ast(node_t *ast);

prgm_t *init_program(char **envs)
{
  prgm_t *program           = (prgm_t*)malloc(sizeof(prgm_t));
  program->cmd              = (cmd_t*)malloc(sizeof(cmd_t));
  program->env              = init_env();
  program->lexer            = init_lexer();
  program->parser           = init_parser();
  program->free             = free_program;
  program->print_prompt     = print_prompt;
  program->readline         = read_line;
  program->is_exit          = false;
  program->cmd->line        = NULL;

  program->env->load(program->env, envs);

  return program;
}

/**/

static void read_line(prgm_t *program)
{
  char *lexer_line          = program->lexer->line;
  char *temp                = program->cmd->line;
  program->cmd->line        = readline("");
  program->cmd->len         = strlen(program->cmd->line);

  program->lexer->load(program->lexer, program->cmd->line);
  program->ast = program->parser->parse(program->parser, program->lexer);

  evaluate(program);

  add_history(program->cmd->line);

  free_ast(program->ast);
  free(temp);

  if(lexer_line != NULL)
    free(lexer_line);
}

/**/

static void evaluate(prgm_t *program)
{
  switch(program->ast->type)
  {
    case PASS_THROUGH:
      printf("fork and execute!\n");
      return;

    case ASSIGN_OPERATOR:
      assign_var(program);
      return;

    default:
      builtins(program);
  }
}


/**/

static void free_program(prgm_t *program)
{
  program->env->free(program->env);

  free(program->lexer->line);
  free(program->lexer);
  free(program->parser);
  free(program->cmd->line);
  free(program->cmd);
  free(program);
}

/**/

static void print_prompt(prgm_t *program) 
{
  /* update pwd at every command in case of cd */
  entry_t *pwd          = program->env->vars->get(program->env->vars, PWD_ENV);
  program->env->pwd     = pwd->pair;

  printf("%s %s %s %s", program->env->user, PSEP, program->env->pwd, PSUFIX);

}

/* PRIVATE */

static void assign_var(prgm_t *program)
{
    program->env->vars->insert(program->env->vars, program->ast->left->value, program->ast->right->value);

}


static void free_ast(node_t *ast)
{
  if(ast == NULL)
    return;

  free_ast(ast->left);
  free_ast(ast->right);

  if(ast->value)
    free(ast->value);

  free(ast);
}

static void builtins(prgm_t *program)
{
  switch(program->ast->type)
  {
    case ECHO:
      echo(program);
      return;

    case CD:
      cd(program);
      return;

    case ENV:
      printf("execute ENV\n");
      return;

    case SETENV:
      printf("execute SETENV\n");
      return;

    case UNSETENV:
      printf("execute UNSETENV\n");
      return;
    case EXIT:
      exit_program(program);
      return;

    case WHICH:
      printf("execute SETENV\n");
      return;

    default:
      printf("zsh: command not found\n");
  }
}


static void exit_program(prgm_t *program)
{
  program->is_exit = true;
}


static void echo(prgm_t *program)
{
  /* when not a binary expression, always take left */
  node_t *root = program->ast->left;
  node_t *literal;

  while(root)
  {
    if(root->type == LITERAL || root->type == WHITESPACE)
      printf("%s", root->value); 

    if(root->type == VARIABLE)
    {
      entry_t *var = program->env->vars->get(program->env->vars, root->value);

      if(var)
        printf("%s", var->pair);
    }

    if(root->type == LINE_BREAK)
      printf("\n"); 


    root = root->left;
  }

  printf("\n"); 

} 

static void cd(prgm_t *program)
{

  pathnode_t *dir_names = split_path(program->ast->left->value);

  entry_t *pwd = program->env->vars->get(program->env->vars, PWD_ENV);

  if(path_exists(dir_names, pwd->pair))
  {
    printf("found.\n");
  }
  else
    printf("No such directory.\n");

  //  if(program->ast->left->value)

}


