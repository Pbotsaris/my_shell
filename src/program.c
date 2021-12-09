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
static void pass_through(prgm_t *program);
static void assign_var(prgm_t *program);
static void echo(prgm_t *program);
static void cd(prgm_t *program);
static void env(prgm_t *program);

static envflag_t extract_flags(node_t **root);
static void extract_env(prgm_t *program, node_t *root, bool was_assignment);
static void extract_command(prgm_t *program, node_t *root);
static void restore_temp_env(prgm_t *program);

static void free_ast(node_t *ast);

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
  program->ast = NULL;
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
      pass_through(program);
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

static void assign_var(prgm_t *program)
{
    program->env->vars->insert(program->env->vars, program->ast->left->value, program->ast->right->value);

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

static void exit_program(prgm_t *program)
{
  program->is_exit = true;
}


/**/

static void echo(prgm_t *program)
{
  /* when not a binary expression, always take left */
  node_t *root              = program->ast->left;

  while(root)
  {
    if(root->type == LITERAL || root->type == WHITESPACE)
      printf("%s", root->value); 

    if(root->type == VARIABLE)
    {
      entry_t *var         = program->env->vars->get(program->env->vars, root->value);

      if(var)
        printf("%s", var->pair);
    }

    if(root->type == LINE_BREAK)
      printf("\n"); 

    root = root->left;
  }

  printf("\n"); 

} 

/**/

static void cd(prgm_t *program)
{ 

  if((strcmp(program->ast->left->value, HOME))== 0)
  {
    entry_t *home = program->env->vars->get(program->env->vars, HOME_ENV);
    program->env->vars->insert(program->env->vars, PWD_ENV, home->pair);
    return;
  }

  pathnode_t *dir_names   = split_path(program->ast->left->value);
  entry_t *pwd            = program->env->vars->get(program->env->vars, PWD_ENV);

  if(path_exists(dir_names, pwd->pair))
  {
    char *npwd = new_pwd(pwd->pair, program->ast->left->value);
    program->env->vars->insert(program->env->vars, PWD_ENV, npwd);
    free(npwd);
  }
  else
    printf("No such directory\n");

  free_paths(dir_names);

}

/**/

static void env(prgm_t *program)
{
  node_t *root              = program->ast->left;
  envflag_t flag            = extract_flags(&root);
  program->exec->root       = NULL;

  /* env with no operands */
  if(!root && flag == INIT)
    program->env->print(program->env);

  if(flag == UNSET)
  {
    if(!program->env->temp_vars->destroy(program->env->temp_vars, root->value))
      printf("%s does not exist in the enviroment\n", root->value);

    root = root->left;
  }

  /* extracts env vars assigment operands. stores new root position in program->exec->root to extract command */
  if(root && root->type == ASSIGN_OPERATOR)
    extract_env(program, root, false);


  program->exec->envp = program->env->temp_vars->to_array(program->env->temp_vars);
  /* if no assigment, set program->exec->root manually */
  if(!program->exec->root)
    program->exec->root = root;

  /* extract command to be execute via env*/
  if(program->exec->root)
    extract_command(program, program->exec->root);

  restore_temp_env(program);
  program->exec->free_envp(program->exec);

}

/**/

static envflag_t extract_flags(node_t **root)
{
  envflag_t flag           = INIT;

  while(*root && ((*root)->type == FLAG || (*root)->type == DOUBLE_FLAG))
  {
    /* env takes a single option. if more flags are passed in skip remaining */
    if(flag != INIT)
    {
      *root                = (*root)->left;
      continue;
    }

    if((strcmp((*root)->value, IGNORE_SINGLE) == 0) || strcmp((*root)->value, IGNORE_DOUBLE) == 0 )
      flag = IGNORE;

    else if((strcmp((*root)->value, UNSET_SINGLE) == 0))
      flag = UNSET;

    else if((strcmp((*root)->value, NILL_SINGLE) == 0) || strcmp((*root)->value, NILL_DOUBLE) == 0 )
      flag = NILL;

    else if((strcmp((*root)->value, CDIR_SINGLE) == 0) || strcmp((*root)->value, CDIR_DOUBLE) == 0 )
      flag = CDIR;

    else
      return ERR;

    /* uniary operations always to left */
    *root = (*root)->left;
  }

  return flag;

}

/**/

static void extract_env(prgm_t *program, node_t *root, bool was_assignment)
{
  bool was_assignment_next   = false;

  if(!root)
    return;

  if(root->type == ASSIGN_OPERATOR)
  {
    program->env->temp_vars->insert(program->env->temp_vars, root->left->value, root->right->value);
    was_assignment_next = true;
  }

  if(!was_assignment && !(root->type == ASSIGN_OPERATOR))
  {
    program->exec->root = root;
    return;
  }

  extract_env(program, root->left, was_assignment_next);
  extract_env(program, root->right, was_assignment_next);
}

/**/

static void extract_command(prgm_t *program, node_t *root)
{

  /* root must have a value */
  if(!root && !root->value)
    return;

  int index             = 0; 
  program->exec->bin    = root->value;

  while(root)
  {
    /* limit of 99 arguments */
    if(index >= MAX_ARGV_LEN)
      break;

    program->exec->argv[index] = root->value;
    root = root->left;

    index++;

  }

  /* last item is a null */
  program->exec->argv[index] = NULL;

}

/**/

static void restore_temp_env(prgm_t *program)
{

  char **env        = program->env->vars->to_array(program->env->vars);
  int count         = 0;

  program->env->temp_vars->destroy_all(program->env->temp_vars);
  program->env->restore_env(program->env, env, false);

  while(env[count])
  {
    free(env[count]);
    count++;
  }

  free(env);

}



