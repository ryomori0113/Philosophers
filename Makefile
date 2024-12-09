# ************************************************************************** */
#                                                                            */
#                                                        :::      ::::::::   */
#   Makefile                                           :+:      :+:    :+:   */
#                                                    +:+ +:+         +:+     */
#   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
#                                                +#+#+#+#+#+   +#+           */
#   Created: 2024-12-09 00:33:45 by ryomori           #+#    #+#             */
#   Updated: 2024-12-09 00:33:45 by ryomori          ###   ########.fr       */
#                                                                            */
# ************************************************************************** */

NAME	= philo

SRCS	= main.c philo_dine.c philo_eating.c philo_sleeping_thinking.c philo_monitor.c reserve_philo.c utils_libft.c utils.c

OBJS	= ${SRCS:.c=.o}

CC		= cc

FLAGS	= -Wall -Wextra -Werror

RM		= rm -f

all:		${NAME}

${NAME}:	${OBJS}
			${CC} ${FLAGS} ${OBJS} -o ${NAME}

%.o: %.c
			${CC} ${FLAGS} -c $< -o $@

clean:	
			${RM} ${OBJS}
	
fclean:	clean
			${RM} ${NAME}

re:		fclean all

.PHONY:	all clean fclean re
