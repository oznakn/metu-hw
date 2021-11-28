var num: int;

int func factorial(number: int)
    if number <= 1
    then
        return 1;
    endif;

    return (number * factorial(num - 1));
endfunc

int func main()
    read num;

    if num < 0
    then
        print "Input must be zero or positive.";
        return 1;
    endif;

    print num, "! = ", factorial(num);
    return 0;
endfunc