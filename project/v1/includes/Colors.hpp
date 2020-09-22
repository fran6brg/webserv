#ifndef COLORS_HPP
# define COLORS_HPP

// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal

# define BLACK_C		"\033[1;30m"
# define RED_C			"\033[1;31m"
# define GREEN_C		"\033[1;32m"
# define YELLOW_C		"\033[1;33m"
# define BLUE_C			"\033[1;34m"
# define MAGENTA_C		"\033[1;35m"
# define CYAN_C			"\033[1;36m"
# define WHITE_C		"\033[1;37m"

# define BLACK_B		"\033[1;40m"
# define RED_B			"\033[1;41m"
# define GREEN_B		"\033[1;42m"
# define YELLOW_B		"\033[1;43m"
# define BLUE_B			"\033[1;44m"
# define MAGENTA_B		"\033[1;45m"
# define CYAN_B			"\033[1;46m"
# define WHITE_B		"\033[1;47m"

# define RESET			"\033[1;0m"  // (everything back to normal)
# define BOLD       	"\033[1;1m"  // (often a brighter shade of the same colour)
# define BOLD_OFF		"\033[1;21m"
# define UNDERLINE		"\033[1;4m"
# define UNDERLINE_OFF	"\033[1;24m"
# define INVERSE		"\033[1;7m"  // (swap foreground and background colours)
# define INVERSE_OFF	"\033[1;27m"

#endif