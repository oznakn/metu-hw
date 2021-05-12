#include "the9.h"
void large() {
    char *text = new char[4001];
    char *pattern = new char[21];
    char alphabet[] = "AZ";

    std::fill(text, text + 4000, 'A');
    std::fill(pattern, pattern + 20, 'A');
    pattern[20] = text[4000] = '\0';

    pattern[0] = '?';
    text[0] = 'B';

    char *result = search(text, pattern, alphabet);

    delete[] text;
    delete[] pattern;

    std::istringstream ss;
    ss.rdbuf()->str(result);
    int read = -1;
    int i = 1;
    while (!(ss >> read).eof())
        if (read != i++) {
            std::cout << "FAILED - Expected " << i - 1 << ", found " << read
                      << std::endl;
            return;
        }
    if (i != 4000 - 20 + 2)
        std::cout << "FAILED - Expected " << 4000 - 20 + 1
                  << " results, found " << i - 1 << " results" << std::endl;
    else
        std::cout << "SUCCESS" << std::endl;
}

int main()
{
    srandom(time(0));

    char txt[] = "AAABAABAAABAAACBBAAAFGHAAABAAAAAABAAAAABBBBAABBB";
    char pat[] = "AAAB";
    char alp[] = "AO";
    char *ret;

    ret = search (txt, pat, alp);

    for (unsigned i=0 ; i<strlen(ret) ; i++){
        std::cout << ret[i];
    }

#ifdef DEBUG
    DEBUG_ARRAY(arr, 5);
    DEBUG_STDERR(arr[3]);
#endif
    large();
    return 0;
}
