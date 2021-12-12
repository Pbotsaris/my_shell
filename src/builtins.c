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

/* CD HELPERS*/
static void cd_handle_prevpwd(prgm_t *program);
static void cd_handle_home(prgm_t *program);
static void cd_handle_change(prgm_t *program, char *new_pwd);

/* ENV HELPERS*/
static envflag_t extract_flags(node_t **root);
static void extract_env(prgm_t *program, node_t *root, bool was_assignment);
static void extract_command(prgm_t *program, node_t *root);
static void restore_temp_env(prgm_t *program);
static bool handle_flags(prgm_t *program, node_t *root, envflag_t flag);
static node_t *handle_unset(prgm_t *program, node_t *root, envflag_t flag);


/* SETENV HELPERS */
static void setenv_binary_expression(prgm_t *program, node_t *root);
static void setenv_unary_expression(prgm_t *program, node_t *root);

/* PUBLIC */

void exit_program(prgm_t *program) { program->is_exit = true; }

/**/

void set_env(prgm_t *program)
{
  node_t *root = program->ast->left;

  if(root->type == ASSIGN_OPERATOR)
    setenv_binary_expression(program, root);
  else
    setenv_unary_expression(program, root);
}

/**/

void unset_env(prgm_t *program)
{
  node_t *root = program->ast->left;

  while(root)
  {

    program->env->vars->destroy(program->env->vars, root->value);
    root = root->left;

  }
}

/**/

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

void cd(prgm_t *program)
{ 
  char *new_pwd;
  char *path = program->ast->left->value;

  if((strcmp(path, PREVPWD))== 0)
  {
    cd_handle_prevpwd(program);
    return;
  }

  if((strcmp(path, HOME))== 0)
  {
    cd_handle_home(program);
    return;
  }

  if(path[0] == '/')
    new_pwd                 = join_path(ROOT, ++path); /* move pointer to skip '/' */
  else
    new_pwd                 = join_path(program->env->pwd, path);

  pathnode_t *pathhead      = split_to_list(new_pwd);
  new_pwd                   = build_path(pathhead);

  if(dir_exists(new_pwd))
    cd_handle_change(program, new_pwd);

  free(new_pwd);
}

/**/

void env(prgm_t *program)
{
  node_t *root              = program->ast->left;
  envflag_t flag            = extract_flags(&root);
  program->exec->root       = NULL;

  if(handle_flags(program, root, flag))
    return;

  /* extracts env vars assigment operands. stores new root position in program->exec->root to extract command */
  if(root && root->type == ASSIGN_OPERATOR)
    extract_env(program, root, flag == NILL);

  if(flag == IGNORE)
    program->exec->empty_envp(program->exec);

  else
    program->exec->envp = program->env->temp_vars->to_array(program->env->temp_vars);

  /* if no assigment, set program->exec->root manually */
  if(!program->exec->root)
    program->exec->root = root;

  /* extract command to be execute via env*/
  if(program->exec->root)
    extract_command(program, program->exec->root);

  /* print if no cmd to execute */
  if(!program->exec->bin)
    program->env->print_temp(program->env, flag == NILL);
  else
    /* this will execute the command */
    program->exec->execute(program->exec, program->env->paths, program->env->paths_len);

  /* restore temp env according to env */
  restore_temp_env(program);

  program->exec->free_envp(program->exec);

}

/*PRIVATE: CD HELPERS */

static void cd_handle_prevpwd(prgm_t *program)
{

  size_t len        = strlen(program->env->pwd);
  char buffer[len + 1];
  strncpy(buffer, program->env->pwd, len);
  buffer[len]       = '\0';

  program->env->vars->insert(program->env->vars, PWD_ENV, program->env->prev_pwd);
  program->env->update_pwdprev(program->env, buffer);

}

/**/

static void cd_handle_home(prgm_t *program)
{
  program->env->update_pwdprev(program->env, program->env->pwd);
  entry_t *home = program->env->vars->get(program->env->vars, HOME_ENV);
  program->env->vars->insert(program->env->vars, PWD_ENV, home->pair);
}

/**/

static void cd_handle_change(prgm_t *program, char *new_pwd)
{
  program->env->update_pwdprev(program->env, program->env->pwd);
  program->env->vars->insert(program->env->vars, PWD_ENV, new_pwd);
}


/*PRIVATE: ENV HELPERS */

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

/**/

static bool handle_flags(prgm_t *program, node_t *root, envflag_t flag)
{

  /* env with no operands */
  if(!root && flag == INIT)
  {
    program->env->print(program->env, false);
    return true;
  }

  if(!root && flag == IGNORE)
    return true;

  if((flag == UNSET && !(handle_unset(program, root,flag))))
    return true;

  if(flag == NILL)
  {
    program->env->print_temp(program->env, flag == NILL);
    return true;
  }

  return false;
}

/**/

static node_t *handle_unset(prgm_t *program, node_t *root, envflag_t flag )
{
  if(flag == UNSET)
  {
    if(!root)
    {
      printf("env: option requires an argument -- \'u\' \n");
      return NULL;
    }

    if(!(program->env->temp_vars->destroy(program->env->temp_vars, root->value)))
    {
      printf("env: %s does not exist in the enviroment\n", root->value);
      return NULL;
    }

    /* if left is null print the modified envirioment prematurely as parent function will return */ 
    if(!root->left)
      program->env->print_temp(program->env, flag == NILL);

    return root->left;
  }

  return root;
}

/* SETENV HELPER */

static void setenv_binary_expression(prgm_t *program, node_t *root)
{

  if(!root)
    return;

  if(root->type == ASSIGN_OPERATOR)
    program->env->temp_vars->insert(program->env->vars, root->left->value, root->right->value);

  setenv_binary_expression(program, root->left);
  setenv_binary_expression(program, root->right);
}

/**/

static void setenv_unary_expression(prgm_t *program, node_t *root)
{
  while(root)
  {

    if(!root->left)
      break;

    program->env->vars->insert(program->env->vars, root->value, root->left->value);
    root = root->left->left;

  }

}

