#my_zsh

my_zsh is a naive implementation of the Z-shell. 


## build

Build:

    make

Clean

    make clean


Run

    ./bin/my_zsh

Or

    make run

## Run

The following commands are built-in to the shell: `cd`, `which`, `env`, `setenv`, `unsetenv`, `pwd`, `exit` and `echo`. All binaries in your `$PATH` are also available for execution. 

The shell allows for setting environment varibles using literals such as `variable=value` and the user can create as many :

        user - /home/dev %> USER=khalil PWD=/home EDITOR=vim 
        khalil - /home   %> echo $EDITOR
        vim

String literal and interpolation is supported when using the `echo` command like so:

        user - /home/dev %>name=John age=41 city=Lagos
        user - /home/dev %>echo "My name is $name and I am from $city. I have $age years old."
        My name is John and I am from Lagos. I have 41 years old.

String literals and interpolation, however, are not available for assignment like so `var="$PWD"`.

### env

Synopsis:

        env [options [key=pair]... [command [args]


When a command is provided, `env` runs the specified program with the current shell's environment.  
When not provided, `env` prints the environment to stdout.

The environment can be modified by passing `key=pair`. `env` accepts as many `key=pair` as necessary and will update a variable if already existing in the environment.
You can test this command using `./bin/printenvs`. This is a simple program that prints an incoming environment for testing purposes.

        env PWD=/home my_var=something ./bin/printenvs

Additionally, the following options are also available:

         -i, --ignore-enviroment

`env` produces an empty environment.

        -u [key]...

Unsets the avaiable with the provided `key`. At least one `key` must be provided.

        -0, --null

Prints the environment without the line break (`\n`). This options will not execute the `command`.

        -C [path]

This options runs the `command` from the `path` directory as opposed to shells `$PWD` directory. Example:

        user - /home/dev %> ls  
        project1   clang   rust     ruby
        user - /home/dev %>env -C /home ls  
        photos   dev  dotfiles   downloads   desktop

### setenv

With this command you can set variables in the shells environment. `setenv` accepts both `key=pair` or `key pair` syntax.

        setenv PWD=/home USER=Joe PATH=/usr/bin

The below syntax is also allowed:

        setenv PWD /home USER Joe PATH /usr/bin


### unsetenv

`unsetenv` as the name suggests, unsets a variable of a given `key`.


         unsetenv USER TERMINAL

