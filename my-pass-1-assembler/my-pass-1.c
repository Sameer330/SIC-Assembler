#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define STR_SIZE_LOW 16
#define STR_SIZE_HIGH 32
#define STR_LINE 128

#define OT_OP 8
/// Optab_Opcode - ADD, COMP, LDA, ...

#define OT_MNC 16
/// Optab_Mnemonic - 1D, 20, EF, ...

typedef struct instruction {
    /// Circular Doubly Linked List
    /// Insertion: O(1)
    /// Deletion: O(1)
    /// Search/Traversal: O(n)

    /// Struct for each instruction in 'input.txt'

    struct instruction *prev;

    char label[STR_SIZE_LOW];
    char opcode[STR_SIZE_LOW];
    char operand[STR_SIZE_HIGH];

    struct instruction *next;
} *INSTR;

INSTR head = NULL;
/// Head pointer for INSTR - 'struct instruction*'
INSTR curr = NULL;
/// Tail pointer for INSTR

typedef struct optab {
    /// Singly Linked List
    /// Insertion: O(1) | By maintaining a tail pointer.
    /// Deletion: O(1) | Deleting at tail only.
    /// Search/Traversal: O(n)

    /// Struct for each permitted opcode in 'optab.txt'

    char opcode[OT_OP];
    char mnemonic[OT_MNC];

    struct optab *next;
} *OT;

OT head_ot = NULL;
/// Head pointer for 'struct optab*'
OT curr_ot = NULL;
/// Tail pointer for 'struct optab*'


/// Function: add_to_head
/// Description: Initializes 'head'(INSTR) pointer and adds data to it
///              and assigns 'head' to 'curr'(Tail)
/// Input Param: char arrays - label, opcode & operand
/// Return Type: NULL
void add_to_head(char* _label, char* _opcode, char* _operand) {
    head = (struct instruction*)malloc(sizeof(struct instruction));

    strcpy(head->label, _label);
    strcpy(head->opcode, _opcode);
    strcpy(head->operand ,_operand);

    head->prev = head;
    head->next = head;

    curr = head;
}

/// Function: add_to_list
/// Description: Adds a INSTR node to existing INSTR list
///              and assigns new node to 'curr'(Tail)
/// Input Param: char arrays - label, opcode & operand
/// Return Type: NULL
void add_to_list(char* _label, char* _opcode, char* _operand) {
    INSTR temp = (struct instruction*)malloc(sizeof(struct instruction));

    strcpy(temp->label, _label);
    strcpy(temp->opcode ,_opcode);
    strcpy(temp->operand ,_operand);

    curr->next = temp;
    temp->prev = curr;
    temp->next = head;
    head->prev = curr;

    curr = temp;
}

/// Function: add_to_ot_list (Add to OT(OpTab) List)
/// Description:
///              1. if does not exist, Creates a OT list('head_ot' pointer)
///              2. Else adds new node to end of list
///              3. Assigns new node/head to 'curr_ot'(Tail)
/// Input Param: char arrays - opcode & mnemonic
/// Return Type: NULL
void add_to_ot_list(char* _opcode, char* _mnemonic) {

    OT temp_ot = (struct optab*)malloc(sizeof(struct optab));
    if(temp_ot == NULL) {
        printf("Error: add_to_ot_list() - Memory Allocation!\n");
        return;
    }

    strcpy(temp_ot->opcode, _opcode);
    strcpy(temp_ot->mnemonic, _mnemonic);

    temp_ot->next = NULL;

    if(head_ot == NULL) {
        head_ot = (struct optab*)malloc(sizeof(struct optab));
        curr_ot = (struct optab*)malloc(sizeof(struct optab));
        if(head_ot == NULL || curr_ot == NULL) {
            printf("Error: head_ot or curr_ot - NULL\n");
            return;
        }
        head_ot = temp_ot;
        curr_ot = head_ot;
    } else {
        curr_ot->next = temp_ot;
        curr_ot = temp_ot;
    }
}

/// Function: countWords
/// Description: Counts number of words in a string
/// Input Param: char array(string)
/// Return Type: int - count of words
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

