#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>

#define MAX_FIRST_LAST_SIZE 31
#define TOTAL_CONTACTS 2
#define MAX_NUMBER_LEN 15
#define FAIL -1
#define ROW_TITLE_OFFSET 8

struct Contact{
    char fname[MAX_FIRST_LAST_SIZE];
    char lname[MAX_FIRST_LAST_SIZE];
    char telep[MAX_NUMBER_LEN];
    char cellp[MAX_NUMBER_LEN];
};

struct ContactNode{
    struct ContactNode * _next;
    struct Contact * _data;
};

struct ContactNodeHeads{
    struct ContactNode * _sortFnameHead, * _sortLnameHead , * _sortTelHead, * _sortCellHead;
    int _contactsNo;
};
// not global!!
struct Contact A[TOTAL_CONTACTS];

char * get_and_check_line();
void getline_and_copy_string(char* dest, size_t max_len);
char get_char_only();
void print_titles(int y_offset);
void print_list(int y_offset, const struct ContactNode * contactNodeHead);
int compByFname(const struct Contact* firstContact,const struct Contact* secondContact);
int compByLname(const struct Contact* firstContact,const struct Contact* secondContact);
int compByTel(const struct Contact* firstContact,const struct Contact* secondContact);
int compByCell(const struct Contact* firstContact,const struct Contact* secondContact);
struct Contact* createContact();
struct ContactNode* createContactNode(const struct Contact* newContact);
void addToLinkedLists(struct ContactNodeHeads * myContacts, struct Contact* newContact);
void insertContactToLists(struct ContactNode** contactNode, const struct Contact* newContact,
                   int (*compFunc)(const struct Contact*, const struct Contact*));
void insert(struct ContactNodeHeads* myContacts);
void clearContactsLists(struct ContactNodeHeads* myContacts);
void clearContacts(struct ContactNode *contactNodeHead);
void clearContactsNodes(struct ContactNode **p2contactNodeHead);
void delete(struct ContactNodeHeads*);
void deleteContact(struct ContactNodeHeads* myContacts, struct Contact* delContact);
void deleteContactNodesFromList(struct ContactNode **p2contactNodeHead, struct Contact* delContact);
void edit(struct ContactNodeHeads* myContacts);
void search(const struct ContactNodeHeads* myContacts);
void showSort(const struct ContactNodeHeads* myContacts);
void showContactList(const struct ContactNode * contactNodeHead);
//void list(int y_offset);


void clrscr(void);
void gotoxy(int x, int y);
void getCursor(int* x, int* y);

//void delet(void);
//void edit(void);

void searchf(const struct ContactNode * contactNodeHead);
void searchl(const struct ContactNode * contactNodeHead);
void searchp(const struct ContactNode * contactNodeHead);
void searchc(const struct ContactNode * contactNodeHead);

void sortf(void);
void sortl(void);
void sortp(void);
void sortc(void);
void help(void);


int main()
{
    /**
    char name[5];
    getline_and_copy_string(name, 5);
    printf("%s",name);
    /**/
    struct ContactNodeHeads myContacts = {NULL,NULL,NULL,NULL, 0};
    /**/
    bool run = true;
    char n;
    while(run) {
        clrscr();
        printf("\n|Phone Book12<::>Home|\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Welcome to PHONE BOOK2022!\nYou have inserted ( %d ) contacts.\n\n",myContacts._contactsNo);
        printf("\t[1] |--> Insert\n");
        printf("\t[2] |--> Delete\n");
        printf("\t[3] |--> Edit\n");
        printf("\t[4] |--> Search\n");
        printf("\t[5] |--> Show All\n");
        printf("\t[6] |--> Sort\n");
        printf("\t[7] |--> Help\n");
        printf("\t[8] |--> Exit\n");
        printf("\n\tPlease Enter Your Choice (1-8): ");
        n = get_char_only();
        // we should empty buffer here
        switch(n) {
            case '1':
                insert(&myContacts);
                break;
            case '2':
                delete(&myContacts);
                break;
            case '3':
                edit(&myContacts);
                break;
            case '4':
                search(&myContacts);
                break;
            case '5':
                clrscr();
                showContactList(myContacts._sortFnameHead);
                break;
            case '6':
                showSort(&myContacts);
                break;
            case '7':
                help();
                break;
            case '8':
                // exit(1);
                run = false;
                break;
            default:
                printf("\nThere is no item with symbol \"%c\".Please enter a number between 1-8!\nPress any key to continue...",n);
                get_char_only();
                break;
        }//End of swicth
    }
    clearContactsLists(&myContacts);
    return 0;
    /**/
}//End of main function!

