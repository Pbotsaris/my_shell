/*
 * =====================================================================================
 *
 *       Filename:  builtins.c
 *
 *    Description: builtins commands 
 *
 *        Version:  1.0
 *        Created:  12/09/21 09:30:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/program.h"
#include "../include/builtins.h"

/* PRIVATE HELPERS*/
static envflag_t extract_flags(node_t **root);
static void extract_env(prgm_t *program, node_t *root, bool was_assignment);
static void extract_command(prgm_t *program, node_t *root);
static void restore_temp_env(prgm_t *program);

 void echo(prgm_t *program)
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

void exit_program(prgm_t *program)
{
  program->is_exit = true;
}

/**/

void cd(prgm_t *program)
{ 

  if((strcmp(program->ast->left->value, PREVPWD))== 0)
  {
   size_t len        = strlen(program->env->pwd);
   char buffer[len + 1];
   strncpy(buffer, program->env->pwd, len);
   buffer[len]       = '\0';

   program->env->vars->insert(program->env->vars, PWD_ENV, program->env->prev_pwd);
   program->env->update_pwdprev(program->env, buffer);
   return;

  }

  if((strcmp(program->ast->left->value, HOME))== 0)
  {

    program->env->update_pwdprev(program->env, program->env->pwd);
    
    entry_t *home = program->env->vars->get(program->env->vars, HOME_ENV);
    program->env->vars->insert(program->env->vars, PWD_ENV, home->pair);
    return;
  }

  pathnode_t *dir_names   = split_path(program->ast->left->value);
  entry_t *pwd            = program->env->vars->get(program->env->vars, PWD_ENV);

  if(path_exists(dir_names, pwd->pair))
  {

    program->env->update_pwdprev(program->env, program->env->pwd);
    char *npwd = new_pwd(pwd->pair, program->ast->left->value);
    program->env->vars->insert(program->env->vars, PWD_ENV, npwd);
    free(npwd);
  }
  else
    printf("No such directory\n");

  free_paths(dir_names);

}

/**/

void env(prgm_t *program)
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

/* PRIVATE HELPERS */

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



