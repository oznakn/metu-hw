grammar V;

program
  : declarationList functionList
  ;


functionList
  : function*
  ;


function
  : basicType 'func' ID '(' parameterList ')' functionBody 'endfunc'
  ;


functionBody
  : declarationListInFunction statementList
  ;


declarationListInFunction
  : (declarationInFunction ';')*
  ;


declarationList
  : (declaration ';')*
  ;


declarationInFunction
  : 'var' variableList
  ;


declaration
  : 'var' variableList
  ;


parameterList
  : variableList?
  ;


variableList
  : ID ':' type (',' ID ':' type)*
  ;


type
  : basicType vectorExtension?
  ;


basicType
  : 'int'
  | 'real'
  ;


vectorExtension
  : '[' NUM ']'
  | '[' ']'
  ;


statementList
  : (statement ';')*
  ;


statement
  : returnStatement
  | printStatement
  | readStatement
  | forStatement
  | ifStatement
  | whileStatement
  | assignmentStatement
  ;


assignmentStatement
  : variable ':=' expression
  ;

variable
  : ID
  | ID '[' expression ']'
  ;


lexpression
  : expression
  | expression relop expression
  | lexpression logop lexpression
  | logop lexpression
  ;


expression
  : term
  | expression addop term
  ;


term
  : factor
  | term mulop factor
  ;


factor
  : variable
  | functionCall
  | NUM
  | '(' expression ')'
  | '-' expression
  ;

functionCall
  : ID '(' expressionList? ')'
  ;

expressionList
  : expression (',' expression)*
  ;


printStatement
  : 'print' printStatementItem (',' printStatementItem)*
  ;

printStatementItem
  : expression
  | STRINGLITERAL
  ;


readStatement
  : 'read' variable (',' variable)*
  ;


returnStatement
  : 'return' expression
  ;


forStatement
  : 'for' variable ':=' expression 'to' expression 'by' expression statementList 'endfor'
  ;


ifStatement
  : 'if' lexpression 'then' statementList 'endif'
  | 'if' lexpression 'then' statementList 'else' statementList 'endif'
  ;


whileStatement
  : 'while' lexpression 'do' statementList 'endwhile'
  ;


mulop
  : '*'
  | '/'
  | 'mod'
  | 'div'
  ;


addop
  : '+'
  | '-'
  ;


relop
  : '='
  | '<'
  | '>'
  | '<='
  | '>='
  | '<>'
  ;


logop
  : 'and'
  | 'or'
  | 'not'
  ;


STRINGLITERAL
  : '"' (~["\\\r\n] | '\\' ["\\rn])* '"'
  ;


NUM
  : [0-9]+ ('.' [0-9]+)? ([e+] [+-] [0-9]+)?
  ;



COMMENTS
  : '%' ~[\n\r]* -> skip
  ;


WHITESPACE
  : [ \t\r\n]+ -> skip
  ;


ID
  : [A-Za-z]+ [A-Za-z0-9]*;