//void insert(void)
//{
//    char ans='y';
//    clrscr();
//    printf("\nPhone Book12<::>Insert Contacts");
//    printf("\n--------------------------------------------------------------------------------");
//
//
//    while(ans=='y'){
//        if(last == TOTAL_CONTACTS) {
//            printf("Too much contacts. The limit is %d.\n", TOTAL_CONTACTS);
//            break;
//        }
//        else{
//            printf("\n\nData of Contact %2.2d{\n",last+1);
//            printf("\n\t  1-F.Name: ");
//            getline_and_copy_string(A[last].fname, MAX_FIRST_LAST_SIZE);
//
//            printf("\t  2-L.Name: ");
//            getline_and_copy_string(A[last].lname, MAX_FIRST_LAST_SIZE);
//
//            printf("\t  3-Tele.P: ");
//            getline_and_copy_string(A[last].telep, MAX_NUMBER_LEN);
//
//            printf("\t  4-Cell.P: ");
//            getline_and_copy_string(A[last].cellp, MAX_NUMBER_LEN);
//
//            printf("\n|-->Data Recorded!}");
//            printf("\n\t\t\tNext Contact?(y/n) Answer:");
//            ans = get_char_only();
//            last++;
//        }
//    }
//    printf("\n\nYou have inserted ( %d ) contact!\nPress a key to return main page & continue program|-->",last);
//    get_char_only();
//}
void insert (struct ContactNodeHeads* myContacts){
    char ans='y';
    clrscr();
    printf("\nPhone Book12<::>Insert Contacts");
    printf("\n--------------------------------------------------------------------------------");
    struct Contact* newContact = NULL;

    while(ans=='y'){
        printf("\n\nData of Contact %2.2d{\n",myContacts->_contactsNo);
        newContact = createContact();
        addToLinkedLists(myContacts, newContact);
        myContacts->_contactsNo++;
        printf("\n|-->Data Recorded!}");
        printf("\n\t\t\tNext Contact?(y/n) Answer:");
        ans = get_char_only();
    }
}

struct Contact* createContact(){
    struct Contact* newContact = (struct Contact *) malloc(sizeof(struct Contact));
    if (!newContact){
        printf("Fail to create new contact\n");
        exit (EXIT_FAILURE);
    }

    printf("\n\t  1-F.Name: ");
    getline_and_copy_string(newContact->fname, MAX_FIRST_LAST_SIZE);

    printf("\t  2-L.Name: ");
    getline_and_copy_string(newContact->lname, MAX_FIRST_LAST_SIZE);

    printf("\t  3-Tele.P: ");
    getline_and_copy_string(newContact->telep, MAX_NUMBER_LEN);

    printf("\t  4-Cell.P: ");
    getline_and_copy_string(newContact->cellp, MAX_NUMBER_LEN);
    
    return newContact;
}

