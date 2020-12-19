-- @author: ots

module Pprint(pprint) where
import Expression
symbol Plus = "\27[33m+\27[0m"
symbol Times = "\27[33m*\27[0m"
usymbol Minus = "\27[33m-\27[0m"


pprint :: (Show a) => Expression a -> IO  ()
pprint t = putStrLn (showPre " " " " "\9472\9472" t) where
        showPre _ _ inn (Leaf n) = inn ++ " \27[1m" ++
                        (show n) ++ "\27[0m\n"
        showPre first second inn (UnaryOperation a subexp) =
            let newf = first ++ "     "
                news = second ++ "     "
                newinn = inn ++ "\9472  " ++ (usymbol a) ++ " "
            in
                showPre newf news newinn subexp
        showPre first second inn (BinaryOperation a lt rt) =
            let newf = first ++ " \9474   "
                news = second ++ " \9474   "
                nullf = first ++ "     "
                nulls = second ++ "     "
                newinnf= first ++ " \9484\9472\9472\9472\9472"
                newinns= second ++ " \9492\9472\9472\9472\9472"
            in (showPre nullf newf newinnf rt) ++
                --newf ++ "\n" ++
                inn ++ "\9532 " ++ (symbol a) ++ "\n" ++
                --news ++ "\n" ++
                (showPre news nulls newinns lt)