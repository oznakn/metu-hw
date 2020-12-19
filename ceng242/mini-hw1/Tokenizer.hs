module Tokenizer (
    Token(..), 
    tokenize
    ) where

import qualified Data.Char as C

import Expression

-- a very simple tokenizer/lexer for the input

-- What's a tokenizer? As some of you might be familiar with,
-- compilation is performed in multiple steps. The first step,
-- although it is sometimes also considered together with the
-- second step (parsing) since they are tightly coupled, is tokenization. 
-- In this part, groups of characters are bunched up together to 
-- create single entities, which abstracts over character sequences 
-- quite nicely. Example:
-- Input: myvar+x->y
-- Sequence: 'm','y','v','a','r','+','x','-','>','y'
-- Tokenized: Id("myvar"),Plus,Id("x"),Rightarrow,Id("y")
-- ... which representation do you think would be easier to work with?

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
