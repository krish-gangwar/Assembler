#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
struct symtab{
    char label[32];
    int LOC_value;
};
int LOCCTR;
char label[30],opcode[30];
char op[30];
char ope[6];
char operand[30];
int checkerror(FILE *fp3){

    if(!strcmp(opcode, "WORD")||!strcmp(opcode, "BYTE")||!strcmp(opcode, "RESB")||!strcmp(opcode, "RESW")){

        return 1;
    }
    while(!feof(fp3)){

            fscanf(fp3, "%s %s%*[^\n]",op, ope);
            if(!strcmp(op,opcode)){

                fseek(fp3,0,SEEK_SET);
                return 1;
            }

    }

    printf("There is a error in the mnemonic: %s. \n", opcode);
    printf("Aborting this line..\n\n");
    fseek(fp3,0,SEEK_SET);
    return 0;

}
int increment_LOCCTR(int loc, char opcode[30], char operand[30]){

    int op_num;
    int i;
    int byte_count = 0;
    if(isdigit(operand[0])){
        op_num = atoi(operand);
    }
    if(opcode[0] == '+'){
        loc = loc + 4;
        return loc;
    }
    if(!strcmp(opcode,"WORD")){
        loc = loc + 3;
        return loc;
    }
    if(!strcmp(opcode,"RESW")){
        loc = loc + (op_num * 3);
        return loc;
    }
    if(!strcmp(opcode,"RESB")){
        loc = loc + (op_num * 1);
        return loc;
    }
    if(!strcmp(opcode,"BYTE")){
        for(i = 3; i < 30; i++){
            byte_count++;
            if(operand[i] == '\''){
                break;
            }
        }

        loc = loc + (byte_count);
        return loc;

    }
    else{

        loc = loc + 3;
        return loc;
    }

}
int main(){

  char c;
  char d;
  char k[10];
  int i;
  int table_ctr = 0;
  FILE *fp = fopen("code.txt", "r");
  FILE *fp1 = fopen("symtab.txt", "w");
  FILE *fp2 = fopen("intermediate.txt","w");
  FILE *fp3 = fopen("optab.txt","r");
  struct symtab table[100];
  if(fp == NULL){
    printf("Sorry! Can't open file. ");
  }

  fscanf(fp,"%s %s %x%*[^\n]",label,opcode,&LOCCTR);

  while(!feof(fp)){

    c = getc(fp);
    d = getc(fp);
    fseek(fp,-2L,1);

    if(d != '-'){
      fscanf(fp,"%s %s %s%*[^\n]",label,opcode,operand);
      if(checkerror(fp3) == 0){

            continue;

      }
      table[table_ctr].LOC_value = LOCCTR;
      strcpy(table[table_ctr].label, label);
      table_ctr++;
      fprintf(fp2, "\n%X\t%s\t%s\t%s\n", LOCCTR, label, opcode, operand);
      LOCCTR = increment_LOCCTR(LOCCTR,opcode,operand);

    }
    else{
      fscanf(fp,"%s %s %s%*[^\n]",k,opcode,operand);
      if(checkerror(fp3) == 0){

            continue;

      }
      fprintf(fp2, "\n%X\t%c\t%s\t%s\n", LOCCTR, d, opcode, operand);
      LOCCTR = increment_LOCCTR(LOCCTR,opcode,operand);
    }

  }
    fclose(fp);
    fprintf(fp1,"\n");

  for(i = 0; i < table_ctr; i++){

        printf("%s\t", table[i].label);
        printf("%X", table[i].LOC_value);
        printf("\n");
        fprintf(fp1,"%s\t", table[i].label);
        fprintf(fp1,"%X", table[i].LOC_value);
        fprintf(fp1,"\n");
        fprintf(fp1,"\n");
    }

    fclose(fp1);

}
