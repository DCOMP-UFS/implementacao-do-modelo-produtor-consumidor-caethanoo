#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

// Cores ANSI
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"

#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"

static const char* PROD_COLORS[] = {"\033[94m", "\033[95m", "\033[96m"};
static const char* CONS_COLORS[] = {"\033[32m", "\033[33m", "\033[36m"};

static inline void print_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* tm_info = localtime(&tv.tv_sec);
    printf(DIM "[%02d:%02d:%02d.%03ld]" RESET " ", 
           tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, tv.tv_usec/1000);
}

static inline void print_queue_bar(int count, int max) {
    printf(" [");
    for (int i = 0; i < max; i++) {
        if (i < count) printf(GREEN "█" RESET);
        else printf(DIM "░" RESET);
    }
    printf("] %d/%d", count, max);
}

static inline void log_header(int num_prod, int num_cons, int buffer, int delay_prod, int delay_cons) {
    printf("\n");
    printf(BOLD "╔════════════════════════════════════════════════════════════════╗\n");
    printf("║          " MAGENTA "PRODUTOR" RESET BOLD " - " CYAN "CONSUMIDOR" RESET BOLD "                            ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n" RESET);
    printf(BOLD "║" RESET "  Produtores: " MAGENTA "%d" RESET "   Consumidores: " CYAN "%d" RESET "   Buffer: " YELLOW "%d" RESET "            " BOLD "║\n", 
           num_prod, num_cons, buffer);
    printf(BOLD "║" RESET "  Delay Prod: " MAGENTA "%ds" RESET "  Delay Cons: " CYAN "%ds" RESET "                        " BOLD "║\n",
           delay_prod, delay_cons);
    printf(BOLD "╠════════════════════════════════════════════════════════════════╣\n");
    printf("║" RESET " " MAGENTA "▶ PRODUZ" RESET " = Produtor adiciona    " CYAN "◀ CONSUM" RESET " = Consumidor retira " BOLD "║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

static inline void log_produz(int id, int r0, int r1, int r2, int count, int max) {
    print_time();
    printf(MAGENTA "▶ PRODUZ" RESET " %sP%d%s → [%2d,%2d,%2d]        ", 
           PROD_COLORS[id % 3], id, RESET, r0, r1, r2);
    print_queue_bar(count, max);
    printf("\n");
}

static inline void log_consome(int cons_id, int prod_id, int r0, int r1, int r2, int count, int max) {
    print_time();
    printf(CYAN "◀ CONSUM" RESET " %sC%d%s ← P%d [%2d,%2d,%2d]", 
           CONS_COLORS[cons_id % 3], cons_id, RESET, prod_id, r0, r1, r2);
    print_queue_bar(count, max);
    printf("\n");
}

static inline void log_espera_cheio(int id) {
    print_time();
    printf(BG_RED BOLD " ESPERA " RESET " %sP%d%s aguardando - fila cheia\n", 
           PROD_COLORS[id % 3], id, RESET);
}

static inline void log_espera_vazio(int id) {
    print_time();
    printf(BG_RED BOLD " ESPERA " RESET " %sC%d%s aguardando - fila vazia\n", 
           CONS_COLORS[id % 3], id, RESET);
}

#endif

