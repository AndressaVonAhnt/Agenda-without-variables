/*
    Agenda without variables
    Author: Andressa Von Ahnt
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_STRING 50
#define SIZE_FIX_BUFFER ((3 * sizeof(int)) + (SIZE_STRING * sizeof(char)))

// Global Variables
void *pOffset, *pNumPeople, *pAuxiliary1, *pAuxiliary2;

// Start Buffer
void startBuffer(void **pBuffer){
    *pBuffer = (void *)malloc(SIZE_FIX_BUFFER);

    if(*pBuffer == NULL){
        printf("Error allocating memory");
        exit(1);
    }

    *(int *)*pBuffer = (SIZE_FIX_BUFFER); // OFFSET
    *((int *)*pBuffer + 1) = 0; // NUMBER PEOPLE
    *((int *)*pBuffer + 2) = 0; // AUXILIARY 1
    *((char *)*pBuffer + 3) = '\0'; // AUXILIARY 2
}

// Define Pointers
void pointers(void **pBuffer){
    pOffset = (int *)*pBuffer; 
    pNumPeople = ((int *)*pBuffer + 1); 
    pAuxiliary1 = ((int *)*pBuffer + 2);
    pAuxiliary2 = ((int *)*pBuffer + 3);
}

void FlushStdin() {
    while(getchar() != '\n');
}

void addPerson(void **pBuffer){
    char *person;

    //name
    getchar();
    printf("Enter name: ");
    fgets((char *)pAuxiliary2, SIZE_STRING, stdin);
    char *pName = strchr((char *)pAuxiliary2, '\n');
    if (pName) 
        *pName = '\0';
    else{
        *((char *)*pBuffer + 3 + SIZE_STRING) = '\0';
        FlushStdin();
    }

    *(int *)pAuxiliary1 = strlen((char *)pAuxiliary2) + sizeof(char) + *(int *)pOffset;

    *pBuffer = realloc(*pBuffer, *(int *)pAuxiliary1);
    if (*pBuffer == NULL) {
        printf("Error reallocating memory\n");
        exit(1); 
    }

    pointers(pBuffer);
    person = (char *)*pBuffer + *(int *)pOffset;
    strncpy(person, (char *)pAuxiliary2, strlen((char *)pAuxiliary2) + sizeof(char));

    *(int *)pOffset = *(int *)pAuxiliary1;

    //age
    printf("Enter age: ");
    scanf("%d", (int *)pAuxiliary2);

    *(int *)pAuxiliary1 = sizeof(int) + *(int *)pOffset;

    *pBuffer = realloc(*pBuffer, *(int *)pAuxiliary1);
    if (*pBuffer == NULL) {
        printf("Error reallocating memory\n");
        exit(1); 
    }

    pointers(pBuffer);
    person = (char *)*pBuffer + *(int *)pOffset;
    *(int *)(person) = *(int *)pAuxiliary2;

    *(int *)pOffset = *(int *)pAuxiliary1;
    
    //email
    getchar();
    printf("Enter email: ");
    fgets((char *)pAuxiliary2, SIZE_STRING, stdin);
    char *pEmail = strchr((char *)pAuxiliary2, '\n');
    if (pEmail) 
        *pEmail = '\0';
    else{
        *((char *)*pBuffer + 3 + SIZE_STRING) = '\0';
        FlushStdin();
    }
    
    *(int *)pAuxiliary1 = strlen((char *)pAuxiliary2) + sizeof(char) + *(int *)pOffset;

    *pBuffer = realloc(*pBuffer, *(int *)pAuxiliary1);
    if (*pBuffer == NULL) {
        printf("Error reallocating memory\n");
        exit(1); 
    }

    pointers(pBuffer);
    person = (char *)*pBuffer + *(int *)pOffset;
    strncpy(person, (char *)pAuxiliary2, strlen((char *)pAuxiliary2) + sizeof(char));

    *(int *)pOffset = *(int *)pAuxiliary1;
}

void *printPerson(void *aux){
    printf("\nName: %s", (char *)aux);
    aux = ((char *)aux + strlen((char *)aux) + 1);

    printf("\nAge: %d\n", *(int *)aux);
    aux = ((int *)aux + 1);

    printf("Email: %s\n", (char *)aux);
    aux = (void *)((char *)aux + strlen((char *)aux) + 1);

    return aux;
}

void listPeople(void **pBuffer) {
    if(*(int *)pOffset == SIZE_FIX_BUFFER){
        printf("\n>>>>>>>> Empty agenda <<<<<<<<\n");
        return;
    }

    void *aux = (char *)*pBuffer + SIZE_FIX_BUFFER;
    *(int *)pAuxiliary1 = 1;

    printf("\n----------- AGENDA -----------\n");
    while(*(int *)pAuxiliary1 <= *(int *)pNumPeople){
        printf("\nPerson %d", *(int *)pAuxiliary1);
        aux = printPerson(aux);
        (*(int *)pAuxiliary1)++;
    }
    printf("\n------------------------------\n");
}

void *searchPerson(void **pBuffer){
    if(*(int *)pOffset == SIZE_FIX_BUFFER){
        printf("\n>>>>>>>> Empty agenda <<<<<<<<\n");
        return NULL;
    }
 
    getchar();
    printf("\nEnter name: ");
    fgets((char *)pAuxiliary2, SIZE_STRING, stdin);
    char *pName = strchr((char *)pAuxiliary2, '\n');
    if (pName) 
        *pName = '\0';
    else{
        *((char *)*pBuffer + 3 + SIZE_STRING) = '\0';
        FlushStdin();
    }

    void *aux = (char *)*pBuffer + SIZE_FIX_BUFFER;
    *(int *)pAuxiliary1 = SIZE_FIX_BUFFER;

    while (*(int *)pAuxiliary1 < *(int *)pOffset){
        if(strcmp(aux, (char *)pAuxiliary2) == 0)
            return aux;
        
        *(int *)pAuxiliary1 += strlen((char *)aux) + sizeof(int) + sizeof(char);

        aux += strlen((char *)aux) + 1;
        aux += sizeof(int);
        *(int *)pAuxiliary1 += strlen((char *)aux) + sizeof(char);
        aux += strlen((char *)aux) + 1;
    }

    printf("\n------- NAME NOT FOUND -------\n");
    return NULL;
}

void deletePerson(void **pBuffer){
    void *p = searchPerson(pBuffer);
    if (p == NULL) 
        return; 

    void *nextPerson = p;
    void *startList = (char *)*pBuffer + SIZE_FIX_BUFFER;

    *(int *)pAuxiliary2 = 0;

    while(startList != p){
        *(int *)pAuxiliary2 += strlen((char *)startList) + 1;
        startList = (char *)startList + strlen((char *)startList) + 1;
        *(int *)pAuxiliary2 += sizeof(int);
        startList = (char *)startList + sizeof(int);
        *(int *)pAuxiliary2 += strlen((char *)startList) + 1;
        startList = (char *)startList + strlen((char *)startList) + 1;
    }

    *(int *)pAuxiliary1 = strlen((char *)nextPerson) + 1;
    nextPerson = (char *)nextPerson + strlen((char *)nextPerson) + 1;
    *(int *)pAuxiliary1 += sizeof(int);
    nextPerson = (char *)nextPerson + sizeof(int); 
    *(int *)pAuxiliary1 += strlen((char *)nextPerson) + 1; 
    nextPerson = (char *)nextPerson + strlen((char *)nextPerson) + 1;

    *(int *)pAuxiliary2 = *(int *)pOffset - (SIZE_FIX_BUFFER + *(int *)pAuxiliary1 + *(int *)pAuxiliary2);
    if(*(int *)pAuxiliary2 != 0)
        memmove(p, nextPerson, *(int *)pAuxiliary2);

    *(int *)pOffset -= *(int *)pAuxiliary1;

    *pBuffer = realloc(*pBuffer, *(int *)pOffset);
    if (*pBuffer == NULL) {
        printf("Error reallocating memory\n");
        exit(1); 
    }
    pointers(pBuffer);
    (*(int *)pNumPeople)--;
    printf("\n>>>>>>> PERSON DELETED <<<<<<<\n");
}

int main(){
    void *pBuffer;
    void *p;

    startBuffer(&pBuffer); 
    pointers(&pBuffer);

    while(1){
    printf("\n------------ MENU ------------\n");
    printf("1. Add Person\n");
    printf("2. Delete Person\n");
    printf("3. Search Person\n");
    printf("4. List all\n");
    printf("5. Exit\n");
    printf("Enter your option: ");
    scanf("%d", (int *)pAuxiliary1);

        switch (*(int *)pAuxiliary1){
            case 1:
            (*(int *)pNumPeople)++;
            printf("\nPerson %d\n", *(int *)pNumPeople);
            addPerson(&pBuffer);
            printf("\n>>>>>>>> PERSON ADDED <<<<<<<<\n");
            break;
            
            case 2:
            deletePerson(&pBuffer);
            break;
            
            case 3:
            p = searchPerson(&pBuffer);
            if(p){
                printf("\n-------- PERSON FOUND --------\n");
                printPerson(p);
                printf("\n------------------------------\n");
            }
            break;

            case 4:
            listPeople(&pBuffer);
            break;
            
            case 5:
            free(pBuffer);
            exit(1);
            break;

            default:
            printf("\nInvalid option. Try again!\n");
            break;
        }
    }
    return 0;
}