#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LABEL 16
#define STR_OPCODE 16
#define STR_OPERAND 32

struct instruction {
    /// Circular Doubly Linked List
    struct instruction* prev;

    char label[STR_LABEL];
    char opcode[STR_OPCODE];
    char operand[STR_OPERAND];

    struct instruction* next;
};
typedef struct instruction* IN;

IN head_in = NULL;
IN tail_in = NULL;

struct optab {
    /// Singly Linked List
    char opcode[STR_OPCODE];
    char mnemonic[STR_OPCODE];

    struct optab* next;
};
typedef struct optab* OT;

OT head_ot = NULL;
OT tail_ot = NULL;

int countWords(char* str) {
    int state = 0;
    int wordCount = 0;

    while(*str) {
        if(*str == ' ' || *str == '\n' || *str == '\t')
        {
            state = 0;
        }
        else if(state == 0)
        {
            state = 1;
            ++wordCount;
        }
        ++str;
    }

    return wordCount;
}

void addToIN(char* label, char* opcode, char* operand) {
    if(head_in == NULL) {
        head_in = (struct instruction*)malloc(sizeof(struct instruction));
        if(head_in == NULL) {
            printf("Error: Memory Allocation - head_in\n");
            exit(1);
        }

        strcpy(head_in->label, label);
        strcpy(head_in->opcode, opcode);
        strcpy(head_in->operand, operand);

        head_in->prev = head_in;
        head_in->next = head_in;
        tail_in = head_in;
    } else {
        IN temp_in = (struct instruction*)malloc(sizeof(struct instruction));
        if(temp_in == NULL) {
            printf("Error: Memory Allocation - temp_in\n");
            exit(1);
        }

        strcpy(temp_in->label, label);
        strcpy(temp_in->opcode, opcode);
        strcpy(temp_in->operand, operand);

        temp_in->prev = tail_in;
        temp_in->next = head_in;
        tail_in->next = temp_in;
        head_in->prev = tail_in;
        tail_in = temp_in;
    }
}

void addToOT() {
    FILE* fp_ot = fopen("optab.txt", "r");

    /// File Pointer should point to 'optab.txt' - ALWAYS
    char* opcode = (char*)malloc(sizeof(char) * 16);
    char* mnemonic = (char*)malloc(sizeof(char) * 16);

    while(1) {
        fscanf(fp_ot,"%s %s\n", opcode, mnemonic);

        if(head_ot == NULL) {
            head_ot = (struct optab*)malloc(sizeof(struct optab));
            if(head_ot == NULL) {
                printf("Error: Memory Allocation - head_ot - addToOT()");
                exit(1);
            }
            strcpy(head_ot->opcode, opcode);
            strcpy(head_ot->mnemonic, mnemonic);
            head_ot->next = NULL;
            tail_ot = head_ot;
        } else {
            OT temp = (struct optab*)malloc(sizeof(struct optab));
            if(temp == NULL) {
                printf("Error: Memory Allocation - temp - addToOT()");
                exit(1);
            }
            strcpy(temp->opcode, opcode);
            strcpy(temp->mnemonic, mnemonic);
            temp->next = NULL;
            tail_ot->next = temp;
            tail_ot = temp;
        }

        if(feof(fp_ot)) break;
    }

    fclose(fp_ot);
}

void createTokens(char* str) {
    int wordCount = countWords(str);
    char* label = (char*)malloc(sizeof(char) * STR_LABEL);
    char* opcode = (char*)malloc(sizeof(char) * STR_OPCODE);
    char* operand = (char*)malloc(sizeof(char) * STR_OPERAND);

    char* token = (char*)malloc(sizeof(char) * STR_OPERAND);

    if(wordCount == 1) {
        label = "   ";
        token = strtok(str, "\n");
        opcode = token;
        operand = "   ";
    } else if(wordCount == 2) {
        label = "   ";
        token = strtok(str, " ");
        opcode = token;
        token = strtok(NULL, "\n");
        operand = token;
    } else if(wordCount > 2) {
        token = strtok(str, " ");
        label = token;
        token = strtok(NULL, " ");
        opcode = token;
        token = strtok(NULL, "\n");
        operand = token;
    }

    addToIN(label, opcode, operand);
}

