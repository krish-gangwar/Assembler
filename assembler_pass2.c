#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char label[10],opcode[15],operand[10],mod_operand[10],op[10],operand_symtab[10],obj[6],loc[6];
int i,len,k=-1,x;
int flag_1 = 0;
int flag_2 = 0;
int get_mnemonic(FILE *f3){
    while(!feof(f3))
        {
            if(!strcmp(opcode, "RESB")||!strcmp(opcode, "RESW")){
                return 1;
            }
            if(strcmp(opcode,"BYTE")==0)
            {
                len = strlen(operand);
                if(operand[0]=='C')
                {
                    for(i = 2; i <= len-2; i++)
                    {
                        k++;
                        sprintf(obj+k*2, "%02X", operand[i]);      //Conversion of char to its respective ASCII values
                    }
                    k = -1;
                    return 1;
                }
                else if(operand[0]=='X')
                {
                    for(i = 2; i <= len-2; i++)
                    {
                        k++;
                        sprintf(obj+k, "%c", operand[i]);
                    }
                    k = -1;
                    return 1;
                }
            }
            else if(strcmp(opcode,"WORD")==0)
            {
                len = strlen(operand);
                for(i = 0; i <= 5-len; i++)
                    strcat(obj,"0");
                sscanf(operand, "%d", &x);
                sprintf(operand, "%X", x);
                strcat(obj,"0");
                strcat(obj,operand);
                return 1;
            }
            else if(strcmp(opcode,op)==0)
            {
                fscanf(f3,"%s",op);
                strcpy(obj, op);
                return 1;
            }
            else
                fscanf(f3,"%s",op);
        }
        return 0;
}
int search_symtab(FILE *f2){
    while(!feof(f2))
        {
            if(!strcmp(opcode, "RESB")||!strcmp(opcode, "RESW")){
                return 1;
            }
            if(strcmp(opcode,"BYTE")==0 || strcmp(opcode,"WORD")==0)
                return 1;
            else if(strcmp(opcode,"RSUB")==0)
            {
                strcat(obj,"0000");
                return 1;
            }
            else if(strcmp(operand,operand_symtab)==0)
            {
                strcat(obj,loc);
                return 1;
            }
            else
            {
                fscanf(f2,"%s",operand_symtab);
                fscanf(f2,"%s",loc);
            }
        }
        return 0;
}
void main(){
    FILE *f1,*f2,*f3,*f4;
    f1=fopen("code.txt","r");
	f2=fopen("symtab.txt","r");
	f3=fopen("optab.txt","r");
	f4=fopen("objcode.txt","w");
	while(!feof(f1))
    {
        fscanf(f1,"%s %s %s",label,opcode,operand);
        //printf("%s %s %s\n",label,opcode,operand);
        fscanf(f2,"%s",operand_symtab);
        fscanf(f2,"%s",loc);
        fscanf(f3,"%s",op);
        strcpy(mod_operand,operand);
        flag_1 = get_mnemonic(f3);
        fseek(f3,0,SEEK_SET);
        flag_2 = search_symtab(f2);
        fseek(f2,0,SEEK_SET);
        if (flag_1 == 0){
            printf("Incorrect Mnemonic error. \n");
        }
        else if (flag_2 == 0){
            printf("Label referencing error. \n");
        }
        else if(flag_1 == 1&&flag_2 == 1){
            printf("%s\t%s\t%s\t\t%s\n",label,opcode,mod_operand,obj);
            fprintf(f4,"%s\t%s\t%s\t\t%s\n",label,opcode,mod_operand,obj);
        }
        else{
            printf("%s\t%s\t%s\n",label,opcode,mod_operand);
            fprintf(f4,"%s\t%s\t%s\n",label,opcode,mod_operand);
        }
        strcpy(obj,"");
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
}
