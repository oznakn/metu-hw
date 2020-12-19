module Mini1 (
    gridMap,
    gridMapIf,
    evalExpr,
    getVars,
    evalDeriv,
    parse -- reexported to allow use
    ) where

import Expression
import Parser

import Data.List
import Data.Maybe

-- Do not modify the module declaration and imports above!
-- Also do not change the function signatures and do not
-- remove the dummy implementations of the functions if
-- you want your code to compile.

-- Feel free to import anything else here

-- gridMap (20 points), map function over grid elements
gridMap :: (a -> b) -> [[a]] -> [[b]]
gridMap fn items =
    map (\item -> map fn item) items


-- gridMapIf (20 points), map functions over grid elements
-- that satisfy the predicate provided as the first arg.
gridMapIf :: (a -> Bool) -> (a -> a) -> [[a]] -> [[a]]
gridMapIf select_fn map_fn items =
    map (\item -> map (\a -> if select_fn a then map_fn a else a) item) items


-- evalExpr (20 points), evaluate the expression by
-- substituting (var, value) pairs given in the first arg.
evalExpr :: [(String, Int)] -> ExprV -> Int
evalExpr vars value =
    let result = eval vars value
    in case result of
        Leaf (Constant c) -> c
        otherwise -> 0

    where
        eval vars (Leaf (Variable s)) =
            let maybe_var = lookup s vars
            in case maybe_var of
                Just c -> (Leaf (Constant c))
                Nothing -> (Leaf (Constant 0))


        eval vars (UnaryOperation Minus (Leaf (Constant c))) =
            (Leaf (Constant (-c)))

        eval vars (BinaryOperation Plus (Leaf (Constant c1)) (Leaf (Constant c2))) =
            (Leaf (Constant (c1 + c2)))

        eval vars (BinaryOperation Times (Leaf (Constant c1)) (Leaf (Constant c2))) =
            (Leaf (Constant (c1 * c2)))

        eval vars (UnaryOperation operation e) =
            eval vars (UnaryOperation operation (eval vars e))

        eval vars (BinaryOperation operation e1 e2) =
            eval vars (BinaryOperation operation (eval vars e1) (eval vars e2))

        eval vars value = value


-- getVars (20 points), return the variables contained
-- in the expression in a list (ordered, no duplicates)
getVars :: ExprV -> [String]
getVars value =
    helper value

    where
        helper (Leaf (Variable s)) =
            [s]

        helper (UnaryOperation operation e) =
            helper e

        helper (BinaryOperation operation e1 e2) =
            foldl (\results i -> insert_set results i) [] ((helper e1) ++ (helper e2))

        helper value =
            []


        insert_set items a =
            let (first_span, second_span) = span (< a) items
            in if length second_span == 0
                then first_span ++ [a]
                else if head second_span == a
                    then items
                    else first_span ++ [a] ++ second_span


-- evalDeriv (20 points), evaluate the first derivative
-- with respect to the variable given in the second
-- arg. using (var, value) pairs given in the first arg.
evalDeriv :: [(String, Int)] -> String -> ExprV -> Int
evalDeriv vars deriv_s value =
    let maybe_var = lookup deriv_s vars
        var = case maybe_var of
            Just v -> v
            Nothing -> 0
        calcs = deriv (calculate vars deriv_s value)

    in foldl (\result (c, e) -> result + (c * (var^e))) 0 calcs

    where
        calculate vars deriv_s (Leaf (Constant c)) =
            [(c, 0)]

        calculate vars deriv_s (Leaf (Variable s)) =
            let maybe_var = lookup s vars
            in if s == deriv_s
                then [(1, 1)]
                else case maybe_var of
                    Just v -> [(v, 0)]
                    Nothing -> [(0, 0)]

        calculate vars deriv_s (UnaryOperation Minus (Leaf (Constant c))) =
            [(-c, 0)]

        calculate vars deriv_s (UnaryOperation Minus (Leaf (Variable s))) =
            [(-1, 1)]

        calculate vars deriv_s (UnaryOperation Minus e) =
            let calc = calculate vars deriv_s e
            in map (\(c, e) -> (-c, e)) calc

        calculate vars deriv_s (BinaryOperation Plus e1 e2) =
            (calculate vars deriv_s e1) ++ (calculate vars deriv_s e2)

        calculate vars deriv_s (BinaryOperation Times e1 e2) =
            let calc1 = calculate vars deriv_s e1
                calc2 = calculate vars deriv_s e2
            in foldl (\results (a1, a2) -> results ++ (map (\(b1, b2) -> (a1 * b1, a2 + b2)) calc2)) [] calc1


        deriv calcs =
            let deriv_calcs = map (\(c, e) -> if e == 0 then (0, 0) else (c * e, e-1)) calcs
            in filter (\(c, e) -> not (c == 0 && e == 0)) deriv_calcs

-- Looks like that's all!
