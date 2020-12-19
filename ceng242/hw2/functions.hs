-- @author: ucanyiit

import HW2
import Expression

main = do
    print $ foldAndPropagateConstants [("x", parse "1+2+3")] == [("x",Leaf (Constant 6))]
    print $ foldAndPropagateConstants [("x", parse "1+2+3"), ("y", parse "5*x + 7")] == [("x",Leaf (Constant 6)),("y",Leaf (Constant 37))]
    print $ foldAndPropagateConstants [("x", parse "1+2+3"), ("y", parse "5*x + 7"), ("z", parse "y+var")] == [("x",Leaf (Constant 6)),("y",Leaf (Constant 37)),("z",BinaryOperation Plus (Leaf(Constant 37)) (Leaf (Variable "var")))]
    print $ foldAndPropagateConstants [("a", parse "x+3*5"), ("b", parse "(7+5)*a")] == [("a",BinaryOperation Plus (Leaf (Variable "x")) (Leaf (Constant 15))),("b",BinaryOperation Times (Leaf (Constant 12)) (Leaf (Variable "a")))]
    print $ foldAndPropagateConstants [("a", parse "1+2+x")] == [("a",BinaryOperation Plus (Leaf (Constant 3)) (Leaf (Variable "x")))]
    print $ foldAndPropagateConstants [("a", parse "1+x+2")] == [("a",BinaryOperation Plus (BinaryOperation Plus (Leaf (Constant 1)) (Leaf (Variable "x"))) (Leaf (Constant 2)))]
    print $ foldAndPropagateConstants [("a", parse "x+1+2")] == [("a",BinaryOperation Plus (BinaryOperation Plus (Leaf (Variable "x")) (Leaf (Constant 1))) (Leaf (Constant 2)))]

    print $ foldAndPropagateConstants [("y",parse "- -x")] == [("y",UnaryOperation Minus (UnaryOperation Minus (Leaf (Variable "x"))))]
    print $ foldAndPropagateConstants [("y",parse "-5")] == [("y",Leaf (Constant (-5)))]
    print $ foldAndPropagateConstants [("a", parse "1+x+2"), ("x",parse "3")] == [("a",BinaryOperation Plus (BinaryOperation Plus (Leaf (Constant 1)) (Leaf (Variable "x"))) (Leaf (Constant 2))),("x",Leaf (Constant 3))]

    print $ (assignCommonSubexprs $ parse "1+2") == ([],BinaryOperation Plus (Leaf (Constant 1)) (Leaf (Constant 2)))
    print $ (assignCommonSubexprs $ parse "(x+1)+(x+1)") == ([("$0",BinaryOperation Plus (Leaf (Variable "x")) (Leaf (Constant 1)))],BinaryOperation Plus (Leaf (Variable "$0")) (Leaf (Variable "$0")))
    print $ (assignCommonSubexprs $ parse "(-x+1)+(-x+1)") == ([("$0",UnaryOperation Minus (Leaf (Variable "x"))),("$1",BinaryOperation Plus (Leaf (Variable "$0")) (Leaf (Constant 1)))],BinaryOperation Plus (Leaf (Variable "$1")) (Leaf (Variable "$1")))
    print $ (assignCommonSubexprs $ parse "(x+1+2)+(x+1+2)") == ([("$0",BinaryOperation Plus (Leaf (Variable "x")) (Leaf (Constant 1))),("$1",BinaryOperation Plus (Leaf (Variable "$0")) (Leaf (Constant 2)))],BinaryOperation Plus(Leaf (Variable "$1")) (Leaf (Variable "$1")))
    print $ (assignCommonSubexprs $ parse "(x+1+2)+(x+1+2)+(x+1)") == ([("$0",BinaryOperation Plus (Leaf (Variable "x")) (Leaf (Constant 1))),("$1",BinaryOperation Plus (Leaf (Variable "$0")) (Leaf (Constant 2)))],BinaryOperation Plus(BinaryOperation Plus (Leaf (Variable "$1")) (Leaf (Variable "$1"))) (Leaf (Variable "$0")))

    print $ (reducePoly $ parse "2+3") == Leaf (Constant 5)
    print $ (reducePoly $ parse "2+x+x+x+-x") == BinaryOperation Plus (Leaf (Constant 2)) (BinaryOperation Times (Leaf (Constant 2)) (Leaf (Variable "x")))
    print $ (reducePoly $ parse "1+a*(a+1)") == BinaryOperation Plus (BinaryOperation Plus (Leaf (Constant 1)) (Leaf (Variable "a"))) (BinaryOperation Times (Leaf (Variable "a")) (Leaf (Variable "a")))
    print $ (reducePoly $ parse "(2*x+2)*(2*x+2)") == BinaryOperation Plus (BinaryOperation Plus (Leaf (Constant 4)) (BinaryOperation Times (Leaf (Constant 8)) (Leaf (Variable "x")))) (BinaryOperation Times (BinaryOperation Times (Leaf (Constant 4)) (Leaf (Variable "x"))) (Leaf (Variable "x")))

    print $ (reducePoly $ parse "-2*x*x") == BinaryOperation Times (BinaryOperation Times (Leaf (Constant (-2))) (Leaf (Variable "x"))) (Leaf (Variable "x"))
    print $ (reducePoly $ parse "-x*x*x") == BinaryOperation Times (BinaryOperation Times (UnaryOperation Minus (Leaf (Variable "x"))) (Leaf (Variable "x"))) (Leaf (Variable "x"))
    print $ (reducePoly $ parse "-1 + -x*x*x") == BinaryOperation Plus (Leaf (Constant (-1))) (BinaryOperation Times (BinaryOperation Times (UnaryOperation Minus (Leaf (Variable "x"))) (Leaf (Variable "x"))) (Leaf (Variable "x")))
    print $ (reducePoly $ parse "a*a*a*(a+a+-a)") == BinaryOperation Times (BinaryOperation Times (BinaryOperation Times (Leaf (Variable "a")) (Leaf (Variable "a"))) (Leaf (Variable "a"))) (Leaf (Variable "a"))
    print $ (reducePoly $ parse "-((-c+1)*(-c*-c)*(3*c+-7*c+5))") == BinaryOperation Plus (BinaryOperation Plus (BinaryOperation Times (BinaryOperation Times (Leaf (Constant (-5))) (Leaf (Variable "c"))) (Leaf (Variable "c"))) (BinaryOperation Times (BinaryOperation Times (BinaryOperation Times (Leaf (Constant 9)) (Leaf (Variable "c"))) (Leaf (Variable "c"))) (Leaf (Variable "c")))) (BinaryOperation Times (BinaryOperation Times (BinaryOperation Times (BinaryOperation Times (Leaf (Constant (-4))) (Leaf (Variable "c"))) (Leaf (Variable "c"))) (Leaf (Variable "c"))) (Leaf (Variable "c")))
    print $ (reducePoly $ parse "1*x*x+(-2*x)+5+-6+(x*-x*x)") == BinaryOperation Plus (BinaryOperation Plus (BinaryOperation Plus (Leaf (Constant (-1))) (BinaryOperation Times (Leaf (Constant (-2))) (Leaf (Variable "x")))) (BinaryOperation Times (Leaf (Variable "x")) (Leaf (Variable "x")))) (BinaryOperation Times (BinaryOperation Times (UnaryOperation Minus (Leaf (Variable "x"))) (Leaf (Variable "x"))) (Leaf (Variable "x")))
    print $ (reducePoly $ parse "(1+-var)*(1+-var+var*var+-var*var*var)") == BinaryOperation Plus (BinaryOperation Plus (BinaryOperation Plus (BinaryOperation Plus (Leaf (Constant 1)) (BinaryOperation Times (Leaf (Constant (-2))) (Leaf (Variable "var")))) (BinaryOperation Times (BinaryOperation Times (Leaf (Constant 2)) (Leaf (Variable "var"))) (Leaf (Variable "var")))) (BinaryOperation Times (BinaryOperation Times (BinaryOperation Times (Leaf (Constant (-2))) (Leaf (Variable "var"))) (Leaf (Variable "var"))) (Leaf (Variable "var")))) (BinaryOperation Times (BinaryOperation Times (BinaryOperation Times (Leaf (Variable "var")) (Leaf (Variable "var"))) (Leaf (Variable "var"))) (Leaf (Variable "var")))
