grammar Regex;

//ALTERNATION
r1 : r1 '|' r2 | r2 ;

//CONCATENATION
r2 : r2 r3 | r3 ;

//sub-expresii
r3 : r3 (star | plus | maybe | range_0) | group | any_0 | set_0 | SYMBOL;


star : '*' ;
plus : '+' ;
maybe : '?' ;

group : '(' r1 ')' ;
any_0 : '.' ;

//reguli pentru set
set_0 : '[' set_1 ']' ;
set_1 : set_2 | set_1 set_2 ;
set_2 : SYMBOL | set_3 ;
set_3 : symbol_0 '-' symbol_1 ;
symbol_0 : SYMBOL ;
symbol_1 : SYMBOL ;

//reguli pentru range
range_0 : fixed | min_0 | max_0 | interval ;
fixed : R_FIXED ;
min_0 : R_MIN ;
max_0 : R_MAX ;
interval : R_INTERVAL ;

//tokeni pentru simboluri si pentru fiecare tip de range
fragment NUMBER : [0-9]+ ;
SYMBOL : [a-zA-Z0-9] ;

R_FIXED : '{' NUMBER '}' ;
R_MIN : '{' NUMBER ',' '}' ;
R_MAX : '{' ',' NUMBER '}' ;
R_INTERVAL : '{' NUMBER ',' NUMBER '}' ;

WS : [ \t\r\n]+ -> skip ;