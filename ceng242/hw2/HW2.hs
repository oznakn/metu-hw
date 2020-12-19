module HW2 (
    parse, -- reexport for easy terminal use
    foldAndPropagateConstants,
    assignCommonSubexprs,
    reducePoly
) where

import Data.List
import Data.Maybe

import Expression
import Parser

-- Do not change the module definition and imports above! Feel free
-- to modify the parser, but please remember that it is just a helper
-- module and is not related to your grade. You should only submit
-- this file. Feel free to import other modules, especially Data.List!

foldAndPropagateConstants :: [(String, ExprV)] -> [(String, ExprV)]
foldAndPropagateConstants variables =
    foldl (\result (variable, value) -> result ++ [(variable, helper result value)]) [] variables

    where
        findVariableWithName s [] = Nothing
        findVariableWithName s variables =
            if length filtered == 0
                then Nothing
                else Just $ snd $ head filtered

            where
                filtered = filter (\(name, _) -> name == s) variables



        executor variables (Leaf (Variable s)) =
            let maybe_variable = findVariableWithName s variables

            in case maybe_variable of
                Just variable ->
                    case variable of
                        Leaf (Constant c) -> variable
                        otherwise -> (Leaf (Variable s))
                Nothing ->  (Leaf (Variable s))

        executor variables (UnaryOperation Minus (Leaf (Constant c))) =
            Leaf (Constant (-c))

        executor variables (BinaryOperation Plus (Leaf (Constant c1)) (Leaf (Constant c2))) =
            Leaf (Constant (c1 + c2))

        executor variables (BinaryOperation Times (Leaf (Constant c1)) (Leaf (Constant c2))) =
            Leaf (Constant (c1 * c2))

        executor variables e = e



        helper variables (UnaryOperation o e) =
            executor variables $ UnaryOperation o (helper variables e)

        helper variables (BinaryOperation o e1 e2) =
            executor variables $ BinaryOperation o (helper variables e1) (helper variables e2)

        helper variables e = executor variables e


assignCommonSubexprs :: ExprV -> ([(String, ExprV)], ExprV)
assignCommonSubexprs v =
    helper [] v

    where
        findVariableWithExprV e [] = Nothing
        findVariableWithExprV e variables =
            if length filtered == 0
                then Nothing
                else Just $ fst $ head filtered

            where
                filtered = filter (\(_, e1) -> e == e1) variables



        helper variables value =
            let dup_values = fst (findPossibleDuplicates [] [] value)
                new_variable_names = map (\i -> "$" ++ show i) [(length variables)..((length variables) + (length dup_values) - 1)]
                new_variables = variables ++ (zip new_variable_names dup_values)
                result = changeDups dup_values new_variables value

            in if length dup_values == 0
                then (new_variables, result)
                else helper new_variables result

            where
                findPossibleDuplicates dups results (UnaryOperation o (Leaf l)) =
                    let node = UnaryOperation o (Leaf l)
                    in if notElem node results
                        then (dups, results ++ [node])
                        else if notElem node dups
                            then (dups ++ [node], results)
                            else (dups, results)


                findPossibleDuplicates dups results (UnaryOperation o e) = findPossibleDuplicates dups results e

                findPossibleDuplicates dups results (BinaryOperation o (Leaf l1) (Leaf l2)) =
                    let node = BinaryOperation o (Leaf l1) (Leaf l2)
                    in if notElem node results
                        then (dups, results ++ [node])
                        else if notElem node dups
                            then (dups ++ [node], results)
                            else (dups, results)

                findPossibleDuplicates dups results (BinaryOperation o e1 e2) =
                    let (new_dups, new_results) = findPossibleDuplicates dups results e1
                    in findPossibleDuplicates new_dups new_results e2

                findPossibleDuplicates dups results e = (dups, results)



                changeDups dups variables (UnaryOperation o (Leaf l)) =
                    let node = (UnaryOperation o (Leaf l))
                        maybe_s = findVariableWithExprV node variables
                    in case maybe_s of
                        Just s -> (Leaf (Variable s))
                        Nothing -> node

                changeDups dups variables (UnaryOperation o e) =
                    UnaryOperation o (changeDups dups variables e)

                changeDups dups variables (BinaryOperation o (Leaf l1) (Leaf l2)) =
                    let node = (BinaryOperation o (Leaf l1) (Leaf l2))
                        maybe_s = findVariableWithExprV node variables
                    in case maybe_s of
                        Just s -> (Leaf (Variable s))
                        Nothing -> node

                changeDups dups variables (BinaryOperation o e1 e2) =
                    BinaryOperation o (changeDups dups variables e1) (changeDups dups variables e2)

                changeDups dups variables e = e


reducePoly :: ExprV -> ExprV
reducePoly value =
    let calcs = sortOn snd3 $ calculate value

        uniq_calcs = nubBy (\(_, e1, _) (_, e2, _) -> e1 == e2) calcs

        results = map (\(_, item, s) -> (sum $ map fst3 (filter (\(_, a, _) -> a == item) calcs), item, s)) uniq_calcs
        filtered_results = filter (\(c, _, _) -> c /= 0) results

        binary_results = map result_to_binary filtered_results

    in
        foldl (\left right -> BinaryOperation Plus left right) (head binary_results) (tail binary_results)

    where
        fst3 (a, _, _) = a
        snd3 (_, a, _) = a



        result_to_binary (c, 0, _) = Leaf (Constant c)

        result_to_binary (1, e, s) =
            foldl (\left _ -> BinaryOperation Times left (Leaf (Variable s)))
                (Leaf (Variable s)) [1..(e-1)]

        result_to_binary (-1, e, s) =
            foldl (\left _ -> BinaryOperation Times left (Leaf (Variable s)))
                (UnaryOperation Minus (Leaf (Variable s))) [1..(e-1)]

        result_to_binary (c, e, s) =
            foldl (\left _ -> BinaryOperation Times left (Leaf (Variable s)))
                (Leaf (Constant c)) [0..(e-1)]



        calculate (Leaf (Constant c)) =
            [(c, 0, "")]

        calculate (Leaf (Variable s)) =
            [(1, 1, s)]

        calculate (UnaryOperation Minus (Leaf (Constant c))) =
            [(-c, 0, "")]

        calculate (UnaryOperation Minus (Leaf (Variable s))) =
            [(-1, 1, s)]

        calculate (UnaryOperation Minus e) =
            let calc = calculate e
            in map (\(c, e, s) -> (-c, e, s)) calc

        calculate (BinaryOperation Plus e1 e2) =
            (calculate e1) ++ (calculate e2)

        calculate (BinaryOperation Times e1 e2) =
            let calc1 = calculate e1
                calc2 = calculate e2

            in
                foldl (\results (item1_1, item1_2, s1) -> (

                    results ++ (map (\(item2_1, item2_2, s2) -> (
                        (item1_1 * item2_1, item1_2 + item2_2, if length s1 == 0 then s2 else s1)
                    )) calc2)

                )) [] calc1

        -- calculate e = [] -- redundant



-- an extra dummy variable, so as to not crash the GUI
notImpl :: ExprV
notImpl = Leaf $ Variable "Not Implemented"

