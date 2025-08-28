#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// CLIを叩いて大量のデータを流し込み、scan結果を検証する
// ./bptree プロセスに add/del/scan コマンドをパイプする

#define N_INSERT 100000   // 挿入するデータ数
#define N_DELETE 20000    // 削除するデータ数

int main(void) {
    srand((unsigned)time(NULL));

    // 1. 大量のコマンドを生成
    FILE *tmp = fopen("cmds.txt", "w");
    if (!tmp) { perror("fopen cmds.txt"); return 1; }

    // 挿入
    for (int i = 0; i < N_INSERT; i++) {
        int key = rand() % (N_INSERT*10);
        fprintf(tmp, "add %d\n", key);
    }

    // 削除
    for (int i = 0; i < N_DELETE; i++) {
        int key = rand() % (N_INSERT*10);
        fprintf(tmp, "del %d\n", key);
    }

    // 最終 scan
    fprintf(tmp, "scan\nexit\n");
    fclose(tmp);

    // 2. ./bptree に流し込む
    FILE *fp = popen("./bptree < cmds.txt", "r");
    if (!fp) { perror("popen"); return 1; }

    // 出力を読み込む
    size_t cap = 1024*1024, len = 0;
    char *out = malloc(cap);
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (len + 1 >= cap) {
            cap *= 2;
            out = realloc(out, cap);
        }
        out[len++] = (char)c;
    }
    out[len] = '\0';
    pclose(fp);

    // 3. scan の結果行(RESULT: で始まる行)を探す
    char *scan_line = strstr(out, "RESULT: ");
    if (!scan_line) {
        fprintf(stderr, "scan の結果が見つかりません\n");
        free(out);
        return 2;
    }
    
    // "RESULT: " の後ろから数字部分を取得
    scan_line += 8;  // "RESULT: " をスキップ
    
    // 改行まで切り出し
    char *end = strchr(scan_line, '\n');
    if (end) *end = '\0';

    // 4. 整列確認
    int prev = -1, cur;
    char *tok = strtok(scan_line, " ");
    int count = 0, bad = 0;
    while (tok) {
        cur = atoi(tok);
        if (cur < prev) {
            fprintf(stderr, "[FAIL] scan 結果がソートされていません: %d の後に %d\n", prev, cur);
            bad = 1;
            break;
        }
        prev = cur;
        count++;
        tok = strtok(NULL, " ");
    }

    if (!bad) {
        printf("大規模テスト成功 ✅  要素数=%d\n", count);
    }

    free(out);
    return bad ? 3 : 0;
}