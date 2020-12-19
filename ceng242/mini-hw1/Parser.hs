module Parser (
    parse
    ) where

import Data.List

import Expression
import Tokenizer

parse :: String -> ExprV
parse string =
    if null string
       then error "Empty string input."
       else let tokens = tokenize string
                postfix = shuntingYard tokens
             in fromPostfix postfix 

fromPostfix :: [Token] -> ExprV
fromPostfix tokens = 
    let (expr:rest) = foldl' buildExpr [] tokens
     in if null rest
           then expr 
           else error "Invalid expression. Remember that '-' is only unary."
  where
    buildExpr :: [ExprV] -> Token -> [ExprV]
    buildExpr stack (Id name) = (Leaf $ Variable name) : stack
    buildExpr stack (Literal str) = (Leaf $ Constant (read str :: Int)) : stack
    buildExpr (x:rest) (UnOp op) = (UnaryOperation op x) : rest
    buildExpr (x:y:rest) (BinOp op) = (BinaryOperation op y x) : rest
    buildExpr _ _ = error "Unexpected pattern in buildExpr. Maybe a missing \
                           \operand to a binary operator?"

precedence :: BinaryOperator -> Int
precedence Plus = 1
precedence Times = 2

isLeftAssoc :: BinaryOperator -> Bool
isLeftAssoc Plus = True
isLeftAssoc Times = True

shuntingYard :: [Token] -> [Token]
shuntingYard tokens = 
    let (remOpStack, tmpRevQ) = foldl' shuntingYardStep ([], []) tokens
        revQ = foldl' (\q op -> if op == Lpar
                                   then error "Mismatched parentheses in expression\
                                               \(extra left)"
                                   else op : q)
                      tmpRevQ remOpStack
     in reverse revQ
  where
    shuntingYardStep :: ([Token], [Token]) -> Token -> ([Token], [Token])
    shuntingYardStep (opStack, outQ) lit@(Literal _) = (opStack, lit : outQ)
    shuntingYardStep (opStack, outQ) idtok@(Id _) = (opStack, idtok : outQ)
    shuntingYardStep (opStack, outQ) op@(UnOp _) = (op : opStack, outQ)
    shuntingYardStep (opStack, outQ) op@(BinOp binOp) = 
        let (removed, remStack) = span binOpWhileCondition opStack -- take stuff from the opStack
            newQ = pushToQ removed outQ
         in (op : remStack, newQ)
      where
        binOpWhileCondition :: Token -> Bool
        binOpWhileCondition Lpar = False
        binOpWhileCondition (UnOp _) = True
        binOpWhileCondition (BinOp topOp) = 
            let topOpPrec = precedence topOp
             in topOpPrec > binOpPrec || 
                topOpPrec == binOpPrec && isLeftAssoc binOp 
        binOpWhileCondition _ = False
        binOpPrec = precedence binOp 
    shuntingYardStep (opStack, outQ) Lpar = (Lpar : opStack, outQ)
    shuntingYardStep (opStack, outQ) Rpar = 
        let (removed, tentativeStack) = break (==Lpar) opStack
         in if null tentativeStack || head tentativeStack /= Lpar
               then error "Mismatched parentheses in expression (extra right)"
               else let newStack = tail tentativeStack -- remove the top left par
                        newQ = pushToQ removed outQ
                     in (newStack, newQ)

    pushToQ :: [Token] -> [Token] -> [Token]
    pushToQ elems queue = foldl' (\q elemt -> elemt : q) queue elems
