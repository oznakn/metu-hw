import HW1

main = do

    print $ form [1,2,3] (1, 3) == [[1,2,3]]
    print $ form [1,2,3] (3, 1) == [[1],[2],[3]]
    print $ form [1..10] (2, 5) == [[1,2,3,4,5],[6,7,8,9,10]]
    print $ form [1..10] (5, 2) == [[1,2],[3,4],[5,6],[7,8],[9,10]]
    print $ form "abcdefghi" (3, 3) == ["abc","def","ghi"]
    print $ form [True,False,True,False,True,False,True,False,True] (3, 3) == [[True,False,True],[False,True,False],[True,False,True]]

    print $ constGrid False (1, 1) == [[False]]
    print $ constGrid False (1, 3) == [[False,False,False]]
    print $ constGrid 0 (5, 5) == [[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0]]
    print $ constGrid 'a' (2, 4) == ["aaaa","aaaa"]

    print $ flatten [[1, 7], [3, 5], [0, -2]] == [1,7,3,5,0,-2]
    print $ (flatten $ form [5..16] (3, 4)) == [5,6,7,8,9,10,11,12,13,14,15,16]
    print $ flatten [["hi"]] == ["hi"]

    print $ access [[1, 2, 3], [4, 5, 6], [7, 8, 9]] (0, 0) == 1
    print $ access [[1, 2, 3], [4, 5, 6], [7, 8, 9]] (0, 2) == 3
    print $ access [[1, 2, 3], [4, 5, 6], [7, 8, 9]] (2, 2) == 9

    let grid = form [25..49] (5, 5)
    print $ slice grid (0, 2) (0, 1) == [[25],[30]]
    print $ slice grid (1, 3) (1, 3) == [[31,32],[36,37]]
    print $ slice grid (1, 4) (1, 4) == [[31,32,33],[36,37,38],[41,42,43]]
    print $ slice grid (1, 4) (1, 5) == [[31,32,33,34],[36,37,38,39],[41,42,43,44]]

    let grid = form [25..49] (5, 5)
    print $ without grid (0, 1) (0, 1)
    print $ without grid (0, 1) (0, 1) == [[31,32,33,34],[36,37,38,39],[41,42,43,44],[46,47,48,49]]
    print $ without grid (0, 0) (0, 1) == [[26,27,28,29],[31,32,33,34],[36,37,38,39],[41,42,43,44],[46,47,48,49]]
    print $ without grid (0, 0) (0, 0) == [[25,26,27,28,29],[30,31,32,33,34],[35,36,37,38,39],[40,41,42,43,44],[45,46,47,48,49]]
    print $ without grid (0, 3) (0, 0) == [[40,41,42,43,44],[45,46,47,48,49]]
    print $ without grid (0, 3) (0, 3) == [[43,44],[48,49]]
    print $ without grid (2, 3) (1, 4) == [[25,29],[30,34],[40,44],[45,49]]

    let grid = [[1, 2, 3], [4, 5, 6]]
    print $ vcat grid grid == [[1,2,3], [4,5,6], [1,2,3], [4,5,6]]
    print $ hcat grid grid == [[1,2,3,1,2,3],[4,5,6,4,5,6]]
    let grid = constGrid 0 (3, 3)
    let pattern = [[0]]
    print $ matches2d grid pattern
    print $ matches2d grid pattern == [(0,0),(0,1),(0,2),(1,0),(1,1),(1,2),(2,0),(2,1),(2,2)]
    let pattern = [[1]]
    print $ matches2d grid pattern == []

    let grid = [[0,0,0,0],[0,1,1,0],[0,1,1,0],[0,0,0,0]]
    print $ matches2d grid [[1, 1], [1, 1]]
    print $ matches2d grid [[1, 1], [1, 1]] == [(1,1)]
    print $ matches2d grid [[1, 1], [0, 0]] == [(2,1)]
    print $ matches2d grid [[1, 0], [0, 0]] == [(2,2)]
    let grid = [[1,0,1,0,1],[0,1,0,1,0],[1,0,1,0,1],[0,1,0,1,0],[1,0,1,0,1]]
    let pattern = [[0,1,0],[1,0,1],[0,1,0]]
    print $ matches2d grid pattern
    print $ matches2d grid pattern == [(0,1),(1,0),(1,2),(2,1)]

    let g = form [1..10000000] (1000, 10000) :: [[Int]]
    print $ matches2d g [[300, 301], [10300, 10301]]