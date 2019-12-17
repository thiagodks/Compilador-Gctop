#ifndef COLORS_H
#define COLORS_H

#define foreground(color) FORE##color
#define background(color) BACK##color
#define style(style_) style_

/** Foreground Colors **/
#define FOREBLACK "\033[30m"
#define FORERED  "\033[31m"
#define FOREGREEN  "\033[32m"  
#define FOREYELLOW  "\033[33m"  
#define FOREBLUE  "\033[34m"  
#define FOREMARGENTA  "\033[35m"  
#define FORECYAN  "\033[36m"  
#define FOREWHITE  "\033[37m"  
#define FORENORMAL_COLOR "\033[39m"

/** Background Colors **/
#define BACKGREY  "\033[35m"  
#define BACKBLACK  "\033[40m"  
#define BACKRED  "\033[41m"  
#define BACKGREEN  "\033[42m"  
#define BACKYELLOW  "\033[43m"  
#define BACKBLUE  "\033[44m"  
#define BACKMAGENTA  "\033[45m"  
#define BACKCYAN  "\033[46m"  
#define BACKWHITE " \033[47m" 
#define BACKNORMAL "\033[49m"

/** Style **/
#define BRIGHT  "\033[1m" 
#define DIM  "\033[2m" 
#define NORMAL  "\033[22m" 
#define RESETALL  "\033[0m" 
#define UNDERLINE  "\033[4m" 
#define BLINKSLOW  "\033[5m" 
#define BLINKRAPID  "\033[6m" 
#define ITALIC  "\033[3m" 
#define NEGATIVE  "\033[7m"
#define BOLD_ON "\x1b[1m"
#define BOLD_OFF "\x1b[0m"
#endif /* COLORS_H */
