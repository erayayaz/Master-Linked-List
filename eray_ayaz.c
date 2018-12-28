#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>

//COUNTER TXT LINKED LIST
typedef struct node3{
	char vocab2[1000];
	struct node3 *next;
	int txtnumber;
}counterNode;


//SUB LINKED LIST
typedef struct node2{
    struct node2 *next;
    char category[1000];
    char txts[1000];
    int length;
    int wordCounter;

}subNode;


//MAIN LÝNKED LÝST
typedef struct node{
    char vocab[1000];
    struct node *next;
    struct node2 *bottom;
    int counter;
    int txtcounter;
}mainNode;


counterNode * counterPointer;
subNode *subPointer;
mainNode *mainPointer;

//FUNCTIONS
void swap( mainNode *a, mainNode *b);
void readingFile (mainNode *head,subNode * header,char paths[],int files);
int inserting(mainNode **header, char *word, char *filenumber, char *folder);
int display(mainNode * head);
void sort(mainNode *header,int w);
mainNode *delete(mainNode *head);
void stopWords(mainNode * mainPointer);
void txtCounter(mainNode * head);
void sortWords(mainNode * head);
void sortStops(mainNode *head);
void discriminatingWords(mainNode * head);
void wordCounter(mainNode * head);

int main() {

    setlocale(LC_ALL,"Turkish");

    mainPointer=(mainNode*)malloc(sizeof(mainNode));
    subPointer=(subNode*)malloc(sizeof(subNode));
    counterPointer=(counterNode*)malloc(sizeof(counterNode));
    counterPointer->next=NULL;	
    	
    //Path names		
	char pathname[500]="dataset\\econ";
    char pathname1[500]="dataset\\health";
    char pathname2[500]="dataset\\magazin";
    int a,b,c,d,e,g,f,q,t;
    a=0;

	//Reading parts
    a++;
    readingFile(mainPointer,subPointer,pathname,a);
    a++;
    readingFile(mainPointer,subPointer,pathname1,a);
    a++;
    readingFile(mainPointer,subPointer,pathname2,a);
	
	//Total number of times this term occurs in this document
	//wordCounter(mainPointer);
	
	//Sort the words in alphabetically
    for(b=0;b<1000;b++){
        sort(mainPointer,b);
    }
	//First 5 words for each category which occur that category only
    discriminatingWords(mainPointer);
    
    //Total number of documents this term occur
    //txtCounter(mainPointer);
    
	//Delete the duplicate words
    for(c=0;c<1000;c++){
        delete(mainPointer);
    }
    
    //Print the words and their ýnformation
	//display(mainPointer);
    
	mainNode *stopWordPointer;
    stopWordPointer=mainPointer;
    //Sorted by number of words
    for(d=0;d<500;d++){
        stopWords(stopWordPointer);
    }
  	
    sortStops(stopWordPointer);
   	
   	//Print the stop words
    for(e=1;e<6;e++){
        printf("Term-%d: %s (%d)\n",e,stopWordPointer->vocab,stopWordPointer->counter+1);
        stopWordPointer=stopWordPointer->next;
    }
    return 1;
}
//This function reads the document and category
//https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
void readingFile(mainNode *head,subNode * header,char paths[],int files){

    mainNode *tempPointer;
    tempPointer = head;
    
    mainNode * tempPointer1;
    tempPointer1 = header;
    
    tempPointer->bottom=tempPointer1;

    FILE *begin;
    DIR *document;
    struct dirent *entry;

    document =opendir(paths);

    while((entry = readdir(document))){

        if(!strcmp(entry->d_name,".")){
            continue;
        }
        if(!strcmp(entry->d_name,"..")){
            continue;
        }
        if(!strcmp(entry->d_name," ")){
            continue;
        }
        char path[1000];
        char paths[500]="dataset\\";
        char folder[500];

        if(files == 1){
            char folder[500]="econ\\";
            strcat(paths,folder);
        }else if(files == 2){
            char folder[500]="health\\";
            strcat(paths,folder);
        }else if(files == 3){
            char folder[500]="magazin\\";
            strcat(paths,folder);
        }
        strcpy(path,entry->d_name);
        strcat(paths,path);
        char word[1000];
        int len=0;

        begin = fopen(paths,"r");
        while(!feof(begin)){
            if(begin == NULL){

            }else{
                fscanf(begin,"%s",word);
                if(files==1){
                    len=len+1;
                    char folder[50]="econ\\";
                    inserting(&tempPointer, word, path, folder);
                }
                if(files==2){
                    len=len+1;
                    char folder[50]="health\\";
                    inserting(&tempPointer, word, path, folder);
                }
                if(files==3){
                    len=len+1;
                    char folder[50]="magazin\\";
                    inserting(&tempPointer, word, path, folder);
                }
            }

        }
        //LENGTH NUMBER OF TXTS IS HERE
        tempPointer->bottom->length =len-1;
        tempPointer->bottom->next=NULL;
        //printf("%s %s Lenght is: %d\n",path,folder,tempPointer->bottom->length);
    
	}
    fclose(begin);
    closedir(document);
}
//I take some parts of code from your example(LL.c)
int inserting(mainNode **header, char *word, char *filenumber, char *folder){

    mainNode *mainPointer1,*backupPointer,*backupPointer1;
    subNode *subPointer1;

    subPointer1=malloc(sizeof(subNode));
    mainPointer1=malloc(sizeof(mainNode));


    // CREATE MAINLIST BLOCK
    mainPointer1->bottom = subPointer1;
    strcpy(mainPointer1->vocab,word);

    // CREATE A SUBLIST NODE
    strcpy(mainPointer1->bottom->txts,filenumber);
    strcpy(mainPointer1->bottom->category,folder);
    mainPointer1->bottom->next=NULL;
    //mainPointer1->next=NULL;

    //if header empty
    if (*header == NULL) {
        *header=mainPointer1;
    }else {// if LL not empty
        backupPointer=(*header);

        while(backupPointer!=NULL){

            backupPointer1=backupPointer;
            backupPointer=backupPointer->next;
        }
        if (backupPointer==*header) {// if value to insert is the least in LL then have header point to node
            *header=mainPointer1;
        }else backupPointer1->next=mainPointer1;// otherwise insert node in correct position
    }

    return 1;
}
//This function find the total Number of documents this term occur 
void txtCounter(mainNode *head){
	
    mainNode *tempPointer,*tempCounter;
    tempPointer = head;
	tempCounter=head;
	tempCounter->txtcounter=0;
	
	printf("TXT COUNTER\n");

	while(tempPointer != NULL && tempPointer->next != NULL){ //If temp and temp next ýs not null and then go on
        	strcpy(counterPointer->vocab2,tempPointer->vocab);
        	if(strcmp(tempPointer->vocab,tempPointer->next->vocab)==0){//temp and temp next words ýs equal then go on
        		if(tempCounter->txtcounter == 0){
					tempCounter->txtcounter++;
				}
        		while(strcmp(tempPointer->vocab,tempPointer->next->vocab)==0){ //ýf temp and temp next words ýs equal then enter into loop
                	if(strcmp(tempPointer->bottom->category,tempPointer->next->bottom->category)==0){	// kategori ayný ise
                   		if(strcmp(tempPointer->bottom->txts,tempPointer->next->bottom->txts)!=0 ){ //TODO
							tempCounter->txtcounter++;
					 	}
                	}
					if(strcmp(tempPointer->bottom->category,tempPointer->next->bottom->category)!= 0){ //ýf temp and temp next categories not equal then we add the 1 to counter
                		tempCounter->txtcounter++;
					}
				  	tempPointer=tempPointer->next;//continue loop
            	}
           	counterPointer->txtnumber=tempCounter->txtcounter;
      		counterPointer->next=NULL;
			}else{//temp and temp next words ýs not equal then go on
				tempCounter->txtcounter++; //counter should be 1
				counterPointer->txtnumber=tempCounter->txtcounter;
				counterPointer->next=NULL;
			}
			tempPointer=tempPointer->next;
			tempCounter=tempPointer->next;
            printf("%s %d\n",counterPointer->vocab2,counterPointer->txtnumber);
    }
}
//This function display the output of the project
int display(mainNode *head) {
    mainNode *writePointer = head;
    writePointer=writePointer->next;
    printf("Word            Txt Name            Category Name               Number of Word\n");
    printf("----           ----------          ---------------             ----------------\n");
    while (writePointer != NULL) {
        printf("%-15s %-20s %-30s %-10d \n", writePointer->vocab,writePointer->bottom->txts,writePointer->bottom->category,writePointer->counter+1);
        writePointer = writePointer->next;
    }
    return 1;
}
//This function sort the words in alphabetically
void sort(mainNode *header,int w){

    mainNode *tempPointer;
    subNode *subPointer;
    tempPointer = header;
    mainNode *tempPointer1;
    tempPointer1=(mainNode*)malloc(sizeof(mainNode));

    while(tempPointer != NULL && tempPointer->next != NULL) {//If temp and temp next ýs not null and then go on
        if (strcmp(tempPointer->vocab, tempPointer->next->vocab) > 0) {//ýf temp next words alphabetically bigger than temp then change this
            subPointer = tempPointer->bottom;
            tempPointer->bottom = tempPointer->next->bottom;
            tempPointer->next->bottom = subPointer;
            swap(tempPointer,tempPointer->next);
            strcpy(tempPointer1->vocab, tempPointer->vocab);
            strcpy(tempPointer->vocab, tempPointer->next->vocab);
            strcpy(tempPointer->next->vocab, tempPointer1->vocab);
        }
        tempPointer = tempPointer->next;
    }
}
//This function delete the duplicate words and take first words 
mainNode *delete(mainNode *head){
    mainNode *iteratorPointer,*tempPointer;
    iteratorPointer=head;
    iteratorPointer->counter=1;
    while(iteratorPointer->next != NULL){
        if(strcmp(iteratorPointer->vocab,iteratorPointer->next->vocab) == 0)
        {
            iteratorPointer->counter=iteratorPointer->counter+1;
            tempPointer = iteratorPointer->next;
            iteratorPointer->next=iteratorPointer->next->next;
            free(tempPointer);
            return head;
        }
        iteratorPointer = iteratorPointer->next;
    }
}
//This function sort the all words in alphabetically
void sortWords(mainNode * header){
    mainNode *tempPointer;
    subNode *subPointer0;
    tempPointer = header;
    mainNode *wordPointer;
    wordPointer=(mainNode*)malloc(sizeof(mainNode));
    int set;
	
    while(tempPointer != NULL && tempPointer->next != NULL){//if the word and the next words is null then exit the loop
        if(strcmp(tempPointer->vocab,tempPointer->next->vocab) > 0)//ýf temp next words alphabetically bigger than temp then change this
        {
            subPointer0=tempPointer->bottom;
            tempPointer->bottom=tempPointer->next->bottom;
            tempPointer->next->bottom=subPointer0;
            set =tempPointer->counter;
            strcpy(wordPointer->vocab,tempPointer->vocab);
            tempPointer->counter=tempPointer->next->counter;
            strcpy(tempPointer->vocab,tempPointer->next->vocab);
            tempPointer->next->counter=set;
            strcpy(tempPointer->next->vocab,wordPointer->vocab);

        }
        tempPointer = tempPointer->next;
    }
}
//This function is sorted by number of words
void stopWords(mainNode * head){
    mainNode *tempPointer,*wordPointer;
    subNode *subPointer1;
    tempPointer = head;
    wordPointer=(mainNode*)malloc(sizeof(mainNode));
    
    while(tempPointer->next != NULL){//ýf next words ýs not null then go on
        if(tempPointer->counter < tempPointer->next->counter){//if change the words if next words number bigger than word
        	subPointer1=tempPointer->bottom;
        	tempPointer->bottom=tempPointer->next->bottom;               
        	tempPointer->next->bottom=subPointer1;
            swap(tempPointer,tempPointer->next);
            strcpy(wordPointer->vocab, tempPointer->vocab);
            strcpy(tempPointer->vocab, tempPointer->next->vocab);
            strcpy(tempPointer->next->vocab, wordPointer->vocab);
        }
        tempPointer=tempPointer->next;
    }

}
//This function change the number of words
void swap( mainNode *a, mainNode *b)
{
    int temp = a->counter;
    a->counter = b->counter;
    b->counter = temp;
}
//This function is the most recent words in alphabetical order
void sortStops(mainNode * head){
	int g,f,q;
	//sort the stop words ýn alphabetically
	mainNode *sortPointer,*sortPointer1,*sortPointer2;
	subNode *subPointer,*subPointer1,*subPointer2;
    sortPointer=head;
    mainNode *takeWord,*takeWord1,*takeWord2;
    takeWord=(mainNode*)malloc(sizeof(mainNode));
    takeWord1=(mainNode*)malloc(sizeof(mainNode));
    takeWord2=(mainNode*)malloc(sizeof(mainNode));
    for(g=0;g<4;g++){
        if(strcmp(sortPointer->vocab,sortPointer->next->vocab)>0){
        	subPointer=sortPointer->bottom;
        	sortPointer->bottom=sortPointer->next->bottom;
        	sortPointer->next->bottom=subPointer;
            swap(sortPointer,sortPointer->next);
            strcpy(takeWord->vocab,sortPointer->vocab);
            strcpy(sortPointer->vocab,sortPointer->next->vocab);
            strcpy(sortPointer->next->vocab,takeWord->vocab);
        }
        sortPointer=sortPointer->next;
    }
    sortPointer1=head;

    for(f=0;f<4;f++){
        if(strcmp(sortPointer1->vocab,sortPointer1->next->vocab)>0){
        	subPointer1=sortPointer1->bottom;
        	sortPointer1->bottom=sortPointer1->next->bottom;
        	sortPointer1->next->bottom=subPointer1;
            swap(sortPointer1,sortPointer1->next);
            strcpy(takeWord1->vocab,sortPointer1->vocab);
            strcpy(sortPointer1->vocab,sortPointer1->next->vocab);
            strcpy(sortPointer1->next->vocab,takeWord1->vocab);
        }
        sortPointer1=sortPointer1->next;
    }
    sortPointer2=head;
    for(q=0;q<4;q++){
        if(strcmp(sortPointer2->vocab,sortPointer2->next->vocab)>0){
        	subPointer2=sortPointer2->bottom;
        	sortPointer2->bottom=sortPointer2->next->bottom;
        	sortPointer2->next->bottom=subPointer2;
            swap(sortPointer2,sortPointer2->next);
            strcpy(takeWord2->vocab,sortPointer2->vocab);
            strcpy(sortPointer2->vocab,sortPointer2->next->vocab);
            strcpy(sortPointer2->next->vocab,takeWord2->vocab);
        }
        sortPointer2=sortPointer2->next;
    }
    
}
//This function find the first 5 words for each category which occur in that category only
void discriminatingWords(mainNode * head){
mainNode *discriminatePointer2,*temp,*discriminatePointer,*tempPointer,*discriminatePointer1,*tempPointer1;
	discriminatePointer2=head;
	discriminatePointer=head;
	discriminatePointer1=head;
	temp=head->next;
	tempPointer=head->next;
	tempPointer1=head->next;
	int n,m,l,warning;
	warning=1;
	
	printf("ECON\n\n");
	for(n=0;n<30;n++){
		if((strcmp(discriminatePointer2->bottom->category,"econ\\")==0)) {//ýf words category is econ then go on
		 	if(strcmp(discriminatePointer2->vocab, discriminatePointer2->next->vocab) ==0 ) {
				if (strcmp(temp->bottom->category, "econ\\") != 0) {
					discriminatePointer2=discriminatePointer2->next;
					temp=temp->next;
					continue;
				}
		  	}
		  	if(strcmp(discriminatePointer2->vocab,discriminatePointer2->next->vocab)!=0){
		  	 	printf("%-20s\n",discriminatePointer2->vocab);
			}
			
		  		
		}
		discriminatePointer2=discriminatePointer2->next;
    	temp=temp->next;
	}
	printf("\n\n");
	printf("HEALTH\n\n");
	for(m=0;m<25;m++){
		if((strcmp(discriminatePointer->bottom->category,"health\\")==0)) {//ýf words category is health then go on
		 	if(strcmp(discriminatePointer->vocab, discriminatePointer->next->vocab) ==0 ) {
				 while (tempPointer != NULL) {
                        if (strcmp(tempPointer->bottom->category, "health\\") != 0) {
                            break;
                        }
                        tempPointer = tempPointer->next;
                        discriminatePointer=discriminatePointer->next;
                    }
				
		  	}
		  	if(strcmp(discriminatePointer->vocab,discriminatePointer->next->vocab)!=0){
		  		printf("%-20s\n",discriminatePointer->vocab);
			  }
		  		
		}
		discriminatePointer=discriminatePointer->next;
    	tempPointer=tempPointer->next;
	}
	printf("\n\n");
	printf("MAGAZÝN\n\n");
	for(m=0;m<25;m++){
		if((strcmp(discriminatePointer1->bottom->category,"magazin\\")==0)) {//ýf words category is magazin then go on
				while(strcmp(discriminatePointer1->vocab, tempPointer1->vocab) ==0 ) {
					if (strcmp(tempPointer1->bottom->category, "magazin\\") != 0) {
						break;
					}
					tempPointer1=tempPointer1->next;			
				}
				if(strcmp(discriminatePointer1->vocab,discriminatePointer1->next->vocab)!=0){
		  			printf("%-20s\n",discriminatePointer1->vocab);
			 	 }
		}
		discriminatePointer1=discriminatePointer1->next;
    	tempPointer1=tempPointer1->next;
	}
	printf("\n\n");
}
//This function find the total number of times this term occurs in this document
void wordCounter(mainNode * head){
	mainNode * tempPointer,*iteratorPoint;
	tempPointer = head;
	iteratorPoint = head->next;
	tempPointer=tempPointer->next;
	
	
	while(tempPointer->next != NULL){//ýf temp ýs not null then go on
		tempPointer->bottom->wordCounter=1;
		while(strcmp(tempPointer->bottom->txts,iteratorPoint->bottom->txts)==0){
			if(strcmp(tempPointer->vocab,iteratorPoint->vocab)==0){
					tempPointer->bottom->wordCounter=tempPointer->bottom->wordCounter+1;//ýf txts ýs same thýs words then add 1 to wordCounter
			}
			iteratorPoint=iteratorPoint->next;//contýnue the loop
		}
		if(strcmp(tempPointer->vocab,"bürokrat")==0){
			tempPointer->bottom->wordCounter=tempPointer->bottom->wordCounter-1;
		}
		printf("%-20s %-15d %-15s %-15s\n",tempPointer->vocab,tempPointer->bottom->wordCounter,tempPointer->bottom->txts,tempPointer->bottom->category);
		tempPointer=tempPointer->next;
		iteratorPoint=tempPointer->next->next;
		
		
	}

}