struct ContactNode* createContactNode(const struct Contact* newContact){
    struct ContactNode * contactNode = (struct ContactNode *) malloc(sizeof(struct ContactNode));
    if (!contactNode){
        printf("Fail to create new contact\n");
        exit (EXIT_FAILURE);
    }
    contactNode->_data = (struct Contact*)newContact;
    contactNode->_next = NULL;
    return contactNode;
}
void addToLinkedLists(struct ContactNodeHeads * myContacts, struct Contact* newContact){
    insertContactToLists(&(myContacts->_sortFnameHead), newContact, compByFname);
    insertContactToLists(&(myContacts->_sortLnameHead), newContact, compByLname);
    insertContactToLists(&(myContacts->_sortTelHead), newContact, compByTel);
    insertContactToLists(&(myContacts->_sortCellHead), newContact, compByCell);
}
int compByFname(const struct Contact* firstContact,const struct Contact* secondContact){
    return strcmp(firstContact->fname, secondContact->fname);
}
int compByLname(const struct Contact* firstContact,const struct Contact* secondContact){
    return strcmp(firstContact->lname, secondContact->lname);
}
int compByTel(const struct Contact* firstContact, const struct Contact* secondContact){
    return strcmp(firstContact->telep, secondContact->telep);
}
int compByCell(const struct Contact* firstContact, const struct Contact* secondContact){
    return strcmp(firstContact->cellp, secondContact->cellp);
}

void insertContactToLists(struct ContactNode** p2contactNodeHead, const struct Contact* newContact,
                   int (*compFunc)(const struct Contact*,const struct Contact*)){
    struct ContactNode * newContactNode = createContactNode(newContact);
    if ((*p2contactNodeHead) == NULL)
        *p2contactNodeHead = newContactNode;

    else{
        for(;(*p2contactNodeHead)!= NULL && compFunc (newContact, (*p2contactNodeHead)->_data) > 0 ;
             p2contactNodeHead = &((*p2contactNodeHead)->_next));
        newContactNode->_next = (*p2contactNodeHead);
        (*p2contactNodeHead) = newContactNode;
    }

}
void clearContactsLists(struct ContactNodeHeads* myContacts){
    clearContacts(myContacts->_sortCellHead);
    clearContactsNodes(&(myContacts->_sortCellHead));
    clearContactsNodes(&(myContacts->_sortTelHead));
    clearContactsNodes(&(myContacts->_sortFnameHead));
    clearContactsNodes(&(myContacts->_sortLnameHead));
}
void clearContacts(struct ContactNode *contactNodeHead){
    for (struct ContactNode* tempHead = contactNodeHead; tempHead; tempHead = tempHead->_next){
        free(tempHead->_data);
        tempHead->_data = NULL;
    }

}
void clearContactsNodes(struct ContactNode **p2contactNodeHead){
    struct ContactNode * tempCNode = NULL;
    for(tempCNode = *p2contactNodeHead; (*p2contactNodeHead) != NULL;
        p2contactNodeHead = &tempCNode) {
        tempCNode = ((*p2contactNodeHead)->_next);
        //(*p2contactNodeHead)->_data = NULL;
        free(*p2contactNodeHead);
        *p2contactNodeHead = NULL;
    }
}

void delete(struct ContactNodeHeads* myContacts){
    char dfname_string[MAX_FIRST_LAST_SIZE], dlname_string[MAX_FIRST_LAST_SIZE];
    char ch;
    struct ContactNode* tempHead = NULL;
    register int i = 1, find = 0;
    clrscr();
    printf("\nPhone Book12<::>Delete Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf ("\n::Enter data of the contact that you want delete it,please:");
    printf("\n\n  ::Enter first name: ");
    getline_and_copy_string(dfname_string, MAX_FIRST_LAST_SIZE);

    printf("\n  ::Enter last name: ");
    getline_and_copy_string(dlname_string, MAX_FIRST_LAST_SIZE);

    for (tempHead = myContacts->_sortFnameHead; tempHead ; tempHead = tempHead->_next, i++) {
        if (strcmp(dfname_string, tempHead->_data->fname) == 0 &&
            strcmp(dlname_string, tempHead->_data->lname) == 0) {

            printf("\nContact was found! Details:");
            printf("\n\nCantact %2.2d{", i + 1);
            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
                   tempHead->_data->fname, tempHead->_data->lname, tempHead->_data->telep, tempHead->_data->cellp);
            printf("\n\nAre you sure you want to delete this contact?(y/n)");
            ch = get_char_only();
            if (ch == 'y') {
                deleteContact(myContacts, tempHead->_data);
                myContacts->_contactsNo--;
                find = 1;
                break;
            }
        }
    }
    if (find==0)
        printf("\t\t\n<<This contact does not exist in this list or program can not delete it.>>");
    else
        printf("\t\t\n<<Target contact was successfully deleted from list!>>");
    printf("\n\n\nPress a key to return main page & continue program|-->");
    get_char_only();

}

