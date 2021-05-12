#include "the9.h"

vector<string> calculate_pattern_list(char pattern[], char alphabet[]) {
    vector<string> results;

    stack<string> item_queue;

    string start(pattern);
    item_queue.push(start);

    while (!item_queue.empty()) {
        string head = item_queue.top();
        item_queue.pop();

        bool found = false;

        for (int i = 0; i < head.size(); i++) {
            if (head[i] == '?') {
                for (char c = alphabet[0]; c <= alphabet[1]; c++) {
                    head[i] = c;
                    item_queue.push(head);
                }

                found = true;
                break;
            }
        }

        if (!found) {
            results.push_back(head);
        }
    }

    return results;
}

int fetch_next_state(string pattern, int M, int state, char letter) {
    if (state < M && pattern[state] == letter) {
        return state + 1;
    }

    for (int i = state; i > 0; i--) {
        if (pattern[i - 1] == letter) {
            int j;

            for (j = 0; j < i - 1; j++) {
                if (pattern[j] != pattern[state - i + j + 1]) {
                    break;
                }
            }

            if (j == i - 1) {
                return i;
            }
        }
    }

    return 0;
}

char * search (char txt[], char pat[], char alphabet[]) {
    int N = strlen(txt);
    int M = strlen(pat);

    vector<int> results;

    char first_letter = alphabet[0];
    int alphabet_size = alphabet[1] - alphabet[0] + 1;

    int TF[M + 1][alphabet_size];

    vector<string> patterns = calculate_pattern_list(pat, alphabet);

    for (int k = 0; k < patterns.size(); k++) {
        for (int i = 0; i <= M; i++) {
            for (int j = 0; j < alphabet_size; j++) {
                TF[i][j] = fetch_next_state(patterns[k], M, i, first_letter + j);
            }
        }

        int state = 0;
        for (int i = 0; i < N; i++) {
            state = TF[state][(int) txt[i] - first_letter];

            if (state == M) {
                results.push_back(i - M + 2);
            }
        }

    }

    sort(results.begin(), results.end());

    char buf[30000] = {0};

    if (patterns.size() == 1) {
        for (int i = 0; i <= M; i++) {
            for (int j = 0; j < alphabet_size; j++) {
                sprintf(buf + strlen(buf), "%d ", TF[i][j]);
            }

            buf[strlen(buf)] = '\n';
        }
    }

    for (int i = 0; i < results.size(); i++) {
        sprintf(buf + strlen(buf), "%d ", results[i]);
    }

    int buf_size = strlen(buf);

    char *char_r = new char[buf_size + 1];

    for (int i = 0; i < buf_size; i++) {
        char_r[i] = buf[i];
    }

    char_r[buf_size] = 0;

    return char_r;
}

