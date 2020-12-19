module Tokenizer (
    Token(..), 
    tokenize
    ) where

import qualified Data.Char as C

import Expression

-- a very simple tokenizer/lexer for the input

-- what's a tokenizer? as some of you might be familiar with,
-- compilation is performed in multiple steps. the first step,
-- although it is sometimes also considered together with the
-- second step (parsing) since they are tightly coupled, is tokenization. 
-- in this part, groups of characters are bunched up together to 
-- create single entities, which abstracts over character sequences 
-- quite nicely. example:
-- input: myvar+x->y
-- sequence: 'm','y','v','a','r','+','x','-','>','y'
-- tokenized: id("myvar"),plus,id("x"),rightarrow,id("y")
-- ... which representation do you think would be easier to work with?

-- feel free to modify this file but remember that it will not be used in grading!

data Token = Id String
           | Literal String
           | UnOp UnaryOperator
           | BinOp BinaryOperator
           | Lpar
           | Rpar
           deriving (Eq, Show)

splitBy :: (Char -> Bool) -> String -> [String]
splitBy p string = foldr splitfold [] string
  where 
    splitfold :: Char -> [String] -> [String]
    splitfold c [] = [[c]]
    splitfold c split@(acc@(firstChar:_):splitRest) = 
            if p firstChar || p c -- previous or current char is a split symbol
               then [c] : split
               else (c : acc) : splitRest
    splitfold _ _ = error "Unexpected pattern with empty string in split"

isSymbol :: Char -> Bool
isSymbol c = c `elem` ['+', '-', '*', '(', ')']

toToken :: String -> Token
toToken [] = error "Empty token"
toToken "+" = BinOp Plus 
toToken "-" = UnOp Minus 
toToken "*" = BinOp Times 
toToken "(" = Lpar
toToken ")" = Rpar
toToken string@(first:_) = 
    if C.isDigit first
       then extractInt string
       else extractId string
  where
    extractInt :: String -> Token
    extractInt = extract C.isDigit Literal "digits" "literal"

    extractId :: String -> Token
    extractId = extract C.isAlphaNum Id "alphanumeric characters" "identifier"

    extract :: (Char -> Bool) -> (String -> Token) -> String -> String -> String 
            -> Token
    extract predicate constructor expected target str =
        let (_, rest) = span predicate str
         in if null rest
               then constructor str
               else error $ "Expected " ++ expected ++ " while tokenizing "
                            ++ target ++ ", found: '" ++ [head rest] ++ "'"

tokenize :: String -> [Token]
tokenize = map toToken . splitBy isSymbol . filter (not . C.isSpace)