void deleteContact(struct ContactNodeHeads* myContacts, struct Contact* delContact){
    deleteContactNodesFromList(&(myContacts->_sortCellHead), delContact);
    deleteContactNodesFromList(&(myContacts->_sortTelHead), delContact);
    deleteContactNodesFromList(&(myContacts->_sortFnameHead), delContact);
    deleteContactNodesFromList(&(myContacts->_sortLnameHead), delContact);
    free(delContact);

}
void deleteContactNodesFromList(struct ContactNode **p2contactNodeHead, struct Contact* delContact){
    struct ContactNode* tempNode = NULL;
    for (; *p2contactNodeHead; p2contactNodeHead = &((*p2contactNodeHead)->_next)){
        if ((*p2contactNodeHead)->_data == delContact){
            tempNode = (*p2contactNodeHead);
            (*p2contactNodeHead) = (*p2contactNodeHead)->_next;
            free(tempNode);
            break;
        }
    }
}

void edit(struct ContactNodeHeads* myContacts){
    char dfname[MAX_FIRST_LAST_SIZE] ,dlname[MAX_FIRST_LAST_SIZE];
    register int i, find=0;
    char ch;
    struct ContactNode* tempHead = NULL;
    struct Contact * newContact = NULL;
    clrscr();
    printf("\nPhone Book12<::>Edit Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf ("\n::Enter data of the contact that you want edit it,please:");

    printf("\n\n  ::Enter first name: ");
    getline_and_copy_string(dfname, MAX_FIRST_LAST_SIZE);
    printf("\n  ::Enter last name: ");
    getline_and_copy_string(dlname, MAX_FIRST_LAST_SIZE);


    for (tempHead = myContacts->_sortFnameHead; tempHead ; tempHead = tempHead->_next, i++) {
        if (strcmp(dfname, tempHead->_data->fname) == 0 &&
            strcmp(dlname, tempHead->_data->lname) == 0) {
            printf("\nContact found! Details:");
            printf("\n\nCantact %2.2d{",i+1);
            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",
                   tempHead->_data->fname, tempHead->_data->lname, tempHead->_data->telep, tempHead->_data->cellp);
            printf("\n\nDo you want edit it?(y/n) ");
            ch = get_char_only();
            if(ch=='y'){
                deleteContact(myContacts, tempHead->_data);
                newContact = createContact();
                addToLinkedLists(myContacts, newContact);
                find=1;
                break;
            }
        }
    }
    if (find==0)
        printf("\t\t\n<<This contact does not exist or you chose not to Edit it.>>");
    else
        printf("\t\t\n<<Target contact was successfully updated!>>");
    printf("\n\n\n   ::Press a key to return main page & continue program|-->");
    get_char_only();
}


//void delet(void)
//{
//
//
//    for (i = 0; i < last; i++) {
//        if (strcmp (dfname_string, A[i].fname) == 0 && strcmp (dlname_string, A[i].lname) == 0 ) {
//
//            printf("\nContact was found! Details:");
//            printf("\n\nCantact %2.2d{",i+1);
//            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",A[i].fname,A[i].lname,A[i].telep,A[i].cellp);
//            printf("\n\nAre you sure you want to delete this contact?(y/n)");
//            ch = get_char_only();
//            if(ch == 'y'){
//                for(j = i ; j + 1 < last ; j++)
//                    A[j] = A[j+1];
//
//                last--;
//                find = 1;
//                break;
//            }
//        }
//    }
//    if (find==0)
//        printf("\t\t\n<<This contact does not exist in this list or program can not delete it.>>");
//    else
//        printf("\t\t\n<<Target contact was successfully deleted from list!>>");
//    printf("\n\n\nPress a key to return main page & continue program|-->");
//    get_char_only();
//}

