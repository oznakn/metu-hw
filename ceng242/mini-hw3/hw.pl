:- module(hw, [cost/2, sort_units/2, buyable/4, divide_list/3]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

cost_one(Name, Star, Cost) :-
    unit(Name, UnitCost, _), Cost is (3 ** (Star - 1)) * UnitCost.

cost([], 0).
cost([active_unit(Name, Star) | Tail], Cost) :-
    cost_one(Name, Star, CostOne),
    cost(Tail, TailSum),
    Cost is CostOne + TailSum.


divide_list([], [], []).
divide_list([Name1], [Name1], []).
divide_list(List, Result1, Result2) :-
    length(List, ListLength),
    FirstLength is ListLength - (ListLength // 2),
    SecondLength is (ListLength // 2),
    append(Result1, Result2, List),
    length(Result1, FirstLength),
    length(Result2, SecondLength),
    !.

combine_lists([], [], []).
combine_lists(List, [], List).
combine_lists([], List, List).
combine_lists([Name1 | Tail1], [Name2 | Tail2], [Name1 | Rest]) :-
    unit(Name1, Cost1, _),
    unit(Name2, Cost2, _),
    Cost1 >= Cost2,
    combine_lists(Tail1, [Name2 | Tail2], Rest),
    !.

combine_lists([Name1 | Tail1], [Name2 | Tail2], [Name2 | Rest]) :-
    unit(Name1, Cost1, _),
    unit(Name2, Cost2, _),
    Cost1 < Cost2,
    combine_lists([Name1 | Tail1], Tail2, Rest),
    !.

sort_units([], []).
sort_units([Name1], [Name1]).
sort_units(List, Result) :-
    divide_list(List, List1, List2),
    sort_units(List1, Result1),
    sort_units(List2, Result2),
    combine_lists(Result1, Result2, Result),
    !.


buyable([], Money, BoughtUnits, Money) :- BoughtUnits \= [].
buyable([Name | _], Money, [Name], RemainingMoney) :-
    unit(Name, Cost, _),
    Money >= Cost,
    RemainingMoney is Money - Cost.

buyable([Name | Tail], Money, [Name|BoughtUnitsTail], RemainingMoney) :-
    unit(Name, Cost, _),
    Money >= Cost,
    LeftMoney is Money - Cost,
    buyable(Tail, LeftMoney, BoughtUnitsTail, RemainingMoney).

buyable([_ | Tail], Money, BoughtUnits, RemainingMoney) :-
    buyable(Tail, Money, BoughtUnits, RemainingMoney).
