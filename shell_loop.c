#include "shell.h"

/**
 * shell_loop - Main shell loop
 * @info: Pointer to the parameter and return info structure
 * @av: Argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(info_t *info, char **av)
{
    ssize_t read_status = 0;
    int builtin_result = 0;

    while (read_status != -1 && builtin_result != -2)
    {
        clear_info(info);
        if (is_interactive(info))
            print_prompt("$ ");
        flush_buffer(BUF_FLUSH);
        read_status = get_input(info);
        if (read_status != -1)
        {
            set_info(info, av);
            builtin_result = find_builtin(info);
            if (builtin_result == -1)
                find_command(info);
        }
        else if (is_interactive(info))
            _putchar('\n');
        free_info(info, 0);
    }
    write_history(info);
    free_info(info, 1);
    if (!is_interactive(info) && info->status)
        exit(info->status);
    if (builtin_result == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return (builtin_result);
}

/**
 * find_builtin - Find a built-in command
 * @info: Pointer to the parameter and return info structure
 *
 * Return: -1 if built-in not found,
 *         0 if built-in executed successfully,
 *         1 if built-in found but not successful,
 *         2 if built-in signals exit()
 */
int find_builtin(info_t *info)
{
    int i, built_in_ret = -1;
    builtin_table builtintbl[] = {
        {"exit", handle_exit},
        {"env", handle_env},
        {"help", handle_help},
        {"history", handle_history},
        {"setenv", handle_setenv},
        {"unsetenv", handle_unsetenv},
        {"cd", handle_cd},
        {"alias", handle_alias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].name; i++)
        if (_strcmp(info->argv[0], builtintbl[i].name) == 0)
        {
            info->line_count++;
            built_in_ret = builtintbl[i].func(info);
            break;
        }
    return (built_in_ret);
}

/**
 * find_command - Find an external command in PATH
 * @info: Pointer to the parameter and return info structure
 *
 * Return: void
 */
void find_command(info_t *info)
{
    char *path = NULL;
    int i, arg_count = 0;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0; info->argv[i]; i++)
        if (!is_delim(info->argv[i], " \t\n"))
            arg_count++;
    if (!arg_count)
        return;

    path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        fork_cmd(info);
    }
    else
    {
        if ((is_interactive(info) || _getenv(info, "PATH=")
                    || info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
            fork_cmd(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            print_error(info, "not found\n");
        }
    }
}

/**
 * fork_cmd - Fork a child process to execute a command
 * @info: Pointer to the parameter and return info structure
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, get_environ(info)) == -1)
        {
            free_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                print_error(info, "Permission denied\n");
        }
    }
}