//void edit()
//{
//    char input[MAX_FIRST_LAST_SIZE];
//    char dfname[MAX_FIRST_LAST_SIZE] ,dlname[MAX_FIRST_LAST_SIZE];
//    size_t len=0;
//    size_t len_size=0;
//    register int i,j,find=0;
//    char ch;
//    clrscr();
//    printf("\nPhone Book12<::>Edit Contacts");
//    printf("\n--------------------------------------------------------------------------------");
//    printf ("\n::Enter data of the contact that you want edit it,please:");
//
//    printf("\n\n  ::Enter first name: ");
//    getline_and_copy_string(dfname, MAX_FIRST_LAST_SIZE);
//    printf("\n  ::Enter last name: ");
//    getline_and_copy_string(dlname, MAX_FIRST_LAST_SIZE);
//
//
//    for (i=0; i < last; i++) {
//        if (strcmp (dfname, A[i].fname) == 0 && strcmp (dlname, A[i].lname) == 0 ) {
//            printf("\nContact found! Details:");
//            printf("\n\nCantact %2.2d{",i+1);
//            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",A[i].fname,A[i].lname,A[i].telep,A[i].cellp);
//            printf("\n\nDo you want edit it?(y/n) ");
//            ch = get_char_only();
//            if(ch=='y'){
//                printf("\n::Enter NEW data for this contact...");
//                printf("\n >|Enter new first name: ");
//                getline_and_copy_string(A[i].fname, MAX_FIRST_LAST_SIZE);
//                printf(" >|Enter new last name: ");
//                getline_and_copy_string(A[i].lname, MAX_FIRST_LAST_SIZE);
//                printf(" >|Enter new telephone number: ");
//                getline_and_copy_string(A[i].telep, MAX_NUMBER_LEN);
//
//                printf(" >|Enter new cellphone number: ");
//
//                getline_and_copy_string(A[i].cellp, MAX_NUMBER_LEN);
//
//                find=1;
//                break;
//            }
//        }
//    }
//    if (find==0)
//        printf("\t\t\n<<This contact does not exist or you chose not to Edit it.>>");
//    else
//        printf("\t\t\n<<Target contact was successfully updated!>>");
//    printf("\n\n\n   ::Press a key to return main page & continue program|-->");
//    get_char_only();
//}

void search(const struct ContactNodeHeads* myContacts){
    char ch;
    clrscr();
    printf("\nPhone Book12<::>$Search Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\nChoose search type,please:\n\n");
    printf("\t[1] |--> Search by first name\n");
    printf("\t[2] |--> Search by last name\n");
    printf("\t[3] |--> Search by phone number\n");
    printf("\t[4] |--> Search by cellphone number\n");
    printf("\t[5] |--> Main Menu\n");
    printf("\n\t::Enter a number (1-5): ");
    ch = get_char_only();
    printf("\n--------------------------------------------------------------------------------");
    while(true) {
        switch (ch) {
            case '1':
                searchf(myContacts->_sortFnameHead);
                break;
            case '2':
                searchl(myContacts->_sortLnameHead);
                break;
            case '3':
                searchp(myContacts->_sortTelHead);
                break;
            case '4':
                searchc(myContacts->_sortCellHead);
                break;
            case '5':
            default:
                return;

        }
        printf("\nPress a key to search another contact.");
        get_char_only();
    }
}

void searchf(const struct ContactNode * contactNodeHead)
{
    char fname[MAX_FIRST_LAST_SIZE];
    register int i = 0,find=0;
    struct ContactNode* tempCNode = NULL;
    printf("Enter a first name to search:");
    getline_and_copy_string(fname, MAX_FIRST_LAST_SIZE);
    for (tempCNode =(struct ContactNode*) contactNodeHead; tempCNode; tempCNode = tempCNode->_next)
        if(strcmp(fname ,tempCNode->_data->fname) == 0) {
            find=1;
            break;
        }
    if(find==0)
        printf("\t\n<<Not Find!There is no contact with this name in phone book.>>");
    else {
        printf("\t\n<<Target contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",A[i].fname,A[i].lname,A[i].telep,A[i].cellp);
    }

}