/// Function: tokens_to_list
/// Description:
///              1. Tokenizes a string based on word count of that string
///              2. Tokens are used to create INSTR linked list
/// Input Param: char array(string)
/// Return Type: NULL
void tokens_to_list(char* line) {
    int line_words_count = countWords(line);

    char *_label = (char *)malloc(sizeof(char) * STR_SIZE_LOW);
    char *_opcode = (char *)malloc(sizeof(char) * STR_SIZE_LOW);
    char *_operand = (char *)malloc(sizeof(char) * STR_SIZE_HIGH);

    char* token = (char*)malloc(sizeof(char) * STR_SIZE_HIGH);
    const char* delim1 = " ";
    const char* delim2 = "\n";

    if(line_words_count == 1) {
        _label = "   ";

        token = strtok(line, delim1);
        _opcode = token;

        _operand = "   ";
    } else if(line_words_count == 2) {
        _label = "   ";


        token = strtok(line, delim1);
        _opcode = token;

        token = strtok(NULL, delim2);
        _operand = token;
    } else if(line_words_count >= 3) {
        token = strtok(line, delim1);
        _label = token;

        token = strtok(NULL, delim1);
        _opcode = token;

        token = strtok(NULL, delim2);
        _operand = token;
    }

    if(head == NULL) {
        add_to_head(_label, _opcode, _operand);
    } else if(head != NULL) {
        add_to_list(_label, _opcode, _operand);
    }
}

/// Function: countFileLines
/// Description: counts number of lines in a file
/// Input Param: FILE* - File pointer
/// Return Type: int - number of lines
int countFileLines(char *filename) {
    FILE *fp = fopen(filename, "r");

    if(fp == NULL) {
        printf("%s : ERROR", filename);
        return 0;
    }

    int count = 0;
    char ch;
    while(!feof(fp)) {
        ch = getc(fp);
        if(ch == '\n') {
            count = count + 1;
        }
    }

    fclose(fp);

    return count;
}