int search_optab(char* opcode) {
    OT temp = (struct instruction*)malloc(sizeof(struct instruction));
    temp = head_ot;

    while(temp != NULL) {
        if(strcmp(temp->opcode, opcode) == 0) {
            return 1;
        }
        temp= temp->next;
    }
    return 0;
}

void display_in() {
    IN temp = head_in;
    do {
        printf("%-10s %-10s %-10s\n", temp->label, temp->opcode, temp->operand);
        temp = temp->next;
    } while(temp != head_in);
}

void display_ot() {
    OT temp = head_ot;
    do {
        printf("%-10s %-10s\n", temp->opcode, temp->mnemonic);
        temp = temp->next;
    } while(temp != NULL);
}

int main() {
    FILE* fp_in = fopen("input.txt", "r");
    FILE* fp_im = fopen("intermediate.txt", "w");
    FILE* fp_st = fopen("symtab.txt", "r");

    addToOT();


    char* str = (char*)malloc(sizeof(char) * 128);
    while(1) {
        fgets(str, 128, fp_in);
        createTokens(str);

        if(feof(fp_in)) break;
    }


    IN iter = (struct instruction*)malloc(sizeof(struct instruction));
    iter = head_in;
    char* start_address = (char*)malloc(sizeof(char) * 8);
    int locctr;

    if(strcmp(iter->opcode, "START") == 0) {
        fprintf(fp_im, "$ %-10s %-10s %-10s\n", iter->label, iter->opcode, iter->operand);
        // First Line does not have an 'address' - hence the '$' symbol.
        strcpy(start_address, iter->operand);
        locctr = strtol(start_address, NULL, 16);
        iter = iter->next; /// At second line;

        do {
            if(search_optab(iter->opcode)) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + 3;
            } else if(strcmp(iter->opcode, "WORD") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + 3;
            } else if(strcmp(iter->opcode, "RESW") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + (3 * strtol(iter->operand, NULL, 10));
            } else if(strcmp(iter->opcode, "RESB") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + strtol(iter->operand, NULL, 10);
            } else if(strcmp(iter->opcode, "BYTE") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                if(iter->operand[0] == 'C') {
                    locctr = locctr + (strlen(iter->operand) - 3);
                } else if(iter->operand[0] == 'X') {
                    locctr = locctr + ((strlen(iter->operand) - 3) / 2);
                }
            } else {
                printf("%-10s - ERROR!", iter->opcode);
                exit(1);
            }
            iter = iter->next;
        } while(iter != head_in);
    } else {
        locctr = strtol("0000", NULL, 16);
        // fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
        // ^ This line prints the first line. If used, will print the first line twice - unnecessary.

        do {
            if (search_optab(iter->opcode)) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + 3;
            } else if (strcmp(iter->opcode, "WORD") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + 3;
            } else if (strcmp(iter->opcode, "RESW") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + (3 * strtol(iter->operand, NULL, 10));
            } else if (strcmp(iter->opcode, "RESB") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                locctr = locctr + strtol(iter->operand, NULL, 10);
            } else if (strcmp(iter->opcode, "BYTE") == 0) {
                fprintf(fp_im, "%-10x %-10s %-10s %-10s\n", locctr, iter->label, iter->opcode, iter->operand);
                if(iter->operand[0] == 'X') {
                    locctr = locctr + strtol((strlen(iter->operand) - 3) / 2, NULL, 16);
                } else if(iter->opcode == 'C') {
                    locctr = locctr + strtol(strlen(iter->operand) - 3, NULL, 16);
                }
            } else {
                printf("%-10s - ERROR!", iter->opcode);
                exit(1);
            }
            iter = iter->next;
        } while(iter != head_in);
    }


    fclose(fp_in);
    fclose(fp_im);
    fclose(fp_st);
    return 0;
}