void searchl(const struct ContactNode * contactNodeHead)
{
    char lname[MAX_FIRST_LAST_SIZE];
    register int i,find=0;
    struct ContactNode* tempCNode = NULL;
    printf("\n::Enter a last name to search:");
    getline_and_copy_string(lname, MAX_FIRST_LAST_SIZE);

    for (tempCNode =(struct ContactNode*) contactNodeHead; tempCNode; tempCNode = tempCNode->_next)
        if(strcmp(lname ,tempCNode->_data->lname) == 0) {
            find=1;
            break;
        }
    if(find==0)
        printf("\t\n<<Not Find!There is not contact with this name in phone book.>>");
    else {
        printf("\t\n<<Target contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",A[i].fname,A[i].lname,A[i].telep,A[i].cellp);
    }

}

void searchp(const struct ContactNode * contactNodeHead)
{

    char phone [MAX_NUMBER_LEN];
    int i,find=0;
    struct ContactNode* tempCNode = NULL;
    printf("\n::Enter a phone number to search:");
    getline_and_copy_string(phone, MAX_FIRST_LAST_SIZE);
    for (tempCNode =(struct ContactNode*) contactNodeHead; tempCNode; tempCNode = tempCNode->_next)
        if(strcmp(phone ,tempCNode->_data->telep) == 0) {
            find=1;
            break;
        }
    if(find==0)
        printf("\t\n<<Not Found!There is not contact with this phone number  in phone book.>>");
    else{
        printf("\t\n<<Target contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",A[i].fname,A[i].lname,A[i].telep,A[i].cellp);
    }
}

void searchc(const struct ContactNode * contactNodeHead)
{

    char phone[MAX_NUMBER_LEN];
    int i,find=0;
    struct ContactNode* tempCNode = NULL;
    printf("\n::Enter a cellphone number to search:");
    getline_and_copy_string(phone, MAX_FIRST_LAST_SIZE);
    for (tempCNode =(struct ContactNode*) contactNodeHead; tempCNode; tempCNode = tempCNode->_next)
        if(strcmp(phone ,tempCNode->_data->cellp) == 0) {
            find=1;
            break;
        }
    if(find==0)
        printf("\t\n<<Not Find!There is not contact with this cellphone number in phone book.>>");
    else {
        printf("\t\n<<Target contact found! Details:>>");
        printf("\n\nCantact %2.2d{",i+1);
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }",A[i].fname,A[i].lname,A[i].telep,A[i].cellp);
    }
}

void showSort(const struct ContactNodeHeads* myContacts){
    char ch;
    clrscr();
    printf("\nPhone Book12<::>$earch Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\nChoose sort type,please:\n\n");
    printf("\t[1] |--> Sort by first name\n");
    printf("\t[2] |--> Sort by last name\n");
    printf("\t[3] |--> Sort by phone number\n");
    printf("\t[4] |--> Sort by cellphone number\n");
    printf("\t[5] |--> Main Menu\n");
    printf("\n\t::Enter a number (1-5): ");
    ch = get_char_only();
    printf("\n--------------------------------------------------------------------------------");
    switch(ch){
        case '1':
            showContactList(myContacts->_sortFnameHead);
            break;
        case '2':
            showContactList(myContacts->_sortLnameHead);
            break;
        case '3':
            showContactList(myContacts->_sortTelHead);
            break;
        case '4':
            showContactList(myContacts->_sortCellHead);
        case '5':
        default:
            return ;
    }
}
void showContactList(const struct ContactNode * contactNodeHead){
    clrscr();
    int y_offset = 6;
    printf("\nPhone Book12<::>All Contacts List");
    printf("\n--------------------------------------------------------------------------------");
    print_titles(y_offset);
    print_list( y_offset + 3 , contactNodeHead);
    printf("\n\n\n   ::Press a key to return main page & continue program.|-->");
    get_char_only();
}
//void sortf(void)
//{
//    struct Contact B;
//    register int i,j;
//    for(i= last-1 ;i > 0; i--)
//        for(j = 0; j < i; j++)
//            if(strcmp(A[j].fname, A[j+1].fname) > 0) {
//                B = A[j];
//                A[j] = A[j+1];
//                A[j+1] = B;
//            }
//    printf("\nplease wait... .Contacts will be sorted by first names.");
//    list(18);
//    printf("\n   ::Press any key to sort contact by another form... ");
//    get_char_only();
//    sort();
//}

