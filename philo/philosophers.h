/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/25 09:27:13 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/07 10:08:24 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>

typedef struct s_arguments
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;
	long			time_at_start;

	pthread_mutex_t	forks[200];
	int				*fork_available;

	pthread_mutex_t	start_time_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	sim_lock;
	pthread_mutex_t	check_eaten_lock;
	int				*philos_eaten;
	int				can_print;

	int				stop_sim;
}	t_arguments;

typedef struct s_philo
{
	int			philosopher_id;

	int			times_eaten;
	long		start_time;

	int			left_fork;
	int			right_fork;

	t_arguments	*arguments;
}	t_philo;

int		initialize_args(t_arguments *arguments, char **argv);
int		start_simulation(t_arguments *arguments);
void	*ft_philosopher(void *void_philo);
void	try_to_eat(t_philo *s_philo);
void	put_down_forks(t_philo *s_philo);
int		pick_up_forks_right(t_philo *s_philo);
int		pick_up_forks_left(t_philo *s_philo);
void	sleeping(t_philo *s_philo);
void	thinking(t_philo *s_philo);

long	get_curr_time(void);
int		is_dead(t_philo *s_philo);
int		stop_sim(t_philo *s_philo);
void	printing(char *str, t_philo *s_philo);
void	*check_times_eaten(void *void_philo);
void	*check_deaths(void *void_philo);

int		ft_error(char *str);

int		ft_strlen(char *str);
int		ft_atoi(const char *str);
void	*ft_calloc(int count, int size);

#endif