/// Function: search_symtab
/// Description: Searches a given label in symtab data structure
/// Input Param: char array - label, char[][] - symtab - 2D array
/// Return Type:
///             1 - if label exists
///             0 - if label does not exist
int search_symtab(char* label, char symtab[STR_SIZE_LOW][STR_SIZE_LOW]) {
    for(int i = 0; i < STR_SIZE_LOW; i++) {
        if(strcmp(label, symtab[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/// Function: search_optab
/// Description: searches a given opcode in OpTab linked list
/// Input Param: char array - opcode
/// Return Type:
///             1 - if opcode exists
///             0 - if opcode does not exist
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

/// Function: display
/// Description: Prints the contents of INSTR linked list
/// Input Param: NULL
/// Return Type: NULL
void display() {
    if(head == NULL) {
        printf("List is Empty!");
        return;
    }
    INSTR temp = head;

    do{
        printf("%s \t %s \t %s\n", temp->label, temp->opcode, temp->operand);
        temp = temp->next;
    } while(temp != head);
}

/// Function: display_ot
/// Description: Prints the contents of OT(OpTab) linked list
/// Input Param: NULL
/// Return Type: NULL
void display_ot() {
    if(head_ot == NULL) {
        printf("Optab list is empty!\n");
        return;
    }

    OT temp = head_ot;

    do {
        printf("%s \t %s\n", temp->opcode, temp->mnemonic);
        temp = temp->next;
    } while(temp != NULL);
}

int main() {
    curr = (struct instruction*)malloc(sizeof(struct instruction));
    /// curr -- Acts as 'tail' pointer for
    INSTR rcrsve = (struct instruction*)malloc(sizeof(struct instruction));

    char* filename = "input.txt"; /// Write your input SIC Program in this file

    FILE* fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("\nError: FILE* fp = %s", filename);
        exit(1);
    }

    char* line = (char*)malloc(sizeof(char) * STR_LINE);

    fgets(line, STR_LINE, fp);
    /// Read First Line from 'filename'

    tokens_to_list(line);
    /// Create tokens from first line
    /// Add token to linked list - as 'head' pointer

    while(fgets(line, STR_LINE, fp) != NULL) {
        /// Read all successive lines
        tokens_to_list(line);
        /// Create tokens - append all tokens as list item to head pointer
    }
    fclose(fp);
    /// ------------------------------------------

    /// Optab - Creation
    FILE* fp_ot = fopen("optab.txt", "r");

    char* opcode = (char*)malloc(sizeof(char) * STR_SIZE_LOW);
    char* mnemonic = (char*)malloc(sizeof(char) * STR_SIZE_LOW);

    while(fscanf(fp_ot, "%s %s\n", opcode, mnemonic) != EOF) {

        add_to_ot_list(opcode, mnemonic);

        //printf("%s %s\n", opcode, mnemonic);

    }

    fclose(fp_ot);
    /// Optab - Created

    /// ------------------------------------------

    /// SYMTAB - Creation - Starts Here

    char symtab[STR_SIZE_LOW][STR_SIZE_LOW];
    /// Change symtab 2D array size as required
    /// Current size - 16 x 16
    int st_counter = 0;

    INSTR temp_st = (struct instruction*)malloc(sizeof(struct instruction));
    temp_st = head;

    do {
        if(strcmp(temp_st->label, "   ") != 0) {
            strcpy(symtab[st_counter], temp_st->label);
            st_counter = st_counter + 1;

        }
        temp_st = temp_st->next;
    } while(temp_st != head);

    free(temp_st);

    /// SYMTAB - Creation - Ends Here

    /// -------------------------------------------------

    char* first_start_address = head->opcode;

    if(strcmp(first_start_address, "START") != 0) {
            /// If opcode is not "START"
            /// Assign 0 to starting address
            first_start_address = "0000";
    } else {
        /// Else assign the first operand
        /// i.e. operand corresponding to head pointer
        strcpy(first_start_address, head->operand);
    }

    int _locctr = strtol(first_start_address, NULL, 16);
    /// Convert 'first_start_address' to respective hexadecimal integer
    /// Use - %x - to print _locctr

    /// Intermediate File - Creation - Starts Here

    FILE* fp_if = fopen("intermediate.txt", "w");
    /// fp_if - File Pointer _ Intermediate File
    FILE* fp_st = fopen("symtab.txt", "w");
    /// fp_st - File Pointer _ Symbol Table(SymTab)

    INSTR temp_if = (struct instruction*)malloc(sizeof(struct instruction));
    /// A pointer to iterate INSTR Linked list

    temp_if = head;
    //fprintf(fp_if, "%x \t %s \t %s \t %s \n", _locctr, temp_if->label, temp_if->opcode, temp_if->operand);
    //temp_if = temp_if->next;
    /// These 2 commented lines might cause problems in intermediate file
    /// !!! Uncomment them at your own risk


    do {
        if(search_symtab(temp_if->label, symtab)) {
            /// If label in INSTR list
            /// insert(label, locctr) to symtab.txt
            fprintf(fp_st, "%s \t %x\n", temp_if->label, _locctr);
        }

        /// These lines of code are according to the Pass-1 Assembler Algorithm
        if (search_optab(temp_if->opcode) == 1) {
            fprintf(fp_if, "%x \t %s \t %s \t %s \n", _locctr, temp_if->label, temp_if->opcode, temp_if->operand);
            _locctr = _locctr + 3;
        } else if (strcmp(temp_if->opcode, "WORD") == 0) {
            fprintf(fp_if, "%x \t %s \t %s \t %s \n", _locctr, temp_if->label, temp_if->opcode, temp_if->operand);
            _locctr = _locctr + 3;
        } else if (strcmp(temp_if->opcode, "RESW") == 0) {
            fprintf(fp_if, "%x \t %s \t %s \t %s \n", _locctr, temp_if->label, temp_if->opcode, temp_if->operand);
            _locctr = _locctr + (3 * strtol(temp_if->operand, NULL, 10));
        } else if (strcmp(temp_if->opcode, "RESB") == 0) {
            fprintf(fp_if, "%x \t %s \t %s \t %s \n", _locctr, temp_if->label, temp_if->opcode, temp_if->operand);
            _locctr = _locctr + strtol(temp_if->operand, NULL, 10);
        } else if (strcmp(temp_if->opcode, "BYTE") == 0) {
            fprintf(fp_if, "%x \t %s \t %s \t %s \n", _locctr, temp_if->label, temp_if->opcode, temp_if->operand);
            _locctr = _locctr + strtol(strlen(temp_if->operand) - 3, NULL, 16);
        } else {
            printf("Invalid Operation Code!\n");
            printf("%s", temp_if->opcode);
            exit(1);
        }

        temp_if = temp_if->next;
    } while(temp_if != head);


    fclose(fp_st);
    fclose(fp_if);
    free(temp_if);

    /// Intermediate File - Creation - Ends Here

    /// ----------------------------------------------------

    /// ! Program Ends Here !

    return 0;
}
