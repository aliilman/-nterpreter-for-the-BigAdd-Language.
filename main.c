#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>

int main()
{
    char ch;
    int line=1;         //line
    int intlength=0;    //integer length
    int wordorkey=0;    //Keyword? true or false
    int wordlength=0;   //word length

    char integer[10];
    char word[20];
    char keywords[10][8]= {"int","move","to","loop","times","out","newline","add","sub","from"};
    char string[500];

    FILE*fileR;// File Read
    FILE*fileW;// File Write

    char file_name[25];

    printf("Enter name of a file name you want interpretered(without .ba)\n");
    gets(file_name);
    strcat(file_name,".ba");
    fileR = fopen(file_name, "r"); // read mode

    if (fileR == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fileW=fopen("myscript.lx","w");

    int variable_counter=0;  //variable that holds how many variables have been created

    ch=fgetc(fileR);// get first char

    int nokta=0;
    if(fileR != NULL)//File check
    {

        while(!feof(fileR) ) //continues unless error or end of file
        {
            int control=1; // control for unrecognized characters

            if(ch ==' '|| ch=='\n' || ch=='\t'|| ch==EOF)//space, tab, line break check
            {
                if(ch=='\n'|| ch==EOF) //line counter
                {
                    line++;
                    if(nokta==0)
                    {
                        printf("Error: There must be a dot at the end of the line or the command cannot follow the period (line:%d)",line);
                        return 0;
                    }
                    else
                        nokta=0;
                }

                ch=fgetc(fileR);// get char
                while(ch ==' '|| ch=='\n' || ch=='\t'|| ch=='['||ch==']')
                {
                    if(ch=='\n') //line counter
                        line++;
                    if(ch=='[')
                    {
                        fputs("OpenBlock\n", fileW);//puts operation to .lx file
                    }
                    if(ch==']')
                    {
                        fputs("CloseBlock\n", fileW);
                    }
                    ch=fgetc(fileR);
                }
            }

            if(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')//integer check
            {
                integer[intlength]=ch;
                ch=fgetc(fileR);
                if(ch =='.'|| ch==',')
                {
                    printf("\tERROR : Decimal point should not be displayed (line %d)\n",line);
                }

                while(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9') // integer after integer check
                {
                    intlength++;
                    integer[intlength]=ch;
                    ch=fgetc(fileR);
                }
                integer[intlength+1]='\0'; //final string assignment
                if(intlength>10) //int length error check
                {
                    printf("\tERROR : Maximum integer size is 10 digits (line %d)\n",line);
                    return 0;
                }
                else
                {
                    fputs("IntConstant ",fileW); //puts operation to .lx file
                    fputs(integer, fileW);
                    fputs("\n",fileW);
                }
                memset(integer, 0, sizeof(integer)); //clearing array from ram
                intlength=0; //reset the counter
                continue;
                control=0;
            }
            if(ch=='-') //negative int check
            {
                integer[intlength]=ch;
                intlength++;
                ch=fgetc(fileR);
                if(ch==' ')//No spaces after '-'
                {
                    printf("\tERROR :There should be no blank between the minus sign and the first digit (line %d)\n",line);
                    return 0;
                }
                else if(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')//same as integer check
                {
                    integer[intlength]=ch;
                    ch=fgetc(fileR);
                    while(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')
                    {
                        intlength++;
                        integer[intlength]=ch;
                        ch=fgetc(fileR);
                    }
                    integer[intlength+1]='\0';
                    if(intlength>10)
                    {
                        printf("\tERROR : Maximum integer size is 10 digits (line %d)\n",line);
                        return 0;
                    }
                    else
                    {
                        fputs("IntConstant ",fileW); //puts operation to .lx file
                        fputs(integer, fileW);
                        fputs("\n",fileW);
                    }
                    memset(integer, 0, sizeof(integer));//clearing array from ram
                    intlength=0;
                    continue;
                    control=0;
                }
                else
                {
                    printf("\tERROR :The number must come after the minus (line %d)\n",line);
                    return 0;
                }
            }

            if(ch=='.')//End Of Line
            {
                fputs("EndofLine\n",fileW);
                nokta=1;
                control=0;

            }
            if(ch==',')//Separator
            {
                fputs("Separator\n", fileW);
                control=0;

            }
            if(ch=='{'||ch=='}' || ch=='['||ch==']')//brackets check
            {
                if(ch=='{') //open comment
                {

                    ch=fgetc(fileR);

                    while(ch!=EOF && ch!='}')  //pass comment's char
                    {
                        ch=fgetc(fileR);
                    }
                    if(ch=='}')// close comment
                    {
                        ch=getc(fileR);
                        continue;
                    }
                    else if(ch==EOF)//file ending before comment ends
                    {
                        printf("ERROR :A big lexeme is just left open (a Comment or a string constant that starts but does not terminate before the end of file) (line %d)",line);
                        return 0;
                    }

                }
                if(ch=='[')
                {
                    fputs("OpenBlock\n", fileW);//puts operation to .lx file
                }
                if(ch==']')
                {
                    fputs("CloseBlock\n", fileW);
                }
                control=0;

            }

            if(ch=='"')//strings check //open string
            {
                int counter=0;
                string[counter]=ch;
                counter++;

                ch=fgetc(fileR);

                while(ch!= '"' ) // pass string's char // close string
                {
                    string[counter]=ch;
                    counter++;
                    ch=fgetc(fileR);
                    if(ch==EOF) //file ending before string ends error
                    {
                        printf("ERROR :A big lexeme is just left open (a comment or a String constant that starts but does not terminate before the end of file) (line %d)",line);
                        return 0;
                    }
                }
                string[counter]=ch;

                string[counter+1]='\0'; //string's final indice assignment
                fputs("StringConstants ",fileW ); //puts operation to .lx file
                fputs(string,fileW);
                fputs("\n",fileW);
                control=0;
            }

            if(('a' <= ch && ch <='z') || ('A' <= ch && ch <= 'Z'))//identifier and keywords check
            {

                wordorkey=0;
                wordlength=0;
                while(('a' <= ch && ch <='z')|| ('A' <= ch && ch <= 'Z') || (ch=='_')) //alphabetical char control
                {
                    if(wordlength>20) //max size check
                    {
                        printf("\tERROR : Maximum identifier size is 20 characters. (line %d)",line);
                        return 1;
                    }
                    word[wordlength]=ch;
                    wordlength++;
                    ch=fgetc(fileR);

                    if(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')//Check for integer after alphabetic value
                    {
                        word[wordlength]=ch;
                        wordlength++;
                        ch=fgetc(fileR);
                        while(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')
                        {

                            word[wordlength]=ch;
                            wordlength++;
                            ch=fgetc(fileR);
                        }
                    }
                }
                word[wordlength]='\0';
                //KEYWORDS OR IDENTIFIERS
                for(int x=0; x<10; x++)
                {
                    if(strcmp(word,keywords[x]) == 0 ) //If the word is a keyword program enters this part and it sets the wordorkey variable to 1.
                    {
                        if(strcmp("int",word)==0)//variable detection
                            variable_counter++;
                        fputs("Keyword ",fileW);
                        fputs(word,fileW);
                        fputs("\n",fileW);
                        if(strcmp("loop",word)==0)
                            nokta=1;
                        wordorkey=1;
                        break;
                    }
                }

                if(wordorkey == 0 )// wordorkey = 1 => string is keyword
                {
                    fputs("Identifier ",fileW);
                    fputs(word,fileW);
                    fputs("\n",fileW);
                }
                memset(word, 0, sizeof(word));//clearing array from ram
                wordlength=0;
                control=0;
                continue;
            }


            if(control==1 && ch!=EOF) //unrecognized character check
            {
                printf("\tERROR : An unrecognized character is detected in code (line %d)",line);
                return 0;
            }
            ch=getc(fileR);
        }//while
    }//if
    fclose(fileW);

    FILE*fileRLX;
    fileRLX=fopen("myscript.lx","r");

    char istring[30];                       //string of words
    char atanmislar[variable_counter][20];  //this list holds the variables' names which we defined before
    int counter=0; //counter
    int block=0; //block check
    int degerler[variable_counter]; //this list holds variables' values
    int degerlerindex=0;
    int value; //Variable value
    wordlength=0;
    long int konum; //location for loop
    int loop_times;
    int loop_counter=0;
    int line1=1;
    int looptype=0;
    if(fileRLX != NULL)
    {
        while(! feof(fileRLX) )
        {
            kelime_al(istring,wordlength,fileRLX,ch);

            if(strcmp("EndofLine",istring)==0)//line check
            {
                kelime_al(istring,wordlength,fileRLX,ch);
                line1++;
            }
            ///Keyword
            if(strcmp("Keyword",istring)==0)
            {
                kelime_al(istring,wordlength,fileRLX,ch);
            }
            ///integer
            if(strcmp("int",istring)==0)
            {
                kelime_al(istring,wordlength,fileRLX,ch);

                if(strcmp( "Identifier",istring)==0)
                {
                    kelime_al(istring,wordlength,fileRLX,ch); //get new word
                    int counterbefore=counter;
                    counter+=atama_kontrol(istring,atanmislar,variable_counter,counter);//assignment check
                    if(counter==counterbefore)
                    {
                        printf("ERROR:'%s' has been defined before (line:%d)\n",istring,line1);
                    }

                    atama_yap(istring,atanmislar,variable_counter,counter,0,degerler);//make an assignment
                }
                else
                    printf("ERROR: There must be a variable name after 'int' keyword (line:%d)\n",line1);
            }
            ///MOVE
            if(strcmp("move",istring)==0)
            {
                kelime_al(istring,wordlength,fileRLX,ch);
                if(strcmp("IntConstant",istring)==0)
                {
                    kelime_al(istring,wordlength,fileRLX,ch);

                    value=atoi(istring);//string to integer converter

                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                        kelime_al(istring,wordlength,fileRLX,ch);

                    if(strcmp("to",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);
                        if(strcmp("Identifier",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);
                            if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                                printf("ERROR: '%s' hasn't been defined yet (line:%d)\n",istring,line1);

                            atama_yap(istring,atanmislar,variable_counter,counter,value,degerler);
                        }
                        else
                            printf("ERROR: Identifier name expected after 'to' keyword (line:%d)\n",line1);
                    }
                    else
                        printf("ERROR: There must be a 'to' keyword before number or identifier (line:%d)\n",line1);
                }
                else if(strcmp("Identifier",istring)==0)//move indentifier
                {
                    kelime_al(istring,wordlength,fileRLX,ch);

                    if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                        printf("ERROR: '%s' is not recognized (line:%d) \n",istring,line1);

                    value=getvalue(istring,atanmislar,value,degerler,variable_counter); //getvalue

                    kelime_al(istring,wordlength,fileRLX,ch);

                    if(strcmp("Keyword",istring)==0)
                        kelime_al(istring,wordlength,fileRLX,ch);


                    if(strcmp("to",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        if(strcmp("Identifier",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);
                            if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                                printf("ERROR: '%s' is not recognized (line:%d)\n",istring,line1);

                            atama_yap(istring,atanmislar,variable_counter,counter,value,degerler);
                        }
                        else
                            printf("ERROR: Variable name expected after 'to' keyword (line:%d) \n",line1);
                    }
                    else
                        printf("ERROR: Expecting 'to' keyword before number or variable (line:%d)\n",line1);
                }
                else
                    printf("ERROR: Number or variable has to come after 'move' command (line:%d)\n",line1);
            }
            ///LOOP
            if(strcmp("loop",istring)==0)
            {
                kelime_al(istring,wordlength,fileRLX,ch);

                if(strcmp("Identifier",istring)==0)
                {
                    kelime_al(istring,wordlength,fileRLX,ch);
                    looptype=1; //identifre
                    if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                        printf("ERROR: '%s' is not recognized. There must be an identifier or an int constant after 'loop' command (line:%d)\n",istring,line1);


                    value=getvalue(istring,atanmislar,value,degerler,variable_counter);

                    for(int i=0; i<variable_counter; i++)
                    {
                        if(strcmp(atanmislar[i],istring)==0)
                        {
                            degerlerindex=i;
                        }
                    }
                    //loop_times=value;
                    if(value <0) //negative loop value check
                    {
                        printf("ERROR: 'Loop' command does not work with negative values (line:%d)\n",line1);

                    }

                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);
                        if(strcmp("times",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);
                            if(strcmp("OpenBlock",istring)==0)
                            {
                                line1++;
                                block=1;
                                konum=ftell(fileRLX);

                            }
                            else if(strcmp("Keyword",istring)==0)
                            {
                                kelime_al(istring,wordlength,fileRLX,ch);
                                if(strcmp("out",istring)==0)
                                {
                                    kelime_al(istring,wordlength,fileRLX,ch);
                                    if(strcmp("StringConstants",istring)==0)
                                    {
                                        kelime_al(istring,wordlength,fileRLX,ch);

                                        for(int l=0; istring[l]!='\0'; l++)//get istring size
                                            wordlength++;

                                        if(degerler[degerlerindex] <0)//negative value check
                                        {
                                            printf("ERROR: 'Loop' command does not work with negative values (line:%d)\n",line1);
                                            return 0;
                                        }
                                        while(degerler[degerlerindex]>0) //string print after loop 'out'
                                        {
                                            for(int l=1; l<wordlength-1; l++)
                                            {
                                                printf("%c",istring[l]);
                                            }
                                            degerler[degerlerindex]--;
                                        }
                                        degerler[degerlerindex]=1;//The process of assigning a value of 1 after the loop ends
                                        wordlength=0;
                                    }
                                    else
                                    {
                                        printf("ERROR:Open parenthesis after 'times' for an operation (except printing a string) (line:%d)\n",line1);
                                        return 0;
                                    }
                                }
                                else
                                {
                                    printf("ERROR: Incorrect command usage (line:%d)\n");
                                    return 0;

                                }
                            }
                            else
                            {
                                printf("ERROR:'[]':open parenthesis after 'times' keyword (line:%d)\n",line1);
                                return 0;
                            }
                        }
                        else
                        {
                            printf("ERROR: 'times' keyword is missing (line:%d)\n",line1);
                            return 0;
                        }
                    }
                    else
                    {
                        printf("ERROR: Incorrect command usage. Waiting: 'times' (line:%d)\n",line1);
                        return 0;
                    }
                }
                else if(strcmp("IntConstant",istring)==0)
                {
                    kelime_al(istring,wordlength,fileRLX,ch);
                    value=atoi(istring);
                    if(value <0)
                    {
                        printf("ERROR: 'Loop' command does not work with negative values (line:%d)\n",line1);
                    }
                    looptype=2;
                    loop_times=value;
                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);
                        if(strcmp("times",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);
                            if(strcmp("OpenBlock",istring)==0)
                            {
                                block=1;
                                konum=ftell(fileRLX);
                            }
                            else if(strcmp("Keyword",istring)==0)
                            {
                                kelime_al(istring,wordlength,fileRLX,ch);
                                if(strcmp("out",istring)==0)
                                {
                                    kelime_al(istring,wordlength,fileRLX,ch);
                                    if(strcmp("StringConstants",istring)==0)
                                    {
                                        kelime_al(istring,wordlength,fileRLX,ch);

                                        for(int l=0; istring[l]!='\0'; l++)
                                            wordlength++;
                                        while(loop_times>0)
                                        {
                                            for(int l=1; l<wordlength-1; l++)
                                            {
                                                printf("%c",istring[l]);
                                            }
                                            loop_times--;
                                        }
                                        wordlength=0;
                                    }
                                    else
                                    {
                                        printf("ERROR: Open parenthesis after 'times' (line:%d)\n",line1);
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                printf("ERROR:'[]': Open parenthesis after 'times' (Without out string print) (line:%d)\n",line1);
                                return 0;
                            }
                        }
                        else
                        {
                            printf("ERROR: Missing 'times' keyword (line:%d)\n",line1);
                            return 0;
                        }
                    }
                    else
                    {
                        printf("ERROR: Incorrect command usage 'waiting: times' (line:%d)\n",line1);
                        return 0;
                    }
                }
                else
                {
                    printf("ERROR: 'loop' command must be followed by an identifier or an int constant (line:%d)\n",line1);
                    return 0;
                }

            }
            if(strcmp("CloseBlock",istring)==0)
            {
                line1++;
                if(block==0)
                    printf("ERROR: Block has not been opened before (line:%d)",line1);
                else
                    block=0;

                if(looptype==1)
                {
                    degerler[degerlerindex]--;
                    if(degerler[degerlerindex]>0)
                    {
                        fseek(fileRLX,konum,0); //Changing file read location for loop operation
                        loop_counter++;
                        block=1;
                    }
                    if(degerler[degerlerindex]==0)
                        degerler[degerlerindex]=1;

                }
                else if(looptype==2)
                {
                    loop_times--;
                    if(loop_times>0)
                    {
                        fseek(fileRLX,konum,0); //Changing file read location for loop operation
                        loop_counter++;
                        block=1;
                    }
                }
            }
            ///out
            if(strcmp("out",istring)==0)
            {
                int seperator_control=0;
                int print_counter=0;
                kelime_al(istring,wordlength,fileRLX,ch);

                while(1)
                {
                    if(strcmp("EndofLine",istring)==0)//work until the end of the line
                    {
                        line1++;
                        break;
                    }
                    if(strcmp("Keyword",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);
                    }
                    if(strcmp("Identifier",istring)==0) //print identifier value
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);
                        if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                            printf("ERROR: %s is not recognized line:%d\n",istring,line1);

                        value=getvalue(istring,atanmislar,value,degerler,variable_counter);

                        printf("%d",value);
                        print_counter++;
                    }
                    if(strcmp("newline",istring)==0)//print \n for newline commant
                    {
                        printf("\n");
                        print_counter++;
                    }
                    if(strcmp("StringConstants",istring)==0) //print string
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        for(int l=0; istring[l]!='\0'; l++)
                            wordlength++;
                        for(int l=1; l<wordlength-1; l++)
                        {
                            printf("%c",istring[l]);
                        }
                        print_counter++;
                        wordlength=0;
                    }
                    if(strcmp("IntConstant",istring)==0) //print number
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        value=atoi(istring);
                        printf("%d",value);
                        print_counter++;
                    }
                    if(strcmp("Separator",istring)==0) //separator check
                    {
                        seperator_control++;
                    }
                    kelime_al(istring,wordlength,fileRLX,ch);
                }//end out while
                if(print_counter-seperator_control!=1)//separator check
                {
                    printf("ERROR: Separate processes with commas\n");
                    return 0;
                }

            }//end out
            ///ADD
            if(strcmp("add",istring)==0)
            {
                kelime_al(istring,wordlength,fileRLX,ch);

                if(strcmp("IntConstant",istring)==0)
                {
                    kelime_al(istring,wordlength,fileRLX,ch);

                    value=atoi(istring);

                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                        kelime_al(istring,wordlength,fileRLX,ch);

                    if(strcmp("to",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        if(strcmp("Identifier",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);

                            if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                                printf("ERROR: %s is not recognized (line:%d)\n",istring,line1);

                            add_sub(istring,atanmislar,value,degerler,variable_counter,1); //processing method
                            //1 = add 0=sub

                        }
                        else
                            printf("ERROR: Missing an identifier after 'to' keyword (line:%d)\n",line1);
                    }
                    else
                        printf("ERROR: Missing 'to' keyword (line:%d)\n",line1);
                }
                else if(strcmp("Identifier",istring)==0)//move indentifre
                {
                    kelime_al(istring,wordlength,fileRLX,ch);

                    if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                        printf("ERROR: '%s' is not recognized (line:%d)\n",istring,line1);

                    value=getvalue(istring,atanmislar,value,degerler,variable_counter);

                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                        kelime_al(istring,wordlength,fileRLX,ch);

                    if(strcmp("to",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        if(strcmp("Identifier",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);

                            if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                                printf("ERROR: '%s' is not recognized (line:%d)\n",istring,line1);

                            add_sub(istring,atanmislar,value,degerler,variable_counter,1);

                        }
                        else
                            printf("ERROR: Missing an identifier after 'to' keyword (line:%d)\n",line1);
                    }
                    else
                        printf("ERROR: Missing 'to' keyword (line:%d)\n");
                }
                else
                    printf("ERROR: There must be a number or an identifier after 'add' command (line:%d)\n",line1);
            }
            ///SUB
            if(strcmp("sub",istring)==0)
            {
                kelime_al(istring,wordlength,fileRLX,ch);

                if(strcmp("IntConstant",istring)==0)
                {
                    kelime_al(istring,wordlength,fileRLX,ch);

                    value=atoi(istring);

                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                        kelime_al(istring,wordlength,fileRLX,ch);

                    if(strcmp("from",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        if(strcmp("Identifier",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);
                            if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                                printf("ERROR: '%s' is not recognized (line:%d)\n",istring,line1);

                            add_sub(istring,atanmislar,value,degerler,variable_counter,0);

                        }
                        else
                            printf("ERROR: Missing an identifier after 'from' keyword (line:%d)\n",line1);
                    }
                    else
                        printf("ERROR: Missing a 'from' keyword (line:%d)\n",line1);
                }
                else if(strcmp("Identifier",istring)==0)//move indentifre
                {

                    kelime_al(istring,wordlength,fileRLX,ch);

                    if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                        printf("ERROR: '%s' is not recognized (line:%d)\n",istring,line1);

                    value=getvalue(istring,atanmislar,value,degerler,variable_counter);

                    kelime_al(istring,wordlength,fileRLX,ch);
                    if(strcmp("Keyword",istring)==0)
                        kelime_al(istring,wordlength,fileRLX,ch);

                    if(strcmp("from",istring)==0)
                    {
                        kelime_al(istring,wordlength,fileRLX,ch);

                        if(strcmp("Identifier",istring)==0)
                        {
                            kelime_al(istring,wordlength,fileRLX,ch);

                            if( atama_kontrol(istring,atanmislar,variable_counter,counter)!=0)
                                printf("ERROR: '%s' is not recognized (line:%d)\n",istring,line1);

                            add_sub(istring,atanmislar,value,degerler,variable_counter,0);

                        }
                        else
                            printf("ERROR: Missing an identifier after 'to' keyword (line:%d)\n",line1);
                    }
                    else
                        printf("ERROR: Missing 'from' keyword (line:%d)\n",line1);
                }
                else
                    printf("ERROR: There must be a number or an identifier after 'sub' command (line:%d)\n",line1);
            }
        } //while
    }//if
    if(block==1)
        printf("ERROR: There is an unclosed block in the code\n"); //block check

    printf("\n");
    //for(int i=0; i<variable_counter; i++)
    //  printf("%s:>%d\n",atanmislar[i],degerler[i]);

    return 0;
}//main

void kelime_al(char istring[],int wordlength, FILE*fileRLX, char ch) //get word from .lx file
{
    memset(istring, 0, sizeof(istring));
    ch=fgetc(fileRLX);
    if(ch ==' '|| ch=='\n')//space, line break check
    {
        ch=fgetc(fileRLX);
    }
    wordlength=0;

    while(!(ch ==' '|| ch=='\n'))
    {
        istring[wordlength]=ch;
        wordlength++;
        if(ch =='"')//check for string word
        {
            ch=fgetc(fileRLX);
            istring[wordlength]=ch;
            wordlength++;
            while(ch!='"')
            {
                ch=fgetc(fileRLX);
                istring[wordlength]=ch;
                wordlength++;
            }
        }

        ch=fgetc(fileRLX);

    }
    istring[wordlength]='\0';
}
void add_sub(char istring[],char atanmislar[][20],int value,int degerler[],int variable_counter,int ADDorSUB) //Adding/subbing method
{
    if(ADDorSUB==1)
    {
        for(int i=0; i<variable_counter; i++)
        {
            if(strcmp(atanmislar[i],istring)==0)
            {
                degerler[i]=degerler[i]+value;
            }
        }
    }
    if(ADDorSUB==0)
    {
        for(int i=0; i<variable_counter; i++)
        {
            if(strcmp(atanmislar[i],istring)==0)
            {
                degerler[i]-=value;
            }
        }
    }
}
int getvalue(char istring[],char atanmislar[][20],int value,int degerler[],int variable_counter) //get value
{
    for(int i=0; i<variable_counter; i++)
    {
        if(strcmp(atanmislar[i],istring)==0)
        {
            value=degerler[i];
            return value;
        }
    }
}
void atama_yap(char istring[],char atanmislar[][20],int variable_counter,int sayac,int value,int degerler[])//make an assignment
{

    for(int i=0; i<variable_counter; i++)
    {
        if(strcmp(atanmislar[i],istring)==0)//if it returns 0, then that word is assigned before
        {
            degerler[i]=value;
        }
    }

}
int atama_kontrol(char istring[],char atanmislar[][20],int variable_counter,int sayac)//assignment control
{

    for(int i=0; i<variable_counter; i++)
    {
        if(strcmp(atanmislar[i],istring)==0)
        {
            return 0;//if taken before
        }
    }

    int i;
    for( i=0; istring[i]!='\0'; i++)//make an assignment
    {
        atanmislar[sayac][i]=istring[i];
    }
    atanmislar[sayac][i]='\0';
    return 1;
}