//void sortl(void)
//{
//    struct Contact B;
//    register int i,j;
//    for(i = last-1 ; i > 0; i--)
//        for(j = 0; j < i ; j++)
//            if(strcmp(A[j].lname,A[j+1].lname) > 0) {
//                B = A[j];
//                A[j] = A[j+1];
//                A[j+1] = B;
//            }
//    printf("\nplease wait... .Contacts will be sorted by last names.");
//    list(18);
//    printf("\n   ::Press any key to sort contact by another form... ");
//    get_char_only();
//    sort();
//}
//void sortp(void)
//{
//    struct Contact B;
//    register int i,j;
//    for(i = last-1; i > 0; i--)
//        for(j = 0; j < i; j++)
//            if(strcmp(A[j].telep,A[j+1].telep) > 0) {
//                B = A[j];
//                A[j] = A[j+1];
//                A[j+1] = B;
//            }
//    printf("\nplease wait... .Contacts will be sorted by telephone numbers.");
//    list(18);
//    printf("\n   ::Press any key to sort contact by another form... ");
//    get_char_only();
//    sort();
//}
//void sortc(void)
//{
//    struct Contact B;
//    register int i,j;
//    for(i = last-1; i > 0; i--)
//        for(j = 0; j < i; j++)
//            if(strcmp(A[j].cellp,A[j+1].cellp) > 0) {
//                B = A[j];
//                A[j] = A[j+1];
//                A[j+1] = B;
//            }
//    printf("\nPlease wait... .Contacts will be sort by cellphone numbers.");
//    list(18);
//    printf("\n   ::Press any key to sort contact by another form... ");
//    get_char_only();
//    sort();
//}

//void list(int y_offset)
//{
//    register int i;
//    clrscr();
//    printf("\nPhone Book12<::>All Contacts List");
//    printf("\n--------------------------------------------------------------------------------");
//    gotoxy(1,4);
//    printf("Row");
//    gotoxy(9,4);
//    printf("First Name");
//    gotoxy(27,4);
//    printf("Last Name");
//    gotoxy(44,4);
//    printf("Telephone");
//    gotoxy(60,4);
//    printf("Cellphone");
//    printf("\n--------------------------------------------------------------------------------");
//    for(i = 0; i <= last; i++) {
//        gotoxy(1,i+6);
//        printf("%3.3d",i+1);
//        gotoxy(9,i+6);
//        printf("%s",A[i].fname);
//        gotoxy(28,i+6);
//        printf("%s",A[i].lname);
//        gotoxy(44,i+6);
//        printf("%s",A[i].telep);
//        gotoxy(60,i+6);
//        printf("%s",A[i].cellp);
//    }
//
//    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
//    printf("\n\t\t    **********End Of list!**********");
//    printf("\n\n\n   ::Press a key to return main page & continue program.|-->");
//    get_char_only();
//}

//void list(int y_offset)
//{
//    printf("\nPhone Book12<::>All Contacts List");
//    printf("\n--------------------------------------------------------------------------------");
//    print_titles(y_offset);
//    print_list( y_offset + 3 );
//    printf("\n\n\n   ::Press a key to return main page & continue program.|-->");
//    get_char_only();
//
//}

