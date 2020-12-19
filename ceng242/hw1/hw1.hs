module HW1 (
    form,
    constGrid,
    flatten,
    access,
    slice,
    vcat,
    hcat,
    without,
    matches2d
) where

-- do not modify the module declaration above!
-- this will ensure that you cannot load (compile)
-- the module without implementing all of the functions.

-- If you have functions you do not want to implement,
-- leave them as undefined or make them have another
-- default value. If you fully remove any of their definitions,
-- that will be a compilation error during evaluation,
-- and you will be eligible for (yay!) a 5 point deduction
-- (that's bad for your grade). Runtime errors in your code
-- (rather than compilation errors) are acceptable and will simply
-- result in you getting zero from the specific test case causing
-- an error.

-------------------------
-- Fellowship of the Grid (25, 5, 5, 5 points)
form :: [a] -> (Int, Int) -> [[a]]
form list (height, width) =
    fst (foldl (\(items, list) i ->
        (
            items ++ [take width list],
            drop width list
        )
    ) ([], list) [1..height])

constGrid :: a -> (Int, Int) -> [[a]]
constGrid item (height, width) =
    replicate height (replicate width item)


flatten :: [[a]] -> [a]
flatten grid =
    concat grid


access :: [[a]] -> (Int, Int) -> a
access grid (i, j) = (grid !! i) !! j


----------------------------
-- The Two Signatures (10, 5, 5, 10 points)
slice :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
slice grid (start_i, end_i) (start_j, end_j) =
    map (\list -> drop start_j (take end_j list)) (drop start_i (take end_i grid))


vcat :: [[a]] -> [[a]] -> [[a]]
vcat grid1 grid2 = grid1 ++ grid2


hcat :: [[a]] -> [[a]] -> [[a]]
hcat grid1 grid2 =
    map (\(list1, list2) -> list1 ++ list2) (zip grid1 grid2)


without :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
without grid (start_i, end_i) (start_j, end_j) =
    map (\list -> ((take start_j list) ++ (drop end_j list))) ((take start_i grid) ++ (drop end_i grid))


----------------------------
-- Return of the Non-trivial (30 points, 15 subject to runtime constraints)
matches2d :: Eq a => [[a]] -> [[a]] -> [(Int, Int)]
matches2d grid pattern =
    snd (foldl (\(c_grid, items) i ->
                (
                    (tail c_grid),
                    items ++
                        (snd (foldl (\(r_c_grid, r_items) j ->
                            (
                                (map tail r_c_grid),
                                if (map (take pattern_width) r_c_grid) == pattern
                                    then r_items ++ [(i, j)]
                                    else r_items
                            ))

                            ((take pattern_height c_grid), []) grid_width_map))
                ))

                (grid, []) grid_height_map)

    where
        pattern_width = (length (head pattern))
        pattern_height = (length pattern)
        grid_width_map = [0..((length (head grid)) - pattern_width)]
        grid_height_map = [0..((length grid) - pattern_height)]


----------------------------
-- What is undefined? Just a value that will cause an error
-- when evaluated, from the GHC implementation:
-- undefined = error "Prelude.undefined"
-- But it allows your module to be compiled
-- since the function definitions will exist.