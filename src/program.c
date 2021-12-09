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

/* PUBLIC METHODS */
static void read_line(prgm_t *program);
static void free_program(prgm_t *program);
static void print_prompt(prgm_t *program);

/* PRIVATE */
static void evaluate(prgm_t *program);
static void builtins(prgm_t *program);
static void pass_through(prgm_t *program);
static void assign_var(prgm_t *program);
static void free_ast(node_t *ast);


/* PUBLIC INITIALIZER */
prgm_t *init_program(char **envs)
{
  prgm_t *program           = (prgm_t*)malloc(sizeof(prgm_t));
  program->cmd              = (cmd_t*)malloc(sizeof(cmd_t));
  program->exec             = init_exec();
  program->env              = init_env();
  program->lexer            = init_lexer();
  program->parser           = init_parser();
  program->ast              = NULL;
  program->free             = free_program;
  program->print_prompt     = print_prompt;
  program->readline         = read_line;
  program->is_exit          = false;
  program->cmd->line        = NULL;


  program->env->load(program->env, envs);

  return program;
}

/* PUBLIC METHODS */

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
  program->ast = NULL;
  free(temp);

  if(lexer_line != NULL)
    free(lexer_line);
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
  free(program->exec);
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

static void evaluate(prgm_t *program)
{
  switch(program->ast->type)
  {
    case PASS_THROUGH:
      pass_through(program);
      return;

    case ASSIGN_OPERATOR:
      assign_var(program);
      return;

    default:
      builtins(program);
  }
}


static void assign_var(prgm_t *program)
{
    program->env->vars->insert(program->env->vars, program->ast->left->value, program->ast->right->value);

}



static void pass_through(prgm_t *program)
{
  int index                   = 0;
  node_t *root                = program->ast;
  program->exec->bin          = root->value;

  while(root)
  {
    /* only 99 argvs allowed */
    if(index >= MAX_ARGV_LEN - 1)
      break;

    program->exec->argv[index] = root->value;

    root = root->left;
    index++;
  }

  program->exec->argv[index]   = NULL;
  program->exec->envp          = program->env->vars->to_array(program->env->temp_vars);

  program->exec->execute(program->exec, program->env->paths, program->env->paths_len);
  program->exec->free_envp(program->exec);

}

/**/

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
      env(program);
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
      printf("execute WHICH\n");
      return;

    case PWD:
      printf("%s\n", program->env->pwd);
      return;

    default:
      printf("zsh: command not found\n");
  }
}

/**/

static void free_ast(node_t *ast)
{
  if(ast == NULL)
    return;

  free_ast(ast->left);
  free_ast(ast->right);

  if(ast->value)
  {
    free(ast->value);
    ast->value = NULL;
  }

  free(ast);
}

/**/


