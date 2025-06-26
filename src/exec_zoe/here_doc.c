/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:37:42 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/14 15:37:52 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void heredoc_sigint_handler(int sig)
{
    (void)sig;
    g_sig = SIGINT;
    
    // Interrompre readline
    rl_done = 1;
    
    // Afficher une nouvelle ligne
    write(STDOUT_FILENO, "\n", 1);
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;
    
    // Configurer le gestionnaire SIGINT pour heredoc
    sa.sa_handler = heredoc_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Redémarrer les appels système interrompus
    sigaction(SIGINT, &sa, NULL);
    
    // Dire à readline de ne pas capturer SIGINT
    rl_catch_signals = 0;
}

// Version alternative avec callback et sigaction
int check_signal_callback(void)
{
    if (g_sig == SIGINT)
    {
        rl_done = 1;
        return 1;
    }
    return 0;
}

static void write_in_heredoc(int fd, char *delim, t_shell_data *data)
{
    char *buf;
    struct sigaction sa, old_sa;
    
    sigaction(SIGINT, NULL, &old_sa);
    sa.sa_handler = heredoc_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    rl_event_hook = check_signal_callback;
    rl_catch_signals = 0;
    
    while(1)
    {
        g_sig = 0;
        rl_done = 0;
        buf = readline("> ");
		if (!buf)
        {
            ft_putstr_fd("bash: warning: here-document delimited by EOF\n", 2);
            break;
        }
        if (g_sig == SIGINT)
        {
            data->last_exit_status = 130;
            break;
        }

        if (!ft_strcmp(buf, delim))
        {
            free(buf);
            break;
        }
        buf = expand_variables(buf, data);
        ft_putstr_fd(buf, fd);
        ft_putstr_fd("\n", fd);
        free(buf);
    }
    sigaction(SIGINT, &old_sa, NULL);
    rl_event_hook = NULL;
    rl_catch_signals = 1;
    close(fd);
}

int heredoc(char *delim, t_shell_data *data)
{
    int fd;
    
    // Correction : utiliser les flags correctement combinés avec |
    fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)  // Correction : vérifier fd == -1, pas !fd
        return (-1);
    
    write_in_heredoc(fd, delim, data);
    close(fd);  // Fermer le fd d'écriture
    
    // Rouvrir le fichier en lecture
    fd = open("heredoc.tmp", O_RDONLY);
    if (fd == -1)
    {
        unlink("heredoc.tmp");  // Nettoyer en cas d'erreur
        return (-1);
    }
    
    unlink("heredoc.tmp");  // Supprimer le fichier (il reste accessible via fd)
    return (fd);
}


