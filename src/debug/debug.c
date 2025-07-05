/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:33:59 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:34:00 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// static void	print_token_list(t_ta *lx)
// {
//     int i;

//     printf("---- Lexer Token List ----\n");
//     for (i = 0; i < lx->t_tot; i++)
//     {
//         printf("token[%d]: [%s] (quoted: %d)\n", i,
//  lx->tokens[i], lx->quoted[i]);
//     }
//     printf("--------------------------\n");
// }

// static void	print_expand_debug(const char *input, const char *expanded)
// {
//     printf("---- Expander Debug ----\n");
//     printf("Input   : [%s]\n", input);
//     printf("Expanded: [%s]\n", expanded);
//     printf("------------------------\n");
// }

// static void print_cmd_args(t_cmd *cmd)
// {
//     int i;
//     t_cmd *cur = cmd;
//     int cmd_num = 1;
//     t_redirect *redir;

//     while (cur)
//     {
//         printf("---- Command %d ----\n", cmd_num++);
//         if (cur->c_name)
//             printf("c_name: [%s] (quoted: %d)\n", cur->c_name, cur->quoted);
//         else
//             printf("c_name: [NULL]\n");
//         for (i = 0; i < cur->arg_count; i++)
//             printf("arg[%d]: [%s] (quoted: %d)\n", i,
//  cur->args[i], cur->arg_quoted[i]);
//         redir = cur->redirects;
//         while (redir)
//         {
//             const char *type_str = "UNKNOWN";
//             if (redir->type == REDIR_IN)
//                 type_str = "<";
//             else if (redir->type == REDIR_OUT)
//                 type_str = ">";
//             else if (redir->type == APPEND)
//                 type_str = ">>";
//             else if (redir->type == HERE_DOC)
//                 type_str = "<<";
// //             printf("redir: [%s] file: [%s]\n",
//  type_str, redir->file);
//             redir = redir->next;
//         }
//         printf("-------------------\n");
//         cur = cur->next;
//     }
// }