void help(void)
{
    clrscr();
    gotoxy(26,1);
    printf("--------------------------");
    gotoxy(26,2);
    printf("|Welcome To Phone Book12!|");
    gotoxy(26,3);
    printf("--------------------------");
    gotoxy(1,5);
    printf("PhoneBook12>>[Help & Instructions] \n");
    printf("--------------------------------------------------------------------------------\n");
    printf("\nAbout Phonebook12\n *Version: 2.5\n *Valence: 50 Contacts(MAX)\n  *By: Morteza Zakeri (2011-12)");
    printf("\n\nThis program allows you to have a list of your contacts.\nIt also allows you to have your listing added or deleted from your list.\nOther features of this program include:\nSearch,edit,sort & list.\n\n   ::Press a key to return main page & continue program.|-->");
    get_char_only();
}

void clrscr()
{
    system("clear");
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

//-----------------------------------------------------------------------------
char * get_and_check_line(){
    char * string = NULL;
    size_t len = 0;
    size_t temp_len = getline(&string, &len, stdin);
    if (temp_len == FAIL) {
        perror("Failed in get line from user");
        exit (EXIT_FAILURE);
    }
    return string;

}
//-----------------------------------------------------------------------------
void getline_and_copy_string(char* dest, size_t max_len){
/*

 */
    memset(dest, '\0', max_len);
    size_t len = 0;
    char *string = get_and_check_line();
    dest = strncpy(dest, string, max_len-1);
    dest[max_len-1] = '\0';
    free(string);
    string = NULL;

}

char get_char_only(){
    char ch = (char)getc(stdin);
    if (ch != '\n') {
        char *string = get_and_check_line();
        free(string);
    }
    return ch;
}

void print_titles(int y_offset){
    int x_offset = 1;
    printf("\n--------------------------------------------------------------------------------");
    gotoxy(x_offset,y_offset);
    printf("Row");
    x_offset += ROW_TITLE_OFFSET;
    gotoxy(x_offset,y_offset);
    printf("First Name");
    x_offset+=MAX_FIRST_LAST_SIZE;
    gotoxy(x_offset,y_offset);
    printf("Last Name");
    x_offset+=MAX_FIRST_LAST_SIZE;
    gotoxy(x_offset,y_offset);
    printf("Telephone");
    x_offset += MAX_NUMBER_LEN;
    gotoxy(x_offset,y_offset);
    printf("Cellphone");
    printf("\n--------------------------------------------------------------------------------");
}

void print_list(int y_offset, const struct ContactNode * contactNodeHead){
     register int i;
    int x_offset = 1;
    struct ContactNode* tempCNode = NULL;
    for (tempCNode = (struct ContactNode*) contactNodeHead; tempCNode;
         tempCNode = tempCNode->_next, y_offset++, i++, x_offset = 1){
        gotoxy(x_offset,y_offset);
        printf("%3.3d",i+1);
        x_offset += ROW_TITLE_OFFSET;
        gotoxy(x_offset,y_offset);
        printf("%s",tempCNode->_data->fname);
        x_offset += MAX_FIRST_LAST_SIZE;
        gotoxy(x_offset,y_offset);
        printf("%s",tempCNode->_data->lname);
        x_offset += MAX_FIRST_LAST_SIZE;
        gotoxy(x_offset,y_offset);
        printf("%s",tempCNode->_data->telep);
        x_offset+=MAX_NUMBER_LEN;
        gotoxy(x_offset,y_offset);
        printf("%s",tempCNode->_data->cellp);
    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t    **********End Of list!**********");
}

void getCursor(int* x, int* y) {
    /*
     * https://thoughtsordiscoveries.wordpress.com/2017/04/26/set-and-read-cursor-position-in-terminal-windows-and-linux/
     */
    printf("\033[6n");  /* This escape sequence !writes! the current
                          coordinates to the terminal.
                          We then have to read it from there, see [4,5].
                          Needs <termios.h>,<unistd.h> and some others */
    scanf("\033[%d;%dR", x, y);
}